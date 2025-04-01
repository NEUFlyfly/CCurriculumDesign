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
	// ��ӡһЩ������Ϣ����ʽ�ǣ���Ʒ��� ��Ʒ���� ��Ʒ���� ��Ʒ���� ��Ӧ��

	TimerStart(); // ������ʱ��
	Init_PurchaseList(&purchase_record);
	Init_CustomerList(&customer_list);
	Init_SaleList(&sale_record);
	Init_AllGoodList(&all_good_list);
	while (1)
	{
		system("cls");
		// ��ӡ��ǰʱ��
		char current_time[20];
		GetCurrentTime(current_time);
		printf("��ǰʱ�䣺%ss\n", current_time);

		printf("��ӭʹ����Ʒ����ϵͳ��\n");
		printf("������ѡ�񣺣�0����λϵͳ��1��������2���˿���Ϣ����3�����ۣ�4����Ʒ��Ϣ����5���˳���\n");
		int choice1;
		int choice3;
		scanf("%d", &choice1);
		switch (choice1)
		{
		case 0: // ��λϵͳ
			system("cls");
			printf("�Ƿ�ȷ����λϵͳ����0����1���ǣ�\n");
			int choice4;
			scanf("%d", &choice4);
			if (choice4 == 1) // ȷ����λϵͳ
			{
				ResetSystem(); // ���ø�λϵͳ����
			}
			break;
		case 1: // ����
			system("cls");
			while (1) // ���Ͻ�����ֱ������#�˳�
			{
				int choice2;
				printf("������ѡ�񣺣�1���ֶ����������2�����ļ����������3���˳������ַ�����ͬʱʹ�ã���\n");
				scanf("%d", &choice2);
				if (choice2 == 1) // �ֶ��������
				{
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
					printf("\n\n");
					Purchase(category, good_name, cost_price, quantity, supplier);
				}
				else if (choice2 == 2) // ���ļ��������
				{
					// ��ȡsource.txt�ļ������ļ��еĽ�����Ϣ���뵽ϵͳ��
					FILE *fp = fopen("txt/GoodSource.txt", "r");
					if (fp == NULL)
					{
						printf("�ļ���ʧ�ܣ�\n");
						break; // ���ļ�ʧ�ܣ��˳�ѭ��
					}
					char category[20], good_name[50], supplier[50];
					float cost_price;
					int quantity;
					while (fscanf(fp, "%19s %49s %f %d %49s", category, good_name, &cost_price, &quantity, supplier) != EOF)
					{
						Purchase(category, good_name, cost_price, quantity, supplier);
					}
					fclose(fp); // �ر��ļ�
					printf("�����ɹ�\n");
					printf("���س�������...\n");
					while (getchar() != '\n')
						;	   // ������뻺����
					getchar(); // �ȴ��û����»س���
				}
				else if (choice2 == 3) // �˳�
				{
					break; // �˳�ѭ��
				}
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
					if (customer_list->next == NULL)
					{
						printf("�˿���ϢΪ�գ�\n");
						break;
					}
					else
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
					int choice4;
					printf("������ѡ�񣺣�1���ֶ�����˿���Ϣ��2�����ļ�����˿���Ϣ��3���˳� �����ַ�����ͬʱʹ�ã���\n");
					scanf("%d", &choice4);
					if (choice4 == 1) // �ֶ�����˿���Ϣ
						Add_Customer_To_System();
					else if (choice4 == 2) // ���ļ�����˿���Ϣ
					{
						FILE *fp = fopen("txt/CustomerSource.txt", "r");
						if (fp == NULL)
						{
							printf("�ļ���ʧ�ܣ�\n");
							break; // ���ļ�ʧ�ܣ��˳�ѭ��
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
						// �ر��ļ�
						fclose(fp);
						// ���ɹ˿���Ϣ�ļ�
						Create_CustomerInfo_File();
						printf("�˿���Ϣ����ɹ�\n");
						printf("���س�������...\n");
						while (getchar() != '\n')
							;	   // ������뻺����
						getchar(); // �ȴ��û����»س���
					}
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
				Sale_To_Customer(customer_id, good_name, quantity);
				printf("���س�������...");
				while (getchar() != '\n')
					;
				// �ȴ��û����»س�
				getchar(); // �������Ļ��з�
			}
			break;
		case 4:
			while (1)
			// ��Ʒ��Ϣ���������1.�鿴��Ʒ��Ϣ 2.�޸���Ʒ�۸� 3.�޸���Ʒ������Ϣ 4.ɾ����Ʒ��Ϣ 5.�鿴������Ʒ���
			// 6.������Ʒ��� 7.ɾ��ĳһ������������Ʒ 100.�˳�
			{
				int choice2;
				system("cls");
				printf("��Ʒ��Ϣ����\n");
				printf("1.�鿴��Ʒ��Ϣ\n");
				printf("2.�޸���Ʒ�۸�\n");
				printf("3.�޸���Ʒ������Ϣ\n");
				printf("4.ɾ����Ʒ��Ϣ\n");
				printf("5.�鿴������Ʒ���\n");
				printf("6.������Ʒ���\n");
				printf("7.ɾ��ĳһ������������Ʒ\n");
				printf("100.�˳�\n");
				scanf("%d", &choice2);
				switch (choice2)
				{
				case 1:
					system("cls");
					printf("������Ʒ��Ϣ���£�\n");
					// ���س�����
					Print_AllGoodList(all_good_list);
					printf("���س�������...");
					while (getchar() != '\n')
						;	   // �ȴ��û����»س�
					getchar(); // �������Ļ��з�
					break;
				case 2:
					system("cls");
					Print_AllGoodList(all_good_list);
					printf("�Ѵ洢����Ʒ��Ϣ���ϣ����ο�\n");
					Set_Good_Price();
					break;
				case 3:
					system("cls");
					Set_Good_Promotion_Info();
					break;
				case 4:
					Delete_Good_From_System(); // ������������н��湦��
					break;
				case 5:
					system("cls");
					View_Good_Category();
					break;
				case 6: // ������Ʒ���
					Adjust_Good_Category();
					break;
				case 7:
					Delete_Good_Category();
					break;
				case 100:
					break; // �˳���Ʒ��Ϣ����
				default:
					printf("��Ч��ѡ�����������룡\n");
					break;
				}
				if (choice2 == 100)
					// ���ѡ���˳���Ʒ��Ϣ����������ѭ��
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
