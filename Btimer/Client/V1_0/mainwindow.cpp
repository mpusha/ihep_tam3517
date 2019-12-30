#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QWSServer>
#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->centralWidget->setVisible(false);

  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_NoSystemBackground);

  iniFileName=QString("/home/work/.config/IHEP/btu70.conf");

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(setBTimerParameters()));

  move(-9,-9);
  scrSvr=new myScrSaver;
  QWSServer::setScreenSaver(scrSvr);
  QSettings *Ini ;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat);
  Ini->beginGroup("Control");
  timeOutScrSvr=Ini->value("scr_saver_time",300000).toInt();
  scanParTime=Ini->value("scan_time",10000).toInt();
  Ini->endGroup();
  delete Ini;
  QWSServer::setScreenSaverInterval(timeOutScrSvr);
  setBTimerParameters();
  timer->start(scanParTime);
  //QWSServer::setCursorVisible( false );
}

MainWindow::~MainWindow()
{
  delete ui;
}
void MainWindow::setBTimerParameters(void)
{
  int i,OnOff;

  QSettings *Ini ;
  Ini = new QSettings(iniFileName,QSettings::NativeFormat);

  QString current="BTParameters";

  Ini->beginGroup(current);
  BVal[0]=Ini->value("BCh0",7000).toDouble();
  BVal[1]=Ini->value("BCh1",7000).toDouble();
  BVal[2]=Ini->value("BCh2",7000).toDouble();
  BVal[3]=Ini->value("BCh3",7000).toDouble();
  BVal[4]=Ini->value("BCh4",7000).toDouble();
  BVal[5]=Ini->value("BCh5",7000).toDouble();


  OnOff=Ini->value("OnOffCh",10).toInt();
  for(i=0;i<12;i++){
    OO[i]=OnOff&1;
    OnOff>>=1; 
  }
  B1=Ini->value("B1",380).toDouble();
  B2=Ini->value("B2",354).toDouble();

  BMid=Ini->value("BMiddleFlat",4020).toDouble();
  IMid=Ini->value("IMiddleFlat",4000).toDouble();

  BTop=Ini->value("BTopFlat",8020).toDouble();
  ITop=Ini->value("ITopFlat",8000).toDouble();

  Ini->endGroup();
  Ini->beginGroup("Control");
  ctrl=Ini->value("ManualMode", 0).toInt();
  status=Ini->value("status",0).toInt();
  HWStatus=Ini->value("statusHW",0).toInt();
  Ini->endGroup();

  delete Ini ;

  showParamOnScr();
// Page 1-3 down header
  if(ctrl){
    ui->label1->setText(trUtf8("Таймер не управляется СУ У70!"));
    ui->label1->setStyleSheet("QLabel {font: bold 20px; color: rgb(244,0,0);}");
    ui->label2->setText(trUtf8("Таймер не управляется СУ У70!"));
    ui->label2->setStyleSheet("QLabel {font: bold 20px; color: rgb(244,0,0);}");
    ui->label3->setText(trUtf8("Таймер не управляется СУ У70!"));
    ui->label3->setStyleSheet("QLabel {font: bold 20px; color: rgb(244,0,0);}");
  }
  else{
    ui->label1->setText(trUtf8("Таймер работает от системы управления."));
    ui->label1->setStyleSheet("QLabel {font: bold 20px; color: rgb(0,0,0);}");
    ui->label2->setText(trUtf8("Таймер работает от системы управления."));
    ui->label2->setStyleSheet("QLabel {font: bold 20px; color: rgb(0,0,0);}");
    ui->label3->setText(trUtf8("Таймер работает от системы управления."));
    ui->label3->setStyleSheet("QLabel {font: bold 20px; color: rgb(0,0,0);}");
  }


  QString msg;
  if((HWStatus==MESS_OK)){
    msg=trUtf8("Статус B-таймера - OK");
    ui->labelStErr->setStyleSheet("QLabel {font: bold 20px; color: rgb(0,0,0);}");
  }
  if(HWStatus){
    msg=QString(trUtf8("Статус B-таймера - ошибка! Код (0x%1).")).arg(HWStatus,0,16);
    ui->labelStErr->setStyleSheet("QLabel {font: bold 20px; color: rgb(255,0,0);}");
  }
  ui->labelStErr->setText(msg);

  const char *nM[]={"HC","KC1","B1","B2","AlarmHC","Ext","Res"};
  const char *oN[]={"Off","On"};
  msg=trUtf8("Find pulses: ");
  int stt=status,indx;
  for(int ist=0;ist<7;ist++){
    indx=stt&1;
    msg+=QString(" %1-%2;").arg(nM[ist]).arg(oN[indx]);
    stt>>=1;
  }
  ui->labelPulse->setText(msg);

  ui->centralWidget->setVisible(true);
}

