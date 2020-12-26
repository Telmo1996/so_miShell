#include "funproc.h"

int cmdGetpriority(int argc, char *argv[]){
	pid_t pid;

	if(argc == 2)
		printf("%d\n", getpriority(PRIO_PROCESS, atoi(argv[1])));
	else{
		pid = getpid();
		printf("%d\n", getpriority(PRIO_PROCESS, pid));
	}

	return 0;
}

int cmdSetpriority(int argc, char *argv[]){
	pid_t pid;
	char* getprio[] = {"getpriority"};

	if(argc == 3)
		setpriority(PRIO_PROCESS, atoi(argv[1]), atoi(argv[2]));
	else if(argc == 2){
		pid = getpid();
	setpriority(PRIO_PROCESS, pid, atoi(argv[1]));
	}else{
		cmdGetpriority(1, getprio);
	}

	return 0;
}

char * NombreUsuario (uid_t uid){
	struct passwd *p;
	if ((p=getpwuid(uid))==NULL)
		return (" ??????");
	return p->pw_name;
}

uid_t UidUsuario (char * nombre){
	struct passwd *p;
	if ((p=getpwnam (nombre))==NULL)
		return (uid_t) -1;
	return p->pw_uid;
}

int cmdGetuid (int argc, char *tr[]){
	uid_t real=getuid(), efec=geteuid();
	printf ("Credencial real: %d, (%s)\n", real, NombreUsuario (real));
	printf ("Credencial efectiva: %d, (%s)\n", efec, NombreUsuario (efec));

	return 0;
}

int cmdSetuid (int argc, char *tr[]){
	uid_t uid;
	int u;
	if (tr[0]==NULL || (!strcmp(tr[0],"-l") && tr[1]==NULL)){
		cmdGetuid(argc, tr);
		return 0;
	}
	if (!strcmp(tr[0],"-l")){
		if ((uid=UidUsuario(tr[1]))==(uid_t) -1){
			printf ("Usuario no existente %s\n", tr[1]);
			return 1;
		}
	}
	else if ((uid=(uid_t) ((u=atoi (tr[0]))<0)? -1: u) ==(uid_t) -1){
		printf ("Valor no valido de la credencial %s\n",tr[0]);
		return 1;
	}
	if (setuid (uid)==-1)
		printf ("Imposible cambiar credencial: %s\n", strerror(errno));
	
	return 0;
}

int cmdFork(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdExecute(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdForeground(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdBackground(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdRunas(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdExecuteas(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdProg(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdListprocs(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdProc(int argc, char *argv[]){
	printf("hola");
	return 0;
}

int cmdDeleteprocs(int argc, char *argv[]){
	printf("hola");
	return 0;
}

