#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#define DATA_SIZE 1024

int main()
{
    char cmd[DATA_SIZE];
    char delete[] = " ";
    char *stop = "exit";
    char *quit = "quit";
    char *and = "&";
    char *pargs[DATA_SIZE];
    char *temp;
    int status;
    int i;
    pid_t pid;

    typedef struct
    {
        pid_t id;

    } person_t;

    while (1)
    {

        while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            if (WIFEXITED(status) != 0)
            {
                printf("終了[%d]\n", pid);
            }
        }

        puts("");

        for (int l = 0; l < DATA_SIZE; l++)
        {
            pargs[l] = NULL;
        }

        printf("> ");

        fgets(cmd, DATA_SIZE, stdin);

        temp = strtok(cmd, delete);

        for (i = 0; temp != NULL; i++)
        {
            pargs[i] = temp;
            temp = strtok(NULL, delete);
            printf("%s\n",pargs[i]);
        }

        pargs[i - 1] = strtok(pargs[i - 1], "\n");

        if (strcmp(pargs[0], stop) == 0 || strcmp(pargs[0], quit) == 0)
        {

            return 0;
        }

        pid = fork();

        if (pid == -1)
        {

            exit(EXIT_FAILURE);
        }

        if (strcmp(pargs[i - 1], and) == 0)
        {
            pargs[i - 1] = NULL;

            if (pid == 0)
            {

                execv(pargs[0], pargs);

                perror(pargs[0]);

                exit(EXIT_SUCCESS);
            }
            else
            {
                puts("バックグラウンドで処理中...");
            }
        }
        else
        {

            if (pid == 0)
            {

                puts("");

                execv(pargs[0], pargs);

                perror(pargs[0]);

                exit(EXIT_SUCCESS);
            }
            else
            {
                wait(&status);
            }
        }
    }
}