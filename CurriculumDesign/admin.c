#include "admin.h"
#include "goods.h"
#include "purchase.h"
#include "customer.h"
#include "stdlib.h"
#include "system.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

extern AllGoodList all_good_list;
extern PurchaseList purchase_record;
extern CustomerList customer_list;

static int good_id_counter = 1;     // 用于自动生成商品编号，初始值为1
static int purchase_id_counter = 1; // 用于自动生成进货记录编号，初始值为1
void ResetGoodPurchaseId()
{
    good_id_counter = 1;
    purchase_id_counter = 1;
}

// 函数声明

/// 进货函数
// 即增加商品信息
void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{

    static int first_time = 1; // 用于判断是否第一次调用Purchase函数，如果是第一次调用，则清空purchaseInfo.txt文件
    if (first_time)
    {
        FILE *fp = fopen("txt/purchaseInfo.txt", "w");
        if (fp == NULL)
        {
            printf("文件打开失败！\n");
            exit(1);
        }
        fclose(fp);
        first_time = 0; // 设置为0，表示已经清空过文件
    }

    char current_time[10];
    GetCurrentTime(current_time);

    // 1. 创建新的进货记录
    PurchaseNode purchase;
    strcpy(purchase.good_name, good_name);
    strcpy(purchase.purchase_time, current_time); // 修改为使用purchase_time
    strcpy(purchase.supplier, supplier);
    strcpy(purchase.category, category);
    purchase.id = purchase_id_counter++;
    purchase.quantity = quantity;
    purchase.cost_price = cost_price;
    Insert_PurchaseList(&purchase_record, purchase);

    // 2. 更新商品库存
    // 查找商品是否已经存在，如果存在则更新库存，否则创建新的商品节点
    GoodNode *p = all_good_list->next;
    while (p != NULL)
    {
        if (strcmp(p->good_name, good_name) == 0 && strcmp(p->category, category) == 0 && strcmp(p->manufacturer, supplier) == 0 && p->price == cost_price)
        {
            p->stock += quantity; // 更新库存
            break;
        }
        p = p->next;
    }
    if (p == NULL) // 如果商品不存在，则创建新的商品节点
    {
        GoodNode new_good;
        strcpy(new_good.good_name, good_name);
        strcpy(new_good.category, category);
        strcpy(new_good.manufacturer, supplier);
        new_good.price = cost_price;
        new_good.stock = quantity;
        new_good.is_promotion = 0; // 默认不促销
        new_good.promotion_price = 0;
        new_good.id = good_id_counter++; // 分配商品编号
        Insert_AllGoodList(&all_good_list, new_good);
    }
    // 3. 保存进货日志到purchaseInfo.txt，格式：商品类别 商品名称 进货单价 进货数量 实际时间 供应商名称
    // 获取当前时间并格式化为字符串

    FILE *fp = fopen("txt/purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }
    // 格式：商品id 商品类别 商品名称 进货单价 进货数量 实际时间 供应商名称
    fprintf(fp, "%d %s %s %.2f %d %s %s\n", purchase.id, purchase.category, purchase.good_name,
            purchase.cost_price, purchase.quantity, current_time, purchase.supplier);
    fclose(fp);
}

// 删除指定商品的进货记录
// 这里的删除是指从txt文件中中删除指定商品名称的进货记录，而不是删除商品本身，也不是从链表中删除
void Delete_PurchaseRecord(int id)
{
    FILE *fp = fopen("txt/purchaseInfo.txt", "r+");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }

    FILE *temp_fp = fopen("txt/temp.txt", "w");
    if (temp_fp == NULL)
    {
        perror("临时文件打开失败");
        fclose(fp);
        return;
    }

    char line[256];
    int found = 0; // 标记是否找到要删除的商品编号
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int current_id;
        // 从行中解析出第一个整数作为商品编号
        if (sscanf(line, "%d", &current_id) == 1 && current_id == id)
        {
            found = 1; // 找到要删除的商品编号，跳过该行
            continue;
        }
        // 如果当前行不包含要删除的商品编号，则写入临时文件
        fputs(line, temp_fp);
    }

    fclose(fp);
    fclose(temp_fp);

    // 如果找到要删除的商品编号，则替换原文件
    if (found)
    {
        remove("txt/purchaseInfo.txt");                 // 删除原文件
        rename("txt/temp.txt", "txt/purchaseInfo.txt"); // 重命名临时文件为原文件名
    }
    else
    {
        remove("txt/temp.txt"); // 删除临时文件
        printf("未找到该商品编号的进货记录！\n");
    }
}

