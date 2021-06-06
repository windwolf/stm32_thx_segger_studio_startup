#include "basic/command.h"

static void SimpleCommand_EndPhase2Async(SimpleCommand *simpleCommand);

static void SimpleCommand_SendPhase1Async(SimpleCommand *simpleCommand)
{
    simpleCommand->phase = 1;
    COMMAND_BASE_CS_ENABLE(simpleCommand->base);
    COMMAND_BASE_DC_SET(simpleCommand->base, 1);
    COMMAND_BASE_RW_SET(simpleCommand->base, 1);

    simpleCommand->base.device.TxN8Async(&(simpleCommand->base.device), &simpleCommand->commandId, 1);
}

static void SimpleCommand_SendPhase2Async(SimpleCommand *simpleCommand)
{
    simpleCommand->phase = 2;
    if (simpleCommand->dataSize > 0)
    {
        COMMAND_BASE_DC_SET(simpleCommand->base, 0);
        if (simpleCommand->flagBits.isWrite)
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 1);
            if (simpleCommand->flagBits.is16Bits)
            {
                simpleCommand->base.device.TxN16Async(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize);
            }
            else
            {
                simpleCommand->base.device.TxN8Async(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize);
            }
        }
        else
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 0);
            if (simpleCommand->flagBits.is16Bits)
            {
                simpleCommand->base.device.RxN16Async(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize, 0);
            }
            else
            {
                simpleCommand->base.device.RxN8Async(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize, 0);
            }
        }
    }
    else
    {
        SimpleCommand_EndPhase2Async(simpleCommand);
    }
};

static void SimpleCommand_EndPhase2Async(SimpleCommand *simpleCommand)
{

    COMMAND_BASE_CS_DISABLE(simpleCommand->base);
    simpleCommand->base.hasError = 0;
    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
};

static inline void SimpleCommand_DoTxRxCplt(PacketIoDevice *device)
{
    switch (((SimpleCommand *)device)->phase)
    {
    case 1:
        return SimpleCommand_SendPhase2Async(((SimpleCommand *)device));
    case 2:
        return SimpleCommand_EndPhase2Async(((SimpleCommand *)device));
    }
}

static void SimpleCommand_DoError_(PacketIoDevice *device)
{
    ((SimpleCommand *)device)->base.hasError = 1;
    if (CommandBase_IsBusy(&(((SimpleCommand *)device)->base)))
    {
        EVENTS_RESET_FLAGS(((SimpleCommand *)device)->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(((SimpleCommand *)device)->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    }

    if (((SimpleCommand *)device)->base.onError)
    {
        ((SimpleCommand *)device)->base.onError(((CommandBase *)device));
    }
};

DEVICE_STATUS SimpleCommand_SendCommandSync(SimpleCommand *simpleCommand)
{
    if (!(simpleCommand->base.device.opMode & PACKET_IO_DEVICE_OP_MODE_SYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    COMMAND_BASE_RETURN_IF_BUSY(simpleCommand->base)

    EVENTS_CLEAR_FLAGS(simpleCommand->base.events);

    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);

    COMMAND_BASE_CS_ENABLE(simpleCommand->base);
    COMMAND_BASE_DC_SET(simpleCommand->base, 1);
    COMMAND_BASE_RW_SET(simpleCommand->base, 1);

    simpleCommand->base.device.TxN8(&(simpleCommand->base.device), &simpleCommand->commandId, 1);

    if (simpleCommand->dataSize > 0)
    {
        COMMAND_BASE_DC_SET(simpleCommand->base, 0);
        if (simpleCommand->flagBits.isWrite)
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 1);
            if (simpleCommand->flagBits.is16Bits)
            {
                simpleCommand->base.device.TxN16(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize);
            }
            else
            {
                simpleCommand->base.device.TxN8(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize);
            }
        }
        else
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 0);
            if (simpleCommand->flagBits.is16Bits)
            {
                simpleCommand->base.device.RxN16(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize, 0);
            }
            else
            {
                simpleCommand->base.device.RxN8(&(simpleCommand->base.device), simpleCommand->data, simpleCommand->dataSize, 0);
            }
        }
    }

    COMMAND_BASE_CS_DISABLE(simpleCommand->base);

    simpleCommand->base.hasError = 0;
    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS SimpleCommand_SendCommandAsync(SimpleCommand *simpleCommand)
{
    if (!(simpleCommand->base.device.opMode & PACKET_IO_DEVICE_OP_MODE_ASYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }

    COMMAND_BASE_RETURN_IF_BUSY(simpleCommand->base)

    EVENTS_CLEAR_FLAGS(simpleCommand->base.events);

    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    LOG("CMD-SND-CMD")

    SimpleCommand_SendPhase1Async(simpleCommand);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS SimpleCommand_Init(SimpleCommand *simpleCommand)
{
    CommandBase_Init((CommandBase *)simpleCommand);
    simpleCommand->base.device.base.host = simpleCommand;
    simpleCommand->base.device.onTxComplete = &SimpleCommand_DoTxRxCplt;
    simpleCommand->base.device.onRxComplete = &SimpleCommand_DoTxRxCplt;
    simpleCommand->base.device.onError = &SimpleCommand_DoError_;
    simpleCommand->base.device.Init(&simpleCommand->base.device);
    return DEVICE_STATUS_OK;
};