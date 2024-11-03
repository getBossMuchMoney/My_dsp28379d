/*******************************************************************************
 * File Name: PWM_User.c
 * Description: The ePWM initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "F28x_Project.h"
#include "PWM_User.h"


void sPWMInit(void);
void sPwmUserInit(uint16_t n, uint16_t Period, uint16_t coutdir, uint16_t phase,uint16_t deadtime);
void sPwmXbarInit(void);
void sPwmTzDc_CBCInit(uint16_t n,uint16_t TripSrc,uint16_t ActA,uint16_t ActB);
void sFanPwmInit(void);
//void sPwmUserDCCMPSSInit(uint16_t n);

/*******************************************************************************
 * Function Name: sPWMInit
 * Description: Initialize the ePWM 1~8.
 *              EPWM1: 1ST AC-DC R; EPWM4: 2ND AC-DC R
 *              EPWM2: 1ST AC-DC S; EPWM5: 2ND AC-DC S
 *              EPWM3: 1ST AC-DC T; EPWM6: 2ND AC-DC T
 *              EPWM7: 1ST DC-DC ;  EPWM8: 2ND DC-DC
 *              All the PWM carrier frequency is 36kHz
 *******************************************************************************/
void sPWMInit(void)
{
    EALLOW;
    //PWM pins for Interleaved AC-DC PWM rectifier
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)
    //PWM pins for Interleaved DC-DC synchronous buck
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)
    //IP RELAY-GPIO22
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;    // Disable pull-up on GPIO15 (EPWM12A)
    //FAN SPEED CONTRL - GPIO23
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;    // Disable pull-up on GPIO15 (EPWM12B)

    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B

#if cPWM_DAC == 0
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as EPWM7A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as EPWM7B
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as EPWM8B
#else
    GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
    GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B
    GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
    GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B
#endif




    //Stop the TBCLK
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    //Set the EPWMSYNCIN signal for EPWM1, GPIO70 is the SYN-RX
    GpioCtrlRegs.GPCPUD.bit.GPIO70 = 0;    // Enable pull-up on GPIO70
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 0;    // Input
    InputXbarRegs.INPUT5SELECT = 70;
    SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN = 0;   //EPWM4,5,6 use the EPWM1 synout
    SyncSocRegs.SYNCSELECT.bit.EPWM7SYNCIN = 0;   //EPWM7,8,9 use the EPWM1 synout

    sPwmUserInit(cEPWM1,Fs_PFC,TB_COUNT_UPDOWN,0,cDeadtime1);
    sPwmUserInit(cEPWM2,Fs_PFC,TB_COUNT_UPDOWN,0,cDeadtime1);
    sPwmUserInit(cEPWM3,Fs_PFC,TB_COUNT_UPDOWN,0,cDeadtime1);
    sPwmUserInit(cEPWM4,Fs_PFC,TB_COUNT_UPDOWN,0,cDeadtime1);
    sPwmUserInit(cEPWM5,Fs_PFC,TB_COUNT_UPDOWN,0,cDeadtime1);
    sPwmUserInit(cEPWM6,Fs_PFC,TB_COUNT_UPDOWN,0,cDeadtime1);

    sPwmUserInit(cEPWM7,Fs_Buck,TB_COUNT_UPDOWN,0,cDeadtime2);
    sPwmUserInit(cEPWM8,Fs_Buck,TB_COUNT_UPDOWN,cILPhase,cDeadtime2);

    //Set the ADC SOCA/B for the PFC
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;

    EPwm1Regs.ETSEL.bit.SOCBEN = 1;
    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTR_PRD;
    EPwm1Regs.ETPS.bit.SOCBPRD = ET_1ST;

    //Set the ADC SOCA/B for the buck
    EPwm7Regs.ETSEL.bit.SOCAEN = 1;
    EPwm7Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm7Regs.ETPS.bit.SOCAPRD = ET_1ST;

    EPwm7Regs.ETSEL.bit.SOCBEN = 1;
    EPwm7Regs.ETSEL.bit.SOCBSEL = ET_CTR_PRD;
    EPwm7Regs.ETPS.bit.SOCBPRD = ET_1ST;

    sPwmXbarInit();
    sPwmTzDc_CBCInit(cEPWM1,DC_TRIPIN4,TZ_DCAH_LOW,TZ_DCBH_LOW);   //PFC R1
    sPwmTzDc_CBCInit(cEPWM4,DC_TRIPIN5,TZ_DCAH_LOW,TZ_DCBH_LOW);   //PFC R2
    sPwmTzDc_CBCInit(cEPWM2,DC_TRIPIN7,TZ_DCAH_LOW,TZ_DCBH_LOW);   //PFC S1
    sPwmTzDc_CBCInit(cEPWM5,DC_TRIPIN8,TZ_DCAH_LOW,TZ_DCBH_LOW);   //PFC S2
    sPwmTzDc_CBCInit(cEPWM3,DC_TRIPIN9,TZ_DCAH_LOW,TZ_DCBH_LOW);   //PFC T1
    sPwmTzDc_CBCInit(cEPWM6,DC_TRIPIN10,TZ_DCAH_LOW,TZ_DCBH_LOW);  //PFC T2
    sPwmTzDc_CBCInit(cEPWM7,DC_TRIPIN11,TZ_DCAH_LOW,TZ_DCBH_LOW);
    sPwmTzDc_CBCInit(cEPWM8,DC_TRIPIN12,TZ_DCAH_LOW,TZ_DCBH_LOW);

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;


