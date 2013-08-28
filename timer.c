#include "timer.h"

/**********************************************************************\
* initTimer                                                             *
* Just map the Timer memory to the process memory.                     *
\**********************************************************************/

void initTimer() {
	mTimer = mapMem ( TIMER_BASE, 0x1000 );
}
