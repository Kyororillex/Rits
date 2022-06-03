#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int fd;
    int n, ret;
    char filename[65536];
    int erro;

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
    erro = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if (erro < 0)
    {
        perror("bind");
        printf("%d\n", errno);
        return 1;
    }

    /* TCPクライアントからの接続要求を待てる状態にする */
    erro = listen(sock0, 5);
    if (erro < 0)
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

    memset(filename, 0, sizeof(filename));
    n = read(sock, filename, sizeof(filename));

    printf("%d, %s\n", n, filename);

    if (strcmp(filename, "erro") == 0)
    {
        puts("送信先でファイル名の重複");

        close(sock);

        close(sock0);

        return 0;
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        puts("clientが存在しないファイルを指定");

        write(sock, "erro", sizeof("erro"));

        close(sock);

        close(sock0);

        return 0;
    }

    while ((n = read(fd, filename, sizeof(filename))) > 0)
    {
        ret = write(sock, filename, n);
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
