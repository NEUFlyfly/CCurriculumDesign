#ifndef SALE_H_INCLUDED
#define SALE_H_INCLUDED

typedef struct SaleNode
{
    char good_name[20]; // 商品名称
    float sale_price;   // 实际销售单价（考虑折扣或促销）
    char sale_time[20]; // 销售时间（格式：四位十进制整数）
    int quantity;       // 销售数量
    struct SaleNode *next;
} SaleNode, *SaleList;

void Init_SaleList(SaleList *list);
void Insert_SaleList(SaleList *list, SaleNode sale);

#endif // SALE_H_INCLUDED
