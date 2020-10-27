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
#include <regex.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAXLINEA 4095
char linea[MAXLINEA+1];
char *trozos[200];
char *args[50];

typedef struct node {
    struct node * next;
    char *val;
} node_t;

node_t * lista;


struct datoCmd {
    char *nombre;
    int (*fun)(int argc, char *argv[]);
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
    current->next->val = strdup(valor);
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
	free(current->next->val);
    free(current->next);
    current->next = NULL;

}

void print_list(node_t * head, int hasta) {
    node_t * current = head->next;
	int i = 1;

	if(hasta < 0){
		printf("n < 0\n");
		return;
	}

    while (current != NULL) {
        printf("%d %s", i,  current->val);
        current = current->next;
		if(i == hasta)
			break;
		i++;
    }
}

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

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

int mi_regex(char * regexString, char * cadena){
    regex_t regex;
    int reti;
    char msgbuf[100];

    reti = regcomp(&regex, regexString, 0);
    if (reti){
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
    }

    reti = regexec(&regex, cadena, 0, NULL, 0);
    if (!reti) {
        return(1);
    }
    else if (reti == REG_NOMATCH) {
        return(0);
    }
    else {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    exit(1);
    }
    regfree(&regex);

}

int cmdCreate(int argc, char *argv[]){
    char opD=0, opList=0, opC=0;
	char * name;
    DIR *dirp;
    struct dirent *direntp;
	FILE *fp;
	struct stat st = {0};

    if(argc==1){
        opList=1;
    }else{
        if(argv[1] != NULL){
            if(strcmp(argv[1], "-dir")==0){
                opD=1;
				if(argv[2] != NULL){
					name = argv[2];
					opC=1;
				}else{
					opList=1;
				}
			}else{
				name = argv[1];
				opC=1;
			}
        }
    }
    
    if(opList){
        dirp = opendir(".");
        if(dirp == NULL){
            printf("error\n");
            exit(2);
        }

        while((direntp = readdir(dirp)) != NULL){
            //TODO ojo con el directorio actual
            printf("%s\n", direntp->d_name);
        }
    }

	if(opC && !opD){
		fp = fopen(name, "a");
		fclose(fp);
	}

	if(opC && opD){
		if (stat(name, &st) == -1) {
			mkdir(name, 0700);
		}
	}

	return 0;    
}

int cmdList(int argc, char *argv[]){
	//esto lo escribe anna


	//esto lo escribe telmo
	printf("hola soy telmo");


	exit 0;
}

int cmdHistoric(int argc, char *argv[]){
	node_t * head = lista;
    node_t * current = head->next;
    char opC=0, opN=0, oprN=0;
    int i, j, n, cmdNum, ntrozos;
	
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
		{"historic",cmdHistoric},
		{"create",cmdCreate},
		{"list", cmdList},
		{NULL, NULL}
	};

    for(i=1; i<argc; i++){
        if(mi_regex("^-[0-9]", argv[i])==1){
			opN=1;
			n = atoi(argv[i] + 1);
		}
		if(mi_regex("^-r[0-9]", argv[i])==1){
			oprN=1;
			n = atoi(argv[i] + 2);
		}
        if (strcmp(argv[i], "-c")==0) opC=1;
    }
	if(opC + opN + oprN > 1){
		printf("Demasiadas opciones\n");
		return 1;
	}
	//printf("c %d n %d rn %d\n", opC, opN, oprN);
    
	if(opC){
		//Borrar	
		while(lista->next!=NULL){
			RemoveElement(lista);
		}
	}else{
		if(opN){
			print_list(lista, n);
		}else if(oprN){
			//Recuperar el comando
			//printf("comando: %s\n",current->val);
			cmdNum=0;
			while(current->next != NULL){ //TODO comprobar que n no se pase
				if(cmdNum == n-1)
					break;
				current = current->next;
				cmdNum++;
			}
			//printf("%d,%d\n", n, cmdNum);
			//printf("comando: %s\n",current->val);
			ntrozos=TrocearCadena(current->val, trozos);
			for (j=0; ;j++){
		        if (tablaComandos[j].nombre==NULL){
		            printf("no entiendo\n");
		            break;
		        }
		        if (strcmp(tablaComandos[j].nombre, trozos[0])==0){
		            tablaComandos[j].fun(ntrozos, trozos);
		            break;
		        }
		    }
		}else{
			print_list(lista, 0);
		}
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
    {"historic",cmdHistoric},
    {"create",cmdCreate},
	{"list", cmdList},
    {NULL, NULL}
};

int main(int argc, char *argv[]) {
    int ntrozos=0,i;
	char* lineaHistoric;
	lista = CreateList();
	char* test = "sooy un test";
/*
    InsertElement("hola uno", lista);
    InsertElement("hola dos", lista);
    InsertElement("hola tres", lista);
    InsertElement("hola cuatro", lista);
    InsertElement("hola cinco", lista);
    RemoveElement(lista);
    print_list(lista, 0);
	
	char* l="-123";

	printf("%d", atoi(l+1));*/
    
    /*if(mi_regex("^-n$", "-n")==1)
        printf("si");
    else
        printf("no");*/

    while (true){
        printf("@");
        printPwd();
        printf("> ");

        if(fgets(linea, MAXLINEA, stdin)==NULL) exit(0);
		InsertElement(linea, lista);
		//printf("%s", lineaHistoric);
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
