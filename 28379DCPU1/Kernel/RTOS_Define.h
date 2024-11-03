/******************************************************************************
 * File Name: RTOS_Define.h
 * Description: The common used definition for the RTOS
 * Author:	
 * Version: 00
 * Date:
 * ***************************************************************************/
#define   cUserMaxTask		16		//The maximum tasks can be up to 16
#define   cIdleTaskEnable	0		//0: Idle Task Disabled. 1: Enabled

#if    cIdleTaskEnable == 1
#define	  cMaxTask			(cUserMaxTask+1)
#else
#define   cMaxTask			cUserMaxTask
#endif

#define   mRTOS_INT_ENABLE()	{EINT;}
#define	  mRTOS_INT_DISABLE()	{DINT;}

//The task priority
#define	  cTaskPrio_0		0
#define	  cTaskPrio_1		1
#define	  cTaskPrio_2		2
#define	  cTaskPrio_3		3		
#define	  cTaskPrio_4		4
#define	  cTaskPrio_5		5
#define	  cTaskPrio_6		6
#define	  cTaskPrio_7		7
#define	  cTaskPrio_8		8
#define	  cTaskPrio_9		9
#define	  cTaskPrio_10		10
#define	  cTaskPrio_11		11		
#define	  cTaskPrio_12		12
#define	  cTaskPrio_13		13
#define	  cTaskPrio_14		14
#define	  cTaskPrio_15		15

//The task timer constant
#define   c1ms				1
#define	  c2ms				2
#define	  c5ms				5
#define   c10ms				10
#define	  c20ms				20
#define	  c50ms				50
#define   c100ms			100
#define   c1sec				1000

//The Task Block Define
typedef struct
{
    void 		(*pTaskFuncAddr)();
    uint16_t 	uwTaskPrio;
    uint16_t      uwTaskTimerPeriod;
    uint16_t      uwTaskTimerCnt;
    uint16_t      uwTaskEvent;
    uint16_t      uwTaskEventMask;
}TASK_STRUCT;

#define eTimerEventId       0

extern TASK_STRUCT Task_Struct_Table[cMaxTask];
extern uint16_t uwTaskRdyList;



