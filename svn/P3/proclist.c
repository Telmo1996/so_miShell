#include "proclist.h"

procNode_t * procCreateList(){
	procNode_t * head = NULL;
	head = (procNode_t *) malloc(sizeof(procNode_t));

	head->next = NULL;
	head->commandName = "";
	head->fecha = "";
	head->prio = 0;
	head->pid = 0;

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
}
