#include "system.h"
#include "sale.h"
#include "purchase.h"
#include "customer.h"
#include "salesperson.h"
#include "goods.h"
#include "admin.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
static time_t start_time; // 用于存储系统启动时间

extern AllGoodList all_good_list;
extern SaleList sale_record;
extern PurchaseList purchase_record;
extern CustomerList customer_list;

// 启动计时器，记录当前时间
void TimerStart()
{
    start_time = time(NULL); // 获取当前时间戳
}

// 获取自系统开始运行的秒数，以四位十进制整数保存
void GetCurrentTime(char *time_str)
{
    time_t current_time = time(NULL);                    // 获取当前时间戳
    int elapsed_time = (int)(current_time - start_time); // 计算经过的秒数

    // 取后四位十进制数
    elapsed_time %= 10000;

    // 格式化为四位十进制字符串
    snprintf(time_str, 5, "%04d", elapsed_time);
}

// 复位系统
void ResetSystem()
{
    // 重置系统状态，清空数据等操作
    // 清空所有链表
    AllGoodList p = all_good_list->next;
    while (p != NULL)
    {
        AllGoodList temp = p;
        p = p->next;
        free(temp);
    }
    all_good_list->next = NULL;

    SaleList s = sale_record->next;
    while (s != NULL)
    {
        SaleList temp = s;
        s = s->next;
        free(temp);
    }
    sale_record->next = NULL;

    PurchaseList pu = purchase_record->next;
    while (pu != NULL)
    {
        PurchaseList temp = pu;
        pu = pu->next;
        free(temp);
    }
    purchase_record->next = NULL;

    CustomerList c = customer_list->next;
    while (c != NULL)
    {
        CustomerList temp = c;
        c = c->next;
        free(temp);
    }
    customer_list->next = NULL;

    Init_AllGoodList(&all_good_list);    // 重新初始化商品链表
    Init_SaleList(&sale_record);         // 重新初始化销售记录链表
    Init_PurchaseList(&purchase_record); // 重新初始化进货记录链表
    Init_CustomerList(&customer_list);   // 重新初始化顾客链表

    // 重置计时器
    TimerStart();

    // 重置全局id变量
    ResetSaleId();
    ResetGoodPurchaseId(); // 重置商品进货id

    // 清空customerInfo，purchaseInfo，saleInfo文件内容
    FILE *fp1 = fopen("txt/customerInfo.txt", "w"); // 打开文件，清空内容
    FILE *fp2 = fopen("txt/purchaseInfo.txt", "w"); // 打开文件，清空内容
    FILE *fp3 = fopen("txt/saleInfo.txt", "w");     // 打开文件，清空内容
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
    { // 如果文件打开失败，输出错误信息并退出
        printf("文件打开失败！\n");
        exit(1); // 退出程序
    }
    fclose(fp1); // 关闭文件
    fclose(fp2); // 关闭文件
    fclose(fp3); // 关闭文件

    printf("系统已复位。\n");
    // 输入回车继续
    getchar();
}