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

int main()
{
  int sock;
  struct sockaddr_in server;
  struct sockaddr_in senderinfo;
  socklen_t addrlen;
  char buf[2048];
  int n;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
  server.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr *)&server, sizeof(server));

  addrlen = sizeof(senderinfo);
  n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
               (struct sockaddr *)&senderinfo, &addrlen);

  write(fileno(stdout), buf, n);

  puts("\n送り返し");


  sendto(sock,buf, 5, 0, (struct sockaddr *)&senderinfo, sizeof(senderinfo));
    if (n < 1)
    {
        perror("sendto");
        return 1;
    }

  close(sock);

  return 0;
}
