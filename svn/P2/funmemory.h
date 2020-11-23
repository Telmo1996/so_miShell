#include "funcionesCmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memNode {
    struct memNode * next;
    void *puntero;
    char *fecha;
    int tam;
    char tipo; // a->malloc s->shared m->mapped
} memNode_t;

memNode_t * memLista;

memNode_t * memCreateList();
int cmdMemory(int, char**);
int cmdMemdump(int, char**);
int cmdMemfill(int, char**);
int cmdRecurse(int, char**);
int cmdReadfile(int, char**);
int cmdWritefile(int, char**);

