//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <string.h>
#include <math.h>
#include <time.h>
#include <cerrno>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include <ctime>

#include "db.h"
#include "meBehav.h"

#define COM_TIMEOUT 0.1
#define COM_WAIT    0.005

extern struct measSetData cur_par;

//---------------------------------------------------------------------------


// constructor
TMeasH::TMeasH(QCoreApplication *core,unsigned char ComP,int Baud)
{
  coreApp=core;
  comPort=ComP;
  baudRate=Baud;  
  iniFileName=QString("/home/serg/.config/IHEP/measH.conf");
  IniFileRead();
  tout1=tout1req*1000;
  tout2=tout2req*1000;
  start1=start1req;
  start2=start2req;
  initStatus=INIT_INPROG;
  localeSt=0;
  updateTempl=0;
  meas1Wrong=0;
  meas2Wrong=0;
  newMeasData=0;
  abort=false;
  timer1Absent=0;
  timer2Absent=0;
  timer1Timeout=new QTimer();
  timer2Timeout=new QTimer();
  //timer1=new QTimer(); timer2=new QTimer();
  connect(timer1Timeout,SIGNAL(timeout()),this,SLOT(timer1ToutEvent()));
  connect(timer2Timeout,SIGNAL(timeout()),this,SLOT(timer2ToutEvent()));
  for(int i=0;i<ALLREQSTATES;i++) allStates[i]=READY;
  phase = INITIAL_STATE;
  nextPhase=READY;
}
TMeasH::~TMeasH()
{
  qDebug()<<"Start close HW of device procedure.";
  qDebug()<<"Off timers.";
  //timer1->stop();
  //timer2->stop();
  timer1Timeout->stop();
  timer2Timeout->stop();
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
  wait();
  qDebug()<<"Close GPIO descriptors.";
  close_gpios();
  qDebug()<<"All HW of device was closed.";
  exit(0);
}

// qDebug operator owerwrite for print states in debug mode
QDebug operator <<(QDebug dbg, const CPhase &t)
{
  dbg.nospace() <<"STATE=";
  switch(t){
  case READY: dbg.space()                  << "READY" ; break;
  case INITIAL_STATE: dbg.space()          << "INITIAL_STATE" ; break;
  case PULSE_B2_STATE: dbg.space()         << "PULSE_B2_STATE"; break;
  case PULSE_KC1_STATE: dbg.space()        << "PULSE_KC1_STATE" ; break;
  case MEAS1_BEG_STATE: dbg.space()        << "MEAS1_BEG_STATE" ; break;
  case MEAS1_END_STATE: dbg.space()        << "MEAS1_END_STATE" ; break;
  case MEAS2_BEG_STATE: dbg.space()        << "MEAS2_BEG_STATE" ; break;
  case MEAS2_END_STATE: dbg.space()        << "MEAS2_END_STATE" ; break;
  case DATA_UPDATE_STATE: dbg.space()      << "DATA_UPDATEDB_STATE" ; break;
  default:  dbg.space()                    << "UNKNOWN_STATE" ; break;
  }
  return dbg.nospace() ;//<< endl;;
}

