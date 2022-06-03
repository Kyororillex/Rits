#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#define DATA_SIZE 1024

int *b;
int count = 0;
pid_t c;
int status2;

typedef struct
{
    int num;
    pid_t id;
    char status[206];
    char process[206];
} job_manage;

job_manage manager[20];

void handler(int sig)
{
    printf("receive signal %d\n", (*b)++);

    c = waitpid(-1, &status2, 0);

    for (int i = 0; i < count; i++)
    {
        if (manager[i].id == c)
        {
            strcpy(manager[i].status, "EXITED");
            break;
        }
    }
}

void handler2(int sig)
{

    if (c != 0)
    {
        printf("receive SIGINT %d\n", (*b)++);

        kill(c, SIGINT);
    }
}

int main()
{
    char cmd[DATA_SIZE];
    char *delete = " ";
    char *delete2 = ":";
    char *stop = "exit";
    char *quit = "quit";
    char *and = "&";
    char *fg = "fg";
    char *pargs[DATA_SIZE];
    char *temp;
    char *jobs = "jobs";
    int status;
    int i, k;
    int numm;
    pid_t pid;
    char *path = NULL;
    char *pathque[DATA_SIZE];
    char cppath[DATA_SIZE];
    char join[1024];
    DIR *dp;
    struct dirent *dirp;
    int check = 0;

    int a = 1;
    b = &a;

    path = getenv("PATH");

    printf("%s\n",path);

    strcpy(cppath,path);

    temp = strtok(cppath, delete2);

    for (k = 0; temp != NULL; k++)
    {
        pathque[k] = temp;
        temp = strtok(NULL, delete2);
    }

    signal(SIGCHLD, handler);

    struct sigaction sa;

    if (-1 == sigemptyset(&sa.sa_mask))
    {
        exit(1);
    }

    sa.sa_handler = handler2;
    sa.sa_flags = 0;

    /* シグナルハンドラの登録(エラーチェック付き) */
    if (-1 == sigaction(SIGINT, &sa, NULL))
    {
        exit(1);
    }

    while (1)
    {
        check = 0;
        
        c = 0;

        for (int l = 0; l < DATA_SIZE; l++)
        {
            pargs[l] = NULL;
        }

        printf("> ");

        fgets(cmd, DATA_SIZE, stdin);

        if (cmd[0] != '/')
        {
            check = 1;
        }

        temp = strtok(cmd, delete);

        for (i = 0; temp != NULL; i++)
        {
            pargs[i] = temp;
            temp = strtok(NULL, delete);
        }

        pargs[i - 1] = strtok(pargs[i - 1], "\n");

        if (strcmp(pargs[0], stop) == 0 || strcmp(pargs[0], quit) == 0)
        {
            return 0;
        }

        if (strcmp(pargs[0], jobs) == 0)
        {

            for (int i = 0; i < count; i++)
            {

                printf("num[%d]\nPID[%d]\nstatus:%s\nprocess:%s\n\n", manager[i].num, manager[i].id, manager[i].status, manager[i].process);
            }
        }
        else if (strcmp(pargs[0], fg) == 0)
        {
            if (pargs[i - 1] == NULL)
            {
                wait(&status);
            }
            else
            {

                numm = atoi(pargs[i - 1]);

                for (int i = 0; i < count; i++)
                {
                    if (numm == manager[i].num)
                    {
                        puts("切り替え");
                        strcpy(manager[i].status, "EXITED");
                        strcpy(manager[i].process, "FG");
                        c = manager[i].id;
                        waitpid(manager[i].id, &status, 0);
                    }
                }
            }
        }
        else if (check == 0)
        {

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
                    setpgid(0, 0);

                    execv(pargs[0], pargs);

                    perror(pargs[0]);

                    exit(EXIT_SUCCESS);
                }
                else
                {

                    puts("バックグラウンドで処理中...");

                    manager[count].num = count + 1;
                    manager[count].id = pid;
                    strcpy(manager[count].status, "RUNNING");
                    strcpy(manager[count].process, "BG");

                    count += 1;
                }
            }
            else
            {

                if (pid == 0)
                {
                    setpgid(0, 0);

                    puts("");

                    execv(pargs[0], pargs);

                    perror(pargs[0]);

                    exit(EXIT_SUCCESS);
                }
                else
                {
                    c = pid;

                    waitpid(c, &status, 0);
                }
            }
        }
        else if (check == 1)
        {

            for (int l = 0; l < k; l++)
            {

                dp = opendir(pathque[l]);

                while ((dirp = readdir(dp)) != NULL)
                {

                    if (strcmp(pargs[0], dirp->d_name) == 0)
                    {
                        sprintf(join, "%s/%s", pathque[l], dirp->d_name);

                        pargs[0] = join;

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
                                setpgid(0, 0);

                                execv(pargs[0], pargs);

                                perror(pargs[0]);

                                exit(EXIT_SUCCESS);
                            }
                            else
                            {

                                puts("バックグラウンドで処理中...");

                                manager[count].num = count + 1;
                                manager[count].id = pid;
                                strcpy(manager[count].status, "RUNNING");
                                strcpy(manager[count].process, "BG");

                                count += 1;
                            }
                        }
                        else
                        {

                            if (pid == 0)
                            {
                                setpgid(0, 0);

                                puts("");

                                execv(pargs[0], pargs);

                                perror(pargs[0]);

                                exit(EXIT_SUCCESS);
                            }
                            else
                            {
                                c = pid;

                                waitpid(c, &status, 0);

                                break;
                            }
                        }
                    }
                }

                closedir(dp);
            }
        }
    }
}