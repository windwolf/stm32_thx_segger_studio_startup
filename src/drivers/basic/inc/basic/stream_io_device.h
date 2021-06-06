#ifndef ___STREAM_DEVICE_H__
#define ___STREAM_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "basic/device.h"
#include "basic/buffer.h"

    typedef struct StreamIoDevice
    {
        DeviceBase base;
        void *instance;
        Buffer _rxBuffer;

        void (*Init)(struct StreamIoDevice *device);
        DEVICE_STATUS(*StartRx)
        (struct StreamIoDevice *device, uint8_t* data, uint16_t size);
        DEVICE_STATUS(*StopRx)
        (struct StreamIoDevice *device);
        DEVICE_STATUS(*Tx)
        (struct StreamIoDevice *device, uint8_t *data, uint32_t size);

        /**
         * @brief tx完成后的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onTxComplete)(struct StreamIoDevice *device);
        /**
         * @brief rx有值时的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onRxReady)(struct StreamIoDevice *device, uint16_t pos);
        void (*onError)(struct StreamIoDevice *device);
    } StreamIoDevice;

#ifdef __cplusplus
}
#endif

#endif // ___STREAM_DEVICE_H__