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

	perror("init started");
	initRealTime();
	initGPIO();
	initUART();
	initTimer();
	perror("init complete");
	sendByte(RESET_BYTE);

	while ( 1 ) {
		mNextFrameTime = *(mTimer + kTimerCLO) + 40000;
		gpioBit ( kGPIODirectionPort, kFromPi );
		sendFrame();
		// wait for sending to complete
		while ( !( *(mAux + kAuxMiniUartLineStat) & kUART1XmitIdle ) );
		// wait for time to  send next frame
		while ( *(mTimer + kTimerCLO) < mNextFrameTime );
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
	int screenWidth = 128;
	int screenHeight = 8;
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


