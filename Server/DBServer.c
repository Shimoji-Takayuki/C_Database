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
void getArgument(char *arg, char **tableName, char **procCategory, char **argList);
void splitArgument(char **arg, char **updateList, char **conditionList, int *updateListCount, int *conditionListCount);

int main()
{
    WSADATA w;
    struct sockaddr_in server, client;

    char recvArg[256];
    char *tableName = (char *)malloc(TABLE_NAME_LENGTH + 1);
    char *procCategory;
    char *argList[MAX_ARGUMENT] = {0};
    char *updateList[MAX_COLUMN_NUM * 2] = {0};
    char *conditionList[MAX_COLUMN_NUM * 2] = {0};

    int updateListCount = 0;
    int conditionListCount = 0;

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
        len = sizeof(client);
        int sock2 = accept(sock, (struct sockaddr *)&client, &len);

        recv(sock2, recvArg, sizeof(recvArg), 0);

        getArgument(recvArg, &tableName, &procCategory, argList);

        switch (*procCategory)
        {
        case 'S': // SELECT:検索
            result = tableSelect(tableName, &count);
            break;
        case 'I': // INSERT:挿入
            // argList:テーブルに挿入する各列の値
            result = tableInsert(tableName, argList, &count);
            break;
        case 'U': // UPDATE:更新
                  // argList:更新するテーブルの行ID,更新する列、更新する値
            splitArgument(argList, updateList, conditionList, &updateListCount, &conditionListCount);

            // for (int i = 0; i < sizeof(updateList) / 4; i++)
            // {
            //     printf("%s\n", updateList[i]);
            // }

            result = tableUpdate(tableName, updateList, conditionList, updateListCount, conditionListCount, &count);

            break;
        case 'D': // DELETE:削除
            printf("%s", "DELETEします\n");

            break;
        default:
            printf("%s\n", "失敗！");

            break;
        }

        printf("%s", result);
        // printf("件数:%d\n", count);

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

void getArgument(char *argStr, char **tableName, char **procCategory, char **argList)
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
}

void splitArgument(char **argList, char **updateList, char **conditionList, int *updateListCount, int *conditionListCount)
{
    int i, j = 0;

    for (i = 0; i < MAX_COLUMN_NUM; i++)
    {
        if (strcmp(argList[i], "WHERE") == 0 || !argList[i])
        {
            i++;
            break;
        }
        updateList[i] = argList[i];
    }

    // 更新用の引数の数
    *updateListCount = i - 1;

    for (; i < MAX_COLUMN_NUM; i++)
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
