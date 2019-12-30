#ifndef DBPROC_H
#define DBPROC_H
#include <QtCore/QCoreApplication>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDebug>
#include <QWaitCondition>

#include "meBehav.h"

extern "C" int db_init(void);
extern "C" int db_process(unsigned int *update);
extern "C" short sh_put_meas_er(short er_code);
extern "C" short sh_put_data_er(short er_code);
extern "C" void sh_put_mess(char *mes);
extern "C" void set_state(unsigned int num, double *field, char *state);

class TDbProc : public QThread
{
  Q_OBJECT

  public:
  TDbProc(void) ; // constructor
  ~TDbProc();
  unsigned int dbInit(void);

protected:
    void run();

private:
  QCoreApplication *coreApp;
  bool abort;

  QMutex mutex;
  QWaitCondition condition;


public slots:


} ;

#endif // DBPROC_H
