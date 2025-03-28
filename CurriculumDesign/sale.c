#include "sale.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
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
