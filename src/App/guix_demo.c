#include "guix_demo.h"

#include "../Drivers/common/inc/common/command.h"
#include "../Drivers/common/inc/common/device.h"
#include "../Drivers/device/inc/st77xx/st7735.h"
#include "../Drivers/driver/inc/guix/gx_st77xx_driver.h"
#include "bsp.h"
#include "stm32h7xx_hal.h"
#include "tx_api.h"
#include "gx_api.h"
#include "h750_180_90_resources.h"
#include "h750_180_90_specifications.h"

#define LOG_MODULE "guix"
#include "log.h"

/* Define the number of pixels on the canvas */
#define DEFAULT_CANVAS_PIXELS (MAIN_DISPLAY_WIDTH * MAIN_DISPLAY_HEIGHT)
#define GX_DEMO_STACK_SIZE 4096
uint8_t gx_demo_thread_stack[GX_DEMO_STACK_SIZE];
TX_THREAD gx_demo_thread;
GX_DISPLAY gx_demo_display;
GX_WINDOW_ROOT *root_window;

extern SPI_HandleTypeDef hspi4;
static PinDevice csPin;
static PinDevice dcPin;
static SpiDevice spi4Dev;
static SpiWithPinsDevice spi4pDev;
static CommandSpi st7735_cmd;
#define DISPLAY_MEMORY_SIZE (MAIN_DISPLAY_X_RESOLUTION) * (MAIN_DISPLAY_Y_RESOLUTION)
static RAM2_BUFFER uint16_t display_memory[DISPLAY_MEMORY_SIZE];
static RAM2_BUFFER ST77XX st7735;

/* User-defined color ID */
#define GX_COLOR_ID_BLACK GX_FIRST_USER_COLOR
#define GX_COLOR_ID_WHITE (GX_FIRST_USER_COLOR + 1)

/* User-defined color table. */
// static GX_COLOR demo_color_table[] =
//     {
//         /* First, bring in GUIX default color table. These colors are used by GUIX internals. */
//         GX_SYSTEM_DEFAULT_COLORS_DECLARE,

//         /* In this demo, two color entries are added to the color table. */
//         GX_COLOR_BLACK,
//         GX_COLOR_WHITE};

static void guix_demo_worker();

void guix_demo_application_define()
{
    tx_thread_create(&gx_demo_thread, "gx_demo",
                     &guix_demo_worker, 0,
                     gx_demo_thread_stack, GX_DEMO_STACK_SIZE,
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
};

static void guix_demo_worker()
{
    LOG_I("worker begin.");
    pin_device_create(&dcPin, GPIOE, GPIO_PIN_13, PIN_DEVICE_STATUS_INVERSE_NORMAL);
    spi_device_create(&spi4Dev, &hspi4, 4);
    spi_with_pins_device_create(&spi4pDev, &spi4Dev, NULL, NULL, &dcPin);
    command_spi_create(&st7735_cmd, &spi4pDev);
    st7735_create(&st7735, (Command *)&st7735_cmd);

    st7735.xOffset = 1;
    st7735.yOffset = 26;
    st7735.width = 160;
    st7735.height = 80;
    st7735.colorMode = ST7735_COLOR_MODE_16BIT;
    st7735.orientation = ST7735_DISPLAY_DIRECTION_XY_EXCHANGE_Y_MIRROR | ST7735_DISPLAY_COLOR_DIRECTION_BGR | ST7735_DISPLAY_REFRESH_ORDER_T2B_L2R;
    st7735_reset(&st7735);
    st7735_graphics_driver_rgb565_instance_set(&st7735);
    gx_system_initialize();
    gx_demo_display.gx_display_driver_data = &st7735;

    gx_studio_display_configure(MAIN_DISPLAY, &st7735_graphics_driver_rgb565_setup,
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root_window);
    gx_canvas_memory_define(root_window->gx_window_root_canvas, (GX_COLOR *)display_memory, DISPLAY_MEMORY_SIZE);
    gx_studio_named_widget_create("window", (GX_WIDGET *)root_window, GX_NULL);

    /* Show the root window to make it visible. */
    gx_widget_show(root_window);

    /* Let GUIX run. */
    gx_system_start();
    LOG_I("gx system started");
}
