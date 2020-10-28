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

char *trozos[200];
char linea[MAXLINEA+1];
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

node_t * CreateList();
int TrocearCadena(char*, char**);
int mi_regex(char*, char*);
void InsertElement(char*, node_t*);
void RemoveElement(node_t*);
void print_list(node_t*,int);
int cmdAutores(int, char**);
int cmdExit(int, char**);
int cmdGetpid(int, char**);
int cmdGetppid(int, char**);
int printPwd();
int cmdPwd(int, char**);
int cmdDate(int, char**);
int cmdTime(int, char**);
int cmdChdir(int, char**);
int cmdCreate(int, char**);
int cmdHistoric(int, char**);