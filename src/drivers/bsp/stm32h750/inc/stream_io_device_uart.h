#ifndef ___STREAM_UART_H__
#define ___STREAM_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/stream_io_device.h"
#include "bsp.h"



    void Uart_StreamDevice_Create(StreamIoDevice *device, UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif // ___STREAM_UART_H__