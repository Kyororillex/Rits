#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  int errocheck;

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
  addr.sin_port = htons(12347);
  addr.sin_addr.s_addr = INADDR_ANY;
  errocheck = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
  if (errocheck < 0)
  {
    perror("bind");
    printf("%d\n", errno);
    return 1;
  }

  /* TCPクライアントからの接続要求を待てる状態にする */
  errocheck = listen(sock0, 5);
  if (errocheck < 0)
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

  /* 5文字送信 */
  errocheck = write(sock, "HELLO", 5);
  if (errocheck < 0)
  {
    perror("write");
    printf("%d\n", errno);
    return 1;
  }

  /* TCPセッションの終了 */
  close(sock);

  /* listen するsocketの終了 */
  close(sock0);

  return 0;
}
