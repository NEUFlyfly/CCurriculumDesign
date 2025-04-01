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

// ������Ʒ
void Sale_To_Customer(int customer_id, char *good_name, int quantity)
{
    // ����good_nameӦ���Ƕ�Ӧall_good_list�е��ĸ��ڵ�
    // �ڵ���ѡ��ԭ����ѡȡ����good_name��ͬ��quntity�㹻��price��ߵĽڵ�
    AllGoodList p = all_good_list->next;
    AllGoodList best_good = NULL;
    int found_good = 0; // ����Ƿ��ҵ���Ʒ
    while (p != NULL)   // ����all_good_list���ҵ�����good_name��ͬ��quntity�㹻��price����promotion_price��ߵĽڵ�
    {
        if (strcmp(p->good_name, good_name) == 0)
        {
            found_good = 1; // �ҵ���Ʒ
            if (p->stock >= quantity)
            {
                // �Ƚ� price �� promotion_price �е����ֵ
                float current_max_price = (p->price > p->promotion_price) ? p->price : p->promotion_price;
                float best_max_price = (best_good != NULL) ? ((best_good->price > best_good->promotion_price) ? best_good->price : best_good->promotion_price) : 0;

                if (best_good == NULL || current_max_price > best_max_price)
                {
                    best_good = p;
                }
            }
        }
        p = p->next;
    }
    if (!found_good)
    {
        printf("û���ҵ�����Ʒ��\n");
        return; // û���ҵ�����Ʒ��ֱ�ӷ���
    }
    if (!best_good)
    {
        printf("��Ʒ��治�㣡\n");
        return; // ��Ʒ��治�㣬ֱ�ӷ���
    }
    Sale(customer_id, best_good->id, quantity); // ����Sale��������best_good��id��quantity����
}