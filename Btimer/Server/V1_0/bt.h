//---------------------------------------------------------------------------
#ifndef TI_H
#define TI_H
#include <QString>
#include <stdio.h>
#include <math.h>
#include <syslog.h>
#include "serial.h"

#define BT_ADDR "01"
#define MESS_OK                 0x0
#define MESS_BTIMER_ERR_BCH     0x1
#define MESS_BTIMER_ERR_ONOFF   0x2
#define MESS_BTIMER_ERR_B1      0x4
#define MESS_BTIMER_ERR_B2      0x8
#define MESS_BTIMER_ERR_BMF    0x10
#define MESS_BTIMER_ERR_IMF    0x20
#define MESS_BTIMER_ERR_BTF    0x40
#define MESS_BTIMER_ERR_ITF    0x80
#define MESS_BTIMER_ERR       0x100
#define MESS_BTIMER_ERR_INTC  0x200
#define MESS_BTIMER_ERR_INT   0x400


#define  REPCOM 3
#define  TIMEOUT_CYCLE 11

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
extern "C" void set_state(short my_cy);
//---------------------------------------------------------------------------
// Btimer  procedure
class TBt
{
  public:

  unsigned char comPort;
  int baudRate;
  char buf[64];
  int BTstate;

  SerialGate sg;
  TBt(unsigned char ComP,int Boud) ; // constructor
  void ValuesAnalyse(void); // analyse input values 
  int  initialSystem(void);// initialization of system after all object has been created
  int  WriteBTimerData(unsigned int update);// write data into timer HW
  void findCycle(void); // wait pulse Ext
  int  getStatus(void);// get status from system
  void getDataFromDB(void); 
  void IniFileRead();
  void exportManualCtrl(int ctrl);    // export in ini manual control
  void exportStatus(void);   // export in ini file state of B-timer
  void exportHWStatus(int status); // export in ini file HW state

private:
  double cShH,cK,cShAdc;
  int cShInt; // shift for integrator of B-timer
  int cInt; // постоянная интегрирования ( величина одного Эрстеда в FPGA)
  int bStatus; // status of b-timer 7-FPGA error,6-Reset
// Rs232 function
  void OpenCom(void);
  void CloseCom(void);
  void writeSerial(char *);
  int readSerial(void);
  
  int execCmdStr(char* addrStr,char*cmdStr, bool answ=false, bool retData=false, int *data=0);
  int resetBt(void);
  int setChTime(int ch,double timeS);
  int setChEn(int chEn);
  int setCWA(int cw);
  int setCWB(int cw);
  int setImitatorTime(int ch, double timeS);
  int setImitatorRegime(int regime);
  QString iniFileName;

} ;

#endif