//-----------------------------------------------------------------------------
//--- Run process. Main cycle with state machine
//-----------------------------------------------------------------------------
void TMeasH::run()
{
  CPhase deb=READY;//for debug only
  QEventLoop *loop ;
  loop= new QEventLoop();

  //msleep(1000); // wait if devise turn on in this time
  //QTimer::singleShot(6000,this,SLOT(timer1ToutEvent()));
  while(!abort) { // run until destructor not set abort
    //msleep(1000);
    loop->processEvents();
    //loop->exec();
    //loop->exit();
    mutex.lock();
    if(phase==READY){
      for(int i=0;i<ALLREQSTATES;i++) { // read all statese request and run if state!= READY state. high priority has low index
        if(allStates[i]!=READY){
          phase=allStates[i];
          allStates[i]=READY;
          break;
        }
      }
    }
    mutex.unlock();
    if(deb!=phase) { qDebug()<<phase;deb=phase;}


    switch(phase) {
      case READY: {
        //QApplication::processEvents();
        // get events
        unsigned int ev=polling_gpios(1); // 10 ms polling
        if(ev==1) allStates[PULSE_B2_STATE]=PULSE_B2_STATE;
        if(ev==2) allStates[PULSE_KC1_STATE]=PULSE_KC1_STATE;
        break;
      }//end case READY:

    case INITIAL_STATE: { // connect to USB device
      initStatus=initialSystem();
      allStates[INITIAL_STATE]=READY;
      phase = READY;
      break;
    }// end case INITIAL_STATE
    case DATA_UPDATE_STATE: { // connect to USB device
      WriteData(updateTempl);
      allStates[DATA_UPDATE_STATE]=READY;
      phase = READY;
      break;
    }// end case DATA_UPDATE_STATE

// Sample device request from timer
      case PULSE_B2_STATE: {measPulse_gpios();
        allStates[PULSE_B2_STATE]=READY; // reset state
        if(start1==0) { //ch1 start from B2
          //QTimer::singleShot(tout1,this,SLOT(timer1Event()));
          timer1Timeout->start(TIMEOUT_CYCLE*1000); //in ms restart timeout time
        }
        if(start2==0) { //ch2 start from B2
          //QTimer::singleShot(tout2,this,SLOT(timer2Event()));
          timer2Timeout->start(TIMEOUT_CYCLE*1000); //in ms restart timeout time
        }
        phase = READY; // set next phase from stack (for work with continue process in state machine
        allStates[MEAS1_BEG_STATE]=MEAS1_BEG_STATE;
        break;
      }//end case PULSE_B2_STATE:
      case PULSE_KC1_STATE: {
        allStates[PULSE_KC1_STATE]=READY; // reset state
        if(start1==1) { //ch1 start from KC1
          QTimer::singleShot(tout1,this,SLOT(timer1Event()));
          timer1Timeout->start(TIMEOUT_CYCLE*1000); //in ms restart timeout timer
        }
        if(start2==1) { //ch2 start from KC1
          QTimer::singleShot(tout2,this,SLOT(timer2Event()));
          timer2Timeout->start(TIMEOUT_CYCLE*1000); //in ms restart timeout time
        }
        phase = READY; // set next phase from stack (for work with continue process in state machine f
        break;
      }//end case PULSE_KC1_STATE:
      case MEAS1_BEG_STATE: {
        measPulse_gpios();
        timer1Absent=0;
        allStates[MEAS1_BEG_STATE]=READY; // reset state
        QTimer::singleShot(TIMEOUT_MEAS,this,SLOT(meas1Event()));
        newMeasData&=0xfe;  // reset 0 - bit
        phase = READY; // set next phase from stack (for work with continue process in state machine
        break;
      }//end case MEAS1_BEG_STATE:
      case MEAS2_BEG_STATE: {
        measPulse_gpios();
        timer1Absent=0;
        allStates[MEAS2_BEG_STATE]=READY; // reset state
        QTimer::singleShot(TIMEOUT_MEAS,this,SLOT(meas2Event()));
        newMeasData&=0xfd; // reset 1 - bit
        phase = READY; // set next phase from stack (for work with continue process in state machine
        break;
      }//end case MEAS2_BEG_STATE
      case MEAS1_END_STATE: {measPulse_gpios();
        allStates[MEAS1_END_STATE]=READY; // reset state
        // read data from RS485
        meas1Wrong=0; // or set wrong if reciver error
        newMeasData|=0x1;  // set 0 - bit
        phase = READY; // set next phase from stack (for work with continue process in state machine
        break;
      }//end case MEAS1_END_STATE
      case MEAS2_END_STATE: {
        allStates[MEAS2_END_STATE]=READY; // reset state
        // read data from RS485
        meas2Wrong=0; // or set wrong if reciver error
        newMeasData|=0x2;  //set 1 - bit
        phase = READY; // set next phase from stack (for work with continue process in state machine
        break;
      }//end case MEAS2_END_STATE
      default:;
    }
    if(abort) {loop->exit(); return;}
  }
  loop->exit();
}
// update data and write in next cycle
void TMeasH::writeHWReq(unsigned int update)
{
  mutex.lock();
  updateTempl=update;
  allStates[DATA_UPDATE_STATE]=DATA_UPDATE_STATE;
  mutex.unlock();
}

//measNum 1,2
void TMeasH::readMeasReq(unsigned int measNum,char *status, double *val )
{
  mutex.lock();
  if(measNum==1){ // meas 1
    *status=0;
    newMeasData&=0xfe;  // reset 0 - bit
    if(timer1Absent) { *status=MESS_ERR_SYNC; *val=0.0; }
    else if(meas1Wrong)   { *status=MESS_ERR_REC; *val=0.1;  }
  }
  else if(measNum==2) { // meas 2
    *status=0;
    newMeasData&=0xfd;  // reset 1 - bit
    if(timer2Absent) { *status=MESS_ERR_SYNC; *val=0.0; }
    else if(meas2Wrong)   { *status=MESS_ERR_REC; *val=0.1;  }
  }
  mutex.unlock();
}

void TMeasH::OpenCom(void)
{
  if(!sg.Open(comPort,baudRate)) {
    perror("System error! Com port can't open!");
  }
}


void TMeasH::CloseCom(void)
{
  sg.Close() ;
}

void TMeasH::writeSerial(char *sendStr )
{
  int len=0;
  char *tmpStr;
  tmpStr=sendStr;
  while(*tmpStr++) len++;
  len++; // 0 char send too
  sg.Send(sendStr,len); // write chars
}