void MainWindow::showParamOnScr(void)
{
  QString tmp;

// B1
  tmp=QString("%1 Gs").arg(B1,5,'f',1);
  ui->Ch1Time->setText(tmp);
  if(OO[6]) ui->Ch1Time->setEnabled(true); else ui->Ch1Time->setEnabled(false);
// B2
  tmp=QString("%1 Gs").arg(B2,5,'f',1);
  ui->Ch2Time->setText(tmp);
  if(OO[7]) ui->Ch2Time->setEnabled(true); else ui->Ch2Time->setEnabled(false);
// BMid
  tmp=QString("%1 Gs").arg(BMid,7,'f',1);
  ui->Ch9Time->setText(tmp);
  if(OO[8]) ui->Ch9Time->setEnabled(true); else ui->Ch9Time->setEnabled(false);
// BMid
  tmp=QString("%1 Gs").arg(IMid,7,'f',1);
  ui->Ch10Time->setText(tmp);
  if(OO[9]) ui->Ch10Time->setEnabled(true); else ui->Ch10Time->setEnabled(false);
// BTop
  tmp=QString("%1 Gs").arg(BTop,7,'f',1);
  ui->Ch11Time->setText(tmp);
  if(OO[10]) ui->Ch11Time->setEnabled(true); else ui->Ch11Time->setEnabled(false);
// IMid
  tmp=QString("%1 Gs").arg(ITop,7,'f',1);
  ui->Ch12Time->setText(tmp);
  if(OO[11]) ui->Ch12Time->setEnabled(true); else ui->Ch12Time->setEnabled(false);
// B ch 1
  tmp=QString("%1 Gs").arg(BVal[0],7,'f',1);
  ui->Ch17Time->setText(tmp);
  if(OO[0]) ui->Ch17Time->setEnabled(true); else ui->Ch17Time->setEnabled(false);
// B ch 2
  tmp=QString("%1 Gs").arg(BVal[1],7,'f',1);
  ui->Ch18Time->setText(tmp);
  if(OO[1]) ui->Ch18Time->setEnabled(true); else ui->Ch18Time->setEnabled(false);
// B ch 3
  tmp=QString("%1 Gs").arg(BVal[2],7,'f',1);
  ui->Ch19Time->setText(tmp);
  if(OO[2]) ui->Ch19Time->setEnabled(true); else ui->Ch19Time->setEnabled(false);
// B ch 4
  tmp=QString("%1 Gs").arg(BVal[3],7,'f',1);
  ui->Ch20Time->setText(tmp);
  if(OO[3]) ui->Ch20Time->setEnabled(true); else ui->Ch20Time->setEnabled(false);
// B ch 5
  tmp=QString("%1 Gs").arg(BVal[4],7,'f',1);
  ui->Ch21Time->setText(tmp);
  if(OO[4]) ui->Ch21Time->setEnabled(true); else ui->Ch21Time->setEnabled(false);
// B ch 6
  tmp=QString("%1 Gs").arg(BVal[5],7,'f',1);
  ui->Ch22Time->setText(tmp);
  if(OO[5]) ui->Ch22Time->setEnabled(true); else ui->Ch22Time->setEnabled(false);

}
