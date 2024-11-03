/******************************************************************************
File name: UartDebug.c
Author: Miller
Version: V00
Date: 2020.4.9
Description: 	for IPMOS debug
Notes:
Copyright:
******************************************************************************/
#include "app.h"
#include "F28x_Project.h"
#include "RTOS_APP.h"
#include "RTOS_Define.h"
#include "ShareData.h"
#include "Adc_User.h"
#include "UartDebug.h"
#include "Gpio_User.h"
#include "SuperTask.h"
#include "MeasureTask.h"

#ifdef cDebugUartEn
//Viriable
uint8 bUserDataBuf[MAX_SCI_NO][cUserBufSize];
uint8 bCommandBuffer[MAX_SCI_NO][MAX_COMMAND_LENGTH];
uint8 bConoffCnt=0;
uint8 *pbCmdBuffer[MAX_SCI_NO];
uint16_t	uwCommandLength[MAX_SCI_NO];
uint16_t 	uwTranmitDataBuff[4020];
uint16_t 	uwInterval = 0;			// the interval between two data
uint16_t 	uwInterval1 = 0;
uint16_t 	uwTrigger = 0;
uint16_t 	uwTriggerSource = 0;
uint16_t 	uwTriggerWaiting = 0;
uint16_t 	uwSnatchDataCnt = 0;	// the number of data to be snatched

extern uint16_t wTurnTestCmd;
void sSendMatlabGraphdata(void);


void 	sRs232Chk(uint8 bSciID);
void 	sParsing(uint8 bSciID);
void 	sQ1Command(uint8 bSciID);
void    sSciQ3Com(uint8 bSciID);
void    sSciQDCom(uint8 bSciID);
void    sSciTONCom(uint8 bSciID);
void    sSciTOFFCom(uint8 bSciID);
void    sSciRONCom(uint8 bSciID);
void    sSciIACom(uint8 bSciID);
void    sSciIDCom(uint8 bSciID);
void    sSciKICom(uint8 bSciID);
void    sSciKPCom(uint8 bSciID);
void    sSciOPCom(uint8 bSciID);
void    sSciSCCom(uint8 bSciID);
void    sSciQHCom(uint8 bSciID);


void	sInitialSci(uint8 bSciID,uint8 bType);
void	sSplit(uint16_t wSendData);
uint8	sSciGetTxStatus(uint8 bSciID);
uint8	sBaudChangeStatue[MAX_SCI_NO];
uint8	sSciGetTxRdy0(void);
uint8	sSciGetRxRdy0(void);
void	sSciTx0(uint8 bData);
void	sSciStopTx0(void);
void	sSciStopRx0(void);
void	sSciStartRx0(void);
void	sSciResetRx0(void);
void	sSciResetTx0(void);
void	sSciChangeBaudRate0(uint8 bBandrate);
uint8	sbGetSciRxData0(void);
uint8	sbGetSciRxRdy0(void);
uint8	sbGetSciTxRdy0(void);
void	sSciTxData0(uint8 bData);

uint8	sSciGetTxRdy1(void);
uint8	sSciGetRxRdy1(void);
void	sSciTx1(uint8 bData);
void	sSciStopTx1(void);
void	sSciStopRx1(void);
void	sSciStartRx1(void);
void	sSciResetRx1(void);
void	sSciResetTx1(void);
void	sSciChangeBaudRate1(uint8 bBandrate);
uint8	sbGetSciRxData1(void);
uint8	sbGetSciRxRdy1(void);
uint8	sbGetSciTxRdy1(void);
void	sSciTxData1(uint8 bData);

uint8	sSciGetTxRdy2(void);
uint8	sSciGetRxRdy2(void);
void	sSciTx2(uint8 bData);
void	sSciStopTx2(void);
void	sSciStopRx2(void);
void	sSciStartRx2(void);
void	sSciResetRx2(void);
void	sSciResetTx2(void);
void	sSciChangeBaudRate2(uint8 bBandrate);
uint8	sbGetSciRxData2(void);
uint8	sbGetSciRxRdy2(void);
uint8	sbGetSciTxRdy2(void);
void	sSciTxData2(uint8 bData);

void	sQInit(QUEUE *pq,void *start,uint16_t size,uint16_t AddrStep);
uint8	sQDataIn(QUEUE	*pq,void *pdata,uint8 option);
uint8	sQDataOut(QUEUE *pq,void *pdata);

uint8 sbNumToXXXx(uint16_t wNumber, uint8 *pbBuffer);
uint8 sbNumToXXxx(uint16_t wNumber, uint8 *pbBuffer);
uint8 sbNumToXXXxx(uint16_t wNumber, uint8 *pbBuffer);
uint8 sbNumAsciiPick(uint16_t wNumber, uint8 bPosition);
uint8 sbNumToXXXX(uint16_t wNumber, uint8 *pbBuffer);
uint8 sbNumToXxxxx(uint16_t wNumber, uint8 *pbBuffer);
uint8 sbNumToXXxxx(uint16_t wNumber, uint8 *pbBuffer);


uint8 sbNumToXXXxUnit(uint16_t wNumber, uint8 *pbBuffer, uint8 bUnit);
uint8 sbNumToXXxxUnit(uint16_t wNumber, uint8 *pbBuffer, uint8 bUnit);
uint8 sbNumToXXXXX(uint16_t wNumber, uint8 *pbBuffer);


// List of Sci structure and queue
uint8   wBinaryMode = 0;
uint8   bHighByte = 0;
uint8   bLowByte = 0;
uint8   bSendHighHalfByte = 0;
uint8	bSciDataTemp = 0;
uint16_t  u16_MatlabGraphDataStart = 0;
uint16_t  u16_Tx2RxFlag[MAX_SCI_NO];

SciStruct   SciList[MAX_SCI_NO];
QUEUE       QList[MAX_SCI_NO];
uint8		bQueueBuffer[MAX_SCI_NO][cQueueBufSize];

uint16_t wscitasktest;

unsigned char GetCheckSum(unsigned char *buf, unsigned char bLen);
void sSetCode_CH0(uint8 bSciID);

#if (cDataBufEn == 1)
#define DLOG_SIZE 1000
int16_t Gui_wSampleRate,Gui_wSampleLength;
int16_t Gui_CH1_ID,Gui_CH2_ID,Gui_CH3_ID,Gui_CH4_ID;
int16_t Gui_wLogEnable,Gui_wLogStatus,wDataLogCnt,wDataLogPointer;
//int16_t DBUFF[4][DLOG_SIZE];
void sDataLog(void);
void sDataLogInit(void);
void sSendGuiGraphData(void);

