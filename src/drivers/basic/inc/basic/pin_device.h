#ifndef ___PIN_DEVICE_H__
#define ___PIN_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "basic/device.h"

    typedef struct PinDevice
    {
        DeviceBase base;
        void *instance;
        void (*Init)(struct PinDevice *device);
        DEVICE_STATUS(*Write)
        (struct PinDevice *device, uint16_t pinMask, uint8_t value);
        DEVICE_STATUS(*Read)
        (struct PinDevice *device, uint16_t pinMask, uint16_t *value);
        DEVICE_STATUS(*Toggle)
        (struct PinDevice *device, uint16_t pinMask);
    } PinDevice;

#ifdef __cplusplus
}
#endif

#endif // ___PIN_DEVICE_H__