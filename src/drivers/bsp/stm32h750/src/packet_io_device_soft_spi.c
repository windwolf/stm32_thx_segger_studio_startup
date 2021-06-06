// #include "packet_io_device_soft_spi.h"

// #define SoftSpi_Clk_Set(port, pin, cpol) (HAL_GPIO_WritePin(port, pin, cpol))
// #define SoftSpi_Clk_Reset(port, pin, cpol) (HAL_GPIO_WritePin(port, pin, !cpol))
// #define Mosi_WriteBit(port, pin, bit) (HAL_GPIO_WritePin(port, pin, ((bit) != 0)))
// #define Miso_Read_Msb(port, pin, data) (data = (data << 1) + HAL_GPIO_ReadPin(port, pin))
// #define Miso_Read_Lsb(port, pin, data, bitNum) (data += (HAL_GPIO_ReadPin(port, pin) << bitNum))

// static inline void Read_Dummy(GPIO_TypeDef *clkPort, uint16_t clkPin, uint8_t cpol)
// {
//     SoftSpi_Clk_Set(clkPort, clkPin, cpol);
//     SoftSpi_Clk_Reset(clkPort, clkPin, cpol); //sample
// }

// static inline uint8_t Read_Msb(GPIO_TypeDef *clkPort, uint16_t clkPin, GPIO_TypeDef *misoPort, uint16_t misoPin, uint8_t cpha, uint8_t cpol)
// {
//     uint8_t data;
//     for (uint8_t i = 0; i < 8; i++)
//     {
//         if (cpha == SOFTSPI_CLOCK_PHASE_1_EDGE)
//         {
//             SoftSpi_Clk_Set(clkPort, clkPin, cpol);
//             Miso_Read_Msb(misoPort, misoPin, data);

//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol); //sample
//         }
//         else
//         {
//             Miso_Read_Msb(misoPort, misoPin, data);

//             SoftSpi_Clk_Set(clkPort, clkPin, cpol); //sample
//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol);
//         }
//     }
//     return data;
// }

// static inline uint8_t Read_Lsb(GPIO_TypeDef *clkPort, uint16_t clkPin, GPIO_TypeDef *misoPort, uint16_t misoPin, uint8_t cpha, uint8_t cpol)
// {
//     uint8_t data;
//     for (uint8_t i = 0; i < 8; i++)
//     {
//         if (cpha == SOFTSPI_CLOCK_PHASE_1_EDGE)
//         {
//             SoftSpi_Clk_Set(clkPort, clkPin, cpol);
//             Miso_Read_Lsb(misoPort, misoPin, data, i);

//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol); //sample
//         }
//         else
//         {
//             Miso_Read_Lsb(misoPort, misoPin, data, i);

//             SoftSpi_Clk_Set(clkPort, clkPin, cpol); //sample
//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol);
//         }
//     }
//     return data;
// }

// static inline void Write_Msb(GPIO_TypeDef *clkPort, uint16_t clkPin, GPIO_TypeDef *mosiPort, uint16_t mosiPin, uint8_t data, uint8_t cpha, uint8_t cpol)
// {
//     for (uint8_t i = 0x80; i != 0; i >>= 1)
//     {
//         if (cpha == SOFTSPI_CLOCK_PHASE_1_EDGE)
//         {
//             SoftSpi_Clk_Set(clkPort, clkPin, cpol);
//             Mosi_WriteBit(mosiPort, mosiPin, data & i);

//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol); //sample
//         }
//         else
//         {
//             Mosi_WriteBit(mosiPort, mosiPin, data & i);

//             SoftSpi_Clk_Set(clkPort, clkPin, cpol); //sample
//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol);
//         }
//     }
// }

// static inline void Write_Lsb(GPIO_TypeDef *clkPort, uint16_t clkPin, GPIO_TypeDef *mosiPort, uint16_t mosiPin, uint8_t data, uint8_t cpha, uint8_t cpol)
// {
//     for (uint8_t i = 0x01; i != 0; i <<= 1)
//     {
//         if (cpha == SOFTSPI_CLOCK_PHASE_1_EDGE)
//         {
//             SoftSpi_Clk_Set(clkPort, clkPin, cpol);
//             Mosi_WriteBit(mosiPort, mosiPin, data & i);

