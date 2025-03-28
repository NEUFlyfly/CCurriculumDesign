#include "system.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

static time_t start_time; // ���ڴ洢ϵͳ����ʱ��

// ������ʱ������¼��ǰʱ��
void TimerStart() 
{
    start_time = time(NULL); // ��ȡ��ǰʱ���
}

// ��ȡ��ϵͳ��ʼ���е�����������λʮ������������
void GetCurrentTime(char *time_str)
{
    time_t current_time = time(NULL); // ��ȡ��ǰʱ���
    int elapsed_time = (int)(current_time - start_time); // ���㾭��������

    // ȡ����λʮ������
    elapsed_time %= 10000;

    // ��ʽ��Ϊ��λʮ�����ַ���
    snprintf(time_str, 5, "%04d", elapsed_time);
}