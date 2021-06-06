#include "basic/command.h"

static void CommandMaster_SendFrameSync(CommandMaster *commandMaster, CommandFrame frame)
{
    COMMAND_BASE_CS_ENABLE(commandMaster->base);
    COMMAND_BASE_DC_SET(commandMaster->base, frame.statusBits.isCmd);
    COMMAND_BASE_RW_SET(commandMaster->base, frame.statusBits.isWrite);

    if (frame.statusBits.isWrite)
    {
        if (frame.statusBits.is16Bits)
        {
            commandMaster->base.device.TxN16(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size);
        }
        else
        {
            commandMaster->base.device.TxN8(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size);
        }
    }
    else
    {
        if (frame.statusBits.is16Bits)
        {
            commandMaster->base.device.RxN16(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
        }
        else
        {
            commandMaster->base.device.RxN8(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
        }
    }
    if (!frame.statusBits.csNotBreak)
    {
        COMMAND_BASE_CS_DISABLE(commandMaster->base);
    }
};

static void CommandMaster_SendFrameAsync(CommandMaster *commandMaster)
{
    if (commandMaster->_curCommandFrameIndex < commandMaster->_curCommandFrameSize)
    {
        CommandFrame frame = commandMaster->_curCommand[commandMaster->_curCommandFrameIndex];

        COMMAND_BASE_CS_ENABLE(commandMaster->base);
        COMMAND_BASE_DC_SET(commandMaster->base, frame.statusBits.isCmd);
        COMMAND_BASE_RW_SET(commandMaster->base, frame.statusBits.isWrite);

        if (frame.statusBits.isWrite)
        {
            if (frame.statusBits.is16Bits)
            {
                LOG("CMD-SF-A-W:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->base.device.TxN16Async(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size);
                LOG("CMD-SF-A-W:ed %lu", commandMaster->_curCommandFrameIndex)
            }
            else
            {
                LOG("CMD-SF-A-W:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->base.device.TxN8Async(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size);
                LOG("CMD-SF-A-W:ed %lu", commandMaster->_curCommandFrameIndex)
            }
        }
        else
        {
            if (frame.statusBits.is16Bits)
            {
                LOG("CMD-SF-A-R:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->base.device.RxN16Async(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
                LOG("CMD-SF-A-R:ed %lu", commandMaster->_curCommandFrameIndex)
            }
            else
            {
                LOG("CMD-SF-A-R:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->base.device.RxN8Async(&(commandMaster->base.device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
                LOG("CMD-SF-A-R:ed %lu", commandMaster->_curCommandFrameIndex)
            }
        }
    }
    else
    {
        LOG("CMD-SF-E:")
        COMMAND_BASE_CS_DISABLE(commandMaster->base);
        commandMaster->base.hasError = 0;
        EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
        EVENTS_RESET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    }
};

static inline void CommandMaster_DoTxRxCplt(PacketIoDevice *device)
{
    ((CommandMaster *)device)->_curCommandFrameIndex++;
    CommandMaster_SendFrameAsync(((CommandMaster *)device));
}

static void CommandMaster_DoError_(PacketIoDevice *device)
{
    ((CommandMaster *)device)->base.hasError = 1;
    if (CommandBase_IsBusy(&(((CommandMaster *)device)->base)))
    {
        EVENTS_RESET_FLAGS(((CommandMaster *)device)->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(((CommandMaster *)device)->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    }

    if (((CommandMaster *)device)->base.onError)
    {
        ((CommandMaster *)device)->base.onError(((CommandBase *)device));
    }
};

DEVICE_STATUS CommandMaster_SendCommandSync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size)
{
    if (!(commandMaster->base.device.opMode & PACKET_IO_DEVICE_OP_MODE_SYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    COMMAND_BASE_RETURN_IF_BUSY(commandMaster->base)

    EVENTS_CLEAR_FLAGS(commandMaster->base.events);

    EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);

    for (size_t i = 0; i < size; i++)
    {
        CommandMaster_SendFrameSync(commandMaster, command[i]);
    }
    commandMaster->base.hasError = 0;
    EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_SendCommandAsync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size)
{
    if (!(commandMaster->base.device.opMode & PACKET_IO_DEVICE_OP_MODE_ASYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }

    COMMAND_BASE_RETURN_IF_BUSY(commandMaster->base)

    commandMaster->_curCommand = command;
    commandMaster->_curCommandFrameIndex = 0;
    commandMaster->_curCommandFrameSize = size;

    EVENTS_CLEAR_FLAGS(commandMaster->base.events);

    EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    LOG("CMD-SND-CMD")
    CommandMaster_SendFrameAsync(commandMaster);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_Init(CommandMaster *commandMaster)
{
    CommandBase_Init((CommandBase *)commandMaster);
    commandMaster->base.device.base.host = commandMaster;
    commandMaster->base.device.onTxComplete = &CommandMaster_DoTxRxCplt;
    commandMaster->base.device.onRxComplete = &CommandMaster_DoTxRxCplt;
    commandMaster->base.device.onError = &CommandMaster_DoError_;
    commandMaster->base.device.Init(&commandMaster->base.device);
    return DEVICE_STATUS_OK;
};