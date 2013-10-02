/*
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#include "bsp_types.h"

#include <stdio.h>
#include "RT_Agent.h"

/*  Generic types used in lwIP. */
typedef uint8_t  u8_t;
typedef int8_t   s8_t;
typedef uint16_t u16_t;
typedef int16_t  s16_t;
typedef uint32_t u32_t;
typedef int32_t  s32_t;
typedef u32_t    mem_ptr_t;


#define BYTE_ORDER LITTLE_ENDIAN

/* Compiler hints for packing structures. */
#define PACK_STRUCT_BEGIN  __packed
#define PACK_STRUCT_STRUCT

#define LWIP_PLATFORM_ASSERT(x)
#define LWIP_PLATFORM_DIAG(x)    

#define LWIP_PROVIDE_ERRNO

/* LWIP_PLATFORM_BYTESWAP = 1 (all network/host byte swapping done by the 
   macros below). */
#define LWIP_PLATFORM_BYTESWAP  1
/* Byte swapping macros */
#define LWIP_PLATFORM_HTONS(x)  ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define LWIP_PLATFORM_HTONL(x)  ((((x) & 0xff) << 24) | \
                                (((x) & 0xff00) << 8) | \
                                (((x) & 0xff0000UL) >> 8) | \
                                (((x) & 0xff000000UL) >> 24))


/* Define printf formatters for diagnostics and debug. */
#define X8_F  "02x"
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"
#define SZT_F U32_F

#endif /* __ARCH_CC_H__ */
