

#include "F28x_Project.h"
#include "RTOS_APP.h"
#include "RTOS_Define.h"
#include "ShareData.h"
#include "Adc_User.h"

#pragma DATA_SECTION(stSampleCal,"CLADataLS1");
SAMPLE_CAL stSampleCal;
#pragma DATA_SECTION(stSampleCla,"CLADataLS1");
ADC_SAMPLE_CLA stSampleCla;
#pragma DATA_SECTION(stSampleCalCla,"CLADataLS1");
SAMPLE_CAL_CLA stSampleCalCla;

#pragma DATA_SECTION(stCtrlPiVout,"CLADataLS1");
CTRL_PI_ST   stCtrlPiVout = {0};
#pragma DATA_SECTION(stCtrlPiIdc,"CLADataLS1");
CTRL_PI_ST   stCtrlPiIdc = {0};
#pragma DATA_SECTION(stCtrlPiPout,"CLADataLS1");
CTRL_PI_ST   stCtrlPiPout = {0};
#pragma DATA_SECTION(stCtrlPiIdcDiff,"CLADataLS1");
CTRL_PI_ST   stCtrlPiIdcDiff = {0};
#pragma DATA_SECTION(stOutputCtrl,"CLADataLS1");
OUTPUT_CTRL_ST stOutputCtrl = {0};
#pragma DATA_SECTION(stDcPwmCmpr,"CLADataLS1");
CMPRDC_ST    stDcPwmCmpr = {0};


void sShareDataInit(void);
void sDcControllerReset(void);

void sShareDataInit(void)
{
    stSampleCal.u16_SumCnt = 0;
    stSampleCal.u16_SumCntSet = 720;   //
    stSampleCal.u16_DcSumStatus = 0;

    stSampleCalCla.u16_SumCntCla = 0;
    stSampleCalCla.u16_DcSumStatus = 0;
    stSampleCalCla.u16_SumCntSetCla = 720;

    stSampleCla.u16_MidCalCnt = 0;
    stSampleCla.u16_Buck_I1_Mid = 2048;
    stSampleCla.u16_Buck_I2_Mid = 2048;
    stSampleCla.u16_Output_I_Mid = 2048;
    stSampleCla.u32_Buck_I1_MidCalSumTemp = 0;
    stSampleCla.u32_Buck_I2_MidCalSumTemp = 0;
    stSampleCla.u32_Output_I_MidCalSumTemp = 0;
    stSampleCla.u16_MidCalFlag = 0;

    stAdcSample.u16_MidCalFlag = 0;
    stAdcSample.u16_MidCalCnt = 0;
    stAdcSample.u32_PfcI_R1_MidCalSumTemp = 0;
    stAdcSample.u32_PfcI_R2_MidCalSumTemp = 0;
    stAdcSample.u32_PfcI_S1_MidCalSumTemp = 0;
    stAdcSample.u32_PfcI_S2_MidCalSumTemp = 0;
    stAdcSample.u32_PfcI_T1_MidCalSumTemp = 0;
    stAdcSample.u32_PfcI_T2_MidCalSumTemp = 0;
    stAdcSample.u32_GridI_R_MidCalSumTemp = 0;
    stAdcSample.u32_GridI_S_MidCalSumTemp = 0;
    stAdcSample.u32_GridI_T_MidCalSumTemp = 0;
    stAdcSample.u32_Buck_I1_MidCalSumTemp = 0;
    stAdcSample.u32_Buck_I2_MidCalSumTemp = 0;
    stAdcSample.u32_Output_I_MidCalSumTemp = 0;

    //DC-DC current loop
    stCtrlPiIdc.f32_Ref = 0.0f;
    stCtrlPiIdc.f32_Fbk = 0.0f;
    stCtrlPiIdc.f32_Kp = 2.0f;
    stCtrlPiIdc.f32_Ki = 0.3f;
    stCtrlPiIdc.f32_Max = 700.0f;
    stCtrlPiIdc.f32_Min = -700.0f;
    stCtrlPiIdc.f32_Err = 0.0f;
    stCtrlPiIdc.f32_Err0 = 0.0f;
    stCtrlPiIdc.f32_Out = 0.0f;
    stCtrlPiIdc.f32_Ui = 0.0f;
    stCtrlPiIdc.f32_Uo = 0.0f;
    stCtrlPiIdc.f32_Up = 0.0f;

    //Differential current loop
    stCtrlPiIdcDiff.f32_Ref = 0.0f;
    stCtrlPiIdcDiff.f32_Fbk = 0.0f;
    stCtrlPiIdcDiff.f32_Kp = 2.0f;
    stCtrlPiIdcDiff.f32_Ki = 4.0f;
    stCtrlPiIdcDiff.f32_Max = 0.1f;
    stCtrlPiIdcDiff.f32_Min = -0.1f;
    stCtrlPiIdcDiff.f32_Err = 0.0f;
    stCtrlPiIdcDiff.f32_Err0 = 0.0f;
    stCtrlPiIdcDiff.f32_Out = 0.0f;
    stCtrlPiIdcDiff.f32_Ui = 0.0f;
    stCtrlPiIdcDiff.f32_Uo = 0.0f;
    stCtrlPiIdcDiff.f32_Up = 0.0f;

    //DC-DC voltage loop
   /* 0.02 s + 200
      ------------  small capacitor
            s        */
    stCtrlPiVout.f32_Ref = 0.0f;
    stCtrlPiVout.f32_Fbk = 0.0f;
    stCtrlPiVout.f32_Kp = 0.002f;
    stCtrlPiVout.f32_Ki = 0.02f;
    stCtrlPiVout.f32_Max = 210.0f;
    stCtrlPiVout.f32_Min = -210.0f;
    stCtrlPiVout.f32_Err = 0.0f;
    stCtrlPiVout.f32_Err0 = 0.0f;
    stCtrlPiVout.f32_Out = 0.0f;
    stCtrlPiVout.f32_Ui = 0.0f;
    stCtrlPiVout.f32_Uo = 0.0f;
    stCtrlPiVout.f32_Up = 0.0f;

    //DC-DC power loop
    stCtrlPiPout.f32_Ref = 0.0f;
    stCtrlPiPout.f32_Fbk = 0.0f;
    stCtrlPiPout.f32_Kp = 0.0002f;
    stCtrlPiPout.f32_Ki = 0.0002f;
    stCtrlPiPout.f32_Max = 210.0f;
    stCtrlPiPout.f32_Min = -210.0f;
    stCtrlPiPout.f32_Err = 0.0f;
    stCtrlPiPout.f32_Err0 = 0.0f;
    stCtrlPiPout.f32_Out = 0.0f;
    stCtrlPiPout.f32_Ui = 0.0f;
    stCtrlPiPout.f32_Uo = 0.0f;
    stCtrlPiPout.f32_Up = 0.0f;

    //Default output control setting
    stOutputCtrl.f32_VoutRefCmd = 600.0f;
    stOutputCtrl.f32_VoutRef = 0.0f;
    stOutputCtrl.f32_VoutStep = 10.0f;       //10V/ms

    stOutputCtrl.f32_IoutRefCmd = 210.0f;
    stOutputCtrl.f32_IoutRef = 0.0f;
    stOutputCtrl.f32_IoutStep = 5.0f/Fs;       //5A V/ms

    stOutputCtrl.f32_PoutRefCmd = 125000.0f;
    stOutputCtrl.f32_PoutRef = 0.0f;
    stOutputCtrl.f32_PoutStep = 1000.0f;       //1000W/ms

    stOutputCtrl.u16_OutputMode = 0;          //CC
    stOutputCtrl.u16_DcLoopCnt = 0;

    stDcPwmCmpr.u16_PeriodCnt = (uint16_t)((int32_t)CPU_CLK*1000L/Fs)>>2;
    stDcPwmCmpr.u16_PwmCmpr1 = 0;
    stDcPwmCmpr.u16_PwmCmpr2 = 0;
    stDcPwmCmpr.f32_DcDuty1 = 0.0f;
    stDcPwmCmpr.f32_DcDuty2 = 0.0f;
    stDcPwmCmpr.f32_DcDutyDiff = 0.0f;
}

