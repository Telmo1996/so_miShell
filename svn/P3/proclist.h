#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXL 4096


typedef struct procNode_t {
	char* commandName;
	char* fecha;
	int prio;
	pid_t pid;
	char finished;
	char* state;
	int exitStatus;
	char* returned;
} procNode_t;

typedef struct lista_t{
	procNode_t nodos[MAXL];
	int lastpos;
}lista_t;

typedef lista_t *List;

List procLista;

void procCreateList(List);
char procInsertElement(char*, pid_t, int, List);
void procRemoveElement(int, List);
