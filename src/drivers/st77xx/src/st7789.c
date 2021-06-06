// #include "st77xx/st7789.h"
// #include "basic/shared.h"
// #include "stdio.h"

// DEVICE_STATUS ST7789_Init(ST77XX *instance)
// {
//     tx_event_flags_create(&instance->events, "st7789");


//     return DEVICE_STATUS_OK;
// };
 
// DEVICE_STATUS ST7789_Reset(ST77XX *instance, ST7789_COLOR_MODE color_mode, ST7789_DISPLAY_DIRECTION displayDirection, ST7789_DISPLAY_COLOR_DIRECTION colorDirection, ST7789_DISPLAY_REFRESH_ORDER refreshOrder)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     LOG("ST7789:BSY set")

//     LOG("ST7789:SWRST s")
//     ST77XX_Command(instance, ST7789_CMD_SOFTWARE_RESET);
//     tx_thread_sleep(120);
//     LOG("ST7789:SWRST e")

//     LOG("ST7789:st PIXFMT")
//     ST77XX_Command_Write8xN(instance, ST7789_CMD_INTERFACE_PIXEL_FORMAT, color_mode, 0, 0, 0, 0, 0, 1);

//     uint8_t memSetting = displayDirection | colorDirection | refreshOrder;
//     LOG("ST7789:st MEMDAC")
//     ST77XX_Command_Write8xN(instance, ST7789_CMD_MEMORY_DATA_ACCESS_CONTROL, memSetting, 0, 0, 0, 0, 0, 1);

//     LOG("ST7789:SLPOUT s")
//     ST77XX_Command(instance, ST7789_CMD_SLEEP_OUT);
//     tx_thread_sleep(120);
//     LOG("ST7789:SLPOUT e")

//     ST77XX_EVENTS_RESET_BUSY(instance);

//     return DEVICE_STATUS_OK;
// };

// DEVICE_STATUS ST7789_Inversion(ST77XX *instance, uint8_t on)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     ST77XX_Command(instance, (on ? ST7789_CMD_DISPLAY_INVERSION_ON : ST7789_CMD_DISPLAY_INVERSION_OFF));

//     ST77XX_EVENTS_RESET_BUSY(instance);
//     return DEVICE_STATUS_OK;
// }

// DEVICE_STATUS ST7789_Sleep(ST77XX *instance, uint8_t on)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     ST77XX_Command(instance, (on ? ST7789_CMD_SLEEP_IN : ST7789_CMD_SLEEP_OUT));

//     ST77XX_EVENTS_RESET_BUSY(instance);
//     return DEVICE_STATUS_OK;
// }

// DEVICE_STATUS ST7789_Display(ST77XX *instance, uint8_t on)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     ST77XX_Command(instance, (on ? ST7789_CMD_DISPLAY_ON : ST7789_CMD_DISPLAY_OFF));

//     ST77XX_EVENTS_RESET_BUSY(instance);
//     return DEVICE_STATUS_OK;
// }
// DEVICE_STATUS ST7789_Row_Address_Set(ST77XX *instance, uint16_t start, uint16_t end)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     ST77XX_Command_Write16xN(instance, ST7789_CMD_ROW_ADDRESS_SET, start, end, 2);

//     ST77XX_EVENTS_RESET_BUSY(instance);
//     return DEVICE_STATUS_OK;
// }

// DEVICE_STATUS ST7789_Column_Address_Set(ST77XX *instance, uint16_t start, uint16_t end)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     ST77XX_Command_Write16xN(instance, ST7789_CMD_COLUMN_ADDRESS_SET, start, end, 2);

//     ST77XX_EVENTS_RESET_BUSY(instance);
//     return DEVICE_STATUS_OK;
// }

// DEVICE_STATUS ST7789_Data_Write(ST77XX *instance, uint16_t *data, uint32_t size)
// {
//     if (ST77XX_IsBusy(instance))
//     {
//         return DEVICE_STATUS_BUSY;
//     }
//     ST77XX_EVENTS_SET_BUSY(instance);

//     void *pData = instance->command[1].buffer.data;
//     instance->cmdId = ST7789_CMD_MEMORY_WRITE;
//     instance->command[1].buffer.data = data;
//     instance->command[1].buffer.size = size;
//     CommandMaster_SendCommandAsync(&instance->commandMaster, instance->command, 2);
//     CommandMaster_WaitForComplete(&instance->commandMaster, TX_WAIT_FOREVER);
//     instance->command[1].buffer.data = pData;

//     ST77XX_EVENTS_RESET_BUSY(instance);
//     return DEVICE_STATUS_OK;
// }
