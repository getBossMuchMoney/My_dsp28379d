/*******************************************************************************
 * File Name: Adc_User.c
 * Description: The ADC initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _ADC_USER_H
#define _ADC_USER_H

typedef struct {
    int16_t i16_Pfc_I_R1;
    int16_t i16_Pfc_I_R2;
    int16_t i16_Pfc_I_S1;
    int16_t i16_Pfc_I_S2;
    int16_t i16_Pfc_I_T1;
    int16_t i16_Pfc_I_T2;

    int16_t i16_Grid_I_R;
    int16_t i16_Grid_I_S;
    int16_t i16_Grid_I_T;

    int16_t i16_Grid_V_RS;
    int16_t i16_Grid_V_ST;
    int16_t i16_Grid_V_TR;
    int16_t i16_Bus_V;
    int16_t i16_NBus_V;

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

    uint16_t u16_Pfc_I_R1_Mid;
    uint16_t u16_Pfc_I_S1_Mid;
    uint16_t u16_Pfc_I_T1_Mid;
    uint16_t u16_Pfc_I_R2_Mid;
    uint16_t u16_Pfc_I_S2_Mid;
    uint16_t u16_Pfc_I_T2_Mid;
    uint16_t u16_Grid_I_R_Mid;
    uint16_t u16_Grid_I_S_Mid;
    uint16_t u16_Grid_I_T_Mid;
    uint16_t u16_Grid_V_RS_Mid;
    uint16_t u16_Grid_V_ST_Mid;
    uint16_t u16_Grid_V_TR_Mid;
    uint16_t u16_Buck_I1_Mid;
    uint16_t u16_Buck_I2_Mid;
    uint16_t u16_Output_I_Mid;

    uint32_t u32_PfcI_R1_MidCalSumTemp;
    uint32_t u32_PfcI_S1_MidCalSumTemp;
    uint32_t u32_PfcI_T1_MidCalSumTemp;
    uint32_t u32_PfcI_R2_MidCalSumTemp;
    uint32_t u32_PfcI_S2_MidCalSumTemp;
    uint32_t u32_PfcI_T2_MidCalSumTemp;
    uint32_t u32_GridI_R_MidCalSumTemp;
    uint32_t u32_GridI_S_MidCalSumTemp;
    uint32_t u32_GridI_T_MidCalSumTemp;
    uint32_t u32_Buck_I1_MidCalSumTemp;
    uint32_t u32_Buck_I2_MidCalSumTemp;
    uint32_t u32_Output_I_MidCalSumTemp;

    uint16_t u16_MidCalFlag;
    uint16_t u16_MidCalCnt;
}ADC_SAMPLE;

#define   mAdc_PfcI_R1_0()         AdcaResultRegs.ADCRESULT0
#define   mAdc_PfcI_R1_1()         AdcaResultRegs.ADCRESULT8
#define   mAdc_PfcI_R2_0()         AdcaResultRegs.ADCRESULT1
#define   mAdc_PfcI_R2_1()         AdcaResultRegs.ADCRESULT9

#define   mAdc_PfcI_S1_0()         AdcbResultRegs.ADCRESULT0
#define   mAdc_PfcI_S1_1()         AdcbResultRegs.ADCRESULT4
#define   mAdc_PfcI_S2_0()         AdcbResultRegs.ADCRESULT1
#define   mAdc_PfcI_S2_1()         AdcbResultRegs.ADCRESULT5

#define   mAdc_PfcI_T1_0()         AdccResultRegs.ADCRESULT0
#define   mAdc_PfcI_T1_1()         AdccResultRegs.ADCRESULT3
#define   mAdc_PfcI_T2_0()         AdccResultRegs.ADCRESULT1
#define   mAdc_PfcI_T2_1()         AdccResultRegs.ADCRESULT4

#define   mAdc_GridI_R_0()         AdcaResultRegs.ADCRESULT2
#define   mAdc_GridI_R_1()         AdcaResultRegs.ADCRESULT10
#define   mAdc_GridI_S_0()         AdcaResultRegs.ADCRESULT4
#define   mAdc_GridI_S_1()         AdcaResultRegs.ADCRESULT12
#define   mAdc_GridI_T_0()         AdcbResultRegs.ADCRESULT2
#define   mAdc_GridI_T_1()         AdcbResultRegs.ADCRESULT6

#define   mAdc_GridV_RS_0()         AdcaResultRegs.ADCRESULT3
#define   mAdc_GridV_RS_1()         AdcaResultRegs.ADCRESULT11
#define   mAdc_GridV_ST_0()         AdccResultRegs.ADCRESULT2
#define   mAdc_GridV_ST_1()         AdccResultRegs.ADCRESULT5
#define   mAdc_GridV_TR_0()         AdcbResultRegs.ADCRESULT3
#define   mAdc_GridV_TR_1()         AdcbResultRegs.ADCRESULT7

#define   mAdc_Buck_I1_0()          AdcdResultRegs.ADCRESULT0
#define   mAdc_Buck_I1_1()          AdcdResultRegs.ADCRESULT5
#define   mAdc_Buck_I2_0()          AdcdResultRegs.ADCRESULT1
#define   mAdc_Buck_I2_1()          AdcdResultRegs.ADCRESULT6
#define   mAdc_Output_I_0()         AdcdResultRegs.ADCRESULT2
#define   mAdc_Output_I_1()         AdcdResultRegs.ADCRESULT7
#define   mAdc_Output_V_0()         AdcdResultRegs.ADCRESULT3
#define   mAdc_Output_V_1()         AdcdResultRegs.ADCRESULT8

#define   mAdc_Bus_V()              AdcaResultRegs.ADCRESULT5
#define   mAdc_NBus_V()             AdcaResultRegs.ADCRESULT7

#define   mAdc_Temperature()        AdcdResultRegs.ADCRESULT5

#define   cAdcMidCalN               15
#define   cAdcMidCalCntSet          ((uint16_t)1<<cAdcMidCalN)



extern void sAdcInit(void);
extern ADC_SAMPLE stAdcSample;
extern void sAdcCpuSample(void);

#endif

