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

void Sale(int customer_id, char *good_name, int quantity)// 向id为customer_id的顾客一次销售商品good_name，数量为quantity
{
    static int opened = 0; // 用于判断是否第一次调用Sale函数，如果是第一次调用，则清空saleInfo.txt文件
    if (opened == 0)
    {
        FILE *fp = fopen("saleInfo.txt", "w");
        if (fp == NULL)
        {
            printf("文件打开失败！\n");
            exit(1);
        }
        fclose(fp);
        opened = 1; // 设置为1，表示已经清空过文件
    }
    

    char current_time[10];
    GetCurrentTime(current_time); // 获取当前时间

    // 0. 函数合法性检查，检查顾客id是否存在，商品名称是否存在，库存是否足够
    int id_exist = 0, good_exist = 0, stock_enough = 0;
    CustomerNode *customer_ptr = customer_list->next;
    while (customer_ptr != NULL) // 若顾客id存在，则customer_ptr指向该顾客，否则指向NULL
    {
        if (customer_ptr->id == customer_id)
        {
            id_exist = 1; // 顾客id存在
            break;
        }
        customer_ptr = customer_ptr->next;
    }
    if (customer_ptr == NULL)
    {
        printf("顾客id不存在！\n");
        return;
    }
    // 查找商品名称是否存在
    AllCategoryNode *category_ptr = all_category_list->next;
    CategoryNode *good_ptr = NULL;
    while (category_ptr != NULL) // 若商品名称存在，则category_ptr指向该商品，否则指向NULL
    {
        good_ptr = category_ptr->categorylist->next;
        while (good_ptr != NULL)
        {
            if (strcmp(good_ptr->good.good_name, good_name) == 0)
            {
                good_exist = 1; // 商品名称存在
                if (good_ptr->good.stock >= quantity) // 库存足够
                {
                    stock_enough = 1;
                }
                break;
            }
            good_ptr = good_ptr->next;
        }
        if (stock_enough == 1)
        {
            break; // 找到商品且库存足够，退出循环
        }
        category_ptr = category_ptr->next;
    }
    if (category_ptr == NULL || good_ptr == NULL)
    {
        printf("商品名称不存在！\n");
        return;
    }
    if (stock_enough == 0)
    {
        printf("库存不足！\n");
        return;
    }
    
    // 三者任意一个不满足则退出函数
    if(id_exist == 0 || good_exist == 0 || stock_enough == 0)
    {
        return;
    }

    // 1. 创建新的销售记录
    SaleNode sale;
    strcpy(sale.good_name, good_name);
    strcpy(sale.sale_time, current_time); // 使用当前时间
    //检查该商品是否促销
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
    // 2. 更新商品库存

    good_ptr->good.stock -= quantity;
    printf("销售成功！\n");

    // 3. 保存销售记录到saleInfo.txt，格式：顾客id 顾客姓名 商品名称 销售单价 销售数量 销售时间
    FILE *fp = fopen("saleInfo.txt", "a");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }
    // 按顾客星级打折
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
