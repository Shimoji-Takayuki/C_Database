#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tableOperate.h"
#include "common/common.h"

#define SENDABLE_LENGTH 100
#define MAX_COLUMN_NUM 50
#define LINE_LENGTH 10000
#define READABLE_LENGTH 100000

// commonn_func.cで定義済み
int checkCondition(char *readLine, char **conditionList, int *conditionColumnindexList, int conditionCount, char **line);

/*
指定したファイル名の指定されたレコードを変更
*/
char *tableUpdate(char *tblName, char **updateList, char **conditionList, int updateListCount, int conditionListCount, int *count)
{
    static char result[SENDABLE_LENGTH];
    char fileContent[READABLE_LENGTH];
    FILE *inp, *oup, *column;
    char filePathIn[256], filePathColumn[256], filePathOut[256], filePathBackup[256];
    int columnIndex = 0;
    int argCount = 0;
    int conditionCount = 0;
    int targetColumnIndexList[MAX_COLUMN_NUM] = {0};
    int conditionColumnIndexList[MAX_COLUMN_NUM] = {0};

    size_t columnSize;
    char readLine[LINE_LENGTH] = {0};
    char *line[MAX_COLUMN_NUM] = {0};
    char *targetColumn[MAX_COLUMN_NUM] = {0};
    char *conditionColumn[MAX_COLUMN_NUM] = {0};

    // 更新日時としてシステム日時を取得
    char systemDate[17];
    getSystemDate(systemDate);

    // CSVファイルのディレクトリ
    sprintf(filePathIn, "data/%s/%s.csv", tblName, tblName);
    sprintf(filePathColumn, "data/%s/%s_COLUMN.csv", tblName, tblName);
    sprintf(filePathOut, "data/%s/temp.csv", tblName);
    sprintf(filePathBackup, "data/%s/bk/%s_%s.csv", tblName, tblName, systemDate);

    // 読み込み／書き込み用ファイル
    inp = fopen(filePathIn, "r");
    column = fopen(filePathColumn, "r");
    oup = fopen(filePathOut, "w");

    // 更新列(SET句)をCOLUMN情報ファイルから検索
    while (fgets(readLine, MAX_COLUMN_NUM, column) != NULL)
    {
        // 改行コード除外
        readLine[strlen(readLine) - 1] = '\0';

        for (int i = 0; i < updateListCount; i += 2)
        {
            if (strcmpIgnoreCase(updateList[i], readLine) == 0)
            {
                targetColumnIndexList[i / 2] = columnIndex;

                // 更新対象の列の数をカウント
                argCount++;
            }
        }

        for (int i = 0; i < conditionListCount; i += 2)
        {
            if (strcmpIgnoreCase(conditionList[i], readLine) == 0)
            {
                conditionColumnIndexList[i / 2] = columnIndex;

                // 更新条件の列の数をカウント
                conditionCount++;
            }
        }
        columnIndex++;
    }

    fclose(column);

    // ファイルの内容を全て読み込む
    while (fgets(readLine, LINE_LENGTH, inp) != NULL)
    {
        // 元データ保持のため新しい文字列生成
        char readLineCopy[LINE_LENGTH] = {0};
        strcpy(readLineCopy, readLine);

        // 改行コード除外
        readLineCopy[strlen(readLineCopy) - 1] = '\0';

        // 更新対象の行か判定、更新条件がない場合は判定なしで更新
        if (checkCondition(readLineCopy, conditionList, conditionColumnIndexList, conditionCount, line) == 0 || conditionListCount == 0)
        {

            // 更新対象の列を置換
            char *updatedLine = (char *)malloc(LINE_LENGTH + 1);
            updatedLine[0] = '\0';

            for (int i = 0; i < columnIndex; i++)
            {

                int targetIndex = containInt(targetColumnIndexList, argCount, i);

                if (targetIndex != -1)
                {
                    // 更新対象のデータを連結
                    strcat(updatedLine, updateList[targetIndex * 2 + 1]);
                }
                else
                {
                    // それ以外は元のデータをそのまま連結
                    strcat(updatedLine, line[i]);
                }

                strcat(updatedLine, ",");
            }

            // 末端文字、不要なカンマ（,）を改行コードに置換
            updatedLine[strlen(updatedLine) - 1] = '\n';

            fprintf(oup, updatedLine);
            free(updatedLine);
        }
        else
        {
            // 更新対象の行以外はそのまま書き込み
            fprintf(oup, readLine);
        }
    }

    fclose(inp);
    fclose(oup);

    // 更新前ファイルをリネーム(システム日時を付加)してbkフォルダへ移動
    rename(filePathIn, filePathBackup);
    rename(filePathOut, filePathIn);

    // 処理結果を戻り値変数resultへ格納
    strcpy(result, "result_cd:0\n");

    // 処理結果の総バイト数をポインタで渡す
    *count = strlen(result);
    printf("%d\n", *count);

    return result;
}
