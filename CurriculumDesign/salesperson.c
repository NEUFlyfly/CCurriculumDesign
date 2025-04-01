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

// 销售商品
void Sale_To_Customer(int customer_id, char *good_name, int quantity)
{
    // 决策good_name应该是对应all_good_list中的哪个节点
    // 节点挑选的原则是选取所有good_name相同，quntity足够且price最高的节点
    AllGoodList p = all_good_list->next;
    AllGoodList best_good = NULL;
    int found_good = 0; // 标记是否找到商品
    while (p != NULL)   // 遍历all_good_list，找到所有good_name相同，quntity足够且price或者promotion_price最高的节点
    {
        if (strcmp(p->good_name, good_name) == 0)
        {
            found_good = 1; // 找到商品
            if (p->stock >= quantity)
            {
                // 比较 price 和 promotion_price 中的最大值
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
        printf("没有找到该商品！\n");
        return; // 没有找到该商品，直接返回
    }
    if (!best_good)
    {
        printf("商品库存不足！\n");
        return; // 商品库存不足，直接返回
    }
    Sale(customer_id, best_good->id, quantity); // 调用Sale函数，将best_good的id和quantity传入
}