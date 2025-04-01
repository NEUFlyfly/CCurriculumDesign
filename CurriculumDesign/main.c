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
AllGoodList all_good_list;
PurchaseList purchase_record;
SaleList sale_record;
CustomerList customer_list;

int main()
{
	// 打印一些货物信息，格式是：商品类别 商品名称 商品进价 商品数量 供应商

	TimerStart(); // 启动计时器
	Init_PurchaseList(&purchase_record);
	Init_CustomerList(&customer_list);
	Init_SaleList(&sale_record);
	Init_AllGoodList(&all_good_list);
	while (1)
	{
		system("cls");
		// 打印当前时间
		char current_time[20];
		GetCurrentTime(current_time);
		printf("当前时间：%ss\n", current_time);

		printf("欢迎使用商品管理系统！\n");
		printf("请输入选择：（0：复位系统，1：进货，2：顾客信息管理，3：销售，4：商品信息管理，5：退出）\n");
		int choice1;
		int choice3;
		scanf("%d", &choice1);
		switch (choice1)
		{
		case 0: // 复位系统
			system("cls");
			printf("是否确定复位系统？（0：否，1：是）\n");
			int choice4;
			scanf("%d", &choice4);
			if (choice4 == 1) // 确定复位系统
			{
				ResetSystem(); // 调用复位系统函数
			}
			break;
		case 1: // 进货
			system("cls");
			while (1) // 不断进货，直到输入#退出
			{
				int choice2;
				printf("请输入选择：（1：手动输入进货，2：从文件导入进货，3：退出（两种方法可同时使用））\n");
				scanf("%d", &choice2);
				if (choice2 == 1) // 手动输入进货
				{
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
					printf("\n\n");
					Purchase(category, good_name, cost_price, quantity, supplier);
				}
				else if (choice2 == 2) // 从文件导入进货
				{
					// 读取source.txt文件，将文件中的进货信息导入到系统中
					FILE *fp = fopen("txt/GoodSource.txt", "r");
					if (fp == NULL)
					{
						printf("文件打开失败！\n");
						break; // 打开文件失败，退出循环
					}
					char category[20], good_name[50], supplier[50];
					float cost_price;
					int quantity;
					while (fscanf(fp, "%19s %49s %f %d %49s", category, good_name, &cost_price, &quantity, supplier) != EOF)
					{
						Purchase(category, good_name, cost_price, quantity, supplier);
					}
					fclose(fp); // 关闭文件
					printf("进货成功\n");
					printf("按回车键继续...\n");
					while (getchar() != '\n')
						;	   // 清除输入缓冲区
					getchar(); // 等待用户按下回车键
				}
				else if (choice2 == 3) // 退出
				{
					break; // 退出循环
				}
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
					if (customer_list->next == NULL)
					{
						printf("顾客信息为空！\n");
						break;
					}
					else
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
					int choice4;
					printf("请输入选择：（1：手动输入顾客信息，2：从文件导入顾客信息，3：退出 （两种方法可同时使用））\n");
					scanf("%d", &choice4);
					if (choice4 == 1) // 手动输入顾客信息
						Add_Customer_To_System();
					else if (choice4 == 2) // 从文件导入顾客信息
					{
						FILE *fp = fopen("txt/CustomerSource.txt", "r");
						if (fp == NULL)
						{
							printf("文件打开失败！\n");
							break; // 打开文件失败，退出循环
						}
						char name[20], contact[20], type[50];
						int customer_id;
						int temp = 1;
						while (fscanf(fp, "%19s %19s %49s", name, type, contact) != EOF)
						{
							CustomerNode customer;
							customer.id = temp++;
							strcpy(customer.name, name);
							strcpy(customer.contact, contact);
							strcpy(customer.type, type);
							Insert_CustomerList(&customer_list, customer);
						}
						// 关闭文件
						fclose(fp);
						// 生成顾客信息文件
						Create_CustomerInfo_File();
						printf("顾客信息导入成功\n");
						printf("按回车键继续...\n");
						while (getchar() != '\n')
							;	   // 清除输入缓冲区
						getchar(); // 等待用户按下回车键
					}
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
				Sale_To_Customer(customer_id, good_name, quantity);
				printf("按回车键继续...");
				while (getchar() != '\n')
					;
				// 等待用户按下回车
				getchar(); // 处理多余的换行符
			}
			break;
		case 4:
			while (1)
			// 商品信息管理包括：1.查看商品信息 2.修改商品价格 3.修改商品促销信息 4.删除商品信息 5.查看已有商品类别
			// 6.调整商品类别 7.删除某一个类别的所有商品 100.退出
			{
				int choice2;
				system("cls");
				printf("商品信息管理：\n");
				printf("1.查看商品信息\n");
				printf("2.修改商品价格\n");
				printf("3.修改商品促销信息\n");
				printf("4.删除商品信息\n");
				printf("5.查看已有商品类别\n");
				printf("6.调整商品类别\n");
				printf("7.删除某一个类别的所有商品\n");
				printf("100.退出\n");
				scanf("%d", &choice2);
				switch (choice2)
				{
				case 1:
					system("cls");
					printf("所有商品信息如下：\n");
					// 按回车继续
					Print_AllGoodList(all_good_list);
					printf("按回车键继续...");
					while (getchar() != '\n')
						;	   // 等待用户按下回车
					getchar(); // 处理多余的换行符
					break;
				case 2:
					system("cls");
					Print_AllGoodList(all_good_list);
					printf("已存储的商品信息如上，供参考\n");
					Set_Good_Price();
					break;
				case 3:
					system("cls");
					Set_Good_Promotion_Info();
					break;
				case 4:
					Delete_Good_From_System(); // 函数内完成所有界面功能
					break;
				case 5:
					system("cls");
					View_Good_Category();
					break;
				case 6: // 调整商品类别
					Adjust_Good_Category();
					break;
				case 7:
					Delete_Good_Category();
					break;
				case 100:
					break; // 退出商品信息管理
				default:
					printf("无效的选择，请重新输入！\n");
					break;
				}
				if (choice2 == 100)
					// 如果选择退出商品信息管理，则跳出循环
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
