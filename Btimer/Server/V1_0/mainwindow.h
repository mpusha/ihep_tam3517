#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "3rdparty/qextserialport/src/qextserialport.h"
#include "3rdparty/qextserialport/src/qextserialenumerator.h"
#include "gpio.h"
#include "calculator.h"

extern "C" int configureFPGA(char * fName);


#define INTERVAL_SCAN 1000
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel *stsLabel,*stsExtLabel;
    bool checkValidData(void);
    QTimer *timer,*timerScanCycle;
    QString getReplay(QString cmd);
    QString parseStrAnswer(QString str);
    QString execCmdStr(QString cmd);
    bool testAliveDevice(void);
    bool writeAllData(void);
    int convertParamToDigital(void);
    int convertDigitalToParam(void);
    QString intToString(int val);
    void writeData(void);
    void readData(void);
    bool eventFilter(QObject *obj, QEvent *ev);
public slots:
  void onOpenCloseButtonClicked(bool);
  void onDataAvailable(void);
  void onTimeout(void);
  void onReadStatusInCycle(void);
private slots:





  void on_execBtn_clicked();

  void on_cmdExec_returnPressed();



  void on_btnInit_clicked();
  void on_btnClear_clicked();
  void on_btnRstSts_clicked();
  void on_btnReadSts_clicked();
  void on_status2Val_textChanged(void);
  void on_openDevBtn_toggled(bool checked);
  void on_btnWriteCycle_clicked();


private:
    Ui::MainWindow *ui;
    QextSerialPort *port;
    QString processRecStr,origRecStr;
    QString iniFileName;
    bool recData,timeoutRec;
    bool errHSYNC,writeCycleReqest;
    int S2;
    double B1C,B2C,B1COld,B2COld;
    TCalculator editWidget;

};

#endif // MAINWINDOW_H
