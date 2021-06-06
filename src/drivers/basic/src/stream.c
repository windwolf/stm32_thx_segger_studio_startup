#include "basic/stream.h"

#define STREAM_EVENT_RX_READY 0b0001
#define STREAM_EVENT_TX_COMPLETE 0b0010
#define STREAM_EVENT_TX_READY 0b0100

static inline void Stream_DoTxComplete_(Stream *stream)
{
    if (stream->onTxComplete)
    {
        stream->onTxComplete(stream);
    }
    tx_event_flags_set(&(stream->events), STREAM_EVENT_TX_COMPLETE | STREAM_EVENT_TX_READY, TX_OR);
};

static inline void Stream_DoRxReady_(Stream *stream, uint16_t pos)
{
    RingBuffer8_SyncWrite(stream->rxBuffer, pos);

    if (stream->onRxReady)
    {
        stream->onRxReady(stream);
    }
    tx_event_flags_set(&(stream->events), STREAM_EVENT_RX_READY, TX_OR);
};

static inline void Stream_DoError_(Stream *stream)
{
    if (stream->onError)
    {
        stream->onError(stream);
    }
};

static inline void Stream_DoTxCompleteWrap_(StreamIoDevice *device)
{
    Stream_DoTxComplete_((Stream *)device);
}
static inline void Stream_DoRxReadyWrap_(StreamIoDevice *device, uint16_t pos)
{
    Stream_DoRxReady_((Stream *)device, pos);
}
static inline void Stream_DoErrorWrap_(StreamIoDevice *device)
{
    Stream_DoError_((Stream *)device);
}

DEVICE_STATUS Stream_Init(Stream *stream, RingBuffer8 *rxBuffer)
{
    //assert_param(rxBuffer != NULL);
    stream->rxBuffer = rxBuffer;
    tx_event_flags_create(&(stream->events), "stream");
    tx_event_flags_set(&(stream->events), STREAM_EVENT_TX_READY, TX_OR);
    stream->device.base.host = stream;
    stream->device.onRxReady = &Stream_DoRxReadyWrap_;
    stream->device.onTxComplete = &Stream_DoTxCompleteWrap_;
    stream->device.onError = Stream_DoErrorWrap_;

    stream->device.Init(&stream->device);
    return DEVICE_STATUS_OK;
}

DEVICE_STATUS Stream_StartServer(Stream *stream)
{

    if (stream->device.StartRx(&stream->device, stream->rxBuffer->data, stream->rxBuffer->size) != DEVICE_STATUS_OK)
    {
        Stream_DoError_(stream);
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}

DEVICE_STATUS Stream_StopServer(Stream *stream)
{
    if (stream->device.StopRx(&stream->device) != DEVICE_STATUS_OK)
    {
        Stream_DoError_(stream);
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}

DEVICE_STATUS Stream_Tx(Stream *stream, uint8_t *data, uint32_t size)
{
    if (stream->device.Tx(&stream->device, data, size) != DEVICE_STATUS_OK)
    {
        Stream_DoError_(stream);
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}

UINT Stream_WaitForTxComplete(Stream *stream, ULONG timeout)
{
    ULONG actualFlags;
    return tx_event_flags_get(&stream->events, STREAM_EVENT_TX_COMPLETE, TX_AND_CLEAR, &actualFlags, timeout);
}
/**
     * @brief 阻塞等待rx准备数据.
     * 
     * @param stream 
     * @param timeout 
     * @return DEVICE_STATUS 
     */
UINT Stream_WaitForRxReady(Stream *stream, ULONG timeout)
{
    ULONG actualFlags;
    UINT rst = tx_event_flags_get(&stream->events, STREAM_EVENT_RX_READY, TX_AND_CLEAR, &actualFlags, timeout);
    return rst;
}