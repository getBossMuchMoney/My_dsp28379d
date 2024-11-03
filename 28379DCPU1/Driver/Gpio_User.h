/*******************************************************************************
 * File Name: Gpio_User.h
 * Description: The GPIO initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _GPIO_USER_H
#define _GPIO_USER_H

#define  mLedRed_On()      {GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;}
#define  mLedRed_Off()      {GpioDataRegs.GPBSET.bit.GPIO34 = 1;}
#define  mLedRed_Toggle()      {GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;}

#define  mLedBlue_On()      {GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;}
#define  mLedBlue_Off()      {GpioDataRegs.GPASET.bit.GPIO31 = 1;}
#define  mLedBlue_Toggle()      {GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;}

#define  mOnBoardLed_On()       {GpioDataRegs.GPBCLEAR.bit.GPIO45 = 1;}
#define  mOnBoardLed_Off()      {GpioDataRegs.GPBSET.bit.GPIO45 = 1;}
#define  mOnBoardLed_Toggle()      {GpioDataRegs.GPBTOGGLE.bit.GPIO45 = 1;}
#define  mSysStsLed_On()        {GpioDataRegs.GPCSET.bit.GPIO73 = 1;}
#define  mSysStsLed_Off()        {GpioDataRegs.GPCCLEAR.bit.GPIO73 = 1;}
#define  mSysFaultLed_On()        {GpioDataRegs.GPCSET.bit.GPIO74 = 1;}
#define  mSysFaultLed_Off()        {GpioDataRegs.GPCCLEAR.bit.GPIO74 = 1;}
#define  mUnitStsLed_On()        {GpioDataRegs.GPCSET.bit.GPIO73 = 1;}
#define  mUnitStsLed_Off()        {GpioDataRegs.GPCCLEAR.bit.GPIO73 = 1;}
#define  mUnitFaultLed_On()        {GpioDataRegs.GPCSET.bit.GPIO74 = 1;}
#define  mUnitFaultLed_Off()        {GpioDataRegs.GPCCLEAR.bit.GPIO74 = 1;}

#define  mPhyAddrData()     ((GpioDataRegs.GPCDAT.all & 0x0007E000UL) >> 13)

#define  mIPRelay_On()      {GpioDataRegs.GPASET.bit.GPIO22 = 1;stSuper.u16_SysStatus.BIT.IpMainRly = 1;}
#define  mIPRelay_Off()     {GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;stSuper.u16_SysStatus.BIT.IpMainRly = 0;}

#define  mIPSoftRelay_On()      {GpioDataRegs.GPASET.bit.GPIO31 = 1; stSuper.u16_SysStatus.BIT.IpSoftRly = 1;}
#define  mIPSoftRelay_Off()     {GpioDataRegs.GPACLEAR.bit.GPIO31 = 1; stSuper.u16_SysStatus.BIT.IpSoftRly = 0;}

#define  mOPSoftRelay_On()      {GpioDataRegs.GPBSET.bit.GPIO40 = 1;stSuper.u16_SysStatus.BIT.OpSoftRly = 1;}
#define  mOPSoftRelay_Off()     {GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;stSuper.u16_SysStatus.BIT.OpSoftRly = 0;}

#define  mOPRelay_On()          {GpioDataRegs.GPBSET.bit.GPIO41 = 1;stSuper.u16_SysStatus.BIT.OpMainRly = 1;}
#define  mOPRelay_Off()         {GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;stSuper.u16_SysStatus.BIT.OpMainRly = 0;}

#define  mSyncIn_Sts()          (GpioDataRegs.GPBDAT.bit.GPIO62)
#define  mSyncOutTx_High()        {GpioDataRegs.GPBSET.bit.GPIO63 = 1;}
#define  mSyncOutTx_Low()         {GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1;}

#define  mCarrierSyncTx_Sts()         (GpioDataRegs.GPCDAT.bit.GPIO70)
#define  mCarrierSyncTx_High()        {GpioDataRegs.GPCSET.bit.GPIO71 = 1;}
#define  mCarrierSyncTx_Low()         {GpioDataRegs.GPCLEAR.bit.GPIO71 = 1;}

#define  m485_RX1_EN()               {GpioDataRegs.GPACLEAR.bit.GPIO30 = 1; }
#define  m485_TX1_EN()               {GpioDataRegs.GPASET.bit.GPIO30 = 1; }

#define  m485_RX2_EN()               {GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1; }
#define  m485_TX2_EN()               {GpioDataRegs.GPBSET.bit.GPIO53 = 1; }

#define  m485_RX3_EN()               {GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1; }
#define  m485_TX3_EN()               {GpioDataRegs.GPBSET.bit.GPIO58 = 1; }


#define  mPrtectR1_Sts()        (GpioDataRegs.GPCDAT.bit.GPIO68)
#define  mPrtectR2_Sts()        (GpioDataRegs.GPBDAT.bit.GPIO60)
#define  mPrtectS1_Sts()        (GpioDataRegs.GPCDAT.bit.GPIO64)
#define  mPrtectS2_Sts()        (GpioDataRegs.GPBDAT.bit.GPIO61)
#define  mPrtectT1_Sts()        (GpioDataRegs.GPBDAT.bit.GPIO59)
#define  mPrtectT2_Sts()        (GpioDataRegs.GPCDAT.bit.GPIO67)
#define  mPrtectBUCK1_Sts()     (GpioDataRegs.GPCDAT.bit.GPIO65)
#define  mPrtectBUCK2_Sts()     (GpioDataRegs.GPCDAT.bit.GPIO66)

#define  mPowerFault_Sts()      (GpioDataRegs.GPEDAT.bit.GPIO133)
#define  mEmgcyStop_Sts()       (GpioDataRegs.GPBDAT.bit.GPIO47)

#define  mDO1_On()             {GpioDataRegs.GPCSET.bit.GPIO83 = 1;}
#define  mDO1_Off()             {GpioDataRegs.GPCCLEAR.bit.GPIO83 = 1;}
#define  mDO2_On()             {GpioDataRegs.GPCSET.bit.GPIO85 = 1;}
#define  mDO2_Off()             {GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;}

#define  mTempSelect(x)         {GpioDataRegs.GPCDAT.all |= ((uint32_t)(x&0x7)<<22);}

extern void sGpioInit(void);

#endif




