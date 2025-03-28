#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED

typedef struct CustomerNode// 顾客编号、顾客姓名、顾客类型、顾客联系方式
{
    int id;                // 顾客编号（唯一标识）
    char name[50];        // 顾客姓名
    char type[30];        // 顾客类型（取值为三种，Normal、VIP、SVIP）
    char contact[50];     // 顾客联系方式（如：电话、邮箱等）
    struct CustomerNode *next; // 指向下一个节点的指针
}CustomerNode,*CustomerList;

void Init_CustomerList(CustomerList *list);
void Insert_CustomerList(CustomerList *list, CustomerNode customer);
int Delete_CustomerList(CustomerList *list, int id);
void Print_CustomerList(CustomerList list);

#endif // CUSTOMER_H_INCLUDED
