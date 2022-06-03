#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
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
    int fd;
    char buf[65536];
    char ipadd[16];
    int n, ret;
    int erro;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    if ((erro = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
        printf("error %d\n", erro);
        return 1;
    }

    addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
    inet_ntop(AF_INET, &addr, ipadd, sizeof(ipadd));
    printf("ip address : %s\n", ipadd);

    if (argc != 3)
    {
        fprintf(stderr, "Usage : %s hostname file\n", argv[0]);
        return 1;
    }
    

    fd = open(argv[2], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
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
    inet_pton(AF_INET, ipadd, &server.sin_addr.s_addr);

    /* サーバに接続 */
    erro = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (erro < 0)
    {
        perror("connect");
        printf("%d\n", errno);
        return 1;
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
    /* socketの終了 */
    close(sock);

    freeaddrinfo(res);

    return 0;
}
