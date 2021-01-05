#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "funcionesCmd.h"
#include "proclist.h"

int cmdGetpriority(int, char**);
int cmdSetpriority(int, char**);
int cmdGetuid(int, char**);
int cmdSetuid(int, char**);
int cmdFork(int, char**);
int cmdExecute(int, char**);
int cmdForeground(int, char**);
int cmdBackground(int, char**);
int cmdRunas(int, char**);
int cmdExecuteas(int, char**);
int cmdListprocs(int, char**);
int cmdProc(int, char**);
int cmdDeleteprocs(int, char**);
void noEntiendo(int, char**);

