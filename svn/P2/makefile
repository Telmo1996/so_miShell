all: miShell list delete

delete: delete.o fundelete.o funlist.o
	gcc -o delete delete.o fundelete.o funlist.o

delete.o: fundelete.h fundelete.c delete.c
	gcc -c -Wall delete.c

list: list.o funlist.o
	gcc -o list list.o funlist.o

list.o: funlist.h funlist.c list.c
	gcc -c -Wall list.c

miShell: shell.o funcionesCmd.o funlist.o fundelete.o funmemory.o memlist.o
	gcc -o miShell shell.o funcionesCmd.o funlist.o fundelete.o funmemory.o memlist.o

shell.o: shell.c
	gcc -c -Wall shell.c

funcionesCmd.o: funcionesCmd.c funcionesCmd.h
	gcc -c -Wall funcionesCmd.c

funlist.o: funlist.h funlist.c
	gcc -c -Wall funlist.c

funmemory.o: funmemory.h funmemory.c funcionesCmd.h memlist.h
	gcc -c -Wall funmemory.c

memlist.o: memlist.c memlist.h
	gcc -c -Wall memlist.c

clean:
	rm -f miShell list delete *.o
