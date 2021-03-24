#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "list.h"

struct node * create_node (int pid, char * name, int state){
    struct node * p = (struct node *)malloc(sizeof(struct node));
    p->pid = pid;
    p->process_name = malloc(sizeof(char) * strlen(name) + 1);
    strcpy(p->process_name, name);
    p->next = NULL;
    p->state = state ? RUNNING : STOPPED;
    
    return p;
}

void add_job(int pid, char * name, int state){
   
    struct node * p = create_node(pid, name, state);
    
    if(list.head == NULL){
        list.head = p;
        list.tail = p;
    }
    else{
        list.tail->next = p;
        list.tail = p;
    }
    list_len ++ ;
}

int remove_job(int pid){
    struct node * p = list.head, *q = NULL; 
    while(p->pid != pid){
        q = p;
        p = p ->next;
    }
    if(p == NULL){
        return -1;
    }

    if(list.head == p)
        list.head = p->next;
    else{
        q->next = p->next;
    }
    if(p->next == NULL && list.head != list.tail)
        list.tail = q;
    
    list_len --;
    return 0;
}

struct node * getn(int n){
    int count  = 1;
    struct node * p = list.head;
    if(n > list_len || n < 0){
        return NULL;
    }
    
    while(count != n){
        count ++;
        p=p->next;             
        
    }
    return p;
}   
char * get_state(int state){
    switch(state){
        case 0:
            return "STOPPED";
        case 1: 
            return "RUNNING";
    }

}

void display_joblist(){
    struct node * p = list.head;
    int count = 1;
    while(p != NULL ){
        printf("[%d] %d  %s - %s\n", count, p->pid, get_state(p->state), p->process_name);
        p = p->next;
        count ++;
    }
}


struct node *  find_job(int pid){
    struct node * p = list.head;
    if(p == NULL)
        return p;
    else{
        while(p != NULL){
            if(p->pid == pid){
                return p;
            }
        }
    }
    return NULL;
}

