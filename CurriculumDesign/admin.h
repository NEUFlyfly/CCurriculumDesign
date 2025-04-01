#ifndef ADMIN_H_INCLUDED
#define ADMIN_H_INCLUDED

typedef struct Adminster
{
    char username[50]; // ”√ªß√˚
    char password[50]; // √‹¬Î
} Adminster;

void ResetGoodPurchaseId();
void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Delete_PurchaseRecord(int id);
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void CustomerInfo_Generate();
void Create_CustomerInfo_File();
void Add_Customer_To_System();
void Delete_Customer_From_System();
void Adjust_Customer_Type();
void Delete_Good_From_System();
void Set_Good_Price();
void Set_Good_Promotion_Info();
void Get_Good_Category(char category[][100]);
void View_Good_Category();
void Adjust_Good_Category();
void Delete_Good_Category();
#endif // ADMIN_H_INCLUDED
