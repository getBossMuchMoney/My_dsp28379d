/*******************************************************************************
 * File Name: PfcController.c
 * Description: The PFC controller functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "F28x_Project.h"
#include "DCController.h"
#include "Gpio_User.h"
#include "Adc_User.h"
#include "PWM_User.h"
#include "PfcController.h"
#include "DCController.h"
#include "MeasureTask.h"
#include "RTOS_APP.h"
#include "RTOS_Define.h"
#include "ShareData.h"
#include "SuperTask.h"
#include "math.h"
#include "stdio.h"
#include "app.h"
#include "UartDebug.h"

uint16_t u16_CtrlLoopCnt;
int16_t  i16_PeriodCnt;
uint16_t u16_SampleCnt;
CTRL_2P2Z_ST stCtrl2p2zAs1 = {0};    //Grid current loop
CTRL_2P2Z_ST stCtrl2p2zBs1 = {0};
CTRL_2P2Z_ST stCtrl2p2zAs2 = {0};
CTRL_2P2Z_ST stCtrl2p2zBs2 = {0};
CTRL_2P2Z_ST stCtrl2p2zZs2 = {0};
//CTRL_2P2Z_ST stCtrl2p2zBus;

CTRL_PI_ST   stCtrlPiPll = {0};
#pragma DATA_SECTION(stCtrlPiBus,"CLADataLS1");
CTRL_PI_ST   stCtrlPiBus = {0};
CLARK_ST     stClarkGridVolt = {0};
CLARK_ST     stClarkGridCurr1 = {0};
CLARK_ST     stClarkGridCurr2 = {0};
CLARK_ST     stClarkGridCurr = {0};
CLARK_ST     stClarkCapCurr = {0};
SVPWM_ST     stSvPwm1 = {0};
SVPWM_ST     stSvPwm2 = {0};
CMPR_ST      stPwmCmpr1 = {0};
CMPR_ST      stPwmCmpr2 = {0};
PLL_ST       stDqPll = {0};
float        f32_Ts;

float f32_DummyRes = (0.5*PFC_L/(DUMMY_R*FLT_C));

void sSampelCalculation(void);
void sPfcControllerInit(void);
void sPfcControllerReset(void);
void sPfcController(void);
void sSvpwm(SVPWM_ST *stSvpwm);
void sSvpwm_Zero(SVPWM_ST *stSvpwm,float f32_Vz);

#ifdef _FLASH
#pragma CODE_SECTION(sSampelCalculation, ".TI.ramfunc");
#endif
void sSampelCalculation(void)
{
    stSampleCal.f32_PFC_I_R1 = (float)stAdcSample.i16_Pfc_I_R1 * cPFC_I_Ratio;
    stSampleCal.f32_PFC_I_R2 = (float)stAdcSample.i16_Pfc_I_R2 * cPFC_I_Ratio;
    stSampleCal.f32_PFC_I_S1 = (float)stAdcSample.i16_Pfc_I_S1 * cPFC_I_Ratio;
    stSampleCal.f32_PFC_I_S2 = (float)stAdcSample.i16_Pfc_I_S2 * cPFC_I_Ratio;
    stSampleCal.f32_PFC_I_T1 = (float)stAdcSample.i16_Pfc_I_T1 * cPFC_I_Ratio;
    stSampleCal.f32_PFC_I_T2 = (float)stAdcSample.i16_Pfc_I_T2 * cPFC_I_Ratio;
    stSampleCal.f32_Grid_I_R = (float)stAdcSample.i16_Grid_I_R * cGrid_I_Ratio;
    stSampleCal.f32_Grid_I_S = (float)stAdcSample.i16_Grid_I_S * cGrid_I_Ratio;
    stSampleCal.f32_Grid_I_T = (float)stAdcSample.i16_Grid_I_T * cGrid_I_Ratio;
    stSampleCal.f32_Grid_V_RS = (float)stAdcSample.i16_Grid_V_RS * cGrid_V_Ratio;
    stSampleCal.f32_Grid_V_ST = (float)stAdcSample.i16_Grid_V_ST * cGrid_V_Ratio;
    stSampleCal.f32_Grid_V_TR = (float)stAdcSample.i16_Grid_V_TR * cGrid_V_Ratio;
    stSampleCal.f32_BUS_V = (float)stAdcSample.i16_Bus_V * cBus_V_Ratio;
    stSampleCal.f32_NBUS_V = (float)stAdcSample.i16_NBus_V * cBus_V_Ratio;

    stSampleCal.f32_PFC_I_R1_SumTemp += stSampleCal.f32_PFC_I_R1 * stSampleCal.f32_PFC_I_R1;
    stSampleCal.f32_PFC_I_R2_SumTemp += stSampleCal.f32_PFC_I_R2 * stSampleCal.f32_PFC_I_R2;
    stSampleCal.f32_PFC_I_S1_SumTemp += stSampleCal.f32_PFC_I_S1 * stSampleCal.f32_PFC_I_S1;
    stSampleCal.f32_PFC_I_S2_SumTemp += stSampleCal.f32_PFC_I_S2 * stSampleCal.f32_PFC_I_S2;
    stSampleCal.f32_PFC_I_T1_SumTemp += stSampleCal.f32_PFC_I_T1 * stSampleCal.f32_PFC_I_T1;
    stSampleCal.f32_PFC_I_T2_SumTemp += stSampleCal.f32_PFC_I_T2 * stSampleCal.f32_PFC_I_T2;
    stSampleCal.f32_Grid_I_R_SumTemp += stSampleCal.f32_Grid_I_R * stSampleCal.f32_Grid_I_R;;
    stSampleCal.f32_Grid_I_S_SumTemp += stSampleCal.f32_Grid_I_S * stSampleCal.f32_Grid_I_S;
    stSampleCal.f32_Grid_I_T_SumTemp += stSampleCal.f32_Grid_I_T * stSampleCal.f32_Grid_I_T;
    stSampleCal.f32_Grid_V_RS_SumTemp += stSampleCal.f32_Grid_V_RS * stSampleCal.f32_Grid_V_RS;
    stSampleCal.f32_Grid_V_ST_SumTemp += stSampleCal.f32_Grid_V_ST * stSampleCal.f32_Grid_V_ST;
    stSampleCal.f32_Grid_V_TR_SumTemp += stSampleCal.f32_Grid_V_TR * stSampleCal.f32_Grid_V_TR;
    stSampleCal.f32_BUS_V_SumTemp += stSampleCal.f32_BUS_V;
    stSampleCal.f32_NBUS_V_SumTemp += stSampleCal.f32_NBUS_V;
    stSampleCal.u16_SumCnt++;
    if(stSampleCal.u16_SumCnt >= stSampleCal.u16_SumCntSet)
    {
        stSampleCal.u16_SumCnt = 0;
        stSampleCal.f32_PFC_I_R1_Sum = stSampleCal.f32_PFC_I_R1_SumTemp;
        stSampleCal.f32_PFC_I_R2_Sum = stSampleCal.f32_PFC_I_R2_SumTemp;
        stSampleCal.f32_PFC_I_S1_Sum = stSampleCal.f32_PFC_I_S1_SumTemp;
        stSampleCal.f32_PFC_I_S2_Sum = stSampleCal.f32_PFC_I_S2_SumTemp;
        stSampleCal.f32_PFC_I_T1_Sum = stSampleCal.f32_PFC_I_T1_SumTemp;
        stSampleCal.f32_PFC_I_T2_Sum = stSampleCal.f32_PFC_I_T2_SumTemp;
        stSampleCal.f32_Grid_I_R_Sum = stSampleCal.f32_Grid_I_R_SumTemp;
        stSampleCal.f32_Grid_I_S_Sum = stSampleCal.f32_Grid_I_S_SumTemp;
        stSampleCal.f32_Grid_I_T_Sum = stSampleCal.f32_Grid_I_T_SumTemp;
        stSampleCal.f32_Grid_V_RS_Sum = stSampleCal.f32_Grid_V_RS_SumTemp;
        stSampleCal.f32_Grid_V_ST_Sum = stSampleCal.f32_Grid_V_ST_SumTemp;
        stSampleCal.f32_Grid_V_TR_Sum = stSampleCal.f32_Grid_V_TR_SumTemp;
        stSampleCal.f32_BUS_V_Sum = stSampleCal.f32_BUS_V_SumTemp;
        stSampleCal.f32_NBUS_V_Sum = stSampleCal.f32_NBUS_V_SumTemp;

        stSampleCal.f32_PFC_I_R1_SumTemp = 0;
        stSampleCal.f32_PFC_I_R2_SumTemp = 0;
        stSampleCal.f32_PFC_I_S1_SumTemp = 0;
        stSampleCal.f32_PFC_I_S2_SumTemp = 0;
        stSampleCal.f32_PFC_I_T1_SumTemp = 0;
        stSampleCal.f32_PFC_I_T2_SumTemp = 0;
        stSampleCal.f32_Grid_I_R_SumTemp = 0;
        stSampleCal.f32_Grid_I_S_SumTemp = 0;
        stSampleCal.f32_Grid_I_T_SumTemp = 0;
        stSampleCal.f32_Grid_V_RS_SumTemp = 0;
        stSampleCal.f32_Grid_V_ST_SumTemp = 0;
        stSampleCal.f32_Grid_V_TR_SumTemp = 0;
        stSampleCal.f32_BUS_V_SumTemp = 0;
        stSampleCal.f32_NBUS_V_SumTemp = 0;

        sRTOSEventSendIsr(cPrioMeasure,eMeasureACCalEvt);
    }
    if(stSampleCal.u16_DcSumStatus != stSampleCalCla.u16_DcSumStatus)
    {
        sRTOSEventSendIsr(cPrioMeasure,eMeasureDCCalEvt);
    }
    stSampleCal.u16_DcSumStatus = stSampleCalCla.u16_DcSumStatus;
    stSampleCal.u16_SumCntCla = stSampleCalCla.u16_SumCntCla;
    /*stSampleCal.f32_Buck_I1_Cla = stSampleCalCla.f32_Buck_I1;
    stSampleCal.f32_Buck_I2_Cla = stSampleCalCla.f32_Buck_I2;
    stSampleCal.f32_Output_I_Cla = stSampleCalCla.f32_Output_I;
    stSampleCal.f32_Output_V_Cla = stSampleCalCla.f32_Output_V;
    stSampleCal.f32_Buck_I1_Sum_Cla = stSampleCalCla.f32_Buck_I1_Sum;
    stSampleCal.f32_Buck_I2_Sum_Cla = stSampleCalCla.f32_Buck_I2_Sum;
    stSampleCal.f32_Output_I_Sum_Cla = stSampleCalCla.f32_Output_I_Sum;
    stSampleCal.f32_Output_V_Sum_Cla = stSampleCalCla.f32_Output_V_Sum;
    stSampleCal.f32_Buck_I1_SumTemp_Cla = stSampleCalCla.f32_Buck_I1_SumTemp;
    stSampleCal.f32_Buck_I2_SumTemp_Cla = stSampleCalCla.f32_Buck_I2_SumTemp;
    stSampleCal.f32_Output_I_SumTemp_Cla = stSampleCalCla.f32_Output_I_SumTemp;
    stSampleCal.f32_Output_V_SumTemp_Cla = stSampleCalCla.f32_Output_V_SumTemp;*/
}

