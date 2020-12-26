#include "proclist.h"

procNode_t * proCreateList(){
	procNode_t * head = NULL;
	head = (procNode_t *) malloc(sizeof(procNode_t));

	head->next = NULL;

	return head;
}

void procInsertElement(int foo, procNode_t * head){
	procNode_t * current = head;

	while(current->next != NULL){
		current = current-> next;
	}
	current->next=(procNode_t *) malloc(sizeof(procNode_t));
	current->next->next=NULL;
	current->next->foo = foo;
}
