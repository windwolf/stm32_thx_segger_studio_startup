#ifndef __utils_h__
#define __utils_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

#ifdef DEBUG
#define LOG(fmt, ...)           \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n");
#else
#define LOG(fmt, ...) 
#endif

#ifdef __cplusplus
}
#endif

#endif // __utils_h__