/*
 * DebugTask.h
 *
 *  Created on: 2024Äê10ÔÂ17ÈÕ
 *      Author: dell
 */

#ifndef APP_DEBUGTASK_H_
#define APP_DEBUGTASK_H_

//Super task definition
#define    cPrioDebug                     2
#define    eDebugTimerEvt                 0

extern void sDebugTaskInit(void);
extern void sDebugTask(void);

#endif /* APP_DEBUGTASK_H_ */
