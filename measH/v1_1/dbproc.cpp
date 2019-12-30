#include "dbproc.h"

extern TMeasH *meas;
TDbProc::TDbProc(void)
{
  abort=false;
}

TDbProc::~TDbProc()
{
  qDebug()<<"Start close db.";
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
  wait();
  qDebug()<<"Close dbs.";
}
//-----------------------------------------------------------------------------
//--- Run process. Main cycle with state machine
//-----------------------------------------------------------------------------
void TDbProc::run()
{
  QEventLoop *loop ;
  loop= new QEventLoop();
  unsigned int update,newData=0;
  int writeHWReq=0;
  double field=0;
  char st=0;
  printf("Start processing DB\n");
  while(!abort) { // run until destructor not set abort
    loop->processEvents();
    msleep(300);
    if(db_process(&update)) { // delay 300 ms
      writeHWReq=1;
    }
    if(writeHWReq){
      if(update){
        printf("Start write measH parameters update %0x\n",update);
        meas->writeHWReq(update);
        printf("All parameters  was writen successfull!\n");
      }
      writeHWReq=0;
    }
    newData=meas->haveNewMeasDataReq();

    if(newData&1) {
      meas->readMeasReq(1,&st,&field);
      printf("ch1 st %0x data %e\n",st,field);
      set_state(1,&field,&st);
    }
    if(newData&2) {
      meas->readMeasReq(2,&st,&field);
      printf("ch2 st %0x data %e\n",st,field);
      set_state(2,&field,&st);
    }
  }
  printf("End processing DB");
}

unsigned int TDbProc::dbInit()
{
  unsigned int sts=db_init();//initialization of DB 0 - if Ok.
  if(sts==0) sh_put_mess((char*)"Task measH is starting.");
  return(sts);
}