void sPfcControllerInit(void)
{
    stSampleCal.f32_PFC_I_R1_SumTemp = 0;
    stSampleCal.f32_PFC_I_R2_SumTemp = 0;
    stSampleCal.f32_PFC_I_S1_SumTemp = 0;
    stSampleCal.f32_PFC_I_S2_SumTemp = 0;
    stSampleCal.f32_PFC_I_T1_SumTemp = 0;
    stSampleCal.f32_PFC_I_T2_SumTemp = 0;
    stSampleCal.f32_Grid_I_R_SumTemp = 0;
    stSampleCal.f32_Grid_I_S_SumTemp = 0;
    stSampleCal.f32_Grid_I_T_SumTemp = 0;
    stSampleCal.f32_Grid_V_RS_SumTemp = 0;
    stSampleCal.f32_Grid_V_ST_SumTemp = 0;
    stSampleCal.f32_Grid_V_TR_SumTemp = 0;
    stSampleCal.f32_BUS_V_SumTemp = 0;
    stSampleCal.f32_NBUS_V_SumTemp = 0;
    stSampleCal.u16_SumCnt = 0;
    stSampleCal.u16_SumCntSet = 720;
    stSampleCal.u16_DcSumStatus = 0;

    u16_CtrlLoopCnt = 0;
    i16_PeriodCnt = (int16_t)((int32_t)CPU_CLK*1000L/Fs)>>2;
    u16_SampleCnt = 0;
    f32_Ts = 1.0f/36000.0f;

  //Current loop Controller parameter
  /*    0.3819 z^2 + 0.0496 z - 0.3323
  ------------------------------
      z^2 - 1.286 z + 0.2857         */
    stCtrl2p2zAs1.f32_A0 = 1.286f;
    stCtrl2p2zAs1.f32_A1 = -0.286f;
    stCtrl2p2zAs1.f32_B0 = 0.3819f;
    stCtrl2p2zAs1.f32_B1 = 0.0496f;
    stCtrl2p2zAs1.f32_B2 = -0.3323f;
    stCtrl2p2zAs1.f32_Fbk = 0.0f;
    stCtrl2p2zAs1.f32_Ref = 0.0f;
    stCtrl2p2zAs1.f32_Max = 700.0f * INV_SQRT3;
    stCtrl2p2zAs1.f32_Min = -700.0f * INV_SQRT3;
    stCtrl2p2zAs1.f32_x0 = 0.0f;
    stCtrl2p2zAs1.f32_x1 = 0.0f;
    stCtrl2p2zAs1.f32_y = 0.0f;
    stCtrl2p2zAs1.f32_y0 = 0.0f;
    stCtrl2p2zAs1.f32_y1 = 0.0f;
    stCtrl2p2zAs1.f32_Temp = 0;

    /*    0.3819 z^2 + 0.0496 z - 0.3323
  ------------------------------
      z^2 - 1.286 z + 0.2857         */
    stCtrl2p2zBs1.f32_A0 = 1.286f;
    stCtrl2p2zBs1.f32_A1 = -0.286f;
    stCtrl2p2zBs1.f32_B0 = 0.3819f;
    stCtrl2p2zBs1.f32_B1 = 0.0496f;
    stCtrl2p2zBs1.f32_B2 = -0.3323f;
    stCtrl2p2zBs1.f32_Fbk = 0.0f;
    stCtrl2p2zBs1.f32_Ref = 0.0f;
    stCtrl2p2zBs1.f32_Max = 700.0f * INV_SQRT3;
    stCtrl2p2zBs1.f32_Min = -700.0f * INV_SQRT3;
    stCtrl2p2zBs1.f32_x0 = 0.0f;
    stCtrl2p2zBs1.f32_x1 = 0.0f;
    stCtrl2p2zBs1.f32_y = 0.0f;
    stCtrl2p2zBs1.f32_y0 = 0.0f;
    stCtrl2p2zBs1.f32_y1 = 0.0f;
    stCtrl2p2zBs1.f32_Temp = 0;

    stCtrl2p2zAs2.f32_A0 = 1.0f;
    //stCtrl2p2zAs2.f32_A1 = -0.286f;
    //stCtrl2p2zAs2.f32_B0 = 0.3819f * 0.1f;
    //stCtrl2p2zAs2.f32_B1 = 0.0496f * 0.1f;
    //stCtrl2p2zAs2.f32_B2 = -0.3323f * 0.1f;
    stCtrl2p2zAs2.f32_Fbk = 0.0f;
    stCtrl2p2zAs2.f32_Ref = 0.0f;
    stCtrl2p2zAs2.f32_Max = 700.0f * INV_SQRT3 * 0.1f;
    stCtrl2p2zAs2.f32_Min = -700.0f * INV_SQRT3 * 0.1f;
    stCtrl2p2zAs2.f32_x0 = 0.0f;
    stCtrl2p2zAs2.f32_x1 = 0.0f;
    stCtrl2p2zAs2.f32_y = 0.0f;
    stCtrl2p2zAs2.f32_y0 = 0.0f;
    stCtrl2p2zAs2.f32_y1 = 0.0f;
    stCtrl2p2zAs2.f32_Temp = 0;

    stCtrl2p2zBs2.f32_A0 = 1.0f;
    //stCtrl2p2zBs2.f32_A1 = -0.286f;
    //stCtrl2p2zBs2.f32_B0 = 0.3819f;
    //stCtrl2p2zBs2.f32_B1 = 0.0496f;
    //stCtrl2p2zBs2.f32_B2 = -0.3323f;
    stCtrl2p2zBs2.f32_Fbk = 0.0f;
    stCtrl2p2zBs2.f32_Ref = 0.0f;
    stCtrl2p2zBs2.f32_Max = 700.0f * INV_SQRT3 * 0.1f;
    stCtrl2p2zBs2.f32_Min = -700.0f * INV_SQRT3 * 0.1f;
    stCtrl2p2zBs2.f32_x0 = 0.0f;
    stCtrl2p2zBs2.f32_x1 = 0.0f;
    stCtrl2p2zBs2.f32_y = 0.0f;
    stCtrl2p2zBs2.f32_y0 = 0.0f;
    stCtrl2p2zBs2.f32_y1 = 0.0f;
    stCtrl2p2zBs2.f32_Temp = 0;

    /*    0.3819 z^2 + 0.0496 z - 0.3323
      ------------------------------
          z^2 - 1.286 z + 0.2857         */
    stCtrl2p2zZs2.f32_A0 = 1.286f;
    stCtrl2p2zZs2.f32_A1 = -0.286f;
    stCtrl2p2zZs2.f32_B0 = 0.3819f;
    stCtrl2p2zZs2.f32_B1 = 0.0496f;
    stCtrl2p2zZs2.f32_B2 = -0.3323f;
    stCtrl2p2zZs2.f32_Fbk = 0.0f;
    stCtrl2p2zZs2.f32_Ref = 0.0f;
    stCtrl2p2zZs2.f32_Max = 700.0f * INV_SQRT3;
    stCtrl2p2zZs2.f32_Min = -700.0f * INV_SQRT3;
    stCtrl2p2zZs2.f32_x0 = 0.0f;
    stCtrl2p2zZs2.f32_x1 = 0.0f;
    stCtrl2p2zZs2.f32_y = 0.0f;
    stCtrl2p2zZs2.f32_y0 = 0.0f;
    stCtrl2p2zZs2.f32_y1 = 0.0f;
    stCtrl2p2zZs2.f32_Temp = 0;

    //BUS voltage loop
    /* 5 * (s + 200)/s */
    stCtrlPiBus.f32_Ref = 500.0f;
    stCtrlPiBus.f32_Fbk = 0.0f;
    stCtrlPiBus.f32_Kp = 5.0f;
    stCtrlPiBus.f32_Ki = 1.0f;
    stCtrlPiBus.f32_Max = 500.0f;
    stCtrlPiBus.f32_Min = -500.0f;
    stCtrlPiBus.f32_Err = 0.0f;
    stCtrlPiBus.f32_Err0 = 0.0f;
    stCtrlPiBus.f32_Out = 0.0f;
    stCtrlPiBus.f32_Ui = 0.0f;
    stCtrlPiBus.f32_Uo = 0.0f;
    stCtrlPiBus.f32_Up = 0.0f;

    stSvPwm1.M_Limit = 1.0f;
    stSvPwm2.M_Limit = 1.0f;

    //PLL loop
    stCtrlPiPll.f32_Ref = 0.0f;
    stCtrlPiPll.f32_Fbk = 0.0f;
    stCtrlPiPll.f32_Kp = 1.0f;
    stCtrlPiPll.f32_Ki = 0.014f;
    stCtrlPiPll.f32_Max = 100.0f;
    stCtrlPiPll.f32_Min = -100.0f;
    stCtrlPiPll.f32_Err = 0.0f;
    stCtrlPiPll.f32_Err0 = 0.0f;
    stCtrlPiPll.f32_Out = 0.0f;
    stCtrlPiPll.f32_Ui = 0.0f;
    stCtrlPiPll.f32_Uo = 0.0f;
    stCtrlPiPll.f32_Up = 0.0f;

    stDqPll.f32_Dr = 0.0f;
    stDqPll.f32_Qr = 0.0f;
    stDqPll.f32_Sin = 0.0f;
    stDqPll.f32_Cos = 1.0f;
    stDqPll.f32_FreqRef = 50.0f;
    stDqPll.f32_Ts = 1.0f/36000.0f;
}