//float value
float *f32_GraphIdTab[50] = {
	0,
	&stSampleCal.f32_PFC_I_R1, &stSampleCal.f32_PFC_I_R2, &stSampleCal.f32_PFC_I_S1, &stSampleCal.f32_PFC_I_S2, //4
	&stSampleCal.f32_PFC_I_T1, &stSampleCal.f32_PFC_I_T2, &stSampleCal.f32_Grid_I_R, &stSampleCal.f32_Grid_I_S, //8
	&stSampleCal.f32_Grid_I_T, &stSampleCal.f32_Grid_V_RS, &stSampleCal.f32_Grid_V_ST, &stSampleCal.f32_Grid_V_TR, //12
	&stSampleCal.f32_BUS_V,    &stSampleCal.f32_NBUS_V,    &stSampleCalCla.f32_Buck_I1, &stSampleCalCla.f32_Buck_I2, //16
	&stSampleCalCla.f32_Output_I, &stSampleCalCla.f32_Output_V, &stClarkGridVolt.f32_As, &stClarkGridVolt.f32_Bs,    //20
	&stDqPll.f32_Dr,           &stDqPll.f32_Qr,            &stCtrlPiPll.f32_Out,         &stDqPll.f32_ThetaGraph,    //24
	&stClarkGridCurr1.f32_As,  &stClarkGridCurr1.f32_Bs,   &stClarkGridCurr1.f32_Zs,    &stClarkGridCurr2.f32_As,    //28
	&stClarkGridCurr2.f32_Bs,  &stClarkGridCurr1.f32_Zs,   &stClarkGridCurr.f32_As,     &stClarkGridCurr1.f32_Bs,    //32
	&stCtrl2p2zAs2.f32_x0,     &stCtrl2p2zBs2.f32_x0,      &stCtrl2p2zAs1.f32_Ref,      &stCtrl2p2zAs1.f32_Fbk,     //36
	&stCtrl2p2zAs1.f32_y0,     &stCtrl2p2zAs1.f32_y,       &stCtrl2p2zBs1.f32_Ref,      &stCtrl2p2zBs1.f32_Fbk,     //40
    &stCtrl2p2zBs1.f32_y0,     &stCtrl2p2zBs1.f32_y,       &stDqPll.f32_SinGraph,      &stDqPll.f32_CosGraph,     //44
    &stDqPll.f32_FreqRefGraph,
};

//Fix point value
int16_t *i16_GraphIdTab[50] = {
   0,
   &stAdcSample.i16_Pfc_I_R1, &stAdcSample.i16_Pfc_I_R2, &stAdcSample.i16_Pfc_I_S1, &stAdcSample.i16_Pfc_I_S2,    //4
   &stAdcSample.i16_Pfc_I_T1, &stAdcSample.i16_Pfc_I_T2, &stAdcSample.i16_Grid_I_R, &stAdcSample.i16_Grid_I_S,    //8
   &stAdcSample.i16_Grid_I_T, &stAdcSample.i16_Grid_V_RS, &stAdcSample.i16_Grid_V_ST,  &stAdcSample.i16_Grid_V_TR,//12
   &stAdcSample.i16_Bus_V,    &stAdcSample.i16_NBus_V,    &stSampleCla.i16_Buck_I1,    &stSampleCla.i16_Buck_I2,//16
   &stSampleCla.i16_Output_I, &stSampleCla.i16_Output_V,  (int16_t *)&stPwmCmpr1.u16_PwmCmpr1,    (int16_t *)&stPwmCmpr1.u16_PwmCmpr2,  //20
   (int16_t *)&stPwmCmpr1.u16_PwmCmpr3,  (int16_t *)&stDcPwmCmpr.u16_PwmCmpr1,  (int16_t *)&stDcPwmCmpr.u16_PwmCmpr1,
};

#ifdef _FLASH
#pragma CODE_SECTION(sDataLog, ".TI.ramfunc");
#endif
void sDataLog(void)
{
	uint16_t wIndex;
	uint16_t u16_IdTemp;
	if(Gui_wLogEnable == 1)
	{
		wDataLogCnt++;
		if(wDataLogCnt >= Gui_wSampleRate)
		{
			wDataLogCnt = 0;
			wIndex = 3 + wDataLogPointer;

			u16_IdTemp = Gui_CH1_ID;
			if(u16_IdTemp < 50)
			{
			    uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[u16_IdTemp]));
			}
			else
			{
			    u16_IdTemp = u16_IdTemp - 50;
			    uwTranmitDataBuff[wIndex] = (int16_t)((*f32_GraphIdTab[u16_IdTemp]) * 10.0f);
			}
			wIndex += Gui_wSampleLength;

			u16_IdTemp = Gui_CH2_ID;
			if(u16_IdTemp < 50)
            {
                uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[u16_IdTemp]));
            }
            else
            {
                u16_IdTemp = u16_IdTemp - 50;
                uwTranmitDataBuff[wIndex] = (int16_t)((*f32_GraphIdTab[u16_IdTemp]) * 10.0f);
            }
			wIndex += Gui_wSampleLength;

			u16_IdTemp = Gui_CH3_ID;
            if(u16_IdTemp < 50)
            {
                uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[u16_IdTemp]));
            }
            else
            {
                u16_IdTemp = u16_IdTemp - 50;
                uwTranmitDataBuff[wIndex] = (int16_t)((*f32_GraphIdTab[u16_IdTemp]) * 10.0f);
            }
            wIndex += Gui_wSampleLength;

            u16_IdTemp = Gui_CH4_ID;
            if(u16_IdTemp < 50)
            {
                uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[u16_IdTemp]));
            }
            else
            {
                u16_IdTemp = u16_IdTemp - 50;
                uwTranmitDataBuff[wIndex] = (int16_t)((*f32_GraphIdTab[u16_IdTemp]) * 10.0f);
            }

            /*uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[Gui_CH1_ID]));
            wIndex += Gui_wSampleLength;
			uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[Gui_CH2_ID]));
			wIndex += Gui_wSampleLength;
			uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[Gui_CH3_ID]));
			wIndex += Gui_wSampleLength;
			uwTranmitDataBuff[wIndex] = (uint16_t)(*(i16_GraphIdTab[Gui_CH4_ID]));*/
			wDataLogPointer++;
			if(wDataLogPointer >= Gui_wSampleLength)
			{
				wDataLogPointer = 0;
				Gui_wLogEnable = 0;
				Gui_wLogStatus = 2;
			}
		}
	}
	else
	{
		wDataLogPointer = 0;
		wDataLogCnt = 0;
	}
}

void sDataLogInit(void)
{
	Gui_CH1_ID = 1;
	Gui_CH2_ID = 2;
	Gui_CH3_ID = 3;
	Gui_CH4_ID = 4;
	Gui_wSampleRate = 1;
	Gui_wSampleLength = DLOG_SIZE;
	Gui_wLogEnable = 0;
	Gui_wLogStatus = 0;
	wDataLogCnt = 0;
	wDataLogPointer = 0;
}

#endif


