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

/******************************SENALES ******************************************/
struct SEN{
	char *nombre;
	int senal;
};

static struct SEN sigstrnum[]={
	"HUP", SIGHUP,
	"INT", SIGINT,
	"QUIT", SIGQUIT,
	"ILL", SIGILL,
	"TRAP", SIGTRAP,
	"ABRT", SIGABRT,
	"IOT", SIGIOT,
	"BUS", SIGBUS,
	"FPE", SIGFPE,
	"KILL", SIGKILL,
	"USR1", SIGUSR1,
	"SEGV", SIGSEGV,
	"USR2", SIGUSR2,
	"PIPE", SIGPIPE,
	"ALRM", SIGALRM,
	"TERM", SIGTERM,
	"CHLD", SIGCHLD,
	"CONT", SIGCONT,
	"STOP", SIGSTOP,
	"TSTP", SIGTSTP,
	"TTIN", SIGTTIN,
	"TTOU", SIGTTOU,
	"URG", SIGURG,
	"XCPU", SIGXCPU,
	"XFSZ", SIGXFSZ,"VTALRM", SIGVTALRM,
	"PROF", SIGPROF,
	"WINCH", SIGWINCH,
	"IO", SIGIO,
	"SYS", SIGSYS,
	/*senales que no hay en todas partes*/
	#ifdef SIGPOLL
	"POLL", SIGPOLL,
	#endif
	#ifdef SIGPWR
	"PWR", SIGPWR,
	#endif
	#ifdef SIGEMT
	"EMT", SIGEMT,
	#endif
	#ifdef SIGINFO
	"INFO", SIGINFO,
	#endif
	#ifdef SIGSTKFLT
	"STKFLT", SIGSTKFLT,
	#endif
	#ifdef SIGCLD
	"CLD", SIGCLD,
	#endif
	#ifdef SIGLOST
	"LOST", SIGLOST,
	#endif
	#ifdef SIGCANCEL
	"CANCEL", SIGCANCEL,
	#endif
	#ifdef SIGTHAW
	"THAW", SIGTHAW,
	#endif
	#ifdef SIGFREEZE
	"FREEZE", SIGFREEZE,
	#endif
	#ifdef SIGLWP
	"LWP", SIGLWP,
	#endif
	#ifdef SIGWAITING
	"WAITING", SIGWAITING,
	#endif
	NULL,-1,
};		/*fin array sigstrnum */
