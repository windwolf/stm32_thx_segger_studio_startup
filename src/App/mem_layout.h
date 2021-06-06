#ifndef __mem_layout_h__
#define __mem_layout_h__

#ifdef __cplusplus
extern "C"
{
#endif

#define CACHE_LINE_SIZE 32

#define DMA_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))

#define D1_BUFFER __attribute__((section(".RAM1.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define D2_BUFFER __attribute__((section(".RAM2.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define D3_BUFFER __attribute__((section(".RAM3.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))

#define D1_DATA __attribute__((section(".RAM1.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define D2_DATA __attribute__((section(".RAM2.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define D3_DATA __attribute__((section(".RAM3.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))

#ifdef __cplusplus
}
#endif

#endif // __mem_layout_h__