
#include "demo.h"
#include "mem_layout.h"
#include "../Drivers/common/inc/common/stream.h"
#include "../Drivers/common/inc/common/command.h"
#include "../Drivers/common/inc/common/device.h"
#include "../Drivers/common/inc/common/block.h"
#include "bsp.h"
#include "stm32h7xx_ll_spi.h"
#include "../Drivers/device/inc/st77xx/st7735.h"
#include "../Drivers/device/inc/w25qxx/w25qxx.h"
#include "tracex.h"
#include "stdint.h"
#include "filex_demo.h"

#define LOG_MODULE "demo"
#include "log.h"

extern UART_HandleTypeDef huart4;
RAM2_BUFFER UartDevice uartDevice;
RAM2_BUFFER uint8_t txBuf0[64];
RAM2_BUFFER uint8_t uartRxBuffer[65];
RAM2_BUFFER RingBuffer uartRxRingBuffer;
RAM2_BUFFER Stream stream;

extern SPI_HandleTypeDef hspi4;
PinDevice csPin;
PinDevice dcPin;
RAM2_BUFFER SpiDevice spi4Dev;
RAM2_BUFFER SpiWithPinsDevice spi4pDev;
RAM2_BUFFER CommandSpi st7735_cmd;
#define ST7735_BUFFER_SIZE 48114
RAM2_BUFFER uint8_t st7735Buffer[ST7735_BUFFER_SIZE];
RAM2_BUFFER ST77XX st7735;

RAM1_BUFFER uint8_t data1_buf[500];

#define W25QXX_BUFFER_SIZE W25QXX_BLOCK_SIZE

extern SPI_HandleTypeDef hspi1;
uint32_t w25qxx_1_id;
RAM2_BUFFER PinDevice csPin_1;
RAM2_BUFFER SpiDevice spi1Dev;
RAM2_BUFFER SpiWithPinsDevice spi1pDev;
RAM2_BUFFER CommandSpi w25qxx_1_cmd;
RAM2_BUFFER uint8_t w25qxx_1_buf[W25QXX_BUFFER_SIZE];
RAM2_BUFFER W25QXX w25qxx_1;
RAM2_BUFFER Block block1;

RAM2_DATA const char *placehold = "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634"
                                  "856745624652456456347576567sdgwer yy35hewhaeh nwyr5j5 wjrtnsf8vansq3pg9 qerbapeuv qp34uejkv a;sdsvj asfwao gawogi a'oig g4562465246245626456245634";
extern SD_HandleTypeDef hsd1;
SdDevice sdDevice;
Block sd_block;
AXI_BUFFER uint8_t sd_buffer[512];

int32_t cWrite = 0;
int32_t cRead = 0;
RAM2_DATA const char *text = "test wolf!";
TX_THREAD thread_0;
TX_THREAD thread_1;

TX_BYTE_POOL byte_pool_0;

UCHAR memory_area[DEMO_BYTE_POOL_SIZE];

ULONG thread_0_counter;
ULONG thread_1_counter;

static void qspi_test();

static void dcmi_test();

