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
	if ((p=shmat(id,NULL,0))==(void*) -1){ //crea una copia de la memoria compartida en la del programa
		aux=errno; /*si se ha creado y no se puede mapear*/
		if (tam) /*se borra */
			shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}
	shmctl (id,IPC_STAT,&s);
	/* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
	memInsertElement(p, s.shm_segsz, "", 0, clave, 's', memLista);
	return (p);
}

void Cmd_AlocateCreateShared (char *arg[]){ 
	/*arg[0] is the key and arg[1] is the size*/
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

void Cmd_AllocateShared(char *cl){
	key_t clave;
	int id;
	void* p;
	struct shmid_ds s;

	clave=(key_t) atoi(cl);
	if ((id=shmget(clave,0,0666))==-1){
		perror ("shmget: imposible obtener memoria compartida");
		return;
	}
	if ((p=shmat(id,NULL,0))==(void*) -1){
        return;
    }
	shmctl (id,IPC_STAT,&s);
	/* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
	memInsertElement(p, s.shm_segsz, "", 0, clave, 's', memLista);

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
	memInsertElement(p, s.st_size, fichero, df, 0, 'm', memLista);
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

void Cmd_DeallocShared(char *cl){
	memNode_t* current=memLista;
	memNode_t* previous=NULL;
	key_t clave;
	
	if(cl == NULL){
		memPrintList(memLista, 's');
		return;
	}
	clave=(key_t) atoi(cl);
	while(current->next){
		if(current->key == clave && (current->tipo == 's')){
			memDeleteNode(previous);
			return;
		}
		previous=current;
		current=current->next;
	}

	memPrintList(memLista, 's');
}

void Cmd_DeallocAddr(char *addr){
	void *p;
	memNode_t* current=memLista;

	p=(void*)(long)strtol(addr, NULL, 16);

	if(addr == NULL){
		memPrintList(memLista, 'a');
		return;
	}
	while(current->next != NULL){
		//printf("comparando %p y %p\n", current->next->puntero, p);
		if(current->next->puntero == p){ 
			memDeleteNode(current);		//que es el anterior
			return;
		}
		current=current->next;
	}

	printf("No se ha encontrado %p\n", p);
	memPrintList(memLista, 'a');

}

void Cmd_dopmap (char *args[]) /*no arguments necessary*/
{ pid_t pid;
	char elpid[32];
	char *argv[3]={"pmap",elpid,NULL};

	sprintf (elpid,"%d", (int) getpid());
	if ((pid=fork())==-1){
		perror ("Imposible crear proceso");
		return;
	}if (pid==0){
		if (execvp(argv[0],argv)==-1)
			perror("cannot execute pmap");
		exit(1);
	}
	waitpid (pid,NULL,0);
}

void Cmd_deletekey (char *args[]) /*arg[0] points to a str containing the key*/
{
	key_t clave;
	int id;
	char *key=args[0];
	//printf("%s\n",key);

	if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
		printf ("   rmkey  clave_valida\n");
		return;
	}
	if ((id=shmget(clave,0,0666))==-1){
		perror ("shmget: imposible obtener memoria compartida");
		return;
	}
	if (shmctl(id,IPC_RMID,NULL)==-1)
		perror ("shmctl: imposible eliminar memoria compartida\n");
}

void showVars(){
	//variables locales
	int l1, l2, l3;

	printf("Local variables: %p, %p, %p\n", &l1, &l2, &l3);

	//variables globales
	printf("Global variables: %p, %p, %p\n", &g1, &g2, &g3);
}

void showFuncts(){
	//program functions
	printf("Program function: %p, %p, %p\n",&cmdDate, &cmdAutores, &cmdTime);
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


	//printf("%d%d%d%d%d%d%d %d%d%d%d%d\n", opA,opD,opDlt,opS,opSv,opSf,opDo,
	//	opMa,opMm,opCs,opSh,opAll);

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
			memInsertElement(puntero, tam, "", 0, 0, 'a', memLista);
			printf("Allocated %d at %p\n", tam, puntero);
		}else{
			printf("Argumentos incorrectos\n");
			return 1;
		}
	}

	if(sumOpMain == 0 && sumOpExtra==0){
		memPrintList(memLista,'t');
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
		if(argc == 3){
			memPrintList(memLista, 's');
		}else{
			Cmd_AllocateShared(argv[3]);
		}
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
		Cmd_DeallocShared(argv[3]);
	}

	if(opD && argc==3 && sumOpExtra==0){		//-dealloc addr
		Cmd_DeallocAddr(argv[2]);
	}

	if(opDlt && argc==3 && sumOpExtra==0){		//-deletekey cl
		args[0]=argv[2];
		args[1]=NULL;
		Cmd_deletekey(args);
	}

	if(opS && argc==2){		//-show
		showFuncts();
		showVars();
	}

	if(opS && opMa){		//-show -malloc
		memPrintList(memLista,'a');
	}

	if(opS && opSh){		//-show -shared
		memPrintList(memLista,'s');
	}

	if(opS && opMm){		//-show -mmap
		memPrintList(memLista,'m');
	}

	if(opS && opAll){		//-show -all
		memPrintList(memLista,'t');
	}

	if(opSv){		//-show-vars
		showVars();
	}

	if(opSf){		//-show-funcs
		showFuncts();
	}

	if(opDo){		//-dopmap
		args[0]=NULL;
		Cmd_dopmap(args);
	}


	return 0;
}


