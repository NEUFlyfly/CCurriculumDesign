#include "goods.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 创建空的Category链表
void Init_CategoryList(CategoryList *list)
{
    *list = (CategoryList)malloc(sizeof(CategoryNode));
    if (*list == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// 插入新的商品到Category链表，头插法
void Insert_CategoryList(CategoryList *list, Good good)
{
    CategoryNode *newnode = (CategoryNode *)malloc(sizeof(CategoryNode));
    if (newnode == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    newnode->good = good;
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

// 从Category链表中删除指定商品节点
void Delete_CategoryList(CategoryList *list, char *good_name)
{
    CategoryNode *prev = *list;
    CategoryNode *curr = (*list)->next;

    while (curr != NULL && strcmp(curr->good.good_name, good_name) != 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL)
    {
        printf("未找到该商品！\n");
        return;
    }

    prev->next = curr->next;
    free(curr);
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

// 创建空的AllCategory链表
void Init_AllCategoryList(AllCategoryList *list)
{
    *list = (AllCategoryList)malloc(sizeof(AllCategoryNode));
    if (*list == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// 插入新的商品类别到AllCategory链表，头插法
void Insert_AllCategoryList(AllCategoryList *list, CategoryList categorylist, char *category)
{
    AllCategoryNode *newnode = (AllCategoryNode *)malloc(sizeof(AllCategoryNode));
    if (newnode == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    newnode->categorylist = categorylist;
    strcpy(newnode->category, category);
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

// 从AllCategory链表中删除指定类别的商品链表
void Delete_AllCategoryList(AllCategoryList *list, char *category)
{
    AllCategoryNode *p = *list, *q;
    while (p->next != NULL && strcmp(p->next->category, category) != 0)
    {
        p = p->next;
    }
    if (p->next == NULL)
    {
        printf("未找到该类别！\n");
        return;
    }
    q = p->next;
    p->next = q->next;
    free(q);
}

// 打印AllCategory链表的所有商品信息
void Print_AllCategoryList(AllCategoryList list)
{
    AllCategoryNode *p = list->next;
    while (p != NULL)
    {
        printf("[类别]:%s\n", p->category);
        CategoryNode *q = p->categorylist->next;
        while (q != NULL)
        {
            printf("    商品名称：%s\n", q->good.good_name);
            printf("    商品类别：%s\n", q->good.category);
            printf("    生产厂家：%s\n", q->good.manufacturer);
            printf("    销售价格：%.2f\n", q->good.price);
            printf("    库存数量：%d\n\n", q->good.stock);
            q = q->next;
        }
        p = p->next;
        printf("\n");
    }
}

