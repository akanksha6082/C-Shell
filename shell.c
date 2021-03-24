
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
#include<pwd.h>

#include "shell.h"
#include "exec.h"
#include "parser.h"
#include "utils.h"
#include "built-ins.h"
#include "job_control.h"



int if_ampersand_cmd;


int parse_cmd(char * cmd){

    char * dup_cmd = strdup(cmd);

    int pipe = check_pipe(cmd);
    int redirection = check_redirection(cmd), ret = 0;
    struct command * parsed_command;
    
    if(pipe && redirection || pipe){
        
        int esac = determine_case(cmd);
        
        if(esac == 1){
            parsed_command = malloc(sizeof(struct command) * MAX_SIZE);
            int pipe_count = parse_pipe(cmd, parsed_command, redirection);
            ret = exec_pipes( dup_cmd, parsed_command, pipe_count);
            
        }
        else{
            parsed_command = malloc(sizeof(struct command) * 1);
            if(esac == 3){ 
                char * token = strsep(&cmd, "|");
                parse_redirection(token, parsed_command, 1);  
            }
            else if(esac == 2){
                char * token = lasttoken(cmd);
                parse_redirection(token, parsed_command, 2);
            }
            ret = exec_redirection(dup_cmd, parsed_command);
          
        } 
    }
    else if(redirection){

        parsed_command = malloc(sizeof(struct command)  * 1);
        parse_redirection(cmd, parsed_command, redirection );
        ret = exec_redirection( dup_cmd, parsed_command);    
    }
    else{
        parsed_command = malloc(sizeof(struct command) * 1);
        parse_space(cmd, parsed_command);
        char * command_name = parsed_command->args[0];
        
        if(strcmp(command_name, "cd") == 0){
            ret = cd(parsed_command->args, parsed_command->length);
        }
        else if(strcmp(command_name, "exit") == 0){
            ret = handle_exit();
        }
        else if(strcmp(command_name, "history") == 0){
        
            ret = handle_history();
        }
        else if(strcmp(command_name, "jobs") == 0){
            ret = jobs();
        }
        else if(strcmp(command_name, "fg") == 0){
            ret = fg_command(parsed_command);
        }
        else if(strcmp(command_name, "bg") == 0){
            ret = bg_command(parsed_command);
        }
        else if(strcmp(command_name, "kill") == 0){
            ret = kill_command(parsed_command);
        }
        else{
            ret =  exec_cmd(dup_cmd, command_name, parsed_command);
        }
    }

    free(parsed_command);
    free(dup_cmd);
    return ret;
}



/* input from user */
char * read_command(){
    int buf_size = 1024;
    char * input = malloc(sizeof(char) * buf_size);
    int i = 0, tflag = 0;
    char ch, last_ch;
    
    if(input == NULL){
        perror("malloc failed\n");
        exit(EXIT_FAILURE);
    }
    
    while( ( ch = getc(stdin)) != '\n'){
        if(ch == EOF){
            printf("\n");
            exit(0);
        }
        if(i > buf_size){
            buf_size *= 2;
            input = realloc(input, buf_size); 
        }
        if(strcmp(&ch,">") == 0 || strcmp(&ch, "<") == 0){
            input[i++] = ' ';
            input[i++] = ch;
            input[i++] = ' ';
        }
            
        else if(ch == '\t'){
            input[i++] = ' ';
        }
        else
            input[i++] = ch;
    }

    ch = input[i-1];
    if(strcmp(&ch,"&") == 0){
        
        if_ampersand_cmd = 1;
        input[i-1] = '\0';
    }
    else{
         input[i] = '\0';
    }
    
    return input;
}


int main(int argc, char **argv){
    int status = 1;
    static char * cmd;
    
    init();
   
    while(status){

        if(parent_interrupt == 1){
            parent_interrupt = 0;
            continue;
        }
        
        if_ampersand_cmd = 0;

        fflush(stdin);
        my_prompt();
       
        cmd = read_command();
       
       
        if(strcmp(cmd, "") != 0){
            history[hlength] = (char*)malloc(sizeof(char) * strlen(cmd) + 1);
            strcpy(history[hlength], cmd);
           
            if(hlength == MAX_SIZE - 1){
                hlength = 0;
            }
            else
                hlength++;
        }
        else
            continue;
        
        int ret_value = parse_cmd(cmd);
        
        free(cmd);
    }
    free(cmd);
    return 0;
}




