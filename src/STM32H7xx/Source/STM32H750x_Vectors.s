/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : STM32H750x_Vectors.s
Purpose : Exception and interrupt vectors for STM32H750x devices.
Additional information:
  Preprocessor Definitions
    __NO_EXTERNAL_INTERRUPTS
      If defined,
        the vector table will contain only the internal exceptions
        and interrupts.
    __VECTORS_IN_RAM
      If defined,
        an area of RAM, large enough to store the vector table,
        will be reserved.

    __OPTIMIZATION_SMALL
      If defined,
        all weak definitions of interrupt handlers will share the
        same implementation.
      If not defined,
        all weak definitions of interrupt handlers will be defined
        with their own implementation.
*/

        .syntax unified

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
.global  g_pfnVectors

//
// Directly place a vector (word) in the vector table
//
.macro VECTOR Name=
        .section .vectors, "ax"
        .code 16
        .word \Name
.endm

//
// Declare an exception handler with a weak definition
//
.macro EXC_HANDLER Name=
        //
        // Insert vector in vector table
        //
        .section .vectors, "ax"
        .word \Name
        //
        // Insert dummy handler in init section
        //
        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b   // Endless loop
.endm

//
// Declare an interrupt handler with a weak definition
//
.macro ISR_HANDLER Name=
        //
        // Insert vector in vector table
        //
        .section .vectors, "ax"
        .word \Name
        //
        // Insert dummy handler in init section
        //
#if defined(__OPTIMIZATION_SMALL)
        .section .init, "ax"
        .weak \Name
        .thumb_set \Name,Dummy_Handler
#else
        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b   // Endless loop
#endif
.endm

//
// Place a reserved vector in vector table
//
.macro ISR_RESERVED
        .section .vectors, "ax"
        .word 0
.endm

//
// Place a reserved vector in vector table
//
.macro ISR_RESERVED_DUMMY
        .section .vectors, "ax"
        .word Dummy_Handler
.endm

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
        .extern __stack_end__
        .extern Reset_Handler
        .extern HardFault_Handler

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*  Setup of the vector table and weak definition of interrupt handlers
*
*/
        .section .vectors, "ax"
        .code 16
        .balign 512
        .global _vectors
g_pfnVectors:
_vectors:
        //
        // Internal exceptions and interrupts
        //
        VECTOR __stack_end__
        VECTOR Reset_Handler
        EXC_HANDLER NMI_Handler
        VECTOR HardFault_Handler
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        EXC_HANDLER SVC_Handler
        ISR_RESERVED
        ISR_RESERVED
        EXC_HANDLER PendSV_Handler
        EXC_HANDLER SysTick_Handler
        //
        // External interrupts
        //
