#include "st77xx/st7735.h"
#include "basic/shared.h"
#include "stdio.h"


#define ST7735_EVENT_BUSY 0x01

static inline int ST7735_IsBusy(ST77XX *instance)
{
    ULONG actualFlags;
    return tx_event_flags_get(&instance->events, ST7735_EVENT_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

DEVICE_STATUS ST7735_Init(ST77XX *instance)
{
    tx_event_flags_create(&instance->events, "st7735");

    instance->pvGamma[0] = 0x02U;
    instance->pvGamma[1] = 0x1CU;
    instance->pvGamma[2] = 0x07U;
    instance->pvGamma[3] = 0x12U;
    instance->pvGamma[4] = 0x37U;
    instance->pvGamma[5] = 0x32U;
    instance->pvGamma[6] = 0x29U;
    instance->pvGamma[7] = 0x2DU;
    instance->pvGamma[8] = 0x29U;
    instance->pvGamma[9] = 0x25U;
    instance->pvGamma[10] = 0x2BU;
    instance->pvGamma[11] = 0x39U;
    instance->pvGamma[12] = 0x00U;
    instance->pvGamma[13] = 0x01U;
    instance->pvGamma[14] = 0x03U;
    instance->pvGamma[15] = 0x10U;

    instance->nvGamma[0] = 0x03U;
    instance->nvGamma[1] = 0x1DU;
    instance->nvGamma[2] = 0x07U;
    instance->nvGamma[3] = 0x06U;
    instance->nvGamma[4] = 0x2EU;
    instance->nvGamma[5] = 0x2CU;
    instance->nvGamma[6] = 0x29U;
    instance->nvGamma[7] = 0x2DU;
    instance->nvGamma[8] = 0x2EU;
    instance->nvGamma[9] = 0x2EU;
    instance->nvGamma[10] = 0x37U;
    instance->nvGamma[11] = 0x3FU;
    instance->nvGamma[12] = 0x00U;
    instance->nvGamma[13] = 0x00U;
    instance->nvGamma[14] = 0x02U;
    instance->nvGamma[15] = 0x10U;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS ST7735_Reset(ST77XX *instance)
{
    LOG("ST7735:SWRST s")
    ST77XX_Command(instance, ST7735_CMD_SOFTWARE_RESET);
    tx_thread_sleep(120);
    LOG("ST7735:SWRST e")

    LOG("ST7735:SWRST s")
    ST77XX_Command(instance, ST7735_CMD_SOFTWARE_RESET);
    tx_thread_sleep(120);
    LOG("ST7735:SWRST e")

    LOG("ST7735:SLPOUT s")
    ST77XX_Command(instance, ST7735_CMD_SLEEP_OUT);
    LOG("ST7735:SLPOUT e")

    LOG("ST7735:RBGIC s")
    static const uint8_t frctl1[3] = {0x01U, 0x2CU, 0x2DU};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_FRAME_RATE_CTRL1, (const uint8_t *)frctl1, 3);
    LOG("ST7735:RBGIC e")

    LOG("ST7735:PROCH s")
    static const uint8_t frctl2[3] = {0x01U, 0x2CU, 0x2DU};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_FRAME_RATE_CTRL2, (const uint8_t *)frctl2, 3);
    LOG("ST7735:PROCH e")

    LOG("ST7735:FRMRCTL s")
    static const uint8_t frctl3[6] = {0x01U, 0x2CU, 0x2DU, 0x01U, 0x2CU, 0x2DU};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_FRAME_RATE_CTRL3, (const uint8_t *)frctl3, 6);
    LOG("ST7735:FRMRCTL e")

    LOG("ST7735:FRMICTL s")
    static const uint8_t fictl[1] = {0x07U};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_FRAME_INVERSION_CTRL, (const uint8_t *)fictl, 1);
    LOG("ST7735:FRMICTL e")

    LOG("ST7735:PWRCTL1 s")
    static const uint8_t pctl1[3] = {0xA2U, 0x02U, 0x84U};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_PWR_CTRL1, (const uint8_t *)pctl1, 3);
    LOG("ST7735:PWRCTL1 e")

    LOG("ST7735:PWRCTL2 s")
    static const uint8_t pctl2[1] = {0xC5U};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_PWR_CTRL2, (const uint8_t *)pctl2, 1);
    LOG("ST7735:PWRCTL2 e")

    LOG("ST7735:PWRCTL3 s")
    static const uint8_t pctl3[2] = {0x0AU, 0x00U};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_PWR_CTRL3, (const uint8_t *)pctl3, 2);
    LOG("ST7735:PWRCTL3 e")

    LOG("ST7735:PWRCTL4 s")
    static const uint8_t pctl4[2] = {0x8AU, 0x2AU};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_PWR_CTRL4, (const uint8_t *)pctl4, 2);
    LOG("ST7735:PWRCTL4 e")

    LOG("ST7735:PWRCTL5 s")
    static const uint8_t pctl5[2] = {0x8AU, 0xEEU};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_PWR_CTRL5, (const uint8_t *)pctl5, 2);
    LOG("ST7735:PWRCTL5 e")

    LOG("ST7735:PWRCTL s")
    static const uint8_t vcomctl1[1] = {0x0EU};
    ST77XX_Command_WriteData8(instance, ST7735_CMD_VCOMH_VCOML_CTRL1, (const uint8_t *)vcomctl1, 1);
    LOG("ST7735:PWRCTL e")

    LOG("ST7735:DISINV s")
    ST77XX_Command(instance, ST7735_CMD_DISPLAY_INVERSION_ON);
    LOG("ST7735:DISINV e")

    LOG("ST7735:PIXFMT s")
    ST77XX_Command_WriteData8(instance, ST7735_CMD_INTERFACE_PIXEL_FORMAT, &instance->colorMode, 1);
    LOG("ST7735:PIXFMT e")

    LOG("ST7735:PVGAMMA s")
    ST77XX_Command_WriteData8(instance, ST7735_CMD_POSITIVE_VOLTAGE_GAMMA_CONTROL, instance->pvGamma, 16);
    LOG("ST7735:PVGAMMA e")

    LOG("ST7735:NVGAMMA s")
    ST77XX_Command_WriteData8(instance, ST7735_CMD_NAGATIVE_VALTAGE_GAMMA_CONTROL, instance->nvGamma, 16);
    LOG("ST7735:NVGAMMA e")

    LOG("ST7735:DISNOR s")
    ST77XX_Command(instance, ST7735_CMD_NORMAL_DISPLAY_MODE_ON);
    LOG("ST7735:DISNOR e")

    LOG("ST7735:DISP s")
    ST77XX_Command(instance, ST7735_CMD_DISPLAY_ON);
    LOG("ST7735:DISP e")

    LOG("ST7735:MEMDAC s")
    ST77XX_Command_WriteData8(instance, ST7735_CMD_MEMORY_DATA_ACCESS_CONTROL, &instance->orientation, 1);
    LOG("ST7735:MEMDAC e")

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS ST7735_Inversion(ST77XX *instance, uint8_t on)
{

    ST77XX_Command(instance, (on ? ST7735_CMD_DISPLAY_INVERSION_ON : ST7735_CMD_DISPLAY_INVERSION_OFF));

    return DEVICE_STATUS_OK;
}

