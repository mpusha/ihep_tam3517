#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QSettings>


// export QWS_SIZE=640x515

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stsLabel=new QLabel(this);
    stsExtLabel=new QLabel(this);
    ui->statusBar->addWidget(stsLabel);
    ui->statusBar->addWidget(stsExtLabel);
    ui->baudRateComboBox->addItem("1200",BAUD1200);
    ui->baudRateComboBox->addItem("2400",BAUD2400);
    ui->baudRateComboBox->addItem("4800",BAUD4800);
    ui->baudRateComboBox->addItem("9600",BAUD9600);
    ui->baudRateComboBox->addItem("19200",BAUD19200);
    ui->baudRateComboBox->setCurrentIndex(4);
    port=0; errHSYNC=false;
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timerScanCycle=new QTimer(this);
    connect(timerScanCycle, SIGNAL(timeout()), this, SLOT(onReadStatusInCycle()));
    recData=false; origRecStr=""; processRecStr="";
    ui->portNameComboBox->clear();
    ui->portNameComboBox->addItem("/dev/ttyO1");
    connect(ui->openCloseButton, SIGNAL(toggled(bool)), SLOT(onOpenCloseButtonClicked(bool)));
    iniFileName=QCoreApplication::applicationDirPath ()+"/options.ini";

    QFile iniFile(iniFileName);
    if(!iniFile.open(QIODevice::ReadOnly)) {

    }
    QTextStream in(&iniFile);
    QString szLine;
    do {
        szLine = in.readLine();

        if(szLine.contains("baud",Qt::CaseInsensitive)){
          QString baudName=szLine.section('=',1,1);
          baudName=baudName.simplified();
          ui->baudRateComboBox->setCurrentIndex(ui->baudRateComboBox->findText(baudName));
        }
        if(szLine.contains("usermode",Qt::CaseInsensitive)){
          if(szLine.section('=',1,1).simplified().toInt()==1){
            ui->btnInit->setVisible(false);
            ui->btnRstSts->setVisible(false);
            ui->btnReadSts->setVisible(false);
            ui->execBtn->setVisible(false);
          }

        }
      } while(!szLine.isNull());
      iniFile.close();
      ui->tab_2->setEnabled(false);
      ui->tab_3->setEnabled(false);
      ui->tab_7->setEnabled(false);
      readData();
      convertDigitalToParam();
      B1COld=-1; B2COld=-1;
      ui->lEB1Set->installEventFilter(this);
      ui->lEB2Set->installEventFilter(this);
      setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
  if(port){
    port->close();
    delete port;
    port=0;
  }
  timer->stop();
  timerScanCycle->stop();
  delete ui;
}


