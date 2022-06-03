#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void stoptcp(char *buf, int sock, int sock0)
{
  puts("disconnected...");

  close(sock);

  close(sock0);
}

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  char buf[1024];
  char *quit = "quit\n";

  /* ソケットの作成 */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);

  /* ソケットの設定 */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

  /* TCPクライアントからの接続要求を待てる状態にする */
  listen(sock0, 5);

  /* TCPクライアントからの接続要求を受け付ける */
  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr *)&client, &len);

  puts("connected");

  while (1)
  {

    printf("message > ");

    fgets(buf, 1024, stdin);

    write(sock, buf, sizeof(buf));

    if (strcmp(buf, quit) == 0)
    {
      stoptcp(buf, sock, sock0);

      return 0;
    }

    memset(buf, 0, sizeof(buf));

    read(sock, buf, sizeof(buf));

    if (strcmp(buf, quit) == 0)
    {
      stoptcp(buf, sock, sock0);

      return 0;
    }

    printf("%s\n", buf);
  }

  /* TCPセッションの終了 */
  close(sock);

  /* listen するsocketの終了 */
  close(sock0);

  return 0;
}
