#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pwd.h>
#include<unistd.h>
#include<signal.h>


#include "built-ins.h"

/*implementing built-in command : cd */
int cd(char ** args, int count){
    
    char * home = NULL;
    int st;
    if( count == 1){
        home = getenv("HOME");
    }
    else if(strcmp(args[1], "~") == 0){
        home = getenv("HOME");
    }

    if(home != NULL)
        st = chdir(home);
    else
        st = chdir(args[1]);

    if(st != 0){
        fprintf(stderr, "\033[0;31mbash: cd: %s : No such file or directory \033[0m \n", args[1]);
        return -1;
    }
    return 0;
}


int handle_exit(){
    kill(shell_id, SIGKILL);
}

int handle_history(){

   
    if(hlength < SIZE){
        for(int i = 0; i < hlength; i++){
            printf("%d  %s\n", i, history[i]);
        }
    }
    else{
        int start = hlength - SIZE;
        for(int i = start; i < hlength; i++){
            printf("%d  %s\n", i, history[i]);
        }
    }
}   



