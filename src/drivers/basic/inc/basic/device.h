#ifndef __BASIC_DEVICE_H__
#define __BASIC_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "basic/shared.h"

#define DEVICE_STATUS uint32_t

#define DEVICE_STATUS_HARDWARE_ERROR -3
#define DEVICE_STATUS_ARGUMENT_ERROR -2
#define DEVICE_STATUS_GENERAL_ERROR -1

#define DEVICE_STATUS_OK 0
#define DEVICE_STATUS_NOT_NECESSARY 1
#define DEVICE_STATUS_NOT_READY 2
#define DEVICE_STATUS_BUSY 3
#define DEVICE_STATUS_NOT_SUPPORT 4

    // typedef struct P2pDevice
    // {
    //     DEVICE_STATUS (*tx)(void);
    // } P2pDevice;

    // typedef struct BusDevice
    // {

    // } BusDevice;

    typedef struct DeviceBase
    {
        void *host;
    } DeviceBase;
    

#ifdef __cplusplus
}
#endif

#endif // __BASIC_DEVICE_H__