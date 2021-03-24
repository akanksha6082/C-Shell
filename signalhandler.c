#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "signalhandler.h"
#include "shell.h"
#include "list.h"

extern int foreground_process_id;
extern char * fg_process_name;


void ctrlc_handler(int signum){
    
    foreground_process_id = -1;
    fg_process_name = NULL;
    
    parent_interrupt = 1;
    return;
}

void ctrlz_handler(int signum){
    
   
    if(foreground_process_id == -1)
        return;

    printf("\n"); 
    
    printf("[%d]+ STOPPED    %s\n", list_len + 1, fg_process_name);

    int pid = foreground_process_id;
    add_job(pid, fg_process_name, 0);

    kill(foreground_process_id, SIGSTOP);

    foreground_process_id = -1;
    fg_process_name = NULL;

    return;
}

void sigchld_handler(int signum){
   
    int status;
    
    int pid = waitpid(-1, &status, WNOHANG);
    
    if(pid == 0 || pid == -1){
        foreground_process_id  = -1;
        return;
    }
  
    struct node * job = find_job(pid);

    if(job == NULL ){
        return;
    }
    else{
        job->state = DONE;
        remove_job(job->pid);
    }
   
    return;
}
