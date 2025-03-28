#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "purchase.h"
#include "goods.h"
#include "admin.h"
#include "customer.h"
AllCategoryList all_category_list;
PurchaseList purchase_record;
CustomerList customer_list;

int main()
{
	Init_AllCategoryList(&all_category_list);
	Init_PurchaseList(&purchase_record);
	Init_CustomerList(&customer_list);

	// ���ɹ˿���Ϣ�ļ�customerInfo.txt
	CustomerInfo_Generate();

	Print_CustomerList(customer_list);

	// �����˿�Type
	Adjust_Customer_Type();
	Print_CustomerList(customer_list);

	// ��ӹ˿͵�ϵͳ
	Add_Customer_To_System();
	Print_CustomerList(customer_list);

	// ɾ���˿���Ϣ
	Delete_Customer_From_System();
	Print_CustomerList(customer_list);


}
