#ifndef PURCHASE_H_INCLUDED
#define PURCHASE_H_INCLUDED


// 进货record格式：商品名称 进货时间（格式：HH:MM:SS）进货单价 进货数量 供应商名称 商品类别
typedef struct PurchaseNode
{
    char good_name[50];     // 商品名称（冗余存储，方便直接查询）
    char purchase_time[20]; // 进货时间（格式：HH:MM:SS）
    float cost_price;       // 进货单价
    int quantity;           // 进货数量-与Good库存成员关联
    char supplier[50];      // 供应商名称
    char category[50];      // 商品类别
    struct PurchaseNode *next;
} PurchaseNode, *PurchaseList;

void Init_PurchaseList(PurchaseList *list);
void Insert_PurchaseList(PurchaseList *list, PurchaseNode purchase);


#endif // PURCHASE_H_INCLUDED
