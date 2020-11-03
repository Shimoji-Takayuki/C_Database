#include <stdio.h>
#include <WinSock2.h>

#define RECEIVABLE_LENGTH 100000

int main(int argc, char *argv[])
{
    WSADATA w;
    struct sockaddr_in server;

    char sendArg[256];
    char tableName[20];
    char procCategory[1];
    char *result;

    WSAStartup(MAKEWORD(2, 0), &w);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(9999);
    server.sin_family = AF_INET;

    // サーバー側へ送信する文字列
    memset(sendArg, '\0', sizeof(sendArg));
    for (int i = 1; i < argc; i++)
    {
        if (i == 1)
        {
            strcpy(sendArg, argv[i]);
        }
        else
        {
            strcat(sendArg, ",");
            strcat(sendArg, argv[i]);
        }
    }

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    send(sock, sendArg, strlen(sendArg) + 1, 0);

    result = (char *)malloc(RECEIVABLE_LENGTH + 1);
    int data_len = 0; /* 受信済データ長 */
    int recv_len;
    int loop_count = 0;
    for (;;)
    {
        /* 空き容量を算出 */
        int rest_len = RECEIVABLE_LENGTH - data_len;
        if (rest_len <= 0)
        {
            /* 空き無し */
            break;
        }

        /* 格納先の位置を求める */
        char *recv_top = result + data_len;
        recv_len = recv(sock, recv_top, rest_len, 0);
        if (recv_len > 0)
        {
            /* 受信した */
            data_len += recv_len;
            // printf("今回の受信データバイト長=%d, 合計=%d\n", recv_len, data_len);
        }
        else
        {
            break;
        }
    }

    if (recv_len < 0)
    {
        // recv時にエラーが発生した場合
        printf("WSAGetLastError=%d\n", WSAGetLastError());
    }

    /* 終端文字を挿入 */
    *(result + data_len) = '\0';
    printf("受信データ=[\n%s]\n", result);
    free(result);

    closesocket(sock);
    WSACleanup();

    return 0;
}
