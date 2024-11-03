/*
 * CAN_User.h
 *
 *  Created on: 2024��10��22��
 *      Author: RUI_HENG
 */

#ifndef DRIVER_CAN_USER_H_
#define DRIVER_CAN_USER_H_

#include "gpio.h"
#include "pin_map.h"
#include "hw_memmap.h"
#include "can.h"
#include "interrupt.h"

typedef enum
{
    BROADCAST_ID = 0,
    SLAVER1_ID,
    SLAVER2_ID,
    SLAVER3_ID,
    SLAVER4_ID
} ID_manage;

typedef enum
{
    WORKSTART = 0,
    MASTER,
    SLAVER

} WORK_MODE;

typedef enum
{
    STA_OK = 0,
    STA_FUNC_ERR,
    STA_ADDR_ERR,
    STA_CMD_ERR

} STATE_CODE;

// #pragma pack(push, 2)
// #pragma pack(pop)

//
// 20MHz XTAL on controlCARD. For use with SysCtl_getClock().
//
#define DEVICE_OSCSRC_FREQ 20000000U
#define DEVICE_SYSCLK_FREQ ((DEVICE_OSCSRC_FREQ * 20 * 1) / 2)

#define MSG_DATA_LENGTH 8
#define TX_MSG_OBJ_ID 1
#define RX_MSG_OBJ_ID 2
#define BROD_MSG_OBJ_ID 3

typedef enum
{
    CAN_OK = 0,
    CAN_SEND_TIMEOUT,
    CAN_RECV_TIMEOUT,
    CAN_CMD_ERR,
    CAN_PARAM_ERR,

} CAN_ERRCODE;

// unit:ms
typedef enum
{
    TIME_LEVEL1 = 50,
    TIME_LEVEL2 = 100,
    TIMR_LEVEL3 = 150,

} TIMEOUT_SET;

void CAN_Init(void);
void Slaver_command_deal(uint32_t msgID);
CAN_ERRCODE CAN_MultiREG_Master(uint32_t target_ID, uint16_t addr, uint32_t func_code, uint32_t reg_num, uint8_t *pdata, TIMEOUT_SET timeout);
CAN_ERRCODE CAN_SendData_Master(uint32_t target_ID, uint16_t addr, uint32_t func_code, uint32_t reg_num, uint8_t *pdata, TIMEOUT_SET timeout);
CAN_ERRCODE CAN_SendReadData_Master(uint32_t target_ID, uint16_t addr, uint32_t func_code, uint32_t reg_num,
                                    uint8_t *ptxdata, uint8_t *prxdata, TIMEOUT_SET timeout);
CAN_ERRCODE CAN_SendData_Slaver(uint32_t target_ID, uint32_t func_code, STATE_CODE sta_code, uint32_t reg_num, uint8_t *pdata, TIMEOUT_SET timeout);

#endif /* DRIVER_CAN_USER_H_ */
