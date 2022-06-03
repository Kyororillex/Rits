#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in server;
    int sock;
    int fd;
    char buf[65536];
    char check[2048];
    int n, ret;
    int erro;

    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s filename\n", argv[0]);
        return 1;
    }

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
    server.sin_port = htons(12345);

    /* 127.0.0.1はlocalhost */
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

    /* サーバに接続 */
    erro = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (erro < 0)
    {
        perror("connect");
        printf("%d\n", errno);
        return 1;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd < 0)
    {
        puts("すでに同じ名前のファイルが存在");

        write(sock, "erro", sizeof("erro"));

        close(sock);

        return 1;
    }

    write(sock, argv[1], sizeof(argv[1]));

    while ((n = read(sock, buf, sizeof(buf))) > 0)
    {
        if (strcmp(buf, "erro") == 0)
        {
            printf("%sはサーバに存在しません", argv[1]);

            close(sock);

            remove(argv[1]);

            return 0;
        }

        ret = write(fd, buf, n);
        
        if (ret < 1)
        {
            perror("write");
            break;
        }
    }

    /* socketの終了 */
    close(sock);

    return 0;
}
