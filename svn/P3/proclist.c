#include "proclist.h"

procNode_t * procCreateList(){
	procNode_t * head = NULL;
	head = (procNode_t *) malloc(sizeof(procNode_t));

	head->next = NULL;
	head->commandName = "";
	head->fecha = "";
	head->prio = 0;
	head->pid = 0;
	head->finished = 0;
	head->state = "";
	head->exitStatus = 0;
	head->returned = "";

	return head;
}

void timeProc(char out[128]){
    time_t tiempo=time(0);
    struct tm *tlocal=localtime(&tiempo);
    strftime(out,128,"%H:%M:%S %d/%m/%y", tlocal);
}

void procInsertElement(char* cmdName, pid_t pid, int prio, procNode_t * head){
	procNode_t * current = head;
	char out[128];
	char foo[16] = "foo";

	while(current->next != NULL){
		current = current-> next;
	}

	timeProc(out);

	current->next=(procNode_t *) malloc(sizeof(procNode_t));
	current->next->next=NULL;
	current->next->commandName = strdup(cmdName);
	current->next->fecha = strdup(out);
	current->next->prio = prio;
	current->next->pid = pid;
	current->next->finished = 0;
	current->next->state = strdup(foo);
	current->next->exitStatus = 0;
	current->next->returned = strdup(foo);
}

void procRemoveElement(procNode_t * previous){
	procNode_t* current = previous->next;

	previous->next = current->next;

	free(current->fecha);
	free(current->commandName);
	free(current->state);
	free(current->returned);

	free(current);
}
