all: shell list delete

delete: delete.o fundelete.o funlist.o
	gcc -g -o delete delete.o fundelete.o funlist.o

delete.o: fundelete.h fundelete.c delete.c
	gcc -g -c -Wall delete.c

list: list.o funlist.o
	gcc -g -o list list.o funlist.o

list.o: funlist.h funlist.c list.c
	gcc -g -c -Wall list.c

shell: shell.o funcionesCmd.o funlist.o fundelete.o funmemory.o memlist.o funproc.o proclist.o
	gcc -g -o shell shell.o funcionesCmd.o funlist.o fundelete.o funmemory.o memlist.o funproc.o proclist.o

shell.o: shell.c
	gcc -g -c -Wall shell.c

funcionesCmd.o: funcionesCmd.c funcionesCmd.h
	gcc -g -c -Wall funcionesCmd.c

funlist.o: funlist.h funlist.c
	gcc -g -c -Wall funlist.c

funmemory.o: funmemory.h funmemory.c funcionesCmd.h memlist.h
	gcc -g -c -Wall funmemory.c

memlist.o: memlist.c memlist.h
	gcc -g -c -Wall memlist.c

funproc.o: funproc.h funproc.c proclist.h
	gcc -g -c -Wall funproc.c

proclist.o: proclist.c proclist.h
	gcc -g -c -Wall proclist.c

clean:
	rm -f shell list delete *.o