void init_driver()
{
    ringbuffer_create(&uartRxRingBuffer, uartRxBuffer, 65);
    uart_device_create(&uartDevice, &huart4, 0);
    stream_create(&stream, &uartDevice, &uartRxRingBuffer);
    stream_server_start(&stream);

    pin_device_create(&dcPin, GPIOE, GPIO_PIN_13, PIN_DEVICE_STATUS_INVERSE_NORMAL);
    spi_device_create(&spi4Dev, &hspi4, 4);
    spi_with_pins_device_create(&spi4pDev, &spi4Dev, NULL, NULL, &dcPin);
    command_spi_create(&st7735_cmd, &spi4pDev);
    Buffer buf1 = {.data = st7735Buffer, .size = ST7735_BUFFER_SIZE};
    st7735_create(&st7735, (Command *)&st7735_cmd, buf1);

    pin_device_create(&csPin_1, GPIOD, GPIO_PIN_6, PIN_DEVICE_STATUS_INVERSE_INVERSE);
    spi_device_create(&spi1Dev, &hspi1, 4);
    spi_with_pins_device_create(&spi1pDev, &spi1Dev, &csPin_1, NULL, NULL);
    command_spi_create(&w25qxx_1_cmd, &spi1pDev);
    Buffer buf2 = {.data = w25qxx_1_buf, .size = W25QXX_BUFFER_SIZE};
    w25qxx_create(&w25qxx_1, (Command *)&w25qxx_1_cmd, 0);
    w25qxx_block_create(&w25qxx_1, &block1, buf2);

    sd_device_create(&sdDevice, &hsd1, 4);
    sd_device_card_init(&sdDevice);
    Buffer buf = {.data = sd_buffer, .size = 512};
    sd_device_block_create(&sdDevice, &sd_block, buf);

    // HAL_SD_CardStatusTypeDef status;
    // HAL_SD_InitCard(&hsd1);
    // HAL_SD_GetCardStatus(&hsd1, &status);
    // HAL_StatusTypeDef rst = HAL_SD_ReadBlocks(&hsd1, sd_buffer, 0, 1, HAL_MAX_DELAY);
    // (*sd_buffer)++;
    // rst = HAL_SD_WriteBlocks(&hsd1, sd_buffer, 0, 1, HAL_MAX_DELAY);
    // *sd_buffer = 0;
    // rst = HAL_SD_ReadBlocks_DMA(&hsd1, sd_buffer, 0, 1);
    // HAL_Delay(100);
    // HAL_SD_StateTypeDef state = HAL_SD_GetState(&hsd1);

    // SCB_InvalidateDCache_by_Addr(sd_buffer, 512);
    // HAL_Delay(100);
    // state = HAL_SD_GetState(&hsd1);
    // SCB_InvalidateDCache_by_Addr(sd_buffer, 512);
}

