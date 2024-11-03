/*******************************************************************************
 * File Name: SuperTask.c
 * Description: The system status machine.
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
#include "MeasureTask.h"
#include "ShareData.h"
#include "math.h"
#include "stdio.h"
#include "SuperTask.h"
#include "PfcController.h"
#include "MeasureTask.h"

#pragma DATA_SECTION(stSuper,"CLADataLS1");
SUPER_ST stSuper;
void sSuperTaskInit(void);
void sSuperTask(void);
void sPowerOnMode(void);
void sStandbyMode(void);
void sSoftStartMode(void);
void sCCMode(void);
void sCVMode(void);
void sCPMode(void);
void sFaultMode(void);
void sAllPWMOff(void);

void sSuperTaskInit(void)
{
    stSuper.u16_SysMode = cPowerOnMode;
    stSuper.u16_PowerOnModeCnt = 0;
    stSuper.u32_FaultCode.all = 0;
    stSuper.u16_SysStatus.all = 0;
    stSuper.u16_OutputMode = OP_CC;
    stSuper.u16_SoftStartModeCnt = 0;
    stSuper.u32_SwVersion = cSW_VERSION;
}

void sSuperTask(void)
{
    switch(stSuper.u16_SysMode)
    {
        case cPowerOnMode:
            sPowerOnMode();
        break;

        case cStandbyMode:
            sStandbyMode();
        break;

        case cSoftStartMode:
            sSoftStartMode();
        break;

        case cCCMode:
            sCCMode();
        break;

        case cCVMode:
            sCVMode();
        break;

        case cCPMode:
            sCPMode();
        break;

        case cFaultMode:
            sFaultMode();
        break;

        default:
        break;
    }
}

void sPowerOnMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event


    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cFaultMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {
        //Wait for the ADC offset calculation ready
        if((stAdcSample.u16_MidCalFlag == 0) || (stSampleCla.u16_MidCalFlag == 0))
        {
            return;
        }

        //Bus pre-charge
        if(stSuper.u16_SysStatus.BIT.BusPreCharge == 0)
        {
            if(stSuper.u16_SysStatus.BIT.IpSoftRly == 0)
            {
                mIPSoftRelay_On();
            }
            if(stSuper.u16_SysStatus.BIT.OpSoftRly == 0)
            {
                mOPSoftRelay_On();
            }
            if(stSuper.u16_PowerOnModeCnt++ >= cSuper5s)
            {
                //Check the bus voltage
#if cPWM_OpenLoop_Test == 0
                if(stMeasure.f32_Bus_V_Avg >= cPreChargeVolt)
#endif
                {
                    stSuper.u16_PowerOnModeCnt = 0;
                    stSuper.u16_SysStatus.BIT.BusPreCharge = 1;
                    mIPSoftRelay_Off();
                    mOPSoftRelay_Off();
                    mIPRelay_On();
                    stSuper.u16_SysMode = cStandbyMode;
                    return;
                }
                /*else
                {
                    stSuper.u16_PowerOnModeCnt = 0;
                    stSuper.u16_SysStatus.BIT.BusPreCharge = 0;
                }*/
            }
            else
            {
                return;
            }
        }

    }
}
void sStandbyMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event

    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {
#if cPWM_OpenLoop_Test == 0
        if((stSuper.u16_SysStatus.BIT.IpMainRly == 1) &&
           (stSuper.u16_SysStatus.BIT.GridVoltNormal == 1))
#endif
        {
            //Reset the PFC controller;
            sPfcControllerReset();
            //Turn on the PWM
            mPFC1_PwmOn();
            mPFC2_PwmOn();
            mBuck1_PwmOn();
            mBuck2_PwmOn();
            stSuper.u16_SoftStartModeCnt = 0;
            stSuper.u16_SysMode = cSoftStartMode;
            return;
        }
    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cStandbyMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cFaultMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {
        if(stSuper.u16_SysStatus.BIT.OpMainRly == 1)
        {
            mOPRelay_Off();
        }

        stSuper.u16_StandbyCnt++;
        if(stSuper.u16_StandbyCnt >= cSuper1s)
        {
            stSuper.u16_StandbyCnt = 0;
            sRTOSEventSend(cPrioSuper,eSuperTunOnEvt);
        }
    }
}
void sSoftStartMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event

    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cStandbyMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cFaultMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {

#if cPWM_OpenLoop_Test == 0
        if(stCtrlPiBus.f32_Ref < 700.0f)
        {
            stCtrlPiBus.f32_Ref += 5.0f;
        }
        if((stMeasure.u16_Bus_V_Avg >= (cSoftStartVolt - cBusNormalStep)) &&
          (stMeasure.u16_Bus_V_Avg <= (cSoftStartVolt + cBusNormalStep)))
#else
       if(stCtrlPiBus.f32_Ref < 700.0f)
       {
           stCtrlPiBus.f32_Ref += 1.0f;
       }
       else
#endif
       {
           if(stSuper.u16_OutputMode == OP_CC)
           {
               stSuper.u16_SysMode = cCCMode;
               return;
           }
           else if(stSuper.u16_OutputMode == OP_CV)
           {
               stSuper.u16_SysMode = cCVMode;
               return;
           }
           else if(stSuper.u16_OutputMode == OP_CP)
           {
               stSuper.u16_SysMode = cCPMode;
               return;
           }
       }
       if(stSuper.u16_SoftStartModeCnt++ >= cSuper5s)
       {
           sAllPWMOff();
           stSuper.u16_SysMode = cFaultMode;
           stSuper.u16_SoftStartModeCnt = 0;
           stSuper.u32_FaultCode.BIT.SoftStartFail = 1;
           return;
       }
    }

}
void sCCMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event


    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cStandbyMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cFaultMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {
        if(stSuper.u16_SysStatus.BIT.OpMainRly == 0)
        {
            mOPRelay_On();
        }
        else
        {
            if((stSuper.u16_SysStatus.BIT.Dc1PwmStatus == 0) && (stSuper.u16_SysStatus.BIT.Dc2PwmStatus == 0))
            //if(stSuper.u16_SysStatus.BIT.Dc2PwmStatus == 0)
            {
                sDcControllerReset();
                mBuck1_PwmOn();
                mBuck2_PwmOn();
            }

        }
    }
}
void sCVMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event

    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cStandbyMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cFaultMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {

    }
}
void sCPMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event


    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cStandbyMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {
        sAllPWMOff();
        stSuper.u16_SysMode = cFaultMode;
        return;
    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {

    }
}
void sFaultMode(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(cPrioSuper);     //Get the current task event


    if(event & ((uint16_t)1 << eSuperTunOnEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTunOffEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperFaultEvt))
    {

    }
    if(event & ((uint16_t)1 << eSuperTimerEvt))
    {

    }
}

void sAllPWMOff(void)
{
    mPFC1_PwmOff();
    mPFC2_PwmOff();
    mBuck1_PwmOff();
    mBuck2_PwmOff()
}

