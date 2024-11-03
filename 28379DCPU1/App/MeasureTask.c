/*******************************************************************************
 * File Name: MeasurementTask.c
 * Description: The measurement functions and calibration.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "App.h"
#include "F28x_Project.h"
#include "Adc_User.h"
#include "PWM_User.h"
#include "Gpio_User.h"
#include "RTOS_APP.h"
#include "RTOS_Define.h"
#include "MeasureTask.h"
#include "ShareData.h"
#include "math.h"
#include "stdio.h"


MEASURE_ST stMeasure;
void sMeasureTask(void);
void sAcMeasureCalculation(void);
void sDcMeasureCalculation(void);

uint16_t u16_test[5];
void sMeasureTask(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioMeasure);     //Get the current task event


    if(event & ((uint16_t)1 << eMeasureACCalEvt))
    {
        sAcMeasureCalculation();
        u16_test[0]++;
    }

    if(event & ((uint16_t)1 << eMeasureDCCalEvt))
    {
        sDcMeasureCalculation();
        u16_test[1]++;
    }

    if(event & ((uint16_t)1 << eMeasureTimerEvt))
    {
        u16_test[2]++;
    }
}

void sAcMeasureCalculation(void)
{
    float f32_Temp;

    if(stSampleCal.u16_SumCntSet != 0)
    {
        f32_Temp = 1.0f/(float)stSampleCal.u16_SumCntSet;
    }
    else
    {
        f32_Temp = 1.0f/720.0f;
    }
    stMeasure.u16_Pfc_I_R1_Rms = (uint16_t)(__sqrt(stSampleCal.f32_PFC_I_R1_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Pfc_I_R2_Rms = (uint16_t)(__sqrt(stSampleCal.f32_PFC_I_R2_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Pfc_I_S1_Rms = (uint16_t)(__sqrt(stSampleCal.f32_PFC_I_S1_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Pfc_I_S2_Rms = (uint16_t)(__sqrt(stSampleCal.f32_PFC_I_S2_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Pfc_I_T1_Rms = (uint16_t)(__sqrt(stSampleCal.f32_PFC_I_T1_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Pfc_I_T2_Rms = (uint16_t)(__sqrt(stSampleCal.f32_PFC_I_T2_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Grid_I_R_Rms = (uint16_t)(__sqrt(stSampleCal.f32_Grid_I_R_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Grid_I_S_Rms = (uint16_t)(__sqrt(stSampleCal.f32_Grid_I_S_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Grid_I_T_Rms = (uint16_t)(__sqrt(stSampleCal.f32_Grid_I_T_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Grid_V_RS_Rms = (uint16_t)(__sqrt(stSampleCal.f32_Grid_V_RS_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Grid_V_ST_Rms = (uint16_t)(__sqrt(stSampleCal.f32_Grid_V_ST_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Grid_V_TR_Rms = (uint16_t)(__sqrt(stSampleCal.f32_Grid_V_TR_Sum*f32_Temp)*10.0f);
    stMeasure.u16_Bus_V_Avg = (uint16_t)((stSampleCal.f32_BUS_V_Sum*f32_Temp)*10.0f);
    stMeasure.u16_NBus_V_Avg = (uint16_t)((stSampleCal.f32_NBUS_V_Sum*f32_Temp)*10.0f);
#if cMeasureTest == 1
    stMeasure.u16_Pfc_I_R1_Rms = 1101;
    stMeasure.u16_Pfc_I_R2_Rms = 1111;
    stMeasure.u16_Pfc_I_S1_Rms = 1099;
    stMeasure.u16_Pfc_I_S2_Rms = 1088;
    stMeasure.u16_Pfc_I_T1_Rms = 1112;
    stMeasure.u16_Pfc_I_T2_Rms = 1111;
    stMeasure.u16_Grid_I_R_Rms = 2212;
    stMeasure.u16_Grid_I_S_Rms = 2187;
    stMeasure.u16_Grid_I_T_Rms = 2223;
    stMeasure.u16_Grid_V_RS_Rms = 3810;
    stMeasure.u16_Grid_V_ST_Rms = 3825;
    stMeasure.u16_Grid_V_TR_Rms = 3798;
    stMeasure.u16_Bus_V_Avg = 7055;
    stMeasure.u16_NBus_V_Avg = 3785;
    stMeasure.u16_GridFreq = 5001;
    stMeasure.u16_PBus_V_Avg = stMeasure.u16_Bus_V_Avg - stMeasure.u16_NBus_V_Avg;
    stMeasure.u16_Input_P_Total = 1355;
#endif

}
void sDcMeasureCalculation(void)
{
    float f32_Temp;
    if(stSampleCal.u16_SumCntSet != 0)
    {
        f32_Temp = 1.0f/(float)stSampleCal.u16_SumCntSet;
    }
    else
    {
        f32_Temp = 1.0f/720.0f;
    }
    stMeasure.i16_Buck_I1_Avg = (int16_t)((stSampleCalCla.f32_Buck_I1_Sum*f32_Temp)*10.0f);
    stMeasure.i16_Buck_I2_Avg = (int16_t)(stSampleCalCla.f32_Buck_I2_Sum*f32_Temp)*10.0f;
    stMeasure.i16_Output_I_Avg = (int16_t)(stSampleCalCla.f32_Output_I_Sum*f32_Temp)*10.0f;
    stMeasure.u16_Output_V_Avg = (uint16_t)(stSampleCalCla.f32_Output_V_Sum*f32_Temp)*10.0f;

#if cMeasureTest == 1
    stMeasure.i16_Buck_I1_Avg = 1105;
    stMeasure.i16_Buck_I2_Avg = 1098;
    stMeasure.i16_Output_I_Avg = 2203;
    stMeasure.u16_Output_V_Avg = 6499;
    stMeasure.u16_Output_P = 1255;
    stMeasure.i16_Temperature[0] = 275;
    stMeasure.i16_Temperature[1] = 322;
    stMeasure.i16_Temperature[2] = 433;
    stMeasure.i16_Temperature[3] = 345;
#endif
}


