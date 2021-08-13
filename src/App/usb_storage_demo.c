#include "usb_storage_demo.h"

#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_class_storage.h"
#include "ux_device_class_cdc_acm.h"

#include "ux_dcd_stm32.h"

#include "ux_device_descriptors.h"
#include "ux_device_msc.h"
#include "../Drivers/common/inc/common/device.h"
#include "../Drivers/common/inc/common/block.h"
#include "../Drivers/common/inc/common/buffer.h"
#include "bsp.h"
#include "usb_otg.h"

/* Define USBX demo constants.  */
#define UX_DEMO_THREAD_STACK_SIZE (2048)
#define UX_DEMO_MEMORY_SIZE (28 * 1024)

/* Define the counters used in the demo application...  */

static ULONG thread_0_counter;
static ULONG error_counter;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
static UX_SLAVE_CLASS_STORAGE *storage_device;
/* Define USBX demo global variables.  */
extern SD_HandleTypeDef hsd1;
static SdDevice sdDevice;
static Block sdBlock;
static TX_THREAD ux_app_thread;

BUFFER_DECLARE_STATIC(sdBlockBuffer, 1024);
static uint8_t ux_demo_thread_stack_buffer[UX_DEMO_THREAD_STACK_SIZE];
static ULONG ux_demo_memory_buffer[(UX_DEMO_MEMORY_SIZE) / sizeof(ULONG)];

static void sd_init();
static void usbd_init();
static VOID tx_demo_instance_activate(VOID *dpump_instance);
static VOID tx_demo_instance_deactivate(VOID *dpump_instance);
static void usbx_app_thread_entry(ULONG arg);
static VOID error_handler(void);
static UINT sd_media_read(VOID *storage,
                          ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba,
                          ULONG *media_status);
static UINT sd_media_write(VOID *storage,
                           ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba,
                           ULONG *media_status);
static UINT sd_media_status(VOID *storage,
                             ULONG lun, ULONG media_id,
                             ULONG *media_status);

static UINT sd_media_read(VOID *storage,
                          ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba,
                          ULONG *media_status)
{
    OP_RESULT status;
    status = block_read(&sdBlock, data_pointer, lba, number_blocks);
    *media_status = status;
    if (status != OP_RESULT_OK)
    {
        return UX_ERROR;
    }
    return UX_SUCCESS;
};

static UINT sd_media_write(VOID *storage,
                           ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba,
                           ULONG *media_status)
{
    OP_RESULT status;
    status = block_write(&sdBlock, data_pointer, lba, number_blocks);
    *media_status = status;
    if (status != OP_RESULT_OK)
    {
        return UX_ERROR;
    }
    return UX_SUCCESS;
};

static UINT sd_media_status(VOID *storage,
                            ULONG lun, ULONG media_id,
                            ULONG *media_status)
{
    OP_RESULT status;
    status = sd_device_query_status(&sdDevice);
    *media_status = status;
    return UX_SUCCESS;
};

static void sd_init()
{
    sd_device_create(&sdDevice, &hsd1, 4);
    sd_device_block_create(&sdDevice, &sdBlock, sdBlockBuffer);
    sd_device_card_init(&sdDevice);
}

static void usbd_init()
{
    /* Initialization of USB device */
    /* USER CODE BEGIN USB_Device_Init_PreTreatment_0 */
    /* USER CODE END USB_Device_Init_PreTreatment_0 */

    HAL_PWREx_EnableUSBVoltageDetector();

    /* initialize the device controller HAL driver */
    MX_USB_OTG_FS_PCD_Init();

    /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */
    HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x200);
    HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x100);
    /* USER CODE END USB_Device_Init_PreTreatment_1 */

    /* initialize and link controller HAL driver to USBx */
    _ux_dcd_stm32_initialize((ULONG)USB_OTG_HS, (ULONG)&hpcd_USB_OTG_FS);

    /* Start USB device by connecting the DP pullup */
    HAL_PCD_Start(&hpcd_USB_OTG_FS);

    /* USER CODE BEGIN USB_Device_Init_PostTreatment */
    /* USER CODE END USB_Device_Init_PostTreatment */
}

