/*
 * ShareData.h
 *
 *  Created on: 2024Äê10ÔÂ2ÈÕ
 *      Author: Miller
 */

#ifndef MODULE_SHAREDATA_H_
#define MODULE_SHAREDATA_H_

#include "PfcController.h"
typedef struct {

    float f32_PFC_I_R1;
    float f32_PFC_I_R2;
    float f32_PFC_I_S1;
    float f32_PFC_I_S2;
    float f32_PFC_I_T1;
    float f32_PFC_I_T2;
    float f32_Grid_I_R;
    float f32_Grid_I_S;
    float f32_Grid_I_T;
    float f32_Grid_V_RS;
    float f32_Grid_V_ST;
    float f32_Grid_V_TR;
    float f32_BUS_V;
    float f32_NBUS_V;

    float f32_PFC_I_R1_SumTemp;
    float f32_PFC_I_R2_SumTemp;
    float f32_PFC_I_S1_SumTemp;
    float f32_PFC_I_S2_SumTemp;
    float f32_PFC_I_T1_SumTemp;
    float f32_PFC_I_T2_SumTemp;
    float f32_Grid_I_R_SumTemp;
    float f32_Grid_I_S_SumTemp;
    float f32_Grid_I_T_SumTemp;
    float f32_Grid_V_RS_SumTemp;
    float f32_Grid_V_ST_SumTemp;
    float f32_Grid_V_TR_SumTemp;
    float f32_BUS_V_SumTemp;
    float f32_NBUS_V_SumTemp;

    float f32_PFC_I_R1_Sum;
    float f32_PFC_I_R2_Sum;
    float f32_PFC_I_S1_Sum;
    float f32_PFC_I_S2_Sum;
    float f32_PFC_I_T1_Sum;
    float f32_PFC_I_T2_Sum;
    float f32_Grid_I_R_Sum;
    float f32_Grid_I_S_Sum;
    float f32_Grid_I_T_Sum;
    float f32_Grid_V_RS_Sum;
    float f32_Grid_V_ST_Sum;
    float f32_Grid_V_TR_Sum;
    float f32_BUS_V_Sum;
    float f32_NBUS_V_Sum;

    float f32_Buck_I1_Cla;
    float f32_Buck_I2_Cla;
    float f32_Output_I_Cla;
    float f32_Output_V_Cla;

    float f32_Buck_I1_SumTemp_Cla;
    float f32_Buck_I2_SumTemp_Cla;
    float f32_Output_I_SumTemp_Cla;
    float f32_Output_V_SumTemp_Cla;
    float f32_Buck_I1_Sum_Cla;
    float f32_Buck_I2_Sum_Cla;
    float f32_Output_I_Sum_Cla;
    float f32_Output_V_Sum_Cla;

    uint16_t u16_SumCnt;
    uint16_t u16_SumCntSet;
    uint16_t u16_DcSumStatus;
    uint16_t u16_SumCntCla;

}SAMPLE_CAL;

typedef struct {
    int16_t i16_Buck_I1;
    int16_t i16_Buck_I2;
    int16_t i16_Output_I;
    int16_t i16_Output_V;

    uint16_t u16_Temperature_1;
    uint16_t u16_Temperature_2;
    uint16_t u16_Temperature_3;
    uint16_t u16_Temperature_4;
    uint16_t u16_Temperature_5;
    uint16_t u16_Temperature_6;

    uint16_t u16_Buck_I1_Mid;
    uint16_t u16_Buck_I2_Mid;
    uint16_t u16_Output_I_Mid;
    uint32_t u32_Buck_I1_MidCalSumTemp;
    uint32_t u32_Buck_I2_MidCalSumTemp;
    uint32_t u32_Output_I_MidCalSumTemp;

    uint16_t u16_MidCalFlag;
    uint16_t u16_MidCalCnt;
}ADC_SAMPLE_CLA;

typedef struct {
    float f32_Buck_I1;
    float f32_Buck_I2;
    float f32_Output_I;
    float f32_Output_V;

    float f32_Buck_I1_SumTemp;
    float f32_Buck_I2_SumTemp;
    float f32_Output_I_SumTemp;
    float f32_Output_V_SumTemp;
    float f32_Buck_I1_Sum;
    float f32_Buck_I2_Sum;
    float f32_Output_I_Sum;
    float f32_Output_V_Sum;

    uint16_t u16_SumCntCla;
    uint16_t u16_SumCntSetCla;
    uint16_t u16_DcSumStatus;

}SAMPLE_CAL_CLA;

typedef struct {
    uint16_t u16_PwmCmpr1;
    uint16_t u16_PwmCmpr2;
    uint16_t u16_PeriodCnt;
    float f32_DcDuty1;
    float f32_DcDuty2;
    float f32_DcDutyDiff;
}CMPRDC_ST;

typedef struct {
    float f32_VoutRefCmd;
    float f32_VoutRef;
    float f32_VoutStep;

    float f32_IoutRefCmd;
    float f32_IoutRef;
    float f32_IoutStep;

    float f32_PoutRefCmd;
    float f32_PoutRef;
    float f32_PoutStep;
    uint16_t u16_OutputMode;
    uint16_t u16_DcLoopCnt;
}OUTPUT_CTRL_ST;

typedef struct {
    float f32_PfcOpenloopDuty;
    float f32_DcOpenloopDuty;
}SYS_SETTING;

extern SAMPLE_CAL stSampleCal;
extern SAMPLE_CAL_CLA stSampleCalCla;
extern ADC_SAMPLE_CLA stSampleCla;
extern CTRL_PI_ST   stCtrlPiVout;
extern CTRL_PI_ST   stCtrlPiIdc;
extern CTRL_PI_ST   stCtrlPiPout;
extern CTRL_PI_ST   stCtrlPiIdcDiff;
extern OUTPUT_CTRL_ST stOutputCtrl;
extern CMPRDC_ST    stDcPwmCmpr;

extern void sShareDataInit(void);
extern void sDcControllerReset(void);


#endif /* MODULE_SHAREDATA_H_ */