void sPfcControllerReset(void)
{
    stCtrlPiBus.f32_Ref = 500.0f;
    stCtrl2p2zAs1.f32_x0 = 0.0f;
    stCtrl2p2zAs1.f32_x1 = 0.0f;
    stCtrl2p2zAs1.f32_y = 0.0f;
    stCtrl2p2zAs1.f32_y0 = 0.0f;
    stCtrl2p2zAs1.f32_y1 = 0.0f;
    stCtrl2p2zAs1.f32_Temp = 0;

    stCtrl2p2zBs1.f32_x0 = 0.0f;
    stCtrl2p2zBs1.f32_x1 = 0.0f;
    stCtrl2p2zBs1.f32_y = 0.0f;
    stCtrl2p2zBs1.f32_y0 = 0.0f;
    stCtrl2p2zBs1.f32_y1 = 0.0f;
    stCtrl2p2zBs1.f32_Temp = 0;

    stCtrl2p2zAs2.f32_x0 = 0.0f;
    stCtrl2p2zAs2.f32_x1 = 0.0f;
    stCtrl2p2zAs2.f32_y = 0.0f;
    stCtrl2p2zAs2.f32_y0 = 0.0f;
    stCtrl2p2zAs2.f32_y1 = 0.0f;
    stCtrl2p2zAs2.f32_Temp = 0;

    stCtrl2p2zBs2.f32_x0 = 0.0f;
    stCtrl2p2zBs2.f32_x1 = 0.0f;
    stCtrl2p2zBs2.f32_y = 0.0f;
    stCtrl2p2zBs2.f32_y0 = 0.0f;
    stCtrl2p2zBs2.f32_y1 = 0.0f;
    stCtrl2p2zBs2.f32_Temp = 0;

    stCtrl2p2zZs2.f32_x0 = 0.0f;
    stCtrl2p2zZs2.f32_x1 = 0.0f;
    stCtrl2p2zZs2.f32_y = 0.0f;
    stCtrl2p2zZs2.f32_y0 = 0.0f;
    stCtrl2p2zZs2.f32_y1 = 0.0f;
    stCtrl2p2zZs2.f32_Temp = 0;

    stCtrlPiBus.f32_Err = 0.0f;
    stCtrlPiBus.f32_Err0 = 0.0f;
    stCtrlPiBus.f32_Out = 0.0f;
    stCtrlPiBus.f32_Ui = 0.0f;
    stCtrlPiBus.f32_Uo = 0.0f;
    stCtrlPiBus.f32_Up = 0.0f;
}


