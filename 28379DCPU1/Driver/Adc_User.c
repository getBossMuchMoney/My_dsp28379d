/*******************************************************************************
 * File Name: Adc_User.c
 * Description: The ADC initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "F28x_Project.h"
#include "Adc_User.h"

#pragma DATA_SECTION(stAdcSample,"CLADataLS0");
ADC_SAMPLE stAdcSample;

void sAdcInit(void);
void sAdcSampleInit(void);
void sAdcCpuSample(void);
void sAdcChannelSetup(uint16_t u16_Adc,
                      uint16_t u16_ChSel[],
                      uint16_t u16_Acqps[],
                      uint16_t u16_Trigger[],
                      uint16_t u16_ContinousMode,
                      uint16_t u16_IntEn,
                      uint16_t u16_IntSel[]);


/*******************************************************************************
 * Function Name: sAdcInit
 * Description: Initialize the ADC A/B/C/D.
 *******************************************************************************/
void sAdcInit(void)
{
    uint16_t u16_ChSel[16];
    uint16_t u16_Acqps[16];
    uint16_t u16_Trigger[16];
    uint16_t u16_IntSel[4];

    EALLOW;
    //The maximum ADCCLK of the ADC is 50MHz, the minimum is 5MHz
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK = 200MHz/4 = 50MHz
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    //Set the ADC A/B/C/D working mode
    //The ADC in this DSP is external reference only.
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //The minimum sample time is 75ns, 15*5ns = 75ns
    u16_Acqps[0] = 29;
    u16_Acqps[1] = 29;
    u16_Acqps[2] = 29;
    u16_Acqps[3] = 29;
    u16_Acqps[4] = 29;
    u16_Acqps[5] = 29;
    u16_Acqps[6] = 29;
    u16_Acqps[7] = 29;
    u16_Acqps[8] = 29;
    u16_Acqps[9] = 29;
    u16_Acqps[10] = 29;
    u16_Acqps[11] = 29;
    u16_Acqps[12] = 29;
    u16_Acqps[13] = 29;
    u16_Acqps[14] = 29;
    u16_Acqps[15] = 29;

    //Interrupt trigger source
    u16_IntSel[0] = 7;
    u16_IntSel[1] = 0;
    u16_IntSel[2] = 0;
    u16_IntSel[3] = 4;


    //ADC A setting
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;           //The interrupt is generated at the end of conversion
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;              //Power up the analog power
    DELAY_US(1000);

    //ADC A Channel select SOCA
    u16_ChSel[0] = 2;              //ADCINA2, 1st R phase choke current
    u16_ChSel[1] = 4;              //ADCINA4, 2nd R phase choke current
    u16_ChSel[2] = 5;              //ADCINA5,  R phase grid current
    u16_ChSel[3] = 1;              //ADCINA1, RS grid voltage
    u16_ChSel[4] = 0;              //ADCINA0, S phase grid current
    u16_ChSel[5] = 3;              //ADCINA3, BUS voltage
    u16_ChSel[6] = 14;             //ADCIN14, 2nd S phase choke current
    u16_ChSel[7] = 15;             //ADCIN15, half bus voltage
    //SOCB
    u16_ChSel[8] = 2;              //ADCINA2, 1st R phase choke current
    u16_ChSel[9] = 4;              //ADCINA4, 2nd R phase choke current
    u16_ChSel[10] = 5;              //ADCINA5,  R phase grid current
    u16_ChSel[11] = 1;              //ADCINA1, RS grid voltage
    u16_ChSel[12] = 0;              //ADCINA0, S phase grid current
    u16_ChSel[13] = 3;              //ADCINA3, BUS voltage
    u16_ChSel[14] = 14;             //ADCIN14, 2nd S phase choke current
    u16_ChSel[15] = 15;             //ADCIN15, half bus voltage

    u16_Trigger[0] = 5;             //EPWM1SOCA
    u16_Trigger[1] = 5;             //EPWM1SOCA
    u16_Trigger[2] = 5;             //EPWM1SOCA
    u16_Trigger[3] = 5;             //EPWM1SOCA
    u16_Trigger[4] = 5;             //EPWM1SOCA
    u16_Trigger[5] = 5;             //EPWM1SOCA
    u16_Trigger[6] = 5;             //EPWM1SOCA
    u16_Trigger[7] = 5;             //EPWM1SOCA
    u16_Trigger[8] = 6;             //EPWM1SOCB
    u16_Trigger[9] = 6;             //EPWM1SOCB
    u16_Trigger[10] = 6;             //EPWM1SOCB
    u16_Trigger[11] = 6;             //EPWM1SOCB
    u16_Trigger[12] = 6;             //EPWM1SOCB
    u16_Trigger[13] = 6;             //EPWM1SOCB
    u16_Trigger[14] = 6;             //EPWM1SOCB
    u16_Trigger[15] = 6;             //EPWM1SOCB
    sAdcChannelSetup(ADC_ADCA,u16_ChSel,u16_Acqps,u16_Trigger,0,1,u16_IntSel);

    //ADCB setting
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;           //The interrupt is generated at the end of conversion
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;              //Power up the analog power
    DELAY_US(1000);
    //ADC B Channel select SOCA
    u16_ChSel[0] = 2;              //ADCINB2, 1st S phase choke current
    u16_ChSel[1] = 14;             //ADCIN14, 2nd S phase choke current
    u16_ChSel[2] = 1;              //ADCINB1, T phase grid current
    u16_ChSel[3] = 0;              //ADCINB0, TR grid voltage
    //SOCB
    u16_ChSel[4] = 2;              //ADCINB2, 1st S phase choke current
    u16_ChSel[5] = 14;             //ADCIN14, 2nd S phase choke current
    u16_ChSel[6] = 1;              //ADCINB1, T phase grid current
    u16_ChSel[7] = 0;              //ADCINB0, TR grid voltage

    u16_Trigger[0] = 5;             //EPWM1SOCA
    u16_Trigger[1] = 5;             //EPWM1SOCA
    u16_Trigger[2] = 5;             //EPWM1SOCA
    u16_Trigger[3] = 5;             //EPWM1SOCA
    u16_Trigger[4] = 6;             //EPWM1SOCB
    u16_Trigger[5] = 6;             //EPWM1SOCB
    u16_Trigger[6] = 6;             //EPWM1SOCB
    u16_Trigger[7] = 6;             //EPWM1SOCB
    u16_Trigger[8] = 0;             //SW trigger
    u16_Trigger[9] = 0;             //
    u16_Trigger[10] = 0;             //
    u16_Trigger[11] = 0;             //
    u16_Trigger[12] = 0;             //
    u16_Trigger[13] = 0;             //
    u16_Trigger[14] = 0;             //
    u16_Trigger[15] = 0;             //
    sAdcChannelSetup(ADC_ADCB,u16_ChSel,u16_Acqps,u16_Trigger,0,0,u16_IntSel);

    //ADC C setting
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;           //The interrupt is generated at the end of conversion
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;              //Power up the analog power
    DELAY_US(1000);
    //ADC C Channel Select SOCA
    u16_ChSel[0] = 4;              //ADCINC4, 1st T phase choke current
    u16_ChSel[1] = 2;              //ADCINC2, 2nd T phase choke current
    u16_ChSel[2] = 3;              //ADCINA1, ST grid voltage
    //SOCB
    u16_ChSel[3] = 4;              //ADCINC4, 1st T phase choke current
    u16_ChSel[4] = 2;              //ADCINC2, 2nd T phase choke current
    u16_ChSel[5] = 3;              //ADCINA1, ST grid voltage

    u16_Trigger[0] = 5;             //EPWM1SOCA
    u16_Trigger[1] = 5;             //EPWM1SOCA
    u16_Trigger[2] = 5;             //EPWM1SOCA
    u16_Trigger[3] = 6;             //EPWM1SOCB
    u16_Trigger[4] = 6;             //EPWM1SOCB
    u16_Trigger[5] = 6;             //EPWM1SOCB
    u16_Trigger[6] = 0;             //
    u16_Trigger[7] = 0;             //
    u16_Trigger[8] = 0;             //SW trigger
    u16_Trigger[9] = 0;             //
    u16_Trigger[10] = 0;             //
    u16_Trigger[11] = 0;             //
    u16_Trigger[12] = 0;             //
    u16_Trigger[13] = 0;             //
    u16_Trigger[14] = 0;             //
    u16_Trigger[15] = 0;             //

    sAdcChannelSetup(ADC_ADCC,u16_ChSel,u16_Acqps,u16_Trigger,0,0,u16_IntSel);

    //ADC D setting
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;           //The interrupt is generated at the end of conversion
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;              //Power up the analog power
    DELAY_US(1000);
    //ADC D channel select SOCA
    u16_ChSel[0] = 0;              //ADCIND0, 1st DC-DC choke current
    u16_ChSel[1] = 2;              //ADCIND2, 2nd DC-DC choke current
    u16_ChSel[2] = 1;              //ADCIND1, Total output
    u16_ChSel[3] = 3;              //ADCIND3, Output Voltage
    u16_ChSel[4] = 4;              //ADCIND4, Temperature
    //SOCB
    u16_ChSel[5] = 0;              //ADCIND0, 1st DC-DC choke current
    u16_ChSel[6] = 2;              //ADCIND2, 2nd DC-DC choke current
    u16_ChSel[7] = 1;              //ADCIND1, Total output
    u16_ChSel[8] = 3;              //ADCIND3, Output Voltage
    u16_ChSel[9] = 4;              //ADCIND4, Temperature

    u16_Trigger[0] = 0x11;             //EPWM7SOCA
    u16_Trigger[1] = 0x11;             //EPWM7SOCA
    u16_Trigger[2] = 0x11;             //EPWM7SOCA
    u16_Trigger[3] = 0x11;             //EPWM7SOCA
    u16_Trigger[4] = 0x11;             //EPWM7SOCA
    u16_Trigger[5] = 0x12;             //EPWM7SOCB
    u16_Trigger[6] = 0x12;             //EPWM7SOCB
    u16_Trigger[7] = 0x12;             //EPWM7SOCB
    u16_Trigger[8] = 0x12;             //EPWM7SOCB
    u16_Trigger[9] = 0x12;             //EPWM7SOCB
    u16_Trigger[10] = 0;             //
    u16_Trigger[11] = 0;             //
    u16_Trigger[12] = 0;             //
    u16_Trigger[13] = 0;             //
    u16_Trigger[14] = 0;             //
    u16_Trigger[15] = 0;             //
    sAdcChannelSetup(ADC_ADCD,u16_ChSel,u16_Acqps,u16_Trigger,0,1,u16_IntSel);

    EDIS;

    sAdcSampleInit();
}

