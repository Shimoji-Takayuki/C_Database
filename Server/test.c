/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* main */
int main(void)
{
    time_t timer;
    struct tm *local;
    struct tm *utc;
    char systemDate[15];

    /* 現在時刻を取得 */
    timer = time(NULL);

    local = localtime(&timer); /* 地方時に変換 */

    // 年(YYYY)
    char strYear[5];
    itoa(local->tm_year + 1900, strYear, 10);
    printf("strYear:%s\n", strYear);
    // 月(MM)
    char strMonth[3];
    itoa(local->tm_mon + 1, strMonth, 10);
    printf("strMonth:%s\n", strMonth);

    // 日(DD)
    char strDay[3];
    itoa(local->tm_mday, strDay, 10);
    printf("strDay:%s\n", strDay);

    // 時(HH)
    char strHour[3];
    itoa(local->tm_hour, strHour, 10);
    printf("strHour:%s\n", strHour);

    // 分(MI)
    char strMinute[3];
    itoa(local->tm_min, strMinute, 10);
    printf("strMinute:%s\n", strMinute);

    // 秒(SS)
    char strSecond[3];
    itoa(local->tm_sec, strSecond, 10);
    printf("strSecond:%s\n", strSecond);

    // システム日時
    sprintf(systemDate, "%04s%02s%02s%02s%02s%02s\n", strYear, strMonth, strDay, strHour, strMinute, strSecond);
    printf("%s", systemDate);

    return EXIT_SUCCESS;
}
