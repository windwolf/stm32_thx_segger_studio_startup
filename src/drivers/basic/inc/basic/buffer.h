#ifndef ___BUFFER_H__
#define ___BUFFER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    typedef struct Buffer
    {
        void *data;
        uint32_t size;
        //uint32_t count;
    } Buffer;

    // void Buffer_Init(Buffer *buffer, void *data, uint32_t size);
    // void Buffer_SetCount8(Buffer *buffer, uint32_t count);

#ifdef __cplusplus
}
#endif

#endif // ___BUFFER_H__