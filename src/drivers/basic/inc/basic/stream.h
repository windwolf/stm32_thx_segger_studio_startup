#ifndef __STREAM_H__
#define __STREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "basic/device.h"
#include "basic/ring_buffer8.h"
#include "basic/stream_io_device.h"
#include "tx_api.h"

    typedef struct Stream
    {
        StreamIoDevice device;

        RingBuffer8 *rxBuffer;
        TX_EVENT_FLAGS_GROUP events;

        void (*onTxComplete)(struct Stream *stream);

        void (*onRxReady)(struct Stream *stream);

        void (*onError)(struct Stream *stream);

    } Stream;

    DEVICE_STATUS Stream_Init(Stream *stream, RingBuffer8 *rxBuffer);

    DEVICE_STATUS Stream_StartServer(Stream *stream);

    DEVICE_STATUS Stream_StopServer(Stream *stream);

    DEVICE_STATUS Stream_Tx(Stream *stream, uint8_t *data, uint32_t size);
    /**
     * @brief 阻塞等待tx完成.
     * 
     * @param stream 
     * @return DEVICE_STATUS 
     */
    UINT Stream_WaitForTxComplete(Stream *stream, ULONG timeout);
    /**
     * @brief 阻塞等待rx准备数据.
     * 
     * @param stream 
     * @param timeout 
     * @return DEVICE_STATUS 
     */
    UINT Stream_WaitForRxReady(Stream *stream, ULONG timeout);

#ifdef __cplusplus
}
#endif

#endif // __STREAM_H__
