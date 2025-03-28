#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "system.h"
#include "purchase.h"
#include "goods.h"
#include "admin.h"
#include "customer.h"

AllCategoryList all_category_list;
PurchaseList purchase_record;
SaleList sale_record;
CustomerList customer_list;


int main()
{
	TimerStart(); // 启动计时器
	Init_AllCategoryList(&all_category_list);
	Init_PurchaseList(&purchase_record);
	Init_CustomerList(&customer_list);
	Init_SaleList(&sale_record);

	// 进一批商品
	Purchase("food", "apple", 2.5, 100, "supplier1");
	Purchase("food", "banana", 1.5, 200, "supplier1");
	Purchase("food", "orange", 3.0, 150, "supplier2");
	Purchase("drink", "cola", 2.0, 300, "supplier3");
	Purchase("drink", "milk", 5.0, 100, "supplier4");
	Purchase("drink", "juice", 3.0, 200, "supplier5");
	Purchase("snack", "chocolate", 10.0, 50, "supplier6");
	Purchase("snack", "chips", 6.0, 100, "supplier7");
	Purchase("snack", "biscuit", 5.0, 200, "supplier8");

	Print_AllCategoryList(all_category_list);

	// 设置商品售价
	Set_Good_Price();
	Print_AllCategoryList(all_category_list);

	// 设置商品促销
	Set_Good_Promotion_Info();
	Print_AllCategoryList(all_category_list);

}