// 增加一条商品的进货记录
// 这里的增加是指从txt文件中增加进货记录，而不是增加商品本身，id为0，暴露假账
// 格式：商品id 商品类别 商品名称 进货单价 进货数量 实际时间 供应商名称
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{
    FILE *fp = fopen("txt/purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }

    char current_time[9];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    snprintf(current_time, sizeof(current_time), "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);

    // 商品进货信息包括：商品编号、商品名称、进货时间、进货价格、进货数量、进货来源。
    // 这里假设 id 为 0 表示特殊情况（如注释中提到的暴露假账）
    fprintf(fp, "0 %s %s %.2f %d %s\n", good_name, current_time, cost_price, quantity, supplier);
    fclose(fp);
}

// 输入顾客信息
void CustomerInfo_Generate()
{
    char name[50], type[20], contact[50]; // 使用数组代替未初始化的指针

    // 循环输入顾客信息，输入'#'结束输入
    while (1)
    {
        CustomerNode customer;
        printf("请输入顾客姓名（输入'#'结束）：");
        scanf("%49s", name); // 限制输入长度，避免缓冲区溢出
        if (strcmp(name, "#") == 0)
        {
            break; // 输入'#'结束循环
        }
        while (1)
        {
            printf("请输入顾客类型（normal vip svip）：");
            scanf("%19s", type); // 限制输入长度
            if (strcmp(type, "normal") == 0 || strcmp(type, "vip") == 0 || strcmp(type, "svip") == 0)
            {
                break; // 输入合法，退出循环
            }
            else
            {
                printf("输入的顾客类型无效，请重新输入！\n");
            }
        }
        printf("请输入顾客联系方式：");
        scanf("%49s", contact); // 限制输入长度
        printf("顾客信息已添加到系统！\n\n");
        strcpy(customer.name, name);
        strcpy(customer.type, type);
        strcpy(customer.contact, contact);
        Insert_CustomerList(&customer_list, customer);
    }
    // 生成顾客信息文件
    Create_CustomerInfo_File();
}

void Create_CustomerInfo_File()
{
    // 先清空文件
    FILE *fp = fopen("txt/customerInfo.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        exit(1);
    }
    fclose(fp);
    // 按照customer_list的顺序写入文件
    CustomerNode *p = customer_list->next;
    FILE *fp1 = fopen("txt/customerInfo.txt", "a");
    if (fp1 == NULL)
    {
        printf("文件打开失败！\n");
        exit(1);
    }
    while (p != NULL)
    {
        fprintf(fp1, "%d %s %s %s\n", p->id, p->name, p->type, p->contact);
        p = p->next;
    }
    fclose(fp1);
    printf("顾客信息已更新\n");
}

void Add_Customer_To_System()
{
    system("cls");
    printf("请输入添加的顾客信息：\n");
    CustomerInfo_Generate();
}

void Delete_Customer_From_System()
{

    while (1)
    {

        Print_CustomerList(customer_list);
        printf("已存储的顾客信息如上，供参考\n");
        int id;
        printf("请输入要删除的顾客编号（输入'#'退出删除）：");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // 退出删除模式
        }
        id = atoi(input); // 将输入转换为整数
        if (Delete_CustomerList(&customer_list, id) == 1)
        {
            printf("顾客信息已删除！\n\n");
        }
        else
            printf("请重新输入\n\n");
    }
    Create_CustomerInfo_File();
}

void Adjust_Customer_Type()
{

    while (1)
    {
        int id;
        char new_type[30];
        printf("请输入要修改的顾客id（输入'#'退出调整）：");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // 退出调整模式
        }
        id = atoi(input); // 将输入转换为整数
        CustomerNode *p = customer_list->next;
        while (p != NULL && p->id != id)
        {
            p = p->next;
        }
        if (p == NULL)
        {
            printf("未找到该顾客！\n");
            continue;
        }

        printf("请输入新的顾客类型（normal vip svip）：");
        scanf("%s", new_type);
        strcpy(p->type, new_type);

        printf("顾客类型已修改！\n\n");
    }
    Create_CustomerInfo_File();
}

