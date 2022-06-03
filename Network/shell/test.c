#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#define DATA_SIZE 1024

void main()
{
    char *path;

    path = getenv("PATH");

    printf("%s\n",path);

}