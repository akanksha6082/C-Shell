#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

#include "background_cmd.h"
#include "list.h"

extern int foreground_process_id;
extern char * fg_process_name;

void background_cmd_handler(){

    printf("[%d] %s\n", foreground_process_id, fg_process_name);

    add_job(foreground_process_id, fg_process_name, 1);
   
    foreground_process_id = -1;
    fg_process_name = NULL;
   
}
