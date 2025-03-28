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

// ��������

void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Delete_PurchaseRecord(char *good_name);
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Adjust_Category(char *good_name, char *new_category);
void CustomerInfo_Generate();
void Add_Customer_To_System();
void Delete_Customer_From_System();
void Adjust_Customer_Type();


/// ��������
// ��������Ʒ��Ϣ
void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{

    static int first_time = 1; // �����ж��Ƿ��һ�ε���Purchase����������ǵ�һ�ε��ã������purchaseInfo.txt�ļ�
    if (first_time)
    {
        FILE *fp = fopen("purchaseInfo.txt", "w");
        if (fp == NULL)
        {
            printf("�ļ���ʧ�ܣ�\n");
            exit(1);
        }
        fclose(fp);
        first_time = 0; // ����Ϊ0����ʾ�Ѿ���չ��ļ�
    }



    char current_time[9];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    snprintf(current_time, sizeof(current_time), "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);

    // 1. �����µĽ�����¼
    PurchaseNode purchase;
    strcpy(purchase.good_name, good_name);
    strcpy(purchase.purchase_time, current_time); // �޸�Ϊʹ��purchase_time
    strcpy(purchase.supplier, supplier);
    strcpy(purchase.category, category);
    purchase.quantity = quantity;
    purchase.cost_price = cost_price;
    Insert_PurchaseList(&purchase_record, purchase);
    // 2. ������Ʒ���
    // ����all_category_list���Ƿ���category
    AllCategoryNode *allListptr = all_category_list->next;
    while (allListptr != NULL && strcmp(allListptr->category, category) != 0)
    {
        allListptr = allListptr->next;
    }
    if (allListptr == NULL) // δ�ҵ��������Ҫ�½�һ�����ͷ�巨
    {
        CategoryList category_list;
        Init_CategoryList(&category_list);
        Insert_AllCategoryList(&all_category_list, category_list, category);
        allListptr = all_category_list->next; // ��ʱallListptrָ���½������
        // Ϊ�½�����������Ʒ
        Good good;
        strcpy(good.good_name, good_name);
        strcpy(good.category, category);
        strcpy(good.manufacturer, supplier);
        good.price = cost_price;
        good.stock = quantity;
        Insert_CategoryList(&category_list, good);
    }
    else // �ҵ�������ˣ���ôallListptrָ������
    {
        CategoryList category_list = allListptr->categorylist;
        CategoryNode *categoryListptr = category_list->next;
        while (categoryListptr != NULL && strcmp(categoryListptr->good.good_name, good_name) != 0)
        {
            categoryListptr = categoryListptr->next;
        }
        if (categoryListptr == NULL) // δ�ҵ�����Ʒ����Ҫ�½�һ����Ʒ��ͷ�巨
        {
            Good good;
            strcpy(good.good_name, good_name);
            strcpy(good.category, category);
            strcpy(good.manufacturer, supplier);
            good.price = cost_price;
            good.stock = quantity;
            Insert_CategoryList(&category_list, good);
        }
        else // �ҵ�����Ʒ�ˣ���ôcategoryListptrָ�����Ʒ
        {
            categoryListptr->good.stock += quantity;
        }
    }

    // 3. ���������־��purchaseInfo.txt����ʽ����Ʒ��� ��Ʒ���� �������� �������� ʵ��ʱ�䣨����13:05�� ��Ӧ������
    // ��ȡ��ǰʱ�䲢��ʽ��Ϊ�ַ���

    FILE *fp = fopen("purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }
    fprintf(fp, "%s %s %.2f %d %s %s\n", category, good_name, cost_price, quantity, current_time, supplier);
    fclose(fp);
}

// ɾ��ָ����Ʒ�Ľ�����¼
// �����ɾ����ָ��txt�ļ�����ɾ��ָ����Ʒ���ƵĽ�����¼��������ɾ����Ʒ����Ҳ���Ǵ�������ɾ��
void Delete_PurchaseRecord(char *good_name)
{
    FILE *fp = fopen("purchaseInfo.txt", "r+");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }

    FILE *temp_fp = fopen("temp.txt", "w");
    if (temp_fp == NULL)
    {
        perror("��ʱ�ļ���ʧ��");
        fclose(fp);
        return;
    }

    char line[256];
    int found = 0; // ����Ƿ��ҵ�Ҫɾ������Ʒ����
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, good_name) == NULL) // �����ǰ�в�����Ҫɾ������Ʒ���ƣ���д����ʱ�ļ�
        {
            fputs(line, temp_fp);
        }
        else
        {
            found = 1; // �ҵ�Ҫɾ������Ʒ����
        }
    }

    fclose(fp);
    fclose(temp_fp);

    // ����ҵ�Ҫɾ������Ʒ���ƣ����滻ԭ�ļ�
    if (found)
    {
        remove("purchaseInfo.txt"); // ɾ��ԭ�ļ�
        rename("temp.txt", "purchaseInfo.txt"); // ��������ʱ�ļ�Ϊԭ�ļ���
    }
    else
    {
        remove("temp.txt"); // ɾ����ʱ�ļ�
        printf("δ�ҵ�����Ʒ���ƵĽ�����¼��\n");
    }
}

// ����һ����Ʒ�Ľ�����¼
// �����������ָ��txt�ļ������ӽ�����¼����������ӵ������У�Ҳ���������Ʒ����������������
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier)
{
    FILE *fp = fopen("purchaseInfo.txt", "a");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }

    char current_time[9];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    snprintf(current_time, sizeof(current_time), "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);

    fprintf(fp, "%s %s %.2f %d %s %s\n", category, good_name, cost_price, quantity, current_time, supplier);
    fclose(fp);
}

