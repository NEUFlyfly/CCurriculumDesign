#ifndef GOODS_H_INCLUDED
#define GOODS_H_INCLUDED

// GoodΪ������Ʒ������Ϣ�ṹ��
typedef struct Good
{
    int id;                // ��Ʒ��ţ�Ψһ��ʶ��
    char good_name[50];    // ��Ʒ����
    char category[30];     // ��Ʒ���
    char manufacturer[50]; // ��������
    char description[100]; // ��Ʒ��������ѡ��
    float price;           // ��ǰ���ۼ۸�
    int stock;             // �������
    int is_promotion;      // �Ƿ������0=��1=�ǣ�
    float promotion_price; // �����۸񣨴���ʱʹ�ã�
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


typedef struct SaleNode
{
    char good_name[20]; // ��Ʒ����
    float sale_price;   // ʵ�����۵��ۣ������ۿۻ������
    char sale_time[20]; // ����ʱ�䣨��ʽ��HH:MM��
    int quantity;       // ��������
    struct SaleNode *next;
} SaleNode, *SaleList;

void Init_CategoryList(CategoryList *list);
void Insert_CategoryList(CategoryList *list, Good good);
void Delete_CategoryList(CategoryList *list, char *good_name);

void Init_AllCategoryList(AllCategoryList *list);
void Insert_AllCategoryList(AllCategoryList *list, CategoryList categorylist, char *category);
void Delete_AllCategoryList(AllCategoryList *list, char *category);
void Print_AllCategoryList(AllCategoryList list);

void Init_SaleList(SaleList *list);
void Insert_SaleList(SaleList *list, SaleNode sale);

#endif // GOODS_H_INCLUDED
