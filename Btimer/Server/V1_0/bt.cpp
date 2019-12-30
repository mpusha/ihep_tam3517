//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

//#include <QMessageBox>
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
#include "bt.h"

#define COM_TIMEOUT 0.1
#define COM_WAIT    0.005

//extern double cur_t[24];
//extern char cur_OO[24];
//extern char cur_Strt[24];
extern struct Btimer cur_par;

//---------------------------------------------------------------------------


// constructor
TBt::TBt(unsigned char ComP,int Baud)
{
  comPort=ComP;
  baudRate=Baud;  
  iniFileName=QString("/home/work/.config/IHEP/btu70.conf");
  BTstate=0;
  bStatus=0;
}

void TBt::OpenCom(void)
{
  if(!sg.Open(comPort,baudRate)) {
    perror("System error! Com port can't open!");
  }
}


void TBt::CloseCom(void)
{
  sg.Close() ;
}

void TBt::writeSerial(char *sendStr )
{
  int len=0;
  char *tmpStr;
  tmpStr=sendStr;
  while(*tmpStr++) len++;
  len++; // 0 char send too
  sg.Send(sendStr,len); // write chars
}

int TBt::readSerial(void)
{
  int cnt ;
  
  cnt=sg.Recv(&buf[0],64); //read chars

  return(cnt);
}
// ------------------------------------------------------------------------------
int TBt::execCmdStr(char* addrStr,char* cmdStr, bool answ,bool retData, int *data)
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

// Reset timer FPGA
int TBt::resetBt(void)
{
  execCmdStr((char*) BT_ADDR,(char*)"RF"); // reset FPGA
  return(execCmdStr((char*) BT_ADDR,(char*)"RS")); // reset status register
}


//-------------------------------------------------------------------------------

//------------------------------------------------------ Common function

void TBt::ValuesAnalyse(void)
{
}
int TBt::initialSystem(void)
{
  int status_hw;
  char str[100];

  printf("Begin configuration and reset of Btimer FPGA.\n");
  syslog(LOG_NOTICE,"Begin configuration and reset of Btimer FPGA.");
  if((status_hw=configureFPGA((char*)"/home/work/btimer.rbf"))){ //fatal error
    sprintf(str,"Fatal error of BtimerU70 (Btimer FPGA configuration error [0x%x]). Program was terminated!",status_hw);
    printf("%s\n",str);
    syslog(LOG_NOTICE,"Fatal error of BtimerU70 (Btimer FPGA configuration error [0x%x]). Program was terminated!",status_hw);
    sh_put_mess(str);
    set_state(MESS_BTIMER_ERR); // error in b-timer
    sh_put_meas_er(0x4010);// HW error
    sleep(1);
    return(EXIT_FAILURE);
  }
  printf("Configuration of Btimer FPGA is successfull!\n");
  syslog(LOG_NOTICE,"Configuration of Btimer FPGA is successfull!");
  sleep(1);
  if(resetBt()){
    printf("Reset Btimer FPGA is BAD! Program was terminated!\n");
    syslog(LOG_NOTICE,"Reset Btimer FPGA is BAD! Program was terminated!");
    sleep(1);
    return(EXIT_FAILURE);
  }
  else {
    printf("Reset of Btimer FPGA is successfull!\n");
    syslog(LOG_NOTICE,"Reset of Btimer FPGA is successfull!");
  }
  return(EXIT_SUCCESS);
}

void TBt::getDataFromDB(void)
{
}
void TBt::findCycle(void)
{
  const char *nM[]={"HC","KC1","B1","B2","AlarmHC","External","Reset"};
  const char *oN[]={"Off","On"};
  int timeout;
  timeout=0;
  printf("Waiting Ext\n");
  do{
    if(getStatus()==EXIT_SUCCESS)
      if(bStatus&EXT_PRESENT) { printf("Ext present\n"); break; }
    sleep(1);
  }while(timeout++<TIMEOUT_CYCLE);
  printf("Ext or timeout found. Status (0x%x)\n",bStatus);
  int stt=bStatus,indx;
  for(int ist=0;ist<7;ist++){
    indx=stt&1;
    printf("(%s %s)",nM[ist],oN[indx]);
    stt>>=1;
  }
  printf("\n");
  execCmdStr((char*) BT_ADDR,(char*)"RS");
}