int TMeasH::readSerial(void)
{
  int cnt ;
  
  cnt=sg.Recv(&buf[0],64); //read chars

  return(cnt);
}
// ------------------------------------------------------------------------------
int TMeasH::execCmdStr(char* addrStr,char* cmdStr, bool answ,bool retData, int *data)
{
  char buf[255],*pbuf,sbuf[255];

  int tmp,ret=2,n=REPCOM,z;
  clock_t begTime;
  double sec;
  char cmdBuf[64],answOk[64],mcuAnsw[64],getDataTempl[64];
  sprintf(cmdBuf,"%s:%s",addrStr,cmdStr);
  sprintf(answOk,"%s>OK",addrStr);
  sprintf(mcuAnsw,"%s>",addrStr);
  sprintf(getDataTempl,"%s>%%d",addrStr);
  printf("  Exec cmd %s\n",cmdBuf);

  while((ret>0)&&((n--)!=0)){
    buf[0]=0;
    pbuf=buf;
    if(!sg.Open(comPort,baudRate)) {
      perror("System error! Com port can't open!");
      ret=EXIT_FAILURE;
      break;
    }
    sg.Clean();
    writeSerial((char*)cmdBuf);
    begTime=clock();
    sec=0;
    z=0;
    do{
      while(((tmp=sg.Recv(pbuf,1))==0)&&(sec<COM_TIMEOUT)){
        z++;
        sec=((double)(clock()-begTime))/(double)CLOCKS_PER_SEC;
      }
      if(sec>=COM_TIMEOUT) break;
    }
    while(*pbuf++);
    printf("  Spend sec=%lf wait %d cycle\n",sec,z);
    sg.Close();
    if(sec>COM_TIMEOUT){
      errno=62; //sys_errlist[62]="Timer expired"
      sprintf(sbuf,"  Timeout in function TBt::execCmdStr rep=%d",n);
      printf("%s\n",sbuf);
      perror(sbuf);
      ret=EXIT_FAILURE;
    }
    else {
      if(!answ){ // without answer only OK
        if(strncmp((const char*)buf,answOk,5)) {
          errno=6; //sys_errlist[6]= "No such device or address"
          sprintf(sbuf,"  Bad answer on OK request from MCU AtMega162 in function TBt::execCmdStr rep=%d. MCU return '%s'",n,buf);
          printf("%s\n",sbuf);
          perror(sbuf);
          ret=EXIT_FAILURE;
        }
        else{
          ret=EXIT_SUCCESS;
        }
      }
      else {// with answer
        if(strncmp((const char*)buf,mcuAnsw,3)) {
          errno=6; //sys_errlist[6]= "No such device or address"
          sprintf(sbuf,"  Bad answer from MCU AtMega162 in function TBt::execCmdStr rep=%d. MCU return '%s'",n,buf);
          //printf("%s\n",sbuf);
          perror(sbuf);
          ret=EXIT_FAILURE;
        }
        else{
          sscanf(buf,getDataTempl,&ret); //if all OK read 0 (EXIT_SUCCESS)
          if(retData) { *data=ret; ret=EXIT_SUCCESS;}
          if(ret!=EXIT_SUCCESS){
            errno=5; // sys_errlist[5]= "Input/output error
            sprintf(sbuf,"  Data don't write in function TBt::execCmdStr rep=%d. MCU return '%s'.",n,buf);
            printf("%s\n",sbuf);
            perror(sbuf);
          }
        }
      }
    }
    begTime=clock();
    sec=0;
    while(sec<COM_WAIT) sec=((double)(clock()-begTime))/(double)CLOCKS_PER_SEC; // wait
  }
  if(ret==EXIT_SUCCESS) printf("  Exec cmd %s - OK\n",cmdBuf);
  else
    printf("! Exec cmd %s - FAILURE\n",cmdBuf);

  return(ret);
}

//-------------------------------------------------------------------------------

//------------------------------------------------------ Common function


int TMeasH::initialSystem(void)
{
  char str[100];

  printf("Begin initialisation of GPIO.\n");
  syslog(LOG_NOTICE,"Begin initialisation of GPIO.");
  if(init_gpios())
  { //fatal error
    sprintf(str,"Fatal error of measH (GPIO don't initialize). Program was terminated!");
    printf("%s\n",str);
    syslog(LOG_NOTICE,"Fatal error of measH (GPIO don't initialize). Program was terminated!");
    //sh_put_mess(str);
    //set_state(MESS_ERR_MEAS); // error in measH
    //sh_put_meas_er(0x4010);// HW error
    //sleep(1);
    return(INIT_ERROR);
  }
  printf("Initialisation of GPIO is successfull!\n");
  syslog(LOG_NOTICE,"Initialisation of GPIO  is successfull!");

  printf("Initialisation of serial device!\n");
  syslog(LOG_NOTICE,"Initialisation of serail device.");
  sleep(1);
  timer1Timeout->start(TIMEOUT_CYCLE*1000); //in ms
  timer2Timeout->start(TIMEOUT_CYCLE*1000); //in ms
  printf("Initialisation of serial device is successfull!\n");
  syslog(LOG_NOTICE,"Initialisation of serail is successfull!");
  return(INIT_OK);
}




