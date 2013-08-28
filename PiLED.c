/**********************************************************************\
*                                                                      *
* PiLED - GoGo Gaybar!										           *
*                                                                      *
\**********************************************************************/

#include "PiLED.h"
#include "pixelmapping.h"
#include "image_element.h"


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
	blankPixel.red = 0;
	blankPixel.green = 0;
	blankPixel.blue = 0;
	redPixel = greenPixel = bluePixel = blankPixel;
	redPixel.red = 255;
	greenPixel.green = 255;
	bluePixel.blue = 255;
	
	enum eStates mState = kStateInvalid;
	screenWidth = 128;
	screenHeight = 8;
	opacity = 100;
}

void processState() {
	switch ( mState ) {

	case kStateUninitialised:
		initRealTime();
		initGPIO();
		initUART();
		initTimer();
		perror("globals");
		initGlobals();
		perror("imgs");
                initImageElements();
		perror("screen");
		initScreen();
		perror("afterscreen");
		sendByte(1);
		frameCount = 0;
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
		frameCount++;
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
		//perror("lol");
		imgA->update(imgA, frameCount);
		//perror("lol2");
		imgB->update(imgB, frameCount);
		//perror("lol3");
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
	if(pixel->red == 1)
		pixel->red = 2;
	sendByte(pixel->red);
	if(pixel->green == 1)
		pixel->green = 2;
	sendByte(pixel->green);
	if(pixel->blue == 1)
		pixel->blue = 2;
	sendByte(pixel->blue);
}

struct sPixel *mergePixel(struct sPixel p1, struct sPixel p2, short opacity)
{
	mMergePixel->red = (p1.red * opacity / 100) + (p2.red * (100-opacity) / 100);
	mMergePixel->green = (p1.green * opacity / 100) + (p2.green * (100-opacity) / 100);
	mMergePixel->blue = (p1.blue * opacity / 100) + (p2.blue * (100-opacity) / 100);
	return mMergePixel;
}

void sendFrame ()
{
	//perror("sendFrame");
	int n;
	for (n = 0; n < screenWidth*screenHeight; n++)
	{
		//sendPixel(mergePixel(redPixel, bluePixel, opacity));
		sendPixel(mergePixel(imgA->PixelMatrix[mPixelMap[n]], 
			imgB->PixelMatrix[mPixelMap[n]],
			opacity));
	}
	//Send Reset byte:
	sendByte(1);
}

void initScreen()
{	
	mMergePixel = (struct sPixel*)malloc(sizeof(struct sPixel));
	imgA->init(imgA);
	imgB->init(imgB);
	mPixelMap = GayBarMap;
}

void initImageElements()
{
	imgA = (struct image_element*)malloc(sizeof(struct 
image_element));


	imgB = (struct image_element*)malloc(sizeof(struct image_element));
	imgB->colors = (struct sPixel*)malloc(sizeof(struct sPixel));
	imgB->colors[0] = greenPixel;
	imgB->init = &init_test;
	imgB->update = &update_test;
	//perror("lol");
}
