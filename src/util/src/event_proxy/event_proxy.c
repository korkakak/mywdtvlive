/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: event_proxy.c,v 1.1.2.7 2007-11-06 02:09:08 divers_dai Exp $
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
#include "event_proxy.h"

#include "fifo_event_process.h"
#include "list_event_process.h"



/*
*Function description:to create one sub class object.
*@Param:int entity_type:the sub class object type to create (in)
*Realization:basis on the entity_type,call the relative function to create the sub 
* 	    class object.and initialise the variant and so on.
*Return value:
*	NULL:fail to create the object
*	not NULL:the adress of the object.
*/
ptr_event_object_handler create_event_proxy(int  entity_type,char *rd_info,char *wr_info,init_callback cb)
{
	switch(entity_type){
		case EVENT_PROXY_FIFO:
			return create_event_proxy_fifo(entity_type,rd_info,wr_info,cb);	
		default :
			return NULL;
		
	}
	return NULL;
}


ptr_event_object_handler create_list_event_proxy(int  entity_type,list_free_data_callback cb)
{
	if (entity_type == EVENT_PROXY_LIST)
		return create_event_proxy_list(entity_type,cb);
	else
		return NULL;

}

/*
*Function description:destory the sub class object.
*@Param:ptr_event_object_handler ep_handle:the object handle to destory.
*Realization:none
*Return value:
*	0:sucessful
*	>0:fail to do
*Other explain: none
*/
unsigned char destory_event_proxy(ptr_event_object_handler ep_handle)
{
	if(ep_handle != NULL){
		switch(ep_handle->instance_type){
			case  EVENT_PROXY_FIFO:
				return destroy_event_proxy_fifo(ep_handle);
			case EVENT_PROXY_LIST:
				return destory_event_proxy_list(ep_handle);
		}
	}
	return 1;
}




/*
*Function description:send the event data to the communicate channel.
*@Param:ptr_event_object_handler ep_handle:the object handle to send event data.
	void * event:the event information to be send.
	int event_len: the data len that want to send.
*Realization:accourding the handle,despatch to the real sub class 
*	object's realize function.in the realization,it send the event data 
*	to the channel or add it to the event info list based on the 
*	communication method.
* Return value:
*	0:sucessful
*	>0:fail to do
* Other explain: if using the name pipe communication method .
*	after calling it,it can free the event data.
*	and if use list ,it may not be free.according to the inner's 
*	realization on how to deal the event data.
*/

unsigned char event_proxy_send_event(ptr_event_object_handler ep_handle,void *event,int event_len)
{
	if ((ep_handle != NULL)&&(ep_handle->ops_fun!=NULL)&&(ep_handle->ops_fun->send_event != NULL))
		return ep_handle->ops_fun->send_event(ep_handle,event,event_len);
	else
		return 1;
}

/*
*Function description:call this function to get the event data info when there 
*	are event data in communication channel.
*@Param:ptr_event_object_handler ep_handle:the object handle to receive event data.*	   void **event:to get the event data's address.
*Realization:similar to the send data.
*Return value:
*       0:sucessful
*       >0:fail to do
*Other explain: after get the event data ,and deal well,user must free the event's *		space.otherwise,it will make the memory loss.
*/
unsigned char event_proxy_get_event(ptr_event_object_handler ep_handle, void **event,int *event_len)
{
	if ((ep_handle != NULL)&&(ep_handle->ops_fun!=NULL)&&(ep_handle->ops_fun->get_event != NULL))
		return ep_handle->ops_fun->get_event(ep_handle,event,event_len);
	else
		return 1;
}



/*
*Function description:call this function to poll are there any event data in the 
*	communication channel.
*@Param:ptr_event_object_handler ep_handle:the object handle to polling.
*	unsigned char *result:poll result.if there has data ,set 1,else set it 0
*Realization:
*Return value:
*       0:sucessful
*       >0:fail to do
*Other explain: support another method to get know are there some event data info.
*/
unsigned char event_proxy_poll_event(ptr_event_object_handler ep_handle,unsigned char *result)
{
	if ((ep_handle != NULL)&&(ep_handle->ops_fun!=NULL)&&(ep_handle->ops_fun->poll_event!= NULL))
		return ep_handle->ops_fun->poll_event(ep_handle,result);
	else
		return 1;
}


/*
*Function description:clear all the event data info in the communication channel.
*@Param:ptr_event_object_handler ep_handle:the object handle to clear.
*Realization:
*Return value:
*       0:sucessful
*       >0:fail to do
*Other explain: none
*/
unsigned char event_proxy_clear_all(ptr_event_object_handler ep_handle)
{
	if ((ep_handle != NULL)&&(ep_handle->ops_fun!=NULL)&&(ep_handle->ops_fun->clear_all != NULL))
		return ep_handle->ops_fun->clear_all(ep_handle);
	else
		return 1;
}






