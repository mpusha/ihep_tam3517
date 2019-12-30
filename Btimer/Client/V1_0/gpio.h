#define TIMEOUT_ERR 100000
#define LOADFPGA_ERR_MEMMAP 1
#define LOADFPGA_ERR_FNAME 2
#define LOADFPGA_ERR_TIMEOUT 3
#define LOADFPGA_OK  0

#define GPIO_OE 0x34
#define GPIO_DATAIN 0x38
#define GPIO_DATAOUT 0x3c
#define GPIO_SETDATAOUT 0x94
#define GPIO_CLEARDATAOUT 0x90


#define GPIO_SETDIRECTION(gpio,value) *(unsigned long *)(gpio+GPIO_OE) &=~((unsigned long)value) //1 - out
#define GPIO_SETBIT32(gpio,value) *(unsigned long *)(gpio+GPIO_SETDATAOUT)=(unsigned long)value
#define GPIO_CLEARBIT32(gpio,value) *(unsigned long *)(gpio+GPIO_CLEARDATAOUT)=(unsigned long)value
#define GPIO_WRITEDATA32(gpio,value) *(unsigned long *)(gpio+GPIO_DATAOUT)=(unsigned long)value
#define GPIO_READDATA32(gpio) *(unsigned long*)(gpio+GPIO_DATAIN)
#define WRITEREG32(reg,value) *(unsigned long *)(reg)=(unsigned long)value
#define READREG32(reg) *(unsigned long*)(reg)

#define PIN53  1 << (53%32)


#define LED_LCD    PIN53


