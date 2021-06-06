#ifndef __tracex_h__
#define __tracex_h__

#include "tx_api.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define TRACEX_BUFFER_SIZE 4096
    static uint8_t tracex_buffer[TRACEX_BUFFER_SIZE];
    static UINT trace_status;
    void TraceX_EnableTrace()
    {
        trace_status = tx_trace_enable(tracex_buffer, TRACEX_BUFFER_SIZE, 10);
    }

#ifdef __cplusplus
}
#endif
#endif // __tracex_h__