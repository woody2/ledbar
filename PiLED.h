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

enum eBufferDirection {
	kBufferWriting        = 0,
	kBufferReading        = 1
};

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
*  Structs									 *
\********************************************/


struct sPixel blankPixel;
struct sPixel redPixel;
struct sPixel greenPixel;
struct sPixel bluePixel;


/********************************************\
*  Forward Declarations                      *
\********************************************/

void processState ();
void changeState ();
void printState ();
void triggerScope ();
void gpioBit ( char bit, char val );
void sendByte ( unsigned char byte );
//volatile unsigned long *mapMem ( unsigned long armAddr, unsigned int memSize );

void sendPixel ( struct sPixel *pixel );
void sendFrame ();

void initScreen();
void initImageElements();

/********************************************\
*  Global Variables                          *
\********************************************/

volatile unsigned long *mSetAddr;
volatile unsigned long *mClrAddr;
enum eStates mState;

struct sPixel *mScreenMatrix;
short *mPixelMap;
int screenWidth;
int screenHeight;
unsigned long mNextFrameTime;
int mTestCounter;

struct sPixel *mMergePixel;
struct image_element *imgA;
struct image_element *imgB;

short opacity;
int frameCount;


#endif
