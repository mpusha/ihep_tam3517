#ifndef MYSCRSAVER_H
#define MYSCRSAVER_H

#include <QWSScreenSaver>
extern "C" int lcdOnOff(char);
class myScrSaver : public QWSScreenSaver
{
  //Q_OBJECT
public:
  explicit myScrSaver();
  void restore(void);
  bool save(int level);

};

#endif // MYSCRSAVER_H
