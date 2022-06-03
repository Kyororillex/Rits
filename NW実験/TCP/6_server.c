#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void lntrim(char *str)
{
    char *p;
    p = strchr(str, '\n');
    if (p != NULL)
    {
        *p = '\0';
    }
}

int main(int argc, char *argv[])
{
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int errocheck;
    int fd,ret,n;
    char buf[2048];
    char str[256];
    char *cmd = "/bin/ls /Users/kyoheiyamashita/VSCode/NW実験/TCP";
    char fname[] = "acceptlist.txt";
    FILE *fp; // FILE型構造体
    int refusecheck = 0;

    /* ソケットの作成 */
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 < 0)
    {
        perror("socket");
        printf("%d\n", errno);
        return 1;
    }

    /* ソケットの設定 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    errocheck = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if (errocheck < 0)
    {
        perror("bind");
        printf("%d\n", errno);
        return 1;
    }

    /* TCPクライアントからの接続要求を待てる状態にする */
    errocheck = listen(sock0, 5);
    if (errocheck < 0)
    {
        perror("listen");
        printf("%d\n", errno);
        return 1;
    }

    /* TCPクライアントからの接続要求を受け付ける */
    len = sizeof(client);

    sock = accept(sock0, (struct sockaddr *)&client, &len);
    if (sock < 0)
    {
        perror("accept");
        printf("%d\n", errno);
        return 1;
    }

    fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
    if (fp == NULL)
    {
        printf("%s file not open!\n", fname);
        return -1;
    }

    while (fgets(str, 256, fp) != NULL)
    {
        int l = 0;

        while (1)
        {
            if (str[l] == '\n')
            {
                str[l] = '\0';
                break;
            }

            l++;
        }

        if (strcmp(str, inet_ntoa(client.sin_addr)) == 0)
        {
            printf("conected :%s\n", str);

            refusecheck = 1;

            fclose(fp);

            break;
        }
    }

    if (refusecheck == 0)
    {
        puts("許可していないIPアドレスからのアクセス");

        strcpy(buf,"refuse");

        write(sock, buf, sizeof(buf));

        close(sock);
        close(sock0);
        return 0;
    }

    if ((fp = popen(cmd, "r")) != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            lntrim(buf);

            write(sock, buf, sizeof(buf));
        }

        pclose(fp);
    }

    /* 5文字送信 */
    errocheck = write(sock, "HELLO", 5);
    if (errocheck < 0)
    {
        perror("write");
        printf("%d\n", errno);
        return 1;
    }

    read(sock, buf, sizeof(buf));

    printf("%sを送信\n",buf);

    fd = open(buf, O_RDONLY);
    if (fd < 0)
    {
        close(sock);

        close(sock0);

        return 0;
    }

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        ret = write(sock, buf, n);
        if (ret < 1)
        {
            perror("write");
            break;
        }
    }

    /* TCPセッションの終了 */
    close(sock);

    /* listen するsocketの終了 */
    close(sock0);

    return 0;
}