//---------------------------------------------------------
void  TMeasH::WriteData(unsigned int update)
{

mutex.lock();
  printf("Write data\n");

 // QSettings *Ini;
  //Ini = new QSettings(iniFileName,QSettings::NativeFormat) ;
 // Ini->beginGroup("measHPar");

  if((update)&1){ // time 1 ch
    //Ini->setValue("t1",cur_par.tim[0]);
    tout1req=cur_par.tim[0];
    tout1=tout1req*1000; // from s to ms
    qDebug()<<"set t1"<<tout1req;
  }
  if((update>>1)&1){ // time 2 ch
    //Ini->setValue("t2",cur_par.tim[1]);
    tout2req=cur_par.tim[1];
    tout2=tout2req*1000; // from s to ms
    qDebug()<<"set t2"<<tout2req;
  }
  if((update>>2)&1){ // start 1 ch
    //Ini->setValue("start1",(int)cur_par.pulseStart[0]);
    start1req=(int)cur_par.pulseStart[0];
    start1=start1req;
    qDebug()<<"set start1"<<start1req;
  }
  if((update>>3)&1){ // start 2 ch
    //Ini->setValue("start2",(int)cur_par.pulseStart[1]);
    start2req=(int)cur_par.pulseStart[1];
    start2=start2req;
    qDebug()<<"set start2"<<start2req;
  }
  //Ini->endGroup();
  //delete Ini;

  //fflush(stderr);
  //fflush(stdout);
mutex.unlock();
}
//--------------------------------------------------------------------
void TMeasH::IniFileRead()
{
  QSettings *Ini ;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat);

  QString current="measHPar";
  Ini->beginGroup(current);

  cK=Ini->value("cK",95.966).toDouble();

  cur_par.tim[0]=Ini->value("t1",1).toDouble();
  cur_par.tim[1]=Ini->value("t2",2).toDouble();
  cur_par.pulseStart[0]=(char)Ini->value("start1",0).toInt();
  cur_par.pulseStart[1]=(char)Ini->value("start2",0).toInt();
  tout1req=cur_par.tim[0];
  tout2req= cur_par.tim[1];
  start1req=cur_par.pulseStart[0];
  start2req=cur_par.pulseStart[1];
  Ini->endGroup();
  delete Ini ;
}

//-----------------------------------------------------------------------------
void TMeasH::exportManualCtrl(int ctrl)
{
  mutex.lock();
  localeSt=ctrl;
  QSettings *Ini;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat) ;
  QString current="Control";
  Ini->beginGroup(current);
  Ini->setValue("ManualMode",ctrl);
  Ini->endGroup();
  delete Ini;
  mutex.unlock();
}
// public slots
// event on finish count timers 1
void TMeasH::timer1Event(void)
{
  // set out 1 for meas
  // set out 0
  mutex.lock();  allStates[MEAS1_BEG_STATE]=MEAS1_BEG_STATE;
 // qDebug()<<"1 ev";
  mutex.unlock();
}
// event on finish count timers 2
void TMeasH::timer2Event(void)
{
  // set out 1 for meas
  // set out 0
  mutex.lock();
 // qDebug()<<"2 ev";
  allStates[MEAS2_BEG_STATE]=MEAS2_BEG_STATE;
  mutex.unlock();
}
// event on finish count timer1
void TMeasH::timer1ToutEvent(void)
{
  mutex.lock();
  timer1Absent=1; //Absent timer1 pulse
  newMeasData|=1;
 // qDebug()<<"TOUT1";
  mutex.unlock();
}
// event on finish count timers2
void TMeasH::timer2ToutEvent(void)
{
  mutex.lock();
  timer2Absent=1; // absent timer2 pulse
  newMeasData|=2;
 // qDebug()<<"TOUT2";
  mutex.unlock();
}

void TMeasH::meas1Event(void)
{
  mutex.lock();
 // qDebug()<<"me 1";
  allStates[MEAS1_END_STATE]=MEAS1_END_STATE;
  mutex.unlock();
}
void TMeasH::meas2Event(void)
{
  mutex.lock();
 // qDebug()<<"me 2";
  allStates[MEAS2_END_STATE]=MEAS2_END_STATE;
  mutex.unlock();
}
