/******************************************************************************
 * File Name: RTOS_TASK.C
 * Description: The common used definition for the RTOS
 * Author:	
 * Version: 00
 * Date:
 * ***************************************************************************/
#include "F28x_Project.h"
#include "RTOS_Define.h"
//-------------------------Viriable definition begin-------------------------------/

//The Task Block Table
TASK_STRUCT Task_Struct_Table[cMaxTask];

//The flag for finishing the internal running enviroment initialization
uint16_t    uwTaskInternalInit;
uint16_t    uwTaskRdyList;
uint16_t    uwTaskCurrPrio;
//-------------------------Viriable definition end-------------------------------/


//-------------------------Function definition begin-------------------------------/
void   sRTOSInit(void);
void   sRTOSTaskCreate(void (*pTask)(),uint16_t uwPrio, uint16_t uwPeriod, uint16_t uwCnt);
void   sRTOSTaskStart(void);
uint16_t sRTOSFindHighPrioRdyTask(void);
void   sRTOSTaskDefault();
void   sRTOSPendTask(uint16_t uwPrio);
void   sRTOSRdyTask(uint16_t uwPrio);

#ifdef   cIdleTaskEnable
void sRTOSIdleTask();
#endif
//-------------------------Function definition end-------------------------------/

/******************************************************************************
 * Function Name: sRTOSInit
 * Description: Initialize RTOS running environment
 * Input: None
 * Output: None
 * Return: None
 * ***************************************************************************/
void sRTOSInit(void)
{
    int16_t i;

    for(i=0;i<cMaxTask;i++)
    {
        Task_Struct_Table[i].pTaskFuncAddr = sRTOSTaskDefault;
        Task_Struct_Table[i].uwTaskPrio = 0xF;
        Task_Struct_Table[i].uwTaskTimerCnt = 0;
        Task_Struct_Table[i].uwTaskTimerPeriod = 0;
        Task_Struct_Table[i].uwTaskEvent = 0x0000;
        Task_Struct_Table[i].uwTaskEventMask = 0xffff;
    }

    uwTaskInternalInit = 0x0000; //Clear the task internal initialzation flag;
    uwTaskRdyList = 0x0000; 	//Clear the task ready list
    uwTaskCurrPrio = 0;
}

void sRTOSTaskDefault()
{
	asm(" nop");
	return;
}

/******************************************************************************
 * Function Name: sRTOSTaskCreate
 * Description: Initialize RTOS running environment
 * Input: pTask---The task access address
 *        uwPrio--The task priority
 *        uwPeriod--The task timer event period
 *        uwCnt--The task timer cnt initial value
 * Output: The initilized the task talbe
 * Return: None
 * ***************************************************************************/
void sRTOSTaskCreate(void (*pTask)(),uint16_t uwPrio, uint16_t uwPeriod, uint16_t uwCnt)
{
     Task_Struct_Table[uwPrio].pTaskFuncAddr = pTask;
     Task_Struct_Table[uwPrio].uwTaskPrio = uwPrio;
     Task_Struct_Table[uwPrio].uwTaskTimerCnt = uwCnt;
     Task_Struct_Table[uwPrio].uwTaskTimerPeriod = uwPeriod;
     Task_Struct_Table[uwPrio].uwTaskEvent = 0x0000;
     Task_Struct_Table[uwPrio].uwTaskEventMask = 0xffff;
}

/******************************************************************************
 * Function Name: sRTOSTaskStart
 * Description: Start all the task, enable the all the interrupt.
 * Input: none
 * Output:none
 * Return: None
 * ***************************************************************************/
void sRTOSTaskStart(void)
{
 	int16_t  i;
 	uwTaskInternalInit = 0x0000; 
 	uwTaskRdyList = 0x0000;
 	
    for(i=0;i<cMaxTask;i++)    
    {
    	uwTaskRdyList |= (0x1<<i); 	//Clear the task ready list
    }
    
    EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM 
}

/******************************************************************************
 * Function Name: sRTOSFindHighPrioRdyTask
 * Description: Find the highest priority task in the rdy list
 * Input: none
 * Output:none
 * Return: The highest priority of the task
 * ***************************************************************************/
uint16_t sRTOSFindHighPrioRdyTask(void)
{
 	int16_t i;
 	uint16_t uwRdyListTemp;
 	uint16_t uwTaskPrioTemp;
 	
 	mRTOS_INT_DISABLE();
 	uwRdyListTemp = uwTaskRdyList;
 	mRTOS_INT_ENABLE();
 	
 	for(i=0;i<cMaxTask;i++)
 	{
 		if((uwRdyListTemp & ((uint16_t)0x1<<i)) != 0)
 		{
 	        uwTaskPrioTemp = i;
 	        break;
 		}
 		else
 		{
 			uwTaskPrioTemp = (cMaxTask - 1);
 		}
 	}
 	
 	mRTOS_INT_DISABLE();
 	uwTaskRdyList &= (~((uint16_t)0x1<<uwTaskPrioTemp));
 	mRTOS_INT_ENABLE();
 	
 	
 	uwTaskCurrPrio = uwTaskPrioTemp;
 	return(uwTaskPrioTemp); //If there is no task ready, return the lowest
}

void   sRTOSPendTask(uint16_t uwPrio)
{
	mRTOS_INT_DISABLE();
 	uwTaskRdyList &= (~((uint16_t)0x1<<uwPrio));
 	mRTOS_INT_ENABLE();
}

void   sRTOSRdyTask(uint16_t uwPrio)
{
	mRTOS_INT_DISABLE();
 	uwTaskRdyList |=((uint16_t)0x1<<uwPrio);
 	mRTOS_INT_ENABLE();
}
 
#ifdef cIdleTaskEnable
void sRTOSIdleTask(void)
{
 	
}
#endif