/*******************************************************************************
Function name:	sRs232Chk(uint8 bSciID)
Description:  	check the state of command be received, then put data into 
				command buffer by calling sSciRead()				
Calls:        	sParsingCent()
Called By: 		sInterfaceTask()
Parameters: 	bSciID: the ID of SCI channel
Return: 		void
*******************************************************************************/
void 	sRs232Chk(uint8 bSciID)
{
	uint16_t	uwSciTemp;
	
	while(1)
	{

		if(Gui_wLogStatus == 2)
		{
			sSendGuiGraphData();
			Gui_wLogStatus = 0;
		}
		uwSciTemp = sSciRead(bSciID,pbCmdBuffer[bSciID]);
		
		if(uwSciTemp == cSciRxBufEmpty)
		{			
			break;
		}
		
		if(uwCommandLength[bSciID] >= MAX_COMMAND_LENGTH)
		{
			pbCmdBuffer[bSciID] = bCommandBuffer[bSciID];
			uwCommandLength[bSciID] = 0;
		}
		/*else if((bCommandBuffer[bSciID][0] == 'S') && (bCommandBuffer[bSciID][1] == 'P'))
		{
		    if(uwCommandLength[bSciID] == bCommandBuffer[bSciID][2])
		    {

		    }
		}*/
		else if(((*pbCmdBuffer[bSciID]) == 0x0D))
		{	// the end of current command
			*pbCmdBuffer[bSciID] = cEOI;	
			sParsing(bSciID);
			wscitasktest++;
			pbCmdBuffer[bSciID] = bCommandBuffer[bSciID];
			uwCommandLength[bSciID] = 0;
		}
		else
		{
			uwCommandLength[bSciID] ++;
			pbCmdBuffer[bSciID] ++;
		}
	}
}

/*******************************************************************************
Function name:	sParsing
Description:  	Parse command & do something according to the command.	
				Search command index by dichotomy. We use strncmp() function 
				from string standard library
Calls:        	strncmp()
Called By: 		sRs232Chk()
Parameters: 	bSciID: the ID of SCI channel
Return: 		void
*******************************************************************************/
void 	sParsing(uint8 bSciID)
{	
	if ((bCommandBuffer[bSciID][0] == 'Q') && (bCommandBuffer[bSciID][1] == '1'))			//PSnGnLnHnEnDn
	{
		sQ1Command(bSciID);
	}
	else if ((bCommandBuffer[bSciID][0] == 'Q') && (bCommandBuffer[bSciID][1] == '3'))			//PSnGnLnHnEnDn
	{
		sSciQ3Com(bSciID);
	}
	else if ((bCommandBuffer[bSciID][0] == 'Q') && (bCommandBuffer[bSciID][1] == 'D'))			//PSnGnLnHnEnDn
	{
		sSciQDCom(bSciID);
	}
	else if ((bCommandBuffer[bSciID][0] == 'T') && (bCommandBuffer[bSciID][1] == 'N'))          //PSnGnLnHnEnDn
    {
	    sSciTONCom(bSciID);
    }
	else if ((bCommandBuffer[bSciID][0] == 'T') && (bCommandBuffer[bSciID][1] == 'F'))          //PSnGnLnHnEnDn
    {
        sSciTOFFCom(bSciID);
    }

}


void sSetCode_CH0(uint8 bSciID)
{
	
}

void sSendGuiGraphData(void)
{
//  uint16_t uwTemp;
//  int i;
    //long wCheckSum;

    uwTranmitDataBuff[0] = 0xAAAA;
    uwTranmitDataBuff[1] = 0x0001;
    uwTranmitDataBuff[2] = Gui_wSampleLength*4;

    //wCheckSum = 0x01 + Gui_wSampleLength;
    /*for(i = 0;i < Gui_wSampleLength;i ++)
    {
        uwTranmitDataBuff[3 + i] = DBUFF[0][i];
        uwTranmitDataBuff[3 + i + Gui_wSampleLength] = DBUFF[1][i];
        uwTranmitDataBuff[3 + i + Gui_wSampleLength*2] = DBUFF[2][i];
        uwTranmitDataBuff[3 + i + Gui_wSampleLength*3] = DBUFF[1][i];
        //wCheckSum += DBUFF[uwTemp][i];
    }*/

    //uwTranmitDataBuff[2 + Gui_wSampleLength] = wCheckSum & 0x0000FFFF;

    sSciWriteBinary(0,uwTranmitDataBuff,(Gui_wSampleLength*4 + 3));
}


unsigned char GetCheckSum(unsigned char *buf, unsigned char bLen)
{
	unsigned char Checksum, i;
	Checksum = buf[0];
	for(i = 1; i < bLen; i++)
	{
	       Checksum = Checksum^buf[i];
	}
	return Checksum;
}


void 	sQ1Command(uint8 bSciID)
{
    uint8	bStrLen = 0;
    uint16_t i;
    uint16_t *pDataBuf;
    pDataBuf = &stMeasure.u16_Grid_V_RS_Rms;

	if((bCommandBuffer[bSciID][2] == 0x0d))
	{	
	    bUserDataBuf[bSciID][bStrLen++] = 'Q';
	    bUserDataBuf[bSciID][bStrLen++] = '1';
	    bUserDataBuf[bSciID][bStrLen++] = '0';
	    bUserDataBuf[bSciID][bStrLen++] = '1';
	    bUserDataBuf[bSciID][bStrLen++] = 0;
	    bUserDataBuf[bSciID][bStrLen++] = 0;

	    for(i=0; i<cMeasureLength; i++)
	    {
	        bUserDataBuf[bSciID][bStrLen++] = ((*pDataBuf)>>8) & 0xff;
	        bUserDataBuf[bSciID][bStrLen++] = (*pDataBuf) & 0xff;
	        pDataBuf++;
	    }

	    //physical addr
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u16_PhysicalAddr>>8) & 0xff;
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u16_PhysicalAddr) & 0xff;

	    //system mode
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u16_SysMode>>8) & 0xff;
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u16_SysMode) & 0xff;

	    //system status
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u16_SysStatus.all>>8) & 0xff;
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u16_SysStatus.all) & 0xff;

	    //fault code
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_FaultCode.all>>24) & 0xff;
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_FaultCode.all>>16) & 0xff;
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_FaultCode.all>>8) & 0xff;
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_FaultCode.all) & 0xff;

	    //software version
	    bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_SwVersion>>24) & 0xff;
        bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_SwVersion>>16) & 0xff;
        bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_SwVersion>>8) & 0xff;
        bUserDataBuf[bSciID][bStrLen++] = (stSuper.u32_SwVersion) & 0xff;

        bUserDataBuf[bSciID][4] = (bStrLen>>8) & 0xff;
        bUserDataBuf[bSciID][5] = (bStrLen) & 0xff;

		sSciWrite(bSciID,bUserDataBuf[bSciID],bStrLen);
	}
}

