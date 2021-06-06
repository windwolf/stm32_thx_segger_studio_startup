#ifndef ___PIN_H__
#define ___PIN_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/device.h"
#include "basic/pin_device.h"

    typedef enum PIN_STATUS
    {
        PIN_STATUS_RESET = 0U,
        PIN_STATUS_SET
    } PIN_STATUS;

    typedef struct Pin
    {
        PinDevice device;
        uint16_t pinMask;
    } Pin;

    DEVICE_STATUS Pin_Init(Pin *pin);

    DEVICE_STATUS Pin_Set(Pin *pin, PIN_STATUS state);
    DEVICE_STATUS Pin_Get(Pin *pin, uint16_t *value);
    DEVICE_STATUS Pin_Toggle(Pin *pin);

#ifdef __cplusplus
}
#endif

#endif // ___PIN_H__