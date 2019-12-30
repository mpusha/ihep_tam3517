//---------------------------------------------------------------------------
#ifndef MEH_H
#define MEH_H
#include <QtCore/QCoreApplication>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDebug>
#include <QWaitCondition>
#include <stdio.h>
#include <math.h>
#include <syslog.h>
#include "serial.h"


#define MESS_OK           0x0
#define MESS_ERR_SYNC     0x81  // ошибка синхронизации. Синхропалс не пришел
#define MESS_ERR_REC      0x82  // ошибка приемника
#define MESS_ERR_MEAS     0x83  // ошибка измерений (HW error GPIO e.g.)

#define INIT_OK     0x0  // sucs initial device
#define INIT_ERROR  0x1  // error in initialisation
#define INIT_INPROG 0x2  // initialisation in progress

#define  REPCOM 3
#define  TIMEOUT_CYCLE 11
#define  TIMEOUT_MEAS  80

#define  NC_PRESENT 0x01
#define  EXT_PRESENT 0x20
#define  RST_PRESENT 0x40

extern "C" int configureFPGA(char * fName);

extern "C" int db_init(void);
extern "C" int db_process(unsigned int *update);
//extern "C" void updateMeas(void);
extern "C" short sh_put_meas_er(short er_code);
extern "C" short sh_put_data_er(short er_code);
extern "C" void sh_put_mess(char *mes);
extern "C" void set_state(unsigned int num, double *field, char *state);

extern "C" int init_gpios(void) ;
extern "C" void close_gpios(void);
extern "C" unsigned int polling_gpios(int);
extern "C" void measPulse_gpios(void);

typedef enum
{
  PULSE_B2_STATE,
  PULSE_KC1_STATE,
  MEAS1_BEG_STATE,
  MEAS1_END_STATE,
  MEAS2_BEG_STATE,
  MEAS2_END_STATE,
  DATA_UPDATE_STATE,
  ALLREQSTATES,    //limiter on process request states
    READY,
    INITIAL_STATE
}CPhase;  // phases of state machine

//---------------------------------------------------------------------------

class TMeasH  : public QThread
{
  Q_OBJECT

  public:
  TMeasH(QCoreApplication *core,unsigned char ComP,int Boud) ; // constructor
  ~TMeasH();

  unsigned char comPort;
  int baudRate;
  char buf[64];
  int localeSt;

  int getInitStatus(void) { return initStatus ; }
  int haveNewMeasDataReq(void) { return newMeasData; }
  void writeHWReq(unsigned int update) ;
  void readMeasReq(unsigned int measNum, char *status, double *val );
  SerialGate sg;

  //void ValuesAnalyse(void); // analyse input values
  int  initialSystem(void);// initialization of system after all object has been created


  void IniFileRead();
  void exportManualCtrl(int ctrl);    // export in ini manual control


protected:
    void run();

private:
  QCoreApplication *coreApp;
  QTimer *timer1Timeout,*timer2Timeout;
  int timer1Absent,timer2Absent;
  bool abort;
  double cK;
  unsigned int updateTempl;
  int meas1Wrong,meas2Wrong;
  int initStatus; // status initialisation procedure
  double tout1req,tout2req;
  int start1req,start2req;
  int tout1,tout2,start1,start2;
  int newMeasData;
// Rs232 function
  void OpenCom(void);
  void CloseCom(void);
  void writeSerial(char *);
  int readSerial(void);
  
  int execCmdStr(char* addrStr,char*cmdStr, bool answ=false, bool retData=false, int *data=0);
  void WriteData(unsigned int update);// write data into timer HW

  QString iniFileName;
  QMutex mutex;
  QWaitCondition condition;
  CPhase phase,nextPhase;
  CPhase allStates[ALLREQSTATES];

public slots:
  void timer1Event(void);
  void timer2Event(void);
  void timer1ToutEvent(void);
  void timer2ToutEvent(void);
  void meas1Event(void);
  void meas2Event(void);

} ;

#endif
