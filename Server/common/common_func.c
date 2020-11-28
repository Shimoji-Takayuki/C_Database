#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "common.h"

/*
split
Description:文字列を指定された区切り文字で配列に分ける
*/
size_t split(char *target, const char *separator, char **result)
{
    size_t target_len = strlen(target);
    size_t start = 0;
    size_t end = 0;
    size_t i = 0;

    do
    {
        // 区切り文字でない文字が何文字続いているか調べ、
        // 変数 end に加算。加算後の位置に区切り文字がある。
        end = start + strcspn(&target[start], separator);
        target[end] = '\0';

        // 分割後の文字列の先頭アドレスを result へ格納
        result[i] = &target[start];
        ++i;

        // 次に調べる位置を設定
        start = end + 1;

    } while (start <= target_len); // target の末尾が区切り文字で終わっている場合、
                                   // 最後に空文字列があるとみなさせるためには、
                                   // start <= s_len のときにループを続ける必要がある

    return i;
}

/*
contain
Description:整数型配列の中から指定した値を検索し、そのインデックスを返す
*/
int contain(int *intAry, int count, int target)
{
    int index = -1;

    for (int i = 0; i < count; i++)
    {
        if (intAry[i] == target)
        {
            index = i;
            break;
        }
    }

    return index;
}

/*
getSystemDate
Description:システム日時を取得し、YYYYMMDDHHMISSの形で返す
*/
void getSystemDate(char *systemDate)
{

    time_t timer;
    struct tm *local;
    struct tm *utc;

    /* 現在時刻を取得 */
    timer = time(NULL);

    local = localtime(&timer); /* 地方時に変換 */

    // 年(YYYY)
    char strYear[5];
    itoa(local->tm_year + 1900, strYear, 10);

    // 月(MM)
    char strMonth[3];
    itoa(local->tm_mon + 1, strMonth, 10);

    // 日(DD)
    char strDay[3];
    itoa(local->tm_mday, strDay, 10);

    // 時(HH)
    char strHour[3];
    itoa(local->tm_hour, strHour, 10);

    // 分(MI)
    char strMinute[3];
    itoa(local->tm_min, strMinute, 10);

    // 秒(SS)
    char strSecond[3];
    itoa(local->tm_sec, strSecond, 10);

    // 文字列を連結してシステム日時のフォーマット変換
    sprintf(systemDate, "%04s%02s%02s%02s%02s%02s", strYear, strMonth, strDay, strHour, strMinute, strSecond);
}

/*
checkCondition
Description:行が更新対象かどうか確認
result:0→更新対象
result:1→更新対象外
*/
int checkCondition(char *readLine, char **conditionList, int *conditionColumnindexList, int conditionCount, char **line)
{
    int result = 0;
    size_t columnSize;

    columnSize = split(readLine, ",", line);

    for (int i = 0; i < conditionCount; i++)
    {
        if (strcmp(line[conditionColumnindexList[i]], conditionList[2 * i + 1]) != 0)
        {
            // 更新条件に一致しない値なのでスキップ
            result = 1;
            break;
        }
    }

    return result;
}
