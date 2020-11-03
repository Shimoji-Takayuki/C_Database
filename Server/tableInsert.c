#include <stdio.h>
#include <string.h>
#include "tableOperate.h"

#define SENDABLE_LENGTH 100

/*
指定したファイル名に対してレコードを挿入する
*/
char *tableInsert(char *tblName, char **data, int *count)
{
    static char result[SENDABLE_LENGTH];
    FILE *fp;
    char filePath[256];
    int chr;
    int i = 0;

    // CSVファイルのディレクトリ
    sprintf(filePath, "data/%s/%s.csv", tblName, tblName);

    fp = fopen(filePath, "a");

    for (int i = 0; data[i] != NULL; i++)
    {
        if (i != 0)
        {
            fprintf(fp, ",");
        }
        fprintf(fp, data[i]);
    }

    fprintf(fp, "\n");
    fclose(fp);

    strcpy(result, "result_cd:0\n");

    // 処理結果の総バイト数をポインタで渡す
    *count = strlen(result);
    printf("%d\n", *count);

    return result;
}
