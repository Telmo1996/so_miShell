#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

char LetraTF(mode_t m);
int cmdList(int, char**);
