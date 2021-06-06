#ifndef ___COMMAND_SOFT_SPI_H__
#define ___COMMAND_SOFT_SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/packet_io_device.h"
#include "bsp.h"

    typedef enum SOFTSPI_CLOCK_POLARITY
    {
        SOFTSPI_CLOCK_POLARITY_LOW = 0,
        SOFTSPI_CLOCK_POLARITY_HIGH = 1,

    } SOFTSPI_CLOCK_POLARITY;

    typedef enum SOFTSPI_CLOCK_PHASE
    {
        SOFTSPI_CLOCK_PHASE_1_EDGE,
        SOFTSPI_CLOCK_PHASE_2_EDGE,
    } SOFTSPI_CLOCK_PHASE;

    typedef enum SOFTSPI_FIRST_BIT
    {
        SOFTSPI_FIRST_BIT_MSB = 0,
        SOFTSPI_FIRST_BIT_LSB,
    } SOFTSPI_FIRST_BIT;

    typedef struct SoftSpi
    {
        SOFTSPI_CLOCK_POLARITY cpol;
        SOFTSPI_CLOCK_PHASE cpha;
        SOFTSPI_FIRST_BIT firstBit;
        GPIO_TypeDef *clkPort;
        uint16_t clkPin;
        GPIO_TypeDef *mosiPort;
        uint16_t mosiPin;
        GPIO_TypeDef *misoPort;
        uint16_t misoPin;
    } SoftSpi;

    void SoftSpi_CommandDevice_Create(PocketIoDevice *device, SoftSpi *instance);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_SOFT_SPI_H__