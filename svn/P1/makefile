miShell: shell.o funcionesCmd.o
	gcc -o miShell shell.o funcionesCmd.o

shell.o: shell.c
	gcc -c shell.c

funcionesCmd.o: funcionesCmd.c funcionesCmd.h
	gcc -c funcionesCmd.c

clean:
	rm -f miShell *.o
