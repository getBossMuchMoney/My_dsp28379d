/*******************************************************************************
 * File Name: Modbus_RTU.c
 * Description: The CMPSS initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/

#include "Modbus_RTU.h"

U_StateData uStateData;
U_ControlData uControlData;
U_OptionData uOptionData;



void Modbus_Master(uint8_t Station,uint16_t  ADDR,uint8_t func_code,uint8_t pdata,uint16_t length, pSendFunc pSend)
{

}

void Modbus_Slaver(uint16_t  ADDR,uint8_t func_code,uint16_t reg_num, pSendFunc pSend)
{


}



void read_REG(uint16_t addr_base, uint16_t offset, uint16_t reg_num, uint8_t *pdata)
{
    uint8_t i = 0;
    switch (addr_base)
    {

    case STATE_REG_BASE:
    {
        for (i = 0; i < reg_num * 2; i += 2)
        {
            *(pdata + i) = (uStateData.databuf[offset + i / 2]) >> 8;
            *(pdata + i + 1) = (uStateData.databuf[offset + i / 2]) & 0x00FF;
        }
    }
    break;

    case OPT_REG_BASE:
    {
        for (i = 0; i < reg_num * 2; i += 2)
        {
            *(pdata + i) = (uOptionData.databuf[offset + i / 2]) >> 8;
            *(pdata + i + 1) = (uOptionData.databuf[offset + i / 2]) & 0x00FF;
        }
    }
    break;

    default:
    {
    }
    break;
    }
}

void write_REG(uint16_t addr_base, uint16_t offset, uint16_t reg_num, uint8_t *pdata)
{
    uint8_t i = 0;
    switch (addr_base)
    {

    case CTRL_REG_BASE:
    {
        for (i = 0; i < reg_num * 2; i += 2)
        {
            uControlData.databuf[offset + i / 2] = ((*(pdata + i)) << 8) | (*(pdata + i + 1));
        }
    }
    break;

    case OPT_REG_BASE:
    {
        for (i = 0; i < reg_num * 2; i += 2)
        {
            uOptionData.databuf[offset + i / 2] = ((*(pdata + i)) << 8) | (*(pdata + i + 1));
        }
    }
    break;

    default:
    {
    }
    break;
    }
}

void conctrl_scan(void)
{
    uint8_t i = 0;
    for (i = 0; i < 11; i++)
    {
        if (uControlData.databuf[i] == 1)
        {
            uControlData.databuf[i] = 0;
            conctrl_command_deal((CTRL_KIND)i);
        }
    }
}

void conctrl_command_deal(CTRL_KIND i)
{
    switch (i)
    {
    case C_START:
    {
    }
    break;

    case C_CLOSE:
    {
    }
    break;

    case C_PFC1OPD_EN:
    {
    }
    break;

    case C_PFC2OPD_EN:
    {
    }
    break;

    case C_DC1OPD_EN:
    {
    }
    break;

    case C_DC2OPD_EN:
    {
    }
    break;

    case C_IOUTCAL_EN:
    {
    }
    break;

    case C_VOUTCAL_EN:
    {
    }
    break;

    case C_CLEAN_ERR:
    {
    }
    break;

    case C_DATASAVE:
    {
    }
    break;

    case C_IAP_EN:
    {
    }
    break;

    default:
    {
    }
    break;
    }
}

