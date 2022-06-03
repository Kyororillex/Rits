#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    char cmd[1024];
    char *stop = "exit";
    char *quit = "quit";
    char *pargs[2] = {cmd,NULL};
    int status = 0;
    

    while(1)
    {
        printf("> ");

        fgets(cmd,1024,stdin);
        sscanf(cmd,"%s\n",cmd);

        if(strcmp(cmd,stop) == 0 || strcmp(cmd,quit) == 0){

            return 0;
        }

        pid_t pid = fork();

        if (pid == -1) {

            exit(EXIT_FAILURE);

        } 
        
        if (pid == 0){

            execv(cmd,pargs);

            perror(cmd);

            exit(EXIT_SUCCESS);

        } else {

            wait(&status);
        }

    }
    
}