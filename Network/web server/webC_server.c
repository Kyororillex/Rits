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
#include <sys/stat.h>

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  char buf[8192];
  char *path;
  char *server_root = "/Users/kyoheiyamashita/VSCode/htdocs";
  char fullpath[2048];

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

  puts("connected\n");

  memset(buf, 0, sizeof(buf));

  read(sock, buf, sizeof(buf));

  printf("%s", buf);

  strtok(buf, " ");

  path = strtok(NULL, " ");

  printf("%s\n", path);

  sprintf(fullpath, "%s%s", server_root, path);

  printf("%s\n", fullpath);

  struct stat st;
  int result;

  result = stat(fullpath, &st);
  if (result != 0)
  {
    fprintf(stderr, "%s is not found.\n", fullpath);
    return 1;
  }

  if ((st.st_mode & S_IFMT) == S_IFDIR)
  {
    printf("%s is directory.\n", fullpath);

    sprintf(fullpath, "%s%s", fullpath, "/index.html");

    printf("%s\n",fullpath);

  }
  else
  {
    printf("%s is not directory.\n", fullpath);
  }

  /* TCPセッションの終了 */
  close(sock);

  /* listen するsocketの終了 */
  close(sock0);

  return 0;
}