void sDcControllerReset(void)
{
    stCtrlPiIdc.f32_Err = 0.0f;
    stCtrlPiIdc.f32_Err0 = 0.0f;
    stCtrlPiIdc.f32_Out = 0.0f;
    stCtrlPiIdc.f32_Ui = 0.0f;
    stCtrlPiIdc.f32_Uo = 0.0f;
    stCtrlPiIdc.f32_Up = 0.0f;

    stCtrlPiIdcDiff.f32_Err = 0.0f;
    stCtrlPiIdcDiff.f32_Err0 = 0.0f;
    stCtrlPiIdcDiff.f32_Out = 0.0f;
    stCtrlPiIdcDiff.f32_Ui = 0.0f;
    stCtrlPiIdcDiff.f32_Uo = 0.0f;
    stCtrlPiIdcDiff.f32_Up = 0.0f;

    stCtrlPiVout.f32_Err = 0.0f;
    stCtrlPiVout.f32_Err0 = 0.0f;
    stCtrlPiVout.f32_Out = 0.0f;
    stCtrlPiVout.f32_Ui = 0.0f;
    stCtrlPiVout.f32_Uo = 0.0f;
    stCtrlPiVout.f32_Up = 0.0f;

    stCtrlPiPout.f32_Err = 0.0f;
    stCtrlPiPout.f32_Err0 = 0.0f;
    stCtrlPiPout.f32_Out = 0.0f;
    stCtrlPiPout.f32_Ui = 0.0f;
    stCtrlPiPout.f32_Uo = 0.0f;
    stCtrlPiPout.f32_Up = 0.0f;
}

