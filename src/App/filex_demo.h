#ifndef ___FILEX_DEMO_H__
#define ___FILEX_DEMO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "fx_api.h"
#include "fx_sd_driver.h"

    void thread_2_entry(ULONG thread_input);
    void fx_application_define(void *first_unused_memory);
#ifdef __cplusplus
}
#endif

#endif // ___FILEX_DEMO_H__