/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: list_event_process.h,v 1.1.2.1 2007-03-06 04:32:19 divers_dai Exp $
 *vim:cindent:ts=8 sts=8 sw=8
 */
#ifndef __VIIV_EVENT_PROCESS_H__
#define __VIIV_EVENT_PROCESS_H__
#include "event_proxy.h"

ptr_event_object_handler create_event_proxy_list(int type, list_free_data_callback cb);
unsigned char destory_event_proxy_list(ptr_event_object_handler ep_handle);
#endif

