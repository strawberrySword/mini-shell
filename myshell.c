#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int process_arglist(int count, char** arglist){
    // simple case:
    int isWait = 0;
    int isPipe = 0;
    int split;
    isWait = strcmp(arglist[count-1], "&");
    if(isWait == 0){
        arglist[count-1] = NULL;
    }

    for(int i =0; i<count; i++){
        if(strcmp(arglist[i], "|") == 0){
            isPipe = 1;
            split = i;
            arglist[i] = NULL;
        }
    }

    if(isPipe){
        // create a pipe
        int pfds[2];
        pipe(pfds);
        pid_t pid = fork();    
        if(pid == 0){
            // change stdout to the pipe and close the pipe read
            close(pfds[0]);
            dup2(pfds[1], STDOUT_FILENO);
            // close(pfds[1]);
            execvp(arglist[0], arglist);
            // this is failed FIX this
            return 1;
        } 
        pid = fork();
        if(pid == 0){
            printf("i am %s\n", arglist[split + 1]);
            // change stdin to the pipe and close the pipe write
            close(pfds[1]);
            dup2(pfds[0], STDIN_FILENO);
            // close(pfds[0]);
            execvp(arglist[split+1], arglist+ split + 1);
            return 1;
        }
        int status = 0;
        if(isWait != 0){
            while(-1 != wait(&status));
        }
        return 1;
        
    }

    pid_t pid = fork();
    
    if(pid == 0){
        execvp(arglist[0], arglist);
        return 1;
    } else {
        int status = 0;
        if(isWait != 0){
            while(-1 != wait(&status));
        }
        return 1;
    }

}

int prepare(void){
    return 0;
}
int finalize(void){
    return 0;
}