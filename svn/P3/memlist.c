#include "memlist.h"

memNode_t * memCreateList(){
    memNode_t * head = NULL;
    head = (memNode_t *) malloc(sizeof(memNode_t));

    head->puntero = NULL;
    head->fecha = "";
    head->tam = 0;
	head->fich = "";
	head->df = 0;
	head->key = 0;
    head->tipo = '_';
    head->next = NULL;

    return head;
}

void timeMemory(char out[128]){
    time_t tiempo=time(0);
	struct tm *tlocal=localtime(&tiempo);
	strftime(out,128,"%H:%M:%S %d/%m/%y", tlocal);
}

void memInsertElement(
	void *puntero, int tam, char* fich, int df, int key, char tipo ,memNode_t * head)
{
    memNode_t * current = head;
    char out[128];
    
    timeMemory(out);
    while (current->next != NULL){
        current = current->next;
    }
    current->next=(memNode_t *) malloc(sizeof(memNode_t));
    current->next->next=NULL;
    current->next->puntero = puntero;
    current->next->fecha = strdup(out);
    current->next->tam = tam;
	current->next->fich = strdup(fich);
	current->next->df = df;
	current->next->key = key;
    current->next->tipo = tipo;
}



void memPrintList(memNode_t * head, char modo) {
    memNode_t * current = head->next;
    char *tipoBonitoA = "malloc";
    char *tipoBonitoS = "shared";
    char *tipoBonitoM = "mapped";

    while (current != NULL) {
        if(current->tipo=='a' || current->tipo=='t'){
        printf("%p:\tsize:%d\t%s\t%s\n",
            current->puntero, current->tam, tipoBonitoA, current->fecha);
        }
        if(current->tipo=='s' || current->tipo=='t'){
        printf("%p:\tsize:%d\t%s\t(key %d)\t%s)\n",
            current->puntero, current->tam, tipoBonitoS,
			current->key, current->fecha);
        }
        if(current->tipo=='m' || current->tipo=='t'){
        printf("%p:\tsize:%d\t%s\t%s\t(fd:%d)\t%s\n",
            current->puntero, current->tam, tipoBonitoM, current->fich,
			current->df, current->fecha);
        }
        current = current->next;
    }
}

void memDeleteNode(memNode_t * previous){	//Borra el nodo siguiente al que se pasa
	memNode_t* current = previous->next;

	//Lincar al siguiente nodo
	previous->next = current->next;

	if(current->tipo == 'a'){
		free(current->puntero);
	}
	if(current->tipo == 's'){
		shmdt(current->puntero);
		//free(current->puntero);
	}
	if(current->tipo == 'm'){
		close(current->df);
		//free(current->puntero);
	}

	//Borrar current
	free(current->fecha);
	free(current->fich);
	free(current);
}

void memDeleteList(memNode_t * current){
	while(current->next != NULL){
		memDeleteNode(current);
	}
}