#ifndef __NO_EXTERNAL_INTERRUPTS
        ISR_HANDLER WWDG_IRQHandler                   /* Window WatchDog              */
        ISR_HANDLER PVD_AVD_IRQHandler                /* PVD/AVD through EXTI Line detection */
        ISR_HANDLER TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */
        ISR_HANDLER RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */
        ISR_HANDLER FLASH_IRQHandler                  /* FLASH                        */
        ISR_HANDLER RCC_IRQHandler                    /* RCC                          */
        ISR_HANDLER EXTI0_IRQHandler                  /* EXTI Line0                   */
        ISR_HANDLER EXTI1_IRQHandler                  /* EXTI Line1                   */
        ISR_HANDLER EXTI2_IRQHandler                  /* EXTI Line2                   */
        ISR_HANDLER EXTI3_IRQHandler                  /* EXTI Line3                   */
        ISR_HANDLER EXTI4_IRQHandler                  /* EXTI Line4                   */
        ISR_HANDLER DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */
        ISR_HANDLER DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */
        ISR_HANDLER DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */
        ISR_HANDLER DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */
        ISR_HANDLER DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */
        ISR_HANDLER DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */
        ISR_HANDLER DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */
        ISR_HANDLER ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */
        ISR_HANDLER FDCAN1_IT0_IRQHandler             /* FDCAN1 interrupt line 0      */
        ISR_HANDLER FDCAN2_IT0_IRQHandler             /* FDCAN2 interrupt line 0      */
        ISR_HANDLER FDCAN1_IT1_IRQHandler             /* FDCAN1 interrupt line 1      */
        ISR_HANDLER FDCAN2_IT1_IRQHandler             /* FDCAN2 interrupt line 1      */
        ISR_HANDLER EXTI9_5_IRQHandler                /* External Line[9:5]s          */
        ISR_HANDLER TIM1_BRK_IRQHandler               /* TIM1 Break interrupt         */
        ISR_HANDLER TIM1_UP_IRQHandler                /* TIM1 Update interrupt        */
        ISR_HANDLER TIM1_TRG_COM_IRQHandler           /* TIM1 Trigger and Commutation interrupt */
        ISR_HANDLER TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
        ISR_HANDLER TIM2_IRQHandler                   /* TIM2                         */
        ISR_HANDLER TIM3_IRQHandler                   /* TIM3                         */
        ISR_HANDLER TIM4_IRQHandler                   /* TIM4                         */
        ISR_HANDLER I2C1_EV_IRQHandler                /* I2C1 Event                   */
        ISR_HANDLER I2C1_ER_IRQHandler                /* I2C1 Error                   */
        ISR_HANDLER I2C2_EV_IRQHandler                /* I2C2 Event                   */
        ISR_HANDLER I2C2_ER_IRQHandler                /* I2C2 Error                   */
        ISR_HANDLER SPI1_IRQHandler                   /* SPI1                         */
        ISR_HANDLER SPI2_IRQHandler                   /* SPI2                         */
        ISR_HANDLER USART1_IRQHandler                 /* USART1                       */
        ISR_HANDLER USART2_IRQHandler                 /* USART2                       */
        ISR_HANDLER USART3_IRQHandler                 /* USART3                       */
        ISR_HANDLER EXTI15_10_IRQHandler              /* External Line[15:10]s        */
        ISR_HANDLER RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
        ISR_RESERVED_DUMMY                                 /* Reserved                     */
        ISR_HANDLER TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */
        ISR_HANDLER TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */
        ISR_HANDLER TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
        ISR_HANDLER TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */
        ISR_HANDLER DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */
        ISR_HANDLER FMC_IRQHandler                    /* FMC                          */
        ISR_HANDLER SDMMC1_IRQHandler                 /* SDMMC1                       */
        ISR_HANDLER TIM5_IRQHandler                   /* TIM5                         */
        ISR_HANDLER SPI3_IRQHandler                   /* SPI3                         */
        ISR_HANDLER UART4_IRQHandler                  /* UART4                        */
        ISR_HANDLER UART5_IRQHandler                  /* UART5                        */
        ISR_HANDLER TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */
        ISR_HANDLER TIM7_IRQHandler                   /* TIM7                         */
        ISR_HANDLER DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */
        ISR_HANDLER DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */
        ISR_HANDLER DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */
        ISR_HANDLER DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */
        ISR_HANDLER DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */
        ISR_HANDLER ETH_IRQHandler                    /* Ethernet                     */
        ISR_HANDLER ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line */
        ISR_HANDLER FDCAN_CAL_IRQHandler              /* FDCAN calibration unit interrupt*/
        ISR_RESERVED_DUMMY                                 /* Reserved                     */
        ISR_RESERVED_DUMMY                                 /* Reserved                     */
        ISR_RESERVED_DUMMY                                 /* Reserved                     */
        ISR_RESERVED_DUMMY                                 /* Reserved                     */
        ISR_HANDLER DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */
        ISR_HANDLER DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */
        ISR_HANDLER DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */
        ISR_HANDLER USART6_IRQHandler                 /* USART6                       */
        ISR_HANDLER I2C3_EV_IRQHandler                /* I2C3 event                   */
        ISR_HANDLER I2C3_ER_IRQHandler                /* I2C3 error                   */
        ISR_HANDLER OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */
        ISR_HANDLER OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */
        ISR_HANDLER OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */
        ISR_HANDLER OTG_HS_IRQHandler                 /* USB OTG HS                   */
        ISR_HANDLER DCMI_IRQHandler                   /* DCMI                         */
        ISR_HANDLER CRYP_IRQHandler                   /* Crypto                       */
        ISR_HANDLER HASH_RNG_IRQHandler               /* Hash and Rng                 */
        ISR_HANDLER FPU_IRQHandler                    /* FPU                          */
        ISR_HANDLER UART7_IRQHandler                  /* UART7                        */
        ISR_HANDLER UART8_IRQHandler                  /* UART8                        */
        ISR_HANDLER SPI4_IRQHandler                   /* SPI4                         */
        ISR_HANDLER SPI5_IRQHandler                   /* SPI5                         */
        ISR_HANDLER SPI6_IRQHandler                   /* SPI6                         */
        ISR_HANDLER SAI1_IRQHandler                   /* SAI1                         */
        ISR_HANDLER LTDC_IRQHandler                   /* LTDC                         */
        ISR_HANDLER LTDC_ER_IRQHandler                /* LTDC error                   */
        ISR_HANDLER DMA2D_IRQHandler                  /* DMA2D                        */
        ISR_HANDLER SAI2_IRQHandler                   /* SAI2                         */
        ISR_HANDLER QUADSPI_IRQHandler                /* QUADSPI                      */
        ISR_HANDLER LPTIM1_IRQHandler                 /* LPTIM1                       */
        ISR_HANDLER CEC_IRQHandler                    /* HDMI_CEC                     */
        ISR_HANDLER I2C4_EV_IRQHandler                /* I2C4 Event                   */
        ISR_HANDLER I2C4_ER_IRQHandler                /* I2C4 Error                   */
        ISR_HANDLER SPDIF_RX_IRQHandler               /* SPDIF_RX                     */
        ISR_HANDLER OTG_FS_EP1_OUT_IRQHandler         /* USB OTG FS End Point 1 Out   */
        ISR_HANDLER OTG_FS_EP1_IN_IRQHandler          /* USB OTG FS End Point 1 In    */
        ISR_HANDLER OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI */
        ISR_HANDLER OTG_FS_IRQHandler                 /* USB OTG FS                   */
        ISR_HANDLER DMAMUX1_OVR_IRQHandler            /* DMAMUX1 Overrun interrupt    */
        ISR_HANDLER HRTIM1_Master_IRQHandler          /* HRTIM Master Timer global Interrupt */
        ISR_HANDLER HRTIM1_TIMA_IRQHandler            /* HRTIM Timer A global Interrupt */
        ISR_HANDLER HRTIM1_TIMB_IRQHandler            /* HRTIM Timer B global Interrupt */
        ISR_HANDLER HRTIM1_TIMC_IRQHandler            /* HRTIM Timer C global Interrupt */
        ISR_HANDLER HRTIM1_TIMD_IRQHandler            /* HRTIM Timer D global Interrupt */
        ISR_HANDLER HRTIM1_TIME_IRQHandler            /* HRTIM Timer E global Interrupt */
        ISR_HANDLER HRTIM1_FLT_IRQHandler             /* HRTIM Fault global Interrupt   */
        ISR_HANDLER DFSDM1_FLT0_IRQHandler            /* DFSDM Filter0 Interrupt        */
        ISR_HANDLER DFSDM1_FLT1_IRQHandler            /* DFSDM Filter1 Interrupt        */
        ISR_HANDLER DFSDM1_FLT2_IRQHandler            /* DFSDM Filter2 Interrupt        */
        ISR_HANDLER DFSDM1_FLT3_IRQHandler            /* DFSDM Filter3 Interrupt        */
        ISR_HANDLER SAI3_IRQHandler                   /* SAI3 global Interrupt          */
        ISR_HANDLER SWPMI1_IRQHandler                 /* Serial Wire Interface 1 global interrupt */
        ISR_HANDLER TIM15_IRQHandler                  /* TIM15 global Interrupt      */
        ISR_HANDLER TIM16_IRQHandler                  /* TIM16 global Interrupt      */
        ISR_HANDLER TIM17_IRQHandler                  /* TIM17 global Interrupt      */
        ISR_HANDLER MDIOS_WKUP_IRQHandler             /* MDIOS Wakeup  Interrupt     */
        ISR_HANDLER MDIOS_IRQHandler                  /* MDIOS global Interrupt      */
        ISR_HANDLER JPEG_IRQHandler                   /* JPEG global Interrupt       */
        ISR_HANDLER MDMA_IRQHandler                   /* MDMA global Interrupt       */
        ISR_RESERVED_DUMMY                                 /* Reserved                    */
        ISR_HANDLER SDMMC2_IRQHandler                 /* SDMMC2 global Interrupt     */
        ISR_HANDLER HSEM1_IRQHandler                  /* HSEM1 global Interrupt      */
        ISR_RESERVED_DUMMY                                 /* Reserved                    */
        ISR_HANDLER ADC3_IRQHandler                   /* ADC3 global Interrupt       */
        ISR_HANDLER DMAMUX2_OVR_IRQHandler            /* DMAMUX Overrun interrupt    */
        ISR_HANDLER BDMA_Channel0_IRQHandler          /* BDMA Channel 0 global Interrupt */
        ISR_HANDLER BDMA_Channel1_IRQHandler          /* BDMA Channel 1 global Interrupt */
        ISR_HANDLER BDMA_Channel2_IRQHandler          /* BDMA Channel 2 global Interrupt */
        ISR_HANDLER BDMA_Channel3_IRQHandler          /* BDMA Channel 3 global Interrupt */
        ISR_HANDLER BDMA_Channel4_IRQHandler          /* BDMA Channel 4 global Interrupt */
        ISR_HANDLER BDMA_Channel5_IRQHandler          /* BDMA Channel 5 global Interrupt */
        ISR_HANDLER BDMA_Channel6_IRQHandler          /* BDMA Channel 6 global Interrupt */
        ISR_HANDLER BDMA_Channel7_IRQHandler          /* BDMA Channel 7 global Interrupt */
        ISR_HANDLER COMP1_IRQHandler                  /* COMP1 global Interrupt     */
        ISR_HANDLER LPTIM2_IRQHandler                 /* LP TIM2 global interrupt   */
        ISR_HANDLER LPTIM3_IRQHandler                 /* LP TIM3 global interrupt   */
        ISR_HANDLER LPTIM4_IRQHandler                 /* LP TIM4 global interrupt   */
        ISR_HANDLER LPTIM5_IRQHandler                 /* LP TIM5 global interrupt   */
        ISR_HANDLER LPUART1_IRQHandler                /* LP UART1 interrupt         */
        ISR_RESERVED_DUMMY                               /* Reserved                   */
        ISR_HANDLER CRS_IRQHandler                    /* Clock Recovery Global Interrupt */
        ISR_HANDLER ECC_IRQHandler                    /* ECC diagnostic Global Interrupt */
        ISR_HANDLER SAI4_IRQHandler                   /* SAI4 global interrupt      */
        ISR_RESERVED_DUMMY                                 /* Reserved                   */
        ISR_RESERVED_DUMMY                                 /* Reserved                   */
        ISR_HANDLER WAKEUP_PIN_IRQHandler             /* Interrupt for all 6 wake-up pins */
#endif
        //
        .section .vectors, "ax"
_vectors_end:

#ifdef __VECTORS_IN_RAM
        //
        // Reserve space with the size of the vector table
        // in the designated RAM section.
        //
        .section .vectors_ram, "ax"
        .balign 512
        .global _vectors_ram

_vectors_ram:
        .space _vectors_end - _vectors, 0
#endif

/*********************************************************************
*
*  Dummy handler to be used for reserved interrupt vectors
*  and weak implementation of interrupts.
*
*/
        .section .init.Dummy_Handler, "ax"
        .thumb_func
        .weak Dummy_Handler
        .balign 2
Dummy_Handler:
        1: b 1b   // Endless loop


/*************************** End of file ****************************/