/// ������Ʒ���ຯ��
/// ���ܣ���ָ����Ʒ�ӵ�ǰ�����ƶ����µķ���
void Adjust_Category(char *good_name, char *new_category)
{
    AllCategoryNode *allListptr = all_category_list->next;
    AllCategoryNode *oldCategoryNode = NULL;
    CategoryNode *targetGoodNode = NULL;

    // 1. ���������������������Ʒ���ڵ����
    while (allListptr != NULL)
    {
        CategoryList category_list = allListptr->categorylist;
        CategoryNode *categoryListptr = category_list->next;

        while (categoryListptr != NULL)
        {
            if (strcmp(categoryListptr->good.good_name, good_name) == 0)
            {
                oldCategoryNode = allListptr; // ��¼�����ڵ�
                targetGoodNode = categoryListptr; // ��¼Ŀ����Ʒ�ڵ�
                break;
            }
            categoryListptr = categoryListptr->next;
        }

        if (targetGoodNode != NULL)
        {
            break; // �ҵ���Ʒ���˳�ѭ��
        }

        allListptr = allListptr->next;
    }

    // 2. ���δ�ҵ���Ʒ�������ʾ��Ϣ
    if (targetGoodNode == NULL)
    {
        printf("δ�ҵ�����Ϊ %s ����Ʒ��\n", good_name);
        return;
    }

    // 3. �Ӿ����������ɾ������Ʒ�ڵ�
    Delete_CategoryList(&oldCategoryNode->categorylist, good_name);

    // ��������Ƿ�Ϊ�գ����Ϊ����ɾ�������
    if (oldCategoryNode->categorylist->next == NULL)
    {
        Delete_AllCategoryList(&all_category_list, oldCategoryNode->category);
    }

    // 4. ���������Ƿ����
    AllCategoryNode *newCategoryNode = all_category_list->next;
    while (newCategoryNode != NULL && strcmp(newCategoryNode->category, new_category) != 0)
    {
        newCategoryNode = newCategoryNode->next;
    }

    // 5. �������𲻴��ڣ��򴴽�����𲢲�����Ʒ
    if (newCategoryNode == NULL)
    {
        CategoryList new_category_list;
        Init_CategoryList(&new_category_list);
        Insert_AllCategoryList(&all_category_list, new_category_list, new_category);
        newCategoryNode = all_category_list->next; // �����ڵ�
    }

    // 6. �޸���Ʒ�������Ϣ���������������
    Good updatedGood = targetGoodNode->good;
    strcpy(updatedGood.category, new_category);
    strcpy(updatedGood.good_name, good_name); // ������Ʒ�������Ϣ
    Insert_CategoryList(&newCategoryNode->categorylist, updatedGood);
    
}

// ����˿���Ϣ�������ļ�
void CustomerInfo_Generate()
{
    // ������

    char name[50], type[20], contact[50]; // ʹ���������δ��ʼ����ָ��
    // ���customerInfo.txt�ļ�
    FILE *fp = fopen("customerInfo.txt", "w");
    if (fp == NULL)
    {
        printf("�ļ���ʧ�ܣ�\n");
        exit(1);
    }
    fclose(fp);
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
        printf("������˿����ͣ�NORMAL VIP SVIP����");
        scanf("%19s", type); // �������볤��
        printf("������˿���ϵ��ʽ��");
        scanf("%49s", contact); // �������볤��
        strcpy(customer.name, name);
        strcpy(customer.type, type);
        strcpy(customer.contact, contact);
        Insert_CustomerList(&customer_list, customer);
        // ���˿���Ϣд���ļ�
        FILE *fp1 = fopen("customerInfo.txt", "a");
        if (fp1 == NULL)
        {
            printf("�ļ���ʧ�ܣ�\n");
            exit(1);
        }
        fprintf(fp1, "%s %s %s\n", customer.name, customer.type, customer.contact);
        fclose(fp1);
    }
}

void Add_Customer_To_System()
{
    system("cls");
    printf("��������ӵĹ˿���Ϣ��\n");
    CustomerInfo_Generate();
    printf("�˿���Ϣ����ӵ�ϵͳ��\n");
}

void Delete_Customer_From_System()
{
    
    while (1)
    {
        system("cls");
        printf("�Ѵ洢�Ĺ˿���Ϣ��\n");
        Print_CustomerList(customer_list);
        int id;
        printf("������Ҫɾ���Ĺ˿ͱ�ţ�����'#'�˳�ɾ������");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // �˳�ɾ��ģʽ
        }
        id = atoi(input); // ������ת��Ϊ����
        if(Delete_CustomerList(&customer_list, id) == 1)
        {
            printf("�˿���Ϣ��ɾ�������س�����...\n");
        }
        else
        {
            printf("δ�ҵ��ù˿ͣ����س�����...\n");
        }
        getchar(); // �����һ������Ļ��з�
        getchar(); // �ȴ��û����س�
    }
}

void Adjust_Customer_Type()
{
    system("cls");
    while (1)
    {
        printf("�Ѵ洢�Ĺ˿���Ϣ��\n");
        Print_CustomerList(customer_list);
        int id;
        char new_type[30];
        printf("������Ҫ�޸ĵĹ˿ͱ�ţ�����'#'�˳���������");
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "#") == 0)
        {
            break; // �˳�����ģʽ
        }
        id = atoi(input); // ������ת��Ϊ����
        printf("�������µĹ˿����ͣ�NORMAL VIP SVIP����");
        scanf("%s", new_type);
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
        strcpy(p->type, new_type);
        system("cls");
        printf("�˿��������޸ģ�\n");
    }
}
