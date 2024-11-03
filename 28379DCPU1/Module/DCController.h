/*******************************************************************************
 * File Name: Cla_User.c
 * Description: The ePWM initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _DCCONTROLLER_H
#define _DCCONTROLLER_H

#define  cBuck_I_Ratio            0.09768f
#define  cOutput_I_Ratio          0.1954f
#define  cOutput_V_Ratio          0.2422f

#define   cDcDutyMax        0.95f
#define   cDcDutyMin        0.0f

extern __interrupt void Cla1Task1 (void);
extern __interrupt void Cla1Task2 ( void );
extern __interrupt void Cla1Task3 ( void );
extern __interrupt void Cla1Task4 ( void );
extern __interrupt void Cla1Task5 ( void );
extern __interrupt void Cla1Task6 ( void );
extern __interrupt void Cla1Task7 ( void );
extern __interrupt void Cla1Task8 ( void );

#endif


