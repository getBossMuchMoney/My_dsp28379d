/*******************************************************************************
 * File Name: Initial.c
 * Description: The MCU initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "App.h"
#include "F28x_Project.h"
#include "Adc_User.h"
#include "PWM_User.h"
#include "Cla_User.h"
#include "Gpio_User.h"
#include "UartDebug.h"
#include "Cmpss_User.h"

void sMcuInitial(void);
void sEnableINT(void);
void sCpuTimerInit(void);
void sInitScia(void);

void sMcuInitial(void)
{
    //Config the PLL, FLASH,
    InitSysCtrl();

    //Dual CPU configuration
#ifdef _DUAL_CPU
    //EALLOW;
    //DevCfgRegs.CPUSEL0.bit.EPWM1 = 1;
   // DevCfgRegs.CPUSEL11.bit.ADC_A = 1;
    //EDIS;
    // Send boot command to allow the CPU2 application to begin execution
    //IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#endif


   InitPieCtrl();

   CLA_configClaMemory();
   CLA_initCpu1Cla1();
   Cla1ForceTask8();

   sGpioInit();


   sAdcInit();
   sCmpssInit();
   sPWMInit();
   sCpuTimerInit();
   sInitScia();

   sEnableINT();

}


/******************************************************************************
 * Function Name: sCpuTimerInit
 * Description: The CPU timer0 initialization for system time-ticker
 * Input: None
 * Output: None
 * Return: None
 * ***************************************************************************/
void sCpuTimerInit(void)
{
    CpuTimer0Regs.PRD.all =  200000;     // A tasks
    CpuTimer0Regs.TCR.bit.TIF = 0x01;   //Clear the CpuTimer0 Interrupt Flag
    CpuTimer0Regs.TCR.bit.TIE = 0x01;   //Enable the CpuTimer0 Interrupt
}

void sInitScia(void)
{
    // initial SCIA
#ifdef _LAUNCHXL_F28379D
    EALLOW;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO42 = 0x3;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO43 = 0x3;
    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0x3;
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0x3;
    EDIS;
#else
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29  = 1;    //SCIA TX
    GpioCtrlRegs.GPAMUX2.bit.GPIO28  = 1;    //SCIA RX
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;    //GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;     //OUTPUT
    GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;   //SET LOW
    EDIS;
#endif

    //One stop bit, no parity, 8-bit character length
    SciaRegs.SCICCR.bit.SCICHAR = 0x7;

    //Enable transmit and receive
    SciaRegs.SCICTL1.bit.RXENA = 1;
    SciaRegs.SCICTL1.bit.TXENA = 1;

    //The default of the LOSPSP is 50MHz
    //BRR = (50MHz/(baudrate*8)) - 1 = 0xA2B
    SciaRegs.SCIHBAUD.bit.BAUD = 0x01;
    SciaRegs.SCILBAUD.bit.BAUD = 0x44;

    //Enable Receive interrupt
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
    SciaRegs.SCICTL1.bit.SWRESET = 1;    // Relinquish SCI from Reset
}



void sEnableINT(void)
{
    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    //CLA_initCpu1Cla1();
    //Cla1ForceTask8();

    //Interrupt vector table initialization
    EALLOW;
    PieVectTable.ADCA1_INT = &sADCA1_isr; //function for ADCA interrupt 1
    PieVectTable.TIMER0_INT = &sRtosTimerInterruptIsr; //function for TIMER0_INT
    PieVectTable.SCIA_RX_INT = &sSciaRxIsr;
    PieVectTable.SCIA_TX_INT = &sSciaTxIsr;
    //PieVectTable.ADCD1_INT = &sADCD1_isr;  //ADCD interrupt1
    EDIS;

    // enable PIE interrupt
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;      //ADCA1 interrupt
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      //TIMER0 interrupt
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;      //SCIA RX
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;      //SCIA TX
    //PieCtrlRegs.PIEIER1.bit.INTx6 = 1;    //ADCD1 interrupt
    //PieCtrlRegs.PIEIER11.bit.INTx1 = 1;   //CLA Task1 interrupt

    PieCtrlRegs.PIEACK.all = 0xFFFF;


    IER |= (M_INT1 | M_INT11 | M_INT9); //Enable group 1 interrupts
    //EINT;          //Enable global interrupt
    //ERTM;
}



