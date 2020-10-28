miShell: shell.o funcionesCmd.o list.o
	gcc -o miShell shell.o funcionesCmd.o list.o

shell.o: shell.c
	gcc -c shell.c

funcionesCmd.o: funcionesCmd.c funcionesCmd.h
	gcc -c funcionesCmd.c

list.o: list.h list.c
	gcc -c list.c

clean:
	rm -f miShell *.o
