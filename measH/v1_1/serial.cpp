
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>                                                    
#include <sys/stat.h>                                                    
#include <fcntl.h>                                                        
#include <string.h>
#include <termios.h>                                                      
#include <unistd.h>
#include "serial.h"                                                                                                                                   

#define MODEMDEVICE "/dev/ttyO1"                                          
#define _POSIX_SOURCE 1 /* POSIX compliant source */                      

                                                                                
bool SerialGate::Open(QString port, int baud)
{
  struct termios newtio;                                           
  unsigned int BAUDRATE;
  switch(baud){
  case 2400:
    BAUDRATE=B2400;
  break;
  case 4800:
    BAUDRATE=B4800;
  break;
  case 9600:
    BAUDRATE=B9600;
  break;
  case 19200:
    BAUDRATE=B19200;
  break;
  default:
    BAUDRATE=B9600;
  break; 
  }
  fd = open(port.toAscii().data(), O_RDWR | O_NOCTTY );
  if (fd <0) {perror(port.toAscii().data()); return(false); }
  memset(&newtio, 0,sizeof(newtio));                                         
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;             
  newtio.c_iflag = IGNPAR;                                                
  newtio.c_oflag = 0;                                                     
                                                                                
/* set input mode (non-canonical, no echo,...) */                       
  newtio.c_lflag = 0;                                                     
                                                                    
  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */         
  newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */ 
                                                                    
  tcflush(fd, TCIFLUSH);                                                  
  tcsetattr(fd,TCSANOW,&newtio);                                          

  this->state = true;
  return true;	
}

SerialGate::SerialGate()
{
  this->state = false;
}

SerialGate::~SerialGate()
{
  this->Close();
}

void SerialGate::Close()
{
  this->state = false;
  this->Clean();
  close(fd);
}

void SerialGate::Clean()
{
  if(!state)
    return;
  tcflush(fd, TCIOFLUSH);
}

int SerialGate::Send(char* buff, int szBuff)
{
  if(!state)
    return 0;
  if(buff == NULL || szBuff <= 0)
  {
    return 0;
  }
//  printf("send\n");
  int lpdwBytesWrittens = 0;
  lpdwBytesWrittens=write(fd,buff,(size_t)szBuff);
  tcdrain(fd);
  return lpdwBytesWrittens;
}

int SerialGate::Recv(char* buff, int szBuff)
{
  if(!state)
    return 0;
  if(buff == NULL || szBuff <= 0)
  {
    return 0;
  }
  int dwBytesRead = 0;	
  dwBytesRead=read(fd,buff,(size_t) szBuff);	
  return dwBytesRead;
}
