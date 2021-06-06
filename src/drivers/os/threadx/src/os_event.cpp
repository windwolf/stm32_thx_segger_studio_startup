// //
// // Created by windwolf on 2021/4/29.
// //

// #include "common/shared.hpp"
// #include "os/os_event.hpp"
// #include "tx_api.h"

// using namespace windwolf::os;


// OsEvent::OsEvent(const char *name)
//         : OsEvent(name, false) {
// }

// OsEvent::OsEvent(const char *name, bool reuse) {
//     if (!reuse) {
//         tx_event_flags_create(&_host, const_cast<CHAR *>(name));
//     }
// }

// OsEvent::OsEvent(const OsEvent &value) {
//     _host = value._host;
//     flags = value.flags;
// }


// void OsEvent::set() {
//     tx_event_flags_set(&_host, flags, TX_AND);
// }

// void OsEvent::set(ULONG flags) {
//     tx_event_flags_set(&_host, flags, TX_AND);
// }


// bool OsEvent::peek(int32_t timeout) {
//     ULONG actualFlags;
//     if (timeout < 0) {
//         tx_event_flags_get(&_host, flags, TX_AND, &actualFlags, TX_WAIT_FOREVER);
//     } else if (timeout == 0) {
//         tx_event_flags_get(&_host, flags, TX_AND, &actualFlags, TX_NO_WAIT);
//     } else {
//         tx_event_flags_get(&_host, flags, TX_AND, &actualFlags, timeout);
//     }
//     return actualFlags;
// }

// bool OsEvent::peek(ULONG flags, int32_t timeout) {
//     ULONG actualFlags;
//     if (timeout < 0) {
//         tx_event_flags_get(&_host, flags, TX_AND, &actualFlags, TX_WAIT_FOREVER);
//     } else if (timeout == 0) {
//         tx_event_flags_get(&_host, flags, TX_AND, &actualFlags, TX_NO_WAIT);
//     } else {
//         tx_event_flags_get(&_host, flags, TX_AND, &actualFlags, timeout);
//     }
//     return actualFlags;
// }

// bool OsEvent::get(int32_t timeout) {
//     ULONG actualFlags;
//     if (timeout < 0) {
//         tx_event_flags_get(&_host, flags, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
//     } else if (timeout == 0) {
//         tx_event_flags_get(&_host, flags, TX_AND_CLEAR, &actualFlags, TX_NO_WAIT);
//     } else {
//         tx_event_flags_get(&_host, flags, TX_AND_CLEAR, &actualFlags, timeout);
//     }
//     return actualFlags;
// }

// bool OsEvent::get(ULONG flags, int32_t timeout) {
//     ULONG actualFlags;
//     if (timeout < 0) {
//         tx_event_flags_get(&_host, flags, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
//     } else if (timeout == 0) {
//         tx_event_flags_get(&_host, flags, TX_AND_CLEAR, &actualFlags, TX_NO_WAIT);
//     } else {
//         tx_event_flags_get(&_host, flags, TX_AND_CLEAR, &actualFlags, timeout);
//     }
//     return actualFlags;
// }












