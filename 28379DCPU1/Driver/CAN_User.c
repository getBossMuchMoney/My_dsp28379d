/*******************************************************************************
 * File Name: CAN_User.c
 * Description: The CMPSS initialization functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/

#include "F28x_Project.h"
#include "CAN_User.h"
#include "Modbus_RTU.h"

uint8_t userdata[8] = {0};
uint8_t brodcdata[8] = {0};
WORK_MODE mode = MASTER;

uint32_t msgID = 0;
uint32_t device_id = 3;
uint8_t CAN_recv_Master, CAN_send_cplt = 0;
extern uint64_t timercnt;
//
// Function Prototypes
//
uint64_t getTimeMS(void);

uint64_t getTimeMS(void)
{
    return timercnt;
}

__interrupt void canISR(void);

void CAN_Init(void)
{

    GPIO_setPinConfig(GPIO_30_CANRXA);
    GPIO_setPinConfig(GPIO_31_CANTXA);

    //
    // Initialize the CAN controller
    //
    // Puts the module in Initialization mode, Disables the parity function
    // Initializes the MBX RAM, Initiates a S/W reset of the module
    // Seeks write-access to configuration registers.
    //
    CAN_initModule(CANA_BASE);

    //
    // Set up the CAN bus bit rate to 500 kbps
    // Refer to the Driver Library User Guide for information on how to set
    // tighter timing control. Additionally, consult the device data sheet
    // for more information about the CAN module clocking.
    //
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 500000, 20);

    CAN_enableInterrupt(CANA_BASE, CAN_INT_IE0 | CAN_INT_ERROR |
                                       CAN_INT_STATUS);

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    // This registers the interrupt handler in PIE vector table.
    //
    Interrupt_register(INT_CANA0, &canISR);

    //
    // Enable the CAN interrupt signal
    //
    Interrupt_enable(INT_CANA0);
    CAN_enableGlobalInterrupt(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    //
    // Enable CAN test mode with external loopback
    //
    CAN_enableTestMode(CANA_BASE, CAN_TEST_EXL);

    //
    // Initialize the transmit message object used for sending CAN messages.
    // Message Object Parameters:
    //      Message Object ID Number: 1
    //      Message Identifier: 0x1
    //      Message Frame: Standard
    //      Message Type: Transmit
    //      Message ID Mask: 0x0
    //      Message Object Flags: Transmit Interrupt
    //      Message Data Length: 4 Bytes
    //
    // CAN_setupMessageObject(CANA_BASE, TX_MSG_OBJ_ID, 0x1, CAN_MSG_FRAME_STD,
    //                       CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_TX_INT_ENABLE,
    //                      MSG_DATA_LENGTH);

    //
    // Initialize the receive message object used for receiving CAN messages.
    // Message Object Parameters:
    //      Message Object ID Number: 2
    //      Message Identifier: 0x1
    //      Message Frame: Standard
    //      Message Type: Receive
    //      Message ID Mask: 0x0
    //      Message Object Flags: Receive Interrupt
    //      Message Data Length: 4 Bytes (Note that DLC field is a "don't care"
    //      for a Receive mailbox)
    //
    CAN_setupMessageObject(CANA_BASE, RX_MSG_OBJ_ID, 0x0300, CAN_MSG_FRAME_EXT,
                           CAN_MSG_OBJ_TYPE_RX, 0xFF00, CAN_MSG_OBJ_RX_INT_ENABLE | CAN_MSG_OBJ_USE_EXT_FILTER | CAN_MSG_OBJ_USE_ID_FILTER,
                           MSG_DATA_LENGTH);

    CAN_setupMessageObject(CANA_BASE, BROD_MSG_OBJ_ID, 0x0500, CAN_MSG_FRAME_EXT,
                           CAN_MSG_OBJ_TYPE_RX, 0xFF00, CAN_MSG_OBJ_RX_INT_ENABLE | CAN_MSG_OBJ_USE_EXT_FILTER | CAN_MSG_OBJ_USE_ID_FILTER,
                           MSG_DATA_LENGTH);

    //
    // Start CAN module operations
    //
    CAN_startModule(CANA_BASE);
}


/*can read or write more than 3 registers*/
CAN_ERRCODE CAN_MultiREG_Master(uint32_t target_ID, uint16_t addr, uint32_t func_code, uint32_t reg_num, uint8_t *pdata, TIMEOUT_SET timeout)
{
    uint8_t i = 0;
    CAN_ERRCODE err = CAN_OK;
    uint8_t databuf[8] = {0};
    if (func_code == REG_READ)
    {
        if (reg_num > 3)
        {
            for (i = 0; i < reg_num / 3; i++)
            {
                err = CAN_SendReadData_Master(target_ID, addr + i * 3, func_code, 3, databuf, pdata + i * 6, timeout);
                if (err != CAN_OK)
                {
                    return err;
                }
            }

            if (reg_num % 3 != 0)
            {
                return CAN_SendReadData_Master(target_ID, addr + i * 3, func_code, reg_num % 3, databuf, pdata + i * 6, timeout);
            }

            return err;
        }
        else
        {
            return CAN_SendReadData_Master(target_ID, addr, func_code, reg_num, databuf, pdata, timeout);
        }
    }
    else if (func_code == REG_WRITE)
    {
        if (reg_num > 3)
        {
            for (i = 0; i < reg_num / 3; i++)
            {
                err = CAN_SendReadData_Master(target_ID, addr + i * 3, func_code, 3, pdata + i * 6, databuf, timeout);
                if (err != CAN_OK)
                {
                    return err;
                }
            }

            if (reg_num % 3 != 0)
            {
                return CAN_SendReadData_Master(target_ID, addr + i * 3, func_code, reg_num % 3, pdata + i * 6, databuf, timeout);
            }

            return err;
        }
        else
        {
            return CAN_SendReadData_Master(target_ID, addr, func_code, reg_num, pdata, databuf, timeout);
        }
    }
    return err;
}

