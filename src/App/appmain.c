#include "main.h"

#include "tx_api.h"
#include "utils.h"
#include "demo.h"
#include "mem_layout.h"

// using namespace windwolf::drivers::Bsp;

// static inline void Uart_RxEventCpltCallback__(UART_HandleTypeDef *_handle, uint16_t pos)
// {
//     //RingBuffer_SyncWriteIndex(&buffer, pos);
//     RingBuffer8_SyncWrite(&buffer2, pos);
//     cWrite++;
//     // if (pos > _lastRxPos)
//     // {
//     //     if (txWriteIndex == 0)
//     //     {
//     //         memcpy(txBuf0, _handle->pRxBuffPtr + _lastRxPos, pos - _lastRxPos);
//     //         txBuf0Count = pos - _lastRxPos;
//     //         tx0Ready = 1;
//     //         txWriteIndex = 1;
//     //     }
//     //     else
//     //     {
//     //         memcpy(txBuf1, _handle->pRxBuffPtr + _lastRxPos, pos - _lastRxPos);
//     //         txBuf1Count = pos - _lastRxPos;
//     //         tx1Ready = 1;
//     //         txWriteIndex = 0;
//     //     }
//     // }
//     // else if (pos < _lastRxPos)
//     // {
//     //     if (txWriteIndex == 0)
//     //     {
//     //         memcpy(txBuf0, _handle->pRxBuffPtr + _lastRxPos, _handle->RxXferSize - _lastRxPos);
//     //         memcpy(txBuf0 + _handle->RxXferSize - _lastRxPos, _handle->pRxBuffPtr, pos);
//     //         txBuf0Count = _handle->RxXferSize - _lastRxPos + pos;
//     //         txWriteIndex = 1;
//     //     }
//     //     else
//     //     {
//     //         memcpy(txBuf1, _handle->pRxBuffPtr + _lastRxPos, _handle->RxXferSize - _lastRxPos);
//     //         memcpy(txBuf1 + _handle->RxXferSize - _lastRxPos, _handle->pRxBuffPtr, pos);
//     //         txBuf1Count = _handle->RxXferSize - _lastRxPos + pos;
//     //         txWriteIndex = 0;
//     //     }
//     // }
//     // else
//     // {
//     //     // _lastRxPos == pos. Because half and full buffer pos will gen rx event, so lastrxpos equals pos when
//     //     // and only no validate data received.
//     // }
//     // _lastRxPos = (pos == _handle->RxXferSize) ? 0 : pos;
// }

int main(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
    MX_All_Init();
    LOG("begin trace\n")

    LOG("begin kernel\n")
    tx_kernel_enter();

    // HAL_StatusTypeDef rst;
    // HAL_UART_RegisterRxEventCallback(&huart4, Uart_RxEventCpltCallback__);
    // UartDevice_Tx(&uart, text, strlen(text));
    // UartDevice_WaitForTxComplete(&uart, TX_WAIT_FOREVER);
    //rst = HAL_UART_Transmit(&huart4, text, strlen(text), 10000);
    //rst = HAL_UART_Transmit_DMA(&huart4, text, strlen(text));
    //LL_USART_EnableIT_IDLE(huart4.Instance);
    // rst = HAL_UARTEx_ReceiveToIdle_DMA(&huart4, testRxBuf, 65);
    // if (rst)
    // {
    // }
    while (1)
    {

        // uint32_t len = RingBuffer8_GetCount(&buffer2);
        // if (len > 0)
        // {
        //     RingBuffer8_Read(&buffer2, txBuf0, len);
        //     rst = HAL_UART_Transmit_DMA(&huart4, txBuf0, len);
        //     HAL_Delay(10);
        //     cRead++;
        // }

        // uint32_t len = RingBuffer_GetMemoryLength(&buffer);
        // if (len > 0)
        // {
        //     RingBuffer_Read(&buffer, txBuf0, len);
        //     rst = HAL_UART_Transmit_DMA(&huart4, txBuf0, len);
        // }
        // if (txReadIndex == 0)
        // {
        //     if (tx0Ready)
        //     {
        //         rst = HAL_UART_Transmit_DMA(&huart4, txBuf0, txBuf0Count);
        //         tx0Ready = 0;
        //         txReadIndex = 1;
        //     }
        // }
        // else if (txReadIndex == 1)
        // {
        //     if (tx1Ready)
        //     {
        //         rst = HAL_UART_Transmit_DMA(&huart4, txBuf1, txBuf1Count);
        //         tx1Ready = 0;
        //         txReadIndex = 0;
        //     }
        // }
    }
}