void sSendMatlabGraphdata(void)
{
	/*uint8	bStrLen = 0;
	int16_t   i16_Temp = 0;
	uint8   bDataTemp = 0;
	uint8   bCheckSum = 0;

	bUserDataBuf[0][bStrLen++] = 0xAA;
	bCheckSum += 0xAA;          //1
	bUserDataBuf[0][bStrLen++] = 0xAA;
	bCheckSum += 0xAA;          //2    

	//i16_Temp = stOisAlgData.iq_Gyro_X>>12;
	bDataTemp = (uint8)((i16_Temp>>8) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					 //3     gyro_x
	
	bDataTemp = (uint8)((i16_Temp) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					    //4
	
	//i16_Temp = stOisAlgData.iq_Gyro_Y>>12;
	bDataTemp = (uint8)((i16_Temp>>8) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;						 //5   gyro_y
	
	bDataTemp = (uint8)((i16_Temp) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp; 					 //6
	
	//i16_Temp = stOisAlgData.iq_Gyro_Z>>12;
	bDataTemp = (uint8)((i16_Temp>>8) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					    //7   gyroz
	
	bDataTemp = (uint8)((i16_Temp) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;  				    //8   
	
	i16_Temp = stOisAlgData.iq_Pitch_Graph>>12;
	bDataTemp = (uint8)((i16_Temp>>8) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;				      //9   pitch
	
	bDataTemp = (uint8)((i16_Temp) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					 //10
	
	i16_Temp = stOisAlgData.iq_Yaw_Graph>>12;
	bDataTemp = (uint8)((i16_Temp>>8) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					 //11   yaw
	
	bDataTemp = (uint8)((i16_Temp) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					 //12
	
	i16_Temp = stOisAlgData.iq_Roll_Graph>>12;
	bDataTemp = (uint8)((i16_Temp>>8) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;						 //13   roll
	
	bDataTemp = (uint8)((i16_Temp) & 0xff);
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					 //14
	
	bDataTemp = 0;
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;						 //15   temp
	
	bDataTemp = 0;
	bUserDataBuf[0][bStrLen++] = bDataTemp;
	bCheckSum += bDataTemp;					 //16
	
	bCheckSum &= 0xff;
	bUserDataBuf[0][bStrLen++] = bCheckSum;
	//bCheckSum += bDataTemp;;                //17
	
	bUserDataBuf[0][bStrLen++] = 0x55;       //18
	bUserDataBuf[0][bStrLen++] = 0x55;       //19

	sSciWrite(0,bUserDataBuf[0],bStrLen); */
}


/*******************************************************************************
Function name:	sQDCommand(unsigned char bSciID)
Description:  	transmit Data has been snatched
Calls:        	
Called By: 		
Parameters: 	bSciID: the ID of SCI channel
Return: 		void
*******************************************************************************/
void    sSciQDCom(unsigned char bSciID)
{
	uint16_t uwTemp;
	int i;
	long wCheckSum;
	
	uwTemp = bCommandBuffer[bSciID][2] - 48;
	
	if((uwTemp >= 4) || (Gui_wLogStatus != 2))
	{
		return;
	}

	uwTranmitDataBuff[0] = 0x01;		
	uwTranmitDataBuff[1] = Gui_wSampleLength;	
	
	wCheckSum = 0x01 + Gui_wSampleLength;
	for(i = 0;i < Gui_wSampleLength;i ++)
	{
		//uwTranmitDataBuff[2 + i] = DBUFF[uwTemp][i];
		//wCheckSum += DBUFF[uwTemp][i];
	}	
	
	uwTranmitDataBuff[2 + Gui_wSampleLength] = wCheckSum & 0x0000FFFF;

	sSciWriteBinary(bSciID,uwTranmitDataBuff,Gui_wSampleLength + 3);
}

/*******************************************************************************
Function name:	sQ3Command(unsigned char bSciID)
Description:  	Set parameters for SnatchData 
Calls:        	
Called By: 		
Parameters: 	bSciID: the ID of SCI channel
Return: 		void
*******************************************************************************/
void    sSciQ3Com(unsigned char bSciID)
{
	uint16_t uwSnatchDataCntTemp;
	uint16_t uwIntervalTemp;
	uint16_t uwDataKindTemp1;
	uint16_t uwDataKindTemp2;
	uint16_t uwDataKindTemp3;
	uint16_t uwDataKindTemp4;
	//uint16_t uwTriggerTemp;
	//uint16_t i;
	//uint16_t uwLengthTemp;
	
	/*uwLengthTemp = uwCommandLength[bSciID];
	
	for(i = 0;i < uwLengthTemp;i ++)
	{
		bUserDataBuf[bSciID][i] = bCommandBuffer[bSciID][i];		
	}*/

	//sSciWrite(bSciID,bUserDataBuf[bSciID],uwLengthTemp);
	
	//aaa
	uwSnatchDataCntTemp = (bCommandBuffer[bSciID][2] - 48) * 1000 + 
			     (bCommandBuffer[bSciID][3] - 48) * 100 + (bCommandBuffer[bSciID][4] - 48)*10 +
			     (bCommandBuffer[bSciID][5] - 48);
	//bbb
	uwIntervalTemp = (bCommandBuffer[bSciID][7] - 48) * 10 + bCommandBuffer[bSciID][8] - 48; 
	//cc
	uwDataKindTemp1 = (bCommandBuffer[bSciID][10] - 48) * 10 + 
	                  bCommandBuffer[bSciID][11] - 48;
	//dd
	uwDataKindTemp2 = (bCommandBuffer[bSciID][13] - 48) * 10 + 
	                  bCommandBuffer[bSciID][14] - 48;
	//ee
	uwDataKindTemp3 = (bCommandBuffer[bSciID][16] - 48) * 10 + 
	                  bCommandBuffer[bSciID][17] - 48;
	//ff
	uwDataKindTemp4 = (bCommandBuffer[bSciID][19] - 48) * 10 + 
	                  bCommandBuffer[bSciID][20] - 48;
	//gg
	//uwTriggerTemp = bCommandBuffer[bSciID][22] - 48;
	
	
	/*if(uwSnatchDataCntTemp > DLOG_SIZE || uwIntervalTemp > DLOG_SIZE || uwTriggerTemp > 4)
	{
		return;
	}*/
	
	DINT;
	//uwSaveDataCnt = 0;
	Gui_wSampleLength = uwSnatchDataCntTemp;
	Gui_wSampleRate = uwIntervalTemp;
	//uwInterval1 = uwIntervalTemp;
	Gui_CH1_ID = uwDataKindTemp1;
	Gui_CH2_ID = uwDataKindTemp2;
	Gui_CH3_ID = uwDataKindTemp3;
	Gui_CH4_ID = uwDataKindTemp4;
	Gui_wLogEnable = 1;
	Gui_wLogStatus = 1;
	EINT;
}

void    sSciTONCom(uint8 bSciID)
{
	/*uint8	bStrLen = 0;


	bUserDataBuf[bSciID][bStrLen++] = 'O';
	bUserDataBuf[bSciID][bStrLen++] = 'K';
	bUserDataBuf[bSciID][bStrLen++] = 0xd;
	sSciWrite(bSciID,bUserDataBuf[bSciID],bStrLen);*/
    sRTOSEventSend(cPrioSuper,eSuperTunOnEvt);
}
void    sSciTOFFCom(uint8 bSciID)
{
	/*uint8	bStrLen = 0;

	bUserDataBuf[bSciID][bStrLen++] = 'O';
	bUserDataBuf[bSciID][bStrLen++] = 'K';
	bUserDataBuf[bSciID][bStrLen++] = 0xd;
	sSciWrite(bSciID,bUserDataBuf[bSciID],bStrLen);*/
    sRTOSEventSend(cPrioSuper,eSuperTunOffEvt);
}
/**** global function ****/
/*******************************************************************************
Function name:	sInitialSci(void)
Description:  	initialize sci struct & receive buffer
Calls:     sQInit()
Called By: SCI task
Parameters:	bSciID: the number of sci port
			bType: the type of sci
Return: 	null
*******************************************************************************/
void    sInitialSci(uint8 bSciID,uint8 bType)
{
        SciStruct   *pSci;
        QUEUE       *pQ;

        pSci =& SciList[bSciID];

        pSci->pqRx =& QList[bSciID];
        pQ = pSci->pqRx;
        sQInit(pQ,bQueueBuffer[bSciID],cQueueBufSize,sizeof(bQueueBuffer[bSciID][0]));

        pSci->bTxStatus = cSciTxRdy;
        pSci->wTxLength = 0;

        pSci->bSciType = bType;

		pbCmdBuffer[bSciID] = bCommandBuffer[bSciID];
		u16_Tx2RxFlag[bSciID] = 0;
}

