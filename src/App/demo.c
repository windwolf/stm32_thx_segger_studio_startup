
#include "demo.h"
#include "basic/ring_buffer8.h"
#include "mem_layout.h"
#include "basic/stream.h"
#include "basic/command.h"
#include "basic/pin.h"
#include "stream_io_device_uart.h"
#include "packet_io_device_spi.h"
#include "pin_gpio.h"
#include "stm32h7xx_ll_spi.h"
#include "st77xx/st7735.h"
#include "w25qxx/w25qxx_spi.h"
#include "tracex.h"
#define W25QXX_BUFFER_SIZE 2048
extern SPI_HandleTypeDef hspi4;
Pin csPin;
Pin dcPin;
D2_BUFFER PacketIoDevice spi4_pio;
D2_BUFFER ST77XX st7735;

extern UART_HandleTypeDef huart4;
D2_BUFFER uint8_t txBuf0[64];
D2_BUFFER uint8_t testRxBuf[65];
D2_BUFFER RingBuffer8 buffer2;
D2_BUFFER Stream stream;

extern QSPI_HandleTypeDef hqspi;

extern SPI_HandleTypeDef hspi1;
Pin csPin_1;
D2_BUFFER PacketIoDevice spi1_pio;
D2_BUFFER SimpleCommand w25qxx_cmd;

D2_BUFFER uint8_t w25qxx_1_buf1[W25QXX_BUFFER_SIZE];
D2_BUFFER W25QXX_SPI w25qxx_1;

int32_t cWrite = 0;
int32_t cRead = 0;
D2_DATA const char *text = "test wolf!";
TX_THREAD thread_0;
TX_THREAD thread_1;

TX_BYTE_POOL byte_pool_0;

UCHAR memory_area[DEMO_BYTE_POOL_SIZE];

ULONG thread_0_counter;
ULONG thread_1_counter;

void init_driver()
{
    RingBuffer8_Create(&buffer2, testRxBuf, 65);

    Uart_StreamDevice_Create((StreamIoDevice *)&stream, &huart4);
    Stream_Init(&stream, &buffer2);
    Stream_StartServer(&stream);

    // Gpio_PinDevice_Create((PinDevice *)&csPin, GPIOE);
    // Pin_Init(&csPin);
    // csPin.pinMask = GPIO_PIN_11;

    Gpio_PinDevice_Create((PinDevice *)&dcPin, GPIOE);
    Pin_Init(&dcPin);
    dcPin.pinMask = GPIO_PIN_13;

    spi_packet_io_device_create(&spi4_pio, &hspi4);
    simple_command_create(&st7735.command, &spi4_pio,
                          NULL, NULL,
                          NULL, NULL,
                          &dcPin, COMMAND_DATACMD_PIN_MODE_DATA);

    ST7735_Init(&st7735);

    spi_packet_io_device_create(&spi1_pio, &hspi1);
    Gpio_PinDevice_Create((PinDevice *)&csPin_1, GPIOD);
    Pin_Init(&csPin_1);
    csPin_1.pinMask = GPIO_PIN_6;
    simple_command_create(&w25qxx_cmd, &spi1_pio, &csPin_1, COMMAND_SELECT_PIN_MODE_UNSELECT, NULL, NULL, NULL, NULL);
    Buffer buf = {.data = w25qxx_1_buf1, .size = W25QXX_BUFFER_SIZE};
    w25qxx_spi_create(&w25qxx_1, buf, &w25qxx_cmd);
}

void tx_application_define(void *first_unused_memory)
{
    TraceX_EnableTrace();
    init_driver();
    // UartDevice_Init(&uart, &huart4, &buffer2);
    // UartDevice_StartServer(&uart);

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
}

/* Define the test threads.  */

#define LCD_DATA_SIZE 100
D2_BUFFER static uint16_t lcddata[LCD_DATA_SIZE];
void test05_init()
{
    st7735.xOffset = 1;
    st7735.yOffset = 26;
    st7735.width = 160;
    st7735.height = 80;
    st7735.colorMode = ST7735_COLOR_MODE_16BIT;
    st7735.orientation = ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_Y_MIRROR | ST7735_DISPLAY_COLOR_DIRECTION_BGR | ST7735_DISPLAY_REFRESH_ORDER_T2B_L2R;
    ST7735_Reset(&st7735);
    //ST7735_Inversion(&st7735, 1);
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
    ST7735_DrawRect(&st7735, 0, 0, st7735.width, st7735.height, color0); //inv:1=red; inv:0=yellow
    ST7735_DrawHLine(&st7735, 10, 10, 20, color1);                       //inv:1=red+green; inv:0=sky+pink
    ST7735_DrawRect(&st7735, 20, 20, 10, 10, color2);                    //inv:1=blue; inv:0=sky
    ST7735_DrawRect(&st7735, 40, 20, 10, 10, color3);                    //inv:1=red; inv:0=yellow
}

void thread_0_entry(ULONG thread_input)
{
    float num = 0.1;
    //UINT status;
    // UartDevice_Tx(&uart, (uint8_t *)text, strlen(text));
    // UartDevice_WaitForTxComplete(&uart, TX_WAIT_FOREVER);
    Stream_Tx(&stream, (uint8_t *)text, strlen(text));
    Stream_WaitForTxComplete(&stream, TX_WAIT_FOREVER);

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

D2_DATA uint32_t w25qxx_1_id;
D1_BUFFER uint8_t w25qxx_data_buf[500];
void thread_1_entry(ULONG thread_input)
{

    //UINT status;
    w25qxx_spi_reset(&w25qxx_1);
    w25qxx_spi_read_id(&w25qxx_1, &w25qxx_1_id);
    LOG("W25QXX: id=%d", w25qxx_1_id);
    w25qxx_spi_write(&w25qxx_1, &w25qxx_1_id, 0x0400, 4);
    w25qxx_spi_read(&w25qxx_1, w25qxx_data_buf, 0x0400, 4);
    LOG("W25QXX: r=%d", *((uint32_t*)w25qxx_data_buf));
    /* This thread simply sends messages to a queue shared by thread 2.  */
    while (1)
    {
        Stream_WaitForRxReady(&stream, TX_WAIT_FOREVER);
        uint32_t len = RingBuffer8_GetCount(&buffer2);
        if (len > 0)
        {

            RingBuffer8_Read(&buffer2, txBuf0, len);
            Stream_Tx(&stream, txBuf0, len);
            w25qxx_spi_write(&w25qxx_1, txBuf0, 0x0400, len);
            w25qxx_spi_read(&w25qxx_1, w25qxx_data_buf, 0x0400, len);
            LOG("W25QXX: ri=%s", *((uint32_t*)w25qxx_data_buf));
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
