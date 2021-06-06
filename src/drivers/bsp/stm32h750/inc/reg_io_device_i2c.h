#ifndef ___COMMAND_IIC_H__
#define ___COMMAND_IIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/reg_io_device.h"
#include "bsp.h"

    void I2C_RegIoDevice_Create(RegIoDevice *device, I2C_HandleTypeDef *instance);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_IIC_H__