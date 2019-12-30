#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myscrsaver.h"

#define MESS_OK             0



namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QString iniFileName;

  void showParamOnScr(void);
  myScrSaver *scrSvr;
public slots:
  void setBTimerParameters(void);
private:
  Ui::MainWindow *ui;
  double BVal[6],B1,B2,BMid,IMid,BTop,ITop;
  int OO[12],ctrl,status,HWStatus;

  int timeOutScrSvr,scanParTime;
};

#endif // MAINWINDOW_H