// 从all_good_list中删除商品信息
// ... existing code ...

// 从all_good_list中删除商品信息
void Delete_Good_From_System()
{
    while (1)
    {
        system("cls");
        char input[100];
        int id;
        Print_AllGoodList(all_good_list);
        printf("最新的所有商品信息如上所示，供参考：\n");
        printf("请输入要删除的商品ID（输入 '#' 退出）：");
        scanf("%s", input);
        // 清理输入缓冲区
        while (getchar() != '\n')
            ;

        if (strcmp(input, "#") == 0)
        {
            break;
        }

        id = atoi(input);
        if (id == 0 && strcmp(input, "0") != 0)
        {
            printf("输入的商品ID无效，请重新输入。\n");
            continue;
        }

        // 调用 Delete_GoodList 函数删除商品
        if (Delete_AllGoodList(&all_good_list, id))
        {
            printf("商品ID为 %d 的商品已成功删除。\n", id);
        }
        else
        {
            printf("未找到商品ID为 %d 的商品，请重新输入。\n", id);
        }
        printf("按回车键继续...");
        while (getchar()!= '\n')
            ;
        getchar();
    }
}

// 管理商品售价
void Set_Good_Price()
{
    while (1)
    {
        char input[100];
        int id;
        float new_price;

        printf("请输入要修改售价的商品ID（输入 '#' 退出）：");
        scanf("%s", input);
        // 清理输入缓冲区
        while (getchar() != '\n')
            ;

        if (strcmp(input, "#") == 0)
        {
            break;
        }

        id = atoi(input);
        if (id == 0 && strcmp(input, "0") != 0)
        {
            printf("输入的商品ID无效，请重新输入。\n");
            continue;
        }

        printf("请输入新的商品售价：");
        if (scanf("%f", &new_price) != 1)
        {
            printf("输入的售价无效，请重新输入。\n");
            // 清理输入缓冲区
            while (getchar() != '\n')
                ;
            continue;
        }
        // 清理输入缓冲区
        while (getchar() != '\n')
            ;

        // 查找商品并更新售价
        GoodNode *p = all_good_list->next;
        while (p != NULL)
        {
            if (p->id == id)
            {
                p->price = new_price;
                printf("商品ID为 %d 的商品售价已更新为 %.2f。\n", id, new_price);
                break;
            }
            p = p->next;
        }
        if (p == NULL)
        {
            printf("未找到商品ID为 %d 的商品，请重新输入。\n", id);
        }
    }
}

// 管理商品促销信息
void Set_Good_Promotion_Info()
{
    // 先显示所有商品信息
    Print_AllGoodList(all_good_list);
    printf("已有的所有商品信息如上所示，供参考：\n");
    // 循环处理，输入#退出
    while (1)
    {
        int id;
        printf("请输入要设置促销信息的商品id（输入'#'退出）：");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // 退出设置促销信息模式
        }
        id = atoi(input); // 将输入转换为整数
        // 找到id对应的商品
        GoodNode *p = all_good_list->next;
        while (p != NULL && p->id != id)
        {
            p = p->next;
        }
        if (p == NULL)
        {
            printf("未找到该商品！\n");
            continue;
        }
        // 找到商品后，设置促销信息
        char promotion_info[20];
        printf("请输入是否促销，1设置为促销，0设置为不促销：");
        int is_promotion;
        scanf("%d", &is_promotion);
        if (is_promotion == 1)
        {
            p->is_promotion = 1; // 设置为促销
            printf("请输入促销比例（例如输入0.8表示打8折）：");
            float promotion_rate;
            scanf("%f", &promotion_rate);
            p->promotion_price = p->price * promotion_rate;       // 计算促销价格
            sprintf(promotion_info, "%.2f", promotion_rate * 10); // 保留一位小数
            strcat(promotion_info, "折");                         // 添加折号
        }
        else
        {
            p->is_promotion = 0;              // 设置为不促销
            p->promotion_price = 0.0;         // 促销价格为0
            strcpy(promotion_info, "不促销"); // 设置促销信息为不促销
        }
        printf("%s促销信息已设置为：%s\n\n", p->good_name, promotion_info); // 输出促销信息
    }
}

