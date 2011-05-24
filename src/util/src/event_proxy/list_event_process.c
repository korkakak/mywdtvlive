/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: list_event_process.c,v 1.1.2.1 2007-03-06 04:32:19 divers_dai Exp $
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
#include "list_event_process.h"

//init ,use list.allocate the head.
static unsigned char  event_proxy_list_init(ptr_event_object_handler ep_handle,list_free_data_callback cb)
{
	event_proxy_list_t *p_instance = NULL;
	if (ep_handle == NULL){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	p_instance = (event_proxy_list_t *)ep_handle;	
	p_instance->destory_event_cb = cb;
	p_instance->head = NULL;
	return EVENT_PROXY_SUCCESS; 
}



//add one event to the list tail.
static unsigned char event_proxy_list_send_event(ptr_event_object_handler ep_handle,void *event,int event_len)
{
	event_proxy_list_data *p=NULL;
	event_proxy_list_data *v=NULL;
	event_proxy_list_data *insert_data = NULL;
	event_proxy_list_t *p_instance = NULL;

	if((ep_handle == NULL)||(event == NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	p_instance = (event_proxy_list_t *)ep_handle;
	insert_data = (event_proxy_list_data *)malloc(sizeof(event_proxy_list_data));
	if (insert_data == NULL){
		printf("***%s:%s malloc fail***\n",__FILE__,__FUNCTION__);
		return EVENT_PROXY_FAIL;
	}
	insert_data->data = event;
	insert_data->next = NULL;
	v = p_instance->head;
	// go to the list tailer.
	while(v){
		p = v;
		v = v->next;
	}
	if(p==v){
		/* the list is null */
		p_instance->head = insert_data;
	}else{
		p->next = insert_data;
	}
	return EVENT_PROXY_SUCCESS;
}


//get event data from the event list head.
static unsigned char event_proxy_list_get_event(ptr_event_object_handler ep_handle, void **event,int *event_len)
{
	
	event_proxy_list_data *p = NULL;
	event_proxy_list_t *p_instance = NULL;

	if((ep_handle == NULL)||(event == NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	if (event_len != NULL)
		*event_len = -1;

	p_instance = (event_proxy_list_t *)ep_handle;
	p = p_instance->head;
	//the list have data item
	if(p){
		p_instance->head = p->next;
		p->next = NULL;
		*event = p->data;
		free(p);
		p = NULL;
	}else{
		*event = NULL;
	}
	return EVENT_PROXY_SUCCESS;

}


//to poll if there have data in the list.
static unsigned char event_proxy_list_poll_event(ptr_event_object_handler ep_handle,unsigned char *result)
{
	event_proxy_list_data *p = NULL;
	event_proxy_list_t *p_instance = NULL;

	if ((result == NULL)||(ep_handle == NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	p_instance = (event_proxy_list_t *)ep_handle;
	p = p_instance->head;
	if(p){
		*result = 1;
		return EVENT_PROXY_SUCCESS;
	}else{
		*result = 0;
		return EVENT_PROXY_SUCCESS;
	}
}


//free all node in the list¡£
static unsigned char event_proxy_list_clearall(ptr_event_object_handler ep_handle)
{
	event_proxy_list_data *p = NULL;
	event_proxy_list_data  *v = NULL;
	event_proxy_list_t *p_instance = NULL;

	if(ep_handle==NULL){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

        p_instance = (event_proxy_list_t *)ep_handle;
	p = p_instance->head;
	if (p == NULL){
		return EVENT_PROXY_SUCCESS;
	}

	if(p_instance->destory_event_cb == NULL){	
		//fprintf(stderr,"*** the destory event cb is NULL,so you have to destory it yourself:%s***\n",__FILE__,__FUNCTION__);
	}
	while(p){
		v = p->next;
		if (p_instance->destory_event_cb)
			p_instance->destory_event_cb(p->data);
		p->data = NULL;
		p->next = NULL;
		free(p);
		p = v;
	}
	p_instance->head = NULL;
	return EVENT_PROXY_SUCCESS;

}


//free the head space.
static unsigned char event_proxy_list_close(ptr_event_object_handler ep_handle)
{
	event_proxy_list_t *p_instance = NULL;
	if(ep_handle==NULL){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

        p_instance = (event_proxy_list_t *)ep_handle;
	if (p_instance->head != NULL){	
		if (event_proxy_list_clearall(ep_handle) == EVENT_PROXY_FAIL){ 
			fprintf(stderr,"***clear the list item fail ,so check it%s:%s\n",__FILE__,__FUNCTION__); 
			return EVENT_PROXY_FAIL;
		}
		p_instance->head = NULL;
	}
	return EVENT_PROXY_SUCCESS;
}


event_proxy_ops list_event_ops_proxy ={
        event_proxy_list_send_event,
        event_proxy_list_get_event,
	event_proxy_list_poll_event,
        event_proxy_list_clearall
};


unsigned char destory_event_proxy_list(ptr_event_object_handler ep_handle)
{
	if ((ep_handle ==NULL)||(ep_handle->instance_type!=EVENT_PROXY_LIST)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
                return EVENT_PROXY_PARAERR;
	}
	event_proxy_list_close(ep_handle);
	free(ep_handle);
	return EVENT_PROXY_SUCCESS;
}


ptr_event_object_handler create_event_proxy_list(int type, list_free_data_callback cb)
{
	event_proxy_t *parent;
        event_proxy_list_t * p_instance = NULL;
        p_instance = malloc(sizeof(event_proxy_list_t));
	if (p_instance == NULL)
		return NULL;
        memset(p_instance,0,sizeof(event_proxy_list_t));
        p_instance->parent.instance_type = type;
        p_instance->parent.ops_fun = &list_event_ops_proxy;
	p_instance->head = NULL;
	parent = (event_proxy_t*)p_instance;
	if(event_proxy_list_init(parent,cb)==EVENT_PROXY_SUCCESS){
		return parent;
	}else{
		destory_event_proxy_list(parent);
                p_instance = NULL;
                parent = NULL;
                fprintf(stderr,"***%s:%d do fail***\n",__FUNCTION__,__LINE__);
                return NULL;
	}
}