/*******************************************************************************
Function name:	sSciRxISR()
Description:  	deal with SCI receive interrupt & put received data into queue
Calls:
Called By: ISR of SCI receive
Parameters:	bSciID: the number of sci port
Return: 	null
*******************************************************************************/
void    sSciRxISR(uint8 bSciID)
{
    SciStruct       *psci;
    QUEUE           *pq;

    psci =& SciList[bSciID];
    pq = psci->pqRx;

    if(sbSciGetRxRdy(bSciID) == cSciRxRdy)	// confirm recerve finished
    {
        //sSciResetRx(bSciID);
        bSciDataTemp = sbSciGetRxData(bSciID);
        (void)sQDataIn(pq,&bSciDataTemp,cQCoverLast);
    }
}

/*******************************************************************************
Function name:	sSciRead()
Description:  	Transfer data read from queue to the array for applications
Calls:
Called By: SCI timer event
Parameters:	bSciID: the number of sci port
			pBuf:	the pointer to destination array
Return: 	cSciRxBufEmpty: no more data
			cSciRxRdy: more data to be transfered
*******************************************************************************/
uint8   sSciRead(uint8  bSciID,uint8 *pBuf)
{
        QUEUE           *pq;
        uint8   temp;
        SciStruct       *psci;

        psci =& SciList[bSciID];
        pq=psci->pqRx;


        DINT;
        temp = sQDataOut(pq,pBuf);
        EINT;

        if(temp == cQBufEmpty)
        {
            return(cSciRxBufEmpty);
        }
        else
        {
            return(cSciRxRdy);
        }
}

/*******************************************************************************
Function name:	sSciTxISR()
Description:  	deal with SCI transmission interrupt & decides next action which
				is to stop from transmitting or to continue
Calls:
Called By: ISR of SCI transmission
Parameters:	bSciID: the number of sci port
Return: 	null
*******************************************************************************/
void    sSciTxISR(uint8 bSciID)
{
        SciStruct       *psci;

        psci =& SciList[bSciID];

        if(sbSciGetTxRdy(bSciID) == cSciTxRdy)
        {
            sSciResetTx(bSciID);
            if(psci->wTxLength == 0)
            {
                wBinaryMode = 0;
                psci->bTxStatus = cSciTxRdy;
                sSciStopTx(bSciID);
            }
            else
            {
                if(wBinaryMode == 0)
                {
                    sSciTxData(bSciID,*(psci->pbTx));
                    (psci->pbTx)++;
                    (psci->wTxLength)--;
                }
                else	// sent 16-bit data in binary mode
                {
                    if(bSendHighHalfByte == 1)	// low byte to be sent
                    {
                        sSciTxData(bSciID,bLowByte);
                        bSendHighHalfByte = 0;
                        (psci->pwTx)++;
                        (psci->wTxLength)--;
                    }
                    else	// high byte to be sent
                    {
                        sSplit(*(psci->pwTx));
                        bSendHighHalfByte = 1;
                        sSciTxData(bSciID,bHighByte);
                    }
                }
            }
        }
}

/*******************************************************************************
Function name:	sSciWrite()
Description:  	Initiate transmission of SCI in ASCII model
Calls:
Called By: User applications
Parameters:	bSciID: the number of sci port
			pstart: pointer to the pointer of start address
			wLength:  the length of data to be transmitted
Return: 	cSciTxBusy: failed to transmit for transmision is proceeding
			cSciTxRdy: begin transmission
*******************************************************************************/
uint8   sSciWrite(uint8 bSciID,uint8 *pstart,uint16_t wLength)
{
    SciStruct       *psci;

    psci =& SciList[bSciID];

    if(psci->bTxStatus == cSciTxBusy)
    {
        return(cSciTxBusy);
    }

    DINT;
    psci->pbTx = pstart;
    psci->wTxLength = wLength;
    psci->bTxStatus = cSciTxBusy;

    sSciTxData(bSciID,*(psci->pbTx));
    (psci->pbTx)++;
    (psci->wTxLength)--;
    EINT;


    return(cSciTxRdy);
}

/*******************************************************************************
Function name:	sSplit()
Description:  	split a 16-bit word into two bytes,
					high 8-bit byte & low 8-bit byte
Calls:
Called By: sSciWriteBinary()
Parameters:	wSendData: the value to be splitted

Return: 	cSciTxBusy: failed to transmit for transmision is proceeding
			cSciTxRdy: begin transmission
*******************************************************************************/
void sSplit(uint16_t wSendData)
{
    bHighByte = (uint8)(wSendData >> 8);
    bLowByte = (uint8)(wSendData & 0x00FF);
}

/*******************************************************************************
Function name:	sSciWriteBinary()
Description:  	Initiate transmission of SCI in binary model
Calls:
Called By: User applications
Parameters:	bSciID: the number of sci port
			pstart: pointer to the pointer of start address
			wLength:  the length of data to be transmitted
Return: 	cSciTxBusy: failed to transmit for transmision is proceeding
			cSciTxRdy: begin transmission
*******************************************************************************/
uint8   sSciWriteBinary(uint8 bSciID,uint16_t *pstart,uint16_t wLength)
{
    SciStruct       *psci;

    psci =& SciList[bSciID];

    if(psci->bTxStatus == cSciTxBusy)
    {
        return(cSciTxBusy);
    }

    DINT;
    wBinaryMode = 1;
    psci->pwTx = pstart;
    psci->wTxLength = wLength;
    psci->bTxStatus = cSciTxBusy;
    //split
    sSplit(*(psci->pwTx));
    bSendHighHalfByte = 1;
    sSciTxData(bSciID,bHighByte);
    EINT;
    return(cSciTxRdy);
}

/*******************************************************************************
Function name:	sSciGetTxStatus()
Description:  	Get the status of SCI transmission
Calls:
Called By: SCI timer event
Parameters:	bSciID: the number of sci port
Return: 	cSciTxBusy: transmision is proceeding
			cSciTxRdy: complete transmission
*******************************************************************************/
uint8   sSciGetTxStatus(uint8 bSciID)
{
    SciStruct       *psci;

    psci =& SciList[bSciID];

    return(psci->bTxStatus);
}

/*******************************************************************************
Function name:	sbSciGetRxData()
Description:  	Get data from SCI register
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	return data be received
*******************************************************************************/
uint8	sbSciGetRxData(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			return(sbGetSciRxData0());
			//break;
		case 1:
			return(sbGetSciRxData1());
			//break;
		case 2:
			return(sbGetSciRxData2());
			//break;
		default:
			break;
	}
	return(0);
}

