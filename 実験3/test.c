#include <stdio.h>
#include <ctype.h>

#define H2D(x) ((x) <= '9' ? (x) - '0' : (toupper(x)) - 'A' + 10) // 16進数1桁の文字を数値に
// 0-9,a-f,A-Fのみ正しく動作する
static char st1[] = "This is a pen.";
static char st2[] = "I have an apple.";

// 目標：入力だけでこの関数を実行する
void sub2(void)
{
    printf("Bingo!\n");
}

// gets()を使った危険な関数
int sub1(int hint)
{
    char *p = st1;
    char str[] = "Hello, World!!!";
    char buf[32]; // gets()で入力される文字列のバッファ(小さいのですぐ溢れる)
    char *s, *d;

    // ヒントとして各変数や関数のアドレスを出す
    if (hint)
    {
        printf("各変数が格納されているアドレス:\n");
        printf("st1  : %p\n", st1);
        printf("st2  : %p\n", st2);
        printf("d    : %p\n", &d);
        printf("s    : %p\n", &s);
        printf("buf  : %p\n", buf);
        printf("str  : %p\n", str);
        printf("p    : %p\n", &p);
        printf("各関数のアドレス:\n");
        printf("sub1 : %p\n", sub1);
        printf("sub2 : %p\n\n", sub2);
    }

    puts("文字列を入力:");
    gets(buf);

    // bufに入力された文字列に%xxという形式の部分があれば
    // xxを16進数とみなして1バイトの値に置き換える
    s = d = buf; // sは入力文字、dは変換後を指すポインタ
                 // 変換後のデータはbufに上書きされていく
    do
    {
        if (*s == '%')
        {                                            // %があれば
            *d = H2D(*(s + 1)) * 16 + H2D(*(s + 2)); // 16進数2桁の文字を数値に
            s += 3;                                  // %xx
            d++;
        }
        else
        {
            *d++ = *s++;
        }
    } while (*d);

    puts("\n出力文字列:");
    puts(buf);
    puts(str);
    if (hint)
    {
        printf("ヒント：pの値 %p\n", p);
    }
    puts(p);
    return 0; // 返却値はいつも0のはず
}

int main(int argc, char *argv[]) {
  int r;
  char buf[4096]; // バッファオーバーフローしやすいように空間を空けてある
  r = sub1(argc > 1); // 引数があればヒントを出すように
  printf("sub1() returns %d\n", r); // sub1()の返り値表示
  return 0;
}