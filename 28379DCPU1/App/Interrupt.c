/*******************************************************************************
 * File Name: Interrupt.c
 * Description: The interrupt ISR functions.
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
#include "PfcController.h"
#include "UartDebug.h"

uint16_t u16_Adca[16];
uint64_t timercnt = 0;

interrupt void sADCA1_isr(void);
interrupt void sADCD1_isr(void);
interrupt void sRtosTimerInterruptIsr(void);
interrupt void sSciaRxIsr(void);
interrupt void sSciaTxIsr(void);

uint16_t u16_inttest;
/*******************************************************************************
 * Function Name: sADCA1_isr
 * Description: The function is for PWM rectifier of PFC controller.
 *******************************************************************************/
#ifdef _FLASH
#pragma CODE_SECTION(sADCA1_isr, ".TI.ramfunc");
#endif
interrupt void sADCA1_isr(void)
{
   /* u16_Adca[0] = AdcaResultRegs.ADCRESULT0;
    u16_Adca[1] = AdcaResultRegs.ADCRESULT1;
    u16_Adca[2] = AdcaResultRegs.ADCRESULT2;
    u16_Adca[3] = AdcaResultRegs.ADCRESULT3;
    u16_Adca[4] = AdcaResultRegs.ADCRESULT4;
    u16_Adca[5] = AdcaResultRegs.ADCRESULT5;
    u16_Adca[6] = AdcaResultRegs.ADCRESULT6;
    u16_Adca[7] = AdcaResultRegs.ADCRESULT7;

    u16_Adca[8] = AdcaResultRegs.ADCRESULT8;
    u16_Adca[9] = AdcaResultRegs.ADCRESULT9;
    u16_Adca[10] = AdcaResultRegs.ADCRESULT10;
    u16_Adca[11] = AdcaResultRegs.ADCRESULT11;
    u16_Adca[12] = AdcaResultRegs.ADCRESULT12;
    u16_Adca[13] = AdcaResultRegs.ADCRESULT13;
    u16_Adca[14] = AdcaResultRegs.ADCRESULT14;
    u16_Adca[15] = AdcaResultRegs.ADCRESULT15;*/

    /*if(++u16_inttest >= 18000)
    {
        //mLedRed_Toggle();
        u16_inttest = 0;
    }*/

    sAdcCpuSample();
    sSampelCalculation();
    sPfcController();

#if cDataBufEn == 1
    sDataLog();
#endif

    if(u16_Tx2RxFlag[0] != 0)
    {
        u16_Tx2RxFlag[0]++;
        if(u16_Tx2RxFlag[0] >= Fs)  //1ms
        {
            u16_Tx2RxFlag[0] = 0;
            m485_RX1_EN();
        }
    }

    EALLOW;
    EPwm1Regs.TZCLR.all = 0x0052;
    EPwm2Regs.TZCLR.all = 0x0052;
    EPwm3Regs.TZCLR.all = 0x0052;
    EPwm4Regs.TZCLR.all = 0x0052;
    EPwm5Regs.TZCLR.all = 0x0052;
    EPwm6Regs.TZCLR.all = 0x0052;

    EDIS;

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


interrupt void sADCD1_isr(void)
{
    //u16_inttest++;
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

#ifdef _FLASH
#pragma CODE_SECTION(sRtosTimerInterruptIsr, ".TI.ramfunc");
#endif
interrupt void sRtosTimerInterruptIsr(void)
{
	timercnt++;
    sRTOSTimerTicker();

    //Reset the interrupt
    CpuTimer0Regs.TCR.bit.TIF = 1;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}


interrupt void sSciaRxIsr(void)
{

    if(SciaRegs.SCIRXST.bit.RXERROR)    //soft reset sci module when errors exist
    {
        SciaRegs.SCICTL1.bit.SWRESET = 0;
        SciaRegs.SCICTL1.bit.SWRESET = 1;
    }
    else
    {
        u16_inttest++;
        sSciRxISR(0);
    }
    // Enable future SCIA RX (PIE Group 9) interrupts
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
interrupt void sSciaTxIsr(void)
{
    sSciTxISR(0);
    // Enable future SCIA TX (PIE Group 9) interrupts
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}





