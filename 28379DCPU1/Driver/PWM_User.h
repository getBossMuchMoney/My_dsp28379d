/*******************************************************************************
 * File Name: PWM_User.c
 * Description: The ePWM initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _PWM_USER_H
#define _PWM_USER_H

#define cEPWM1          0
#define cEPWM2          1
#define cEPWM3          2
#define cEPWM4          3
#define cEPWM5          4
#define cEPWM6          5
#define cEPWM7          6
#define cEPWM8          7
#define cEPWM9          8
#define cEPWM10         9
#define cEPWM11         10
#define cEPWM12         11

#define Fs_PFC          1389    //100MHz/(36kHz*2)
#define Fs_Buck         1389
#define cDeadtime1       150      //1.5us
#define cDeadtime2       150      //1.5us
#define cILPhase        1389

#define cPWM_DAC        0

#if cPWM_DAC == 1
#define mPWM7_On()         {EALLOW;EPwm7Regs.TZCLR.bit.OST = 1;EDIS;}
#define mPWM7_Off()        {EALLOW;EPwm7Regs.TZFRC.bit.OST = 1;EDIS;}
#define mPWM8_On()         {EALLOW;EPwm8Regs.TZCLR.bit.OST = 1;EDIS;}
#define mPWM8_Off()        {EALLOW;EPwm8Regs.TZFRC.bit.OST = 1;EDIS;}
#else

#endif

#define mPFC1_PwmOn()      {EALLOW; \
                            EPwm1Regs.TZCLR.bit.OST = 1; \
                            EPwm2Regs.TZCLR.bit.OST = 1; \
                            EPwm3Regs.TZCLR.bit.OST = 1; \
                            stSuper.u16_SysStatus.BIT.Pfc1PwmStatus = 1; \
                            EDIS;}
/*#define mPFC1_PwmOn()      {EALLOW; \
                            EPwm2Regs.TZCLR.bit.OST = 1; \
                            stSuper.u16_SysStatus.BIT.Pfc1PwmStatus = 1; \
                            EDIS;}*/
#define mPFC2_PwmOn()      {EALLOW; \
                            EPwm4Regs.TZCLR.bit.OST = 1; \
                            EPwm5Regs.TZCLR.bit.OST = 1; \
                            EPwm6Regs.TZCLR.bit.OST = 1; \
                            stSuper.u16_SysStatus.BIT.Pfc2PwmStatus = 1; \
                            EDIS;}
/*#define mPFC2_PwmOn()      {EALLOW; \
                            EPwm6Regs.TZCLR.bit.OST = 1; \
                            stSuper.u16_SysStatus.BIT.Pfc2PwmStatus = 1; \
                            EDIS;}*/
#define mPFC1_PwmOff()      {EALLOW; \
                            EPwm1Regs.TZFRC.bit.OST = 1; \
                            EPwm2Regs.TZFRC.bit.OST = 1; \
                            EPwm3Regs.TZFRC.bit.OST = 1; \
                            stSuper.u16_SysStatus.BIT.Pfc1PwmStatus = 0; \
                            EDIS;}
#define mPFC2_PwmOff()      {EALLOW; \
                            EPwm4Regs.TZFRC.bit.OST = 1; \
                            EPwm5Regs.TZFRC.bit.OST = 1; \
                            EPwm6Regs.TZFRC.bit.OST = 1; \
                            stSuper.u16_SysStatus.BIT.Pfc1PwmStatus = 0; \
                            EDIS;}

#define mBuck1_PwmOn()       {EALLOW;EPwm7Regs.TZCLR.bit.OST = 1;stSuper.u16_SysStatus.BIT.Dc1PwmStatus = 1;EDIS;}
#define mBuck2_PwmOn()       {EALLOW;EPwm8Regs.TZCLR.bit.OST = 1;stSuper.u16_SysStatus.BIT.Dc2PwmStatus = 1;EDIS;}
#define mBuck1_PwmOff()      {EALLOW;EPwm7Regs.TZFRC.bit.OST = 1;stSuper.u16_SysStatus.BIT.Dc1PwmStatus = 0;EDIS;}
#define mBuck2_PwmOff()      {EALLOW;EPwm8Regs.TZFRC.bit.OST = 1;stSuper.u16_SysStatus.BIT.Dc2PwmStatus = 0;EDIS;}

#define mPFC1_CMPR_R()      EPwm1Regs.CMPA.bit.CMPA
#define mPFC1_CMPR_S()      EPwm2Regs.CMPA.bit.CMPA
#define mPFC1_CMPR_T()      EPwm3Regs.CMPA.bit.CMPA
#define mPFC2_CMPR_R()      EPwm4Regs.CMPA.bit.CMPA
#define mPFC2_CMPR_S()      EPwm5Regs.CMPA.bit.CMPA
#define mPFC2_CMPR_T()      EPwm6Regs.CMPA.bit.CMPA
#define mDC1_CMPR()         EPwm7Regs.CMPA.bit.CMPA
#define mDC2_CMPR()         EPwm8Regs.CMPA.bit.CMPA
#define mFanSpdCtrl(x)       {EPwm12Regs.CMPB.bit.CMPB = x;}

extern void sPWMInit(void);

#endif




