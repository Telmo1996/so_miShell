#include "memlist.h"

memNode_t * memCreateList(){
    memNode_t * head = NULL;
    head = (memNode_t *) malloc(sizeof(memNode_t));

    head->puntero = NULL;
    head->fecha = "";
    head->tam = 0;
    head->tipo = '_';
    head->next = NULL;

    return head;
}

void memInsertElement(void *puntero, int tam, char tipo ,memNode_t * head){
    memNode_t * current = head;
    time_t tiempo=time(0);
    struct tm *tlocal=localtime(&tiempo);
    char charTime[128];
    strftime(charTime,128,"%d/%m/%y", tlocal);

    while (current->next != NULL){
        current = current->next;
    }
    current->next=(memNode_t *) malloc(sizeof(memNode_t));
    current->next->next=NULL;
    current->next->puntero = puntero;
    current->next->fecha = charTime;
    current->next->tam = tam;
    current->next->tipo = tipo;
}

void memPrintList(memNode_t * head, char modo) {
    memNode_t * current = head->next;

    while (current != NULL) {
        printf("%p:\tsize:%d\t%c\t%s\n",
            current->puntero, current->tam, current->tipo, current->fecha);
        current = current->next;
    }
}
