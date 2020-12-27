#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct procNode {
	struct procNode * next;
	char* commandName;
	pid_t pid;
} procNode_t;

procNode_t * procLista;

procNode_t * procCreateList();
void procInsertElement(char*, pid_t, procNode_t *);