/*******************************************************************************
Function name:	sbSciGetRxRdy()
Description:  	Get state of RXD from SCI register
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	return state value of RXD
*******************************************************************************/
uint8	sbSciGetRxRdy(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			return(sbGetSciRxRdy0());
			//break;
		case 1:
			return(sbGetSciRxRdy1());
			//break;
		case 2:
			return(sbGetSciRxRdy2());
			//break;
		default:
			break;
	}
	return(0);
}

/*******************************************************************************
Function name:	sbSciGetTxRdy()
Description:  	Get state of TXD from SCI register
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	return state value of TXD
*******************************************************************************/
uint8	sbSciGetTxRdy(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			return(sbGetSciTxRdy0());
			//break;
		case 1:
			return(sbGetSciTxRdy1());
			//break;
		case 2:
			return(sbGetSciTxRdy2());
			//break;
		default:
			break;
	}
	return(0);
}

/*******************************************************************************
Function name:	sSciTxData()
Description:  	write data to SCI register
Calls:
Called By:
Parameters:	bSciID: the number of sci port
			bData: the data-byte to be transimitted
Return: 	null
*******************************************************************************/
void	sSciTxData(uint8 bSciID,uint8 bData)
{
	switch(bSciID)
	{
		case 0:
			sSciTxData0(bData);
			break;
		case 1:
			sSciTxData1(bData);
			break;
		case 2:
			sSciTxData2(bData);
			break;
		default:
			break;
	}
}

/*******************************************************************************
Function name:	sSciStopTx()
Description:  	Stop TXD
Calls:
Called By:
Parameters:	bSciID: the number of sci port
			bData: the data-byte to be transimitted
Return: 	null
*******************************************************************************/
void	sSciStopTx(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			sSciStopTx0();
			u16_Tx2RxFlag[bSciID] = 1;
			break;
		case 1:
			sSciStopTx1();
			break;
		case 2:
			sSciStopTx2();
			break;
		default:
			break;
	}
}

/*******************************************************************************
Function name:	sSciStopRx()
Description:  	Stop RXD
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	null
*******************************************************************************/
void	sSciStopRx(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			sSciStopRx0();
			break;
		case 1:
			sSciStopRx1();
			break;
		case 2:
			sSciStopRx2();
			break;
		default:
			break;
	}
}

/*******************************************************************************
Function name:	sSciStartRx()
Description:  	Start TXD
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	null
*******************************************************************************/
void	sSciStartRx(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			sSciStartRx0();
			break;
		case 1:
			sSciStartRx1();
			break;
		case 2:
			sSciStartRx2();
			break;
		default:
			break;
	}
}

/*******************************************************************************
Function name:	sSciResetRx()
Description:  	Reset RXD
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	null
*******************************************************************************/
void	sSciResetRx(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			sSciResetRx0();
			break;
		case 1:
			sSciResetRx1();
			break;
		case 2:
			sSciResetRx2();
			break;
		default:
			break;
	}
}

/*******************************************************************************
Function name:	sSciResetTx()
Description:  	Reset TXD
Calls:
Called By:
Parameters:	bSciID: the number of sci port
Return: 	null
*******************************************************************************/
void	sSciResetTx(uint8 bSciID)
{
	switch(bSciID)
	{
		case 0:
			sSciResetTx0();
			break;
		case 1:
			sSciResetTx1();
			break;
		case 2:
			sSciResetTx2();
			break;
		default:
			break;
	}
}


/******************************************************************************/
/*Get Sci Rx Data															  */
/******************************************************************************/
uint8	sbGetSciRxData0(void)
{
	return(SCIA_RX_DATA);
}

uint8	sbGetSciRxData1(void)
{
	return(SCIB_RX_DATA);
}

uint8	sbGetSciRxData2(void)
{
	return(SCIC_RX_DATA);
}
/******************************************************************************/
/*Get Sci Rx ready status													  */
/******************************************************************************/
uint8	sbGetSciRxRdy0(void)
{
	if(SCIA_RX_RDY_STATUS == 1)
	{
		return(cSciRxRdy);
	}
	else
	{
		return(cSciRxBusy);
	}
}

uint8	sbGetSciRxRdy1(void)
{
	if(SCIB_RX_RDY_STATUS == 1)
	{
		return(cSciRxRdy);
	}
	else
	{
		return(cSciRxBusy);
	}
}

uint8	sbGetSciRxRdy2(void)
{
	if(SCIC_RX_RDY_STATUS == 1)
	{
		return(cSciRxRdy);
	}
	else
	{
		return(cSciRxBusy);
	}
}

/******************************************************************************/
/*Get Sci Tx status															  */
/******************************************************************************/
uint8	sbGetSciTxRdy0(void)
{
	if(SCIA_TX_RDY_STATUS == 1)
	{
		return(cSciTxRdy);
	}
	else
	{
		return(cSciTxBusy);
	}
}

uint8	sbGetSciTxRdy1(void)
{
	if(SCIB_TX_RDY_STATUS == 1)
	{
		return(cSciTxRdy);
	}
	else
	{
		return(cSciTxBusy);
	}
}

uint8	sbGetSciTxRdy2(void)
{
	if(SCIC_TX_RDY_STATUS == 1)
	{
		return(cSciTxRdy);
	}
	else
	{
		return(cSciTxBusy);
	}
}

/******************************************************************************/
/*tx a byte and enable tx interrupt											  */
/******************************************************************************/
void	sSciTxData0(uint8 bData)
{
	//LL_USART_ClearFlag_TC(USART2);
    m485_TX1_EN();
	mSciAEnableTxInt();
	mSciATxData(bData);
}

void	sSciTxData1(uint8 bData)
{
    m485_TX2_EN();
	mSciBEnableTxInt();
	mSciBTxData(bData);
}

void	sSciTxData2(uint8 bData)
{
    m485_TX3_EN();
	mSciCEnableTxInt();
	mSciCTxData(bData);
}
/******************************************************************************/
/*disable tx interrupt														  */
/******************************************************************************/
void	sSciStopTx0(void)
{
	mSciADisableTxInt();
	//m485_RX1_EN();
}

void	sSciStopTx1(void)
{
	mSciBDisableTxInt();
}

void	sSciStopTx2(void)
{
	mSciCDisableTxInt();
}
/******************************************************************************/
/*Disable rx interrupt														  */
/******************************************************************************/
void	sSciStopRx0(void)
{
	mSciADisableRxInt();
}

void	sSciStopRx1(void)
{
	mSciBDisableRxInt();
}

void	sSciStopRx2(void)
{
	mSciCDisableRxInt();
}
/******************************************************************************/
/*Enable rx interrupt														  */
/******************************************************************************/
void	sSciStartRx0(void)
{
	mSciAEnableRxInt();
}

void	sSciStartRx1(void)
{
	mSciBEnableRxInt();
}

void	sSciStartRx2(void)
{
	mSciCEnableRxInt();
}

/******************************************************************************/
/*Reset Rx interrupt														  */
/******************************************************************************/
void	sSciResetRx0(void)
{
	return;
}

void	sSciResetRx1(void)
{
	return;
}

void	sSciResetRx2(void)
{
	return;
}
/******************************************************************************/
/*Reset Tx interrupt														  */
/******************************************************************************/
void	sSciResetTx0(void)
{
	return;
}

