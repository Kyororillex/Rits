#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

struct clientdata
{
  int sock;
  struct sockaddr_in saddr;
};

void *threadfunc(void *data)
{
  int sock;
  struct clientdata *cdata = data;
  char buf[1024];
  int n, i;

  if (data == NULL)
  {
    return (void *)-1;
  }

  /* (5)新規TCPコネクションのソケットを取得 */
  sock = cdata->sock;

  n = read(sock, buf, sizeof(buf));
  if (n < 0)
  {
    perror("read");
    goto err;
  }

  for (i = 0; buf[i] != '\0'; i++)
  {
    if ('a' <= buf[i] && buf[i] <= 'z')
    {
      buf[i] = buf[i] - ('a' - 'A');
    }
    else
    {
      buf[i] = buf[i] + ('a' - 'A');
    }
  }

  buf[i-1] = '\0';

  n = write(sock, buf, n);
  if (n < 0)
  {
    perror("write");
    goto err;
  }

  /* 新規TCPセッションの終了 */
  if (close(sock) != 0)
  {
    perror("close");
    goto err;
  }
  /* 親スレッドでmallocされた領域を開放 */
  free(data);

  return NULL;

err:
  free(data);
  return (void *)-1;
}

int main()
{
  int sock0;
  struct sockaddr_in addr;
  socklen_t len;
  pthread_t th;
  struct clientdata *cdata;

  /* (1) */
  /* ソケットの作成 */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);

  /* ソケットの設定 */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

  /* TCPクライアントからの接続要求を待てる状態にする */
  listen(sock0, 5);
  /* (1) 終わり */

  /* (2)新規TCPコネクションに関する情報をclientdata構造体に格納 */
  for (;;)
  {
    cdata = malloc(sizeof(struct clientdata));
    if (cdata == NULL)
    {
      perror("malloc");
      return 1;
    }

    /* TCPクライアントからの接続要求を受け付ける */
    len = sizeof(cdata->saddr);
    cdata->sock = accept(sock0, (struct sockaddr *)&cdata->saddr, &len);

    /* (3)threadfunc()の処理を新スレッドとして実行 */
    if (pthread_create(&th, NULL, threadfunc, cdata) != 0)
    {
      perror("pthread_create");
      return 1;
    }

    /* (4)親スレッド側で子スレッドをdetach */
    if (pthread_detach(th) != 0)
    {
      perror("pthread_detach");
      return 1;
    }
  }

  /* (8) */
  /* listen するsocketの終了 */
  if (close(sock0) != 0)
  {
    perror("close");
    return 1;
  }

  return 0;
}
