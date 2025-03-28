#include "system.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

static time_t start_time; // 用于存储系统启动时间

// 启动计时器，记录当前时间
void TimerStart() 
{
    start_time = time(NULL); // 获取当前时间戳
}

// 获取自系统开始运行的秒数，以四位十进制整数保存
void GetCurrentTime(char *time_str)
{
    time_t current_time = time(NULL); // 获取当前时间戳
    int elapsed_time = (int)(current_time - start_time); // 计算经过的秒数

    // 取后四位十进制数
    elapsed_time %= 10000;

    // 格式化为四位十进制字符串
    snprintf(time_str, 5, "%04d", elapsed_time);
}