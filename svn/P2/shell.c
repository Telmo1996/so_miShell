/*
anna.taboada@udc.es     Anna Taboada Pardiñas	20625286J
telmo.fcorujo@udc.es    Telmo Fernandez Corujo	77481943A
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

#include "funcionesCmd.h"
#include "funlist.h"
#include "fundelete.h"
#include "funmemory.h"

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
	{"delete", cmdDelete},
	{"memory", cmdMemory},
	{"memdump", cmdMemdump},
	{"memfill", cmdMemfill},
	{"recurse", cmdRecurse},
	{"readfile", cmdReadfile},
	{"writefile", cmdWritefile},
    {NULL, NULL}
};

int main(int argc, char *argv[]) {
    int ntrozos=0,i;
	lista = CreateList();
	memLista = memCreateList();

    while (true){
        printf("@");
        printPwd();
        printf("> ");

        if(fgets(linea, MAXLINEA, stdin)==NULL) exit(0);
		if(strcmp("\n", linea)!=0){
			InsertElement(linea, lista);
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
		}else{
			printf("escribe algo\n");
		}
    }

	free(lista);
	memDeleteList(memLista);
	free(memLista);

    return 0;
}
