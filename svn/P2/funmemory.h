#include "funcionesCmd.h"
#include "memlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/wait.h>

#define TAMANO 4096
#define LEERCOMPLETO ((ssize_t)-1)

int g1, g2, g3;;

memNode_t * memCreateList();
int cmdMemory(int, char**);
int cmdMemdump(int, char**);
int cmdMemfill(int, char**);
int cmdRecurse(int, char**);
int cmdReadfile(int, char**);
int cmdWritefile(int, char**);

