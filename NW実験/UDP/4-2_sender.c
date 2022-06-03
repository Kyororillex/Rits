#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    struct addrinfo hints, *res;
    struct in_addr addr;
    int n;
    int errocheck;
    int portnum;
    char ipadd[16];

    portnum = atoi(argv[2]);

    if (argc != 3)
    {
        fprintf(stderr, "Usage : %s hostname portnumber\n", argv[0]);
        return 1;
    }

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

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(portnum);
    inet_pton(AF_INET, ipadd, &server.sin_addr.s_addr);

    n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&server, sizeof(server));
    if (n < 1)
    {
        perror("sendto");
        return 1;
    }

    close(sock);

    return 0;
}
