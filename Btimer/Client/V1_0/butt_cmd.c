#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/epoll.h>
#include <linux/input.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>

//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>



#define DAEMON_NAME "srv_pbutt"
#define RUN_AS_USER "root"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void child_handler(int signum)
{
  switch(signum) {
  case SIGALRM: exit(EXIT_FAILURE); break;
  case SIGUSR1: exit(EXIT_SUCCESS); break;
  case SIGCHLD: exit(EXIT_FAILURE); break;
  }
}

static void daemonize( const char *lockfile )
{
  pid_t pid, sid, parent;
  int lfp = -1;

  /* already a daemon */
  if ( getppid() == 1 ) return;

  /* Create the lock file as the current user */
  if ( lockfile && lockfile[0] ) {
    lfp = open(lockfile,O_RDWR|O_CREAT,0640);
    if ( lfp < 0 ) {
      syslog( LOG_ERR, "unable to create lock file %s, code=%d (%s)",
	      lockfile, errno, strerror(errno) );
      exit(EXIT_FAILURE);
    }
  }

  /* Drop user if there is one, and we were run as root */
  if ( getuid() == 0 || geteuid() == 0 ) {
    struct passwd *pw = getpwnam(RUN_AS_USER);
    if ( pw ) {
      syslog( LOG_NOTICE, "setting user to " RUN_AS_USER );
      setuid( pw->pw_uid );
    }
  }

  /* Trap signals that we expect to recieve */
  signal(SIGCHLD,child_handler);
  signal(SIGUSR1,child_handler);
  signal(SIGALRM,child_handler);

  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    syslog( LOG_ERR, "unable to fork daemon, code=%d (%s)",
	    errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }
  /* If we got a good PID, then we can exit the parent process. */
  if (pid > 0) {

    /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
       for two seconds to elapse (SIGALRM).  pause() should not return. */
    alarm(2);
    pause();

    exit(EXIT_FAILURE);
  }

  /* At this point we are executing as the child process */
  parent = getppid();

  /* Cancel certain signals */
  signal(SIGCHLD,SIG_DFL); /* A child process dies */
  signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);
  signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
  signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

  /* Change the file mode mask */
  umask(0);

  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) {
    syslog( LOG_ERR, "unable to create a new session, code %d (%s)",
	    errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }

  /* Change the current working directory.  This prevents the current
     directory from being locked; hence not being able to remove it. */
  if ((chdir("/")) < 0) {
    syslog( LOG_ERR, "unable to change directory to %s, code %d (%s)",
	    "/", errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }

  /* Redirect standard files to /dev/null */
  freopen( "/dev/null", "r", stdin);
  freopen( "/dev/null", "w", stdout);
  freopen( "/dev/null", "w", stderr);

  /* Tell the parent process that we are A-okay */
  kill( parent, SIGUSR1 );
}

int main()
{
  int kfd; //keys input event epoll dsc 
  int butt_state=1;


  kfd=open("/dev/input/event0",O_RDWR);
  if (kfd<0) {
    perror("open event error\n");
    exit(1);
  }
  daemonize("/tmp/pbutt.lock");
  while(1) {
    int len;
    struct input_event keydat;
    len=read(kfd,&keydat,sizeof(keydat));
    //printf("Evtype %d, key %d. val%d, (%d)\n",keydat.type,keydat.code,keydat.value,len);
    if (keydat.type!=EV_KEY || keydat.value!=1) continue;
    if (keydat.code==KEY_ENTER) {
      int ret;
      if (butt_state==0) { // if state is off, turn it on
	usleep(100000L);
	printf("Power ON\n");
	ret=system("init 3");
	syslog( LOG_ERR, "Power ON, init ret %d",ret);
	butt_state=1;
      }
      else { // turn off otherwise
	printf("Power OFF\n");

	ret=system("init 1");
	syslog( LOG_ERR, "Power OFF, init ret %d",ret);
	butt_state=0;
      }
    }
  }
}  



