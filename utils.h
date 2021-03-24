#ifndef __UTILS_H
#define  __UTILS_H

void my_prompt(void);
void init();
char * read_command(void);
int check_pipe(char *);
int check_redirection(char *);
int determine_case(char * );
char * lasttoken(char *);

#endif

