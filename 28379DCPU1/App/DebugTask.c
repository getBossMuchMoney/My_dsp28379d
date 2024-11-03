/*******************************************************************************
 * File Name: DebugTask.c
 * Description: The uart debug task.
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
#include "DebugTask.h"
#include "UartDebug.h"

void sDebugTaskInit(void);
void sDebugTask(void);

uint16_t u16_debugtasktest;
void sDebugTaskInit(void)
{
    sInitialSci(0,0);
#if (cDataBufEn == 1)
    sDataLogInit();
#endif
}
void sDebugTask(void)
{
    uint16_t event;

   event = suwRTOSGetEvent(cPrioDebug);     //Get the current task event

   if(event & ((uint16_t)1 << eDebugTimerEvt))
   {
       sRs232Chk(0);
       if(++u16_debugtasktest >= 25)
       {
           u16_debugtasktest = 0;
           mLedRed_Toggle();
       }
   }
}
