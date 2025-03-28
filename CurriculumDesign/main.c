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

	// 生成顾客信息文件customerInfo.txt
	CustomerInfo_Generate();

	Print_CustomerList(customer_list);
    printf("111");
	// 调整顾客Type
	Adjust_Customer_Type();
	Print_CustomerList(customer_list);

	// 添加顾客到系统
	Add_Customer_To_System();
	Print_CustomerList(customer_list);

	// 删除顾客信息
	Delete_Customer_From_System();
	Print_CustomerList(customer_list);


}
