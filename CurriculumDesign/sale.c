#include "salesperson.h"
#include "sale.h"
#include "goods.h"
#include "purchase.h"
#include "customer.h"
#include "system.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern AllGoodList all_good_list;
extern SaleList sale_record;
extern PurchaseList purchase_record;
extern CustomerList customer_list;

#define NORMAL_DISCOUNT 1.0
#define VIP_DISCOUNT 0.9
#define SVIP_DISCOUNT 0.8

static int sale_id = 1;

void ResetSaleId()
{
    sale_id = 1;
}

// �����յ�Sale����
void Init_SaleList(SaleList *list)
{
    *list = (SaleList)malloc(sizeof(SaleNode));
    if (*list == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// �����µ����ۼ�¼��Sale����ͷ�巨
void Insert_SaleList(SaleList *list, SaleNode sale)
{
    SaleNode *newnode = (SaleNode *)malloc(sizeof(SaleNode));
    newnode->quantity = sale.quantity;
    newnode->sale_price = sale.sale_price;
    strcpy(newnode->good_name, sale.good_name);

    if (newnode == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

// ��idΪcustomer_id�Ĺ˿�һ��������Ʒgood_name������Ϊquantity
// ��Ҫ���и�����ķ�װ����Sale_To_Customer��������good_name�Ʋ��good_id��Ȼ�����Sale����
void Sale(int customer_id, int good_id, int quantity)
{

    static int opened = 0; // �����ж��Ƿ��һ�ε���Sale����������ǵ�һ�ε��ã������saleInfo.txt�ļ�
    if (opened == 0)
    {
        FILE *fp = fopen("txt/saleInfo.txt", "w");
        if (fp == NULL)
        {
            printf("�ļ���ʧ�ܣ�\n");
            exit(1);
        }
        fclose(fp);
        opened = 1; // ����Ϊ1����ʾ�Ѿ���չ��ļ�
    }

    char current_time[10];
    GetCurrentTime(current_time); // ��ȡ��ǰʱ��

    // 0. �����Ϸ��Լ�飬���˿�id�Ƿ���ڣ���Ʒid�Ƿ���ڣ���Ʒ����Ƿ��㹻
    int customer_exist = 0, good_exist = 0, stock_enough = 0;
    CustomerNode *customer_ptr = customer_list->next;
    while (customer_ptr != NULL) // ���˿�id���ڣ���customer_ptrָ��ù˿ͣ�����ָ��NULL
    {
        if (customer_ptr->id == customer_id)
        {
            customer_exist = 1; // �˿�id����
            break;
        }
        customer_ptr = customer_ptr->next;
    }
    if (customer_ptr == NULL)
    {
        printf("�˿�id�����ڣ�\n");
        return;
    }
    AllGoodList good_ptr = all_good_list->next;
    while (good_ptr != NULL) // ����Ʒid���ڣ���good_ptrָ�����Ʒ������ָ��NULL
    {
        if (good_ptr->id == good_id)
        {
            good_exist = 1; // ��Ʒid����
            break;
        }
        good_ptr = good_ptr->next;
    }
    if (good_ptr == NULL)
    {
        printf("��Ʒid�����ڣ�\n");
        return;
    }

    if (good_ptr->stock < quantity) // ������Ƿ��㹻
    {
        printf("��治�㣡\n");
        return;
    }
    else
    {
        stock_enough = 1; // ����㹻
    }
    if (customer_exist == 0 || good_exist == 0 || stock_enough == 0) // ���˿�id�����ڡ���Ʒid�����ڡ���治�㣬�򷵻�
    {
        return;
    }
    // 1. �����µ����ۼ�¼
    SaleNode sale;
    strcpy(sale.good_name, good_ptr->good_name);
    strcpy(sale.sale_time, current_time); // ʹ�õ�ǰʱ��
    // ������Ʒ�Ƿ����������Ǵ����������ۼ۸�Ϊ�����۸񣬷���Ϊԭ��
    if (good_ptr->is_promotion == 1)
    {
        sale.sale_price = good_ptr->promotion_price;
    }
    else
    {
        sale.sale_price = good_ptr->price;
    }
    sale.quantity = quantity;
    Insert_SaleList(&sale_record, sale);
    // 2. ������Ʒ���

    good_ptr->stock -= quantity;
    printf("���۳ɹ���\n");

    // 3. �������ۼ�¼��saleInfo.txt����ʽ���˿�id �˿����� ��Ʒ���� ���۵��� �������� ����ʱ��
    FILE *fp = fopen("txt/saleInfo.txt", "a");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }
    // ���˿��Ǽ�����
    float discount = NORMAL_DISCOUNT;
    if (strcmp(customer_ptr->type, "vip") == 0)
    {
        discount = VIP_DISCOUNT;
    }
    else if (strcmp(customer_ptr->type, "svip") == 0)
    {
        discount = SVIP_DISCOUNT;
    }
    // д���ļ�
    // ��ʽ����Ʒ��š���Ʒ���ơ�����ʱ�䡢���ۼ۸������������˿�����
    fprintf(fp, "%d %s %s %.2f %d %s\n", sale_id++,
            sale.good_name, sale.sale_time, sale.sale_price * discount, sale.quantity, customer_ptr->name);
    fclose(fp);
}
