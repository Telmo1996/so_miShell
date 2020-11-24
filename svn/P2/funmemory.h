#include "funcionesCmd.h"
#include "memlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

memNode_t * memCreateList();
int cmdMemory(int, char**);
int cmdMemdump(int, char**);
int cmdMemfill(int, char**);
int cmdRecurse(int, char**);
int cmdReadfile(int, char**);
int cmdWritefile(int, char**);

