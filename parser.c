#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "parser.h"

int parse_pipe(char * cmd, struct command * array, int redirection){
    char * string = strdup(cmd);
    char * token, *sub_token, *delimiter;
    int pos = 0, cmd_count = 0;
   
    if(redirection != 0) {
        while((token = strsep(&string, "|"))!= NULL){
            if(strcmp(token, "") != 0){
		        struct command * current = &array[cmd_count];
                if(strstr(token, ">") != NULL || strstr(token, "<") != NULL){
                    pos = 0;     
                    while(( sub_token=strsep(&token, " \t") ) != NULL){
                        if(strcmp(sub_token, "") != 0){
                            current->args[pos++] = sub_token;
                        }
                    }
                    if(strcmp(current->args[pos-2],">") == 0){
                      
                        current->flag = 1;
                        current->outfilename = array[cmd_count].args[pos -1];
                        current->infilename = NULL;
                    }
                    else{
                       
                        current->flag = 0;
                        current->infilename = array[cmd_count].args[pos -1];
                        current->outfilename = NULL;
                    }

                    current->args[pos-2] = NULL;
                    current->length = pos;
                }
                else{
                    pos = 0;
                    while(( sub_token= strsep(&token, " \t") ) != NULL){
                        if(strcmp(sub_token, "") != 0){
                            current->args[pos++] = sub_token;
                        }    
                    }
                    current->length = pos - 1;
                    current->flag = -1;
                    current->infilename = NULL;
                    current->outfilename = NULL;
                }
                cmd_count ++;  
	    }
        }
         
    }
    else{    
        while((token = strsep(&string, "|"))!= NULL){
            if(strcmp(token, "") != 0){
		        struct  command * current = &array[cmd_count];
                while(( sub_token= strsep(&token, " \t") ) != NULL){
                    if(strcmp(sub_token, "") != 0){
                        current->args[pos++] = sub_token;
                    }    
                }

                current->length = pos;
                current->infilename = NULL;
                current->outfilename = NULL;
                current->flag = -1;
	
                cmd_count ++;
                pos = 0;
            }
        }
        
    }
    free(string);
    return cmd_count -1;
}


int parse_redirection(char * cmd, struct command * array, int redirection){
    char * token,  *string = strdup(cmd);
    int pos = 0;
    struct command * current = &array[0];

    while(( token = strsep(&string," \t"))!= NULL){
        if(strcmp(token, "") != 0){
            current->args[pos ++] = token;
            
        }
    }
    current->length = pos;        
    current->args[pos - 2] = NULL;
   
    char * file = current->args[pos - 1];
   
    current->flag = redirection;
    switch(redirection){
        case 1:
            current->outfilename = file;
            break;
        case 2:
            current->infilename = file;
            break;
        case 3:
            current->outfilename = file;
            current->args[pos - 4] = NULL;
            current->infilename = current->args[pos - 3];
            break;
        case 4:
            current->outfilename = file;
            current->args[pos - 4] = NULL;
            break;
        case 5:
            current->infilename = file;
            current->args[pos - 4] = NULL;
            break;

    }
    return 0;
}

int parse_space(char * string, struct command * array){
    int pos = 0;    
    char * token;
    while((token = strsep(&string, " \t")) != NULL){
        if(strcmp(token, "") != 0){
            array[0].args[pos++] = token;    
        }
    }
    array[0].length = pos;
    return 0;
}