void usb_storage_demo()
{
    sd_init();
    CHAR *memory_pointer;
    UINT status;
    UX_SLAVE_CLASS_STORAGE_PARAMETER parameter;

    memory_pointer = (CHAR *)ux_demo_memory_buffer;

    status = ux_system_initialize(memory_pointer, UX_DEMO_MEMORY_SIZE, UX_NULL, 0);

    if (status != UX_SUCCESS)
    {
        error_handler();
    }

    /* The code below is required for installing the device portion of USBX */
    ULONG *hs_len;
    UCHAR *hs = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED, &hs_len);
    ULONG *fs_len;
    UCHAR *fs = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED, &fs_len);
    ULONG *str_len;
    UCHAR *str = USBD_Get_String_Framework(&str_len);
    ULONG *id_len;
    UCHAR *id = USBD_Get_Language_Id_Framework(&id_len);
    status = _ux_device_stack_initialize(hs, hs_len,
                                         fs, fs_len,
                                         str, str_len,
                                         id, id_len, UX_NULL);

    /* Check for error.  */
    if (status != UX_SUCCESS)
    {
        error_handler();
    }

    /* Set the parameters for callback when insertion/extraction of a Data Pump device.  */
    parameter.ux_slave_class_storage_instance_activate = tx_demo_instance_activate;
    parameter.ux_slave_class_storage_instance_deactivate = tx_demo_instance_deactivate;

    parameter.ux_slave_class_storage_parameter_product_id = USBD_PRODUCT_STRING;
    parameter.ux_slave_class_storage_parameter_product_rev = "0.1";
    parameter.ux_slave_class_storage_parameter_product_serial = USBD_SERIAL_NUMBER;
    parameter.ux_slave_class_storage_parameter_vendor_id = USBD_MANUFACTURER_STRING;

    parameter.ux_slave_class_storage_parameter_number_lun = 1;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_last_lba = sdDevice.blockNbr - 1;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_block_length = sdDevice.blockSize;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_type = 0;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_removable_flag = 0x80;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_read_only_flag = UX_FALSE;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_read = sd_media_read;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_write = sd_media_write;
    parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_status = sd_media_status;

    /* Initialize the device dpump class. The class is connected with interface 0 */
    status = _ux_device_stack_class_register(_ux_system_slave_class_storage_name,
                                             _ux_device_class_storage_entry,
                                             1, 0, &parameter);

    /* Check for error.  */
    if (status != UX_SUCCESS)
        error_handler();

    /* Initialize the simulated device controller.  */
    status = ux_dcd_stm32_initialize(NULL, &hpcd_USB_OTG_FS);

    /* Check for error.  */
    if (status != UX_SUCCESS)
        error_handler();

    status = tx_thread_create(&ux_app_thread, "main_usbx_app_thread_entry",
                              usbx_app_thread_entry, 0,
                              ux_demo_thread_stack_buffer, UX_DEMO_THREAD_STACK_SIZE,
                              5, 5,
                              TX_NO_TIME_SLICE, TX_AUTO_START);
}

static void usbx_app_thread_entry(ULONG arg)
{
    /* Sleep for 100 ms */
    tx_thread_sleep(0.1 * TX_TIMER_TICKS_PER_SECOND);

    /* Initialization of USB device */
    usbd_init();
}

static VOID tx_demo_instance_activate(VOID *storage_instance)
{

    /* Save the DPUMP instance.  */
    storage_device = (UX_SLAVE_CLASS_STORAGE *)storage_instance;
}

static VOID tx_demo_instance_deactivate(VOID *storage_instance)
{

    UX_PARAMETER_NOT_USED(storage_instance);

    /* Reset the DPUMP instance.  */
    storage_device = UX_NULL;
}

static VOID error_handler(void)
{

    /* Increment error counter.  */
    error_counter++;

    while (1)
    {

        /* Error - just spin here!  Look at call tree in debugger 
           to see where the error occurred.  */
    }
}
