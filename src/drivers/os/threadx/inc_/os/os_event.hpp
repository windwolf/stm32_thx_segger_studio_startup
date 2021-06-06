// //
// // Created by windwolf on 2021/4/29.
// //

// #ifndef STARTUP_OS_EVENT_HPP
// #define STARTUP_OS_EVENT_HPP

// #include "stdint.h"
// #include "tx_api.h"

// namespace windwolf::os {
//     struct OsEvent {

//         ULONG flags;

//         OsEvent(const char *name);

//         OsEvent(const char *name, bool reuse);

//         OsEvent(const OsEvent &value);

//         void set();

//         void set(ULONG flags);

//         bool peek(int32_t timeout);

//         bool peek(ULONG flags, int32_t timeout);

//         bool get(int32_t timeout);

//         bool get(ULONG flags, int32_t timeout);

//     private:
//         TX_EVENT_FLAGS_GROUP _host{};
//     };

// }
// #endif //STARTUP_OS_EVENT_HPP
