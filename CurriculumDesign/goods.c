#include "goods.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// �����յ�Category����
void Init_CategoryList(CategoryList *list)
{
    *list = (CategoryList)malloc(sizeof(CategoryNode));
    if (*list == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// �����µ���Ʒ��Category����ͷ�巨
void Insert_CategoryList(CategoryList *list, Good good)
{
    CategoryNode *newnode = (CategoryNode *)malloc(sizeof(CategoryNode));
    if (newnode == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    newnode->good = good;
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

// ��Category������ɾ��ָ����Ʒ�ڵ�
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
        printf("δ�ҵ�����Ʒ��\n");
        return;
    }

    prev->next = curr->next;
    free(curr);
}

// �����յ�AllCategory����
void Init_AllCategoryList(AllCategoryList *list)
{
    *list = (AllCategoryList)malloc(sizeof(AllCategoryNode));
    if (*list == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    (*list)->next = NULL;
}

// �����µ���Ʒ���AllCategory����ͷ�巨
void Insert_AllCategoryList(AllCategoryList *list, CategoryList categorylist, char *category)
{
    AllCategoryNode *newnode = (AllCategoryNode *)malloc(sizeof(AllCategoryNode));
    if (newnode == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    newnode->categorylist = categorylist;
    strcpy(newnode->category, category);
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

// ��AllCategory������ɾ��ָ��������Ʒ����
void Delete_AllCategoryList(AllCategoryList *list, char *category)
{
    AllCategoryNode *p = *list, *q;
    while (p->next != NULL && strcmp(p->next->category, category) != 0)
    {
        p = p->next;
    }
    if (p->next == NULL)
    {
        printf("δ�ҵ������\n");
        return;
    }
    q = p->next;
    p->next = q->next;
    free(q);
}

// ��ӡAllCategory�����������Ʒ��Ϣ
void Print_AllCategoryList(AllCategoryList list)
{
    AllCategoryNode *p = list->next;
    while (p != NULL)
    {
        printf("[���]:%s\n", p->category);
        CategoryNode *q = p->categorylist->next;
        while (q != NULL)
        {
            printf("    ��Ʒ���ƣ�%s\n", q->good.good_name);
            printf("    ��Ʒ���%s\n", q->good.category);
            printf("    �������ң�%s\n", q->good.manufacturer);
            printf("    ���ۼ۸�%.2f\n", q->good.price);
            printf("    ���������%d\n", q->good.stock);
            printf("    �Ƿ������%s\n", q->good.is_promotion ? "��" : "��");
            if (q->good.is_promotion)
            {
                printf("    �����۸�%.2f\n", q->good.promotion_price);
            }
            else printf("    �����۸�%.2f\n", q->good.price);
            printf("\n");
            q = q->next;
        }
        p = p->next;
        printf("\n");
    }
}

