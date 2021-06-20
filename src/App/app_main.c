

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stm32h7xx_hal.h"
#include "app_main.h"
#include "tx_api.h"
#include "safe_printf.h"
/*
*********************************************************************************************************
*                                 任务优先级，数值越小优先级越高
*********************************************************************************************************
*/
#define SYS_CFG_TASK_START_PRIO 2u
#define SYS_CFG_TASK_STAT_PRIO 30u
#define SYS_CFG_TASK_IDLE_PRIO 31u

/*
*********************************************************************************************************
*                                    任务栈大小，单位字节
*********************************************************************************************************
*/
#define SYS_CFG_TASK_START_STK_SIZE 4096u
#define SYS_CFG_TASK_IDLE_STK_SIZE 1024u
#define SYS_CFG_TASK_STAT_STK_SIZE 1024u

/*
*********************************************************************************************************
*                                       静态全局变量
*********************************************************************************************************
*/
static TX_THREAD _sysTaskStart;
static CHAR _sysTaskStartStk[SYS_CFG_TASK_START_STK_SIZE];

static TX_THREAD _sysTaskIdle;
static CHAR _sysTaskIdleStk[SYS_CFG_TASK_IDLE_STK_SIZE];

static TX_THREAD _sysTaskStatistic;
static CHAR _sysTaskStatStk[SYS_CFG_TASK_STAT_STK_SIZE];

/*
*********************************************************************************************************
*                                      函数声明
*********************************************************************************************************
*/
static void _sys_task_start(ULONG thread_input);
static void _sys_task_idle(ULONG thread_input);
static void _sys_task_statistic(ULONG thread_input);

static void _os_statistic_init(void);

/*
*******************************************************************************************************
*                               变量
*******************************************************************************************************
*/

static __IO uint8_t _osStatisticReady;
static __IO uint32_t _osIdleCount;
static __IO float _osCpuUsage;
static uint32_t _osIdleCountMax;
static uint32_t _osIdleCountRun;

int appMain(void)
{
    // 内核开启前关闭HAL的时间基
    HAL_SuspendTick();

    // 进入ThreadX内核
    tx_kernel_enter();

    while (1)
        ;
}

void tx_application_define(void *first_unused_memory)
{

    // 如果实现任务CPU利用率统计的话，此函数仅用于实现启动任务，统计任务和空闲任务，其它任务在函数
    //   _sys_task_create里面创建

    /**************创建启动任务*********************/
    tx_thread_create(&_sysTaskStart,              // 任务控制块地址
                     "App Task Start",            // 任务
                     _sys_task_start,             // 启动任务函数地址
                     0,                           // 传递给任务的参
                     &_sysTaskStartStk[0],        // 堆栈基地址
                     SYS_CFG_TASK_START_STK_SIZE, // 堆栈空间大小
                     SYS_CFG_TASK_START_PRIO,     // 任务优先
                     SYS_CFG_TASK_START_PRIO,     // 任务抢占阀
                     TX_NO_TIME_SLICE,            // 不开启时间片
                     TX_AUTO_START);              // 创建后立即启

    /**************创建统计任务*********************/
    tx_thread_create(&_sysTaskStatistic,         // 任务控制块地址
                     "App Task STAT",            // 任务
                     _sys_task_statistic,        // 启动任务函数地址
                     0,                          // 传递给任务的参
                     &_sysTaskStatStk[0],        // 堆栈基地址
                     SYS_CFG_TASK_STAT_STK_SIZE, // 堆栈空间大小
                     SYS_CFG_TASK_STAT_PRIO,     //任务优先
                     SYS_CFG_TASK_STAT_PRIO,     // 任务抢占阀
                     TX_NO_TIME_SLICE,           // 不开启时间片
                     TX_AUTO_START);             // 创建后立即启

    /**************创建空闲任务*********************/
    tx_thread_create(&_sysTaskIdle,              // 任务控制块地址
                     "App Task IDLE",            // 任务
                     _sys_task_idle,             //启动任务函数地址
                     0,                          // 传递给任务的参
                     &_sysTaskIdleStk[0],        // 堆栈基地址
                     SYS_CFG_TASK_IDLE_STK_SIZE, // 堆栈空间大小
                     SYS_CFG_TASK_IDLE_PRIO,     // 任务优先
                     SYS_CFG_TASK_IDLE_PRIO,     //任务抢占阀
                     TX_NO_TIME_SLICE,           // 不开启时间片
                     TX_AUTO_START);             // 创建后立即
};

static void _sys_task_start(ULONG thread_input)
{
    (void)thread_input;
    safe_printf_init();

    // 优先执行任务统计
    _os_statistic_init();

    // 内核开启后，恢复HAL里的时间基准
    HAL_ResumeTick();

    // 创建任务
    app_task_create();

    while (1)
    {
        // 需要周期性处理的程序，对应裸机工程调用的SysTick_ISR
        //bsp_ProPer1ms();
        tx_thread_sleep(1000);
    }
};

void _os_statistic_init(void)
{
    TX_INTERRUPT_SAVE_AREA
    _osStatisticReady = 0;

    tx_thread_sleep(2u); // 时钟同步

    TX_DISABLE
    _osIdleCount = 0uL; // 清空闲计
    TX_RESTORE

    tx_thread_sleep(100); // 统计100ms内，最大空闲计

    TX_DISABLE
    _osIdleCountMax = _osIdleCount; //保存最大空闲计
    _osStatisticReady = 1;
    TX_RESTORE
}

static void _sys_task_statistic(ULONG thread_input)
{
    TX_INTERRUPT_SAVE_AREA(void)
    thread_input;

    while (_osStatisticReady == 0)
    {
        tx_thread_sleep(200); // 等待统计任务就绪
    }

    _osIdleCountMax /= 100uL;
    if (_osIdleCountMax == 0uL)
    {
        _osCpuUsage = 0u;
    }
    TX_DISABLE
    _osIdleCount = _osIdleCountMax * 100uL; // 设置初始CPU利用
    TX_RESTORE

    for (;;)
    {
        TX_DISABLE
        _osIdleCountRun = _osIdleCount; //获得100ms内空闲计
        _osIdleCount = 0uL;             // 复位空闲计数
        TX_RESTORE                      // 计算100ms内的CPU利用
            _osCpuUsage = (100uL - (float)_osIdleCountRun / _osIdleCountMax);
        tx_thread_sleep(100); // ms统计一
    }
}

static void _sys_task_idle(ULONG thread_input)
{
    TX_INTERRUPT_SAVE_AREA

    while (1)
    {
        TX_DISABLE
        _osIdleCount++;
        TX_RESTORE
    }
}

void task_info_display(void)
{
    TX_THREAD *p_tcb = &_sysTaskStart;

    // 打印标题
    safe_printf("===============================================================\r\n");
    safe_printf("OS CPU Usage = %5.2f%%\r\n", _osCpuUsage);
    safe_printf("===============================================================\r\n");
    safe_printf("   Prio     StackSize   CurStack    MaxStack   Taskname\r\n");

    // 遍历任务控制块列TCB list)，打印所有的任务的优先级和名
    while (p_tcb != (TX_THREAD *)0)
    {

        safe_printf("   %2d        %5d      %5d       %5d      %s\r\n",
                    p_tcb->tx_thread_priority,
                    p_tcb->tx_thread_stack_size,
                    (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_ptr,
                    (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_highest_ptr,
                    p_tcb->tx_thread_name);

        p_tcb = p_tcb->tx_thread_created_next;

        if (p_tcb == &_sysTaskStart)
            break;
    }
}
