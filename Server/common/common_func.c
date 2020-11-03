#include <stdio.h>
#include <string.h>
#include "common.h"

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