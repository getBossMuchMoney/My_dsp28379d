/*
 * Modbus_RTU.h
 *
 *  Created on: 2024��10��28��
 *      Author: RUI_HENG
 */

#ifndef MODULE_MODBUS_RTU_H_
#define MODULE_MODBUS_RTU_H_

#include "F28x_Project.h"
#include "hw_types.h"

#define STATE_REG_BASE 0x00
#define CTRL_REG_BASE 0x100
#define OPT_REG_BASE 0x200

#define REG_WRITE 0X10
#define REG_READ 0X03

typedef void (*pSendFunc)(uint8_t *,uint16_t);

typedef struct
{
  uint16_t u16Vab;
  uint16_t u16Vbc;
  uint16_t u16Vca;
  uint16_t u16Ia;
  uint16_t u16Ib;
  uint16_t u16Ic;
  uint16_t u16Iai1;
  uint16_t u16Ibi1;
  uint16_t u16Ici1;
  uint16_t u16Iai2;
  uint16_t u16Ibi2;
  uint16_t u16Ici2;
  uint16_t u16Vbus;
  uint16_t u16VbusP;
  uint16_t u16VbusN;
  int16_t i16Idci1;
  int16_t i16Idci2;
  uint16_t u16freq;
  uint16_t reg16Bak18;
  uint16_t reg16Bak19;
  uint16_t reg16Bak20;
  uint16_t reg16Bak21;
  uint16_t reg16Bak22;
  int16_t i16Idcout;
  uint16_t u16Vout;
  int16_t i16Temp1;
  int16_t i16Temp2;
  int16_t i16Temp3;
  int16_t i16Temp4;
  uint16_t reg_bak29;
  uint16_t u16Addr;
  uint16_t u16WorkMode;
  uint16_t u16SysState;

  uint16_t u32ErrCode_L;
  uint16_t u32ErrCode_H;

  uint16_t u32Version_L;
  uint16_t u32Version_H;

  uint16_t reg32Bak35_L;
  uint16_t reg32Bak35_H;

  uint16_t reg32Bak36_L;
  uint16_t reg32Bak36_H;

  uint16_t reg32Bak37_L;
  uint16_t reg32Bak37_H;

  uint16_t reg32Bak38_L;
  uint16_t reg32Bak38_H;

  uint16_t reg32Bak39_L;
  uint16_t reg32Bak39_H;

  uint16_t reg32Bak40_L;
  uint16_t reg32Bak40_H;

  uint16_t reg32Bak41_L;
  uint16_t reg32Bak41_H;

} State_REG_t;

#define STATE_REG_SIZE sizeof(State_REG_t)

typedef struct
{
  uint16_t u16Start;
  uint16_t u16Close;
  uint16_t u16Pfc1OPDEN;
  uint16_t u16Pfc2OPDEN;
  uint16_t u16DC1OPDEN;
  uint16_t u16DC2OPDEN;
  uint16_t u16IoutCalEN;
  uint16_t u16VoutCalEN;
  uint16_t u16CleanErr;
  uint16_t u16DataSave;
  uint16_t u16IapEN;
  uint16_t reg16Bak11;
  uint16_t reg16Bak12;
  uint16_t reg16Bak13;
  uint16_t reg16Bak14;
  uint16_t reg16Bak15;
  uint16_t reg16Bak16;
  uint16_t reg16Bak17;
  uint16_t reg16Bak18;
  uint16_t reg16Bak19;

} Control_REG_t;

#define CONCTROL_REG_SIZE sizeof(Control_REG_t)

