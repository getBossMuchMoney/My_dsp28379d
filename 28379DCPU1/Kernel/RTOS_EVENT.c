/******************************************************************************
 * File Name: RTOS_EVENT.C
 * Description: The event functions
 * Author:	
 * Version: 00
 * Date:
 * ***************************************************************************/
#include "F28x_Project.h"
#include "RTOS_Define.h"

extern TASK_STRUCT Task_Struct_Table[cMaxTask];
extern uint16_t uwTaskRdyList;
void sRTOSEventSend(uint16_t uwTaskPrio,uint16_t uwEventId);
void sRTOSEventSendIsr(uint16_t uwTaskPrio,uint16_t uwEventId);
void sRTOSEventMask(uint16_t uwTaskPrio,uint16_t uwEventId);
uint16_t suwRTOSGetEvent(uint16_t uwTaskPrio);



/******************************************************************************
 * Function Name: sRTOSEventSend
 * Description: Send the event to the corresponding task
 * Input: uwTaskPrio -- The task priority
 *  	  uwEventId -- The Event for the task
 * Output: None
 * Return: None
 * ***************************************************************************/
void sRTOSEventSend(uint16_t uwTaskPrio,uint16_t uwEventId)
{
 	mRTOS_INT_DISABLE();
 	if((((uint16_t)0x1<<uwEventId) & (Task_Struct_Table[uwTaskPrio].uwTaskEventMask)) != 0)
 	{	
 		Task_Struct_Table[uwTaskPrio].uwTaskEvent |= ((uint16_t)0x1<<uwEventId);
 		uwTaskRdyList |= ((uint16_t)0x1<<uwTaskPrio);
 	}
 	mRTOS_INT_ENABLE();
}

/******************************************************************************
 * Function Name: sRTOSEventSendIsr
 * Description: Send the event to the corresponding task in the ISR 
 * Input: uwTaskPrio -- The task priority
 *  	  uwEventId -- The Event for the task
 * Output: None
 * Return: None
 * ***************************************************************************/
void sRTOSEventSendIsr(uint16_t uwTaskPrio,uint16_t uwEventId)
{
 	if((((uint16_t)0x1<<uwEventId) & (Task_Struct_Table[uwTaskPrio].uwTaskEventMask)) != 0)
 	{	
 		Task_Struct_Table[uwTaskPrio].uwTaskEvent |= ((uint16_t)0x1<<uwEventId);
 		uwTaskRdyList |= ((uint16_t)0x1<<uwTaskPrio);
 	}
}

/******************************************************************************
 * Function Name: suwRTOSGetEvent
 * Description: Get the event of the certain task
 * Input: uwTaskPrio -- The task priority
 * Output: None
 * Return: The certain task event
 * ***************************************************************************/
uint16_t suwRTOSGetEvent(uint16_t uwTaskPrio)
{
	uint16_t uwEventTemp;
	
	mRTOS_INT_DISABLE();
	uwEventTemp = Task_Struct_Table[uwTaskPrio].uwTaskEvent;
	Task_Struct_Table[uwTaskPrio].uwTaskEvent &= 0x0000;
	mRTOS_INT_ENABLE();
	
	return(uwEventTemp);
}

/******************************************************************************
 * Function Name: sRTOSEventMask
 * Description: Mask the certain event when needed
 * Input: uwTaskPrio -- The task priority
 *  	  uwEventId -- The Event for the task
 * Output: None
 * Return: None
 * ***************************************************************************/
void sRTOSEventMask(uint16_t uwTaskPrio,uint16_t uwEventId)
{
	uint16_t uwMaskTemp;
	
	uwMaskTemp = Task_Struct_Table[uwTaskPrio].uwTaskEventMask;
	uwMaskTemp &= (~((uint16_t)0x1<<uwEventId));
	
	mRTOS_INT_DISABLE();
	Task_Struct_Table[uwTaskPrio].uwTaskEventMask = uwMaskTemp;
	mRTOS_INT_ENABLE();
	
}


 
 
//
