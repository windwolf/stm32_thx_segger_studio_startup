// //
// // Created by windwolf on 2021/4/29.
// //

// #ifndef STARTUP_OS_THREAD_HPP
// #define STARTUP_OS_THREAD_HPP

// #include "tx_api.h"

// namespace windwolf::os {
//     struct OsThread {
//         OsThread(const char *name, void (*threadFunction)(ULONG), ULONG threadInput, void *stack, ULONG stackSize, UINT priority,
//                  UINT preempt_threshold, ULONG time_slice);

//         OsThread(void *stack, ULONG stackSize);

//         void start(const char *name, void (*threadFunction)(ULONG), ULONG threadInput, UINT priority,
//                    UINT preempt_threshold, ULONG time_slice);

//         void stop();

//     private:
//         TX_THREAD _host;
//         void *_stack;
//         ULONG _stackSize;
//     };
// }


// #endif //STARTUP_OS_THREAD_HPP