void	sSciResetTx1(void)
{
	return;
}

void	sSciResetTx2(void)
{
	return;
}

/************************************************************************************
*Function name:	sQInit																*
*Parameters:	pq:	pointer to queue structure to be initialized					*
*				start:start address of ring buffer									*
*				size:the size of the ring buffer                                    *
*               QueueType:Type of the address point in queue                        *
*Description:	initialize a queue structure										*
*************************************************************************************/
void	sQInit(QUEUE *pq,void *start,uint16_t size,uint16_t AddrStep)
{
	pq->pIn = start;
	pq->pOut = start;
	pq->pStart = start;
	pq->length = 0;
	pq->size = size;
    pq->step = AddrStep;
}

/************************************************************************************
*Function name:	sQDataIn															*
*Parameters:	pq:	pointer to queue structure to be initialized					*
*				pdata:the data point to be inserted into the queue					*
*				option:how to deal with the data when the buffer is full			*
*				cQCoverFirst:cover the first data									*
*				cQCoverLast:cover the latest data									*
*Returns:		cQBufNormal:data has been inserted into the queue					*
*				cQBufFull:the buffer is full										*
*Description:	insert a data into the queue										*
*************************************************************************************/
uint8	sQDataIn(QUEUE	*pq,void *pdata,uint8 option)
{

    uint16_t uwAddrStep,uwCnt;
	uint8 *pubTemp;

	uwAddrStep = pq->step;
    if(pq->length == pq->size)
	{
		if(option == cQCoverFirst)
		{
		    //*(pq->pIn) = data;
			for(uwCnt = 0;uwCnt < uwAddrStep;uwCnt++)
	        {
		        *((uint8 *)pq->pIn + uwCnt) = *((uint8 *)pdata + uwCnt);
	        }
			if((uint8 *)pq->pIn == ((uint8 *)pq->pStart + (pq->size-1) * uwAddrStep))
			{
				pq->pIn = pq->pStart;
			}
			else
			{
				pq->pIn = (uint8 *)pq->pIn + uwAddrStep;
			}
			pq->pOut = pq->pIn;
		}
		else if(option == cQCoverLast)
		{
			if(pq->pIn == pq->pStart)
			{
				//*(pq->pStart + (pq->size - 1)) = data;
				pubTemp = (uint8 *)pq->pStart + (pq->size - 1) * uwAddrStep;
				for(uwCnt = 0;uwCnt < uwAddrStep;uwCnt++)
	            {
		            *(pubTemp+ uwCnt) = *((uint8 *)pdata + uwCnt);
	            }
			}
			else
			{
				//*(pq->pIn - 1) = data;
				pubTemp = (uint8 *)pq->pIn - uwAddrStep;
				for(uwCnt = 0;uwCnt < uwAddrStep;uwCnt++)
	            {
		            *(pubTemp+ uwCnt) = *((uint8 *)pdata + uwCnt);
	            }
			}
		}
		return(cQBufFull);
	}
	else
	{
		//*(pq->pIn) = data;
		for(uwCnt = 0;uwCnt < uwAddrStep;uwCnt++)
        {
	        *((uint8 *)pq->pIn + uwCnt) = *((uint8 *)pdata + uwCnt);
        }
		pq->length++;
		if((uint8 *)pq->pIn == (uint8 *)pq->pStart + (pq->size - 1) * uwAddrStep)
		{
			pq->pIn = pq->pStart;
		}
		else
		{
			pq->pIn = (uint8 *)pq->pIn + uwAddrStep;
		}
		return(cQBufNormal);
	}
}
/************************************************************************************
*Function name:	sQDataOut															*
*Parameters:	pq:	pointer to queue structure to be initialized					*
*				pdata:the address to save the data									*
*Returns:		cQBufNormal:data has been inserted into the queue					*
*				cQBufEmpty:the buffer is empty										*
*Description:	Get a data from the queue											*
*************************************************************************************/
uint8	sQDataOut(QUEUE *pq,void *pdata)
{
    uint16_t uwAddrStep,uwCnt;

	uwAddrStep = pq->step;
	if(pq->length == 0)
	{
		return(cQBufEmpty);
	}
	//*pdata = *pq->pOut;
	for(uwCnt = 0;uwCnt < uwAddrStep;uwCnt++)
	{
		*(((uint8 *)pdata + uwCnt)) = *(((uint8 *)(pq->pOut) + uwCnt));
	}

	pq->length--;

	if((uint8 *)pq->pOut == ((uint8 *)pq->pStart + (pq->size- 1) * uwAddrStep))
	{
		pq->pOut = pq->pStart;
	}
	else
	{
		pq->pOut = (uint8 *)pq->pOut + uwAddrStep;
	}
	return(cQBufNormal);
}

/******************************************************************************
Function Name:
	sbNumToXXXx: Convert input wNumber into 3 digital numbers including
		     1 decimal represented byASCII code.
Parameters:
	wNumber: Number to be converted into ASCII code
	pbBuffer: pointer of conversion register
Internal Variable:
	wNumFormated: Number after formatting
	bConvResult: conversion result
Return Value:
	1:1 ( The Number to be converted is normal )
	0:	0 ( The Number to be converted is over 999.9, and limited to 999.9 )
*******************************************************************************/
uint8 sbNumToXXXx(uint16_t wNumber, uint8 *pbBuffer)
{
	uint16_t	wNumFormated;
	uint8	bConvResult= 1;
	
    	if(wNumber > 9999)
    	{
    		wNumFormated= 9999;
    		bConvResult= 0;
    	}
    	else
    	{
    		wNumFormated= wNumber;
    	}
    	*(pbBuffer)=sbNumAsciiPick(wNumFormated,3);
    	*(pbBuffer+1)=sbNumAsciiPick(wNumFormated,2);
    	*(pbBuffer+2)=sbNumAsciiPick(wNumFormated,1);
    	*(pbBuffer+3)='.';
    	*(pbBuffer+4)=sbNumAsciiPick(wNumFormated,0);
    	*(pbBuffer+5)= ' ';
    	return(bConvResult);
}

uint8 sbNumToXXXxUnit(uint16_t wNumber, uint8 *pbBuffer, uint8 bUnit)
{
	uint16_t	wNumFormated;
	uint8	bConvResult= 1;
	
	if(wNumber > 9999)
	{
		wNumFormated= 9999;
		bConvResult= 0;
	}
	else
	{
		wNumFormated= wNumber;
	}
	*(pbBuffer)=sbNumAsciiPick(wNumFormated,3);
	*(pbBuffer+1)=sbNumAsciiPick(wNumFormated,2);
	*(pbBuffer+2)=sbNumAsciiPick(wNumFormated,1);
	*(pbBuffer+3)='.';
	*(pbBuffer+4)=sbNumAsciiPick(wNumFormated,0);
	*(pbBuffer+5)= bUnit;
	*(pbBuffer+6)= ' ';
	return(bConvResult);
}