#if cPWM_DAC == 1
    EPwm7Regs.CMPA.bit.CMPA = 200;
    EPwm8Regs.CMPA.bit.CMPA = 500;
    mPWM7_On();
    mPWM8_On();
#endif
}

void sPwmXbarInit(void)
{
    //all the EPWM X-bar registers are EALLOW protected
    //EALLOW;            //uncoment if no EALLOW protected before calling
    //trip4 for EPWM1 which is for 1st AC-DC R phase current control
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX0 = 0;   //CMPSS1.CTRIPH
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX1 = 0;   //CMPSS1.CTRIPL
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX0 = 1;     //CMPSS1.MUX0 enable
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX1 = 1;     //CMPSS1.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP4 = 1;        //invert the output, active low

    //trip5 for EPWM4 which is 2nd AC-DC R phase current control
    EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX2 = 0;   //CMPSS2.CTRIPH
    EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX3 = 0;   //CMPSS2.CTRIPL
    EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX2 = 1;     //CMPSS2.MUX0 enable
    EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX3 = 1;     //CMPSS2.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP5 = 1;        //invert the output, active low

    //trip7 for EPWM2 which is 1st AC-DC s phase current control
    EPwmXbarRegs.TRIP7MUX0TO15CFG.bit.MUX4 = 0;   //CMPSS3.CTRIPH
    EPwmXbarRegs.TRIP7MUX0TO15CFG.bit.MUX5 = 0;   //CMPSS3.CTRIPL
    EPwmXbarRegs.TRIP7MUXENABLE.bit.MUX4 = 1;     //CMPSS3.MUX0 enable
    EPwmXbarRegs.TRIP7MUXENABLE.bit.MUX5 = 1;     //CMPSS3.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP7 = 1;        //invert the output, active low

    //trip8 for EPWM5 which is for 2nd AC-DC S phase current control
    EPwmXbarRegs.TRIP8MUX0TO15CFG.bit.MUX6 = 0;   //CMPSS4.CTRIPH
    EPwmXbarRegs.TRIP8MUX0TO15CFG.bit.MUX7 = 0;   //CMPSS4.CTRIPL
    EPwmXbarRegs.TRIP8MUXENABLE.bit.MUX6 = 1;     //CMPSS4.MUX0 enable
    EPwmXbarRegs.TRIP8MUXENABLE.bit.MUX7 = 1;     //CMPSS4.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP8 = 1;        //invert the output, active low

    //trip9 for EPWM3 which is 1st AC-DC T phase current control
    EPwmXbarRegs.TRIP9MUX0TO15CFG.bit.MUX8 = 0;   //CMPSS5.CTRIPH
    EPwmXbarRegs.TRIP9MUX0TO15CFG.bit.MUX9 = 0;   //CMPSS5.CTRIPL
    EPwmXbarRegs.TRIP9MUXENABLE.bit.MUX8 = 1;     //CMPSS5.MUX0 enable
    EPwmXbarRegs.TRIP9MUXENABLE.bit.MUX9 = 1;     //CMPSS5.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP9 = 1;        //invert the output, active low

    //trip10 for EPWM6 which is 2ND AC-DC T phase current control
    EPwmXbarRegs.TRIP10MUX0TO15CFG.bit.MUX10 = 0;   //CMPSS6.CTRIPH
    EPwmXbarRegs.TRIP10MUX0TO15CFG.bit.MUX11 = 0;   //CMPSS6.CTRIPL
    EPwmXbarRegs.TRIP10MUXENABLE.bit.MUX10 = 1;     //CMPSS6.MUX0 enable
    EPwmXbarRegs.TRIP10MUXENABLE.bit.MUX11 = 1;     //CMPSS6.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP10 = 1;        //invert the output, active low

    //trip11 for EPWM7 which is 1st DC-DC current control
    EPwmXbarRegs.TRIP11MUX0TO15CFG.bit.MUX12 = 0;   //CMPSS7.CTRIPH
    EPwmXbarRegs.TRIP11MUX0TO15CFG.bit.MUX13 = 0;   //CMPSS7.CTRIPL
    EPwmXbarRegs.TRIP11MUXENABLE.bit.MUX12 = 1;     //CMPSS7.MUX0 enable
    EPwmXbarRegs.TRIP11MUXENABLE.bit.MUX13 = 1;     //CMPSS7.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP11 = 1;        //invert the output, active low

    //trip12 for EPWM8 which is 1st DC-DC current control
    EPwmXbarRegs.TRIP12MUX0TO15CFG.bit.MUX14 = 0;   //CMPSS8.CTRIPH
    EPwmXbarRegs.TRIP12MUX0TO15CFG.bit.MUX15 = 0;   //CMPSS8.CTRIPL
    EPwmXbarRegs.TRIP12MUXENABLE.bit.MUX14 = 1;     //CMPSS8.MUX0 enable
    EPwmXbarRegs.TRIP12MUXENABLE.bit.MUX15 = 1;     //CMPSS8.MUX1 enable
    EPwmXbarRegs.TRIPOUTINV.bit.TRIP12 = 1;        //invert the output, active low
    //EDIS;
}





