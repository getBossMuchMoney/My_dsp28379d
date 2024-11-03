/*******************************************************************************
 * File Name: SuperTask.c
 * Description: The system status machine.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef APP_SUPERTASK_H_
#define APP_SUPERTASK_H_

//Super task definition
#define    cPrioSuper                     1
#define    eSuperTimerEvt                 0
#define    eSuperTunOnEvt                 1
#define    eSuperTunOffEvt                2
#define    eSuperFaultEvt                 3

#define    cSuper1s                       50
#define    cSuper2s                       100
#define    cSuper5s                       250

#define    cPowerOnMode       0
#define    cStandbyMode       1
#define    cSoftStartMode     2
#define    cCCMode            3
#define    cCVMode            4
#define    cCPMode            5
#define    cFaultMode         6

#define    cPwmOn             1
#define    cPwmOff            0
#define    cRlyOn             1
#define    cRlyOff            0
#define    cFanOn             1
#define    cFanOff            0

#define    OP_CC              0
#define    OP_CV              1
#define    OP_CP              2

#define    cPreChargeVolt     4000
#define    cSoftStartVolt     7000
#define    cBusNormalStep     50

typedef union {
    uint32_t all;
    struct {
        uint32_t HardwareFault:1;
        uint32_t SoftStartFail:1;
        uint32_t GridVoltHigh:1;
        uint32_t GridVoltLow:1;
        uint32_t GridFreqHigh:1;
        uint32_t GridFreqLow:1;
        uint32_t IpCurrOCP:1;
        uint32_t OpCurrOCP:1;
        uint32_t OpVoltOVP:1;
        uint32_t BusOVP:1;
        uint32_t BusUVP:1;
        uint32_t OverTemp1:1;
        uint32_t OverTemp2:1;
        uint32_t OverTemp3:1;
        uint32_t OverTemp4:1;
        uint32_t FanFault:1;
        uint32_t Resv:16;
    }BIT;
}FAULT_BIT;

typedef union{
    uint16_t all;
    struct {
        uint16_t Pfc1PwmStatus:1;
        uint16_t Pfc2PwmStatus:1;
        uint16_t Dc1PwmStatus:1;
        uint16_t Dc2PwmStatus:1;
        uint16_t IpSoftRly:1;
        uint16_t OpSoftRly:1;
        uint16_t IpMainRly:1;
        uint16_t OpMainRly:1;
        uint16_t FanStatus:1;

        uint16_t BusPreCharge:1;
        uint16_t GridVoltNormal:1;
        uint16_t Rsrv:6;
    }BIT;
}SYS_BIT;

typedef struct {
    uint16_t    u16_SysMode;
    uint16_t    u16_PowerOnModeCnt;
    uint16_t    u16_StandbyCnt;
    uint16_t    u16_SoftStartModeCnt;
    uint16_t    u16_OutputMode;
    uint16_t    u16_PhysicalAddr;
    SYS_BIT     u16_SysStatus;
    FAULT_BIT   u32_FaultCode;
    uint32_t    u32_SwVersion;
}SUPER_ST;

extern SUPER_ST stSuper;
extern void sSuperTaskInit(void);
extern void sSuperTask(void);

#endif /* APP_SUPERTASK_H_ */