typedef struct
{
  uint16_t u16MaxIout;
  uint16_t u16MaxVout;
  uint16_t u16MaxPower;
  uint16_t u16OutputMode;
  uint16_t u16SlopeIcc;
  uint16_t u16SlopeVcv;
  uint16_t u16SlopePcp;
  uint16_t u16RefValCCal1;
  uint16_t u16ActValCCal1;
  uint16_t u16RefValCCal2;
  uint16_t u16ActValCCal2;
  uint16_t u16RefValCCal3;
  uint16_t u16ActValCCal3;
  uint16_t u16RefValCCal4;
  uint16_t u16ActValCCal4;
  uint16_t u16RefValCCal5;
  uint16_t u16ActValCCal5;
  uint16_t u16RefValCCal6;
  uint16_t u16ActValCCal6;
  uint16_t u16RefValCCal7;
  uint16_t u16ActValCCal7;
  uint16_t u16RefValCCal8;
  uint16_t u16ActValCCal8;
  uint16_t u16RefValUCal1;
  uint16_t u16ActValUCal1;
  uint16_t u16RefValUCal2;
  uint16_t u16ActValUCal2;
  uint16_t u16RefValUCal3;
  uint16_t u16ActValUCal3;
  uint16_t u16RefValUCal4;
  uint16_t u16ActValUCal4;
  uint16_t u16RefValUCal5;
  uint16_t u16ActValUCal5;
  uint16_t u16RefValUCal6;
  uint16_t u16ActValUCal6;
  uint16_t u16RefValUCal7;
  uint16_t u16ActValUCal7;
  uint16_t u16RefValUCal8;
  uint16_t u16ActValUCal8;
  uint16_t reg16Bak39;
  uint16_t reg16Bak40;
  uint16_t reg16Bak41;
  uint16_t reg16Bak42;
  uint16_t reg16Bak43;
  uint16_t reg16Bak44;
  uint16_t reg16Bak45;
  uint16_t reg16Bak46;
  uint16_t reg16Bak47;

  uint16_t i32CtrlK1_L;
  uint16_t i32CtrlK1_H;

  uint16_t i32CtrlK2_L;
  uint16_t i32CtrlK2_H;

  uint16_t i32CtrlK3_L;
  uint16_t i32CtrlK3_H;

  uint16_t i32CtrlK4_L;
  uint16_t i32CtrlK4_H;

  uint16_t i32CtrlK5_L;
  uint16_t i32CtrlK5_H;

  uint16_t i32CtrlK6_L;
  uint16_t i32CtrlK6_H;

  uint16_t i32CtrlK7_L;
  uint16_t i32CtrlK7_H;

  uint16_t i32CtrlK8_L;
  uint16_t i32CtrlK8_H;

  uint16_t i32CtrlK9_L;
  uint16_t i32CtrlK9_H;

  uint16_t i32CtrlK10_L;
  uint16_t i32CtrlK10_H;

  uint16_t i32CtrlK11_L;
  uint16_t i32CtrlK11_H;

  uint16_t i32CtrlK12_L;
  uint16_t i32CtrlK12_H;

  uint16_t i32CtrlK13_L;
  uint16_t i32CtrlK13_H;

  uint16_t i32CtrlK14_L;
  uint16_t i32CtrlK14_H;

  uint16_t i32CtrlK15_L;
  uint16_t i32CtrlK15_H;

  uint16_t i32CtrlK16_L;
  uint16_t i32CtrlK16_H;

  uint16_t i32CtrlK17_L;
  uint16_t i32CtrlK17_H;

  uint16_t i32CtrlK18_L;
  uint16_t i32CtrlK18_H;

  uint16_t i32CtrlK19_L;
  uint16_t i32CtrlK19_H;

  uint16_t i32CtrlK20_L;
  uint16_t i32CtrlK20_H;

} Option_REG_t;

#define OPTION_REG_SIZE sizeof(Option_REG_t)

typedef union
{
  State_REG_t REG;
  uint16_t databuf[STATE_REG_SIZE];
} U_StateData;

typedef union
{
  Control_REG_t REG;
  uint16_t databuf[CONCTROL_REG_SIZE];
} U_ControlData;

typedef union
{
  Option_REG_t REG;
  uint16_t databuf[OPTION_REG_SIZE];
} U_OptionData;

typedef enum
{
  C_START = 0,
  C_CLOSE,
  C_PFC1OPD_EN,
  C_PFC2OPD_EN,
  C_DC1OPD_EN,
  C_DC2OPD_EN,
  C_IOUTCAL_EN,
  C_VOUTCAL_EN,
  C_CLEAN_ERR,
  C_DATASAVE,
  C_IAP_EN

} CTRL_KIND;

extern U_StateData uStateData;
extern U_ControlData uControlData;
extern U_OptionData uOptionData;

void Modbus_Master(uint8_t Station,uint16_t  ADDR,uint8_t func_code,uint8_t pdata,uint16_t length, pSendFunc pSend);
void Modbus_Slaver(uint16_t  ADDR,uint8_t func_code,uint16_t reg_num, pSendFunc pSend);
void read_REG(uint16_t addr_base, uint16_t offset, uint16_t reg_num, uint8_t *pdata);
void write_REG(uint16_t addr_base, uint16_t offset, uint16_t reg_num, uint8_t *pdata);
void conctrl_scan(void);
void conctrl_command_deal(CTRL_KIND i);

#endif /* MODULE_MODBUS_RTU_H_ */
