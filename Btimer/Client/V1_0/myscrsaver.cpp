#include <QDebug>
#include "myscrsaver.h"

myScrSaver::myScrSaver()
{
}

void myScrSaver::restore(void)
{
  lcdOnOff(1);
}
bool myScrSaver::save(int level)
{
  lcdOnOff(0);
  return true;
}
