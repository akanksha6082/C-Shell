#ifndef __SHELL_H
#define __SHELL_H

#define MAX_SIZE 1024
#define SIZE 10

char ** history;
int hlength;

struct command {
    int length;
    int flag;
    int pos;
    char * infilename;
    char * outfilename;
    int pipe_count;
    char *args[MAX_SIZE];
};

int shell_id;
// int foreground_process_id;
// char * fg_process_name;
int parent_interrupt; 

#endif




