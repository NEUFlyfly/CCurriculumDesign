#ifndef ADMIN_H_INCLUDED
#define ADMIN_H_INCLUDED

typedef struct Adminster
{
    char username[50]; // ”√ªß√˚
    char password[50]; // √‹¬Î
}Adminster;

void Purchase(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Delete_PurchaseRecord(char *good_name);
void Add_PurchaseRecord(char *category, char *good_name, float cost_price, int quantity, char *supplier);
void Adjust_Category(char *good_name, char *new_category);
void CustomerInfo_Generate();
void Add_Customer_To_System();
void Delete_Customer_From_System();
void Adjust_Customer_Type();


#endif // ADMIN_H_INCLUDED
