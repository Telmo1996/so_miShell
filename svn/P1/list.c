#include "list.h"

char LetraTF (mode_t m){
	switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
		case S_IFSOCK:
			return 's';  /*socket */
		case S_IFLNK:
			return 'l';    /*symbolic link*/
		case S_IFREG:
			return '-';   /* fichero normal*/
		case S_IFBLK:
			return 'b';   /*block device*/
		case S_IFDIR:
			return 'd';   /*directorio */
		case S_IFCHR:
			return 'c';   /*char  device*/
		case S_IFIFO:
			return 'p';   /*pipe*/
		default:
			return '?';	/*desconocido, no deberia aparecer*/
	}
}

int cmdList(int argc, char *argv[]){
    char opL=0, opD=0, opH=0, opR=0;
	char muestra=0;
    DIR *dirp;
    struct dirent *direntp;
	char * name=".";
	char tipo;
	struct stat s;
	int i;

    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-long") == 0) opL=1;
        else if (strcmp(argv[i], "-dir") == 0) opD=1;
        else if (strcmp(argv[i], "-hid") == 0) opH=1;
        else if (strcmp(argv[i], "-rec") == 0) opR=1;
    }


	dirp = opendir(name);
	if(dirp == NULL){
		printf("error\n");
		exit(2);
	}
	
	if(opL)
		printf("i-node\toffset\t\t\tlong\tnombre\ttipo\n");
	else
		printf("nombre\n");
	while((direntp = readdir(dirp)) != NULL){
		//TODO ojo con el directorio actual
		if( stat(direntp->d_name,&s) == 0 ){
			tipo = LetraTF(s.st_mode);
		}
		muestra = 1;
		if((direntp->d_name[0] == '.') && !opH)
			muestra=0;
		if(muestra){
			if(opL){
				printf("%ld\t%ld\t%d\t%s\t%c\n", direntp->d_ino, direntp->d_off,
					direntp->d_reclen, direntp->d_name, tipo
				);
			}else{
				printf("%s\n", direntp->d_name);
			}
		}
	}

	return 0;
}
