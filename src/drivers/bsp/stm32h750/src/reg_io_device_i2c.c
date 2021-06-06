#include "reg_io_device_i2c.h"

typedef enum I2C_INDEX
{
    I2C1_INDEX = 0,
    I2C2_INDEX,
    I2C3_INDEX,
    I2C4_INDEX,
    I2C_COUNT,
} I2C_INDEX;

static RegIoDevice *I2C_INSTANCE_[(uint32_t)(I2C_COUNT)];

static inline I2C_INDEX Instance_Index_Get(I2C_HandleTypeDef *handle)
{
    uint32_t ins = (uint32_t)(handle->Instance);

    switch (ins)
    {
    case I2C1_BASE:
        return I2C1_INDEX;
    case I2C2_BASE:
        return I2C2_INDEX;
    case I2C3_BASE:
        return I2C3_INDEX;
    case I2C4_BASE:
        return I2C4_INDEX;
    default:
        return I2C_COUNT;
    }
}

static inline void Instance_Register(RegIoDevice *device)
{
    I2C_INDEX index = Instance_Index_Get((I2C_HandleTypeDef *)(device->instance));
    I2C_INSTANCE_[(uint32_t)index] = device;
};

static inline RegIoDevice *Instance_Find(I2C_HandleTypeDef *huart)
{
    I2C_INDEX index = Instance_Index_Get(huart);
    return I2C_INSTANCE_[(uint32_t)index];
};

static void I2C_TxCpltCallback__(I2C_HandleTypeDef *handle)
{
    RegIoDevice *device = Instance_Find(handle);
    if (device->onWriteComplete)
    {
        device->onWriteComplete(device);
    }
};

static void I2C_RxCpltCallback__(I2C_HandleTypeDef *handle)
{
    RegIoDevice *device = Instance_Find(handle);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    if (device->onReadComplete)
    {
        device->onReadComplete(device);
    }
};

static void I2C_ErrCallback__(I2C_HandleTypeDef *handle)
{
    RegIoDevice *device = Instance_Find(handle);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    if (device->onError)
    {
        device->onError(device);
    }
};

static DEVICE_STATUS Init(RegIoDevice *device)
{
    I2C_HandleTypeDef *handle = (I2C_HandleTypeDef *)device->instance;
    Instance_Register(device);
    HAL_I2C_RegisterCallback(handle, HAL_I2C_MEM_TX_COMPLETE_CB_ID, I2C_TxCpltCallback__);
    HAL_I2C_RegisterCallback(handle, HAL_I2C_MEM_RX_COMPLETE_CB_ID, I2C_RxCpltCallback__);
    HAL_I2C_RegisterCallback(handle, HAL_I2C_ERROR_CB_ID, I2C_ErrCallback__);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS Read8(RegIoDevice *device, uint32_t regAddress, uint8_t *data, uint16_t size, uint32_t deviceAddress)
{
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = size;
    if (HAL_I2C_Mem_Read_DMA((I2C_HandleTypeDef *)device->instance, deviceAddress, regAddress, I2C_MEMADD_SIZE_8BIT, data, size) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS Read16(RegIoDevice *device, uint32_t regAddress, uint16_t *data, uint16_t size, uint32_t deviceAddress)
{
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = size * 2;
    if (HAL_I2C_Mem_Read_DMA((I2C_HandleTypeDef *)device->instance, deviceAddress, regAddress, I2C_MEMADD_SIZE_16BIT, (uint8_t*)data, size /*TODO: 确认要不要x2 */) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS Write8(RegIoDevice *device, uint32_t regAddress, uint8_t *data, uint16_t size, uint32_t deviceAddress)
{
    SCB_CleanDCache_by_Addr((uint32_t *)data, size);
    if (HAL_I2C_Mem_Write_DMA((I2C_HandleTypeDef *)device->instance, deviceAddress, regAddress, I2C_MEMADD_SIZE_8BIT, data, size) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
};
DEVICE_STATUS Write16(RegIoDevice *device, uint32_t regAddress, uint16_t *data, uint16_t size, uint32_t deviceAddress)
{
    SCB_CleanDCache_by_Addr((uint32_t *)data, size * 2);
    if (HAL_I2C_Mem_Write_DMA((I2C_HandleTypeDef *)device->instance, deviceAddress, regAddress, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, size /*TODO: 确认要不要x2 */) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
};

void I2C_RegIoDevice_Create(RegIoDevice *device, I2C_HandleTypeDef *instance)
{
    device->instance = instance;
    device->Init = &Init;
    device->Read8 = &Read8;
    device->Read16 = &Read16;
    device->Write8 = &Write8;
    device->Write16 = &Write16;
    device->onError = NULL;
    device->onReadComplete = NULL;
    device->onWriteComplete = NULL;
}