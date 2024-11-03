/******************************************************************************
 * File Name: RTOS_APP.h
 * Description: The common used RTOS macro call by Application layer
 * Author:	
 * Version: 00
 * Date:
 * ***************************************************************************/

#ifndef RTOS_APP_H_
#define RTOS_APP_H_



extern void sRTOSTimerTicker(void);
extern void sRTOSEventSend(uint16_t uwTaskPrio,uint16_t uwEventId);
extern void sRTOSEventSendIsr(uint16_t uwTaskPrio,uint16_t uwEventId);
extern void sRTOSEventMask(uint16_t uwTaskPrio,uint16_t uwEventId);
extern uint16_t suwRTOSGetEvent(uint16_t uwTaskPrio);

extern void   sRTOSInit(void);
extern void   sRTOSTaskCreate(void (*pTask)(),uint16_t uwPrio, uint16_t uwPeriod, uint16_t uwCnt);
extern void   sRTOSTaskStart();
extern uint16_t sRTOSFindHighPrioRdyTask(void);
extern void   sRTOSTaskDefault();
extern void   sRTOSPendTask(uint16_t uwPrio);
extern void   sRTOSRdyTask(uint16_t uwPrio);

extern uint16_t    uwTaskInternalInit;


#endif /*RTOS_APP_H_*/