void sAdcChannelSetup(uint16_t u16_Adc,
                      uint16_t u16_ChSel[],
                      uint16_t u16_Acqps[],
                      uint16_t u16_Trigger[],
                      uint16_t u16_ContinousMode,
                      uint16_t u16_IntEn,
                      uint16_t u16_IntSel[])
{
    volatile struct ADC_REGS *AdcRegsSel[4] = {&AdcaRegs,&AdcbRegs,&AdccRegs,&AdcdRegs};

    //EALLOW;

    //SOC channel select
    (*AdcRegsSel[u16_Adc]).ADCSOC0CTL.bit.CHSEL = u16_ChSel[0];
    (*AdcRegsSel[u16_Adc]).ADCSOC1CTL.bit.CHSEL = u16_ChSel[1];
    (*AdcRegsSel[u16_Adc]).ADCSOC2CTL.bit.CHSEL = u16_ChSel[2];
    (*AdcRegsSel[u16_Adc]).ADCSOC3CTL.bit.CHSEL = u16_ChSel[3];
    (*AdcRegsSel[u16_Adc]).ADCSOC4CTL.bit.CHSEL = u16_ChSel[4];
    (*AdcRegsSel[u16_Adc]).ADCSOC5CTL.bit.CHSEL = u16_ChSel[5];
    (*AdcRegsSel[u16_Adc]).ADCSOC6CTL.bit.CHSEL = u16_ChSel[6];
    (*AdcRegsSel[u16_Adc]).ADCSOC7CTL.bit.CHSEL = u16_ChSel[7];
    (*AdcRegsSel[u16_Adc]).ADCSOC8CTL.bit.CHSEL = u16_ChSel[8];
    (*AdcRegsSel[u16_Adc]).ADCSOC9CTL.bit.CHSEL = u16_ChSel[9];
    (*AdcRegsSel[u16_Adc]).ADCSOC10CTL.bit.CHSEL = u16_ChSel[10];
    (*AdcRegsSel[u16_Adc]).ADCSOC11CTL.bit.CHSEL = u16_ChSel[11];
    (*AdcRegsSel[u16_Adc]).ADCSOC12CTL.bit.CHSEL = u16_ChSel[12];
    (*AdcRegsSel[u16_Adc]).ADCSOC13CTL.bit.CHSEL = u16_ChSel[13];
    (*AdcRegsSel[u16_Adc]).ADCSOC14CTL.bit.CHSEL = u16_ChSel[14];
    (*AdcRegsSel[u16_Adc]).ADCSOC15CTL.bit.CHSEL = u16_ChSel[15];

    //SOC acquisition select
    (*AdcRegsSel[u16_Adc]).ADCSOC0CTL.bit.ACQPS = u16_Acqps[0];
    (*AdcRegsSel[u16_Adc]).ADCSOC1CTL.bit.ACQPS = u16_Acqps[1];
    (*AdcRegsSel[u16_Adc]).ADCSOC2CTL.bit.ACQPS = u16_Acqps[2];
    (*AdcRegsSel[u16_Adc]).ADCSOC3CTL.bit.ACQPS = u16_Acqps[3];
    (*AdcRegsSel[u16_Adc]).ADCSOC4CTL.bit.ACQPS = u16_Acqps[4];
    (*AdcRegsSel[u16_Adc]).ADCSOC5CTL.bit.ACQPS = u16_Acqps[5];
    (*AdcRegsSel[u16_Adc]).ADCSOC6CTL.bit.ACQPS = u16_Acqps[6];
    (*AdcRegsSel[u16_Adc]).ADCSOC7CTL.bit.ACQPS = u16_Acqps[7];
    (*AdcRegsSel[u16_Adc]).ADCSOC8CTL.bit.ACQPS = u16_Acqps[8];
    (*AdcRegsSel[u16_Adc]).ADCSOC9CTL.bit.ACQPS = u16_Acqps[9];
    (*AdcRegsSel[u16_Adc]).ADCSOC10CTL.bit.ACQPS = u16_Acqps[10];
    (*AdcRegsSel[u16_Adc]).ADCSOC11CTL.bit.ACQPS = u16_Acqps[11];
    (*AdcRegsSel[u16_Adc]).ADCSOC12CTL.bit.ACQPS = u16_Acqps[12];
    (*AdcRegsSel[u16_Adc]).ADCSOC13CTL.bit.ACQPS = u16_Acqps[13];
    (*AdcRegsSel[u16_Adc]).ADCSOC14CTL.bit.ACQPS = u16_Acqps[14];
    (*AdcRegsSel[u16_Adc]).ADCSOC15CTL.bit.ACQPS = u16_Acqps[15];

    //SOC trigger select
    (*AdcRegsSel[u16_Adc]).ADCSOC0CTL.bit.TRIGSEL = u16_Trigger[0];
    (*AdcRegsSel[u16_Adc]).ADCSOC1CTL.bit.TRIGSEL = u16_Trigger[1];
    (*AdcRegsSel[u16_Adc]).ADCSOC2CTL.bit.TRIGSEL = u16_Trigger[2];
    (*AdcRegsSel[u16_Adc]).ADCSOC3CTL.bit.TRIGSEL = u16_Trigger[3];
    (*AdcRegsSel[u16_Adc]).ADCSOC4CTL.bit.TRIGSEL = u16_Trigger[4];
    (*AdcRegsSel[u16_Adc]).ADCSOC5CTL.bit.TRIGSEL = u16_Trigger[5];
    (*AdcRegsSel[u16_Adc]).ADCSOC6CTL.bit.TRIGSEL = u16_Trigger[6];
    (*AdcRegsSel[u16_Adc]).ADCSOC7CTL.bit.TRIGSEL = u16_Trigger[7];
    (*AdcRegsSel[u16_Adc]).ADCSOC8CTL.bit.TRIGSEL = u16_Trigger[8];
    (*AdcRegsSel[u16_Adc]).ADCSOC9CTL.bit.TRIGSEL = u16_Trigger[9];
    (*AdcRegsSel[u16_Adc]).ADCSOC10CTL.bit.TRIGSEL = u16_Trigger[10];
    (*AdcRegsSel[u16_Adc]).ADCSOC11CTL.bit.TRIGSEL = u16_Trigger[11];
    (*AdcRegsSel[u16_Adc]).ADCSOC12CTL.bit.TRIGSEL = u16_Trigger[12];
    (*AdcRegsSel[u16_Adc]).ADCSOC13CTL.bit.TRIGSEL = u16_Trigger[13];
    (*AdcRegsSel[u16_Adc]).ADCSOC14CTL.bit.TRIGSEL = u16_Trigger[14];
    (*AdcRegsSel[u16_Adc]).ADCSOC15CTL.bit.TRIGSEL = u16_Trigger[15];

    //Continuous mode setting
    if((u16_ContinousMode & 0x1) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1CONT = 1;
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1CONT = 0;
    }
    if((u16_ContinousMode & 0x2) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT2CONT = 1;
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT2CONT = 0;
    }
    if((u16_ContinousMode & 0x4) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT3CONT = 1;
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT3CONT = 0;
    }
    if((u16_ContinousMode & 0x8) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT4CONT = 1;
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT4CONT = 0;
    }

    //Interrupt Select
    if((u16_IntEn & 0x1) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1E = 1;
        //(*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1SEL = u16_IntSel[0];
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1E = 0;
    }

    if((u16_IntEn & 0x2) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT2E = 1;
        //(*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT2SEL = u16_IntSel[1];
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT2E = 0;
    }

    if((u16_IntEn & 0x4) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT3E = 1;
        //(*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT3SEL = u16_IntSel[2];
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT3E = 0;
    }

    if((u16_IntEn & 0x8) != 0)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT4E = 1;
        //(*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT4SEL = u16_IntSel[3];
    }
    else
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL3N4.bit.INT4E = 0;
    }

    if(u16_Adc == ADC_ADCA)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1SEL = u16_IntSel[0];
    }
    else if(u16_Adc == ADC_ADCB)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1SEL = u16_IntSel[1];
    }
    else if(u16_Adc == ADC_ADCC)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1SEL = u16_IntSel[2];
    }
    else if(u16_Adc == ADC_ADCD)
    {
        (*AdcRegsSel[u16_Adc]).ADCINTSEL1N2.bit.INT1SEL = u16_IntSel[3];
    }

    (*AdcRegsSel[u16_Adc]).ADCINTFLGCLR.all = 0x000F;       //Clear all the interrupt flags

    //EDIS;
}

