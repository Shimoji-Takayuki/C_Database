#include <stdio.h>
#include <WinSock2.h>
#include "tableOperate.h"
#include "common/common.h"

#define SENDABLE_LENGTH 100000
#define TABLE_NAME_LENGTH 20
#define MAX_ARGUMENT 30
#define MAX_COLUMN_NUM 50
#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

// 引数の分割関数
size_t split(char *s, const char *separator, char **result);
void getArgument(char *arg, char **tableName, char **procCategory, char **argList, int *argListCount);
void splitArgument(char **arg, char **updateList, char **conditionList, int *updateListCount, int *conditionListCount, int argListCount);

int main()
{
    WSADATA w;
    struct sockaddr_in server, client;

    char recvArg[256];
    FILE *fp;
    char *tableName = (char *)malloc(TABLE_NAME_LENGTH + 1);
    char filePath[256];
    char *procCategory;

    WSAStartup(MAKEWORD(2, 0), &w);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9999);
    server.sin_family = AF_INET;

    bind(sock, (struct sockaddr *)&server, sizeof(server));
    listen(sock, 1);

    char *result = '\0';
    int count = 0, len = 0;

    while (1)
    {
        char *argList[MAX_ARGUMENT] = {0};
        char *updateList[MAX_COLUMN_NUM * 2] = {0};
        char *conditionList[MAX_COLUMN_NUM * 2] = {0};
        int updateListCount = 0;
        int conditionListCount = 0;
        int argListCount;

        len = sizeof(client);
        int sock2 = accept(sock, (struct sockaddr *)&client, &len);

        recv(sock2, recvArg, sizeof(recvArg), 0);

        getArgument(recvArg, &tableName, &procCategory, argList, &argListCount);

        // テーブルの存在チェック
        sprintf(filePath, "data/%s/%s.csv", tableName, tableName);
        fp = fopen(filePath, "r");
        if (fp == NULL)
        {
            result = "指定したテーブルは存在しません。\n";
            count = strlen(result);
        }
        else
        {

            switch (*procCategory)
            {

            case 'S': // SELECT:検索
                      // conditionList:取得条件の列と値のペア
                      // conditionListCount:取得条件の数
                splitArgument(argList, updateList, conditionList, &updateListCount, &conditionListCount, argListCount);

                result = tableSelect(tableName, conditionList, conditionListCount, &count);
                break;
            case 'I': // INSERT:挿入
                // argList:テーブルに挿入する各列の値
                result = tableInsert(tableName, argList, &count);
                break;
            case 'U': // UPDATE:更新
                      // updateList:更新される列と値のペア
                      // conditionList:更新条件の列と値のペア
                      // updateListCount:更新対象列の数
                      // conditionListCount:更新条件の数
                splitArgument(argList, updateList, conditionList, &updateListCount, &conditionListCount, argListCount);
                result = tableUpdate(tableName, updateList, conditionList, updateListCount, conditionListCount, &count);

                break;
            case 'D': // DELETE:削除
                      // conditionList:更新条件の列と値のペア
                      // conditionListCount:更新条件の数
                splitArgument(argList, updateList, conditionList, &updateListCount, &conditionListCount, argListCount);
                result = tableDelete(tableName, conditionList, conditionListCount, &count);

                break;
            default:
                // printf("%s\n", "指定した処理区分は無効です。S,I,U,Dのいずれかを指定してください。");
                result = "指定した処理区分は無効です。S,I,U,Dのいずれかを指定してください。\n";
                count = strlen(result);
                break;
            }
        }

        printf("%s", result);

        fclose(fp);

        // クライアント側へ送信
        send(sock2, result, count + 1, 0);

        // 送信した後は初期化
        result = '\0';
        free(result);
        count = 0;

        closesocket(sock2);
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}

/*
getArgument
Description:Client側から渡されたテーブル名と処理区分を取得
*/
void getArgument(char *argStr, char **tableName, char **procCategory, char **argList, int *argListCount)
{
    char *result[MAX_ARGUMENT];
    size_t result_size;

    result_size = split(argStr, ",", result);
    *tableName = result[0];
    *procCategory = result[1];

    for (int i = 2; i < result_size; i++)
    {
        argList[i - 2] = result[i];
    }

    *argListCount = (int)result_size - 2;
}

/*
splitArgument
Description:更新・削除関数の引数作成メソッド
*/
void splitArgument(char **argList, char **updateList, char **conditionList, int *updateListCount, int *conditionListCount, int argListCount)
{
    int i, j = 0;

    for (i = 0; i < argListCount; i++)
    {
        if (strcmpIgnoreCase(argList[i], "WHERE") == 0 || !argList[i])
        {
            i++;
            break;
        }
        updateList[i] = argList[i];
    }

    // 更新用の引数の数
    *updateListCount = i - 1;

    // WHERE句の存在確認
    if (containStr(argList, argListCount, "WHERE") == -1)
    {
        // WHERE句がない場合は処理終了
        return;
    }

    for (; i < argListCount; i++)
    {
        if (!argList[i])
        {
            i++;
            break;
        }

        conditionList[j] = argList[i];
        j++;
    }

    // 更新条件用の引数の数
    *conditionListCount = j;
}