void Slaver_command_deal(uint32_t msgID)
{
    uint32_t target_id = 0;
    uint16_t funcCode = 0, reg_num = 0;
    uint16_t addr = (userdata[0] << 8) | (userdata[1]);
    uint8_t databuf[8] = {0};

    funcCode = (msgID >> 20) & 0xFF;
    reg_num = ((msgID >> 16) & 0x0F) / 2;
    target_id = msgID & 0xFF;

    switch (funcCode)
    {
    case REG_READ:
    {
        switch (addr & 0x300)
        {
        case STATE_REG_BASE:
        {
            if (addr + reg_num > STATE_REG_BASE + STATE_REG_SIZE)
            {
                CAN_SendData_Slaver(target_id, REG_READ, STA_CMD_ERR,0, NULL, TIME_LEVEL1);
            }
            else
            {
                read_REG(STATE_REG_BASE, addr & 0xFF, reg_num, databuf);
                CAN_SendData_Slaver(target_id, REG_READ, STA_OK, reg_num, databuf, TIME_LEVEL1);
            }
        }
        break;

        case OPT_REG_BASE:
        {
            if (addr + reg_num > OPT_REG_BASE + OPTION_REG_SIZE)
            {
                CAN_SendData_Slaver(target_id, REG_READ, STA_CMD_ERR, 0, NULL, TIME_LEVEL1);
            }
            else
            {
                read_REG(OPT_REG_BASE, addr & 0xFF, reg_num, databuf);
                CAN_SendData_Slaver(target_id, REG_READ, STA_OK, reg_num, databuf, TIME_LEVEL1);
            }
        }
        break;

        default:
        {
            CAN_SendData_Slaver(target_id, REG_READ, STA_ADDR_ERR, 0, NULL, TIME_LEVEL1);
        }
        break;
        }
    }
    break;

    case REG_WRITE:
    {
        switch (addr & 0x300)
        {

        case CTRL_REG_BASE:
        {
            if (addr + reg_num > CTRL_REG_BASE + CONCTROL_REG_SIZE)
            {
                CAN_SendData_Slaver(target_id, REG_WRITE, STA_CMD_ERR, 0, NULL, TIME_LEVEL1);
            }
            else
            {
                write_REG(CTRL_REG_BASE, addr & 0xFF, reg_num, userdata + 2);
                CAN_SendData_Slaver(target_id, REG_WRITE, STA_OK, 0, NULL, TIME_LEVEL1);
                conctrl_scan();
            }
        }
        break;

        case OPT_REG_BASE:
        {
            if (addr + reg_num > OPT_REG_BASE + OPTION_REG_SIZE)
            {
                CAN_SendData_Slaver(target_id, REG_WRITE, STA_CMD_ERR, 0, NULL, TIME_LEVEL1);
            }
            else
            {
                write_REG(OPT_REG_BASE, addr & 0xFF, reg_num, userdata + 2);
                CAN_SendData_Slaver(target_id, REG_WRITE, STA_OK, 0, NULL, TIME_LEVEL1);
            }
        }
        break;

        default:
        {
            CAN_SendData_Slaver(target_id, REG_WRITE, STA_ADDR_ERR, 0, NULL, TIME_LEVEL1);
        }
        break;
        }
    }
    break;

    default:
    {
        CAN_SendData_Slaver(target_id, 0, STA_FUNC_ERR, 0, NULL, TIME_LEVEL1);
    }
    break;
    }
}

