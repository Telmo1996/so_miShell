#include <stdio.h>
#include <stdlib.h>

typedef struct procNode {
	struct procNode * next;
	int foo;
} procNode_t;

procNode_t * procLista;

procNode_t * procCreateList();
void procInsertElement(int, procNode_t *);

