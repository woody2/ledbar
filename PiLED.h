#ifndef PILED_H
#define PILED_H

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>


#include "image.h"
#include "memory.h"
#include "server.h"
#include "UART.h"
#include "GPIO.h"
#include "ledbar.h"
#include "timer.h"


/********************************************\
*  Constants                                 *
\********************************************/

#define PAGE_SIZE       0x1000  // 4K
#define BLOCK_SIZE      0x1000  // 4K

enum eStates {
	kStateInvalid,
	kStateUninitialised,
	kStateIdle,
	kStateSendFrame,
	kStateSendingFrame,
	kStateSentFrame,
	kStateExit
};


/********************************************\
*  Forward Declarations                      *
\********************************************/

void processState ();
void changeState ();
void printState ();

void sendPixel ( struct sPixel *pixel );
void sendFrame ();

/********************************************\
*  Global Variables                          *
\********************************************/

enum eStates mState;

unsigned long mNextFrameTime;
int screenWidth;
int screenHeight;


#endif
