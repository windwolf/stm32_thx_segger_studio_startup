/* This is a small demo of the high-performance FileX FAT file system. It includes setup for
   a small 34KB RAM disk and a loop that writes and reads a small file.  */
#include "filex_demo.h"

#ifdef FX_ENABLE_FAULT_TOLERANT
#include "fx_fault_tolerant.h"
#endif /* FX_ENABLE_FAULT_TOLERANT */

#define DEMO_STACK_SIZE 2048

/* Buffer for FileX FX_MEDIA sector cache. This must be large enough for at least one
   sector, which are typically 512 bytes in size.  */

unsigned char media_memory[512];

#ifdef FX_ENABLE_FAULT_TOLERANT
UCHAR fault_tolerant_memory[FX_FAULT_TOLERANT_MAXIMUM_LOG_FILE_SIZE];
#endif /* FX_ENABLE_FAULT_TOLERANT */

/* Define RAM device driver entry.  */

VOID _fx_ram_driver(FX_MEDIA *media_ptr);

/* Define thread prototypes.  */

/* Define FileX global data structures.  */

FX_MEDIA sd_disk;
FX_FILE my_file;

/* Define ThreadX global data structures.  */

#ifndef FX_STANDALONE_ENABLE
TX_THREAD thread_2;
#endif
ULONG thread_2_counter;

/* Define what the initial system looks like.  */

#ifndef FX_STANDALONE_ENABLE
CHAR *fx_application_define(void *first_unused_memory)
{

    CHAR *pointer;

    /* Put first available memory address into a character pointer.  */
    pointer = (CHAR *)first_unused_memory;

    /* Put system definition stuff in here, e.g. thread creates and other assorted
       create information.  */

    /* Create the main thread.  */
    tx_thread_create(&thread_2, "thread 2", thread_2_entry, 0,
                     pointer, DEMO_STACK_SIZE,
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    /* Initialize FileX.  */
    fx_system_initialize();

    return pointer;
}

#endif

extern SdDevice sdDevice;

void thread_2_entry(ULONG thread_input)
{

    UINT status;
    ULONG actual;
    CHAR local_buffer[30];
    fx_sd_driver_device_set(&sdDevice);
    /* Format the RAM disk - the memory for the RAM disk was setup in
       tx_application_define above.  */
#ifdef FX_ENABLE_EXFAT
    fx_media_exFAT_format(&ram_disk,
                          _fx_ram_driver,       // Driver entry
                          ram_disk_memory,      // RAM disk memory pointer
                          media_memory,         // Media buffer pointer
                          sizeof(media_memory), // Media buffer size
                          "MY_RAM_DISK",        // Volume Name
                          1,                    // Number of FATs
                          0,                    // Hidden sectors
                          256,                  // Total sectors
                          512,                  // Sector size
                          8,                    // exFAT Sectors per cluster
                          12345,                // Volume ID
                          1);                   // Boundary unit
#else
    fx_media_format(&sd_disk,
                    fx_sd_driver,         // Driver entry
                    &sdDevice,            // RAM disk memory pointer
                    media_memory,         // Media buffer pointer
                    sizeof(media_memory), // Media buffer size
                    "SD1",                // Volume Name
                    1,                    // Number of FATs
                    256,                  // Directory Entries
                    0,                    // Hidden sectors
                    256,                  // Total sectors
                    512,                  // Sector size
                    8,                    // Sectors per cluster
                    1,                    // Heads
                    1);                   // Sectors per track
#endif /* FX_ENABLE_EXFAT */

    /* Loop to repeat the demo over and over!  */
    do
    {

        /* Open the RAM disk.  */
        status = fx_media_open(&sd_disk, "RAM DISK", fx_sd_driver, &sdDevice, media_memory, sizeof(media_memory));

        /* Check the media open status.  */
        if (status != FX_SUCCESS)
        {

            /* Error, break the loop!  */
            break;
        }

#ifdef FX_ENABLE_FAULT_TOLERANT
        status = fx_fault_tolerant_enable(&ram_disk, fault_tolerant_memory, sizeof(fault_tolerant_memory));

        if (status != FX_SUCCESS)
        {

            /* Fault tolerant enable error, break the loop.  */
            break;
        }
#endif /* FX_ENABLE_FAULT_TOLERANT */

        /* Create a file called TEST.TXT in the root directory.  */
        status = fx_file_create(&sd_disk, "TEST.TXT");

        /* Check the create status.  */
        if (status != FX_SUCCESS)
        {

            /* Check for an already created status. This is expected on the
               second pass of this loop!  */
            if (status != FX_ALREADY_CREATED)
            {

                /* Create error, break the loop.  */
                break;
            }
        }

        /* Open the test file.  */
        status = fx_file_open(&sd_disk, &my_file, "TEST.TXT", FX_OPEN_FOR_WRITE);

        /* Check the file open status.  */
        if (status != FX_SUCCESS)
        {

            /* Error opening file, break the loop.  */
            break;
        }

        /* Seek to the beginning of the test file.  */
        status = fx_file_seek(&my_file, 0);

        /* Check the file seek status.  */
        if (status != FX_SUCCESS)
        {

            /* Error performing file seek, break the loop.  */
            break;
        }

        /* Write a string to the test file.  */
        status = fx_file_write(&my_file, " ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", 28);

        /* Check the file write status.  */
        if (status != FX_SUCCESS)
        {

            /* Error writing to a file, break the loop.  */
            break;
        }

        /* Seek to the beginning of the test file.  */
        status = fx_file_seek(&my_file, 0);

        /* Check the file seek status.  */
        if (status != FX_SUCCESS)
        {

            /* Error performing file seek, break the loop.  */
            break;
        }

        /* Read the first 28 bytes of the test file.  */
        status = fx_file_read(&my_file, local_buffer, 28, &actual);

        /* Check the file read status.  */
        if ((status != FX_SUCCESS) || (actual != 28))
        {

            /* Error reading file, break the loop.  */
            break;
        }

        /* Close the test file.  */
        status = fx_file_close(&my_file);

        /* Check the file close status.  */
        if (status != FX_SUCCESS)
        {

            /* Error closing the file, break the loop.  */
            break;
        }

        /* Close the media.  */
        status = fx_media_close(&sd_disk);

        /* Check the media close status.  */
        if (status != FX_SUCCESS)
        {

            /* Error closing the media, break the loop.  */
            break;
        }

        /* Increment the thread counter, which represents the number
           of successful passes through this loop.  */
        thread_2_counter++;

    } while (1);

    /* If we get here the FileX test failed!  */
    return;
}
