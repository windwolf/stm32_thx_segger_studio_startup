
#include "demo.h"
#include "mem_layout.h"
#include "stdint.h"

#include "filex_demo.h"
#include "guix_demo.h"
#include "st77xx_demo.h"
#include "w25qxx_demo.h"
#include "uart_stream_demo.h"
#include "communication_protocol/inc/message_parser_test.h"
#include "gps_demo.h"
#include "usb_storage_demo.h"

#define LOG_MODULE "demo"
#include "log.h"

void app_task_create(void *first_unused_memory)
{

    //st77xx_demo();

    message_parser_test();

    w25qxx_demo();

    uart_stream_demo();

    fx_application_define();

    guix_demo_application_define();

    gps_application_define();

    usb_storage_demo();
}

/* Define the test threads.  */

// extern DCMI_HandleTypeDef hdcmi;

// static void dcmi_test()
// {
//     HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, dcmi_buf, DCMI_BUFFER_SIZE);
// }
