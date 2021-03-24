#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "shell.h"
#include "exec.h"
#include "background_cmd.h"

int foreground_process_id;
char * fg_process_name;

extern int if_ampersand_cmd;

/*implementation of piped commands */
int exec_pipes(char * cmd, struct command *array, int pipe_count){
    
    int status, p;
    int cmd_count = pipe_count + 1;
    int pfd[cmd_count * 2];
    
    for(int i =0 ; i< cmd_count; i++){
        if(pipe(pfd + 2 * i) == -1){
            fprintf(stderr, "pipe creation failed\n");
            return -1;
        }
        p = fork();
        if(p != 0){
            foreground_process_id = p;
            fg_process_name = cmd;
        }
 
        if(p == -1){
            fprintf(stderr, "cannot fork the child process\n");
            return -1;
        }
        if(p == 0){

            signal(SIGSTOP, SIG_DFL);
            signal(SIGCONT, SIG_DFL);
            
            if(i == 0 && array[i].flag == 0){
                close(0);
                int fd = open(array[i].infilename, O_RDONLY);
            }

            if (i != 0){
                close(0);
                dup2(pfd[2 * (i-1)], 0);
            }
            if(i < cmd_count -1){
                close(1);
                close(pfd[2* i]);
                dup2(pfd[2 * i + 1], 1);
            }

            if(i == cmd_count -1 && array[i].flag == 1){ 
                close(1);
                int fd = open(array[i].outfilename, O_RDONLY | O_WRONLY | O_CREAT, 0644);
            }

            if(execvp(array[i].args[0], array[i].args) == -1){
                fprintf(stderr, "\033[0;31m%s: No such command found\033[0m\n", array[i].args[0]);
                exit(-1);
            }
            exit(0);
        }
        else{
            
            if(i == 0 && array[i].flag == 1){
                return 0;
            }

            if(i != 0){
                close(pfd[2 * (i -1)]);
            }
            close(pfd[2 * i + 1]);
        }
    }
    for( int i = 0; i <cmd_count; i++){
        if(if_ampersand_cmd == 1){
            background_cmd_handler();
        }
           
        else
            waitpid(p, &status, WUNTRACED);
    }
     
    return 0;
}

/*Implementation of redirection commands*/
int exec_redirection( char * cmd, struct command * parsed_command){
    
    int status;
    int flag = parsed_command->flag;
    int p = fork();
    if(p != 0){
        foreground_process_id = p;
        fg_process_name = cmd;
    }

    if(p == -1){
        fprintf(stderr, "cannot fork child process\n");
        return -1;
    }
    if(p == 0){
        
        signal(SIGSTOP, SIG_DFL);
        signal(SIGCONT, SIG_DFL);

        if(flag == 1 || flag == 4){
           close(1);
            int o_fd = open(parsed_command->outfilename, O_TRUNC | O_RDONLY | O_WRONLY | O_CREAT, 0644); 
        }
    
        else if(flag == 2 || flag == 5){
            close(0);
            int in_fd = open(parsed_command->infilename, O_RDONLY);
        }
        else if(flag == 3){
            close(0);
            int in_fd = open(parsed_command->infilename, O_RDONLY);
            close(1);
            int o_fd = open(parsed_command->outfilename, O_TRUNC | O_RDONLY | O_WRONLY | O_CREAT, 0644);
        }
       
        if(execvp(parsed_command->args[0], parsed_command->args) == -1){ 
            fprintf(stderr, "\033[0;31m%s : No such command\033[0m\n", parsed_command->args[0]);
            exit(1);
        }
        exit(0);
    }
    else{

        if(if_ampersand_cmd == 1){
            background_cmd_handler();
        }
        else
            waitpid(p, &status, WUNTRACED);
    }   
    return 0;
}


/*implementation of the system commands*/
int exec_cmd(char * cmd, char * command_name, struct command * parsed_command){
    
    parsed_command->args[parsed_command->length] =NULL;
    int status;
    int pid = fork();

    if(pid !=0 ){
        foreground_process_id = pid;
        fg_process_name = cmd;
       
    }
    
    if(pid == -1){
        fprintf(stderr, "couldnot fork child process\n");
        return -1;
    }
    if(pid == 0){

        signal(SIGSTOP, SIG_DFL);
        signal(SIGCONT, SIG_DFL);

        int ret = execvp(command_name, parsed_command->args);
        if(ret == -1){
            fprintf(stderr, "\033[0;31m%s: No such command\033[0m\n", command_name);
            exit(1);
        }
        exit(0);
    }
    else{
        if(if_ampersand_cmd == 1){
            background_cmd_handler();
        }
        else
            waitpid(pid, &status, WUNTRACED);
        return 0;
    }
    return 0;
}
