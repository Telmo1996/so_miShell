#include "funmemory.h"

void * ObtenerMemoriaShmget (key_t clave, size_t tam){
	void * p;
	int aux,id,flags=0777;
	struct shmid_ds s;

	if (tam) /*si tam no es 0 la crea en modo exclusivo */
		flags=flags | IPC_CREAT | IPC_EXCL;
	/*si tam es 0 intenta acceder a una ya creada*/
	if (clave==IPC_PRIVATE) /*no nos vale*/
		{errno=EINVAL; return NULL;}
	if ((id=shmget(clave, tam, flags))==-1)
		return (NULL);
	if ((p=shmat(id,NULL,0))==(void*) -1){
		aux=errno; /*si se ha creado y no se puede mapear*/
		if (tam) /*se borra */
			shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}
	shmctl (id,IPC_STAT,&s);
	/* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
	memInsertElement(p, s.shm_segsz, "", 0, 's', memLista);
	return (p);
}

void Cmd_AlocateCreateShared (char *arg[]){ /*arg[0] is the key and arg[1] is the size*/
	key_t k;
	size_t tam=0;
	void *p;

	if (arg[0]==NULL || arg[1]==NULL){
		/*Listar Direcciones de Memoria Shared */ 
		memPrintList(memLista, 's');
		return;
	}
	k=(key_t) atoi(arg[0]);
	if (arg[1]!=NULL)
		tam=(size_t) atoll(arg[1]);
	if ((p=ObtenerMemoriaShmget(k,tam))==NULL)
		perror ("Imposible obtener memoria shmget");
	else
		printf ("Memoria de shmget de clave %d asignada en %p\n",k,p);
}

void * MmapFichero (char * fichero, int protection){
	int df, map=MAP_PRIVATE,modo=O_RDONLY;
	struct stat s;
	void *p;
	if (protection&PROT_WRITE)  modo=O_RDWR;
	if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
		return NULL;
	if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)return NULL;
	
	/*Guardar Direccion de Mmap (p, s.st_size,fichero,df......);*/
	memInsertElement(p, s.st_size, fichero, df, 'm', memLista);
	return p;
}

void Cmd_AllocateMmap (char *arg[]){ /*arg[0] is the file name
									   arg[1] are the permissions*/
	char *perm;
	void *p;
	int protection=0;
	if (arg[0]==NULL){
		/*Listar Direcciones de Memoria mmap;*/ 
		memPrintList(memLista, 'm');
		return;
	}
	if ((perm=arg[1])!=NULL && strlen(perm)<4) {
		if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
		if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
		if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
	}
	if ((p=MmapFichero(arg[0],protection))==NULL)
		perror ("Imposible mapear fichero");
	else
		printf ("fichero %s mapeado en %p\n", arg[0], p);
}

void Cmd_DeallocMalloc(char* tamChar){
	int tam;
	memNode_t* current=memLista;
	memNode_t* previous=NULL;
	
	if(tamChar==NULL){
		memPrintList(memLista, 'a');
		return;
	}
	tam=atoi(tamChar);
	while(current->next != NULL){
		if(current->tipo == 'a' && current->tam == tam){
			memDeleteNode(previous);
			return;
		}
		previous=current;
		current=current->next;
	}

	memPrintList(memLista, 'a');
}

void Cmd_DeallocMmap(char* fich){
	memNode_t* current=memLista;
	memNode_t* previous=NULL;

	if(fich==NULL){
		memPrintList(memLista, 'm');
		return;
	}
	while(current->next != NULL){
		if(current->tipo == 'm' && (strcmp(current->fich, fich)==0)){
			memDeleteNode(previous);
			return;
		}
		previous=current;
		current=current->next;
	}

	memPrintList(memLista, 'm');
}

