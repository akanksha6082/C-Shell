#ifndef __SIGNAL_H
#define __SIGNAL_H

void ctrlc_handler(int);
void ctrlz_handler(int);
void sigchld_handler(int);

#endif