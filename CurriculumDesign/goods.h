#ifndef GOODS_H_INCLUDED
#define GOODS_H_INCLUDED
#include "sale.h"
// GoodΪ������Ʒ������Ϣ�ṹ��
typedef struct Good
{
    int id;                // ��Ʒ��ţ�Ψһ��ʶ��
    char good_name[50];    // ��Ʒ����
    char category[30];     // ��Ʒ���
    char manufacturer[50]; // ��������
    char description[100]; // ��Ʒ��������ѡ��
    float price;           // ���ۼ۸񣨲����Ǵ����Լ��˿����
    int stock;             // �������
    int is_promotion;      // �Ƿ������0=��1=�ǣ�
    float promotion_price; // �����۸�����д�����
} Good;



// CategoryListΪһ����Ʒ������CategoryNodeΪ����ڵ�
typedef struct CategoryNode
{
    Good good;                 // ��Ʒ��Ϣ
    struct CategoryNode *next; // ָ����һ���ڵ��ָ��
} *CategoryList, CategoryNode;

typedef struct AllCategoryNode //���е���Ʒֻ��Ҫ��һ������洢
{
    struct AllCategoryNode *next;
    char category[50];     // ��Ʒ�������
    CategoryList categorylist;   // һ����Ʒ�����ͷ�ڵ��ַ
} AllCategoryNode, *AllCategoryList;


void Init_CategoryList(CategoryList *list);
void Insert_CategoryList(CategoryList *list, Good good);
void Delete_CategoryList(CategoryList *list, char *good_name);

void Init_AllCategoryList(AllCategoryList *list);
void Insert_AllCategoryList(AllCategoryList *list, CategoryList categorylist, char *category);
void Delete_AllCategoryList(AllCategoryList *list, char *category);
void Print_AllCategoryList(AllCategoryList list);



#endif // GOODS_H_INCLUDED
