#include "admin.h"
#include "goods.h"
#include "purchase.h"
#include "customer.h"
#include "stdlib.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

extern AllCategoryList all_category_list;
extern PurchaseList purchase_record;
extern CustomerList customer_list;

// 函数声明

void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Delete_PurchaseRecord(char *good_name);
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Adjust_Category(char *good_name, char *new_category);
void CustomerInfo_Generate();
void Add_Customer_To_System();
void Delete_Customer_From_System();
void Adjust_Customer_Type();


/// 进货函数
// 即增加商品信息
void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{

    static int first_time = 1; // 用于判断是否第一次调用Purchase函数，如果是第一次调用，则清空purchaseInfo.txt文件
    if (first_time)
    {
        FILE *fp = fopen("purchaseInfo.txt", "w");
        if (fp == NULL)
        {
            printf("文件打开失败！\n");
            exit(1);
        }
        fclose(fp);
        first_time = 0; // 设置为0，表示已经清空过文件
    }



    char current_time[9];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    snprintf(current_time, sizeof(current_time), "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);

    // 1. 创建新的进货记录
    PurchaseNode purchase;
    strcpy(purchase.good_name, good_name);
    strcpy(purchase.purchase_time, current_time); // 修改为使用purchase_time
    strcpy(purchase.supplier, supplier);
    strcpy(purchase.category, category);
    purchase.quantity = quantity;
    purchase.cost_price = cost_price;
    Insert_PurchaseList(&purchase_record, purchase);
    // 2. 更新商品库存
    // 查找all_category_list中是否有category
    AllCategoryNode *allListptr = all_category_list->next;
    while (allListptr != NULL && strcmp(allListptr->category, category) != 0)
    {
        allListptr = allListptr->next;
    }
    if (allListptr == NULL) // 未找到该类别，需要新建一个类别，头插法
    {
        CategoryList category_list;
        Init_CategoryList(&category_list);
        Insert_AllCategoryList(&all_category_list, category_list, category);
        allListptr = all_category_list->next; // 此时allListptr指向新建的类别
        // 为新建的类别插入商品
        Good good;
        strcpy(good.good_name, good_name);
        strcpy(good.category, category);
        strcpy(good.manufacturer, supplier);
        good.price = cost_price;
        good.stock = quantity;
        Insert_CategoryList(&category_list, good);
    }
    else // 找到该类别了，那么allListptr指向该类别
    {
        CategoryList category_list = allListptr->categorylist;
        CategoryNode *categoryListptr = category_list->next;
        while (categoryListptr != NULL && strcmp(categoryListptr->good.good_name, good_name) != 0)
        {
            categoryListptr = categoryListptr->next;
        }
        if (categoryListptr == NULL) // 未找到该商品，需要新建一个商品，头插法
        {
            Good good;
            strcpy(good.good_name, good_name);
            strcpy(good.category, category);
            strcpy(good.manufacturer, supplier);
            good.price = cost_price;
            good.stock = quantity;
            Insert_CategoryList(&category_list, good);
        }
        else // 找到该商品了，那么categoryListptr指向该商品
        {
            categoryListptr->good.stock += quantity;
        }
    }

    // 3. 保存进货日志到purchaseInfo.txt，格式：商品类别 商品名称 进货单价 进货数量 实际时间（例如13:05） 供应商名称
    // 获取当前时间并格式化为字符串

    FILE *fp = fopen("purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }
    fprintf(fp, "%s %s %.2f %d %s %s\n", category, good_name, cost_price, quantity, current_time, supplier);
    fclose(fp);
}

// 删除指定商品的进货记录
// 这里的删除是指从txt文件中中删除指定商品名称的进货记录，而不是删除商品本身，也不是从链表中删除
void Delete_PurchaseRecord(char *good_name)
{
    FILE *fp = fopen("purchaseInfo.txt", "r+");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }

    FILE *temp_fp = fopen("temp.txt", "w");
    if (temp_fp == NULL)
    {
        perror("临时文件打开失败");
        fclose(fp);
        return;
    }

    char line[256];
    int found = 0; // 标记是否找到要删除的商品名称
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, good_name) == NULL) // 如果当前行不包含要删除的商品名称，则写入临时文件
        {
            fputs(line, temp_fp);
        }
        else
        {
            found = 1; // 找到要删除的商品名称
        }
    }

    fclose(fp);
    fclose(temp_fp);

    // 如果找到要删除的商品名称，则替换原文件
    if (found)
    {
        remove("purchaseInfo.txt"); // 删除原文件
        rename("temp.txt", "purchaseInfo.txt"); // 重命名临时文件为原文件名
    }
    else
    {
        remove("temp.txt"); // 删除临时文件
        printf("未找到该商品名称的进货记录！\n");
    }
}

