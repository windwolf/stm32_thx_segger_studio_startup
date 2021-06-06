#ifndef ___SHARED_H__
#define ___SHARED_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>

#ifdef DEBUG
#define LOG(fmt, ...)           \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n");
#else
#define LOG(fmt, ...)
#endif
#define EVENTS_CLEAR_FLAGS(eg) (tx_event_flags_set(&eg, 0, TX_AND))
#define EVENTS_SET_FLAGS(eg, flags) (tx_event_flags_set(&eg, flags, TX_OR))
#define EVENTS_RESET_FLAGS(eg, flags) (tx_event_flags_set(&eg, ~flags, TX_AND))

#ifdef __cplusplus
}
#endif

#endif // ___SHARED_H__