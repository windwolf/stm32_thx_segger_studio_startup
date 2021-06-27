#include "w25qxx_demo.h"
#include "bsp.h"
#include "tx_api.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32h7xx_hal.h"

#include "../Drivers/common/inc/common/command.h"
#include "../Drivers/common/inc/common/device.h"
#include "../Drivers/common/inc/common/block.h"
#include "../Drivers/device/inc/w25qxx/w25qxx.h"

#define LOG_MODULE "w25qxx_demo"
#include "log.h"

#define W25QXX_BUFFER_SIZE W25QXX_BLOCK_SIZE

extern SPI_HandleTypeDef hspi1;
static uint32_t w25qxx_1_id;
static RAM2_BUFFER PinDevice csPin_1;
static RAM2_BUFFER SpiDevice spi1Dev;
static RAM2_BUFFER SpiWithPinsDevice spi1pDev;
static RAM2_BUFFER CommandSpi w25qxx_1_cmd;
static RAM2_BUFFER uint8_t w25qxx_1_buf[W25QXX_BUFFER_SIZE];
static RAM2_BUFFER W25QXX w25qxx_1;
static RAM2_BUFFER Block block1;

RAM1_BUFFER uint8_t data1_buf[500];

#define THREAD1_STACK_SIZE 4096
static uint8_t thread1_stack[THREAD1_STACK_SIZE];
static TX_THREAD thread_1;

static void thread_1_entry(ULONG thread_input);

void w25qxx_demo()
{
    pin_device_create(&csPin_1, GPIOD, GPIO_PIN_6, PIN_DEVICE_STATUS_INVERSE_INVERSE);
    spi_device_create(&spi1Dev, &hspi1, 4);
    spi_with_pins_device_create(&spi1pDev, &spi1Dev, &csPin_1, NULL, NULL);
    command_spi_create(&w25qxx_1_cmd, &spi1pDev);
    Buffer buf2 = {.data = w25qxx_1_buf, .size = W25QXX_BUFFER_SIZE};
    w25qxx_create(&w25qxx_1, (Command *)&w25qxx_1_cmd, 0);
    w25qxx_block_create(&w25qxx_1, &block1, buf2);

    /* Create threads 1 and 2. These threads pass information through a ThreadX
       message queue.  It is also interesting to note that these threads have a time
       slice.  */
    tx_thread_create(&thread_1, strdup("thread 1"), thread_1_entry, 1,
                     thread1_stack, THREAD1_STACK_SIZE,
                     16, 16, 4, TX_AUTO_START);
};

static void thread_1_entry(ULONG thread_input)
{
    w25qxx_reset(&w25qxx_1);

    w25qxx_id_read(&w25qxx_1);
    LOG_D("W25QXX-1: mdId=%#x jedecId=%#x", w25qxx_1.mdId, w25qxx_1.jedecId);
    w25qxx_status_get(&w25qxx_1);
    LOG_D("W25QXX-1: s1=%d, s2=%d, s3=%d", w25qxx_1.status1, w25qxx_1.status2, w25qxx_1.status3);

    w25qxx_1_id = 0x11223344;
    LOG_D("W25QXX-1: w=%#x", w25qxx_1_id);
    block_write(&block1, (uint8_t *)&w25qxx_1_id, 0x0000, 4);
    block_read(&block1, data1_buf, 0x0000, 256);
    LOG_D("W25QXX-1: r=%#x", *((uint32_t *)data1_buf));
    while (1)
    {
        tx_thread_sleep(10);
    }
}
