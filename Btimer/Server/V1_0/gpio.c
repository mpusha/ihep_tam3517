#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <sys/mman.h>

#include "gpio.h"
int configureFPGA(char *);

int configureFPGA(char * fName)
{
  int mfd,rb,i;
  unsigned char *padconf=NULL,*pGpIo4=NULL,*pGpIo6=NULL;
  unsigned long regTmp;
  unsigned long j,DataLoad;
  unsigned char dataForLoad[512];
  
 // printf("%s\n", *((unsigned char *) &x) == 0 ? "big-endian"\ : "little-endian");

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
  pGpIo4=mmap(NULL,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,mfd,0x49054000);
  if (pGpIo4==MAP_FAILED) {
    perror("mmap gpio4");
    return(LOADFPGA_ERR_MEMMAP);
  }
  pGpIo6=mmap(NULL,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,mfd,0x49058000);
  if (pGpIo6==MAP_FAILED) {
    perror("mmap gpio4");
    return(LOADFPGA_ERR_MEMMAP);
  }
  close(mfd);
  
  WRITEREG32(padconf+0x13C,0x0004011c); // set mode 4 on gpio 116 - inp with PU; 117 - out
  WRITEREG32(padconf+0x140,0x011c0004); // set mode 4 on gpio 118 - out; 119 inp with PU
  regTmp=((READREG32(padconf+0x1D0))&0xffff0000)|0x00000004;
  WRITEREG32(padconf+0x1D0,regTmp); // set mode 4 on gpio 175 - out 

  GPIO_SETDIRECTION(pGpIo4,(n1_DCLK|n1_DATA0)); // on output
  GPIO_SETDIRECTION(pGpIo6,n1_NCONFIG); // on output
  
  mfd=open(fName,O_RDONLY);
  if (mfd<0) {
    perror("Timer FPGA file open. File not found btimer.rbf");
    syslog(LOG_ERR,"Timer FPGA file open error. File not found %s",fName);
    return(LOADFPGA_ERR_FNAME);
  }

// 1  - VCC
// 2  - DCLK      117 HOST
// 4  - Data0     118 HOST
// 6  - nSTATUS   119 FPGA
// 8  - CONF_DONE 116 FPGA
// 10 - nCONFIG   175 HOST
// 17,19 - gnd
//---      --------------------------------------- nCONFIG (host) 175
//  I      I
//  I------I
//-------      ---------------------------------- nSTATUS FPGA 119
//      I      I
//      I------I
//-------                                     --- CONF_DONE (FPGA) 116
//      I                                    I
//      I---------------------~~~~-----------I
//
//             ----  ----  ----  ----  ----  ----  ---- DCLK (host) 117
//             I  I  I  I  I  I  I  I  I  I  I  I  I
//-------------I  I--I  I--I  I--I  I--I  I--I  I--I
//            ---------------------------------------
// ----------                                        Data0 LSB is first (host) 118
//            ---------------------------------------clock data on rise DCLK
 // while(1){
 // GPIO_SETBIT32(pGpIo4,n1_DATA0); //DCLK On
 // GPIO_CLEARBIT32(pGpIo4,n1_DATA0); //DCLK Off
 // }
// form nCONFIG pulse
  GPIO_CLEARBIT32(pGpIo4,n1_DCLK); //CLK Off
  //one bit set/clear ~0,375 us for tam3517
  for(i=0;i<11;i++) GPIO_SETBIT32(pGpIo6,n1_NCONFIG);  // nConfig On and wait ~4us 
  for(i=0;i<160;i++) GPIO_CLEARBIT32(pGpIo6,n1_NCONFIG); // nConfig Off and wait ~60us
  for(i=0;i<107;i++) GPIO_SETBIT32(pGpIo6,n1_NCONFIG);   // nConfig On and wait ~40us
  while((GPIO_READDATA32(pGpIo4)&n1_NSTATUS)==0); // wait nSTATUS going to 1
//  for(i=((hw_option>>3)&1)*SIZE_FPGA;i<SIZE_FPGA+((hw_option>>3)&1)*SIZE_FPGA;i++)
  while((rb=read(mfd,dataForLoad,512))>0){
    for(i=0;i<rb;i++) {
      DataLoad=dataForLoad[i];
      for(j=0;j<8;j++) {
        if(DataLoad&0x01)
          GPIO_SETBIT32(pGpIo4,n1_DATA0); // data0 On
        else
          GPIO_CLEARBIT32(pGpIo4,n1_DATA0); // data0 Off
        DataLoad>>=1;
        GPIO_SETBIT32(pGpIo4,n1_DCLK); //DCLK On
        GPIO_CLEARBIT32(pGpIo4,n1_DCLK); //DCLK Off
      }
    }
  }
  i=0;
  close(mfd);
  while(((GPIO_READDATA32(pGpIo4)&n1_CONF_DONE)==0)&&(i++<TIMEOUT_ERR))
  //while(((GPIO_READDATA32(pGpIo4)&n1_CONF_DONE)==0))
  {
     GPIO_SETBIT32(pGpIo4,n1_DCLK); //DCLK On
     GPIO_CLEARBIT32(pGpIo4,n1_DCLK); //DCLK Off
  }
  munmap(pGpIo4,0x1000);
  munmap(pGpIo6,0x1000);
  munmap(padconf,0x1000);
  if(i>=TIMEOUT_ERR) return(LOADFPGA_ERR_TIMEOUT);
  return(LOADFPGA_OK);
} 
