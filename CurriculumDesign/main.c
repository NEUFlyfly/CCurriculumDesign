#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "system.h"
#include "purchase.h"
#include "goods.h"
#include "admin.h"
#include "sale.h"
#include "customer.h"
#include "salesperson.h"
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

	while (1)
	{
		system("cls");
		// 打印当前时间
		char current_time[20];
		GetCurrentTime(current_time);
		printf("当前时间：%s\n", current_time);

		printf("欢迎使用商品管理系统！\n");
		printf("请输入选择：（1：进货，2：顾客信息管理，3：销售，4：商品信息管理，5：退出）\n");
		int choice;
		int choice3;
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:		  // 进货
			while (1) // 不断进货，直到输入#退出
			{
				system("cls");
				char category[20], good_name[50], supplier[50];
				float cost_price;
				int quantity;
				printf("请输入商品类别（输入'#'退出）：");
				scanf("%19s", category);
				if (strcmp(category, "#") == 0)
				{
					break; // 输入'#'结束循环
				}
				printf("请输入商品名称：");
				scanf("%49s", good_name);
				printf("请输入商品进价：");
				scanf("%f", &cost_price);
				printf("请输入商品数量：");
				scanf("%d", &quantity);
				printf("请输入供应商：");
				scanf("%49s", supplier);
				printf("进货成功");
				Purchase(category, good_name, cost_price, quantity, supplier);
			}
			break;
		case 2:

			while (1) // 顾客信息管理包括：1.查看所有顾客信息 2.设置顾客分类 3.添加顾客信息 4.删除顾客信息 5.退出
			{
				system("cls");
				printf("顾客信息管理：\n");
				printf("1.查看所有顾客信息\n");
				printf("2.设置顾客分类\n");
				printf("3.添加顾客信息\n");
				printf("4.删除顾客信息\n");
				printf("5.退出\n");
				scanf("%d", &choice3);
				switch (choice3)
				{
				case 1:
					system("cls");
					Print_CustomerList(customer_list);
					printf("按回车键继续...");
					while (getchar() != '\n')
						;	   // 等待用户按下回车
					getchar(); // 处理多余的换行符
					break;
				case 2:
					system("cls");
					Print_CustomerList(customer_list);
					printf("已存储的顾客信息如上，供参考\n");
					Adjust_Customer_Type();
					break;
				case 3:
					system("cls");
					Add_Customer_To_System();
					break;
				case 4:
					system("cls");
					Delete_Customer_From_System();
					break;
				case 5:
					break; // 退出顾客信息管理
				default:
					printf("无效的选择，请重新输入！\n");
					break;
				}
				if (choice3 == 5)
					// 如果选择退出顾客信息管理，则跳出循环
					break;
			}
			break;
		case 3: // 销售
			while (1)
			{
				system("cls");
				char good_name[50];
				int quantity;
				int customer_id;
				Print_CustomerList(customer_list);
				printf("顾客信息如上，供参考\n");
				printf("请输入顾客id（输入'#'退出）：");
				char id_str[10];
				scanf("%s", id_str);
				if (strcmp(id_str, "#") == 0)
				{
					break; // 输入'#'结束循环
				}
				customer_id = atoi(id_str); // 将输入转换为整数
				if (customer_id <= 0)
				{
					printf("无效的顾客id，请重新输入！\n");
					continue; // 无效的顾客id，继续循环
				}
				printf("请输入商品名称：");
				scanf("%49s", good_name);
				printf("请输入销售数量：");
				scanf("%d", &quantity);
				// 调用销售函数，将顾客id、商品名称和销售数量传递给系统
				Sale(customer_id, good_name, quantity);
				printf("按回车键继续...");
				while (getchar() != '\n')
					;	   // 等待用户按下回车
				getchar(); // 处理多余的换行符
			}
			break;
		case 4:
			while (1) // 商品信息管理包括：1.查看商品信息 2.修改商品价格 3.修改商品促销信息 4.删除商品信息 5.退出
			{
				int choice2;
				system("cls");
				printf("商品信息管理：\n");
				printf("1.查看商品信息\n");
				printf("2.修改商品价格\n");
				printf("3.修改商品促销信息\n");
				printf("4.删除商品信息\n");
				printf("5.退出\n");
				scanf("%d", &choice2);
				switch (choice2)
				{
				case 1:
					system("cls");
					printf("所有商品信息如下：\n");
					// 按回车继续
					Print_AllCategoryList(all_category_list);
					printf("按回车键继续...");
					while (getchar() != '\n')
						;	   // 等待用户按下回车
					getchar(); // 处理多余的换行符
					break;
				case 2:
					system("cls");
					Set_Good_Price();
					break;
				case 3:
					system("cls");
					Set_Good_Promotion_Info();
					break;
				case 4:
					while (1) // 循环执行删除商品操作，直到用户选择退出
					{
						system("cls");
						char good_name[50];
						Print_AllCategoryList(all_category_list);
						printf("请输入要删除的商品名称（输入'#'退出）：");
						scanf("%49s", good_name);
						if (strcmp(good_name, "#") == 0)
						{
							break; // 输入'#'退出循环
						}
						Delete_Good_From_System(good_name);

						while (getchar() != '\n')
							;	   // 等待用户按下回车
						getchar(); // 处理多余的换行符
					}
					break;
				case 5:
					break; // 退出商品信息管理
				default:
					printf("无效的选择，请重新输入！\n");
					break;
				}
				if (choice2 == 5)
					// 如果选择退出商品信息管理，则跳出循环
					break;
				break;
			}
			break;
		case 5:
			printf("感谢使用商品管理系统，再见！\n");
			exit(0);
		default:
			printf("无效的选择，请重新输入！\n");
			break;
		}
	}
}