#ifdef _FLASH
#pragma CODE_SECTION(sPfcController, ".TI.ramfunc");
#endif
void sPfcController(void)
{
    int16_t i16_Temp;

    //Calculate the 3s/2s by using the clark
    stClarkGridVolt.f32_As = (stSampleCal.f32_Grid_V_RS - stSampleCal.f32_Grid_V_TR) / 3.0f;
    stClarkGridVolt.f32_Bs = stSampleCal.f32_Grid_V_ST * INV_SQRT3;         //0.5774 = 1/sqrt(3)

    stClarkGridCurr1.f32_As = (2.0f*stSampleCal.f32_PFC_I_R1 - (stSampleCal.f32_PFC_I_S1 + stSampleCal.f32_PFC_I_T1))/3.0f;
    stClarkGridCurr1.f32_Bs = (stSampleCal.f32_PFC_I_S1 - stSampleCal.f32_PFC_I_T1) * INV_SQRT3;
    stClarkGridCurr1.f32_Zs = (stSampleCal.f32_PFC_I_R1+stSampleCal.f32_PFC_I_S1+stSampleCal.f32_PFC_I_T1)/3.0f;

    stClarkGridCurr2.f32_As = (2.0f*stSampleCal.f32_PFC_I_R2 - (stSampleCal.f32_PFC_I_S2 + stSampleCal.f32_PFC_I_T2))/3.0f;
    stClarkGridCurr2.f32_Bs = (stSampleCal.f32_PFC_I_S2 - stSampleCal.f32_PFC_I_T2) * INV_SQRT3;
    stClarkGridCurr2.f32_Zs = (stSampleCal.f32_PFC_I_R2 + stSampleCal.f32_PFC_I_S2 + stSampleCal.f32_PFC_I_T2)/3.0f;

    stClarkGridCurr.f32_As = (2.0f*stSampleCal.f32_Grid_I_R - (stSampleCal.f32_Grid_I_S + stSampleCal.f32_Grid_I_T))/3.0f;
    stClarkGridCurr.f32_Bs = (stSampleCal.f32_Grid_I_S - stSampleCal.f32_Grid_I_T) * INV_SQRT3;

    stClarkCapCurr.f32_As = stClarkGridCurr.f32_As - stClarkGridCurr1.f32_As - stClarkGridCurr2.f32_As;
    stClarkCapCurr.f32_Bs = stClarkGridCurr.f32_Bs - stClarkGridCurr1.f32_Bs - stClarkGridCurr2.f32_Bs;

    //PLL controller
    stDqPll.f32_Dr = stClarkGridVolt.f32_As * stDqPll.f32_Cos + stClarkGridVolt.f32_Bs * stDqPll.f32_Sin;
    stDqPll.f32_Qr = -stClarkGridVolt.f32_As * stDqPll.f32_Sin + stClarkGridVolt.f32_Bs * stDqPll.f32_Cos;
    stCtrlPiPll.f32_Err = stDqPll.f32_Dr;
    CTRL_PI(stCtrlPiPll);
    stCtrlPiPll.f32_Out = stDqPll.f32_FreqRef + stCtrlPiPll.f32_Uo;
#if cPWM_OpenLoop_Test == 1
    stCtrlPiPll.f32_Out = 50.0f;
#endif

    stDqPll.f32_Theta += stCtrlPiPll.f32_Out * stDqPll.f32_Ts * PI2;
    if(stDqPll.f32_Theta >= PI2)
    {
        stDqPll.f32_Theta -= PI2;
    }
    else if(stDqPll.f32_Theta <= -PI2)
    {
        stDqPll.f32_Theta += PI2;
    }
    stDqPll.f32_Sin = __sin(stDqPll.f32_Theta);
    stDqPll.f32_Cos = __cos(stDqPll.f32_Theta);

#if cDataBufEn == 1
    stDqPll.f32_SinGraph = stDqPll.f32_Sin*1000.0f;
    stDqPll.f32_CosGraph = stDqPll.f32_Cos*1000.0f;
    stDqPll.f32_FreqRefGraph = stCtrlPiPll.f32_Out*10.0f;
    stDqPll.f32_ThetaGraph = stDqPll.f32_Theta * 100.0f;
#endif

    //PFC controller
    if((stSuper.u16_SysStatus.BIT.Pfc1PwmStatus == 1) ||
        (stSuper.u16_SysStatus.BIT.Pfc2PwmStatus == 1))
    {
        /* Voltage loop */
        if(u16_CtrlLoopCnt++ >= Fs)
        {
            u16_CtrlLoopCnt = 0;
            stCtrlPiBus.f32_Fbk = stSampleCal.f32_BUS_V;
            //stCtrlPiBus.f32_Ref = 700.0f;
            stCtrlPiBus.f32_Err = stCtrlPiBus.f32_Ref - stCtrlPiBus.f32_Fbk;
            CTRL_PI(stCtrlPiBus);
        }

        //stCtrl2p2zBus.f32_y = 310;

        /* Current loop Reference Cal*/
        stCtrl2p2zAs1.f32_Ref = stCtrlPiBus.f32_Uo * stDqPll.f32_Sin;
        stCtrl2p2zBs1.f32_Ref = -stCtrlPiBus.f32_Uo * stDqPll.f32_Cos;

        /* Current Loop */
        /*  Alpha axis */
        stCtrl2p2zAs1.f32_Fbk = stClarkGridCurr.f32_As;
        stCtrl2p2zAs1.f32_x0 = stCtrl2p2zAs1.f32_Ref - stCtrl2p2zAs1.f32_Fbk;
        stCtrl2p2zAs1.f32_x0 = stCtrl2p2zAs1.f32_x0 * 2.0f - stCtrl2p2zAs1.f32_x1;
        stCtrl2p2zAs1.f32_x1 = stCtrl2p2zAs1.f32_Ref - stCtrl2p2zAs1.f32_Fbk;
        CTRL_2P2Z(stCtrl2p2zAs1);

        /*  Beta axis */
        stCtrl2p2zBs1.f32_Fbk = stClarkGridCurr.f32_Bs;
        stCtrl2p2zBs1.f32_x0 = stCtrl2p2zBs1.f32_Ref - stCtrl2p2zBs1.f32_Fbk;
        stCtrl2p2zBs1.f32_x0 = stCtrl2p2zBs1.f32_x0 * 2.0f - stCtrl2p2zBs1.f32_x1;
        stCtrl2p2zBs1.f32_x1 = stCtrl2p2zBs1.f32_Ref - stCtrl2p2zBs1.f32_Fbk;
        CTRL_2P2Z(stCtrl2p2zBs1);

        //Choke current difference loop
        /* Alpha axis differnce */
        stCtrl2p2zAs2.f32_x0 = stClarkGridCurr1.f32_As - stClarkGridCurr2.f32_As;
        stCtrl2p2zAs2.f32_y0 = stCtrl2p2zAs2.f32_x0 * stCtrl2p2zAs2.f32_A0;
        if(stCtrl2p2zAs2.f32_y0 >= stCtrl2p2zAs2.f32_Max)
        {
            stCtrl2p2zAs2.f32_y0 = stCtrl2p2zAs2.f32_Max;
        }
        else if(stCtrl2p2zAs2.f32_y0 <= stCtrl2p2zAs2.f32_Min)
        {
            stCtrl2p2zAs2.f32_y0 = stCtrl2p2zAs2.f32_Min;
        }

        //Beta axis diference
        stCtrl2p2zBs2.f32_x0 = stClarkGridCurr1.f32_Bs - stClarkGridCurr2.f32_Bs;
        stCtrl2p2zBs2.f32_y0 = stCtrl2p2zAs2.f32_x0 * stCtrl2p2zBs2.f32_A0;
        if(stCtrl2p2zBs2.f32_y0 >= stCtrl2p2zBs2.f32_Max)
        {
            stCtrl2p2zBs2.f32_y0 = stCtrl2p2zBs2.f32_Max;
        }
        else if(stCtrl2p2zBs2.f32_y0 <= stCtrl2p2zBs2.f32_Min)
        {
            stCtrl2p2zBs2.f32_y0 = stCtrl2p2zBs2.f32_Min;
        }

        /* zero sequence current */
        stCtrl2p2zZs2.f32_Fbk = stClarkGridCurr2.f32_Zs;
        stCtrl2p2zZs2.f32_x0 = 0 - stClarkGridCurr2.f32_Zs;
        stCtrl2p2zZs2.f32_x0 = stCtrl2p2zZs2.f32_x0 * 2.0f - stCtrl2p2zZs2.f32_x1;
        stCtrl2p2zZs2.f32_x1 = stCtrl2p2zZs2.f32_Ref - stCtrl2p2zZs2.f32_Fbk;
        CTRL_2P2Z(stCtrl2p2zZs2);

        /* Dummy Resistance take effect */
        stCtrl2p2zAs1.f32_y = -stCtrl2p2zAs1.f32_y0 - stClarkCapCurr.f32_As * f32_DummyRes;
        stCtrl2p2zBs1.f32_y = -stCtrl2p2zBs1.f32_y0 - stClarkCapCurr.f32_Bs * f32_DummyRes;

        /* Svpwm Calculation */
#if cPWM_OpenLoop_Test == 0
        stSvPwm1.Vx = (stCtrl2p2zAs1.f32_y + stCtrl2p2zAs2.f32_y0) * 2.0/stCtrlPiBus.f32_Ref;
        stSvPwm1.Vy = (stCtrl2p2zBs1.f32_y + stCtrl2p2zBs2.f32_y0) * 2.0/stCtrlPiBus.f32_Ref;
        stSvPwm2.Vx = (stCtrl2p2zAs1.f32_y - stCtrl2p2zAs2.f32_y0) * 2.0/stCtrlPiBus.f32_Ref;
        stSvPwm2.Vy = (stCtrl2p2zBs1.f32_y - stCtrl2p2zBs2.f32_y0) * 2.0/stCtrlPiBus.f32_Ref;
        /*stSvPwm1.Vx = (stCtrl2p2zAs1.f32_y) * 2.0/stCtrl2p2zBus.f32_Ref;
        stSvPwm1.Vy = (stCtrl2p2zBs1.f32_y) * 2.0/stCtrl2p2zBus.f32_Ref;
        stSvPwm2.Vx = (stCtrl2p2zAs1.f32_y) * 2.0/stCtrl2p2zBus.f32_Ref;
        stSvPwm2.Vy = (stCtrl2p2zBs1.f32_y) * 2.0/stCtrl2p2zBus.f32_Ref;*/
        sSvpwm(&stSvPwm1);
        //sSvpwm(&stSvPwm2);
        sSvpwm_Zero(&stSvPwm2,-stCtrl2p2zZs2.f32_y0 * 2.0f / stCtrlPiBus.f32_Ref);
#else
        stSvPwm1.Vx = (stCtrlPiBus.f32_Ref) * stDqPll.f32_Sin/700.0f;
        stSvPwm1.Vy = -(stCtrlPiBus.f32_Ref) * stDqPll.f32_Cos/700.0f;
        stSvPwm2.Vx = stSvPwm1.Vx;
        stSvPwm2.Vy = stSvPwm1.Vy;
        sSvpwm(&stSvPwm1);
        sSvpwm(&stSvPwm2);
        /*stSvPwm1.Ta = 0.3f;
        stSvPwm1.Tb = 0.3f;
        stSvPwm1.Tc = 0.3f;
        stSvPwm2.Ta = 0.3f;
        stSvPwm2.Tb = 0.3f;
        stSvPwm2.Tc = 0.3f;*/
#endif


        /* Duty Cal */
        i16_Temp = i16_PeriodCnt>>1;
        stPwmCmpr1.u16_PwmCmpr1 = (int16_t)(stSvPwm1.Ta * i16_Temp) + i16_Temp;
        stPwmCmpr1.u16_PwmCmpr2 = (int16_t)(stSvPwm1.Tb * i16_Temp) + i16_Temp;
        stPwmCmpr1.u16_PwmCmpr3 = (int16_t)(stSvPwm1.Tc * i16_Temp) + i16_Temp;

        stPwmCmpr2.u16_PwmCmpr1 = (int16_t)(stSvPwm2.Ta * i16_Temp) + i16_Temp;
        stPwmCmpr2.u16_PwmCmpr2 = (int16_t)(stSvPwm2.Tb * i16_Temp) + i16_Temp;
        stPwmCmpr2.u16_PwmCmpr3 = (int16_t)(stSvPwm2.Tc * i16_Temp) + i16_Temp;
    }
    else
    {
        stPwmCmpr1.u16_PwmCmpr1 = i16_PeriodCnt>>1;
        stPwmCmpr1.u16_PwmCmpr2 = i16_PeriodCnt>>1;
        stPwmCmpr1.u16_PwmCmpr3 = i16_PeriodCnt>>1;
        stPwmCmpr2.u16_PwmCmpr1 = i16_PeriodCnt>>1;
        stPwmCmpr2.u16_PwmCmpr2 = i16_PeriodCnt>>1;
        stPwmCmpr2.u16_PwmCmpr3 = i16_PeriodCnt>>1;
    }
    mPFC1_CMPR_R() = stPwmCmpr1.u16_PwmCmpr1;
    mPFC1_CMPR_S() = stPwmCmpr1.u16_PwmCmpr2;
    mPFC1_CMPR_T() = stPwmCmpr1.u16_PwmCmpr3;
    mPFC2_CMPR_R() = stPwmCmpr2.u16_PwmCmpr1;
    mPFC2_CMPR_S() = stPwmCmpr2.u16_PwmCmpr2;
    mPFC2_CMPR_T() = stPwmCmpr2.u16_PwmCmpr3;
}

