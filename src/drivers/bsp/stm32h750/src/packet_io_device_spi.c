#include "packet_io_device_spi.h"

typedef enum SPI_INDEX
{
    SPI1_INDEX = 0,
    SPI2_INDEX,
    SPI3_INDEX,
    SPI4_INDEX,
    SPI6_INDEX,
    SPI_COUNT,
} SPI_INDEX;

static PacketIoDevice *SPI_INSTANCE_[(uint32_t)(SPI_COUNT)];

static inline SPI_INDEX Instance_Index_Get(SPI_HandleTypeDef *handle)
{
    uint32_t ins = (uint32_t)(handle->Instance);

    switch (ins)
    {
    case SPI1_BASE:
        return SPI1_INDEX;
    case SPI2_BASE:
        return SPI2_INDEX;
    case SPI3_BASE:
        return SPI3_INDEX;
    case SPI4_BASE:
        return SPI4_INDEX;
    case SPI6_BASE:
        return SPI6_INDEX;
    default:
        return SPI_COUNT;
    }
}

static inline void Instance_Register(PacketIoDevice *device)
{
    SPI_INDEX index = Instance_Index_Get((SPI_HandleTypeDef *)(device->instance));
    SPI_INSTANCE_[(uint32_t)index] = device;
}

static inline PacketIoDevice *Instance_Find(SPI_HandleTypeDef *huart)
{
    SPI_INDEX index = Instance_Index_Get(huart);
    return SPI_INSTANCE_[(uint32_t)index];
}

static void Spi_TxCpltCallback__(SPI_HandleTypeDef *handle)
{
    PacketIoDevice *device = Instance_Find(handle);
    if (device->onTxComplete)
    {
        device->onTxComplete(device);
    }
};

static void Spi_RxCpltCallback__(SPI_HandleTypeDef *handle)
{
    PacketIoDevice *device = Instance_Find(handle);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    if (device->onRxComplete)
    {
        device->onRxComplete(device);
    }
}

static void Spi_ErrCallback__(SPI_HandleTypeDef *handle)
{
    PacketIoDevice *device = Instance_Find(handle);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    if (device->onError)
    {
        device->onError(device);
    }
};

static void Init(PacketIoDevice *device)
{
    Instance_Register(device);
    HAL_SPI_RegisterCallback(device->instance, HAL_SPI_TX_COMPLETE_CB_ID, Spi_TxCpltCallback__);
    HAL_SPI_RegisterCallback(device->instance, HAL_SPI_RX_COMPLETE_CB_ID, Spi_RxCpltCallback__);
    HAL_SPI_RegisterCallback(device->instance, HAL_SPI_ERROR_CB_ID, Spi_ErrCallback__);
}

static void SwitchTo8Bits(PacketIoDevice *device)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->instance);
    uint32_t stream_number_tx = (((uint32_t)((uint32_t *)handle->hdmatx->Instance) & 0xFFU) - 0x010UL) / 0x018UL;
    uint32_t dma_base_tx = (uint32_t)((uint32_t *)handle->hdmatx->Instance) - stream_number_tx * 0x018UL - 0x010UL;
    uint32_t stream_number_rx = (((uint32_t)((uint32_t *)handle->hdmarx->Instance) & 0xFFU) - 0x010UL) / 0x018UL;
    uint32_t dma_base_rx = (uint32_t)((uint32_t *)handle->hdmarx->Instance) - stream_number_rx * 0x018UL - 0x010UL;

    handle->Init.DataSize = SPI_DATASIZE_8BIT;
    LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_8BIT);
    handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_BYTE);
}

static void SwitchTo16Bits(PacketIoDevice *device)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->instance);
    uint32_t stream_number_tx = (((uint32_t)((uint32_t *)handle->hdmatx->Instance) & 0xFFU) - 0x010UL) / 0x018UL;
    uint32_t dma_base_tx = (uint32_t)((uint32_t *)handle->hdmatx->Instance) - stream_number_tx * 0x018UL - 0x010UL;
    uint32_t stream_number_rx = (((uint32_t)((uint32_t *)handle->hdmarx->Instance) & 0xFFU) - 0x010UL) / 0x018UL;
    uint32_t dma_base_rx = (uint32_t)((uint32_t *)handle->hdmarx->Instance) - stream_number_rx * 0x018UL - 0x010UL;

    handle->Init.DataSize = SPI_DATASIZE_16BIT;
    LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_16BIT);
    handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_HALFWORD);
    LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_HALFWORD);
}

static DEVICE_STATUS TxN8(PacketIoDevice *device, uint8_t *data, uint32_t size)
{
    SwitchTo8Bits(device);

    if (HAL_SPI_Transmit(device->instance, data, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS RxN8(PacketIoDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount)
{
    SwitchTo8Bits(device);
    if (HAL_SPI_Receive(device->instance, buffer, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS TxN16(PacketIoDevice *device, uint16_t *data, uint32_t size)
{
    SwitchTo16Bits(device);
    if (HAL_SPI_Transmit(device->instance, (uint8_t *)data, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS RxN16(PacketIoDevice *device, uint16_t *buffer, uint32_t size, uint8_t dummyCycleCount)
{
    SwitchTo16Bits(device);
    if (HAL_SPI_Receive(device->instance, (uint8_t *)buffer, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS TxN8Async(PacketIoDevice *device, uint8_t *data, uint32_t size)
{
    SwitchTo8Bits(device);
    SCB_CleanDCache_by_Addr((uint32_t *)data, size);
    if (HAL_SPI_Transmit_DMA(device->instance, data, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS RxN8Async(struct PacketIoDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount)
{
    device->_rxBuffer.data = buffer;
    device->_rxBuffer.size = size;
    SwitchTo8Bits(device);
    if (HAL_SPI_Receive_DMA(device->instance, (uint8_t *)buffer, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS TxN16Async(struct PacketIoDevice *device, uint16_t *data, uint32_t size)
{
    SwitchTo16Bits(device);
    SCB_CleanDCache_by_Addr((uint32_t *)data, size * 2);
    if (HAL_SPI_Transmit_DMA(device->instance, (uint8_t *)data, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS RxN16Async(struct PacketIoDevice *device, uint16_t *buffer, uint32_t size, uint8_t dummyCycleCount)
{
    device->_rxBuffer.data = buffer;
    device->_rxBuffer.size = size * 2;

    SwitchTo16Bits(device);
    if (HAL_SPI_Receive_DMA(device->instance, (uint8_t *)buffer, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

void Spi_PacketIoDevice_Create(PacketIoDevice *device, SPI_HandleTypeDef *handle)
{
    device->instance = handle;
    device->Init = &Init;
    device->TxN8 = &TxN8;
    device->TxN8Async = &TxN8Async;
    device->RxN8 = &RxN8;
    device->RxN8Async = &RxN8Async;
    device->TxN16 = &TxN16;
    device->TxN16Async = &TxN16Async;
    device->RxN16 = &RxN16;
    device->RxN16Async = &RxN16Async;
    device->opMode = PACKET_IO_DEVICE_OP_MODE_SYNC | PACKET_IO_DEVICE_OP_MODE_ASYNC;

    device->_rxBuffer.data = 0;
    device->_rxBuffer.size = 0;

    device->onError = NULL;
    device->onRxComplete = NULL;
    device->onTxComplete = NULL;
};
