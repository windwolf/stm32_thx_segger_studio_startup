#ifndef ___PIN_GPIO_H__
#define ___PIN_GPIO_H__ 

#ifdef __cplusplus
extern "C" {
#endif

#include "basic/pin.h"
#include "bsp.h"

    void Gpio_PinDevice_Create(PinDevice *device, GPIO_TypeDef *handle);

#ifdef __cplusplus
}
#endif

#endif	// ___PIN_GPIO_H__