//0-B1, 1-B2, 2-7 B-timer channels, 8 - B middle flat, 9 - I middle flat,
//10 - B top flat, 11 - I top flat, 12 - onOff map
//---------------------------------------------------------
int  TBt::WriteBTimerData(unsigned int update)
{
  //const char *nM[]={"HC","KC1","B1","B2","AlarmHC","External","Reset"};
  //const char *oN[]={"Off","On"};
  int error=MESS_OK,timeout=0;
/*
  execCmdStr((char*) BT_ADDR,(char*)"RS");
  printf("Waiting Ext\n");
  do{
    if(getStatus()==EXIT_SUCCESS)
      if(bStatus&EXT_PRESENT) { printf("Ext present\n"); break; }
    sleep(1);
  }while(timeout++<TIMEOUT_CYCLE);
  printf("Ext or timeout found. Status (0x%x)\n",bStatus);
  execCmdStr((char*) BT_ADDR,(char*)"RS");
*/
  printf("Write data\n");

  QSettings *Ini;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat) ;

  Ini->beginGroup("BTParameters");
  if(update&0x2000) update |=0x33003f; // if change b2 recalc all B ch settings
  int updateL=update;
  char tmp[20];
  // write integrator 1 Gauss value & b-timer ADC(B) shift parameter
  if(update=0xffffff){
    sprintf(tmp,"WW 14 %d",cInt);
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_INT;
    sprintf(tmp,"WW 15 %d",cShInt);
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_INTC;
  }

  for(int i=0;i<6;i++) { // write B timer values
    if(updateL&0x1){
      Ini->setValue(QString("BCh%1").arg(i+1),cur_par.fBt[i]);
      sprintf(tmp,"WW %d %d",i+2,(int)(cur_par.fBt[i]-cur_par.fB1_2[1]));
      if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_BCH;
    }
    updateL>>=1;
  }
  if(update&0xcccfc0){ // write all timer chanels enable status
    int mask=0;
    mask=cur_par.onOffBt[0]|(((int)cur_par.onOffBt[1])<<1)|(((int)cur_par.onOffBt[2])<<2)|
         (((int)cur_par.onOffBt[3])<<3)|(((int)cur_par.onOffBt[4])<<4)|(((int)cur_par.onOffBt[5])<<5)|
         (((int)cur_par.onOffB1_2[0])<<6)|(((int)cur_par.onOffB1_2[1])<<7)|
         (((int)cur_par.onOffBIMiddle[0])<<8)|(((int)cur_par.onOffBIMiddle[1])<<9)|
         (((int)cur_par.onOffBITop[0])<<10)|(((int)cur_par.onOffBITop[1])<<11);
    Ini->setValue("OnOffCh",mask);
    sprintf(tmp,"WW 12 %d",mask);
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_ONOFF;
  }
  if((update>>12)&1){ // write B1 ((B-355.7)*95.966+7698))
    Ini->setValue("B1",cur_par.fB1_2[0]);
    sprintf(tmp,"WW 0 %d",(int)((cur_par.fB1_2[0]-cShH)*cK+cShAdc));
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_B1;
  }
  if((update>>13)&1){ // write B2
    Ini->setValue("B2",cur_par.fB1_2[1]);
    sprintf(tmp,"WW 1 %d",(int)((cur_par.fB1_2[1]-cShH)*cK+cShAdc));
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_B2;
  }
  if((update>>16)&1){ // write BMiddle flat
    Ini->setValue("BMiddleFlat",cur_par.fBIMiddle[0]);
    sprintf(tmp,"WW 8 %d",(int)(cur_par.fBIMiddle[0]-cur_par.fB1_2[1]));
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_BMF;
  }
  if((update>>17)&1){ // write I middle flat
    Ini->setValue("IMiddleFlat",cur_par.fBIMiddle[1]);
    sprintf(tmp,"WW 9 %d",(int)(cur_par.fBIMiddle[1]-cur_par.fB1_2[1]));
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_IMF;
  }
  if((update>>20)&1){ // write B top flat
    Ini->setValue("BTopFlat",cur_par.fBITop[0]);
    sprintf(tmp,"WW 10 %d",(int)(cur_par.fBITop[0]-cur_par.fB1_2[1]));
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_BTF;
  }
  if((update>>21)&1){ // write I top flat
    Ini->setValue("ITopFlat",cur_par.fBITop[1]);
    sprintf(tmp,"WW 11 %d",(int)(cur_par.fBITop[1]-cur_par.fB1_2[1]));
    if(execCmdStr((char*) BT_ADDR,(char*) tmp,true)) error|=MESS_BTIMER_ERR_ITF;
  }
  Ini->endGroup();
  delete Ini;
  /*
  timeout=0;
  printf("Waiting Ext\n");
  do{
    if(getStatus()==EXIT_SUCCESS)
      if(bStatus&EXT_PRESENT) { printf("Ext present\n"); break; }
    sleep(1);
  }while(timeout++<TIMEOUT_CYCLE);
  printf("Ext or timeout found. Status (0x%x)\n",bStatus);
  int stt=bStatus,indx;
  for(int ist=0;ist<7;ist++){
    indx=stt&1;
    printf("(%s %s)",nM[ist],oN[indx]);
    stt>>=1;
  }
  printf("\n");
  */
  //execCmdStr((char*) BT_ADDR,(char*)"RS");
  fflush(stderr);
  fflush(stdout);
  return(error);
}
//--------------------------------------------------------------------
void TBt::IniFileRead()
{
  int i;
  QSettings *Ini ;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat);

  QString current="BTParameters";
  Ini->beginGroup(current);

  cShH=Ini->value("cShH",353.7).toDouble();
  cK=Ini->value("cK",95.966).toDouble();
  cShAdc=Ini->value("cShAdc",7698).toDouble();
  cShInt=Ini->value("cShInt",8165).toInt();
  cInt=Ini->value("cInt",33305).toInt();

  cur_par.fBt[0]=Ini->value("BCh1",7000).toDouble();
  cur_par.fBt[1]=Ini->value("BCh2",7000).toDouble();
  cur_par.fBt[2]=Ini->value("BCh3",7000).toDouble();
  cur_par.fBt[3]=Ini->value("BCh4",7000).toDouble();
  cur_par.fBt[4]=Ini->value("BCh5",7000).toDouble();
  cur_par.fBt[5]=Ini->value("BCh6",7000).toDouble();

  int OnOff=Ini->value("OnOffCh", 0).toInt();
  for(i=0;i<6;i++){
    cur_par.onOffBt[i]=OnOff&1;
    OnOff>>=1;
  }
  for(i=0;i<2;i++){
    cur_par.onOffB1_2[i]=OnOff&1;
    OnOff>>=1;
  }
  for(i=0;i<2;i++){
    cur_par.onOffBIMiddle[i]=OnOff&1;
    OnOff>>=1;
  }
  for(i=0;i<2;i++){
    cur_par.onOffBITop[i]=OnOff&1;
    OnOff>>=1;
  }

  cur_par.fB1_2[0]=Ini->value("B1",380).toDouble();
  cur_par.fB1_2[1]=Ini->value("B2",354).toDouble();

  cur_par.fBIMiddle[0]=Ini->value("BMiddleFlat",4020).toDouble();
  cur_par.fBIMiddle[1]=Ini->value("IMiddleFalt",4000).toDouble();

  cur_par.fBITop[0]=Ini->value("BTopFlat",8020).toDouble();
  cur_par.fBITop[1]=Ini->value("ITopFlat",8000).toDouble();

  Ini->endGroup();
  delete Ini ;
}
//--------------------------------------------------------------------
int  TBt::getStatus(void)
{
  int status=0,st=0;
  status=execCmdStr((char*) BT_ADDR,(char*)"ST",true,true,&st);
  bStatus=st;
  return(status);
}
//-----------------------------------------------------------------------------
void TBt::exportManualCtrl(int ctrl)
{
  BTstate=ctrl;
  QSettings *Ini;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat) ;
  QString current="Control";
  Ini->beginGroup(current);
  Ini->setValue("ManualMode",ctrl);
  Ini->endGroup();
  delete Ini;
}
//------------------------------------------------------------------------------
void TBt::exportStatus(void)
{
  static int oldstatus=-1;
  if(bStatus!=oldstatus){
    oldstatus=bStatus;
    QSettings *Ini;
    Ini = new QSettings(iniFileName,QSettings::NativeFormat) ;
    QString current="Control";
    Ini->beginGroup(current);
    Ini->setValue("status",bStatus);
    Ini->endGroup();
    delete Ini;
  }
}
//--------------------------------------------------------------------------
void TBt::exportHWStatus(int status)
{
  static int oldstatusHW=-1;
  if(status!=oldstatusHW){
    oldstatusHW=status;
    QSettings *Ini;
    Ini = new QSettings(iniFileName,QSettings::NativeFormat) ;
    QString current="Control";
    Ini->beginGroup(current);
    Ini->setValue("statusHW",status);
    Ini->endGroup();
    delete Ini;
  }
}
