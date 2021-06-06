#ifndef ___COMMAND_SPI_H__
#define ___COMMAND_SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/packet_io_device.h"
#include "bsp.h"
#include "basic/buffer.h"

    void Spi_PacketIoDevice_Create(PacketIoDevice *device, SPI_HandleTypeDef *instance);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_SPI_H__