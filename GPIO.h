#include "memory.h"
 
#define GPIO_BASE       (BCM2708_BASE + 0x200000)       // 0x7e200000

// GPIO setup macros. 
// Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(mGPIO+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(mGPIO+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(mGPIO+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))


enum eGPIOAltFn {
	kGPIOAlt0     = 0b100,
	kGPIOAlt1     = 0b101,
	kGPIOAlt2     = 0b110,
	kGPIOAlt3     = 0b111,
	kGPIOAlt4     = 0b011,
	kGPIOAlt5     = 0b010
};

enum eGPIOPorts {
	kGPIODirectionPort    = 23,
	kGPIODataInPort       = 24,
	kGPIODataOutPort      = 25
};

enum eGPIOWordOffsets {
	kGPIOSetWordOffset    = 7,
	kGPIOClrWordOffset    = 10
};

enum eDataDirection {
	kToPi = 0,
	kFromPi = 1
};

enum eGPIOPinsUsed {
	kBufferDirectionPin   = 23,
	kScopeTriggerPin      = 18,
	kUART1TxPin           = 14,
	kUART1RxPin           = 15,
	kUART1CTSPin          = 16,
	kUART1RTSPin          = 17
};

volatile unsigned long *mGPIO;

int initGPIO();

void gpioBit ( char bit, char val );

volatile unsigned long *mapMem ( unsigned long armAddr, unsigned int memSize );