CAN_ERRCODE CAN_SendData_Master(uint32_t target_ID, uint16_t addr, uint32_t func_code, uint32_t reg_num, uint8_t *pdata, TIMEOUT_SET timeout)
{
    uint32_t message_id = 0;
    uint8_t txbuf[8] = {0};
    CAN_ERRCODE err = CAN_OK;
    uint64_t t0, t1 = 0;

    if ((reg_num > 3) || (reg_num > 0 && pdata == NULL))
    {
        err = CAN_PARAM_ERR;
        return err;
    }

    message_id = device_id;
    message_id |= (target_ID << 8);
    message_id |= ((reg_num * 2) << 16);
    message_id |= (func_code << 20);

    txbuf[0] = addr >> 8;
    txbuf[1] = addr & 0x00FF;

    if (reg_num > 0 && pdata != NULL)
    {
        memcpy(txbuf + 2, pdata, reg_num * 2);
    }

    CAN_setupMessageObject(CANA_BASE, TX_MSG_OBJ_ID, message_id, CAN_MSG_FRAME_EXT,
                           CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_TX_INT_ENABLE,
                           MSG_DATA_LENGTH);
    CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH, txbuf);
    t0 = getTimeMS();
    while (1)
    {
        if (CAN_send_cplt == 1)
        {
            CAN_send_cplt = 0;
            return err;
        }
        t1 = getTimeMS();
        if (t1 - t0 > timeout)
        {
            err = CAN_SEND_TIMEOUT;
            return err;
        }
    }
}

CAN_ERRCODE CAN_SendReadData_Master(uint32_t target_ID, uint16_t addr, uint32_t func_code, uint32_t reg_num, uint8_t *ptxdata, uint8_t *prxdata, TIMEOUT_SET timeout)
{
    uint32_t message_id = 0;
    uint8_t txbuf[8] = {0};
    CAN_ERRCODE err = CAN_OK;
    uint64_t t0, t1 = 0;

    if ((reg_num > 3) || (reg_num > 0 && (ptxdata == NULL || prxdata == NULL)))
    {
        err = CAN_PARAM_ERR;
        return err;
    }

    message_id = device_id;
    message_id |= (target_ID << 8);
    message_id |= ((reg_num * 2) << 16);
    message_id |= (func_code << 20);

    txbuf[0] = addr >> 8;
    txbuf[1] = addr & 0x00FF;

    if (reg_num > 0 && ptxdata != NULL)
    {
        memcpy(txbuf + 2, ptxdata, reg_num * 2);
    }

    CAN_setupMessageObject(CANA_BASE, TX_MSG_OBJ_ID, message_id, CAN_MSG_FRAME_EXT,
                           CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_TX_INT_ENABLE,
                           MSG_DATA_LENGTH);

    CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH, txbuf);
    t0 = getTimeMS();

    while (1)
    {
        if (CAN_send_cplt == 1)
        {
            CAN_send_cplt = 0;
            t0 = getTimeMS();
            break;
        }
        t1 = getTimeMS();
        if (t1 - t0 > timeout)
        {
            err = CAN_SEND_TIMEOUT;
            return err;
        }
    }

    while (1)
    {
        if (CAN_recv_Master == 1)
        {
            CAN_recv_Master = 0;
            if (0 == userdata[0])
            {
                if (reg_num > 0)
                {
                    memcpy(prxdata, userdata + 2, reg_num * 2);
                }
                return err;
            }
            else
            {
                err = CAN_CMD_ERR;
                return err;
            }
        }
        t1 = getTimeMS();
        if (t1 - t0 > timeout)
        {
            err = CAN_RECV_TIMEOUT;
            return err;
        }
    }
}

