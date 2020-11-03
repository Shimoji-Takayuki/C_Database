#include <stdio.h>
#include <string.h>
#include "tableOperate.h"

#define SENDABLE_LENGTH 100000

/*
指定したファイル名の内容を取得して返す
検索条件は実装予定
*/
char *tableSelect(char *tblName, int *count)
{
    static char result[SENDABLE_LENGTH];
    FILE *fp;
    char filePath[256];
    int chr;
    int i = 2; //インデックスが0,1の引数はテーブル名、処理区分

    // CSVファイルのディレクトリ
    sprintf(filePath, "data/%s/%s.csv", tblName, tblName);

    fp = fopen(filePath, "r");

    strcat(result, "{\n");

    // ファイルの内容を戻り値変数resultへ格納
    while ((chr = fgetc(fp)) != EOF)
    {
        result[i] = chr;
        i++;
    }

    /* 終端文字を挿入 */
    result[i] = '\0';

    // 処理結果を戻り値変数resultへ格納
    strcat(result, "},\n");
    strcat(result, "result_cd:0\n");

    // 処理結果の総バイト数をポインタで渡す
    *count = strlen(result);
    printf("%d\n", *count);

    fclose(fp);

    return result;
}
