#include "UART.h" 

/**********************************************************************\
* initUART                                                             *
* Map the AUX memory to the process memory, then configure the UART.   *
\**********************************************************************/

int initUART() {
	mAux = mapMem ( AUX_BASE, 0x1000 );
	// Enable only the mini uart
	*(mAux + kAuxEnables) = kAuxEnableMiniUart;
	// Set Speed
	*(mAux + kAuxMiniUartBaudRate) = 30;
	// 8-bit mode - SEE ERRATA!!
	*(mAux + kAuxMiniUartLineCtrl) = kUART18Bit;
	// Enable Transmitter ONLY and use RTS
	*(mAux + kAuxMiniUartExtraCtrl) = 
		kUART1RTSAuto |
		kUART1TxEnable | 
		kUART1RxEnable;
	// Set RTS
	*(mAux + kAuxMiniUartModemCtrl) = kUART1RTS;
}

void sendByte ( unsigned char byte )
{
	// Wait till FIFO has only 3 bytes in it
	while ( ( *(mAux + kAuxMiniUartExtraStat) >> 24 ) > 3 )
		;
	// Write the character
	*(mAux + kAuxMiniUartIO) = byte;
}

void WaitForSendingToComplete(){
	while ( !( *(mAux + kAuxMiniUartLineStat) & kUART1XmitIdle ) );
}
