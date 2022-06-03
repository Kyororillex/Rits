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

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  char buf[32768];

  /* ソケットの作成 */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);

  /* ソケットの設定 */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

  /* TCPクライアントからの接続要求を待てる状態にする */
  listen(sock0, 5);

  /* TCPクライアントからの接続要求を受け付ける */
  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr *)&client, &len);

  puts("connected");

  memset(buf, 0, sizeof(buf));

  read(sock, buf, sizeof(buf));

  puts("文字列\n");
  printf("%s", buf);

  puts("文字コード\n");

  for(int i = 0 ; buf[i] != '\0' ; i++)
  {
    printf("%X",buf[i]);
  }
 
  /* TCPセッションの終了 */
  close(sock);

  /* listen するsocketの終了 */
  close(sock0);

  return 0;
}
