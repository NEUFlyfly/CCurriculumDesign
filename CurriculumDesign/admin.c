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

static int good_id_counter = 1;     // �����Զ�������Ʒ��ţ���ʼֵΪ1
static int purchase_id_counter = 1; // �����Զ����ɽ�����¼��ţ���ʼֵΪ1
void ResetGoodPurchaseId()
{
    good_id_counter = 1;
    purchase_id_counter = 1;
}

// ��������

/// ��������
// ��������Ʒ��Ϣ
void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{

    static int first_time = 1; // �����ж��Ƿ��һ�ε���Purchase����������ǵ�һ�ε��ã������purchaseInfo.txt�ļ�
    if (first_time)
    {
        FILE *fp = fopen("txt/purchaseInfo.txt", "w");
        if (fp == NULL)
        {
            printf("�ļ���ʧ�ܣ�\n");
            exit(1);
        }
        fclose(fp);
        first_time = 0; // ����Ϊ0����ʾ�Ѿ���չ��ļ�
    }

    char current_time[10];
    GetCurrentTime(current_time);

    // 1. �����µĽ�����¼
    PurchaseNode purchase;
    strcpy(purchase.good_name, good_name);
    strcpy(purchase.purchase_time, current_time); // �޸�Ϊʹ��purchase_time
    strcpy(purchase.supplier, supplier);
    strcpy(purchase.category, category);
    purchase.id = purchase_id_counter++;
    purchase.quantity = quantity;
    purchase.cost_price = cost_price;
    Insert_PurchaseList(&purchase_record, purchase);

    // 2. ������Ʒ���
    // ������Ʒ�Ƿ��Ѿ����ڣ������������¿�棬���򴴽��µ���Ʒ�ڵ�
    GoodNode *p = all_good_list->next;
    while (p != NULL)
    {
        if (strcmp(p->good_name, good_name) == 0 && strcmp(p->category, category) == 0 && strcmp(p->manufacturer, supplier) == 0 && p->price == cost_price)
        {
            p->stock += quantity; // ���¿��
            break;
        }
        p = p->next;
    }
    if (p == NULL) // �����Ʒ�����ڣ��򴴽��µ���Ʒ�ڵ�
    {
        GoodNode new_good;
        strcpy(new_good.good_name, good_name);
        strcpy(new_good.category, category);
        strcpy(new_good.manufacturer, supplier);
        new_good.price = cost_price;
        new_good.stock = quantity;
        new_good.is_promotion = 0; // Ĭ�ϲ�����
        new_good.promotion_price = 0;
        new_good.id = good_id_counter++; // ������Ʒ���
        Insert_AllGoodList(&all_good_list, new_good);
    }
    // 3. ���������־��purchaseInfo.txt����ʽ����Ʒ��� ��Ʒ���� �������� �������� ʵ��ʱ�� ��Ӧ������
    // ��ȡ��ǰʱ�䲢��ʽ��Ϊ�ַ���

    FILE *fp = fopen("txt/purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }
    // ��ʽ����Ʒid ��Ʒ��� ��Ʒ���� �������� �������� ʵ��ʱ�� ��Ӧ������
    fprintf(fp, "%d %s %s %.2f %d %s %s\n", purchase.id, purchase.category, purchase.good_name,
            purchase.cost_price, purchase.quantity, current_time, purchase.supplier);
    fclose(fp);
}

// ɾ��ָ����Ʒ�Ľ�����¼
// �����ɾ����ָ��txt�ļ�����ɾ��ָ����Ʒ���ƵĽ�����¼��������ɾ����Ʒ����Ҳ���Ǵ�������ɾ��
void Delete_PurchaseRecord(int id)
{
    FILE *fp = fopen("txt/purchaseInfo.txt", "r+");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }

    FILE *temp_fp = fopen("txt/temp.txt", "w");
    if (temp_fp == NULL)
    {
        perror("��ʱ�ļ���ʧ��");
        fclose(fp);
        return;
    }

    char line[256];
    int found = 0; // ����Ƿ��ҵ�Ҫɾ������Ʒ���
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int current_id;
        // �����н�������һ��������Ϊ��Ʒ���
        if (sscanf(line, "%d", &current_id) == 1 && current_id == id)
        {
            found = 1; // �ҵ�Ҫɾ������Ʒ��ţ���������
            continue;
        }
        // �����ǰ�в�����Ҫɾ������Ʒ��ţ���д����ʱ�ļ�
        fputs(line, temp_fp);
    }

    fclose(fp);
    fclose(temp_fp);

    // ����ҵ�Ҫɾ������Ʒ��ţ����滻ԭ�ļ�
    if (found)
    {
        remove("txt/purchaseInfo.txt");                 // ɾ��ԭ�ļ�
        rename("txt/temp.txt", "txt/purchaseInfo.txt"); // ��������ʱ�ļ�Ϊԭ�ļ���
    }
    else
    {
        remove("txt/temp.txt"); // ɾ����ʱ�ļ�
        printf("δ�ҵ�����Ʒ��ŵĽ�����¼��\n");
    }
}