DEVICE_STATUS ST7735_Sleep(ST77XX *instance, uint8_t on)
{

    ST77XX_Command(instance, (on ? ST7735_CMD_SLEEP_IN : ST7735_CMD_SLEEP_OUT));

    return DEVICE_STATUS_OK;
}

DEVICE_STATUS ST7735_Display(ST77XX *instance, uint8_t on)
{

    ST77XX_Command(instance, (on ? ST7735_CMD_DISPLAY_ON : ST7735_CMD_DISPLAY_OFF));

    return DEVICE_STATUS_OK;
}

static DEVICE_STATUS ST7735_SetDisplayWindow(ST77XX *instance, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t *buffer = (uint16_t *)instance->buffer;
    x += instance->xOffset;
    y += instance->yOffset;

    buffer[0] = x;
    buffer[1] = x + width - 1;

    ST77XX_Command_WriteData16(instance, ST7735_CMD_COLUMN_ADDRESS_SET, buffer, 2);
    buffer[0] = y;
    buffer[1] = y + height - 1;
    ST77XX_Command_WriteData16(instance, ST7735_CMD_ROW_ADDRESS_SET, buffer, 2);
    return DEVICE_STATUS_OK;
}

static DEVICE_STATUS ST7735_SetCursor(ST77XX *instance, uint16_t x, uint16_t y)
{
    uint16_t *buffer = (uint16_t *)instance->buffer;

    buffer[0] = x + instance->xOffset;
    ST77XX_Command_WriteData16(instance, ST7735_CMD_COLUMN_ADDRESS_SET, buffer, 1);
    buffer[0] = y + instance->yOffset;
    ST77XX_Command_WriteData16(instance, ST7735_CMD_ROW_ADDRESS_SET, buffer, 1);

    return DEVICE_STATUS_OK;
}

DEVICE_STATUS ST7735_SetPixel(ST77XX *instance, uint16_t x, uint16_t y, uint32_t color)
{
    DEVICE_STATUS ret = DEVICE_STATUS_OK;

    if ((x >= instance->width) || (y >= instance->height))
    {
        return DEVICE_STATUS_POSITION_OUT_OF_RANGE;
    }
    ret = ST7735_SetCursor(instance, x, y);

    if (ret != DEVICE_STATUS_OK)
    {
        return ret;
    }
    uint16_t *buf = (uint16_t *)(instance->buffer);
    buf[0] = (uint16_t)color;

    ST77XX_Command_WriteData16(instance, ST7735_CMD_MEMORY_WRITE, buf, 2);

    return DEVICE_STATUS_OK;
}

