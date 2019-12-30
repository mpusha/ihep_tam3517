#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>


#define DAEMON_NAME "BTimerU70"
#define RUN_AS_USER "root" 
#define LOCKFILE  "/var/lock/" DAEMON_NAME
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "bt.h"
//#include "db.h"

char *getTime(void);
void endProg(void);
int testLockFile(const char * fn);
int lockFile(const char * fn);
int fileExists(const char* fn);
static void child_handler(int);

int fileExists(const char* fn)
{
  if(access(fn,F_OK)==0)
    return(1);
  return(0);
}

int testLockFile(const char * fn)
{
  struct flock fl={F_WRLCK,SEEK_SET,0,0,0}; // l_type l_whence l_start l_len l_pid
  int fd;
  if(fileExists(fn)){ //lock file exists, test on locking
    if((fd=open(fn,O_RDWR|O_CREAT))==-1){
      perror("open");
      exit(EXIT_FAILURE);
    }
    fl.l_pid=getpid();
    if(fcntl(fd,F_GETLK,&fl)==-1){
      perror("fcntl");
      exit(EXIT_FAILURE);
    }
    if(fl.l_type==F_UNLCK) return(0); else return(1);
  }
  return(0); // lock file absent
}

int lockFile(const char * fn)
{
  struct flock fl={F_WRLCK,SEEK_SET,0,0,0}; // l_type l_whence l_start l_len l_pid
  int fd=-1;
  char buf[16];
  bzero(buf,16);
/*  fd= open(fn,O_RDWR|O_CREAT|O_EXCL,LOCKMODE);
  if ( fd < 0 ) {
    if(errno==EEXIST){ // lock file is exists program may be running
      return(EXIT_FAILURE);
    }
    syslog( LOG_ERR, "unable to create lock file %s, code=%d (%s)",
            LOCKFILE, errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }*/
  if((fd=open(fn,O_RDWR|O_CREAT))==-1){
    perror("open");
    exit(EXIT_FAILURE);
  }
  fl.l_pid=getpid();
  fcntl(fd,F_SETLKW,&fl); //getch();
  ftruncate(fd,0);
  sprintf(buf,"%d",getpid());
  write(fd,buf,strlen(buf)+1);
  //close(fd);
  return(EXIT_SUCCESS);
}

static void child_handler(int signum)
{
  switch(signum) {
  case SIGALRM: syslog( LOG_ERR,"Program of %s alarm exit. Child process don't run!",DAEMON_NAME); exit(EXIT_FAILURE); break;
  case SIGUSR2: 
     endProg();
     exit(EXIT_SUCCESS); 
  break;
  case SIGTERM:{
   //printf("Program of %s started Ok!\n",DAEMON_NAME );
   syslog( LOG_NOTICE, "Program of %s started Ok!",DAEMON_NAME );
   closelog();
   exit(EXIT_SUCCESS);
  }
  break; 
  case SIGCHLD: 
     //child process finished
    exit(EXIT_FAILURE); 
  break;
  }
}
// remove lock file on finish program from user request
void endProg(void)
{
   remove(LOCKFILE);
   syslog( LOG_NOTICE, "Program %s stoped from user request.",DAEMON_NAME );
   closelog();
}
// get current time structure
char *getTime(void)
{
  time_t now;
  struct tm *ptr;
  static char tbuf[64];
  bzero(tbuf,64);
  time(&now);
  ptr=localtime(&now);
  strftime(tbuf,64,"%Y-%m-%e %H:%M:%S",ptr);
  return tbuf;  
}
// make daemon
static void daemonize( void )
{
  pid_t pid, sid, parent;
//  int lfp = -1;

  /* already a daemon */
  if ( getppid() == 1 ) {
    syslog( LOG_ERR, "Program %s already running from init.",DAEMON_NAME);
    exit(EXIT_FAILURE);
  } // run from init
 
  /* Drop user if there is one, and we were run as root */
  if ( getuid() == 0 || geteuid() == 0 ) {
    struct passwd *pw = getpwnam(RUN_AS_USER);
    if ( pw ) {
      syslog( LOG_NOTICE, "Setting user to " RUN_AS_USER );
      setuid( pw->pw_uid );
    }
  }

  /* Trap signals that we expect to recieve */
  signal(SIGCHLD,child_handler);
  signal(SIGUSR2,child_handler);
  signal(SIGALRM,child_handler);
  signal(SIGTERM,child_handler);

  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    syslog( LOG_ERR, "Unable to fork daemon %s, code=%d (%s).",DAEMON_NAME,
	    errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }
  /* If we got a good PID, then we can exit the parent process. */
  if (pid > 0) {

    /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
       for two seconds to elapse (SIGALRM).  pause() should not return. */ 
    alarm(5);
    pause();
    exit(EXIT_FAILURE);
  }

  sleep(1);
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
    syslog( LOG_ERR, "Unable to create a new session, code %d (%s).",
	    errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }

  /* Change the current working directory.  This prevents the current
     directory from being locked; hence not being able to remove it. */
  if ((chdir("/")) < 0) {
    syslog( LOG_ERR, "Unable to change directory to %s, code %d (%s).",
	    "/", errno, strerror(errno) );
    exit(EXIT_FAILURE);
  }

  /* Redirect standard files to /dev/null */
  freopen( "/dev/null", "r", stdin);
  freopen( "/dev/null", "w", stdout);
  freopen( "/dev/null", "w", stderr);
 
  /* Tell the parent process that we are A-okay */
  kill( parent, SIGTERM );
}


TBt *bTimer;

