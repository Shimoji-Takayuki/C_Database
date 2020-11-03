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

    /* 現在時刻を取得 */
    timer = time(NULL);

    local = localtime(&timer); /* 地方時に変換 */

    /* 地方時 変換後表示 */
    printf("地方時: ");
    printf("%4d/", local->tm_year + 1900);
    printf("%2d/", local->tm_mon + 1);
    printf("%2d ", local->tm_mday);
    printf("%2d:", local->tm_hour);
    printf("%2d:", local->tm_min);
    printf("%2d", local->tm_sec);
    printf("%d\n", local->tm_isdst);

    utc = gmtime(&timer); /* 協定世界時(UTC)に変換 */

    /* 協定世界時 変換後表示 */
    printf("協定世界時: ");
    printf("%4d/", utc->tm_year + 1900);
    printf("%2d/", utc->tm_mon + 1);
    printf("%2d ", utc->tm_mday);
    printf("%2d:", utc->tm_hour);
    printf("%2d:", utc->tm_min);
    printf("%2d", utc->tm_sec);
    printf(" %d\n", utc->tm_isdst);

    return EXIT_SUCCESS;
}
