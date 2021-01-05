#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct procNode {
	struct procNode * next;
	char* commandName;
	char* fecha;
	int prio;
	pid_t pid;
	char finished;
	char* state;
	int exitStatus;
	char* returned;
} procNode_t;

procNode_t * procLista;

procNode_t * procCreateList();
void procInsertElement(char*, pid_t, int, procNode_t *);
void procRemoveElement(procNode_t *);