// ----------------------------------------------------GUI---------------------------------------------------------------------
// Open/Close port button processing -------------------------------------------------------------------------------------------
void MainWindow::onOpenCloseButtonClicked(bool ch)
{
  if(ch){
    PortSettings settings = {(BaudRateType)ui->baudRateComboBox->itemData(ui->baudRateComboBox->currentIndex()).toInt(), DATA_8, PAR_NONE, STOP_1,FLOW_OFF};
    if(port){
      port->close();
      delete port;
      port=0;
    }
    port = new QextSerialPort(ui->portNameComboBox->currentText(), settings, QextSerialPort::EventDriven);
    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
//    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    errHSYNC=false;
    if(!port->open(QIODevice::ReadWrite)) {delete port; port=0; return;}
    ui->baudRateComboBox->setEnabled(false);
    ui->portNameComboBox->setEnabled(false);
    ui->openCloseButton->setText("Close");
    ui->openCloseLabel->setText(QString("Port %1 is open").arg(ui->portNameComboBox->currentText()));
    ui->tab_2->setEnabled(true);
    ui->tab_3->setEnabled(true);
    ui->tab_7->setEnabled(true);
    ui->textAnswer->clear();
    ui->textHistory->clear();
    ui->btnInit->setEnabled(true);
    ui->openDevBtn->setEnabled(true);
    ui->openDevBtn->setChecked(false);
    ui->btnRstSts->setEnabled(true);
    writeCycleReqest=false;
    stsLabel->setText("Serial port is open  ");
    ui->openDevBtn->setText("Open device");
    ui->btnReadSts->setEnabled(true);
    ui->execBtn->setEnabled(true);
//    writeData();
    convertDigitalToParam();
    //if(configureFPGA((char*)"f13.bin")) qDebug()<<"Config error"; else qDebug()<<"Config OK" ;
    //timer->start(1000);
  }
  else{
    ui->baudRateComboBox->setEnabled(true);
    ui->portNameComboBox->setEnabled(true);
    ui->openCloseButton->setText("Open");
    ui->openCloseLabel->setText("Port is close");
    ui->openDevBtn->setChecked(false);
    ui->openDevBtn->setEnabled(false);
    ui->btnInit->setEnabled(false);
    ui->btnRstSts->setEnabled(false);
    ui->tab_2->setEnabled(false);
    ui->tab_3->setEnabled(false);
    ui->tab_7->setEnabled(false);
    timer->stop();
    timerScanCycle->stop();
    stsLabel->setText("Serial port is close  ");
    if(port){
      port->close();
      delete port;
      port=0;
    }
  }
}
// Open/Close device button pressed ---------------------------------------------------------------------
void MainWindow::on_openDevBtn_toggled(bool checked)
{
    timerScanCycle->stop();
    if(!ui->openDevBtn->isEnabled()) return;
    if(checked) ui->openDevBtn->setText("Close device"); else ui->openDevBtn->setText("Open device");
    ui->btnInit->setEnabled(not checked);
    ui->btnWriteCycle->setEnabled(checked);
    ui->btnRstSts->setEnabled(not checked);
    ui->btnReadSts->setEnabled(not checked);
    ui->execBtn->setEnabled(not checked);
    on_btnInit_clicked();
    if(not errHSYNC){
      if(checked) {
        timerScanCycle->start(INTERVAL_SCAN); stsLabel->setText("HSync Device is open  ");
      }
      else {
        stsLabel->setText("HSync Device is close  ");
        stsExtLabel->setText("");
      }
    }
    else {
      stsLabel->setText("HSync Device don't work!  ");
      stsExtLabel->setText("");
      ui->openDevBtn->setEnabled(false);
      ui->btnWriteCycle->setEnabled(false);
      ui->openDevBtn->setChecked(false);
    }
}
// Button Init pressed --------------------------------------------------------------------------------
void MainWindow::on_btnInit_clicked()
{
    //if(configureFPGA((char*)"f13.bin")) qDebug()<<"Config error"; else qDebug()<<"Config OK" ;
    execCmdStr("01:RF");sleep(1);
    execCmdStr("01:RS");
    ui->labelVersion->setText("S:"+parseStrAnswer(execCmdStr("01:SW")));
    ui->labelVersion->setText(ui->labelVersion->text()+" H:"+parseStrAnswer(execCmdStr("01:FW")));
    testAliveDevice();
    if(not errHSYNC){
       stsLabel->setText("HSync Device is open  ");
    }
    else {
      stsLabel->setText("HSync Device don't work!  ");
    }
}
// Write button clicked ------------------------------------------------------------------------------
void MainWindow::on_btnWriteCycle_clicked()
{
    writeCycleReqest=true;
}
// Status presentation on scr -------------------------------------------------------------------------------
void MainWindow::on_status2Val_textChanged(void)
{
    if(ui->status2Val->text().toInt()&1) ui->cBS1_9->setChecked(1); else ui->cBS1_9->setChecked(0);
    if(ui->status2Val->text().toInt()&2) ui->cBS1_10->setChecked(1); else ui->cBS1_10->setChecked(0);
    if(ui->status2Val->text().toInt()&4) ui->cBS1_11->setChecked(1); else ui->cBS1_11->setChecked(0);
    if(ui->status2Val->text().toInt()&8) ui->cBS1_12->setChecked(1); else ui->cBS1_12->setChecked(0);
    if(ui->status2Val->text().toInt()&0x10) ui->cBS1_13->setChecked(1); else ui->cBS1_13->setChecked(0);
    if(ui->status2Val->text().toInt()&0x20) ui->cBS1_14->setChecked(1); else ui->cBS1_14->setChecked(0);
    if(ui->status2Val->text().toInt()&0x40) ui->cBS1_15->setChecked(1); else ui->cBS1_15->setChecked(0);
    if(ui->status2Val->text().toInt()&0x80) ui->cBS1_8->setChecked(1); else ui->cBS1_8->setChecked(0);
}
// read status button clicked
void MainWindow::on_btnReadSts_clicked()
{

    ui->status2Val->setText(parseStrAnswer(execCmdStr("01:ST")));
     ui->lEAdcA->setText(parseStrAnswer(execCmdStr("01:AR 0")));
     ui->lEAdcB->setText(parseStrAnswer(execCmdStr("01:AR 1")));
     ui->lEAdcAM->setText(parseStrAnswer(execCmdStr("01:AR 2")));
     ui->lEAdcBM->setText(parseStrAnswer(execCmdStr("01:AR 3")));
     ui->lEInt->setText(parseStrAnswer(execCmdStr("01:IR 0")));

}
//reset status Button clicked --------------------------------------------------------------------------
void MainWindow::on_btnRstSts_clicked()
{
  execCmdStr("01:RS");
}
// Execute command -----------------------------------------------------------------------------------
void MainWindow::on_execBtn_clicked()
{
  ui->led->setChecked(true);
  QString tmp=getReplay(ui->cmdExec->text());
  if(ui->logCB->isChecked()) ui->textAnswer->append(tmp);
  if(ui->logCB->isChecked()) ui->textHistory->append(ui->cmdExec->text());
  ui->led->setChecked(false);
}
// Key return press in execute mode -------------------------------------------------------------------
void MainWindow::on_cmdExec_returnPressed()
{
  on_execBtn_clicked();
}
// Clear answer and history blocks ---------------------------------------------------------------------
void MainWindow::on_btnClear_clicked()
{
    ui->textAnswer->clear();
    ui->textHistory->clear();
}
//
// Programm logical function ------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------
// Serial interface processing
bool MainWindow::checkValidData(void)
{
 bool tmp=recData;
 recData=false;
 //if(tmp) ui->led->setChecked(true); else ui->led->setChecked(false);
 return tmp;
}

