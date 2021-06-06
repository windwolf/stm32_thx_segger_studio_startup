#include "bsp.h"
#include "basic/pin.h"

DEVICE_STATUS Pin_Init(Pin *pin)
{
    pin->device.Init(&pin->device);
    pin->device.base.host = pin;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS Pin_Set(Pin *pin, PIN_STATUS state)
{
    return pin->device.Write(&pin->device, pin->pinMask, state);
};

DEVICE_STATUS Pin_Get(Pin *pin, uint16_t *value)
{
    return pin->device.Read(&pin->device, pin->pinMask, value);
};

DEVICE_STATUS Pin_Toggle(Pin *pin)
{
    return pin->device.Toggle(&pin->device, pin->pinMask);
};