#ifdef _FLASH
#pragma CODE_SECTION(sSvpwm, ".TI.ramfunc");
#endif
void sSvpwm(SVPWM_ST *stSvpwm)
{
    float Tx,Ty,Tz;
    float VX,VY;
    float Vmax,Vmin,Vcom;
    SVPWM_ST *pstTemp;

    pstTemp = stSvpwm;
    VX = pstTemp->Vx;
    VY = pstTemp->Vy;

    Tx = VX;
    Ty = VY * COS30 - VX*0.5;
    Tz = -VY * COS30 - VX*0.5;

    if(Tx > Ty)
    {
        Vmax = Tx;
        Vmin = Ty;
    }
    else
    {
        Vmax = Ty;
        Vmin = Tx;
    }

    if(Tz > Vmax)
    {
        Vmax = Tz;
    }
    else if(Tz < Vmin)
    {
        Vmin = Tz;
    }

    Vcom = (Vmax + Vmin) * 0.5f;
    pstTemp->Ta = Vcom - Tx;
    pstTemp->Tb = Vcom - Ty;
    pstTemp->Tc = Vcom - Tz;

    /*pstTemp->Ta = -Tx;
    pstTemp->Tb = -Ty;
    pstTemp->Tc = -Tz;*/


    if(pstTemp->Ta >= pstTemp->M_Limit)
    {
        pstTemp->Ta = pstTemp->M_Limit;
    }
    else if(pstTemp->Ta <= -pstTemp->M_Limit)
    {
        pstTemp->Ta = -pstTemp->M_Limit;
    }

    if(pstTemp->Tb >= pstTemp->M_Limit)
    {
        pstTemp->Tb = pstTemp->M_Limit;
    }
    else if(pstTemp->Tb <= -pstTemp->M_Limit)
    {
        pstTemp->Tb = -pstTemp->M_Limit;
    }

    if(pstTemp->Tc >= pstTemp->M_Limit)
    {
        pstTemp->Tc = pstTemp->M_Limit;
    }
    else if(pstTemp->Tc <= -pstTemp->M_Limit)
    {
        pstTemp->Tc = -pstTemp->M_Limit;
    }

    return;
}

