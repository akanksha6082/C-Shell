#ifndef __JOBCONTROL_H
#define __JOBCONTROL_H
#include "shell.h"

int jobs();
int fg_command(struct command *);
int bg_command(struct command *);
int kill_command(struct command *);

#endif