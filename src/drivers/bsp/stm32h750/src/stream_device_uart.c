#include "stream_io_device_uart.h"

typedef enum UART_INDEX
{
    UART4_INDEX = 0,
    UART5_INDEX,
    UART7_INDEX,
    UART8_INDEX,
    UART_COUNT,
} UART_INDEX;

static StreamIoDevice *UART_INSTANCES_[(uint32_t)(UART_COUNT)];

static inline UART_INDEX Instance_Index_Get(UART_HandleTypeDef *huart)
{
    uint32_t ins = (uint32_t)(huart->Instance);

    switch (ins)
    {
    case UART4_BASE:
        return UART4_INDEX;
    case UART5_BASE:
        return UART5_INDEX;
    case UART7_BASE:
        return UART7_INDEX;
    case UART8_BASE:
        return UART8_INDEX;
    default:
        return UART_COUNT;
    }
}

static inline void Instance_Register(StreamIoDevice *stream)
{
    UART_INDEX index = Instance_Index_Get((UART_HandleTypeDef *)(stream->instance));
    UART_INSTANCES_[(uint32_t)index] = stream;
}

static inline StreamIoDevice *Instance_Find(UART_HandleTypeDef *huart)
{
    UART_INDEX index = Instance_Index_Get(huart);
    return UART_INSTANCES_[(uint32_t)index];
}

static inline void Uart_TxCpltCallback__(UART_HandleTypeDef *huart)
{
    StreamIoDevice *device = Instance_Find(huart);
    device->onTxComplete(device);
}

static inline void Uart_RxEventCpltCallback__(UART_HandleTypeDef *huart, uint16_t pos)
{
    StreamIoDevice *device = Instance_Find(huart);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    device->onRxReady(device, pos);
}

static inline void Uart_ErrCpltCallback__(UART_HandleTypeDef *huart)
{
    StreamIoDevice *device = Instance_Find(huart);
    //HAL_UART_DeInit(huart);
    //HAL_UART_Init(huart);
    //Init(device);
    LL_USART_Disable(huart->Instance);
    LL_USART_Enable(huart->Instance);

    device->onError(device);
}

static void Init(StreamIoDevice *device)
{
    Instance_Register(device);
    HAL_UART_RegisterCallback(device->instance, HAL_UART_TX_COMPLETE_CB_ID, Uart_TxCpltCallback__);
    //HAL_UART_RegisterCallback(_handle, HAL_UART_RX_COMPLETE_CB_ID, Uart_RxCpltCallback__);
    HAL_UART_RegisterCallback(device->instance, HAL_UART_ERROR_CB_ID, Uart_ErrCpltCallback__);
    HAL_UART_RegisterRxEventCallback(device->instance, Uart_RxEventCpltCallback__);
}

static DEVICE_STATUS StartRx(StreamIoDevice *device, uint8_t *data, uint16_t size)
{
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = size;
    UART_HandleTypeDef *huart = device->instance;
    if (huart->hdmarx->Init.Mode != DMA_CIRCULAR)
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX)
    {
        return DEVICE_STATUS_BUSY;
    }

    if (HAL_UARTEx_ReceiveToIdle_DMA(huart, data, size) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}
static DEVICE_STATUS StopRx(StreamIoDevice *device)
{
    UART_HandleTypeDef *huart = device->instance;

    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) != HAL_UART_STATE_BUSY_RX)
    {
        return DEVICE_STATUS_NOT_NECESSARY;
    }
    if (HAL_UART_DMAStop(huart) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}
static DEVICE_STATUS Tx(StreamIoDevice *device, uint8_t *data, uint32_t size)
{
    UART_HandleTypeDef *huart = device->instance;
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX)
    {
        return DEVICE_STATUS_BUSY;
    }
    SCB_CleanDCache_by_Addr((uint32_t *)data, size);
    if (HAL_UART_Transmit_DMA(huart, data, size) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }

    return DEVICE_STATUS_OK;
}

void Uart_StreamDevice_Create(StreamIoDevice *device, UART_HandleTypeDef *huart)
{
    device->instance = huart;
    device->Init = &Init;
    device->StartRx = &StartRx;
    device->StopRx = &StopRx;
    device->Tx = &Tx;
    device->onError = NULL;
    device->onRxReady = NULL;
    device->onTxComplete = NULL;
}
