#include "purchase.h"
#include "stdlib.h"
// �����յ�Purchase����
void Init_PurchaseList(PurchaseList *list)
{
    *list = (PurchaseList)malloc(sizeof(PurchaseNode));
    if (*list == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// �����µĽ�����¼��Purchase����ͷ�巨
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
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

