#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void stoptcp(char *buf, int sock)
{
  puts("disconnected...");

  close(sock);
}

int main()
{
  struct sockaddr_in server;
  int sock;
  char buf[1024];
  char *quit = "quit\n";

  /* ソケットの作成 */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  /* 接続先指定用構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(4000);

  /* 127.0.0.1はlocalhost */
  inet_pton(AF_INET, "172.31.137.79", &server.sin_addr.s_addr);

  /* サーバに接続 */
  connect(sock, (struct sockaddr *)&server, sizeof(server));

  puts("connected");

  while (1)
  {

    /* サーバからデータを受信 */
    memset(buf, 0, sizeof(buf));

    read(sock, buf, sizeof(buf));

    if (strcmp(buf, quit) == 0)
    {
      stoptcp(buf, sock);

      return 0;
    }

    printf("%s\n", buf);

    printf("message > ");

    fgets(buf, 1024, stdin);

    write(sock, buf, sizeof(buf));

    if (strcmp(buf, quit) == 0)
    {
      stoptcp(buf, sock);

      return 0;
    }

  }

  /* socketの終了 */
  close(sock);

  return 0;
}
