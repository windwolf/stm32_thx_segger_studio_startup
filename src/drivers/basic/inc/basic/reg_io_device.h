#ifndef ___REG_IO_DEVICE_H__
#define ___REG_IO_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "basic/device.h"
#include "basic/buffer.h"

    typedef struct RegIoDevice
    {
        DeviceBase base;
        void *instance;
        Buffer _rxBuffer;

        DEVICE_STATUS(*Init)
        (struct RegIoDevice *device);
        DEVICE_STATUS(*Read8)
        (struct RegIoDevice *device, uint32_t regAddress, uint8_t *data, uint16_t size, uint32_t deviceAddress);
        DEVICE_STATUS(*Read16)
        (struct RegIoDevice *device, uint32_t regAddress, uint16_t *data, uint16_t size, uint32_t deviceAddress);
        DEVICE_STATUS(*Write8)
        (struct RegIoDevice *device, uint32_t regAddress, uint8_t *data, uint16_t size, uint32_t deviceAddress);
        DEVICE_STATUS(*Write16)
        (struct RegIoDevice *device, uint32_t regAddress, uint16_t *data, uint16_t size, uint32_t deviceAddress);

        void (*onReadComplete)(struct RegIoDevice *device);
        void (*onWriteComplete)(struct RegIoDevice *device);
        void (*onError)(struct RegIoDevice *device);
    } RegIoDevice;

#ifdef __cplusplus
}
#endif

#endif // ___REG_IO_DEVICE_H__