int cmdMemory(int argc, char *argv[]){
	char opA=0;		//allocate
	char opD=0;		//dealloc
	char opDlt=0;	//deletekey
	char opS=0;		//show
	char opSv=0;	//show-vars
	char opSf=0;	//show-funcs
	char opDo=0;	//dopmap

	char opMa=0;	//malloc
	char opMm=0;	//mmap
	char opCs=0;	//createshared
	char opSh=0;	//shared
	char opAll=0;	//all

	int i;
	int tam;
	int sumOpMain, sumOpExtra;

	void * puntero;

	char *arg[2];

    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-allocate")==0) opA=1;
        else if (strcmp(argv[i], "-dealloc")==0) opD=1;
        else if (strcmp(argv[i], "-deletekey")==0) opDlt=1;
        else if (strcmp(argv[i], "-show")==0) opS=1;
        else if (strcmp(argv[i], "-show-vars")==0) opSv=1;
        else if (strcmp(argv[i], "-show-funcs")==0) opSf=1;
        else if (strcmp(argv[i], "-dopmap")==0) opDo=1;
        else if (strcmp(argv[i], "-malloc")==0) opMa=1;
        else if (strcmp(argv[i], "-mmap")==0) opMm=1;
        else if (strcmp(argv[i], "-createshared")==0) opCs=1;
        else if (strcmp(argv[i], "-shared")==0) opSh=1;
        else if (strcmp(argv[i], "-all")==0) opAll=1;
    }
	printf("%d%d%d%d%d%d%d %d%d%d%d%d\n", opA,opD,opDlt,opS,opSv,opSf,opDo,
		opMa,opMm,opCs,opSh,opAll);
	//Comprobar que las opciones sean validas
	sumOpMain=opA+opD+opDlt+opS+opSv+opSf+opDo;
	if(sumOpMain > 1){
		printf("Modos incompatibles\n");
		return 1;
	}
	sumOpExtra=opMa+opMm+opCs+opSh+opAll;
	if(sumOpExtra > 1){
		printf("Opciones incompatibles\n");
		return 1;
	}

	if(opA && opMa){		//-allocate -malloc
		if(argc <= 3){
			printf("Imprimiento lista:\n");
			memPrintList(memLista, 'a');
		}else if(argc == 4){
			tam = atoi(argv[3]);
			puntero = (void *)malloc(tam);
			memInsertElement(puntero, tam, "", 0, 'a', memLista);
			printf("Allocated %d at %p\n", tam, puntero);
		}else{
			printf("Argumentos incorrectos\n");
			return 1;
		}
	}

	if(opA && opMm){		//-allocate -mmap
		arg[0] = argv[3];
		arg[1] = argv[4];
		arg[2] = NULL;
		Cmd_AllocateMmap(arg);
	}

	if(opA && opCs){		//-allocate -createshared
		arg[0] = argv[3];
		arg[1] = argv[4];
		arg[2] = NULL;
		Cmd_AlocateCreateShared(arg);
	}

	if(opA && opSh){		//-allocate -shared
		//TODO
	}

	if(opD && argc==2){		//-dealloc
		memPrintList(memLista, 't');

	}

	if(opD && opMa){		//-dealloc -malloc
		Cmd_DeallocMalloc(argv[3]);
	}

	if(opD && opMm){		//-dealloc -mmap
		Cmd_DeallocMmap(argv[3]);
	}

	if(opD && opSh){		//-dealloc -shared
		//TODO
	}

	if(opD && argc==3 && sumOpExtra==0){		//-dealloc addr
		//TODO
	}

	if(opDlt && argc==3 && sumOpExtra==0){		//-deletekey cl
		//TODO
	}

	if(opS && argc==2){		//-show
		//TODO
	}

	if(opS && opMa){		//-show -malloc
		//TODO
	}

	if(opS && opSh){		//-show -shared
		//TODO
	}

	if(opS && opMm){		//-show -mmap
		//TODO
	}

	if(opS && opAll){		//-show -all
		//TODO
	}

	if(opSv){		//-show-vars
		//TODO
	}

	if(opSf){		//-show-funcs
		//TODO
	}

	if(opDo){		//-dopmap
		//TODO
	}


	return 0;
}


int cmdMemdump(int argc, char *argv[]){
	printf("soy memdump\n");
	return 0;
}
int cmdMemfill(int argc, char *argv[]){
	printf("soy memfill\n");
	return 0;
}
int cmdRecurse(int argc, char *argv[]){
	printf("soy recurse\n");
	return 0;
}
int cmdReadfile(int argc, char *argv[]){
	printf("soy readfile\n");
	return 0;
}
int cmdWritefile(int argc, char *argv[]){
	printf("soy writefile\n");
	return 0;
}
