/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: fifo_event_process.c,v 1.1.2.7 2007-11-01 05:30:07 divers_dai Exp $
 *vim:cindent:ts=8 sts=8 sw=8
 */
#include <stdio.h>
#include <memory.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include  <termios.h>
#include  <unistd.h>
#include  <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fifo_event_process.h"
#include "event_transfer_api.h"
#define DEB_INFO  1
#define FIFOPREHEAD   "/tmp/epfifo."
#define FIFOSEND_MAX_SIZE 1048576
static unsigned char event_proxy_fifo_close(ptr_event_object_handler ep_handle);
static unsigned char event_proxy_fifo_clearall(ptr_event_object_handler ep_handle);

static unsigned char  event_proxy_fifo_init(ptr_event_object_handler ep_handle,init_callback cb)
{	
	event_proxy_fifo_t * p_instance = NULL;

	if (ep_handle == NULL){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}
	p_instance = (event_proxy_fifo_t *)ep_handle;
	if (transfer_fifo_init(p_instance->fifo_recv,p_instance->fifo_send,&p_instance->readfd,&p_instance->writefd,&p_instance->dummy_rdfd,&p_instance->dummy_wrfd)==0){
		if (cb)
			cb((void *)&(p_instance->readfd),(void *)&(p_instance->writefd));
		return EVENT_PROXY_SUCCESS;
	}else{
		fprintf(stderr,"%s:%d fail\n",__FUNCTION__,__LINE__);
		event_proxy_fifo_close(ep_handle);
		return EVENT_PROXY_FAIL;
	}
}