#ifdef _FLASH
#pragma CODE_SECTION(sSvpwm_Zero, ".TI.ramfunc");
#endif
void sSvpwm_Zero(SVPWM_ST *stSvpwm,float f32_Vz)
{
    float Tx,Ty,Tz;
    float VX,VY;
    float Vmax,Vmin,Vcom;
    SVPWM_ST *pstTemp;

    pstTemp = stSvpwm;
    VX = pstTemp->Vx;
    VY = pstTemp->Vy;

    Tx = VX + f32_Vz;
    Ty = VY * COS30 - VX*0.5 + f32_Vz;
    Tz = -VY * COS30 - VX*0.5 + f32_Vz;

    if(Tx > Ty)
    {
        Vmax = Tx;
        Vmin = Ty;
    }
    else
    {
        Vmax = Ty;
        Vmin = Tx;
    }

    if(Tz > Vmax)
    {
        Vmax = Tz;
    }
    else if(Tz < Vmin)
    {
        Vmin = Tz;
    }

    Vcom = (Vmax + Vmin) * 0.5f;
    pstTemp->Ta = Vcom - Tx - f32_Vz;
    pstTemp->Tb = Vcom - Ty - f32_Vz;
    pstTemp->Tc = Vcom - Tz - f32_Vz;

    /*pstTemp->Ta = -Tx;
    pstTemp->Tb = -Ty;
    pstTemp->Tc = -Tz;*/


    if(pstTemp->Ta >= pstTemp->M_Limit)
    {
        pstTemp->Ta = pstTemp->M_Limit;
    }
    else if(pstTemp->Ta <= -pstTemp->M_Limit)
    {
        pstTemp->Ta = -pstTemp->M_Limit;
    }

    if(pstTemp->Tb >= pstTemp->M_Limit)
    {
        pstTemp->Tb = pstTemp->M_Limit;
    }
    else if(pstTemp->Tb <= -pstTemp->M_Limit)
    {
        pstTemp->Tb = -pstTemp->M_Limit;
    }

    if(pstTemp->Tc >= pstTemp->M_Limit)
    {
        pstTemp->Tc = pstTemp->M_Limit;
    }
    else if(pstTemp->Tc <= -pstTemp->M_Limit)
    {
        pstTemp->Tc = -pstTemp->M_Limit;
    }

    return;
}



