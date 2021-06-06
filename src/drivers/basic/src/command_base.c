#include "basic/command.h"
#include "basic/shared.h"

DEVICE_STATUS CommandBase_Init(CommandBase *commandBase)
{
    tx_event_flags_create(&(commandBase->events), "command");
    return DEVICE_STATUS_OK;
};

uint8_t CommandBase_IsBusy(CommandBase *commandBase)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandBase->events, COMMAND_BASE_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

DEVICE_STATUS CommandBase_ConfigCs(CommandBase *commandBase, Pin *csPin, COMMAND_SELECT_PIN_MODE config)
{
    commandBase->csPin = csPin;
    commandBase->csPinCfg = config;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandBase_ConfigRw(CommandBase *commandBase, Pin *rwPin, COMMAND_READWRITE_PIN_MODE config)
{
    commandBase->rwPin = rwPin;
    commandBase->rwPinCfg = config;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandBase_ConfigDc(CommandBase *commandBase, Pin *dcPin, COMMAND_DATACMD_PIN_MODE config)
{
    commandBase->dcPin = dcPin;
    commandBase->dcPinCfg = config;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandBase_WaitForComplete(CommandBase *commandBase, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&commandBase->events, COMMAND_BASE_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
    return commandBase->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};