void app_task_create(void *first_unused_memory)
{
    TraceX_EnableTrace();
    init_driver();

    CHAR *pointer = NULL;

    /* Create a byte memory pool from which to allocate the thread stacks.  */
    tx_byte_pool_create(&byte_pool_0, strdup("byte pool 0"), memory_area, DEMO_BYTE_POOL_SIZE);

    /* Put system definition stuff in here, e.g. thread creates and other assorted
       create information.  */

    /* Allocate the stack for thread 0.  */
    tx_byte_allocate(&byte_pool_0, (VOID **)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create the main thread.  */
    tx_thread_create(&thread_0, strdup("thread 0"), thread_0_entry, 0,
                     pointer, DEMO_STACK_SIZE,
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

    /* Allocate the stack for thread 1.  */
    tx_byte_allocate(&byte_pool_0, (VOID **)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create threads 1 and 2. These threads pass information through a ThreadX
       message queue.  It is also interesting to note that these threads have a time
       slice.  */
    tx_thread_create(&thread_1, strdup("thread 1"), thread_1_entry, 1,
                     pointer, DEMO_STACK_SIZE,
                     16, 16, 4, TX_AUTO_START);

    /* Allocate the memory for a small block pool.  */
    tx_byte_allocate(&byte_pool_0, (VOID **)&pointer, DEMO_BLOCK_POOL_SIZE, TX_NO_WAIT);

    fx_application_define();
}

/* Define the test threads.  */

#define LCD_DATA_SIZE 100
RAM2_BUFFER static uint16_t lcddata[LCD_DATA_SIZE];
void test05_init()
{
    st7735.xOffset = 1;
    st7735.yOffset = 26;
    st7735.width = 160;
    st7735.height = 80;
    st7735.colorMode = ST7735_COLOR_MODE_16BIT;
    st7735.orientation = ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_Y_MIRROR | ST7735_DISPLAY_COLOR_DIRECTION_BGR | ST7735_DISPLAY_REFRESH_ORDER_T2B_L2R;
    st7735_reset(&st7735);
    //st77xx_inversion(&st7735, 1);
    for (size_t i = 0; i < LCD_DATA_SIZE; i++)
    {
        lcddata[i] = (0xF800);
    }
}

void test05()
{
    uint16_t color0 = 0x28A5;
    uint16_t color1 = 0x001F;
    uint16_t color2 = 0xF800;
    uint16_t color3 = 0x04F1;
    st7735_rect_draw(&st7735, 0, 0, st7735.width, st7735.height, color0); //inv:1=red; inv:0=yellow
    st7735_hline_draw(&st7735, 10, 10, 20, color1);                       //inv:1=red+green; inv:0=sky+pink
    st7735_rect_draw(&st7735, 20, 20, 10, 10, color2);                    //inv:1=blue; inv:0=sky
    st7735_rect_draw(&st7735, 40, 20, 10, 10, color3);                    //inv:1=red; inv:0=yellow
}

void thread_0_entry(ULONG thread_input)
{
    float num = 0.1;
    //UINT status;
    // UartDevice_Tx(&uart, (uint8_t *)text, strlen(text));
    // UartDevice_WaitForTxComplete(&uart, TX_WAIT_FOREVER);
    stream_send(&stream, (uint8_t *)text, strlen(text));
    stream_send_cplt_wait(&stream, TX_WAIT_FOREVER);

    test05_init();
    /* This thread simply sits in while-forever-sleep loop.  */
    while (1)
    {
        test05();
        num += 0.15;
        /* Increment the thread counter.  */
        thread_0_counter++;
        /* Sleep for 1000 ticks.  */
        tx_thread_sleep(1000 + (uint32_t)(num * 100));

        // printf("thread0 running\n");
        // HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        // HAL_UART_Transmit(&CONVERT_REFERENCE(huart4, UART_HandleTypeDef), memory_area, 10, HAL_MAX_DELAY);

        /* Check status.  */
        // if (status != TX_SUCCESS)
        //     break;
    }
}

HAL_SD_CardInfoTypeDef CardInfo;
HAL_SD_CardCIDTypeDef CID;
HAL_SD_CardCSDTypeDef CSD;
HAL_SD_CardStatusTypeDef CSTA;
void thread_1_entry(ULONG thread_input)
{
    w25qxx_reset(&w25qxx_1);

    w25qxx_id_read(&w25qxx_1);
    LOG_D("W25QXX-1: mdId=%#x jedecId=%#x", w25qxx_1.mdId, w25qxx_1.jedecId);
    w25qxx_status_get(&w25qxx_1);
    LOG_D("W25QXX-1: s1=%d, s2=%d, s3=%d", w25qxx_1.status1, w25qxx_1.status2, w25qxx_1.status3);

    //qspi_test();
    dcmi_test();
    w25qxx_1_id = 0x11223344;
    LOG_D("W25QXX-1: w=%#x", w25qxx_1_id);
    block_write(&block1, (uint8_t *)&w25qxx_1_id, 0x0000, 4);
    block_read(&block1, data1_buf, 0x0000, 256);
    LOG_D("W25QXX-1: r=%#x", *((uint32_t *)data1_buf));

    while (1)
    {
        stream_receive_ready_wait(&stream, TX_WAIT_FOREVER);
        uint32_t len = ringbuffer_count_get(&uartRxRingBuffer);
        if (len > 0)
        {
            ringbuffer_read(&uartRxRingBuffer, txBuf0, len);
            stream_send(&stream, txBuf0, len);
            //w25qxx_write(&w25qxx_1, txBuf0, 0x0400, len);
            //w25qxx_read(&w25qxx_1, data_buf, 0x0400, len);
            tx_thread_sleep(10);
            //cRead++;
        }
        /* Increment the thread counter.  */
        thread_1_counter++;

        // /* Check completion status.  */
        // if (status != TX_SUCCESS)
        //     break;
    }
}

// static void qspi_test()
// {
//     uint32_t rxData;
//     QSPI_CommandTypeDef cmd;
//     cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//     cmd.Instruction = W25QXX_SPI_READ_CMD;
//     cmd.AddressMode = QSPI_ADDRESS_1_LINE;
//     cmd.AddressSize = QSPI_ADDRESS_24_BITS;
//     cmd.Address = 0x1000;
//     cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//     cmd.DummyCycles = 0;
//     cmd.DataMode = QSPI_DATA_1_LINE;
//     cmd.NbData = 4;
//     cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//     cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//     cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//     HAL_QSPI_Command(&hqspi, &cmd, HAL_MAX_DELAY);
//     HAL_QSPI_Receive(&hqspi, &rxData, HAL_MAX_DELAY);

//     cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//     cmd.Instruction = W25QXX_SPI_WRITE_ENABLE_CMD;
//     cmd.AddressMode = QSPI_ADDRESS_NONE;
//     cmd.AddressSize = QSPI_ADDRESS_24_BITS;
//     cmd.Address = 0x00;
//     cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//     cmd.DummyCycles = 0;
//     cmd.DataMode = QSPI_DATA_NONE;
//     cmd.NbData = 4;
//     cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//     cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//     cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//     HAL_QSPI_Command(&hqspi, &cmd, HAL_MAX_DELAY);

//     uint8_t statusData = 0x02;
//     cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//     cmd.Instruction = W25QXX_SPI_WRITE_STATUS_REG2_CMD;
//     cmd.AddressMode = QSPI_ADDRESS_NONE;
//     cmd.AddressSize = QSPI_ADDRESS_24_BITS;
//     cmd.Address = 0x00;
//     cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//     cmd.DummyCycles = 0;
//     cmd.DataMode = QSPI_DATA_1_LINE;
//     cmd.NbData = 1;
//     cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//     cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//     cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//     HAL_QSPI_Command(&hqspi, &cmd, HAL_MAX_DELAY);
//     HAL_QSPI_Transmit(&hqspi, &statusData, HAL_MAX_DELAY);

//     cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//     cmd.Instruction = W25QXX_SPI_ENTER_QPI_MODE_CMD;
//     cmd.AddressMode = QSPI_ADDRESS_NONE;
//     cmd.AddressSize = QSPI_ADDRESS_24_BITS;
//     cmd.Address = 0x1000;
//     cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//     cmd.DummyCycles = 0;
//     cmd.DataMode = QSPI_DATA_NONE;
//     cmd.NbData = 4;
//     cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//     cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//     cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//     HAL_QSPI_Command(&hqspi, &cmd, HAL_MAX_DELAY);

//     cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//     cmd.Instruction = W25QXX_SPI_READ_CMD;
//     cmd.AddressMode = QSPI_ADDRESS_1_LINE;
//     cmd.AddressSize = QSPI_ADDRESS_24_BITS;
//     cmd.Address = 0x1000;
//     cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//     cmd.DummyCycles = 0;
//     cmd.DataMode = QSPI_DATA_1_LINE;
//     cmd.NbData = 4;
//     cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//     cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//     cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//     HAL_QSPI_Command(&hqspi, &cmd, HAL_MAX_DELAY);
//     HAL_QSPI_Receive(&hqspi, &rxData, HAL_MAX_DELAY);

//     cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
//     cmd.Instruction = W25QXX_SPI_FAST_READ_CMD;
//     cmd.AddressMode = QSPI_ADDRESS_4_LINES;
//     cmd.AddressSize = QSPI_ADDRESS_24_BITS;
//     cmd.Address = 0x1000;
//     cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//     cmd.DummyCycles = 2;
//     cmd.DataMode = QSPI_DATA_4_LINES;
//     cmd.NbData = 4;
//     cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//     cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//     cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//     HAL_QSPI_Command(&hqspi, &cmd, HAL_MAX_DELAY);
//     HAL_QSPI_Receive(&hqspi, &rxData, HAL_MAX_DELAY);
// }

extern DCMI_HandleTypeDef hdcmi;
#define DCMI_BUFFER_SIZE (320 * 240 * 1)
AXI_BUFFER uint8_t dcmi_buf[DCMI_BUFFER_SIZE];

static void dcmi_test()
{
    HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, dcmi_buf, DCMI_BUFFER_SIZE);
}