// ����һ����Ʒ�Ľ�����¼
// �����������ָ��txt�ļ������ӽ�����¼��������������Ʒ����idΪ0����¶����
// ��ʽ����Ʒid ��Ʒ��� ��Ʒ���� �������� �������� ʵ��ʱ�� ��Ӧ������
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{
    FILE *fp = fopen("txt/purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }

    char current_time[9];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    snprintf(current_time, sizeof(current_time), "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);

    // ��Ʒ������Ϣ��������Ʒ��š���Ʒ���ơ�����ʱ�䡢�����۸񡢽���������������Դ��
    // ������� id Ϊ 0 ��ʾ�����������ע�����ᵽ�ı�¶���ˣ�
    fprintf(fp, "0 %s %s %.2f %d %s\n", good_name, current_time, cost_price, quantity, supplier);
    fclose(fp);
}

// ����˿���Ϣ
void CustomerInfo_Generate()
{
    char name[50], type[20], contact[50]; // ʹ���������δ��ʼ����ָ��

    // ѭ������˿���Ϣ������'#'��������
    while (1)
    {
        CustomerNode customer;
        printf("������˿�����������'#'��������");
        scanf("%49s", name); // �������볤�ȣ����⻺�������
        if (strcmp(name, "#") == 0)
        {
            break; // ����'#'����ѭ��
        }
        while (1)
        {
            printf("������˿����ͣ�normal vip svip����");
            scanf("%19s", type); // �������볤��
            if (strcmp(type, "normal") == 0 || strcmp(type, "vip") == 0 || strcmp(type, "svip") == 0)
            {
                break; // ����Ϸ����˳�ѭ��
            }
            else
            {
                printf("����Ĺ˿�������Ч�����������룡\n");
            }
        }
        printf("������˿���ϵ��ʽ��");
        scanf("%49s", contact); // �������볤��
        printf("�˿���Ϣ����ӵ�ϵͳ��\n\n");
        strcpy(customer.name, name);
        strcpy(customer.type, type);
        strcpy(customer.contact, contact);
        Insert_CustomerList(&customer_list, customer);
    }
    // ���ɹ˿���Ϣ�ļ�
    Create_CustomerInfo_File();
}

void Create_CustomerInfo_File()
{
    // ������ļ�
    FILE *fp = fopen("txt/customerInfo.txt", "w");
    if (fp == NULL)
    {
        printf("�ļ���ʧ�ܣ�\n");
        exit(1);
    }
    fclose(fp);
    // ����customer_list��˳��д���ļ�
    CustomerNode *p = customer_list->next;
    FILE *fp1 = fopen("txt/customerInfo.txt", "a");
    if (fp1 == NULL)
    {
        printf("�ļ���ʧ�ܣ�\n");
        exit(1);
    }
    while (p != NULL)
    {
        fprintf(fp1, "%d %s %s %s\n", p->id, p->name, p->type, p->contact);
        p = p->next;
    }
    fclose(fp1);
    printf("�˿���Ϣ�Ѹ���\n");
}

void Add_Customer_To_System()
{
    system("cls");
    printf("��������ӵĹ˿���Ϣ��\n");
    CustomerInfo_Generate();
}

void Delete_Customer_From_System()
{

    while (1)
    {

        Print_CustomerList(customer_list);
        printf("�Ѵ洢�Ĺ˿���Ϣ���ϣ����ο�\n");
        int id;
        printf("������Ҫɾ���Ĺ˿ͱ�ţ�����'#'�˳�ɾ������");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // �˳�ɾ��ģʽ
        }
        id = atoi(input); // ������ת��Ϊ����
        if (Delete_CustomerList(&customer_list, id) == 1)
        {
            printf("�˿���Ϣ��ɾ����\n\n");
        }
        else
            printf("����������\n\n");
    }
    Create_CustomerInfo_File();
}

void Adjust_Customer_Type()
{

    while (1)
    {
        int id;
        char new_type[30];
        printf("������Ҫ�޸ĵĹ˿�id������'#'�˳���������");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // �˳�����ģʽ
        }
        id = atoi(input); // ������ת��Ϊ����
        CustomerNode *p = customer_list->next;
        while (p != NULL && p->id != id)
        {
            p = p->next;
        }
        if (p == NULL)
        {
            printf("δ�ҵ��ù˿ͣ�\n");
            continue;
        }

        printf("�������µĹ˿����ͣ�normal vip svip����");
        scanf("%s", new_type);
        strcpy(p->type, new_type);

        printf("�˿��������޸ģ�\n\n");
    }
    Create_CustomerInfo_File();
}