// 增加一条商品的进货记录
// 这里的增加是指从txt文件中增加进货记录，而不是添加到链表中，也不是添加商品本身，类似于做假账
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{
    FILE *fp = fopen("purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }

    char current_time[9];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    snprintf(current_time, sizeof(current_time), "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);

    fprintf(fp, "%s %s %.2f %d %s %s\n", category, good_name, cost_price, quantity, current_time, supplier);
    fclose(fp);
}

/// 调整商品分类函数
/// 功能：将指定商品从当前分类移动到新的分类
void Adjust_Category(char *good_name, char *new_category)
{
    AllCategoryNode *allListptr = all_category_list->next;
    AllCategoryNode *oldCategoryNode = NULL;
    CategoryNode *targetGoodNode = NULL;

    // 1. 遍历所有类别链表，查找商品所在的类别
    while (allListptr != NULL)
    {
        CategoryList category_list = allListptr->categorylist;
        CategoryNode *categoryListptr = category_list->next;

        while (categoryListptr != NULL)
        {
            if (strcmp(categoryListptr->good.good_name, good_name) == 0)
            {
                oldCategoryNode = allListptr; // 记录旧类别节点
                targetGoodNode = categoryListptr; // 记录目标商品节点
                break;
            }
            categoryListptr = categoryListptr->next;
        }

        if (targetGoodNode != NULL)
        {
            break; // 找到商品后退出循环
        }

        allListptr = allListptr->next;
    }

    // 2. 如果未找到商品，输出提示信息
    if (targetGoodNode == NULL)
    {
        printf("未找到名称为 %s 的商品！\n", good_name);
        return;
    }

    // 3. 从旧类别链表中删除该商品节点
    Delete_CategoryList(&oldCategoryNode->categorylist, good_name);

    // 检查旧类别是否为空，如果为空则删除该类别
    if (oldCategoryNode->categorylist->next == NULL)
    {
        Delete_AllCategoryList(&all_category_list, oldCategoryNode->category);
    }

    // 4. 检查新类别是否存在
    AllCategoryNode *newCategoryNode = all_category_list->next;
    while (newCategoryNode != NULL && strcmp(newCategoryNode->category, new_category) != 0)
    {
        newCategoryNode = newCategoryNode->next;
    }

    // 5. 如果新类别不存在，则创建新类别并插入商品
    if (newCategoryNode == NULL)
    {
        CategoryList new_category_list;
        Init_CategoryList(&new_category_list);
        Insert_AllCategoryList(&all_category_list, new_category_list, new_category);
        newCategoryNode = all_category_list->next; // 新类别节点
    }

    // 6. 修改商品的类别信息并插入新类别链表
    Good updatedGood = targetGoodNode->good;
    strcpy(updatedGood.category, new_category);
    strcpy(updatedGood.good_name, good_name); // 更新商品的类别信息
    Insert_CategoryList(&newCategoryNode->categorylist, updatedGood);
    
}

// 输入顾客信息并生成文件
void CustomerInfo_Generate()
{
    char name[50], type[20], contact[50]; // 使用数组代替未初始化的指针
    // 清空customerInfo.txt文件
    FILE *fp = fopen("customerInfo.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        exit(1);
    }
    fclose(fp);
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
        printf("请输入顾客类型（NORMAL VIP SVIP）：");
        scanf("%19s", type); // 限制输入长度
        printf("请输入顾客联系方式：");
        scanf("%49s", contact); // 限制输入长度
        strcpy(customer.name, name);
        strcpy(customer.type, type);
        strcpy(customer.contact, contact);
        Insert_CustomerList(&customer_list, customer);
        // 将顾客信息写入文件
        FILE *fp1 = fopen("customerInfo.txt", "a");
        if (fp1 == NULL)
        {
            printf("文件打开失败！\n");
            exit(1);
        }
        fprintf(fp1, "%s %s %s\n", customer.name, customer.type, customer.contact);
        fclose(fp1);
    }
}

void Add_Customer_To_System()
{
    printf("请输入添加的顾客信息：\n");
    CustomerInfo_Generate();
    printf("顾客信息已添加到系统！\n");
}

void Delete_Customer_From_System()
{
    while (1)
    {
        printf("已存储的顾客信息：\n");
        Print_CustomerList(customer_list);
        int id;
        printf("请输入要删除的顾客编号（输入'#'退出删除）：");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // 退出删除模式
        }
        id = atoi(input); // 将输入转换为整数
        Delete_CustomerList(&customer_list, id);
        printf("顾客信息已删除！\n");
    }
}

void Adjust_Customer_Type()
{
    while (1)
    {
        printf("已存储的顾客信息：\n");
        Print_CustomerList(customer_list);
        int id;
        char new_type[30];
        printf("请输入要修改的顾客编号（输入'#'退出调整）：");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // 退出调整模式
        }
        id = atoi(input); // 将输入转换为整数
        printf("请输入新的顾客类型（NORMAL VIP SVIP）：");
        scanf("%s", new_type);
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
        strcpy(p->type, new_type);
        printf("顾客类型已修改！\n");
    }
}
