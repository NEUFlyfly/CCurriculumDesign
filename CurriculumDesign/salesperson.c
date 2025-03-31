#include "salesperson.h"
#include "sale.h"
#include "goods.h"
#include "purchase.h"
#include "customer.h"
#include "system.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern AllCategoryList all_category_list;
extern SaleList sale_record;
extern PurchaseList purchase_record;
extern CustomerList customer_list;

#define NORMAL_DISCOUNT 1.0
#define VIP_DISCOUNT 0.9
#define SVIP_DISCOUNT 0.8

void Sale(int customer_id, char *good_name, int quantity)// ��idΪcustomer_id�Ĺ˿�һ��������Ʒgood_name������Ϊquantity
{
    static int opened = 0; // �����ж��Ƿ��һ�ε���Sale����������ǵ�һ�ε��ã������saleInfo.txt�ļ�
    if (opened == 0)
    {
        FILE *fp = fopen("saleInfo.txt", "w");
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

    // 0. �����Ϸ��Լ�飬���˿�id�Ƿ���ڣ���Ʒ�����Ƿ���ڣ�����Ƿ��㹻
    int id_exist = 0, good_exist = 0, stock_enough = 0;
    CustomerNode *customer_ptr = customer_list->next;
    while (customer_ptr != NULL) // ���˿�id���ڣ���customer_ptrָ��ù˿ͣ�����ָ��NULL
    {
        if (customer_ptr->id == customer_id)
        {
            id_exist = 1; // �˿�id����
            break;
        }
        customer_ptr = customer_ptr->next;
    }
    if (customer_ptr == NULL)
    {
        printf("�˿�id�����ڣ�\n");
        return;
    }
    // ������Ʒ�����Ƿ����
    AllCategoryNode *category_ptr = all_category_list->next;
    CategoryNode *good_ptr = NULL;
    while (category_ptr != NULL) // ����Ʒ���ƴ��ڣ���category_ptrָ�����Ʒ������ָ��NULL
    {
        good_ptr = category_ptr->categorylist->next;
        while (good_ptr != NULL)
        {
            if (strcmp(good_ptr->good.good_name, good_name) == 0)
            {
                good_exist = 1; // ��Ʒ���ƴ���
                if (good_ptr->good.stock >= quantity) // ����㹻
                {
                    stock_enough = 1;
                }
                break;
            }
            good_ptr = good_ptr->next;
        }
        if (stock_enough == 1)
        {
            break; // �ҵ���Ʒ�ҿ���㹻���˳�ѭ��
        }
        category_ptr = category_ptr->next;
    }
    if (category_ptr == NULL || good_ptr == NULL)
    {
        printf("��Ʒ���Ʋ����ڣ�\n");
        return;
    }
    if (stock_enough == 0)
    {
        printf("��治�㣡\n");
        return;
    }
    
    // ��������һ�����������˳�����
    if(id_exist == 0 || good_exist == 0 || stock_enough == 0)
    {
        return;
    }

    // 1. �����µ����ۼ�¼
    SaleNode sale;
    strcpy(sale.good_name, good_name);
    strcpy(sale.sale_time, current_time); // ʹ�õ�ǰʱ��
    //������Ʒ�Ƿ����
    if(good_ptr->good.is_promotion == 1)
    {
        sale.sale_price = good_ptr->good.promotion_price;
    }
    else
    {
        sale.sale_price = good_ptr->good.price;
    }
    sale.quantity = quantity;
    Insert_SaleList(&sale_record, sale);
    // 2. ������Ʒ���

    good_ptr->good.stock -= quantity;
    printf("���۳ɹ���\n");

    // 3. �������ۼ�¼��saleInfo.txt����ʽ���˿�id �˿����� ��Ʒ���� ���۵��� �������� ����ʱ��
    FILE *fp = fopen("saleInfo.txt", "a");
    if (fp == NULL)
    {
        perror("�ļ���ʧ��");
        return;
    }
    // ���˿��Ǽ�����
    float discount = NORMAL_DISCOUNT;
    if(strcmp(customer_ptr->type, "vip") == 0)
    {
        discount = VIP_DISCOUNT;
    }
    else if(strcmp(customer_ptr->type, "svip") == 0)
    {
        discount = SVIP_DISCOUNT;
    }
    fprintf(fp, "%d %s %s %.2f %d %s\n", customer_id, customer_ptr->name, good_name, sale.sale_price * discount, quantity, current_time);
    fclose(fp);
    
}
