#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct memNode {
    struct memNode * next;
    void *puntero;
    char *fecha;
    int tam;
	char *fich;
	int df;
    char tipo; // a->malloc s->shared m->mapped t->todo
} memNode_t;

memNode_t * memLista;

memNode_t * memCreateList();
void memInsertElement(void*, int, char*, int, char, memNode_t *);
void memPrintList(memNode_t *, char);
void memDeleteNode(memNode_t *);
