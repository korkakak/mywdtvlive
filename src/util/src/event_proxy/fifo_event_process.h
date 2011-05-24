/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: fifo_event_process.h,v 1.1.2.1 2006-12-11 11:36:35 divers_dai Exp $
 *vim:cindent:ts=8 sts=8 sw=8
 */
#ifndef __DEVICE_EVENT_PROCESS_H__
#define __DEVICE_EVENT_PROCESS_H__
#include "event_proxy.h"
unsigned char destroy_event_proxy_fifo(ptr_event_object_handler ep_handle);
ptr_event_object_handler create_event_proxy_fifo(int type,char *rd_fifo,char *wr_fifo,init_callback cb);

#endif

