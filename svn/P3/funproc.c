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

int cmdSetuid (int argc, char *argv[]){
	uid_t uid;
	char* tr[argc-1];
	int u;

	for(int i=0; i<argc; i++){
		tr[i]=argv[i+1];
	}

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
	//printf("uid: %d\n", uid);
	if (setuid (uid)==-1){
		printf ("Imposible cambiar credencial: %s\n", strerror(errno));
		return 1;
	}
	
	return 0;
}

int cmdFork(int argc, char *argv[]){
	pid_t pid;
	if((pid=fork())==-1){
		perror("Imposible crear proceso");
		return 1;
	}
	waitpid(pid, NULL, 0);
	return 0;
}

pid_t execute(char* args[], char changePri, int pri, char isExecuteas, char* login){
	pid_t pid;
	char* argsUid[3];

	pid = getpid();
	if(changePri)
		setpriority(PRIO_PROCESS, pid, pri);
	
	if(isExecuteas){
		argsUid[0]="setuid"; argsUid[1]="-l"; argsUid[2] = login;
		if((cmdSetuid(3, argsUid))!=0){
			exit(255);
			return 0;
		}
	}

	if (execvp(args[0], args)==-1){
		perror ("Cannot execute");
		exit(255); /*exec has failed for whatever reason*/
	}

	return pid;

}

pid_t createChild(
	char* args[], char changePri, int pri, char isRunas, char* login
){
	pid_t pid;
	char* argsUid[3];

	pid = getpid();
	if(changePri)
		setpriority(PRIO_PROCESS, pid, pri);
	
	if((pid=fork())==0){
		
		if(isRunas){
			argsUid[0]="setuid"; argsUid[1]="-l"; argsUid[2] = login;
			if((cmdSetuid(3, argsUid))!=0){
				exit(255);
				return 0;
			}
		}

		if (execvp(args[0], args)==-1){
			perror ("Cannot execute");
			exit(255); /*exec has failed for whatever reason*/
		}
		
		exit(0);
		return 0;
	}

	return pid;

}

int cmdExecute(int argc, char *argv[]){
	char* args[argc-1];
	int pri;
	char changePri=0;
	int i=1;
	
	if(argc < 2) return 1; //Falla

	if(argv[argc-1][0] == '@'){
		changePri=1;
		pri = atoi(&argv[argc-1][1]);
		argv[argc-1] = NULL;
	}

	while(argv[i] != NULL){
		args[i-1] = argv[i];
		i++;
	}
	args[i-1]=NULL;

	execute(args, changePri, pri, 0, "");


	return 0;
}

int cmdForeground(int argc, char *argv[]){
	char* args[argc-1];
	int pri;
	char changePri;
	pid_t pid;
	int i=1;

	if(argc < 2) return 1; //Falla

	if(argv[argc-1][0] == '@'){
		changePri=1;
		pri = atoi(&argv[argc-1][1]);
		argv[argc-1] = NULL;
	}

	while(argv[i] != NULL){
		args[i-1] = argv[i];
		i++;
	}
	args[i-1]=NULL;

	pid=createChild(args, changePri, pri, 0, "");
	waitpid(pid, NULL, 0);

	return 0;
}

int cmdBackground(int argc, char *argv[]){
	char* args[argc-1];
	int pri;
	char changePri=0;
	pid_t pid;
	int i=1;

	if(argc < 2) return 1; //Falla

	if(argv[argc-1][0] == '@'){
		changePri=1;
		pri = atoi(&argv[argc-1][1]);
		argv[argc-1] = NULL;
	}

	while(argv[i] != NULL){
		args[i-1] = argv[i];
		i++;
	}
	args[i-1]=NULL;

	pid=createChild(args, changePri, pri, 0, "");

	//printf("%d\n", pid);
	procInsertElement(args[0], pid, procLista);

	return 0;
}

int cmdRunas(int argc, char *argv[]){
	char* args[argc-1];
	char* login;
	int pri;
	char changePri=0;
	pid_t pid;
	char toBack = 0;
	int i=0;

	if(argc < 2) return 1; //Falla

	if (strcmp(argv[argc-1],"&")==0){
		toBack = 1;
		argv[argc-1]=NULL;
	}
	printf("%s ",toBack?"si":"no");
	while(argv[i] != NULL){
		i++;
	}
	if(argv[i-1][0] == '@'){
		changePri=1;
		pri = atoi(&argv[i-1][1]);
		argv[i-1]=NULL;
	}
	//printf("%d\n",changePri?pri:42);
	login=argv[1];
	i=2;
	while(argv[i] != NULL){
		args[i-2] = argv[i];
		i++;
	}
	args[i-1]=NULL;


	pid=createChild(args, changePri, pri, 1, login);

	if(!toBack)
		waitpid(pid, NULL, 0);
	
	return 0;

}

int cmdExecuteas(int argc, char *argv[]){
	char* args[argc-1];
	char* login;
	int pri;
	char changePri = 0;
	pid_t pid;
	int i=0;

	if(argc < 2) return 1; //Falla

	if(argv[argc-1][0] == '@'){
		changePri = 1;
		pri = atoi(&argv[argc-1][1]);
		argv[argc-1]=NULL;
	}
	login = argv[1];
	i=2;
	while(argv[i] != NULL){
		args[i-2] = argv[i];
		i++;
	}
	args[i-1]=NULL;

	pid = execute(args, changePri, pri, 1, login);

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

