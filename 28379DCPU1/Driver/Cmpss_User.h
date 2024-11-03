/*******************************************************************************
 * File Name: Cmpss_User.h
 * Description: The CMPSS initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _CMPSS_USER_H
#define _CMPSS_USER_H

//definitions for selecting DACH reference
#define REFERENCE_VDDA     0
#define REFERENCE_VDAC     1
//definitions for COMPH input selection
#define NEGIN_DAC          0
#define NEGIN_PIN          1
//definitions for CTRIPH/CTRIPOUTH output selection
#define CTRIP_ASYNCH       0
#define CTRIP_SYNCH        1
#define CTRIP_FILTER       2
#define CTRIP_LATCH        3

#define AC_CURRENT_DAC_HIGH  4000
#define AC_CURRENT_DAC_LOW   95

#define DC_CURRENT_DAC_HIGH  4000
#define DC_CURRENT_DAC_LOW   95

extern void sCmpssInit(void);

#endif




