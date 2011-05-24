/*  Copyright (c) 2007 Atheros Communications Inc. All rights reserved. */

#ifndef _QUEUE_H
#define _QUEUE_H

#include "oal_dt.h"

struct zsQueueCell
{
    u32_t   tick;
    zbuf_t* buf;
};

struct zsQueue
{
    u16_t   size;
    u16_t   sizeMask;
    u16_t   head;
    u16_t   tail;
    struct zsQueueCell cell[1];
};

#endif //#ifndef _QUEUE_H
