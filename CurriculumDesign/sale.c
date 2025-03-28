#include "sale.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
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