// ��all_good_list��ɾ����Ʒ��Ϣ
// ... existing code ...

// ��all_good_list��ɾ����Ʒ��Ϣ
void Delete_Good_From_System()
{
    while (1)
    {
        system("cls");
        char input[100];
        int id;
        Print_AllGoodList(all_good_list);
        printf("���µ�������Ʒ��Ϣ������ʾ�����ο���\n");
        printf("������Ҫɾ������ƷID������ '#' �˳�����");
        scanf("%s", input);
        // �������뻺����
        while (getchar() != '\n')
            ;

        if (strcmp(input, "#") == 0)
        {
            break;
        }

        id = atoi(input);
        if (id == 0 && strcmp(input, "0") != 0)
        {
            printf("�������ƷID��Ч�����������롣\n");
            continue;
        }

        // ���� Delete_GoodList ����ɾ����Ʒ
        if (Delete_AllGoodList(&all_good_list, id))
        {
            printf("��ƷIDΪ %d ����Ʒ�ѳɹ�ɾ����\n", id);
        }
        else
        {
            printf("δ�ҵ���ƷIDΪ %d ����Ʒ�����������롣\n", id);
        }
        printf("���س�������...");
        while (getchar()!= '\n')
            ;
        getchar();
    }
}

// ������Ʒ�ۼ�
void Set_Good_Price()
{
    while (1)
    {
        char input[100];
        int id;
        float new_price;

        printf("������Ҫ�޸��ۼ۵���ƷID������ '#' �˳�����");
        scanf("%s", input);
        // �������뻺����
        while (getchar() != '\n')
            ;

        if (strcmp(input, "#") == 0)
        {
            break;
        }

        id = atoi(input);
        if (id == 0 && strcmp(input, "0") != 0)
        {
            printf("�������ƷID��Ч�����������롣\n");
            continue;
        }

        printf("�������µ���Ʒ�ۼۣ�");
        if (scanf("%f", &new_price) != 1)
        {
            printf("������ۼ���Ч�����������롣\n");
            // �������뻺����
            while (getchar() != '\n')
                ;
            continue;
        }
        // �������뻺����
        while (getchar() != '\n')
            ;

        // ������Ʒ�������ۼ�
        GoodNode *p = all_good_list->next;
        while (p != NULL)
        {
            if (p->id == id)
            {
                p->price = new_price;
                printf("��ƷIDΪ %d ����Ʒ�ۼ��Ѹ���Ϊ %.2f��\n", id, new_price);
                break;
            }
            p = p->next;
        }
        if (p == NULL)
        {
            printf("δ�ҵ���ƷIDΪ %d ����Ʒ�����������롣\n", id);
        }
    }
}

// ������Ʒ������Ϣ
void Set_Good_Promotion_Info()
{
    // ����ʾ������Ʒ��Ϣ
    Print_AllGoodList(all_good_list);
    printf("���е�������Ʒ��Ϣ������ʾ�����ο���\n");
    // ѭ����������#�˳�
    while (1)
    {
        int id;
        printf("������Ҫ���ô�����Ϣ����Ʒid������'#'�˳�����");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // �˳����ô�����Ϣģʽ
        }
        id = atoi(input); // ������ת��Ϊ����
        // �ҵ�id��Ӧ����Ʒ
        GoodNode *p = all_good_list->next;
        while (p != NULL && p->id != id)
        {
            p = p->next;
        }
        if (p == NULL)
        {
            printf("δ�ҵ�����Ʒ��\n");
            continue;
        }
        // �ҵ���Ʒ�����ô�����Ϣ
        char promotion_info[20];
        printf("�������Ƿ������1����Ϊ������0����Ϊ��������");
        int is_promotion;
        scanf("%d", &is_promotion);
        if (is_promotion == 1)
        {
            p->is_promotion = 1; // ����Ϊ����
            printf("�����������������������0.8��ʾ��8�ۣ���");
            float promotion_rate;
            scanf("%f", &promotion_rate);
            p->promotion_price = p->price * promotion_rate;       // ��������۸�
            sprintf(promotion_info, "%.2f", promotion_rate * 10); // ����һλС��
            strcat(promotion_info, "��");                         // ����ۺ�
        }
        else
        {
            p->is_promotion = 0;              // ����Ϊ������
            p->promotion_price = 0.0;         // �����۸�Ϊ0
            strcpy(promotion_info, "������"); // ���ô�����ϢΪ������
        }
        printf("%s������Ϣ������Ϊ��%s\n\n", p->good_name, promotion_info); // ���������Ϣ
    }
}

