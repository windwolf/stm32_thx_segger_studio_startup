#ifndef ___GPS_DEMO_H__
#define ___GPS_DEMO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "gnss_protocol/inc/nmea.h"
#include "tx_api.h"

    void thread_gps_entry(ULONG thread_input);
    void gps_application_define();

#ifdef __cplusplus
}
#endif

#endif // ___GPS_DEMO_H__