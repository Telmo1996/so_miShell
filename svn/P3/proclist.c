#include "proclist.h"

void procCreateList(List* l){
	if(l->nodos[0].pid == -1)
		l->lastpos=-1;
}

void timeProc(char out[128]){
    time_t tiempo=time(0);
    struct tm *tlocal=localtime(&tiempo);
    strftime(out,128,"%H:%M:%S %d/%m/%y", tlocal);
}

char procInsertElement(char* cmdName, pid_t pid, int prio, List * l){
	char out[256];
	char foo[16];

	strcpy(foo, "foo");

	if(l->lastpos == MAXL - 1)
		return 0;
	else{
		timeProc(out);


		//l->nodos[l->lastpos] = (procNode_t *) malloc(sizeof(procNode_t));
		l->nodos[l->lastpos].commandName = strdup(cmdName);
		l->nodos[l->lastpos].fecha = strdup(out);
		l->nodos[l->lastpos].prio = prio;
		l->nodos[l->lastpos].pid = pid;
		l->nodos[l->lastpos].finished = 0;
		l->nodos[l->lastpos].state = strdup(foo);
		l->nodos[l->lastpos].exitStatus = 0;
		l->nodos[l->lastpos].returned = strdup(foo);

		l->lastpos++;

	}
	return 1;
}

void procRemoveElement(int pos, List * l){
	int i, j=0;

	for(i=0; j<=l->lastpos; i++){
		if(i==pos)
			j++;

		l->nodos[i].commandName = l->nodos[j].commandName ;
		l->nodos[i].fecha = l->nodos[j].fecha;
		l->nodos[i].prio = l->nodos[j].prio;
		l->nodos[i].pid = l->nodos[j].pid;
		l->nodos[i].finished = l->nodos[j].finished;
		l->nodos[i].state = l->nodos[j].state;
		l->nodos[i].exitStatus = l->nodos[j].exitStatus;
		l->nodos[i].returned = l->nodos[j].returned;

		j++;
	}
	l->lastpos--;
}
