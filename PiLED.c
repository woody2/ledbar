/**********************************************************************\
*                                                                      *
* PiLED - GoGo Gaybar!										           *
*                                                                      *
\**********************************************************************/

#include "PiLED.h"
#include "pixelmapping.h"
#include "image_element.h"

#define RESET_BYTE 1


/********************************************\
*  MAIN                                      *
\********************************************/

int main(int argc, char **argv) {
	mState = kStateUninitialised;
	while ( mState != kStateExit ) {
		//perror("main");
		processState();
		changeState();
	}
}

/********************************************\
* processState                               *
* Just do the state, don't try and change    *
* the state.                                 *
\********************************************/

void initGlobals()
{	
	enum eStates mState = kStateInvalid;
	screenWidth = 128;
	screenHeight = 8;
}

void processState() {
	switch ( mState ) {

	case kStateUninitialised:
		perror("init started");
		initRealTime();
		initGPIO();
		initUART();
		initTimer();
		initGlobals();
		perror("init complete");
		sendByte(1);
		mNextFrameTime = *(mTimer + kTimerCLO) + 40000;
		perror("EndOfInit");
		break;

	case kStateSendFrame:
		gpioBit ( kGPIODirectionPort, kFromPi );
		sendFrame();
		//sendByte(0);
		break;

	case kStateSentFrame:
		//gpioBit ( kGPIODirectionPort, kToPi );
		//perror("sent");
		mNextFrameTime = *(mTimer + kTimerCLO) + 40000;
		break;
	}
}

/********************************************\
* changeState                                *
* Detect if the state needs to change, or    *
* simply transition from one state to        *
* another.  Don't process the state.         *
\********************************************/

void changeState() {
	switch ( mState ) {

	case kStateUninitialised:
		mState = kStateSendFrame;
		break;


	case kStateSendFrame:
		mState = kStateSendingFrame;
		break;

	case kStateSendingFrame:
		// Finished sending?
		if ( ( *(mAux + kAuxMiniUartLineStat) & kUART1XmitIdle ) ) {
			mState = kStateSentFrame;
		}
		break;

	case kStateSentFrame:
		mState = kStateIdle;
		break;

	case kStateIdle:
		// Time to send next Frame?
		if ( *(mTimer + kTimerCLO) > mNextFrameTime ) {
			mState = kStateSendFrame;
		}
		break;
	}
}

/**********************************************************************\
* initRealTime                                                         *
* Set the process as a real-time process to stop daemons getting the   *
* processor and causing timing problems.  This should lock out system  *
* when running, but it doesn't for me.  TODO Requires investigation.   *
\**********************************************************************/

int initRealTime() {
	struct sched_param param;
	param.sched_priority = 99;
	int status = sched_setscheduler ( 0, SCHED_RR, &param );
	if ( status != 0 ) {
		perror ( "Can't set realtime scheduling policy" );
	}
}

void sendPixel ( struct sPixel *pixel )
{
	if(pixel->red == RESET_BYTE)
		pixel->red = RESET_BYTE + 1;
	sendByte(pixel->red);
	if(pixel->green == RESET_BYTE)
		pixel->green = RESET_BYTE + 1;
	sendByte(pixel->green);
	if(pixel->blue == RESET_BYTE)
		pixel->blue = RESET_BYTE + 1;
	sendByte(pixel->blue);
}

void sendFrame ()
{
	//perror("sendFrame");
	int n;
	for (n = 0; n < screenWidth*screenHeight; n++)
	{
		struct sPixel pixel;
		pixel.red   =0;
		pixel.green =255;
		pixel.blue  =0;

		sendPixel(&pixel);
	}

	sendByte(RESET_BYTE);
}