/******************************************************************************
Function Name:
	sbNumToXXxx: Convert input wNumber into 2 digital numbers including
		     2 decimal represented byASCII code.
Parameters:
	wNumber: Number to be converted into ASCII code
	pbBuffer: pointer of conversion register
Internal Variable:
	wNumFormated: Number after formatting
	bConvResult: conversion result
Return Value:
	1:1 ( The Number to be converted is normal )
	0:	0 ( The Number to be converted is over 99.99, and limited to 99.99 )
*******************************************************************************/
uint8 sbNumToXXxx(uint16_t wNumber, uint8 *pbBuffer)
{
	uint16_t	wNumFormated;
	uint8	bConvResult= 1;
	
    	if(wNumber > 9999)
    	{
    		wNumFormated= 9999;
    		bConvResult=0;
    	}
    	else
    	{
    		wNumFormated= wNumber;
    	}
    	*(pbBuffer)=sbNumAsciiPick(wNumFormated,3);
    	*(pbBuffer+1)=sbNumAsciiPick(wNumFormated,2);
    	*(pbBuffer+2)='.';
    	*(pbBuffer+3)=sbNumAsciiPick(wNumFormated,1);
    	*(pbBuffer+4)=sbNumAsciiPick(wNumFormated,0);
    	*(pbBuffer+5)= ' ';
    	return(bConvResult);
}

uint8 sbNumToXXxxUnit(uint16_t wNumber, uint8 *pbBuffer, uint8 bUnit)
{
	uint16_t	wNumFormated;
	uint8	bConvResult= 1;
	
	if(wNumber > 9999)
	{
		wNumFormated= 9999;
		bConvResult=0;
	}
	else
	{
		wNumFormated= wNumber;
	}
	*(pbBuffer)=sbNumAsciiPick(wNumFormated,3);
	*(pbBuffer+1)=sbNumAsciiPick(wNumFormated,2);
	*(pbBuffer+2)='.';
	*(pbBuffer+3)=sbNumAsciiPick(wNumFormated,1);
	*(pbBuffer+4)=sbNumAsciiPick(wNumFormated,0);
	*(pbBuffer+5)= bUnit;
	*(pbBuffer+6)= ' ';
	return(bConvResult);
}


/******************************************************************************
Function Name:
	sbNumToXXXxx: Convert input wNumber into 3 digital numbers including
		      2 decimal represented byASCII code.
Parameters:
	wNumber: Number to be converted into ASCII code
	pbBuffer: pointer of conversion register
Internal Variable:
	bConvResult: conversion result
Return Value:
	1:1 ( The Number to be converted is normal )
*******************************************************************************/
uint8 sbNumToXXXxx(uint16_t wNumber, uint8 *pbBuffer)
{
	uint8	bConvResult= 1;
	
    	*(pbBuffer)=sbNumAsciiPick(wNumber,4);
    	*(pbBuffer+1)=sbNumAsciiPick(wNumber,3);
    	*(pbBuffer+2)=sbNumAsciiPick(wNumber,2);
    	*(pbBuffer+3)='.';
    	*(pbBuffer+4)=sbNumAsciiPick(wNumber,1);
    	*(pbBuffer+5)=sbNumAsciiPick(wNumber,0);
    	*(pbBuffer+6)= ' ';
    	return(bConvResult);
}

uint8 sbNumToXXxxx(uint16_t wNumber, uint8 *pbBuffer)
{
	uint8	bConvResult= 1;
	
    	*(pbBuffer)=sbNumAsciiPick(wNumber,4);
    	*(pbBuffer+1)=sbNumAsciiPick(wNumber,3);
    	*(pbBuffer+2)='.';
		*(pbBuffer+3)=sbNumAsciiPick(wNumber,2);
    	*(pbBuffer+4)=sbNumAsciiPick(wNumber,1);
    	*(pbBuffer+5)=sbNumAsciiPick(wNumber,0);
    	*(pbBuffer+6)= ' ';
    	return(bConvResult);
}



uint8 sbNumToXxxxx(uint16_t wNumber, uint8 *pbBuffer)
{
	uint8	bConvResult= 1;
	
    	*(pbBuffer)=sbNumAsciiPick(wNumber,4);
    	*(pbBuffer+1)='.';
    	*(pbBuffer+2)=sbNumAsciiPick(wNumber,3);
    	*(pbBuffer+3)=sbNumAsciiPick(wNumber,2);
    	*(pbBuffer+4)=sbNumAsciiPick(wNumber,1);
    	*(pbBuffer+5)=sbNumAsciiPick(wNumber,0);
    	*(pbBuffer+6)= ' ';
    	return(bConvResult);
}


/******************************************************************************
Function Name:
	sbNumAsciiPick: Convert the secleced digital to Ascii code
Parameters:
	wNumber: Number to be converted into ASCII code
	bPosition:
Return Value:
	bCode
*******************************************************************************/
uint8	sbNumAsciiPick(uint16_t wNumber, uint8 bPosition)
{
	uint8	bCode;
	switch( bPosition)
	{
		case 0: default:
			bCode= wNumber%10 + '0';
			break;
		case 1:
			bCode= (wNumber/10)%10 + '0';
			break;
		case 2:
			bCode= (wNumber/100)%10 + '0';
			break;
		case 3:
			bCode= (wNumber/1000)%10 + '0';
			break;
		case 4:
			bCode= (wNumber/10000)%10 + '0';
			break;
		case 5:
			bCode= (wNumber/100000)%10 + '0';
			break;
	}
	return(bCode);
}

/******************************************************************************
Function Name:
	sbNumToXXXX: Convert input wNumber into 4 digital numbers represented by
		    ASCII code.
Parameters:
	wNumber: Number to be converted into ASCII code
	pbBuffer: pointer of conversion register
Internal Variable:
	wNumFormated: Number after formatting
	bConvResult: conversion result
Return Value:
	1:1 ( The Number to be converted is normal )
	0:	0 ( The Number to be converted is over 999, and limited to 999 )
*******************************************************************************/
uint8 sbNumToXXXX(uint16_t wNumber, uint8 *pbBuffer)
{
	uint16_t	wNumFormated;
	uint8	bConvResult= 1;
	
    	if(wNumber > 9999)
    	{
    		wNumFormated= 9999;
    		bConvResult= 0;
    	}
    	else
    	{
    		wNumFormated= wNumber;
    	}
    	*(pbBuffer)=sbNumAsciiPick(wNumFormated,3);
    	*(pbBuffer+1)=sbNumAsciiPick(wNumFormated,2);
    	*(pbBuffer+2)=sbNumAsciiPick(wNumFormated,1);
		*(pbBuffer+3)=sbNumAsciiPick(wNumFormated,0);
    	*(pbBuffer+4)= ' ';
    	return(bConvResult);
}

uint8 sbNumToXXXXX(uint16_t wNumber, uint8 *pbBuffer)
{
    	uint8	bConvResult= 1;
    	*(pbBuffer)=sbNumAsciiPick(wNumber,4);
    	*(pbBuffer+1)=sbNumAsciiPick(wNumber,3);
    	*(pbBuffer+2)=sbNumAsciiPick(wNumber,2);
		*(pbBuffer+3)=sbNumAsciiPick(wNumber,1);
		*(pbBuffer+4)=sbNumAsciiPick(wNumber,0);
    	*(pbBuffer+5)= ' ';
    	return(bConvResult);
}
#endif





































