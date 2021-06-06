#include "st77xx/st77xx.h"

int ST77XX_IsBusy(ST77XX *instance)
{
    ULONG actualFlags;
    return tx_event_flags_get(&instance->events, ST77XX_EVENT_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

DEVICE_STATUS ST77XX_Lock(ST77XX *instance)
{
    if (ST77XX_IsBusy(instance))
    {
        return DEVICE_STATUS_BUSY;
    }
    EVENTS_SET_FLAGS(instance->events, ST77XX_EVENT_BUSY);
    LOG("ST77XX: LCK")
    return DEVICE_STATUS_OK;
}

void ST77XX_Unlock(ST77XX *instance)
{
    EVENTS_RESET_FLAGS(instance->events, ST77XX_EVENT_BUSY);
    LOG("ST77XX: UNLCK")
}

void ST77XX_Command(ST77XX *instance, uint8_t cmdId)
{
    LOG("ST77XX-CMD0: S: %x", cmdId)
    instance->command.commandId = cmdId;
    instance->command.dataSize = 0;
    SimpleCommand_SendCommandAsync(&instance->command);
    LOG("ST77XX-CMD0: W: %x", cmdId)
    CommandBase_WaitForComplete(&instance->command.base, TX_WAIT_FOREVER);
    LOG("ST77XX-CMD0: WC: %x", cmdId)
}

// void ST77XX_Command_Write8xN(ST77XX *instance, uint8_t cmdId, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t size)
// {
//     instance->cmdId = cmdId;
//     instance->cmdData[0] = data1;
//     instance->cmdData[1] = data2;
//     instance->cmdData[2] = data3;
//     instance->cmdData[3] = data4;
//     instance->cmdData[4] = data5;
//     instance->cmdData[5] = data6;
//     instance->command[1].buffer.size = size;
//     instance->command[1].statusBits.is16Bits = 0;
//     instance->command[1].statusBits.isWrite = 1;
//     LOG("ST77XX-CMD8xN: S: %x", cmdId)
//     CommandMaster_SendCommandAsync(&instance->commandMaster, instance->command, 2);
//     LOG("ST77XX-CMD8xN: W: %x", cmdId)
//     CommandBase_WaitForComplete(&instance->commandMaster.base, TX_WAIT_FOREVER);
//     LOG("ST77XX-CMD8xN: WC: %x", cmdId)
// }

// void ST77XX_Command_Write16xN(ST77XX *instance, uint8_t cmdId, uint16_t data1, uint16_t data2, uint8_t size)
// {
//     instance->cmdId = cmdId;

//     ((uint16_t *)(instance->cmdData))[0] = data1;
//     ((uint16_t *)(instance->cmdData))[1] = data2;
//     // instance->cmdData[0] = (uint8_t)data1 >> 8;
//     // instance->cmdData[1] = (uint8_t)data1;
//     // instance->cmdData[2] = (uint8_t)data2 >> 8;
//     // instance->cmdData[3] = (uint8_t)data2;
//     instance->command[1].buffer.size = size;
//     instance->command[1].statusBits.is16Bits = 1;
//     instance->command[1].statusBits.isWrite = 1;
//     LOG("ST77XX-CMD16xN: S: %x", cmdId)
//     CommandMaster_SendCommandAsync(&instance->commandMaster, instance->command, 2);
//     LOG("ST77XX-CMD16xN: W: %x", cmdId)
//     CommandBase_WaitForComplete(&instance->commandMaster.base, TX_WAIT_FOREVER);
//     LOG("ST77XX-CMD16xN: WC: %x", cmdId)
// }

void ST77XX_Command_WriteData8(ST77XX *instance, uint8_t cmdId, uint8_t *data, uint16_t size)
{
    instance->command.commandId = cmdId;
    instance->command.data = data;
    instance->command.dataSize = size;
    instance->command.flagBits.isWrite = 1;
    instance->command.flagBits.is16Bits = 0;
    LOG("ST77XX-DATA8: S: %x", cmdId)
    SimpleCommand_SendCommandAsync(&instance->command);
    LOG("ST77XX-DATA8: W: %x", cmdId)
    CommandBase_WaitForComplete(&instance->command.base, TX_WAIT_FOREVER);
    LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void ST77XX_Command_WriteData16(ST77XX *instance, uint8_t cmdId, uint16_t *data, uint16_t size)
{
    instance->command.commandId = cmdId;
    instance->command.data = data;
    instance->command.dataSize = size;
    instance->command.flagBits.isWrite = 1;
    instance->command.flagBits.is16Bits = 1;
    LOG("ST77XX-DATA16: S: %x", cmdId)
    SimpleCommand_SendCommandAsync(&instance->command);
    LOG("ST77XX-DATA16: W: %x", cmdId)
    CommandBase_WaitForComplete(&instance->command.base, TX_WAIT_FOREVER);
    LOG("ST77XX-DATA16: WC: %x", cmdId)
}

void ST77XX_Command_ReadData8(ST77XX *instance, uint8_t cmdId, uint16_t size)
{
    instance->command.commandId = cmdId;
    instance->command.data = instance->buffer;
    instance->command.dataSize = size;
    instance->command.flagBits.isWrite = 0;
    instance->command.flagBits.is16Bits = 0;
    LOG("ST77XX-DATA8: S: %x", cmdId)
    SimpleCommand_SendCommandAsync(&instance->command);
    LOG("ST77XX-DATA8: W: %x", cmdId)
    CommandBase_WaitForComplete(&instance->command.base, TX_WAIT_FOREVER);
    LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void ST77XX_Command_ReadData16(ST77XX *instance, uint8_t cmdId, uint16_t size)
{
    instance->command.commandId = cmdId;
    instance->command.data = instance->buffer;
    instance->command.dataSize = size;
    instance->command.flagBits.isWrite = 0;
    instance->command.flagBits.is16Bits = 1;
    LOG("ST77XX-DATA16: S: %x", cmdId)
    SimpleCommand_SendCommandAsync(&instance->command);
    LOG("ST77XX-DATA16: W: %x", cmdId)
    CommandBase_WaitForComplete(&instance->command.base, TX_WAIT_FOREVER);
    LOG("ST77XX-DATA16: WC: %x", cmdId)
}