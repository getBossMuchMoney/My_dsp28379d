/*******************************************************************************
 * File Name: main.c
 * Description: The main function for the system. The MCU initialization and Task
 *              creation will be finished in the function as well as the execution
 *              of the background loop.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#include "F28x_Project.h"
#include "App.h"
#include "RTOS_APP.h"
#include "RTOS_Define.h"
#include "Adc_User.h"
#include "PWM_User.h"
#include "Gpio_User.h"
#include "MeasureTask.h"
#include "ShareData.h"
#include "SuperTask.h"
#include "UartDebug.h"
#include "DebugTask.h"
#include "CAN_User.h"
#include "Modbus_RTU.h"

void sOnboardLedTask(void);
void sDataInit(void);
void sSysTaskInit(void);
void CAN_loopback_test(void);
void led_run(void);

uint16_t u16_TestTaskCnt;
uint16_t u16_PwmTestOn = 0;

uint16_t txMsgData[8];
uint16_t rxMsgData[20];
uint16_t sizeaa = 0;

void main(void)
{
    sMcuInitial();

    //Data initial
    sDataInit();

    //CAN initial
    CAN_Init();

    //The RTOS initialization
    sRTOSInit();



    //Create the task
    sRTOSTaskCreate(sOnboardLedTask,15,c20ms,0);
    sRTOSTaskCreate(sSuperTask,cPrioSuper,c20ms,1);
    sRTOSTaskCreate(sMeasureTask,cPrioMeasure,c20ms,5);
    sRTOSTaskCreate(CAN_loopback_test,12,500,6);

#if cDebugUartEn == 1
    sRTOSTaskCreate(sDebugTask,cPrioDebug,c20ms,6);
#endif

    sSysTaskInit();
    sRTOSTaskStart();


    for(;;)
    {
        //Task schedule
        (*Task_Struct_Table[sRTOSFindHighPrioRdyTask()].pTaskFuncAddr)();
    }
}


void CAN_loopback_test(void)
{
   uint16_t i = 0;
   uint8_t buf[OPTION_REG_SIZE*2] = {0};

   for(i = 0;i<OPTION_REG_SIZE*2;i+=2)
   {
      buf[i+1] = i/2;
   }

   CAN_MultiREG_Master(0x05,OPT_REG_BASE,REG_WRITE,OPTION_REG_SIZE,buf,TIME_LEVEL1);
//
   memset(buf,0,OPTION_REG_SIZE*2);
//
   CAN_MultiREG_Master(0x05,OPT_REG_BASE,REG_READ,OPTION_REG_SIZE,buf,TIME_LEVEL1);
//
   memset(buf,0,OPTION_REG_SIZE*2);
   
   for(i = 0;i<STATE_REG_SIZE;i++)
   {
       uStateData.databuf[i] = 2*i;
   }


   CAN_MultiREG_Master(0x05,STATE_REG_BASE,REG_READ,STATE_REG_SIZE,buf,TIME_LEVEL1);

   memset(buf,0,CONCTROL_REG_SIZE*2);

}


void sDataInit(void)
{
    sShareDataInit();
    sPfcControllerInit();
}

void sSysTaskInit(void)
{
    sSuperTaskInit();
    sDebugTaskInit();
}


void sOnboardLedTask(void)
{
    uint16_t event;

    event = suwRTOSGetEvent(15);     //Get the current task event

    if(event & ((uint16_t)1 << 0))
    {
        if(u16_PwmTestOn == 1)
        {
            u16_PwmTestOn = 0;
            sRTOSEventSend(cPrioSuper,eSuperTunOnEvt);
        }
        else if(u16_PwmTestOn == 2)
        {
            u16_PwmTestOn = 0;
            sRTOSEventSend(cPrioSuper,eSuperTunOffEvt);
        }

        if((stSuper.u16_SysMode == cPowerOnMode) || (stSuper.u16_SysMode == cStandbyMode))
        {
            if(++u16_TestTaskCnt >= 25)
            {
                u16_TestTaskCnt = 0;
                mOnBoardLed_Toggle();
            }
        }
        else if(stSuper.u16_SysMode == cSoftStartMode)
        {
            if(++u16_TestTaskCnt >= 12)
            {
                u16_TestTaskCnt = 0;
                mOnBoardLed_Toggle();
            }
        }
        else if(stSuper.u16_SysMode == cCCMode)
        {
            if(++u16_TestTaskCnt <= 25)
            {
                //u16_TestTaskCnt = 0;
                mOnBoardLed_Off();
            }
            else if(u16_TestTaskCnt <= 75)
            {
                mOnBoardLed_On();
            }
            else
            {
                u16_TestTaskCnt = 0;
            }
        }
        else if(stSuper.u16_SysMode == cCVMode)
        {
            if(++u16_TestTaskCnt <= 50)
            {
                //u16_TestTaskCnt = 0;
                mOnBoardLed_On();
            }
            else if(u16_TestTaskCnt <= 100)
            {
                mOnBoardLed_Off();
            }
            else
            {
                u16_TestTaskCnt = 0;
            }
        }
        else if(stSuper.u16_SysMode == cCPMode)
        {
            if(++u16_TestTaskCnt <= 25)
            {
                //u16_TestTaskCnt = 0;
                mOnBoardLed_On();
            }
            else if(u16_TestTaskCnt <= 75)
            {
                mOnBoardLed_Off();
            }
            else
            {
                u16_TestTaskCnt = 0;
            }
        }
        else
        {
            mOnBoardLed_On();
        }


    }
}



