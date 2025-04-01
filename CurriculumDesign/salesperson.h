#ifndef SALESPERSON_H_INCLUDED
#define SALESPERSON_H_INCLUDED

typedef struct Salesperson
{
    char username[20];
    char password[20];
} Salesperson;

void Sale_To_Customer(int customer_id, char* good_name, int quantity);

#endif // SALESPERSON_H_INCLUDED
