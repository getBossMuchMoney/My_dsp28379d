/*******************************************************************************
 * File Name: Cla_User.h
 * Description: The ePWM initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _CLA_USER_H
#define _CLA_USER_H

extern void CLA_configClaMemory(void);
extern void CLA_initCpu1Cla1(void);
extern __interrupt void Cla1Task1 (void);
extern __interrupt void Cla1Task2 ( void );
extern __interrupt void Cla1Task3 ( void );
extern __interrupt void Cla1Task4 ( void );
extern __interrupt void Cla1Task5 ( void );
extern __interrupt void Cla1Task6 ( void );
extern __interrupt void Cla1Task7 ( void );
extern __interrupt void Cla1Task8 ( void );

#endif