//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol); //sample
//         }
//         else
//         {
//             Mosi_WriteBit(mosiPort, mosiPin, data & i);

//             SoftSpi_Clk_Set(clkPort, clkPin, cpol); //sample
//             SoftSpi_Clk_Reset(clkPort, clkPin, cpol);
//         }
//     }
// }

// static void Init(CommandMasterDevice *device){
//     //SoftSpi *spi = (SoftSpi *)device->instance;
// };

// static DEVICE_STATUS TxN(CommandMasterDevice *device, uint8_t *data, uint32_t size)
// {
//     SoftSpi *spi = (SoftSpi *)device->instance;
//     GPIO_TypeDef *clkPort = spi->clkPort;
//     uint16_t clkPin = spi->clkPin;
//     SOFTSPI_CLOCK_PHASE cpha = spi->cpha;
//     SOFTSPI_CLOCK_POLARITY cpol = spi->cpol;

//     GPIO_TypeDef *mosiPort = spi->mosiPort;
//     uint16_t mosiPin = spi->mosiPin;
//     if (spi->firstBit == SOFTSPI_FIRST_BIT_MSB)
//     {
//         for (uint32_t i = 0; i < size; i++)
//         {
//             Write_Msb(clkPort, clkPin, mosiPort, mosiPin, data[i], cpha, cpol);
//         }
//     }
//     else
//     {
//         for (uint32_t i = 0; i < size; i++)
//         {
//             Write_Lsb(clkPort, clkPin, mosiPort, mosiPin, data[i], cpha, cpol);
//         }
//     }

//     return DEVICE_STATUS_OK;
// };
// static DEVICE_STATUS RxN(struct CommandMasterDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount)
// {
//     SoftSpi *spi = (SoftSpi *)device->instance;
//     GPIO_TypeDef *clkPort = spi->clkPort;
//     uint16_t clkPin = spi->clkPin;
//     SOFTSPI_CLOCK_PHASE cpha = spi->cpha;
//     SOFTSPI_CLOCK_POLARITY cpol = spi->cpol;

//     GPIO_TypeDef *misoPort = spi->misoPort;
//     uint16_t misoPin = spi->misoPin;

//     while (dummyCycleCount > 0)
//     {
//         Read_Dummy(clkPort, clkPin, cpol);
//         dummyCycleCount--;
//     }

//     if (spi->firstBit == SOFTSPI_FIRST_BIT_MSB)
//     {
//         for (uint32_t i = 0; i < size; i++)
//         {
//             buffer[i] = Read_Msb(clkPort, clkPin, misoPort, misoPin, cpha, cpol);
//         }
//     }
//     else
//     {
//         for (uint32_t i = 0; i < size; i++)
//         {
//             buffer[i] = Read_Lsb(clkPort, clkPin, misoPort, misoPin, cpha, cpol);
//         }
//     }

//     return DEVICE_STATUS_OK;
// };
// static DEVICE_STATUS TxNAsync(struct CommandMasterDevice *device, uint8_t *data, uint32_t size)
// {
//     return DEVICE_STATUS_NOT_SUPPORT;
// };
// static DEVICE_STATUS RxNAsync(struct CommandMasterDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount)
// {
//     return DEVICE_STATUS_NOT_SUPPORT;
// };

// void SoftSpi_CommandDevice_Create(CommandMasterDevice *device, SoftSpi *instance)
// {
//     device->instance = instance;
//     device->Init = &Init;
//     device->TxN = &TxN;
//     device->RxN = &RxN;
//     device->TxNAsync = &TxNAsync;
//     device->RxNAsync = &RxNAsync;
//     device->opMode = COMMAND_DEVICE_OP_MODE_SYNC;
// }