/*******************************************************************************
 * File Name: Gpio_User.c
 * Description: The GPIO initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "F28x_Project.h"
#include "Gpio_User.h"


void sGpioInit(void);

void sGpioInit(void)
{
    InitGpio(); //Reset the GPIO

    EALLOW;

#ifdef _LAUNCHXL_F28379D
    GpioCtrlRegs.GPACSEL4.bit.GPIO31 = 1;   //Assign for CPU1 CLA control
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;   // Configure GPIO34 as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;    //output
    GpioDataRegs.GPASET.bit.GPIO31 = 1;    //Set high;
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;   // Configure GPIO34 as GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;    //output
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;    //Set high;
#endif
    //IP RELAY
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0; //GPIO22 as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;  //OUTPUT
    GpioDataRegs.GPACLEAR.bit.GPIO22 = 1; //SET LOW

    //IP SOFT RELAY
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0; //GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;  //OUTPUT
    GpioDataRegs.GPACLEAR.bit.GPIO31 = 1; //SET LOW

    //OP SOFT RELAY
    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;  //OUTPUT
    GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1; //SET LOW

    //OP RELAY
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;  //OUTPUT
    GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1; //SET LOW

    //PROTECT T1
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO59 = 0;  //INPUT
    //PROTECT R2
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 0;  //INPUT
    //PROTECT S2
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0;  //INPUT

    //SYNC IN
    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO62 = 0;  //INPUT
    //SYNC OUT
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;  //INPUT
    GpioDataRegs.GPBSET.bit.GPIO63 = 1; //SET HIGH

    //protect S1
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO64 = 0;  //INPUT
    //PROTECT BUCK1
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO65 = 0;  //INPUT
    //PROTECT BUCK2
    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO66 = 0;  //INPUT
    //PROTECT t2
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO67 = 0;  //INPUT
    //PROTECT r1
    GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO68 = 0;  //INPUT

    //On board LED
    GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO45 = 1;  //OUTPUT
    GpioDataRegs.GPBCLEAR.bit.GPIO45 = 1; //SET LOW

    //Aux power fault
    GpioCtrlRegs.GPEMUX1.bit.GPIO133 = 0; //GPIO
    GpioCtrlRegs.GPEDIR.bit.GPIO133 = 0;  //INPUT

    //Emergency stop
    GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 0; //GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO47 = 0;  //INPUT

    //Carrier Sync in
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 0;  //INPUT
    //Carrier Sync out
    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO71 = 1;  //OUTPUT
    GpioDataRegs.GPCSET.bit.GPIO71 = 1;  //SET HIGH

    //System Stauts Led
    GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO73 = 1;  //OUTPUT
    GpioDataRegs.GPCSET.bit.GPIO73 = 1;  //SET HIGH
    //System fault led
    GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO74 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO74 = 1;  //SET LOW
    //Unit status led
    GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO75 = 1;  //OUTPUT
    GpioDataRegs.GPCSET.bit.GPIO75 = 1;  //SET HIGH
    //Unit fault led
    GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO76 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO76 = 1;  //SET LOW

    //physical address :switch 1~6
    GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO77 = 0;  //INPUT
    GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO78 = 0;  //INPUT
    GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO79 = 0;  //INPUT

    GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO80 = 0;  //INPUT
    GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO81 = 0;  //INPUT
    GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO82 = 0;  //INPUT

    //Dry contact 1
    GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO83 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO83 = 1;  //SET LOW
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO85 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;  //SET LOW

    //Temp select A0~A2
    GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO86 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO86 = 1;  //SET LOW
    GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO87 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1;  //SET LOW
    GpioCtrlRegs.GPCMUX2.bit.GPIO88 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO88 = 1;  //OUTPUT
    GpioDataRegs.GPCCLEAR.bit.GPIO88 = 1;  //SET LOW

    //VBUS_OVP
    GpioCtrlRegs.GPCMUX2.bit.GPIO90 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO90 = 0;  //INPUT
    //FAN_FAULT
    GpioCtrlRegs.GPCMUX2.bit.GPIO91 = 0; //GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO91 = 0;  //INPUT

    EDIS;
}
