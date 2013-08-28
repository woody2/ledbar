#include "GPIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

/**********************************************************************\
* initGPIO                                                             *
* May the GPIO memory to the process memory, then Configure the pins   *
* we require and mark the pins for the UART as their ALT config.       *
\**********************************************************************/

int initGPIO() {
	// Map the GPIO and Aux memory
	mGPIO = mapMem ( GPIO_BASE, 0x1000 );

	// UART1 = Mini-UART, UART0 - PL011
	// Set GPIO15 and 16 for UART1 to ALT5 Mini-Uart
	INP_GPIO ( kBufferDirectionPin );
	OUT_GPIO ( kBufferDirectionPin );
	INP_GPIO ( kScopeTriggerPin );
	OUT_GPIO ( kScopeTriggerPin );
	INP_GPIO ( kUART1TxPin );
	SET_GPIO_ALT ( kUART1TxPin, 5 );
	INP_GPIO ( kUART1RxPin );
	SET_GPIO_ALT ( kUART1RxPin, 5 );
	INP_GPIO ( kUART1RTSPin);
	SET_GPIO_ALT ( kUART1RTSPin, 5 );
}

void gpioBit ( char bit, char val )
{
	if ( val )
	{
		*(mGPIO + kGPIOSetWordOffset) = ( 1 << bit );
	}
	else
	{
		*(mGPIO + kGPIOClrWordOffset) = ( 1 << bit );
	}
}


/********************************************\
*  Set up a memory to access IO Peripherals  *
\********************************************/

volatile unsigned long *mapMem ( unsigned long armAddr, unsigned int memSize )
{
	char *mem, *map;
	long pageSize = sysconf ( _SC_PAGE_SIZE );

	/* open /dev/mem */
	int memFD = open("/dev/mem", O_RDWR|O_SYNC);
	if ( memFD < 0 ) {
		perror ( "can't open /dev/mem" );
		exit ( 1 );
	}

	// Allocate MAP block
	int status = posix_memalign ( (void **) &mem, pageSize, memSize );
	if ( status != 0 ) 
	{
		perror ( "Can't get aligned memory" );
		exit ( 2 );
	}

	// Now map it
	map = (unsigned char *) mmap (
		(caddr_t) mem,              /* start */
		memSize,                    /* length */
		PROT_READ|PROT_WRITE,       /* prot */
		MAP_SHARED|MAP_FIXED,       /* flags */
		memFD,                      /* fd */
		armAddr                     /* offset */
		);
	if ( (long) map < 0) 
	{
		perror ( "Can't map memory" );
		exit (1);
	}

	close(memFD);

	// Always use volatile pointer!
	return (volatile unsigned long*) map;
}