void sAdcSampleInit(void)
{
    stAdcSample.u16_Buck_I1_Mid = 2048;
    stAdcSample.u16_Buck_I2_Mid = 2048;
    stAdcSample.u16_Output_I_Mid = 2048;

    stAdcSample.u16_Grid_I_R_Mid = 2048;
    stAdcSample.u16_Grid_I_S_Mid = 2048;
    stAdcSample.u16_Grid_I_T_Mid = 2048;
    stAdcSample.u16_Pfc_I_R1_Mid = 2048;
    stAdcSample.u16_Pfc_I_R2_Mid = 2048;
    stAdcSample.u16_Pfc_I_S1_Mid = 2048;
    stAdcSample.u16_Pfc_I_S2_Mid = 2048;
    stAdcSample.u16_Pfc_I_T1_Mid = 2048;
    stAdcSample.u16_Pfc_I_T2_Mid = 2048;

    stAdcSample.u16_Grid_V_RS_Mid = 2000;
    stAdcSample.u16_Grid_V_ST_Mid = 2048;
    stAdcSample.u16_Grid_V_TR_Mid = 2048;

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


}
void sAdcCpuSample(void)
{
    if(stAdcSample.u16_MidCalFlag == 0)
    {
        stAdcSample.u32_PfcI_R1_MidCalSumTemp += mAdc_PfcI_R1_0();
        stAdcSample.u32_PfcI_R2_MidCalSumTemp += mAdc_PfcI_R2_0();
        stAdcSample.u32_PfcI_S1_MidCalSumTemp += mAdc_PfcI_S1_0();
        stAdcSample.u32_PfcI_S2_MidCalSumTemp += mAdc_PfcI_S2_0();
        stAdcSample.u32_PfcI_T1_MidCalSumTemp += mAdc_PfcI_T1_0();
        stAdcSample.u32_PfcI_T2_MidCalSumTemp += mAdc_PfcI_T2_0();
        stAdcSample.u32_GridI_R_MidCalSumTemp += mAdc_GridI_R_0();
        stAdcSample.u32_GridI_S_MidCalSumTemp += mAdc_GridI_S_0();
        stAdcSample.u32_GridI_T_MidCalSumTemp += mAdc_GridI_T_0();
        stAdcSample.u32_Buck_I1_MidCalSumTemp += mAdc_Buck_I1_0();
        stAdcSample.u32_Buck_I2_MidCalSumTemp += mAdc_Buck_I2_0();
        stAdcSample.u32_Output_I_MidCalSumTemp += mAdc_Output_I_0();

        stAdcSample.u16_MidCalCnt++;
        if(stAdcSample.u16_MidCalCnt >= cAdcMidCalCntSet)
        {
            stAdcSample.u16_Pfc_I_R1_Mid = (uint16_t)(stAdcSample.u32_PfcI_R1_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Pfc_I_R2_Mid = (uint16_t)(stAdcSample.u32_PfcI_R2_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Pfc_I_S1_Mid = (uint16_t)(stAdcSample.u32_PfcI_S1_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Pfc_I_S2_Mid = (uint16_t)(stAdcSample.u32_PfcI_S2_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Pfc_I_T1_Mid = (uint16_t)(stAdcSample.u32_PfcI_T1_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Pfc_I_T2_Mid = (uint16_t)(stAdcSample.u32_PfcI_T2_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Grid_I_R_Mid = (uint16_t)(stAdcSample.u32_GridI_R_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Grid_I_S_Mid = (uint16_t)(stAdcSample.u32_GridI_S_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Grid_I_T_Mid = (uint16_t)(stAdcSample.u32_GridI_T_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Buck_I1_Mid = (uint16_t)(stAdcSample.u32_Buck_I1_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Buck_I2_Mid = (uint16_t)(stAdcSample.u32_Buck_I2_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_Output_I_Mid = (uint16_t)(stAdcSample.u32_Output_I_MidCalSumTemp>>cAdcMidCalN);
            stAdcSample.u16_MidCalCnt = 0;
            stAdcSample.u16_MidCalFlag = 1;
        }
    }
    else
    {
        stAdcSample.i16_Pfc_I_R1 = (int16_t)(((mAdc_PfcI_R1_0() + mAdc_PfcI_R1_1())>>1) - stAdcSample.u16_Pfc_I_R1_Mid);
        stAdcSample.i16_Pfc_I_R2 = (int16_t)(((mAdc_PfcI_R2_0() + mAdc_PfcI_R2_1())>>1) - stAdcSample.u16_Pfc_I_R2_Mid);
        stAdcSample.i16_Pfc_I_S1 = (int16_t)(((mAdc_PfcI_S1_0() + mAdc_PfcI_S1_1())>>1) - stAdcSample.u16_Pfc_I_S1_Mid);
        stAdcSample.i16_Pfc_I_S2 = (int16_t)(((mAdc_PfcI_S2_0() + mAdc_PfcI_S2_1())>>1) - stAdcSample.u16_Pfc_I_S2_Mid);
        stAdcSample.i16_Pfc_I_T1 = (int16_t)(((mAdc_PfcI_T1_0() + mAdc_PfcI_T1_1())>>1) - stAdcSample.u16_Pfc_I_T1_Mid);
        stAdcSample.i16_Pfc_I_T2 = (int16_t)(((mAdc_PfcI_T2_0() + mAdc_PfcI_T2_1())>>1) - stAdcSample.u16_Pfc_I_T2_Mid);
        //stAdcSample.i16_Grid_I_R = (int16_t)(((mAdc_GridI_R_0() + mAdc_GridI_R_1())>>1) - stAdcSample.u16_Grid_I_R_Mid);
        stAdcSample.i16_Grid_I_T = (int16_t)(((mAdc_GridI_R_0() + mAdc_GridI_R_1())>>1) - stAdcSample.u16_Grid_I_R_Mid);
        stAdcSample.i16_Grid_I_S = (int16_t)(((mAdc_GridI_S_0() + mAdc_GridI_S_1())>>1) - stAdcSample.u16_Grid_I_S_Mid);
        //stAdcSample.i16_Grid_I_T = (int16_t)(((mAdc_GridI_T_0() + mAdc_GridI_T_1())>>1) - stAdcSample.u16_Grid_I_T_Mid);
        stAdcSample.i16_Grid_I_R = (int16_t)(((mAdc_GridI_T_0() + mAdc_GridI_T_1())>>1) - stAdcSample.u16_Grid_I_T_Mid);
        stAdcSample.i16_Grid_V_RS = (int16_t)(((mAdc_GridV_RS_0() + mAdc_GridV_RS_1())>>1) - stAdcSample.u16_Grid_V_RS_Mid);
        stAdcSample.i16_Grid_V_ST = (int16_t)(((mAdc_GridV_ST_0() + mAdc_GridV_ST_1())>>1) - stAdcSample.u16_Grid_V_ST_Mid);
        stAdcSample.i16_Grid_V_TR = (int16_t)(((mAdc_GridV_TR_0() + mAdc_GridV_TR_1())>>1) - stAdcSample.u16_Grid_V_TR_Mid);
        stAdcSample.i16_Bus_V = (int16_t)(mAdc_Bus_V());
        stAdcSample.i16_NBus_V = (int16_t)(mAdc_NBus_V());
    }
}




