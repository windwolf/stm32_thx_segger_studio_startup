#ifndef __st77xx_h__
#define __st77xx_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/command.h"
#include "tx_api.h"
#include "stdint.h"
#include "bsp.h"
#include "basic/device.h"

#define ST77XX_EVENT_BUSY 0x01

#define ST77XX_EVENTS_SET_BUSY(instance) EVENTS_SET_FLAGS(instance->events, ST77XX_EVENT_BUSY)
#define ST77XX_EVENTS_RESET_BUSY(instance) EVENTS_RESET_FLAGS(instance->events, ST77XX_EVENT_BUSY);

#define DEVICE_STATUS_POSITION_OUT_OF_RANGE -4

    typedef struct ST77XX
    {
        SimpleCommand command;
        TX_EVENT_FLAGS_GROUP events;

        uint8_t cmdData[16];

        uint8_t pvGamma[16];
        uint8_t nvGamma[16];

        uint16_t xOffset;
        uint16_t width;
        uint16_t yOffset;
        uint16_t height;
        uint8_t colorMode;
        uint8_t orientation;
        uint8_t buffer[48114];

    } ST77XX;

    int ST77XX_IsBusy(ST77XX *instance);

    DEVICE_STATUS ST77XX_Lock(ST77XX *instance);
    void ST77XX_Unlock(ST77XX *instance);

    void ST77XX_Command(ST77XX *instance, uint8_t cmdId);
    // void ST77XX_Command_Write8xN(ST77XX *instance, uint8_t cmdId, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t size);
    // void ST77XX_Command_Write16xN(ST77XX *instance, uint8_t cmdId, uint16_t data1, uint16_t data2, uint8_t size);
    void ST77XX_Command_WriteData8(ST77XX *instance, uint8_t cmdId, uint8_t *data, uint16_t size);
    void ST77XX_Command_WriteData16(ST77XX *instance, uint8_t cmdId, uint16_t *data, uint16_t size);
    void ST77XX_Command_ReadData8(ST77XX *instance, uint8_t cmdId, uint16_t size);
    void ST77XX_Command_ReadData16(ST77XX *instance, uint8_t cmdId, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif // __st77xx_h__