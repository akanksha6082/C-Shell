#ifndef __PARSER_H
#define __PARSER_H

#include "shell.h"

int parse_space(char *, struct command *);
int parse_pipe(char *, struct command *, int);
int parse_redirection(char *, struct command *, int);

#endif