int cmdMemdump(int argc, char *argv[]){
	char* p;
	char* hex;// = argv[1];                          // here is the hex string
	long addr;// = (int)strtol(hex, NULL, 16);       // number base 16
	int i, j, cont;
	char buff[10];
	//printf("%c\n", num);                        // print it as a char
	//printf("%d\n", num);                        // print it as decimal
	//printf("%X\n", num);                        // print it back as hex

	if(argc < 2)
		return 1;
	
	hex = argv[1];
	addr = (long)strtol(hex, NULL, 16);
	p=(char*)addr;

	if(argv[2] != NULL)
		cont = atoi(argv[2]);
	else
		cont = 25;

	for(j=0; j<(cont/25); j++){
		for(i=0; i<25; i++){
			printf("%c  ", p[j*25+i]);
		}
		printf("\n");
		for(i=0; i<25; i++){
			//ltoa(p[j*25+i], buff, 16);
			sprintf(buff, "%x", p[j*25+i]);
			printf("%s ", buff);
		}
		printf("\n");
	}
	if(cont%25 != 0){
		for(i=0; i<cont%25; i++){
			printf("%c  ", p[(cont/25)*25+i]);
		}
		printf("\n");
		for(i=0; i<cont%25; i++){
			//ltoa(p[(cont/25)*25+i], buff, 16);
			sprintf(buff, "%x", p[(cont/25)*25+i]);
			printf("%s ", buff);
		}
		printf("\n");
	}


	return 0;
}

int cmdMemfill(int argc, char *argv[]){
	char* p;
	char* hex;
	long addr, byte;
	int i, cont;

	if(argc<2)
		return 1;

	hex = argv[1];
	addr = (long)strtol(hex, NULL, 16);
	p=(char*)addr;

	if(argc < 3)
		cont = 128;
	else
		cont = atoi(argv[2]);
	if(argc < 4)
		byte = 65;
	else
		byte = (long)strtol(argv[3], NULL, 16);

	printf("%d, %ld", cont, byte);
	
	for(i=0; i<cont; i++){
		p[i]=byte;
	}

	return 0;
}

void doRecursiva (int n){
	char automatico[TAMANO];
	static char estatico[TAMANO];

	printf ("parametro n:%d en %p\n",n,&n);
	printf ("array estatico en:%p \n",estatico);
	printf ("array automatico en %p\n",automatico);

	n--;
	if (n>0)
		doRecursiva(n);
}

int cmdRecurse(int argc, char *argv[]){
	if(argc < 2)
		return 1;
	doRecursiva(atoi(argv[1]));
	return 0;
}

ssize_t LeerFichero (char *fich, void *p, ssize_t n)
{ /*n=-1 indica que se lea todo*/
	ssize_t nleidos,tam=n;
	int df, aux;
	struct stat s;
	if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1)
		return ((ssize_t)-1);
	if (n==LEERCOMPLETO)
		tam=(ssize_t) s.st_size;
	if ((nleidos=read(df,p, tam))==-1){
		aux=errno;
		close(df);
		errno=aux;
		return ((ssize_t)-1);
	}
	close (df);
	return (nleidos);
}

int cmdReadfile(int argc, char *argv[]){
	void *p;
	long addr;

	if(argc<3)
		return 1;
	
	addr=(long)strtol(argv[2], NULL, 16);
	p=(void*)addr;

	if(argc==3)
		LeerFichero(argv[1], p, LEERCOMPLETO);
	else
		LeerFichero(argv[1], p, (ssize_t)atoi(argv[3]));
		
	return 0;
}

void EscribirFichero(char opO, char* fich, void* p, size_t n){
	int fd;
	if( access( fich, F_OK ) != -1 ) {
		// file exists
		if(! opO){
			printf("Archivo %s ya existe\n", fich);
			printf("Utiliza -o para sobreescribirlo.");
			return;
		}
	}
	
	fd=open(fich, O_WRONLY | O_CREAT);
	write(fd, p, n);


	close(fd);
}

int cmdWritefile(int argc, char *argv[]){
	char opO=0;
	char *fich, *addr, *cont;
	void *p;
	size_t n;
	
	if(argc < 2)
		return 1;
	
	if(strcmp(argv[1],"-o")==0){
		opO=1;
		if(argc < 5)
			return 1;
		fich=argv[2];
		addr=argv[3];
		cont=argv[4];
	}else{
		if(argc<4)
			return 1;
		fich=argv[1];
		addr=argv[2];
		cont=argv[3];
	}
	
	p=(void*)(long)strtol(addr, NULL, 16);
	n=(size_t)atoi(cont);

	EscribirFichero(opO, fich, p, n);

	return 0;
}
