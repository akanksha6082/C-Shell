#ifndef __EXEC_H
#define __EXEC_H

int exec_redirection(char * , struct command*);
int exec_pipes(char *, struct command *, int);
int exec_cmd(char *, char *, struct command *);

#endif