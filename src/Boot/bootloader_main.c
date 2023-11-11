#include "stm32h7xx_hal.h"
#include "dcmi.h"
#include "dma.h"
#include "i2c.h"
#include "quadspi.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

#include "bootloader_qspi.h"
#include "app_main.h"
#include "unit_test.h"

#define LOG_MODULE "BOOT"
#include "log.h"

//#include "../cmbBackTrace/cm_backtrace.h"

extern void MPU_Config(void);
extern void SystemClock_Config();

int main(void)
{
    LOG_US("system begin");
    /* USER CODE BEGIN 1 */
    HAL_MPU_Disable();
    LOG_US("mpu disabled");

    MPU_Config();
    /* USER CODE END 1 */

    /* Enable I-Cache--------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
    LOG_US("mpu configed");
    /* MCU Configuration--------------------------------------------------------*/
#ifdef UNITTEST
    LOG_US("test before hal init begin");
    test_before_hal_init();
    LOG_US("test before hal init end");
#endif
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LOG_US("hal init begin");
    HAL_Init();
    LOG_US("hal init end");
    //cm_backtrace_init("startup", "1.0", "0.1");
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */
    /* Configure the system clock */
    SystemClock_Config();
    LOG_US("system clock configed");
    /* USER CODE BEGIN SysInit */

    HAL_Delay(100);
    MX_QUADSPI_Init();
    LOG_US("qspi inited");

    bootloader_qspi_startup();
    LOG_US("qspi flash configed");
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    LOG_US("gpio inited");
    MX_DMA_Init();
    LOG_US("dma inited");
    //MX_MDMA_Init();
    MX_USART1_UART_Init();
    LOG_US("usart1 inited");
    MX_DCMI_Init();
    LOG_US("dcmi inited");
    MX_I2C1_Init();
    LOG_US("i2c1 inited");
    MX_SDMMC1_SD_Init();
    LOG_US("sdmmc1 inited");
    MX_SPI1_Init();
    LOG_US("spi1 inited");
    MX_SPI4_Init();
    LOG_US("spi4 inited");
    MX_TIM1_Init();
    LOG_US("tim1 inited");
    MX_USB_OTG_FS_PCD_Init();
    LOG_US("usb otg fs inited");
    MX_UART4_Init();
    LOG_US("uart4 inited");
    /* USER CODE BEGIN 2 */
#ifdef UNITTEST
    LOG_US("test after hal init begin");
    test_after_hal_init();
    LOG_US("test after hal init end");
#endif

    LOG_US("app main begin");
    appMain();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}