#include "proclist.h"

procNode_t * procCreateList(){
	procNode_t * head = NULL;
	head = (procNode_t *) malloc(sizeof(procNode_t));

	head->next = NULL;
	head->commandName = "";
	head->pid = 0;

	return head;
}

void procInsertElement(char* cmdName, pid_t pid, procNode_t * head){
	procNode_t * current = head;

	while(current->next != NULL){
		current = current-> next;
	}
	current->next=(procNode_t *) malloc(sizeof(procNode_t));
	current->next->next=NULL;
	current->next->commandName = strdup(cmdName);
	current->next->pid = pid;
}
