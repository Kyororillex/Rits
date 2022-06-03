#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main()
{
  struct sockaddr_in server;
  int sock;
  char buf[32];
  int n;
  int errocheck;

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
  server.sin_port = htons(12347);

  /* 127.0.0.1はlocalhost */
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  /* サーバに接続 */
  errocheck = connect(-1, (struct sockaddr *)&server, sizeof(server));
  if (errocheck < 0)
  {
    perror("connect");
    printf("%d\n", errno);
    return 1;
  }

  /* サーバからデータを受信 */
  memset(buf, 0, sizeof(buf));
  n = read(sock, buf, sizeof(buf));
  if (n < 0)
  {
    perror("read");
    printf("%d\n", errno);
    return 1;
  }

  printf("%d, %s\n", n, buf);

  /* socketの終了 */
  close(sock);

  return 0;
}
