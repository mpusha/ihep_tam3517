#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <sys/mman.h>

#include "gpio.h"

int lcdOnOff(char);

int lcdOnOff(char OnOff)
{
  int mfd;
  unsigned char *padconf=NULL,*pGpIo2=NULL;
  unsigned long regTmp;

  mfd=open("/dev/mem",O_RDWR|MS_SYNC);
  if (mfd<0) {
    perror("mem open");
    return(LOADFPGA_ERR_MEMMAP);
  }

  padconf=mmap(NULL,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,mfd,0x48002000);
  if (padconf==MAP_FAILED) {
    perror("mmap padconf");
    return(LOADFPGA_ERR_MEMMAP);
  }
  pGpIo2=mmap(NULL,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,mfd,0x49050000);
  if (pGpIo2==MAP_FAILED) {
    perror("mmap gpio4");
    return(LOADFPGA_ERR_MEMMAP);
  }
  close(mfd);

  regTmp=((READREG32(padconf+0xb0))&0x0000ffff)|0x00040000; // set mode 4 on gpio 53 - out;
  WRITEREG32(padconf+0xb0,regTmp); // set mode 4 on gpio 53 - out;

  GPIO_SETDIRECTION(pGpIo2,(LED_LCD)); // on output
  
  if(OnOff==0) GPIO_CLEARBIT32(pGpIo2,LED_LCD);
  else
    GPIO_SETBIT32(pGpIo2,LED_LCD);
}
