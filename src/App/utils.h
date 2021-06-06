#ifndef __utils_h__
#define __utils_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

#define LOG(fmt, ...)           \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n");

#ifdef __cplusplus
}
#endif

#endif // __utils_h__