//device proxy send event to stub function.and now it doesn't use.if using,filter the event information here first.
static unsigned char event_proxy_fifo_send_event(ptr_event_object_handler ep_handle,void *event,int event_len)
{
	event_proxy_fifo_t * p_instance = NULL;
	unsigned char *data;
	unsigned int ilen;
	int isendlen;
		

	if ((ep_handle == NULL)||(event == NULL)||(event_len<=0)){		
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}
	p_instance = (event_proxy_fifo_t *)ep_handle;
	//send the data len first and then send the real data.
	ilen = event_len+sizeof(int);
	//printf("%s to send data len:%d\n", __FUNCTION__, ilen);
	if (ilen > FIFOSEND_MAX_SIZE){
		printf("%s the len:%d is bigger than the max ,so fail to send\n", __FUNCTION__, ilen);
		return EVENT_PROXY_FAIL;

	}
	data = malloc(ilen+1);
	if (data!=NULL){
		memset(data,0,ilen+1);
		memcpy(data,&event_len,sizeof(int));
		memcpy(data+sizeof(int),event,event_len);
		isendlen = transfer_fifo_write(p_instance->writefd,data,ilen);
		free(data);
	        data = NULL;
		if (isendlen < ilen){
			fprintf(stderr,"%s:%d fail,the really send len is smaller the the to send \n",__FUNCTION__,__LINE__);
			return EVENT_PROXY_FAIL;
		}else {
			return EVENT_PROXY_SUCCESS;
		}
	}else{
		fprintf(stderr,"%s:%d fail\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_FAIL;
	}	

}


//device proxy get event data from strub.
static unsigned char event_proxy_fifo_get_event(ptr_event_object_handler ep_handle, void **event,int *event_len)
{
	event_proxy_fifo_t * p_instance = NULL;
	int rdlen;
	unsigned char *data=NULL;

	if ((ep_handle == NULL)||(event == NULL)||(event_len==NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}
	p_instance = (event_proxy_fifo_t *)ep_handle;

	*event = NULL;

	//read the event_len and then read the real data
	rdlen = sizeof(int);
	if(transfer_fifo_read(p_instance->readfd,event_len,rdlen)==rdlen){
		rdlen = *event_len;
		//fprintf(stderr, "%s to read data len:%d\n", __FUNCTION__, rdlen + sizeof(int));
		if (rdlen > FIFOSEND_MAX_SIZE){
			fprintf(stderr, "%s the len:%d is bigger than the max ,so fail to send\n", __FUNCTION__, rdlen);
			event_proxy_fifo_clearall(ep_handle);
			*event_len = 0;
			return EVENT_PROXY_FAIL;
		}
		data =  malloc(rdlen+1);
		
		if (data != NULL){
			data[rdlen] = '\0';
			if(transfer_fifo_read(p_instance->readfd,data,rdlen)==rdlen){
				*event =(void *)data;
				return EVENT_PROXY_SUCCESS;
			}else{
				free(data);
				data = NULL;
				*event = NULL;
				*event_len = 0;
				fprintf(stderr,"%s:%d fail:rdlen:%d\n",__FUNCTION__,__LINE__,rdlen);
		                return EVENT_PROXY_FAIL;
			}
		
		}else{
			*event_len = 0;
			*event = NULL;
			fprintf(stderr,"%s:%d malloc data fail\n",__FUNCTION__,__LINE__);
			return EVENT_PROXY_FAIL;
		}
	}else{
		*event_len = 0;
		*event = NULL;
		fprintf(stderr,"%s:%d malloc datalen fail\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_FAIL;
	}
}




//clear the name pipe's unhandle's event's data.
static unsigned char event_proxy_fifo_clearall(ptr_event_object_handler ep_handle)
{
	event_proxy_fifo_t * p_instance = NULL;

	if (ep_handle == NULL){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	p_instance = (event_proxy_fifo_t *)ep_handle;
	return transfer_fifo_clear_all_data(p_instance->readfd);

}

static unsigned char event_proxy_fifo_poll_event(ptr_event_object_handler ep_handle,unsigned char *result)
{

	event_proxy_fifo_t * p_instance = NULL;
	if ((ep_handle == NULL)||(result == NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}
	p_instance = (event_proxy_fifo_t *)ep_handle;
	return transfer_inquire_read(p_instance->readfd,result);
}


static unsigned char event_proxy_fifo_close(ptr_event_object_handler ep_handle)
{
	event_proxy_fifo_t * p_instance = NULL;

	if (ep_handle == NULL){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}
	p_instance = (event_proxy_fifo_t *)ep_handle;

	if((p_instance->writefd == -1)&&(p_instance->readfd == -1)&&(p_instance->dummy_wrfd == -1)&&(p_instance->dummy_rdfd == -1))
		return EVENT_PROXY_SUCCESS;

	if (p_instance->writefd >= 0)
		transfer_fifo_close(p_instance->writefd);
	if (p_instance->readfd >= 0){
		event_proxy_fifo_clearall(ep_handle);
		transfer_fifo_close(p_instance->readfd);
	}
	if (p_instance->dummy_wrfd >= 0)
		transfer_fifo_close(p_instance->dummy_wrfd);
	if (p_instance->dummy_rdfd >= 0)
		transfer_fifo_close(p_instance->dummy_rdfd);
	p_instance->writefd = -1;
	p_instance->readfd = -1;
	p_instance->dummy_wrfd = -1;
	p_instance->dummy_rdfd = -1;
	return EVENT_PROXY_SUCCESS;
}

event_proxy_ops event_proxy_fifo_ops ={
        event_proxy_fifo_send_event,
        event_proxy_fifo_get_event,
        event_proxy_fifo_poll_event,
        event_proxy_fifo_clearall
};



ptr_event_object_handler create_event_proxy_fifo(int type,char *rd_fifo,char *wr_fifo,init_callback cb)
{
        event_proxy_t *parent;
        event_proxy_fifo_t * p_instance = NULL;
	int ilen;
	
	if ((rd_fifo==NULL)||(wr_fifo==NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return NULL;
	}

        p_instance = malloc(sizeof(event_proxy_fifo_t));
        memset(p_instance,0,sizeof(event_proxy_fifo_t));
        p_instance->parent.instance_type = type;
        p_instance->readfd = -1;
        p_instance->writefd = -1;
        p_instance->dummy_wrfd = -1;
        p_instance->dummy_rdfd = -1;

	ilen = strlen(FIFOPREHEAD)+strlen(wr_fifo)+1;
	p_instance->fifo_send = NULL;
        p_instance->fifo_send = malloc(ilen);
	if (p_instance->fifo_send != NULL){
		memset(p_instance->fifo_send,0,ilen);
		strcpy(p_instance->fifo_send,FIFOPREHEAD);
		strcat(p_instance->fifo_send,wr_fifo);
	}else{
		free(p_instance);
		p_instance = NULL;
		fprintf(stderr,"%s:%d do fail\n",__FUNCTION__,__LINE__);
		return NULL;
	}

	ilen = strlen(FIFOPREHEAD)+strlen(rd_fifo)+1;
        p_instance->fifo_recv = NULL;
	p_instance->fifo_recv = malloc(ilen);
	if(p_instance->fifo_recv!=NULL){
		memset(p_instance->fifo_recv,0,ilen);
		strcpy(p_instance->fifo_recv,FIFOPREHEAD);
		strcat(p_instance->fifo_recv,rd_fifo);
	}else{
		free(p_instance->fifo_send);
		p_instance->fifo_send = NULL;
		fprintf(stderr,"***%s:%d do fail\n",__FUNCTION__,__LINE__);
		free(p_instance);
                p_instance = NULL;
                return NULL;
	}

	p_instance->parent.ops_fun = &event_proxy_fifo_ops;

        parent = (event_proxy_t*)p_instance;
	if (event_proxy_fifo_init(parent,cb)==EVENT_PROXY_SUCCESS){
	        return parent;
	}else{
		
		destroy_event_proxy_fifo(parent);
		p_instance = NULL;
		parent = NULL;
		fprintf(stderr,"%s:%d do fail\n",__FUNCTION__,__LINE__);
		return NULL;
	}

}
unsigned char destroy_event_proxy_fifo(ptr_event_object_handler ep_handle)
{
	event_proxy_fifo_t * p_instance = NULL;

	if ((ep_handle == NULL) ||(ep_handle->instance_type!=EVENT_PROXY_FIFO)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	p_instance = (event_proxy_fifo_t *)ep_handle;
	event_proxy_fifo_close(ep_handle);
	if (p_instance->fifo_send != NULL){
		free(p_instance->fifo_send);
		p_instance->fifo_send = NULL;
	}
	if (p_instance->fifo_recv != NULL){
		free(p_instance->fifo_recv);
		p_instance->fifo_recv = NULL;
	}

	free(ep_handle);
	return EVENT_PROXY_SUCCESS;
}

