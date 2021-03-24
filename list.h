#ifndef __LIST_H
#define __LIST_H

enum state {
    STOPPED, RUNNING, DONE
};

struct node {
    int pid;
    char * process_name;
    enum state state;
    struct node * next;
};

struct list{
    struct node * head, *tail;
};

struct list list;
int list_len;

struct node * create_node(int, char *, int );
void add_job(int, char*, int);
int remove_job(int);
void display_joblist();
char * get_state(int);
struct node * getn(int);
struct node * find_job(int);

#endif