CAN_ERRCODE CAN_SendData_Slaver(uint32_t target_ID, uint32_t func_code, STATE_CODE sta_code, uint32_t reg_num, uint8_t *pdata, TIMEOUT_SET timeout)
{
    uint32_t message_id = 0;
    uint8_t txbuf[8] = {0};
    CAN_ERRCODE err = CAN_OK;
    uint64_t t0, t1 = 0;

    if ((reg_num > 3) || (reg_num > 0 && pdata == NULL))
    {
        err = CAN_PARAM_ERR;
        return err;
    }

    message_id = device_id;
    message_id |= (target_ID << 8);
    message_id |= ((reg_num * 2) << 16);
    message_id |= (func_code << 20);

    txbuf[0] = sta_code;
    if (reg_num > 0 && pdata != NULL)
    {
        memcpy(txbuf + 2, pdata, reg_num * 2);
    }

    CAN_setupMessageObject(CANA_BASE, TX_MSG_OBJ_ID, message_id, CAN_MSG_FRAME_EXT,
                           CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_TX_INT_ENABLE,
                           MSG_DATA_LENGTH);
    CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH, txbuf);
    t0 = getTimeMS();
    while (1)
    {
        if (CAN_send_cplt == 1)
        {
            CAN_send_cplt = 0;
            return err;
        }
        t1 = getTimeMS();
        if (t1 - t0 > timeout)
        {
            err = CAN_SEND_TIMEOUT;
            return err;
        }
    }
}

void canISR(void)
{
    uint32_t status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    status = CAN_getInterruptCause(CANA_BASE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if (status == CAN_INT_INT0ID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        status = CAN_getStatus(CANA_BASE);

        //
        // Check to see if an error occurred.
        //
        if (((status & ~(CAN_STATUS_TXOK | CAN_STATUS_RXOK)) != 7) &&
            ((status & ~(CAN_STATUS_TXOK | CAN_STATUS_RXOK)) != 0))
        {
            //
            // Set a flag to indicate some errors may have occurred.
            //
        }
    }

    //
    // Check if the cause is the transmit message object 1
    //
    else if (status == TX_MSG_OBJ_ID)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CAN_clearInterruptStatus(CANA_BASE, TX_MSG_OBJ_ID);
        CAN_send_cplt = 1;

        //
        // Check if the cause is the receive message object 2
        //
    }
    else if (status == RX_MSG_OBJ_ID)
    {
        //
        // Get the received message
        //

        if(mode == MASTER)
        {
          CAN_recv_Master = 1;
        }

        CAN_readMessageWithID(CANA_BASE, RX_MSG_OBJ_ID, CAN_MSG_FRAME_EXT, &msgID, userdata);

        //
        // Getting to this point means that the RX interrupt occurred on
        // message object 2, and the message RX is complete.  Clear the
        // message object interrupt.
        //
        CAN_clearInterruptStatus(CANA_BASE, RX_MSG_OBJ_ID);

        if (mode == SLAVER)
        {
            Slaver_command_deal(msgID);
        }
    }

    else if (status == BROD_MSG_OBJ_ID)
    {
        //
        // Get the received message
        //
        CAN_readMessageWithID(CANA_BASE, BROD_MSG_OBJ_ID, CAN_MSG_FRAME_EXT, &msgID, userdata);

        //
        // Getting to this point means that the RX interrupt occurred on
        // message object 2, and the message RX is complete.  Clear the
        // message object interrupt.
        //
        CAN_clearInterruptStatus(CANA_BASE, BROD_MSG_OBJ_ID);

        Slaver_command_deal(msgID);
    }

    //
    // If something unexpected caused the interrupt, this would handle it.
    //
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }

    //
    // Clear the global interrupt flag for the CAN interrupt line
    //
    CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    //
    // Acknowledge this interrupt located in group 9
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}
