#include "gps_demo.h"
#include "stm32h7xx_hal.h"
#include "../Drivers/common/inc/common/stream.h"
#include "../Drivers/common/inc/common/device.h"
#include "bsp.h"
#include "stdint.h"
#include "tx_api.h"
#include "communication_protocol/inc/message_parser.h"
#include "communication_protocol/inc/nmea.h"
#include "shared.h"

extern UART_HandleTypeDef huart4;
static UartDevice uartDevice;
static RAM2_BUFFER uint8_t uartRxBuffer[1024];
static RingBuffer uartRxRingBuffer;
static Stream stream;
static MessageParser msgParser;
static NmeaParser nmeaParser;
static MessageSchema msgSchema = {
    .mode = MESSAGE_SCHEMA_MODE_FREE_LENGTH,
    .prefix = {'$'},
    .prefixSize = 1,
    .suffix = {'\r', '\n'},
    .suffixSize = 2,
    .crc.length = MESSAGE_SCHEMA_SIZE_NONE,
};

#define THREAD_GPS_STACK_SIZE 4096
static uint8_t thread_gps_stack[THREAD_GPS_STACK_SIZE];
static TX_THREAD thread_gps;

void gps_application_define()
{
    ringbuffer_create(&uartRxRingBuffer, uartRxBuffer, 1, 1024);
    uart_device_create(&uartDevice, &huart4, 4);
    stream_create(&stream, &uartDevice, &uartRxRingBuffer);

    message_parser_create(&msgParser, "", &msgSchema, &uartRxBuffer);
    nmea_sentence_register_default(&msgParser);

    stream_server_start(&stream);
    tx_thread_create(&thread_gps, "thread 2", thread_gps_entry, 0,
                     thread_gps_stack, THREAD_GPS_STACK_SIZE,
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

static char sentenceBuf[1024];
void thread_gps_entry(ULONG thread_input)
{
    while (1)
    {
        stream_receive_ready_wait(&stream, TX_WAIT_FOREVER);
        uint32_t len = ringbuffer_count_get(&uartRxRingBuffer);
        if (len > 0)
        {
            OP_RESULT rst;
            MessageFrame frame;
            rst = message_parser_frame_get(&msgParser, NULL, &frame);
            if (rst == OP_RESULT_OK)
            {
                if (frame.length < 1023)
                {
                    NmeaSentenceEntry *entry;
                    rst = message_parser_frame_extract(&frame, sentenceBuf);
                    if (rst == OP_RESULT_OK)
                    {
                        message_parser_frame_release(&frame);
                        sentenceBuf[frame.length] = '\0';
                        if (nmea_sentence_entry_get(&nmeaParser, sentenceBuf, false, &entry))
                        {
                            if (entry->id == NMEA_SENTENCE_RMC)
                            {

                                NmeaSentenceRmc rmc;
                                if (nmea_parse(entry, &rmc, sentenceBuf))
                                {
                                    //TODO: do something.
                                }
                            }
                        }
                    }
                    else
                    {
                        message_parser_frame_release(&frame);
                    }
                }
            }

            tx_thread_sleep(10);
        }
    }
}
