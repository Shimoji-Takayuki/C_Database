#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tableOperate.h"
#include "common/common.h"

#define SENDABLE_LENGTH 100
#define MAX_COLUMN_NUM 50
#define LINE_LENGTH 10000
#define READABLE_LENGTH 100000

void getColumnData(char *argStr, char **line);

/*
指定したファイル名の指定されたレコードを変更
複数行の更新、複数列の更新は実装予定
*/
char *tableDelete(char *tblName, char **data, int *count)
{
    static char result[SENDABLE_LENGTH];
    char fileContent[READABLE_LENGTH];
    FILE *inp, *oup, *column;
    char filePathIn[256], filePathColumn[256], filePathOut[256];
    int columnIndex = 0;

    size_t columnSize;
    char readLine[LINE_LENGTH] = {0};
    char *line[MAX_COLUMN_NUM] = {0};
    char *targetColumn;
    char *updatedValue;

    // CSVファイルのディレクトリ
    sprintf(filePathIn, "data/%s/%s.csv", tblName, tblName);
    sprintf(filePathColumn, "data/%s/%s_COLUMN.csv", tblName, tblName);
    sprintf(filePathOut, "data/%s/temp.csv", tblName);

    // 読み込み／書き込み用ファイル
    inp = fopen(filePathIn, "r");
    column = fopen(filePathColumn, "r");
    oup = fopen(filePathOut, "w");

    // 更新対象列の名前を取得
    targetColumn = (char *)malloc(strlen(data[1]) + 1);
    updatedValue = (char *)malloc(strlen(data[2]) + 1);

    strcpy(targetColumn, data[1]);
    strcpy(updatedValue, data[2]);

    // 更新対象列をCOLUMN情報ファイルから検索し、インデックス取得
    while (fgets(readLine, MAX_COLUMN_NUM, column) != NULL)
    {
        // 改行コード除外
        readLine[strlen(readLine) - 1] = '\0';

        if (strcmp(targetColumn, readLine) == 0)
        {
            break;
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

        // 改行を削除
        readLineCopy[strlen(readLineCopy) - 1] = '\0';

        columnSize = split(readLineCopy, ",", line);

        // 更新対象の行をSEQIdで判定
        if (strcmp(*line, data[0]) == 0)
        {
            // 更新対象の列を置換
            char *updatedLine = (char *)malloc(LINE_LENGTH + 1);
            updatedLine[0] = '\0';

            for (int i = 0; i < columnSize; i++)
            {
                if (i == columnIndex)
                {
                    // 更新対象のデータを連結
                    strcat(updatedLine, updatedValue);
                }
                else
                {
                    // それ以外は元のデータをそのまま連結
                    strcat(updatedLine, line[i]);
                    printf("%s\n", line[i]);
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

    // 更新前ファイルを削除して更新後ファイルを変更
    if (remove(filePathIn) == 0)
    {
        // printf("ファイルを削除");
    }
    else
    {
        // printf("削除失敗");
    }

    rename(filePathOut, filePathIn);
    // メモリの開放
    free(targetColumn);
    free(updatedValue);

    // 処理結果を戻り値変数resultへ格納
    strcpy(result, "result_cd:0\n");

    // 処理結果の総バイト数をポインタで渡す
    *count = strlen(result);
    printf("%d\n", *count);

    return result;
}

void getColumnData(char *argStr, char **line)
{
    char *result[MAX_COLUMN_NUM];
    size_t result_size;

    result_size = split(argStr, ",", result);

    for (int i = 0; i < result_size; i++)
    {
        line[i] = result[i];
    }
}
