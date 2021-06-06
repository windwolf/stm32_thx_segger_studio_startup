#include "pin_gpio.h"

static void Init(struct PinDevice *device){

};

static DEVICE_STATUS Write(struct PinDevice *device, uint16_t pinMask, uint8_t value)
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)device->instance, pinMask, value);
    return DEVICE_STATUS_OK;
};

static DEVICE_STATUS Read(struct PinDevice *device, uint16_t pinMask, uint16_t *value)
{
    *value = HAL_GPIO_ReadPin((GPIO_TypeDef *)device->instance, pinMask);
    return DEVICE_STATUS_OK;
};

static DEVICE_STATUS Toggle(struct PinDevice *device, uint16_t pinMask)
{
    HAL_GPIO_TogglePin((GPIO_TypeDef *)device->instance, pinMask);
    return DEVICE_STATUS_OK;
};

void Gpio_PinDevice_Create(PinDevice *device, GPIO_TypeDef *handle)
{
    device->instance = handle;
    device->Init = &Init;
    device->Read = &Read;
    device->Write = &Write;
    device->Toggle = &Toggle;
}