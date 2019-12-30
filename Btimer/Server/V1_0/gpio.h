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
#define PIN116 1 << (116%32)
#define PIN117 1 << (117%32)
#define PIN118 1 << (118%32)
#define PIN119 1 << (119%32)
#define PIN175 1 << (175%32)

#define PIN178 1 << (178%32)
#define PIN179 1 << (179%32)
#define PIN180 1 << (180%32)
#define PIN181 1 << (181%32)
#define PIN182 1 << (182%32)

#define n1_NCONFIG    PIN175 
#define n1_NSTATUS    PIN119
#define n1_CONF_DONE  PIN116
#define n1_DCLK       PIN117
#define n1_DATA0      PIN118

#define n2_NCONFIG    PIN182
#define n2_NSTATUS    PIN180
#define n2_CONF_DONE  PIN181
#define n2_DCLK       PIN178
#define n2_DATA0      PIN179

