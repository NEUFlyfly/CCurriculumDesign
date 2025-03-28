#include "purchase.h"
#include "stdlib.h"
// 创建空的Purchase链表
void Init_PurchaseList(PurchaseList *list)
{
    *list = (PurchaseList)malloc(sizeof(PurchaseNode));
    if (*list == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// 插入新的进货记录到Purchase链表，头插法
void Insert_PurchaseList(PurchaseList *list, PurchaseNode purchase)
{
    PurchaseNode *newnode = (PurchaseNode *)malloc(sizeof(PurchaseNode));
    newnode->quantity = purchase.quantity;
    newnode->cost_price = purchase.cost_price;
    strcpy(newnode->purchase_time, purchase.purchase_time);
    strcpy(newnode->good_name, purchase.good_name);
    strcpy(newnode->supplier, purchase.supplier);
    strcpy(newnode->category, purchase.category);

    if (newnode == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

