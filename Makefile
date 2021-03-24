target : shell.o utils.o parser.o exec.o built-ins.o colors.o list.o signalhandler.o job_control.o background_cmd.o
		cc shell.o utils.o parser.o exec.o built-ins.o colors.o list.o signalhandler.o job_control.o background_cmd.o -o shell

shell.o: shell.c shell.h exec.h parser.h utils.h built-ins.h
		cc -c shell.c

utils.o : utils.c utils.h shell.h signalhandler.h colors.h
		cc -c utils.c

parser.o : parser.c parser.h shell.h
		cc -c parser.c

exec.o : exec.c exec.h shell.h background_cmd.h
		cc -c exec.c

built-ins.o : built-ins.c built-ins.h shell.h 
		cc -c built-ins.c

colors.o: colors.c colors.h
		cc -c colors.c

list.o: list.c list.h
		cc -c list.c
		
signalhandler.o : signalhandler.c signalhandler.h list.h shell.h
		cc -c signalhandler.c
		
job_control.o : job_control.c shell.h list.h job_control.h
		cc -c job_control.c

background_cmd.o : background_cmd.c background_cmd.h list.h
		cc -c background_cmd.c
clean : 
		rm *.o shell
		