void sPwmUserInit(uint16_t n, uint16_t Period, uint16_t coutdir, uint16_t phase,uint16_t deadtime)
{
    volatile struct EPWM_REGS *ePWM[12] =
                  {  &EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs,
                     &EPwm5Regs, &EPwm6Regs, &EPwm7Regs, &EPwm8Regs,
                     &EPwm9Regs, &EPwm10Regs, &EPwm11Regs, &EPwm12Regs};

    //Time Base SubModule Register
    (*ePWM[n]).TBCTL.bit.PRDLD = TB_IMMEDIATE;    // set Immediate load
    (*ePWM[n]).TBPRD = Period;
    (*ePWM[n]).TBPHS.bit.TBPHS = phase;
    (*ePWM[n]).TBCTR = 0;
    (*ePWM[n]).TBCTL.bit.CTRMODE = coutdir;
    (*ePWM[n]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
    (*ePWM[n]).TBCTL.bit.CLKDIV = TB_DIV1;

    (*ePWM[n]).TBCTL.bit.FREE_SOFT = 3;            //free-run
    (*ePWM[n]).TBCTL.bit.PHSEN = TB_ENABLE;         //Enable SYNCIN
    (*ePWM[n]).TBCTL.bit.SYNCOSEL = 0x1;            //SYNCOUT when CTR = 0;

    // Counter compare submodule registers
    (*ePWM[n]).CMPA.bit.CMPA = 0;           //Initialize the CMP register
    (*ePWM[n]).CMPA.bit.CMPAHR = 0;
    (*ePWM[n]).CMPB.bit.CMPB = 0;
    (*ePWM[n]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    (*ePWM[n]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    (*ePWM[n]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    (*ePWM[n]).CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    (*ePWM[n]).AQCTLA.bit.CAU = AQ_SET;
    (*ePWM[n]).AQCTLA.bit.CAD = AQ_CLEAR;
    (*ePWM[n]).AQCTLA.bit.PRD = AQ_NO_ACTION;
    (*ePWM[n]).AQCTLA.bit.ZRO = AQ_NO_ACTION;

    //In current control mode, the HiResPwm is disabled, the MCU generate the deadtime automatically
    (*ePWM[n]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //Both rising and falling edge
    (*ePWM[n]).DBCTL.bit.POLSEL = DB_ACTV_HIC;  // Active high complementary
    (*ePWM[n]).DBFED.bit.DBFED = deadtime;                      //main switch off to aux-switch on delay
    (*ePWM[n]).DBRED.bit.DBRED = deadtime;                       //aux-switch off to main switch on delay

    /*EALLOW;
    (*ePWM[n]).HRCNFG.all = 0x0;
    (*ePWM[n]).HRCNFG.bit.EDGMODE = HR_FEP;          // MEP control on both edges
    (*ePWM[n]).HRCNFG.bit.CTLMODE = HR_CMP;          // CMPAHR and TBPRDHR HR control
    (*ePWM[n]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;     // load on CTR = 0 and PRD
    (*ePWM[n]).HRCNFG.bit.AUTOCONV = 1;              // Enable autoconversion for HR period
    (*ePWM[n]).HRPCTL.bit.TBPHSHRLOADE = 0;          // Enable TBPHSHR sync (required for updwn count HR control)
    (*ePWM[n]).HRPCTL.bit.HRPE = 0;                  // Turn on high-resolution period control.
    EDIS;*/

//===========================================================================
// TZA events can force EPWMxA
// TZB events can force EPWMxB
    //(*ePWM[n]).TZSEL.bit.CBC1 = TZ_ENABLE; // EPWM1A will go low
    (*ePWM[n]).TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
    (*ePWM[n]).TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low
    (*ePWM[n]).TZCTL.bit.DCBEVT2 = TZ_FORCE_LO; // EPWMxB will go low
    (*ePWM[n]).TZCTL.bit.DCAEVT2 = TZ_FORCE_LO; // EPWMxB will go low
    (*ePWM[n]).TZFRC.bit.OST = 1;           // Turn off the PWM
    //EDIS;
}

void sPwmTzDc_CBCInit(uint16_t n,uint16_t TripSrc,uint16_t ActA,uint16_t ActB)
{
    volatile struct EPWM_REGS *ePWM[12] =
                      {  &EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs,
                         &EPwm5Regs, &EPwm6Regs, &EPwm7Regs, &EPwm8Regs,
                         &EPwm9Regs, &EPwm10Regs, &EPwm11Regs, &EPwm12Regs};

    //(*ePWM[n]).TZDCSEL.bit.DCAEVT2 = TZ_DCAH_LOW;  //EVT2 is CBC source,when goes low
    //(*ePWM[n]).TZDCSEL.bit.DCBEVT2 = TZ_DCBH_LOW;   //EVT2 is CBC source,when goes low
    (*ePWM[n]).TZDCSEL.bit.DCAEVT2 = ActA;   //EVT2 is CBC source,when goes low
    (*ePWM[n]).TZDCSEL.bit.DCBEVT2 = ActB;   //EVT2 is CBC source,when goes low
    (*ePWM[n]).TZSEL.bit.DCBEVT2 = 1;               //CBC enalbed
    (*ePWM[n]).TZSEL.bit.DCAEVT2 = 1;               //CBC enalbed

    (*ePWM[n]).DCTRIPSEL.bit.DCBHCOMPSEL = TripSrc;     //Select TRIP4 source of the DCBH
    (*ePWM[n]).DCTRIPSEL.bit.DCAHCOMPSEL = TripSrc;     //Select TRIP4 source of the DCAH
    (*ePWM[n]).DCACTL.bit.EVT2SRCSEL = 0;           //Select DCAEVT2
    (*ePWM[n]).DCACTL.bit.EVT2FRCSYNCSEL = 0;       //SYN with TBCLK
    (*ePWM[n]).DCBCTL.bit.EVT2SRCSEL = 0;           //Select DCBEVT2
    (*ePWM[n]).DCBCTL.bit.EVT2FRCSYNCSEL = 0;       //SYN with TBCLK
}

void sFanPwmInit(void)
{
    EALLOW;
    //GpioCtrlRegs.GPAGMUX2.bit.GPIO22 = 1;
    //GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;   // Configure GPIO22 as EPWM12A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO23 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;   // Configure GPIO22 as EPWM12B

    EPwm12Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;    // set Immediate load
    EPwm12Regs.TBPRD = 25000;                     //2k
    EPwm12Regs.TBPHS.bit.TBPHS = 0;
    EPwm12Regs.TBCTR = 0;
    EPwm12Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm12Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    EPwm12Regs.TBCTL.bit.FREE_SOFT = 3;            //free-run
    EPwm12Regs.TBCTL.bit.PHSEN = TB_DISABLE;         //Enable SYNCIN
    EPwm12Regs.TBCTL.bit.SYNCOSEL = 0x1;            //SYNCOUT when CTR = 0;

    // Counter compare submodule registers
    EPwm12Regs.CMPA.bit.CMPA = 0;           //Initialize the CMP register
    EPwm12Regs.CMPA.bit.CMPAHR = 0;
    EPwm12Regs.CMPB.bit.CMPB = 25000>>2;
    EPwm12Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm12Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm12Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm12Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm12Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm12Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm12Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm12Regs.AQCTLB.bit.CBD = AQ_SET;
    EPwm12Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm12Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;

    //In current control mode, the HiResPwm is disabled, the MCU generate the deadtime automatically
    EPwm12Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;  //DB_DISABLE

    /*EALLOW;
    EPwm12Regs.HRCNFG.all = 0x0;
    EPwm12Regs.HRCNFG.bit.EDGMODE = HR_FEP;          // MEP control on both edges
    EPwm12Regs.HRCNFG.bit.CTLMODE = HR_CMP;          // CMPAHR and TBPRDHR HR control
    EPwm12Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;     // load on CTR = 0 and PRD
    EPwm12Regs.HRCNFG.bit.AUTOCONV = 1;              // Enable autoconversion for HR period
    EPwm12Regs.HRPCTL.bit.TBPHSHRLOADE = 0;          // Enable TBPHSHR sync (required for updwn count HR control)
    EPwm12Regs.HRPCTL.bit.HRPE = 0;                  // Turn on high-resolution period control.
    EDIS;*/

//===========================================================================
// TZA events can force EPWMxA
// TZB events can force EPWMxB
    //EPwm12Regs.TZSEL.bit.CBC1 = TZ_ENABLE; // EPWM1A will go low
    //EPwm12Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
    //EPwm12Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

    //EPwm12Regs.TZFRC.bit.OST = 1;           // Turn off the PWM
    EDIS;
}


