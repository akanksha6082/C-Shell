#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"
#include "shell.h"
#include "signalhandler.h"
#include "colors.h"

extern int foreground_process_id;
extern char * fg_process_name;
extern int if_ampersand_cmd;



void init(){

    shell_id = getpid();
    
    parent_interrupt = 0;

    foreground_process_id = -1;

    fg_process_name = NULL;

    if_ampersand_cmd = 0;

    history = (char **)malloc(sizeof(char *) * MAX_SIZE);
    hlength = 0;
    
    signal(SIGINT, ctrlc_handler);

    signal(SIGTSTP, ctrlz_handler);
    
    signal(SIGCHLD, sigchld_handler);
}

void my_prompt(void){
    
    Blue();
    fputs("prompt>> ", stdout);
    Reset();
    
}

/*check for the exsistence of pipes and returns the count*/
int check_pipe(char * cmd){
    if(strstr(cmd, "|") == NULL)
        return 0;
    return 1; 
}

/* checks for redirection and returns the case */
int check_redirection(char * cmd){
    if(strstr(cmd, ">") == NULL &&  strstr(cmd, "<") == NULL)
        return 0;
    else if ( strstr(cmd, ">") != NULL && strstr(cmd, "<") != NULL){
        return 3;
    }
    else if(strstr(cmd, ">") != NULL){
        int count = 0;
        for(int  i = 0; i < strlen(cmd); i++){
            if(cmd[i] == '>') count++;
        }
        if(count == 1)
            return 1;
        else    
            return 4;
    }
    int count = 0;
    for(int i = 0; i<strlen(cmd); i++){
        if(cmd[i] == '<') count++;
    }
    if(count == 1)
        return 2;
    else    
        return 5;
}

char * lasttoken (char * cmd){
    char * token ,*sub_token;
    while((token = strsep(&cmd, "|")) != NULL){
        sub_token = token;
    }
 
    return sub_token;
}

int determine_case(char * cmd){
    char * string = strdup(cmd);
    char * token;
    int index = 0;

    char ** array = malloc(sizeof(char *) * MAX_SIZE);

    while((token = strsep(&string, "|"))!= NULL){
        if(strcmp(token, "") != 0){
            array[index ++] = token;
        }
    }

    char * p1 = strstr(array[0], "<");
    char * p3 = strstr(array[0], ">");

    char * p2 = strstr(array[index -1], ">");
    char * p4 = strstr(array[index -1], "<");
    
    free(array);

    if((p3 == NULL && p4 == NULL) && ( p1 == NULL && p2 == NULL || (p1 != NULL && p2 != NULL) || (p1 != NULL && p2 == NULL) || (p1 == NULL && p2 != NULL))){
        return 1;
    }
    else if(p3 == NULL && p4 != NULL){
        return 2;
    }
    else if(p3 != NULL && p4 == NULL){
        return 3;
    }
    
} 

