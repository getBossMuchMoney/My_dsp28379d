/*******************************************************************************
 * File Name: app.h
 * Description: The system setting and the system macros definition
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _APP_H
#define _APP_H

#define    cPWM_OpenLoop_Test   1
#define    cSW_VERSION          100000001L


extern void sMcuInitial(void);
extern interrupt void sADCA1_isr(void);
extern interrupt void sADCD1_isr(void);
extern interrupt void sRtosTimerInterruptIsr(void);
extern interrupt void sSciaRxIsr(void);
extern interrupt void sSciaTxIsr(void);
#endif


