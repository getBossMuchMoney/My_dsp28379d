/******************************************************************************
File name: UartDebug.h
Author: Miller
Version: V00
Date: 2020.4.9
Description: 	the head file for the UartDebug.c used for IPMOS debug
Notes:
Copyright:
******************************************************************************/
#ifndef _UARTDEBUG_H
#define _UARTDEBUG_H

#include "app.h"

typedef    unsigned char   uint8;

#define     cDebugUartEn            1
#define     cDataBufEn              1

#define     cMeasureLength          27

//Costant define
#define     MAX_COMMAND_LENGTH      50
#define     cUserBufSize            150
#define     cEOI                    0x0D
#define		cConfigTimeOut			1
#define		cDelay1sOut				1

#define		MAX_SCI_NO				1	// Maxmam Sci-port Number
#define		cQueueBufSize			50	// size of each queue


// constant definition
#define		cSciTypeSci				0	/*		normal sci moule 			  */
#define		cSciTypeIo2Sci			1	/*		io2sci module				  */
#define		cSciTypeCapSci			2	/*		Capsci module				  */

#define		cSciTxRdy				0	// return value of write operation
#define		cSciTxBusy				1

#define		cSciRxRdy				0	// return value of read operation
#define		cSciRxBufEmpty			1

#define		cBaudRate1200			12	// input value of change baudrate
#define		cBaudRate2400			24
#define		cBaudRate4800			48
#define		cBaudRate9600			96
#define		cBaudRate19200			92

//constant definition
#define     cSciRxRdy					0
#define	    cSciRxBusy					1

#define     cSciTxRdy					0
#define	    cSciTxBusy					1

#define			cQBufNormal			0
#define			cQBufFull			1
#define			cQBufEmpty			2

#define			cQDiscard			0
#define			cQCoverFirst		1
#define			cQCoverLast			2

//Global Macro
#define mSciAEnableRxInt()          SciaRegs.SCICTL2.bit.RXBKINTENA=1
#define mSciADisableRxInt()         SciaRegs.SCICTL2.bit.RXBKINTENA=0
#define mSciBEnableRxInt()          SciaRegs.SCICTL2.bit.RXBKINTENA=1
#define mSciBDisableRxInt()         SciaRegs.SCICTL2.bit.RXBKINTENA=0
#define mSciCEnableRxInt()          SciaRegs.SCICTL2.bit.RXBKINTENA=1
#define mSciCDisableRxInt()         SciaRegs.SCICTL2.bit.RXBKINTENA=0

#define mSciAEnableTxInt()          SciaRegs.SCICTL2.bit.TXINTENA=1
#define mSciADisableTxInt()         SciaRegs.SCICTL2.bit.TXINTENA=0
#define mSciBEnableTxInt()          SciaRegs.SCICTL2.bit.TXINTENA=1
#define mSciBDisableTxInt()         SciaRegs.SCICTL2.bit.TXINTENA=0
#define mSciCEnableTxInt()          SciaRegs.SCICTL2.bit.TXINTENA=1
#define mSciCDisableTxInt()         SciaRegs.SCICTL2.bit.TXINTENA=0

#define SCIA_RX_STATUS              SciaRegs.SCIRXST.all
#define SCIB_RX_STATUS              SciaRegs.SCIRXST.all
#define SCIC_RX_STATUS              SciaRegs.SCIRXST.all

#define SCIA_RX_RDY_STATUS          SciaRegs.SCIRXST.bit.RXRDY
#define SCIB_RX_RDY_STATUS          SciaRegs.SCIRXST.bit.RXRDY
#define SCIC_RX_RDY_STATUS          SciaRegs.SCIRXST.bit.RXRDY

#define SCIA_TX_RDY_STATUS          SciaRegs.SCICTL2.bit.TXRDY
#define SCIB_TX_RDY_STATUS          SciaRegs.SCICTL2.bit.TXRDY
#define SCIC_TX_RDY_STATUS          SciaRegs.SCICTL2.bit.TXRDY

#define SCIA_RX_ERR_STATUS          SciaRegs.SCIRXST.bit.RXERROR
#define SCIB_RX_ERR_STATUS          ScibRegs.SCIRXST.bit.RXERROR
#define SCIC_RX_ERR_STATUS          ScicRegs.SCIRXST.bit.RXERROR
#define SCIA_TX_EMP_STATUS          SciaRegs.SCICTL2.bit.TXEMPTY
#define SCIB_TX_EMP_STATUS          ScibRegs.SCICTL2.bit.TXEMPTY
#define SCIC_TX_EMP_STATUS          ScicRegs.SCICTL2.bit.TXEMPTY


#define SCIA_RX_DATA                SciaRegs.SCIRXBUF.bit.SAR
#define SCIB_RX_DATA                ScibRegs.SCIRXBUF.bit.SAR
#define SCIC_RX_DATA                ScicRegs.SCIRXBUF.bit.SAR

#define mSciATxData(x)              SciaRegs.SCITXBUF.bit.TXDT=x
#define mSciBTxData(x)              ScibRegs.SCITXBUF.bit.TXDT=x
#define mSciCTxData(x)              ScicRegs.SCITXBUF.bit.TXDT=x


typedef	struct{
    void	*pIn;
    void	*pOut;
    void	*pStart;
    uint16_t	length;
    uint16_t 	size;
    uint16_t  step;
}QUEUE;

typedef struct
{
	unsigned char   bTxStatus;
	uint16_t  wTxLength;
	unsigned char   *pbTx;
	uint16_t  *pwTx;
	QUEUE   *pqRx;
	unsigned char   bSciType;
} SciStruct;


extern void 	sRs232Chk(uint8 bSciID);
extern	void    sInitialSci(uint8 bSciID,uint8 bType);
extern	void	sSciRxISR(uint8	bSciID);
extern	uint8	sSciRead(uint8	bSciID,uint8 *pBuf);
extern	void	sSciTxISR(uint8 bSciID);
extern	uint8	sSciWrite(uint8 bSciID,
									uint8 *pstart,uint16_t wLength);
extern  uint8	sSciWriteBinary(uint8 bSciID,
										uint16_t *pstart,uint16_t wLength);
extern	void	sSciChangeBaudRate(uint8 bSciID,uint8 bBandrate);

// external reference
extern 	void	sSciErrorChk(uint8 bSciID,uint8 bBandrate);
extern	uint8	sbSciGetRxData(uint8 bSciID);
extern	uint8	sbSciGetRxRdy(uint8 bSciID);
extern	uint8	sbSciGetTxRdy(uint8 bSciID);
extern	void	sSciTxData(uint8 bSciID,uint8 bData);
extern	void	sSciStopTx(uint8 bSciID);
extern	void	sSciStopRx(uint8 bSciID);
extern	void	sSciStartRx(uint8 bSciID);
extern	void	sSciResetRx(uint8 bSciID);
extern	void	sSciResetTx(uint8 bSciID);
extern	void	sQInit(QUEUE *pq,void *start,uint16_t size,uint16_t AddrStep);
extern	uint8	sQDataIn(QUEUE	*pq,void *pdata,uint8 option);
extern	uint8	sQDataOut(QUEUE *pq,void *pdata);
extern  uint16_t  u16_Tx2RxFlag[MAX_SCI_NO];

#if cDataBufEn == 1
extern void sDataLogInit(void);
extern void sDataLog(void);
#endif

#endif












































