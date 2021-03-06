#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int sock;
    struct addrinfo hints, *res;
    int err, n, fd;
    struct group_req greq;
    char buf[2048];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    err = getaddrinfo(NULL, "12345", &hints, &res);
    if (err != 0)
    {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    sock = socket(res->ai_family, res->ai_socktype, 0);
    bind(sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    err = getaddrinfo("239.144.136.2", NULL, &hints, &res);
    if (err != 0)
    {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    memset(&greq, 0, sizeof(greq));
    greq.gr_interface = 0; /* 任意のネットワークインターフェースを利用 */
    greq.gr_interface = if_nametoindex("en0");

    /* getaddrinfo()の結果をgroup_req構造体へコピー */
    memcpy(&greq.gr_group, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    /* MCAST_JOIN_GROUPを利用してマルチキャストグループへJOIN */
    if (setsockopt(sock, IPPROTO_IP, MCAST_JOIN_GROUP, (char *)&greq, sizeof(greq)) != 0)
    {
        perror("setsockopt");
        return 1;
    }

    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);

    printf("%s\n", buf);

    close(sock);

    return 0;
}