// 将不同的商品类别存储到一个数组中，相同的类别只存储一次
void Get_Good_Category(char category[][100])
{
    // 初始化 category 数组为空字符串
    for (int i = 0; i < 100; i++)
    {
        category[i][0] = '\0';
    }

    GoodNode *p = all_good_list->next; // 指向头结点的下一个节点
    if (p == NULL)
    { // 如果链表为空
        printf("商品类别信息为空！\n");
        return; // 退出函数
    }
    printf("商品类别信息如下：\n");
    int count = 0; // 记录已有的类别数量
    while (p != NULL)
    { // 遍历链表，一行一个商品节点
        int i;
        for (i = 0; i < count; i++)
            if (strcmp(category[i], p->category) == 0)
                break;
        if (i == count)
        {                                         // 如果没有找到相同的类别
            strcpy(category[count], p->category); // 复制类别信息到数组
            count++;                              // 类别数量加1
        }
        p = p->next; // 继续遍历下一个节点
    }
}

// 查看已有的商品类别，相同类别仅打印一次
// 每行打五个就换行
void View_Good_Category()
{
    char category[100][100]; // 存储不同的商品类别
    Get_Good_Category(category);
    int count = 0; // 记录已有的类别数量
    while (count < 100)
    { // 遍历数组，一行一个类别
        if (strcmp(category[count], "") == 0)
            break;                        // 如果数组为空，则退出循环
        printf("%-20s", category[count]); // 输出类别信息
        count++;                          // 类别数量加1
        if (count % 5 == 0)               // 如果已经输出了五个类别，则换行
            printf("\n");
    }
    printf("\n"); // 换行
    printf("按回车继续...\n");
    while (getchar()!= '\n');
    getchar();
}

/// 调整商品分类函数
/// 功能：将指定商品从当前分类移动到新的分类
void Adjust_Good_Category()
{
    while (1)
    {
        system("cls"); // 清屏
        char input[100];
        int id;
        char new_category[50];
        Print_AllGoodList(all_good_list);
        printf("已有的所有商品信息如上所示，供参考：\n\n");
        printf("请输入要调整分类的商品ID（输入 '#' 退出）：");
        scanf("%s", input);

        if (strcmp(input, "#") == 0)
        {
            break;
        }

        id = atoi(input);
        if (id == 0 && strcmp(input, "0") != 0)
        {
            printf("输入的商品ID无效，请重新输入。\n");
            continue;
        }

        printf("请输入新的商品分类：");
        scanf("%s", new_category);

        // 查找商品是否存在，如果存在则更新分类，否则提示商品不存在
        GoodNode *p = all_good_list->next;
        while (p != NULL)
        {
            if (p->id == id)
            {
                strcpy(p->category, new_category); // 更新分类
                printf("商品分类已更新为 %s。\n\n", new_category);
                printf("按回车继续...\n");
                while (getchar() != '\n')
                    ;      // 清空输入缓冲区
                getchar(); // 等待用户按回车
                break;
            }
            p = p->next;
        }
        if (p == NULL)
        { // 如果商品不存在，则提示商品不存在
            printf("商品不存在！请重新输入。\n\n");
        }
    }
}

// 删除所有的分类为category的商品
void Delete_Good_Category()
{
    while (1)
    {
        system("cls"); // 清屏
        View_Good_Category();
        printf("已有的所有商品类别如上所示，供参考：\n\n");
        printf("请输入要删除的商品类别（输入 '#' 退出）：");
        char category[50];
        scanf("%s", category);

        if (strcmp(category, "#") == 0)
        {
            break; // 退出循环
        }

        int found = 0;
        GoodNode *prev = all_good_list;
        GoodNode *p = all_good_list->next; // 指向头结点的下一个节点
        while (p != NULL)
        {
            if (strcmp(p->category, category) == 0)
            {
                prev->next = p->next;
                free(p);
                p = prev->next;
                found = 1;
            }
            else
            {
                prev = p;
                p = p->next;
            }
        }

        if (found)
        {
            printf("%s 类别商品删除成功\n", category);
        }
        else
        {
            printf("未找到 %s 类别\n", category);
        }

        printf("按回车继续...");
        while (getchar() != '\n')
            ;      // 清空输入缓冲区
        getchar(); // 等待用户按回车
    }
}
