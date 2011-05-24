/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: event_transfer_api.h,v 1.1.2.3 2006-12-11 11:34:14 divers_dai Exp $
 *vim:cindent:ts=8 sts=8 sw=8
 */
#ifndef __EVENT_TRANSFER_API_H__
#define __EVENT_TRANSFER_API_H__
#include "event_proxy.h"

unsigned char transfer_fifo_init(char *fifo_read,char *fifo_write,int *readfd,int *writefd,int *dummy_rdfd,int *dummy_wrfd);
unsigned char transfer_fifo_close(int fd);
int transfer_fifo_write(int writefd, void *event,unsigned int event_size);
int transfer_fifo_read(int readfd, void *event,unsigned int event_size);
unsigned char transfer_inquire_read(int readfd,unsigned char *result);
unsigned char transfer_fifo_clear_all_data(int readfd);



#endif
