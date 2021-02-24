#include <stdio.h>
#include <string.h>
#include "tableOperate.h"
#include "common/common.h"

#define LINE_LENGTH 10000
#define SENDABLE_LENGTH 100000
#define MAX_COLUMN_NUM 50

// commonn_func.cで定義済み
int checkCondition(char *readLine, char **conditionList, int *conditionColumnindexList, int conditionCount, char **line);

/*
指定したファイル名の内容を取得して返す
*/
char *tableSelect(char *tableName, char **conditionList, int conditionListCount, int *count)
{
    static char result[SENDABLE_LENGTH];
    char readLine[LINE_LENGTH] = {0};
    char *line[MAX_COLUMN_NUM] = {0};

    FILE *fp, *column;
    char filePath[256], filePathColumn[256];
    int chr;
    int i = 2; //インデックスが0,1の引数はテーブル名、処理区分

    int columnIndex = 0;
    int conditionColumnIndexList[MAX_COLUMN_NUM] = {0};
    int conditionCount = 0;
    int rowCount = 0;

    // 返却文字列を初期化
    result[0] = '\0';

    // CSVファイルのディレクトリ
    sprintf(filePath, "data/%s/%s.csv", tableName, tableName);
    sprintf(filePathColumn, "data/%s/%s_COLUMN.csv", tableName, tableName);

    fp = fopen(filePath, "r");
    column = fopen(filePathColumn, "r");

    strcat(result, "{\n");

    // 取得条件列(WHERE句)をCOLUMN情報ファイルから検索
    while (fgets(readLine, MAX_COLUMN_NUM, column) != NULL)
    {
        // 改行コード除外
        readLine[strlen(readLine) - 1] = '\0';

        for (int i = 0; i < conditionListCount; i += 2)
        {
            if (strcmpIgnoreCase(conditionList[i], readLine) == 0)
            {
                conditionColumnIndexList[i / 2] = columnIndex;

                // 取得条件の列の数をカウント
                conditionCount++;
            }
        }
        columnIndex++;
    }

    fclose(column);

    // ファイルの内容を戻り値変数resultへ格納
    while (fgets(readLine, LINE_LENGTH, fp) != NULL)
    {
        // 元データ保持のため新しい文字列生成
        char readLineCopy[LINE_LENGTH] = {0};
        strcpy(readLineCopy, readLine);

        // 取得対象の行か判定、取得条件がない場合は判定なしで取得
        if (checkCondition(readLineCopy, conditionList, conditionColumnIndexList, conditionCount, line) == 0 || conditionListCount == 0)
        {
            // 取得対象の行は結果に連結していく
            strcat(result, readLine);
        }
        else
        {
            // 取得対象じゃない場合は除外（一行目はヘッダなので含める）
            if (rowCount == 0)
            {
                strcat(result, readLine);
            }
        }

        // 行数インクリメント
        rowCount++;
    }

    // 処理結果を戻り値変数resultへ格納
    strcat(result, "},\n");
    strcat(result, "result_cd:0\n");

    // 処理結果の総バイト数をポインタで渡す
    *count = strlen(result);
    printf("%d\n", *count);

    fclose(fp);

    return result;
}
