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
	TimerStart(); // ������ʱ��
	Init_AllCategoryList(&all_category_list);
	Init_PurchaseList(&purchase_record);
	Init_CustomerList(&customer_list);
	Init_SaleList(&sale_record);

	while (1)
	{
		system("cls");
		// ��ӡ��ǰʱ��
		char current_time[20];
		GetCurrentTime(current_time);
		printf("��ǰʱ�䣺%s\n", current_time);

		printf("��ӭʹ����Ʒ����ϵͳ��\n");
		printf("������ѡ�񣺣�1��������2���˿���Ϣ����3�����ۣ�4����Ʒ��Ϣ����5���˳���\n");
		int choice;
		int choice3;
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:		  // ����
			while (1) // ���Ͻ�����ֱ������#�˳�
			{
				system("cls");
				char category[20], good_name[50], supplier[50];
				float cost_price;
				int quantity;
				printf("��������Ʒ�������'#'�˳�����");
				scanf("%19s", category);
				if (strcmp(category, "#") == 0)
				{
					break; // ����'#'����ѭ��
				}
				printf("��������Ʒ���ƣ�");
				scanf("%49s", good_name);
				printf("��������Ʒ���ۣ�");
				scanf("%f", &cost_price);
				printf("��������Ʒ������");
				scanf("%d", &quantity);
				printf("�����빩Ӧ�̣�");
				scanf("%49s", supplier);
				printf("�����ɹ�");
				Purchase(category, good_name, cost_price, quantity, supplier);
			}
			break;
		case 2:

			while (1) // �˿���Ϣ���������1.�鿴���й˿���Ϣ 2.���ù˿ͷ��� 3.��ӹ˿���Ϣ 4.ɾ���˿���Ϣ 5.�˳�
			{
				system("cls");
				printf("�˿���Ϣ����\n");
				printf("1.�鿴���й˿���Ϣ\n");
				printf("2.���ù˿ͷ���\n");
				printf("3.��ӹ˿���Ϣ\n");
				printf("4.ɾ���˿���Ϣ\n");
				printf("5.�˳�\n");
				scanf("%d", &choice3);
				switch (choice3)
				{
				case 1:
					system("cls");
					Print_CustomerList(customer_list);
					printf("���س�������...");
					while (getchar() != '\n')
						;	   // �ȴ��û����»س�
					getchar(); // �������Ļ��з�
					break;
				case 2:
					system("cls");
					Print_CustomerList(customer_list);
					printf("�Ѵ洢�Ĺ˿���Ϣ���ϣ����ο�\n");
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
					break; // �˳��˿���Ϣ����
				default:
					printf("��Ч��ѡ�����������룡\n");
					break;
				}
				if (choice3 == 5)
					// ���ѡ���˳��˿���Ϣ����������ѭ��
					break;
			}
			break;
		case 3: // ����
			while (1)
			{
				system("cls");
				char good_name[50];
				int quantity;
				int customer_id;
				Print_CustomerList(customer_list);
				printf("�˿���Ϣ���ϣ����ο�\n");
				printf("������˿�id������'#'�˳�����");
				char id_str[10];
				scanf("%s", id_str);
				if (strcmp(id_str, "#") == 0)
				{
					break; // ����'#'����ѭ��
				}
				customer_id = atoi(id_str); // ������ת��Ϊ����
				if (customer_id <= 0)
				{
					printf("��Ч�Ĺ˿�id�����������룡\n");
					continue; // ��Ч�Ĺ˿�id������ѭ��
				}
				printf("��������Ʒ���ƣ�");
				scanf("%49s", good_name);
				printf("����������������");
				scanf("%d", &quantity);
				// �������ۺ��������˿�id����Ʒ���ƺ������������ݸ�ϵͳ
				Sale(customer_id, good_name, quantity);
				printf("���س�������...");
				while (getchar() != '\n')
					;	   // �ȴ��û����»س�
				getchar(); // �������Ļ��з�
			}
			break;
		case 4:
			while (1) // ��Ʒ��Ϣ���������1.�鿴��Ʒ��Ϣ 2.�޸���Ʒ�۸� 3.�޸���Ʒ������Ϣ 4.ɾ����Ʒ��Ϣ 5.�˳�
			{
				int choice2;
				system("cls");
				printf("��Ʒ��Ϣ����\n");
				printf("1.�鿴��Ʒ��Ϣ\n");
				printf("2.�޸���Ʒ�۸�\n");
				printf("3.�޸���Ʒ������Ϣ\n");
				printf("4.ɾ����Ʒ��Ϣ\n");
				printf("5.�˳�\n");
				scanf("%d", &choice2);
				switch (choice2)
				{
				case 1:
					system("cls");
					printf("������Ʒ��Ϣ���£�\n");
					// ���س�����
					Print_AllCategoryList(all_category_list);
					printf("���س�������...");
					while (getchar() != '\n')
						;	   // �ȴ��û����»س�
					getchar(); // �������Ļ��з�
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
					while (1) // ѭ��ִ��ɾ����Ʒ������ֱ���û�ѡ���˳�
					{
						system("cls");
						char good_name[50];
						Print_AllCategoryList(all_category_list);
						printf("������Ҫɾ������Ʒ���ƣ�����'#'�˳�����");
						scanf("%49s", good_name);
						if (strcmp(good_name, "#") == 0)
						{
							break; // ����'#'�˳�ѭ��
						}
						Delete_Good_From_System(good_name);

						while (getchar() != '\n')
							;	   // �ȴ��û����»س�
						getchar(); // �������Ļ��з�
					}
					break;
				case 5:
					break; // �˳���Ʒ��Ϣ����
				default:
					printf("��Ч��ѡ�����������룡\n");
					break;
				}
				if (choice2 == 5)
					// ���ѡ���˳���Ʒ��Ϣ����������ѭ��
					break;
				break;
			}
			break;
		case 5:
			printf("��лʹ����Ʒ����ϵͳ���ټ���\n");
			exit(0);
		default:
			printf("��Ч��ѡ�����������룡\n");
			break;
		}
	}
}
