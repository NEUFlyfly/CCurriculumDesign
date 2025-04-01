#include "customer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 函数声明
void Init_CustomerList(CustomerList *list);
void Insert_CustomerList(CustomerList *list, CustomerNode customer);
int Delete_CustomerList(CustomerList *list, int id);
void Print_CustomerList(CustomerList list);

void Init_CustomerList(CustomerList *list)
{
    *list = (CustomerList)malloc(sizeof(CustomerNode));
    if (*list == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*list)->next = NULL;
}

void Insert_CustomerList(CustomerList *list, CustomerNode customer)// 每次加入一个顾客的信息三元组(name type contact)时，自动生成顾客编号
{
    static int id_counter = 0; 
    customer.id = ++id_counter; // 自动生成顾客编号

    CustomerNode *newnode = (CustomerNode *)malloc(sizeof(CustomerNode));
    if (newnode == NULL)
    {
        printf("内存分配失败！\n");
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
        printf("未找到该顾客！\n");
        return 0;
    }

    prev->next = curr->next;
    free(curr);
    return 1;
}

void Print_CustomerList(CustomerList list)
{
    // 打印表头
    printf("[顾客信息列表]：\n");
    printf("    %-8s%-12s%-10s%-15s\n", "顾客id", "顾客姓名", "顾客类型", "顾客联系方式");
    // 打印分割线
    CustomerNode *p = list->next;
    while (p != NULL)
    {
        // 打印顾客信息
        printf("    %-8d%-12s%-10s%-15s\n", p->id, p->name, p->type, p->contact);
        p = p->next;
    }
}
