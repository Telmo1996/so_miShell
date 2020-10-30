all: miShell list

list: list.o funlist.o
	gcc -o list list.o funlist.o

list.o: funlist.h funlist.c list.c
	gcc -c list.c

miShell: shell.o funcionesCmd.o funlist.o
	gcc -o miShell shell.o funcionesCmd.o funlist.o

shell.o: shell.c
	gcc -c shell.c

funcionesCmd.o: funcionesCmd.c funcionesCmd.h
	gcc -c funcionesCmd.c

funlist.o: funlist.h funlist.c
	gcc -c funlist.c

clean:
	rm -f miShell list *.o