int main(int argc, char *argv[])
{
    try{
    int retval = 1;
    int debug =0;
    pid_t pid;
    int len,fd;
    char pid_buf[16];
    if(argc>1){
      if(!strcmp(argv[1],"stop")){
        if((fd=open(LOCKFILE,O_RDONLY))<0){
          perror("Lock file not found "LOCKFILE". May be the server "DAEMON_NAME" is not run.\n");
          exit(fd); 
        }
        len=read(fd,pid_buf,16);
        pid_buf[len]=0;
        pid=atoi(pid_buf); 
        kill(pid, SIGUSR2 );  
        printf("Program %s stoped from user request. %d pid.\n",DAEMON_NAME,pid );
        sleep(1);
        exit(EXIT_SUCCESS);
      }
      else if(!strcmp(argv[1],"debug")){
        printf("Run in debug mode. All output place in file bt_log & bt_err.\n");
        debug=1;
      }
      else if(!strcmp(argv[1],"console")){
        printf("Run in debug mode. All output place in stdout.\n");
        debug=2;
      }
      else {
        printf("Usage %s [stop | debug | console]\n",argv[0]);
        exit(EXIT_FAILURE);
      }
    }
// Initialize the logging interface 
    if(testLockFile(LOCKFILE)) {
      printf("Program %s already running.\n",DAEMON_NAME);
      exit(EXIT_FAILURE); 
    }
    openlog( DAEMON_NAME, LOG_PID, LOG_LOCAL5 );
    syslog( LOG_INFO, "Start program %s %s.",DAEMON_NAME,getTime());

// Daemonize if not debug mode
    if(!debug){
      daemonize();
      if(lockFile(LOCKFILE)){
        syslog(LOG_ERR, "Can't lock file %s.",LOCKFILE);
        exit(EXIT_FAILURE);
      }
    }
    if(debug==1){
      freopen( "/home/work/bt_log", "w", stdout);
      freopen( "/home/work/bt_err", "w", stderr);
    }
    sleep(1); // synchronisation
    printf("Start program %s %s.\n",DAEMON_NAME,getTime());

    syslog(LOG_INFO,"Begin initialisation of FPGA and DB.");
    printf("Begin initialisation of FPGA and DB.\n");



  unsigned int status_db;

    // begin work daemon
  bTimer=new TBt(2,19200); // create new object

  printf("Begin initialisation of DB.\n");
  syslog(LOG_NOTICE,"Begin initialisation of DB.");
  bTimer->IniFileRead();
  status_db=db_init(); // initialization of DB 0 - if Ok. If was fatal error then exit
  printf("status=%d\n",status_db);
  if(status_db){
    printf("Task BtimerU70 can't read all data from DB error code [0x%x]!\n",status_db);
    syslog(LOG_NOTICE,"Task BtimerU70 can't read all data from DB error code [0x%x]!",status_db);
    syslog( LOG_INFO, "Can't initialising of DB. Program work in manual mode!");
    printf("Can't initialising of DB. Program work in manual mode!\n");
    bTimer->exportManualCtrl(1); // b-timer in manual mode
    sleep(1);
  }
  else {
    printf("Initialisation of DB is successfull!\n");
    syslog(LOG_NOTICE,"Initialisation of DB is successfull!");
    bTimer->exportManualCtrl(0); // b-timer in remoute control mode
    sh_put_mess((char*)"Task BtimerU70 is starting.");
  }

  if(bTimer->initialSystem()==EXIT_FAILURE) exit(EXIT_FAILURE);
  syslog(LOG_NOTICE,"Task BtimerU70 is starting.");

  unsigned int update;
  int writeHWReq=0,HWError=0;
  bTimer->findCycle();
  if(bTimer->BTstate){ // timer in manual mode one time write data
    printf("Start write b-timer parameters\n");
    HWError=bTimer->WriteBTimerData(0xffffff);
    if(HWError) {
      syslog(LOG_NOTICE, "B-timer write data error (code=0x%x )",HWError);
      printf("B-timer write data error (code=0x%x )\n",HWError);
    }
    else{
      printf("All parameters of B-timer was writen successfull!\n");
    }
    syslog(LOG_NOTICE, "Task %s write data into HW and terminated.",DAEMON_NAME);
    printf("Task %s write data into HW and terminated.\n",DAEMON_NAME);
    bTimer->exportHWStatus(HWError);
    bTimer->exportStatus();
    exit(EXIT_SUCCESS);
  }
  else
    while(1){ //timer in remoute control mode
      if(db_process(&update)) { // delay 300 ms
	    writeHWReq=1;
        if(HWError){
          // rewrite all in next update cycle!!! if was error in last write cycle
          update=0xffffff; //update all parameters of b-timer
        }
      }
      bTimer->findCycle();
      if(writeHWReq){
        bTimer->getDataFromDB();
        bTimer->ValuesAnalyse();
        HWError=0;
        if(update){
          printf("Start write b-timer parameters\n");
          HWError=bTimer->WriteBTimerData(update);
          if(HWError) {
            syslog(LOG_NOTICE, "HW write b-timer data error (code=0x%x )",HWError);
            printf("HW write b-timer data  error (code=0x%x )\n",HWError);
          }
          else{
            printf("All parameters of b-timer was writen successfull!\n");
          }
        }
	    writeHWReq=0;  
      }
      set_state(HWError);
      sh_put_meas_er(0);  // подтверждение обновления измерений в БД
      bTimer->exportHWStatus(HWError);
      bTimer->exportStatus();
    }
    return retval;
}
    catch(...)
    {
      syslog( LOG_NOTICE, "Global Exeption" );
      closelog();
    }
}

