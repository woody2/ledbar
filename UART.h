#include "memory.h"

#define AUX_BASE        (BCM2708_BASE + 0x215000)       // 0x7e215000

enum eAuxWordOffsets {
	kAuxIRQ               = 0x00, // AUX_IRQ
	kAuxEnables           = 0x01, // AUX_ENABLES
	kAuxMiniUartIO        = 0x10, // AUX_MU_IO_REG
	kAuxMiniUartIntEnable = 0x11, // AUX_MU_IER_REG
	kAuxMiniUartIntId     = 0x12, // AUX_MU_IIR_REG
	kAuxMiniUartLineCtrl  = 0x13, // AUX_MU_LCR_REG
	kAuxMiniUartModemCtrl = 0x14, // AUX_MU_MCR_REG
	kAuxMiniUartLineStat  = 0x15, // AUX_MU_LSR_REG
	kAuxMiniUartModemStat = 0x16,
	kAuxMiniUartScratch   = 0x17,
	kAuxMiniUartExtraCtrl = 0x18, // AUX_MU_CNTL_REG
	kAuxMiniUartExtraStat = 0x19, // AUX_MU_STAT_REG
	kAuxMiniUartBaudRate  = 0x1a
};

enum eAuxEnableMask {
	kAuxEnableMiniUart    = 0x01,
	kAuxEnableSPI1        = 0x02,
	kAuxEnableSPI2        = 0x04
};

enum eUART1LineCtrlMask {
	kUART1DLAB            = 0b10000000,
	kUART1Break           = 0b01000000,
	kUART18Bit            = 0b00000011
};

enum eUART1ModemCtrlMask {
	kUART1RTS             = 0b00000010
};

enum eUART1LineStatMask {
	kUART1XmitIdle        = 0b01000000,
	kUART1XmitEmpty       = 0b00100000,
	kUART1RecvOverrun     = 0b00000010,
	kUART1DataReady       = 0b00000001
};

enum eUART1ModemStatMask {
	kUART1XmitFIFOLevel   = 0x0f000000,
	kUART1RecvFIFOLevel   = 0x000f0000,
	kUART1XmitDone        = 0x00000200,
	kUART1XmitFIFOEmpty   = 0x00000100,
};

enum eUART1ExtraCtrlMask {
	kUART1CTSAssert       = 0b10000000,
	kUART1RTSAssert       = 0b01000000,
	kUART1RTSAuto         = 0b00110000,
	kUART1UseCTS          = 0b00001000,
	kUART1RTSIsRTR        = 0b00000100,
	kUART1TxEnable        = 0b00000010,
	kUART1RxEnable        = 0b00000001
};

volatile unsigned long *mAux;

int initUART();

void sendByte ( unsigned char byte );

void WaitForSendingToComplete();
