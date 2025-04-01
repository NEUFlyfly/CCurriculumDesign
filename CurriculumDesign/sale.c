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

// 创建空的Sale链表
void Init_SaleList(SaleList *list)
{
    *list = (SaleList)malloc(sizeof(SaleNode));
    if (*list == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// 插入新的销售记录到Sale链表，头插法
void Insert_SaleList(SaleList *list, SaleNode sale)
{
    SaleNode *newnode = (SaleNode *)malloc(sizeof(SaleNode));
    newnode->quantity = sale.quantity;
    newnode->sale_price = sale.sale_price;
    strcpy(newnode->good_name, sale.good_name);

    if (newnode == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

// 向id为customer_id的顾客一次销售商品good_name，数量为quantity
// 需要进行更抽象的封装，由Sale_To_Customer函数根据good_name推测出good_id，然后调用Sale函数
void Sale(int customer_id, int good_id, int quantity)
{

    static int opened = 0; // 用于判断是否第一次调用Sale函数，如果是第一次调用，则清空saleInfo.txt文件
    if (opened == 0)
    {
        FILE *fp = fopen("txt/saleInfo.txt", "w");
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

    // 0. 函数合法性检查，检查顾客id是否存在，商品id是否存在，商品库存是否足够
    int customer_exist = 0, good_exist = 0, stock_enough = 0;
    CustomerNode *customer_ptr = customer_list->next;
    while (customer_ptr != NULL) // 若顾客id存在，则customer_ptr指向该顾客，否则指向NULL
    {
        if (customer_ptr->id == customer_id)
        {
            customer_exist = 1; // 顾客id存在
            break;
        }
        customer_ptr = customer_ptr->next;
    }
    if (customer_ptr == NULL)
    {
        printf("顾客id不存在！\n");
        return;
    }
    AllGoodList good_ptr = all_good_list->next;
    while (good_ptr != NULL) // 若商品id存在，则good_ptr指向该商品，否则指向NULL
    {
        if (good_ptr->id == good_id)
        {
            good_exist = 1; // 商品id存在
            break;
        }
        good_ptr = good_ptr->next;
    }
    if (good_ptr == NULL)
    {
        printf("商品id不存在！\n");
        return;
    }

    if (good_ptr->stock < quantity) // 检查库存是否足够
    {
        printf("库存不足！\n");
        return;
    }
    else
    {
        stock_enough = 1; // 库存足够
    }
    if (customer_exist == 0 || good_exist == 0 || stock_enough == 0) // 若顾客id不存在、商品id不存在、库存不足，则返回
    {
        return;
    }
    // 1. 创建新的销售记录
    SaleNode sale;
    strcpy(sale.good_name, good_ptr->good_name);
    strcpy(sale.sale_time, current_time); // 使用当前时间
    // 检查该商品是否促销，如果是促销，则销售价格为促销价格，否则为原价
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
    // 2. 更新商品库存

    good_ptr->stock -= quantity;
    printf("销售成功！\n");

    // 3. 保存销售记录到saleInfo.txt，格式：顾客id 顾客姓名 商品名称 销售单价 销售数量 销售时间
    FILE *fp = fopen("txt/saleInfo.txt", "a");
    if (fp == NULL)
    {
        perror("文件打开失败");
        return;
    }
    // 按顾客星级打折
    float discount = NORMAL_DISCOUNT;
    if (strcmp(customer_ptr->type, "vip") == 0)
    {
        discount = VIP_DISCOUNT;
    }
    else if (strcmp(customer_ptr->type, "svip") == 0)
    {
        discount = SVIP_DISCOUNT;
    }
    // 写入文件
    // 格式：商品编号、商品名称、销售时间、销售价格、销售数量、顾客姓名
    fprintf(fp, "%d %s %s %.2f %d %s\n", sale_id++,
            sale.good_name, sale.sale_time, sale.sale_price * discount, sale.quantity, customer_ptr->name);
    fclose(fp);
}
