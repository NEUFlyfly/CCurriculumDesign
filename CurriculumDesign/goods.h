#ifndef GOODS_H_INCLUDED
#define GOODS_H_INCLUDED
#include "sale.h"
// Good为单个商品基本信息结构体
typedef struct Good
{
    int id;                // 商品编号（唯一标识）
    char good_name[50];    // 商品名称
    char category[30];     // 商品类别
    char manufacturer[50]; // 生产厂家
    char description[100]; // 商品描述（可选）
    float price;           // 销售价格（不考虑促销以及顾客类别）
    int stock;             // 库存数量
    int is_promotion;      // 是否促销（0=否，1=是）
    float promotion_price; // 促销价格（如果有促销）
} Good;



// CategoryList为一类商品的链表，CategoryNode为链表节点
typedef struct CategoryNode
{
    Good good;                 // 商品信息
    struct CategoryNode *next; // 指向下一个节点的指针
} *CategoryList, CategoryNode;

typedef struct AllCategoryNode //所有的商品只需要用一个链表存储
{
    struct AllCategoryNode *next;
    char category[50];     // 商品类别名称
    CategoryList categorylist;   // 一类商品链表的头节点地址
} AllCategoryNode, *AllCategoryList;


void Init_CategoryList(CategoryList *list);
void Insert_CategoryList(CategoryList *list, Good good);
void Delete_CategoryList(CategoryList *list, char *good_name);

void Init_AllCategoryList(AllCategoryList *list);
void Insert_AllCategoryList(AllCategoryList *list, CategoryList categorylist, char *category);
void Delete_AllCategoryList(AllCategoryList *list, char *category);
void Print_AllCategoryList(AllCategoryList list);



#endif // GOODS_H_INCLUDED
