#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

#include "list.h"
#include "job_control.h"

extern int foreground_process_id;
extern char * fg_process_name;



int jobs(){
    display_joblist();
    return 0;
}

int bg_command(struct command * array){

    int status, bg_process_id;
    char * str;

    if(array->length > 2){
        fprintf(stderr, "bg : Invalid argumenst\n");
        return -1;    
    }

    if(array->length == 1)
        bg_process_id = list_len;

    else{

        bg_process_id = (int)strtol(&array->args[1][1], &str, 10);
        if(*(str) != '\0'){
            fprintf(stderr, "bg : Invalid argumnets\n");
            return -1;
        }
    }
       
    struct node * job = getn(bg_process_id);
    
    printf("[%d]  %d\n", list_len, job->pid);
    
    if(job != NULL){
        
        job->state = RUNNING;
        kill(job->pid, SIGCONT);
        return 0;
    }
    else{

        fprintf(stderr, "bg : No such job with the given job number\n");
        return -1;  
    }

}

int fg_command(struct command * array){
    
    int status, bg_process_id;
    char * str;

    if (array->length > 2 ){
        fprintf(stderr, "fg : Invalid argumenst\n");
        return -1;
    }
    if(array->length == 1){
        bg_process_id = list_len  + 1;
    }
    else{

        bg_process_id = (int)strtol(&array->args[1][1], &str, 10);
        if(*(str) != '\0'){
            fprintf(stderr, "fg : Invalid argumnets\n");
            return -1;
        }
    }
        
  
    struct node * job = getn(bg_process_id);

    if(job != NULL){

        remove_job(job->pid);

        foreground_process_id = job->pid;
        fg_process_name = job->process_name;

        kill(foreground_process_id, SIGCONT);
       
        waitpid(job->pid, &status, WUNTRACED);
    }
    else{
        fprintf(stderr, "\033[0;31mfg : No such job with the given job number\033[0m\n");
        return -1;
    }
}


int kill_command(struct command * array){
    
    int flag = 0;
    if(array->length != 3){
        fprintf(stderr, "kill : Invalid argumnets\n");
        return -1;
    }
    int process_id, signum;
    char * str;
    signum = (int)strtol(&array->args[1][1], &str, 10);

    if(*(str) != '\0'){
        fprintf(stderr, "kill : Invalid argumnets\n");
        return -1;
    }

    process_id = (int)strtol(&array->args[2][0], &str, 10);

    if(*(str) != '\0'){
        fprintf(stderr, "kill : Invalid argumnets\n");
        return -1;
    }

    struct node * job = find_job(process_id);
    if(job == NULL){
        fprintf(stderr , "\033[0;31mKill : %d no such process\033[0m\n", process_id);
        return -1;
    }
    
   
    if(kill(process_id , signum) == -1){
        fprintf(stderr, "kill : error\n");
        return -1;
    }

    switch (signum)
    {
        case 18:
            job->state = RUNNING;
            break;
        case 19:
            job->state = STOPPED;
            break;
        default:
            break;
    }
    return 0;

}

