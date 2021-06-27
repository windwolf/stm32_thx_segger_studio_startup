#include "guix_demo.h"

#include "../Drivers/common/inc/common/command.h"
#include "../Drivers/common/inc/common/device.h"
#include "../Drivers/device/inc/st77xx/st7735.h"
#include "../Drivers/driver/inc/guix/gx_st77xx_driver.h"
#include "bsp.h"
#include "stm32h7xx_hal.h"
#include "tx_api.h"
#include "gx_api.h"

#define DEMO_DISPLAY_WIDTH 160
#define DEMO_DISPLAY_HEIGHT 80

/* Define the number of pixels on the canvas */
#define DEFAULT_CANVAS_PIXELS (DEMO_DISPLAY_WIDTH * DEMO_DISPLAY_HEIGHT)
#define GX_DEMO_STACK_SIZE 4096
uint8_t gx_demo_thread_stack[GX_DEMO_STACK_SIZE];
TX_THREAD gx_demo_thread;
GX_DISPLAY gx_demo_display;
GX_CANVAS gx_demo_canvas;

/* The root window is a special GUIX background window, right on top of the canvas. */
GX_WINDOW_ROOT gx_demo_root_window;

/* GUIX Prompt displays a string. */
GX_PROMPT gx_demo_prompt;

GX_RECTANGLE gx_demo_root_window_size;

/* Memory for the frame buffer. */
GX_COLOR gx_demo_canvas_memory[DEFAULT_CANVAS_PIXELS];

extern ST77XX st7735;

/* User-defined color ID */
#define GX_COLOR_ID_BLACK GX_FIRST_USER_COLOR
#define GX_COLOR_ID_WHITE (GX_FIRST_USER_COLOR + 1)

/* User-defined color table. */
static GX_COLOR demo_color_table[] =
    {
        /* First, bring in GUIX default color table. These colors are used by GUIX internals. */
        GX_SYSTEM_DEFAULT_COLORS_DECLARE,

        /* In this demo, two color entries are added to the color table. */
        GX_COLOR_BLACK,
        GX_COLOR_WHITE};

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
    st7735_graphics_driver_set(&st7735);

    gx_system_initialize();

    gx_display_create(&gx_demo_display, "gx_demo",
                      &st7735_graphics_driver_rgb565_setup,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);
    /* Create the default canvas. */
    gx_canvas_create(&gx_demo_canvas, "demo canvas", &gx_demo_display,
                     GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                     DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT,
                     gx_demo_canvas_memory, sizeof(gx_demo_canvas_memory));

    /*Define the size of the root window. */
    gx_utility_rectangle_define(&gx_demo_root_window_size, 0, 0,
                                DEMO_DISPLAY_WIDTH - 1, DEMO_DISPLAY_HEIGHT - 1);

    /* Create a background root window and attach to the canvas. */
    gx_window_root_create(&gx_demo_root_window, "demo root window", &default_canvas,
                          GX_STYLE_BORDER_NONE, GX_ID_NONE, &gx_demo_root_window_size);

    /* Set the root window to be black. */
    gx_widget_background_set(&gx_demo_root_window, GX_COLOR_ID_BLACK,
                             GX_COLOR_ID_BLACK);

    /* Create a text prompt on the root window. Set the text color to white, and the background to black. */

    /* Define the size and the position of the prompt. */
    gx_utility_rectangle_define(&gx_demo_prompt, 100, 90, 220, 130);

    /* Create the prompt on top of the root window using the string defined by string ID SID_HELLO_WORLD. */
    gx_prompt_create(&gx_demo_prompt, NULL, &gx_demo_root_window, SID_HELLO_WORLD,
                     GX_STYLE_NONE, GX_ID_NONE, &gx_demo_prompt_position);

    /* Set the text color to be white, and the background color to be black. */
    gx_prompt_text_color_set(&gx_demo_prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    gx_widget_background_set(&gx_demo_prompt, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);

    /* Show the root window. */
    gx_widget_show(&gx_demo_root_window);

    /* let GUIX run! */
    gx_system_start();
}
