/*******************************************************************************
 * File Name: MeasurementTask.h
 * Description: The measurement functions and calibration.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _MASUREMENTTASK_H
#define _MASUREMENTTASK_H



//Measurment task definition
#define    cPrioMeasure                     5
#define    eMeasureTimerEvt                 0
#define    eMeasureACCalEvt                 1
#define    eMeasureDCCalEvt                 2

#define    cMeasureTest                     0

typedef struct {
    uint16_t u16_Grid_V_RS_Rms;
    uint16_t u16_Grid_V_ST_Rms;
    uint16_t u16_Grid_V_TR_Rms;
    uint16_t u16_GridFreq;
    uint16_t u16_Grid_I_R_Rms;
    uint16_t u16_Grid_I_S_Rms;
    uint16_t u16_Grid_I_T_Rms;
    uint16_t u16_Pfc_I_R1_Rms;
    uint16_t u16_Pfc_I_R2_Rms;
    uint16_t u16_Pfc_I_S1_Rms;
    uint16_t u16_Pfc_I_S2_Rms;
    uint16_t u16_Pfc_I_T1_Rms;
    uint16_t u16_Pfc_I_T2_Rms;
    uint16_t u16_Bus_V_Avg;
    uint16_t u16_PBus_V_Avg;
    uint16_t u16_NBus_V_Avg;
    int16_t  i16_Buck_I1_Avg;
    int16_t  i16_Buck_I2_Avg;
    int16_t  i16_Output_I_Avg;
    uint16_t u16_Output_V_Avg;
    uint16_t u16_Input_P_Total;
    uint16_t u16_Input_P_Active;
    uint16_t u16_Output_P;
    int16_t  i16_Temperature[8];
}MEASURE_ST;

extern void sMeasureTask(void);
extern MEASURE_ST stMeasure;

#endif















