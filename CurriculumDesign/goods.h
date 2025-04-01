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
    struct Good *next;     // 指向下一个节点的指针
} *AllGoodList, GoodNode;


void Init_AllGoodList(AllGoodList *list);
void Insert_AllGoodList(AllGoodList *list, GoodNode good);
int Delete_AllGoodList(AllGoodList *list, int id);
void Print_AllGoodList(AllGoodList list);

#endif // GOODS_H_INCLUDED
