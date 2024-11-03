/******************************************************************************
 * File Name: RTOS_TIMER.C
 * Description: The timer ticker function
 * Author:	
 * Version: 00
 * Date:
 * ***************************************************************************/
#include "F28x_Project.h"
#include "RTOS_Define.h"

extern TASK_STRUCT Task_Struct_Table[cMaxTask];
void sRTOSTimerTicker(void);
extern void sRTOSEventSendIsr(uint16_t uwTaskPrio,uint16_t uwEventId);

/******************************************************************************
 * Function Name: sRTOSTimerTicker
 * Description: Generate the timer event for each task.
 * Input: none
 * Output:none
 * Return: The highest priority of the task
 * ***************************************************************************/
//uint32_t u32_CPUtimercnt0,u32_CPUtimercnt1;
#ifdef _FLASH
#pragma CODE_SECTION(sRTOSTimerTicker, ".TI.ramfunc");
#endif
void sRTOSTimerTicker(void)
{
    //u32_CPUtimercnt0 = CpuTimer0Regs.TIM.all;
	uint16_t  i;
	
	for(i=0;i<cMaxTask;i++)
	{
		if((Task_Struct_Table[i].uwTaskTimerCnt++) >=
		        (Task_Struct_Table[i].uwTaskTimerPeriod) &&
		        (Task_Struct_Table[i].uwTaskTimerPeriod != 0))
		{
			Task_Struct_Table[i].uwTaskTimerCnt = 0;
			sRTOSEventSendIsr(i,eTimerEventId);
		}
	}
	//u32_CPUtimercnt1 = u32_CPUtimercnt0 - CpuTimer0Regs.TIM.all;
}