void MainWindow::onTimeout(void)
{ qDebug()<<"timeout";
  timeoutRec=false;
  return;
  if(checkValidData()){
    //ui->original->setText(origRecStr);
    int intOrig;
    intOrig=processRecStr.toInt();
    //intTransf=ui->transform->text().toDouble();
    //ui->result->setText(QString("New=%1").arg(intOrig*intTransf));
    //ui->timeEdit->setText(QString("%1 ms and %2 us").arg(intOrig/20000).arg((double)(intOrig%20000)/20.0));
    processRecStr=""; origRecStr="";
  }
}
void MainWindow::onDataAvailable(void)
{
  recData=false;
//  qDebug()<<"getdata";
  QByteArray data = port->readAll();
  for(int i = 0; i < data.size(); ++i) {
      if(((data.at(i)>=' ') && (data.at(i)<='z')))
      processRecStr+=data.at(i);
      origRecStr+=data.at(i);
    if(data.at(i)==0x0){
      recData=true;
    }
  }
}

QString MainWindow::getReplay(QString cmd)
{
  QByteArray data;
  data=cmd.toAscii();
  data[data.length()]=0;
  processRecStr=""; origRecStr="";
  if(!port) return("");
  port->write(data);
  //QTimer::singleShot(1000, this, SLOT(onTimeout()));
  timer->start(1000);
  timeoutRec=true;
  while(timeoutRec){
    if(checkValidData()) {
      timer->stop();
      return processRecStr;
    }
    QApplication::processEvents();
  }
  timer->stop();
  return "timeout";
}
QString MainWindow::execCmdStr(QString cmd)
{
  //QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->cmdExec->setText(cmd);
  on_execBtn_clicked();
  //QApplication::restoreOverrideCursor();
  return processRecStr;
}
QString MainWindow::parseStrAnswer(QString str)
{
  if(str=="timeout") return("unknown");
  return str.mid(3);
}
bool MainWindow::testAliveDevice(void)
{
  if(parseStrAnswer(execCmdStr("01:TE"))!="0") errHSYNC=true;
  else
    errHSYNC=false;
  return errHSYNC;
}
//-------------------------------------------Timer event
void MainWindow::onReadStatusInCycle(void)
{
//  qDebug()<<"scan";
  timerScanCycle->stop();
  int s2;
  bool ok;
  QApplication::processEvents();
  s2=execCmdStr("01:ST").mid(3).toInt(&ok);
  if(!ok) {timerScanCycle->start(INTERVAL_SCAN); return;}
  if((s2&0x40)==0) {
      timerScanCycle->start(INTERVAL_SCAN);
      if(not writeCycleReqest) stsExtLabel->setText("Wait cycle...  ");
      else
        stsExtLabel->setText("Wait write data  ");
      return;
  }
  stsExtLabel->setText("Impulse Reset detected  ");
  on_btnReadSts_clicked();
  if(writeCycleReqest){
    writeCycleReqest=false;
    writeAllData();
    stsExtLabel->setText("Write data complete  ");
  }
  execCmdStr("01:RS");
  timerScanCycle->start(INTERVAL_SCAN);
}
bool MainWindow::writeAllData(void)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
//  qDebug()<<"Write data";
  if(testAliveDevice()) {
    stsLabel->setText("HSync Device don't work!  ");
    ui->openDevBtn->setEnabled(false);
    ui->btnWriteCycle->setEnabled(false);
    ui->openDevBtn->setChecked(false);
    QApplication::restoreOverrideCursor();
    return false;
  }
  int codeErr;
  if((codeErr=convertParamToDigital())){
     stsLabel->setText("Error in convertion from str to digital. Error code="+intToString(codeErr));
     QApplication::restoreOverrideCursor();
     return false;
  }
  if(B1C!=B1COld) {
    execCmdStr("01:WW 0 "+QString("%1").arg((int)((B1C-355.7)*95.966+7698)));
    B1COld=B1C;
  }
  if(B2C!=B2COld) {
    execCmdStr("01:WW 1 "+QString("%1").arg((int)((B2C-355.7)*95.966+7698)));
    B2COld=B2C;
  }
