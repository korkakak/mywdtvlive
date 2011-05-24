/************************************************************************/
/*  Copyright (c) 2007 Atheros Communications Inc. All rights reserved. */
/*                                                                      */
/*  Module Name : wrap_mem.c                                            */
/*                                                                      */
/*  Abstract                                                            */
/*     This module contains wrapper functions for memory management     */
/*                                                                      */
/*  NOTES                                                               */
/*     Platform dependent.                                              */
/*                                                                      */
/************************************************************************/

#include "oal_dt.h"
#include "usbdrv.h"

#include <linux/netlink.h>

#if WIRELESS_EXT > 12
#include <net/iw_handler.h>
#endif

/* Memory management */
/* Called to allocate uncached memory, allocated memory must    */
/* in 4-byte boundary                                           */
void* zfwMemAllocate(zdev_t* dev, u32_t size)
{
    void* mem = NULL;
    mem = kmalloc(size, GFP_ATOMIC);
    return mem;
}


/* Called to free allocated memory */
void zfwMemFree(zdev_t* dev, void* mem, u32_t size)
{
    kfree(mem);
    return;
}

void zfwMemoryCopy(u8_t* dst, u8_t* src, u16_t length)
{
    //u16_t i;

    memcpy(dst, src, length);
    //for(i=0; i<length; i++)
    //{
    //    dst[i] = src[i];
    //}
    return;
}

void zfwZeroMemory(u8_t* va, u16_t length)
{
    //u16_t i;
    memset(va, 0, length);
    //for(i=0; i<length; i++)
    //{
    //    va[i] = 0;
    //}
    return;
}

void zfwMemoryMove(u8_t* dst, u8_t* src, u16_t length)
{
    memcpy(dst, src, length);
    return;
}

u8_t zfwMemoryIsEqual(u8_t* m1, u8_t* m2, u16_t length)
{
    //u16_t i;
    int ret;
    
    ret = memcmp(m1, m2, length);
    
    return ((ret==0)?TRUE:FALSE);
    //for(i=0; i<length; i++)
    //{
    //    if ( m1[i] != m2[i] )
    //    {
    //        return FALSE;
    //    }
    //}

    //return TRUE;
}

/* Leave an empty line below to remove warning message on some compiler */
