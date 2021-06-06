// //
// // Created by windwolf on 2021/4/29.
// //

// #include "os/os_thread.hpp"
// #include "tx_api.h"

// using namespace windwolf::os;


// OsThread::OsThread(const char *name, void (*threadFunction)(ULONG), ULONG threadInput, void *stack, ULONG stackSize, UINT priority,
//                    UINT preempt_threshold, ULONG time_slice)
//         : OsThread(stack, stackSize) {
//     start(name, threadFunction, threadInput, priority, preempt_threshold, time_slice);
// }

// OsThread::OsThread(void *stack, ULONG stackSize)
//         : _stack(stack), _stackSize(stackSize) {
// }

// void OsThread::start(const char *name, void (*threadFunction)(ULONG), ULONG threadInput, UINT priority,
//                      UINT preempt_threshold, ULONG time_slice) {
//     tx_thread_create(&_host, const_cast<CHAR *>(name), threadFunction, threadInput, _stack, _stackSize, priority, preempt_threshold, time_slice,
//                      true);
// }

// void OsThread::stop() {
//     tx_thread_terminate(&_host);
// }