//  if(testCycle!=testCycleOld){
//    execCmdStr("05:MW 0 "+QString("%1").arg(testCycle));
//    testCycleOld=testCycle;
//  }


  writeData();
  QApplication::restoreOverrideCursor();
  return true;
}

QString MainWindow::intToString(int val)
{
  return QString("%1").arg(val);
}

void MainWindow::readData(void)
{

  QSettings *Ini ;
  Ini = new QSettings(QCoreApplication::applicationDirPath ()+"/data.ini",QSettings::IniFormat);

  Ini->beginGroup("Data");
//  Ini->beginReadArray("T0");
//    for(int i=0;i<29;i++){
//      Ini->setArrayIndex(i);
//      sepData[i]=Ini->value("Sep",i).toInt();
//    }
//    Ini->endArray();

    B1C=Ini->value("B1", 408.5).toDouble();
    B2C=Ini->value("B2", 355.7).toDouble();
  /*  delTvW=Ini->value("delayTvW",0).toInt();
    fDDS1=Ini->value("DDS1",5515700).toInt();
    fDDS2=Ini->value("DDS2",2742000).toInt();
    ctrlReg=Ini->value("Ctrl_register", 0).toInt();
    testCycle=Ini->value("test_cycle", 1).toInt();

    delay1St1=Ini->value("delay1St1", 0).toInt(); delay1PPM1=Ini->value("delay1PPM1", 0).toInt();
    delay1St2=Ini->value("delay1St2", 0).toInt(); delay1PPM2=Ini->value("delay1PPM2", 0).toInt();

    delay2St1=Ini->value("delay2St1", 0).toInt(); delay2PPM1=Ini->value("delay2PPM1", 0).toInt();
    delay2St2=Ini->value("delay2St2", 0).toInt(); delay2PPM2=Ini->value("delay2PPM2", 0).toInt();

    delay3St1=Ini->value("delay3St1", 0).toInt(); delay3PPM1=Ini->value("delay3PPM1", 0).toInt();
    delay3St2=Ini->value("delay3St2", 0).toInt(); delay3PPM2=Ini->value("delay3PPM2", 0).toInt();
*/
    Ini->endGroup();
    delete Ini;

}
void MainWindow::writeData(void)
{

  QSettings *Ini ;
  Ini = new QSettings(QCoreApplication::applicationDirPath ()+"/data.ini",QSettings::IniFormat);

  Ini->beginGroup("Data");
//  Ini->beginWriteArray("T0");
//  for(int i=0;i<29;i++){
//    Ini->setArrayIndex(i);
//    Ini->setValue("Sep",sepData[i]);
//  }
//  Ini->endArray();

  Ini->setValue("B1",B1C);
  Ini->setValue("B2",B2C);
  /*Ini->setValue("delayTvW",delTvW);
  Ini->setValue("DDS1",fDDS1);
  Ini->setValue("DDS2",fDDS2);

  Ini->setValue("Ctrl_register",ctrlReg);
  Ini->setValue("test_cycle",testCycle);

  Ini->setValue("delay1St1",delay1St1); Ini->setValue("delay1PPM1",delay1PPM1);
  Ini->setValue("delay1St2",delay1St2); Ini->setValue("delay1PPM2",delay1PPM2);

  Ini->setValue("delay2St1",delay2St1); Ini->setValue("delay2PPM1",delay2PPM1);
  Ini->setValue("delay2St2",delay2St2); Ini->setValue("delay2PPM2",delay2PPM2);

  Ini->setValue("delay3St1",delay3St1); Ini->setValue("delay3PPM1",delay3PPM1);
  Ini->setValue("delay3St2",delay3St2); Ini->setValue("delay3PPM2",delay3PPM2);
*/
  Ini->endGroup();
  delete Ini;

}
int MainWindow::convertDigitalToParam(void)
{
  ui->lEB1Set->setText(QString("%1").arg(B1C,0,'f',1));
  ui->lEB2Set->setText(QString("%1").arg(B2C,0,'f',1));
  //ui->sepN1->setValue(sepData[0]&0x1f); ui->cbUm1_1->setChecked((sepData[0]>>5)&1); ui->cbUm2_1->setChecked((sepData[0]>>6)&1);
  //ui->delay3St1->setValue(delay3St1); ui->delay3PPM1->setValue(delay3PPM1); ui->delay3St2->setValue(delay3St2); ui->delay3PPM2->setValue(delay3PPM2);

  return(0);
}
int MainWindow::convertParamToDigital(void)
{
  bool ok;
  int tmp;

  B1C=ui->lEB1Set->text().toDouble(&ok); if(!ok) QMessageBox::information(0, "Information", "Ошибка данных");
  B2C=ui->lEB2Set->text().toDouble(&ok); if(!ok) QMessageBox::information(0, "Information", "Ошибка данных");
//  testCycle=ui->measCycle->value();

//  tmp=ui->sepN1->value(); tmp|=(ui->cbUm1_1->isChecked()<<5)|(ui->cbUm2_1->isChecked()<<6);
//  sepData[0]=tmp&255;

//delTvW=ui->delTvW->value();
//  delay3St1=ui->delay3St1->value();delay3PPM1=ui->delay3PPM1->value();delay3St2=ui->delay3St2->value();delay3PPM2=ui->delay3PPM2->value();


/*  QList<QWidget *> widgets = findChildren<QWidget *>();
  for(int i=0;i<30;i++){
  for(int j=0,count=widgets.count();j<count;j++) {
    QWidget * w = widgets[j];
    QDoubleSpinBox * sb = qobject_cast<QDoubleSpinBox *>(w);
    if(sb){
      if((sb->objectName()==QString("sepN%1").arg(i+1))){
        sb->setValue(i+1);
      }
    }
  }
  }
*/
  return 0;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
  if(obj==ui->lEB1Set){
    if(ev->type()==QEvent::MouseButtonPress) {
       //QTimer::singleShot(0, ui->frame, SLOT(setFocus()));
       ui->lEB1Set->setText(editWidget.getValue("SDS",ui->lEB1Set->text()));
       return true;
    }
    else
      return false;
  }
  else if(obj==ui->lEB2Set){
    if(ev->type()==QEvent::MouseButtonPress) {
       //QTimer::singleShot(0, ui->frame, SLOT(setFocus()));
       ui->lEB2Set->setText(editWidget.getValue("SDS",ui->lEB2Set->text()));
       return true;
    }
    else
      return false;
  }
  else
    return MainWindow::eventFilter(obj, ev);
}

