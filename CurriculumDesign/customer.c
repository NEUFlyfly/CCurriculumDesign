#include "customer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ��������
void Init_CustomerList(CustomerList *list);
void Insert_CustomerList(CustomerList *list, CustomerNode customer);
int Delete_CustomerList(CustomerList *list, int id);
void Print_CustomerList(CustomerList list);

void Init_CustomerList(CustomerList *list)
{
    *list = (CustomerList)malloc(sizeof(CustomerNode));
    if (*list == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    (*list)->next = NULL;
}

void Insert_CustomerList(CustomerList *list, CustomerNode customer)// ÿ�μ���һ���˿͵���Ϣ��Ԫ��(name type contact)ʱ���Զ����ɹ˿ͱ��
{
    static int id_counter = 0; 
    customer.id = ++id_counter; // �Զ����ɹ˿ͱ��

    CustomerNode *newnode = (CustomerNode *)malloc(sizeof(CustomerNode));
    if (newnode == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    newnode->id = customer.id;
    strcpy(newnode->name, customer.name);
    strcpy(newnode->type, customer.type);
    strcpy(newnode->contact, customer.contact);
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

int Delete_CustomerList(CustomerList *list, int id)
{
    CustomerNode *prev = *list;
    CustomerNode *curr = (*list)->next;

    while (curr != NULL && curr->id != id)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL)
    {
        printf("δ�ҵ��ù˿ͣ�\n");
        return 0;
    }

    prev->next = curr->next;
    free(curr);
    return 1;
}

void Print_CustomerList(CustomerList list)
{
    printf("[�˿���Ϣ�б�]��\n");
    CustomerNode *p = list->next;
    while (p != NULL)
    {
        printf("    �˿�id��%-3d", p->id);
        printf("  �˿�������%-10s", p->name);
        printf("  �˿����ͣ�%-5s", p->type);
        printf("  �˿���ϵ��ʽ��%-10s\n", p->contact);
        p = p->next;
    }
}
