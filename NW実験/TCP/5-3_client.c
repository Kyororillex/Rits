#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in server;
    struct addrinfo hints, *res;
    struct in_addr addr;
    int sock;
    char buf[1024];
    int n;
    int errocheck;
    int portnum;
    char ipadd[16];

    portnum = atoi(argv[2]);

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    if ((errocheck = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
        printf("error %d\n", errocheck);
        return 1;
    }

    addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
    inet_ntop(AF_INET, &addr, ipadd, sizeof(ipadd));
    printf("接続先ip address : %s\n", ipadd);

    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        printf("%d\n", errno);
        return 1;
    }

    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(portnum);

    /* 127.0.0.1はlocalhost */
    inet_pton(AF_INET, ipadd, &server.sin_addr.s_addr);

    /* サーバに接続 */
    errocheck = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (errocheck < 0)
    {
        perror("connect");
        printf("%d\n", errno);
        return 1;
    }

    read(sock, buf, sizeof(buf));

    if (strcmp(buf, "refused") == 0)
    {
        puts("connection refused");

        puts("このIPアドレスからの接続は許可されていません");

        close(sock);

        freeaddrinfo(res);

        return 0;
    }
    else
    {
        printf("%s\n",buf);

        printf("message >");

        fgets(buf, 1024, stdin);

        write(sock, buf, sizeof(buf));

        read(sock, buf, sizeof(buf));

        printf("%s\n", buf);

        /* socketの終了 */
        close(sock);

        freeaddrinfo(res);

        return 0;
    }
}
