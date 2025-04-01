#include "goods.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化带头结点的商品链表
void Init_AllGoodList(AllGoodList *list)
{
    *list = (AllGoodList)malloc(sizeof(GoodNode)); // 分配头结点内存
    if (*list == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*list)->next = NULL; // 初始化头结点的指针域为NULL
}

// 插入商品节点到链表中，头插法
void Insert_AllGoodList(AllGoodList *list, GoodNode good)
{
    GoodNode *new_node = (GoodNode *)malloc(sizeof(GoodNode)); // 分配新节点内存
    if (new_node == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    *new_node = good;               // 复制商品信息到新节点
    new_node->next = (*list)->next; // 新节点的指针域指向头结点的下一个节点
    (*list)->next = new_node;       // 头结点的指针域指向新节点
}

// 删除商品链表中的指定商品节点
// 成功返回1，失败返回0
// ... existing code ...
int Delete_AllGoodList(AllGoodList *list, int id)
{
    GoodNode *p = *list; // p指向头结点，这里修改为指向头结点而不是头结点的下一个节点
    while (p->next != NULL)
    { // 遍历链表
        if (p->next->id == id)
        {                             // 如果找到要删除的节点
            GoodNode *temp = p->next; // 临时保存要删除的节点
            p->next = temp->next;     // 修改指针域，跳过要删除的节点
            free(temp);               // 释放内存
            return 1;                 // 删除成功，返回 1
        }
        p = p->next; // 继续遍历下一个节点
    }
    return 0; // 未找到要删除的节点，返回 0
}

// 打印商品链表中的所有商品信息
void Print_AllGoodList(AllGoodList list)
{
    GoodNode *p = list->next; // p指向头结点
    if (p == NULL)
    { // 如果链表为空
        printf("商品信息为空！\n");
        return; // 退出函数
    }
    printf("商品信息如下：\n");
    printf("%-5s %-20s %-15s %-20s %-8s %-6s %-12s %-10s\n", "编号", \
        "名称", "类别", "生产厂家", "价格", "库存", "促销状态", "促销价格");
    while (p != NULL)
    { // 遍历链表，一行一个商品节点
        // 设置商品信息对齐
        printf("%-5d %-20s %-15s %-20s %-8.2f %-6d %-12d %-10.2f\n", p->id, p->good_name, p->category,
               p->manufacturer, p->price, p->stock, p->is_promotion, p->promotion_price);
        p = p->next; // 移动到下一个节点
    }
}