// ����ͬ����Ʒ���洢��һ�������У���ͬ�����ֻ�洢һ��
void Get_Good_Category(char category[][100])
{
    // ��ʼ�� category ����Ϊ���ַ���
    for (int i = 0; i < 100; i++)
    {
        category[i][0] = '\0';
    }

    GoodNode *p = all_good_list->next; // ָ��ͷ������һ���ڵ�
    if (p == NULL)
    { // �������Ϊ��
        printf("��Ʒ�����ϢΪ�գ�\n");
        return; // �˳�����
    }
    printf("��Ʒ�����Ϣ���£�\n");
    int count = 0; // ��¼���е��������
    while (p != NULL)
    { // ��������һ��һ����Ʒ�ڵ�
        int i;
        for (i = 0; i < count; i++)
            if (strcmp(category[i], p->category) == 0)
                break;
        if (i == count)
        {                                         // ���û���ҵ���ͬ�����
            strcpy(category[count], p->category); // ���������Ϣ������
            count++;                              // ���������1
        }
        p = p->next; // ����������һ���ڵ�
    }
}

// �鿴���е���Ʒ�����ͬ������ӡһ��
// ÿ�д�����ͻ���
void View_Good_Category()
{
    char category[100][100]; // �洢��ͬ����Ʒ���
    Get_Good_Category(category);
    int count = 0; // ��¼���е��������
    while (count < 100)
    { // �������飬һ��һ�����
        if (strcmp(category[count], "") == 0)
            break;                        // �������Ϊ�գ����˳�ѭ��
        printf("%-20s", category[count]); // ��������Ϣ
        count++;                          // ���������1
        if (count % 5 == 0)               // ����Ѿ����������������
            printf("\n");
    }
    printf("\n"); // ����
    printf("���س�����...\n");
    while (getchar()!= '\n');
    getchar();
}

/// ������Ʒ���ຯ��
/// ���ܣ���ָ����Ʒ�ӵ�ǰ�����ƶ����µķ���
void Adjust_Good_Category()
{
    while (1)
    {
        system("cls"); // ����
        char input[100];
        int id;
        char new_category[50];
        Print_AllGoodList(all_good_list);
        printf("���е�������Ʒ��Ϣ������ʾ�����ο���\n\n");
        printf("������Ҫ�����������ƷID������ '#' �˳�����");
        scanf("%s", input);

        if (strcmp(input, "#") == 0)
        {
            break;
        }

        id = atoi(input);
        if (id == 0 && strcmp(input, "0") != 0)
        {
            printf("�������ƷID��Ч�����������롣\n");
            continue;
        }

        printf("�������µ���Ʒ���ࣺ");
        scanf("%s", new_category);

        // ������Ʒ�Ƿ���ڣ������������·��࣬������ʾ��Ʒ������
        GoodNode *p = all_good_list->next;
        while (p != NULL)
        {
            if (p->id == id)
            {
                strcpy(p->category, new_category); // ���·���
                printf("��Ʒ�����Ѹ���Ϊ %s��\n\n", new_category);
                printf("���س�����...\n");
                while (getchar() != '\n')
                    ;      // ������뻺����
                getchar(); // �ȴ��û����س�
                break;
            }
            p = p->next;
        }
        if (p == NULL)
        { // �����Ʒ�����ڣ�����ʾ��Ʒ������
            printf("��Ʒ�����ڣ����������롣\n\n");
        }
    }
}

// ɾ�����еķ���Ϊcategory����Ʒ
void Delete_Good_Category()
{
    while (1)
    {
        system("cls"); // ����
        View_Good_Category();
        printf("���е�������Ʒ���������ʾ�����ο���\n\n");
        printf("������Ҫɾ������Ʒ������� '#' �˳�����");
        char category[50];
        scanf("%s", category);

        if (strcmp(category, "#") == 0)
        {
            break; // �˳�ѭ��
        }

        int found = 0;
        GoodNode *prev = all_good_list;
        GoodNode *p = all_good_list->next; // ָ��ͷ������һ���ڵ�
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
            printf("%s �����Ʒɾ���ɹ�\n", category);
        }
        else
        {
            printf("δ�ҵ� %s ���\n", category);
        }

        printf("���س�����...");
        while (getchar() != '\n')
            ;      // ������뻺����
        getchar(); // �ȴ��û����س�
    }
}
