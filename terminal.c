/*
anna.taboada@udc.es     Anna Taboada Pardiñas
telmo.fcorujo@udc.es    Telmo Fernandez Corujo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


#define MAXLINEA 4095
char linea[MAXLINEA+1];
char *trozos[200];
char *args[50];

typedef struct node {
    struct node * next;
    char *val;
} node_t;


struct datoCmd {
    char *nombre;
    int (*fun)(int argc, char *argv[]);
};

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

/*int getArgumentos(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena,"\n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL,"\n\t"))!=NULL)
        i++;
    return i; 
}*/

int cmdAutores(int argc, char *argv[]){
    char opL=0, opN=0;
    int i;
    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-l")) opL=1;
        else if (strcmp(argv[i], "-n")) opN=1;
    }
    if (argc==1) opL=opN=1;

    if (opL) printf("anna.taboada@udc.es\n");
    if (opL) printf("telmo.fcorujo@udc.es\n");
    if (opN) printf("Anna Taboada Pardiñas\n");
    if (opN) printf("Telmo Fernandez Corujo\n");

    return 0;
}

int cmdExit(int argc, char *argv[]){
    exit(0);
}

int cmdGetpid(int argc, char *argv[]){
    pid_t pid;
    pid = getpid();
    printf("%d\n", pid);
    return 0;
}

int cmdGetppid(int argc, char *argv[]){
    pid_t ppid;
    ppid = getppid();
    printf("%d\n", ppid);
    return 0;
}

int printPwd(){
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    else
        printf("%s", cwd);
    
    return 0;
}

int cmdPwd(int argc, char *argv[]){
   printPwd();
   printf("\n");

   return 0;
}

int cmdDate(int argc, char *argv[]){

	time_t tiempo=time(0);
	struct tm *tlocal=localtime(&tiempo);
	char output[128];
	strftime(output,128,"%d/%m/%y", tlocal);
	printf("%s\n",output);

    return 0;
}

int cmdTime(int argc, char *argv[]){
	time_t tiempo=time(0);
	struct tm *tlocal=localtime(&tiempo);
	char out[128];
	strftime(out,128,"%H:%M:%S", tlocal);
	printf("%s\n",out);
    
    return 0;
}

int cmdChdir(int argc, char *argv[]){
    if (argc > 1){
        int ch=chdir(argv[1]);
        if (ch < 0){
            printf("No se pudo cambiar\n");
            return 1;
        }
    }else{
        cmdPwd(argc, argv);
    }

    return 0;
}


struct datoCmd tablaComandos[] = {
    {"autores", cmdAutores},
    {"exit", cmdExit},
    {"quit",cmdExit},
    {"end",cmdExit},
    {"getpid", cmdGetpid},
    {"getppid", cmdGetppid},
    {"pwd", cmdPwd},
    {"date",cmdDate},
    {"time",cmdTime},
    {"chdir",cmdChdir},
    {"cd",cmdChdir},
    {NULL, NULL}
};

node_t * CreateList(){
    node_t * head = NULL;
    head = (node_t *) malloc(sizeof(node_t));
    /*if (head == NULL) {
        return 1;
    }*/

    head->val = "";
    head->next = NULL;

    return head;
}

void InsertElement(char *valor ,node_t * head){
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->val = valor;
    current->next->next = NULL;
}

void RemoveElement(node_t * head) {
    /* if there is only one item in the list, remove it */
    if (head->next == NULL) {
        free(head);
    }

    /* get to the second to last node in the list */
    node_t * current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    /* now current points to the second to last item of the list, so let's remove current->next */
    free(current->next);
    current->next = NULL;

}

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%s\n", current->val);
        current = current->next;
    }
}


int main(int argc, char *argv[]) {
    int ntrozos=0,i;
    node_t * lista = CreateList();

    InsertElement("hola", lista);
    InsertElement("hola", lista);
    RemoveElement(lista);
    print_list(lista);

    while (true){
        printf("@");
        printPwd();
        printf("> ");

        if(fgets(linea, MAXLINEA, stdin)==NULL) exit(0);
        ntrozos=TrocearCadena(linea, trozos);
        for (i=0; ;i++){
            if (tablaComandos[i].nombre==NULL){
                printf("no entiendo\n");
                break;
            }
            if (strcmp(tablaComandos[i].nombre, trozos[0])==0){
                tablaComandos[i].fun(ntrozos, trozos);
                break;
            }
        }
    }

    return 0;
}
