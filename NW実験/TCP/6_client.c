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
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in server;
    struct addrinfo hints, *res;
    struct in_addr addr;
    int sock;
    char buf[2048];
    char filenames[1024][2048];
    int n;
    int errocheck;
    int portnum;
    int fd;
    int filecount = 0;
    int filenumber;
    char ipadd[16];

    //ホスト名をIPアドレスへ変換
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
    printf("ip address : %s\n", ipadd);

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

    memset(buf, 0, sizeof(buf));

    puts("Downloadable files");

    //"HELLO"が送られてくるまでサーバからファイル名を受信する
    while ((n = read(sock, buf, sizeof(buf))) > 0)
    {

        if (strcmp(buf, "HELLO") == 0)
        {
            break;
        }

        //IPアドレスが許可されていない場合"refuzed"が送られてくる。
        if (strcmp(buf, "refuse") == 0)
        {
            puts("connection refused");

            close(sock);

            return 0;
        }

        printf("[%d] %-10s   ", filecount, buf);

        //受信したファイル名を配列へ格納
        strcpy(filenames[filecount], buf);

        filecount++;

        if ((filecount % 3) == 0)
        {
            putchar('\n');
        }

        memset(buf, 0, sizeof(buf));
    }

    printf("filenumber >");

    scanf("%d", &filenumber);

    printf("download %s\n", filenames[filenumber]);

    write(sock, filenames[filenumber], sizeof(filenames[filenumber]));

    fd = open(filenames[filenumber], O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd < 0)
    {
        puts("すでに同じ名前のファイルが存在");

        write(sock, "erro", sizeof("erro"));

        close(sock);

        return 1;
    }

    while ((n = read(sock, buf, sizeof(buf))) > 0)
    {
        errocheck = write(fd, buf, n);

        if (errocheck < 1)
        {
            perror("write");
            break;
        }
    }

    /* socketの終了 */
    close(sock);

    return 0;
}