DEVICE_STATUS ST7735_DrawHLine(ST77XX *instance, uint32_t x, uint32_t y, uint32_t length, uint32_t color)
{

    DEVICE_STATUS ret = DEVICE_STATUS_OK;

    if ((x + length) > instance->width)
    {
        ret = DEVICE_STATUS_GENERAL_ERROR;
    }

    ret = ST7735_SetCursor(instance, x, y);

    if (ret != DEVICE_STATUS_OK)
    {
        return ret;
    }
    uint16_t *buf = (uint16_t *)(instance->buffer);
    for (uint32_t i = 0; i < length; i++)
    {
        buf[i] = (uint16_t)(color);
    }

    ST77XX_Command_WriteData16(instance, ST7735_CMD_MEMORY_WRITE, buf, length);

    return ret;
}

DEVICE_STATUS ST7735_DrawVLine(ST77XX *instance, uint16_t x, uint16_t y, uint16_t length, uint32_t color)
{
    DEVICE_STATUS ret = DEVICE_STATUS_OK;
    uint16_t *buf = (uint16_t *)(instance->buffer);

    if ((y + length) > instance->height)
    {
        ret = DEVICE_STATUS_GENERAL_ERROR;
    }

    ret = ST7735_SetDisplayWindow(instance, x, y, 1, length);

    if (ret != DEVICE_STATUS_OK)
    {
        return ret;
    }
    for (uint32_t i = 0; i < length; i++)
    {
        buf[i] = (uint16_t)(color);
    }

    ST77XX_Command_WriteData16(instance, ST7735_CMD_MEMORY_WRITE, buf, length);

    return ret;
}

DEVICE_STATUS ST7735_DrawRect(ST77XX *instance, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    DEVICE_STATUS ret = DEVICE_STATUS_OK;

    if ((x + width) > instance->width)
    {
        ret = DEVICE_STATUS_GENERAL_ERROR;
    }
    if ((y + height) > instance->height)
    {
        ret = DEVICE_STATUS_GENERAL_ERROR;
    }

    ret = ST7735_SetDisplayWindow(instance, x, y, width, height);

    if (ret != DEVICE_STATUS_OK)
    {
        return ret;
    }
    // uint8_t *buf = (uint8_t *)(instance->buffer);
    uint16_t *buf = (uint16_t *)(instance->buffer);
    uint32_t size = width * height;
    for (uint32_t i = 0; i < size; i++)
    {
        buf[i] = color;
        // buf[i << 1] = (uint8_t)(color >> 8);
        // buf[(i << 1) + 1] = (uint8_t)(color);
    }

    ST77XX_Command_WriteData16(instance, ST7735_CMD_MEMORY_WRITE, buf, size);
    // ST77XX_Command_WriteData8(instance, ST7735_CMD_MEMORY_WRITE, buf, size * 2);
    return ret;
}

DEVICE_STATUS ST7735_ReadID(ST77XX *instance, uint32_t *id)
{
    uint32_t id_temp = 0;
    ST77XX_Command_ReadData8(instance, ST7735_CMD_READ_ID1, 1);
    id_temp += instance->cmdData[0];
    id_temp <<= 8;
    ST77XX_Command_ReadData8(instance, ST7735_CMD_READ_ID2, 1);
    id_temp += instance->cmdData[0];
    id_temp <<= 8;
    ST77XX_Command_ReadData8(instance, ST7735_CMD_READ_ID3, 1);
    id_temp += instance->cmdData[0];

    *id = id_temp;

    return DEVICE_STATUS_OK;
}

DEVICE_STATUS ST7735_DrawBitmap(ST77XX *instance, uint32_t x, uint32_t y, uint8_t *pBmp)
{
    uint32_t index, size, width, height, y_pos;
    uint8_t *pbmp;

    /* Get bitmap data address offset */
    index = (uint32_t)pBmp[10] + ((uint32_t)pBmp[11] << 8) + ((uint32_t)pBmp[12] << 16) + ((uint32_t)pBmp[13] << 24);

    /* Read bitmap width */
    width = (uint32_t)pBmp[18] + ((uint32_t)pBmp[19] << 8) + ((uint32_t)pBmp[20] << 16) + ((uint32_t)pBmp[21] << 24);

    /* Read bitmap height */
    height = (uint32_t)pBmp[22] + ((uint32_t)pBmp[23] << 8) + ((uint32_t)pBmp[24] << 16) + ((uint32_t)pBmp[25] << 24);

    /* Read bitmap size */
    size = (uint32_t)pBmp[2] + ((uint32_t)pBmp[3] << 8) + ((uint32_t)pBmp[4] << 16) + ((uint32_t)pBmp[5] << 24);
    size = size - index;

    pbmp = pBmp + index;

    /* Remap Ypos, st7735 works with inverted X in case of bitmap */
    /* X = 0, cursor is on Top corner */
    y_pos = instance->height - y - height;

    ST7735_SetDisplayWindow(instance, x, y_pos, width, height);

    // uint16_t *buf = (uint16_t *)(instance->buffer);

    ST77XX_Command_WriteData16(instance, ST7735_CMD_MEMORY_WRITE, (uint16_t *)pbmp, size / 2);

    return DEVICE_STATUS_OK;
}
