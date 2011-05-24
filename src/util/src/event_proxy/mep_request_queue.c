#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mep_request_queue.h"

//=========== Variables ===============
MREQ* g_mreq_head = (MREQ*)NULL;
EP_HDL_P g_ep_hdl = NULL;

static MREQ* mep_req_new(void);
static int mep_req_add(MREQ* node);
static MREQ* mep_req_search(mep_req_handle hdl);
static int mep_req_del_hdl(mep_req_handle hdl);
static void mep_req_dump(void);
static int mep_req_free(MREQ* node);

//============== Public API ================
ptr_event_object_handler mep_req_init(init_callback cb)
{
	if(!g_ep_hdl)
		g_ep_hdl = create_event_proxy(EVENT_PROXY_FIFO, MF_WHO_KNOWS, MF_WHO_CARES, cb);
	//fprintf(stderr,"%s %s %d g_ep_hdl:%p.\n",__FILE__,__FUNCTION__,__LINE__, g_ep_hdl);
	return g_ep_hdl;
}


//WW: Create a Multiple event proxy request node and adding it into the link list.
mep_req_q_t* mep_req_create()
{
	mep_req_q_t* mreq = NULL;
	if((mreq = (MREQ*)mep_req_new())){
		mep_req_add(mreq);
		return mreq;
	}else{
		fprintf(stderr, "%s %s %d mep_req create failed.\n", __FILE__, __FUNCTION__, __LINE__);
		return (mep_req_q_t*)NULL;
	}
}


mep_req_q_t* mep_req_create_ex(char* data, char* usr_data)
{
	mep_req_q_t* mreq = NULL;
	if((mreq = (MREQ*)mep_req_new())){
		if(data) mreq->data = data;
		if(usr_data) mreq->usr_data = usr_data;
		mep_req_add(mreq);
		return mreq;
	}else{
		fprintf(stderr, "%s %s %d mep_req create failed.\n", __FILE__, __FUNCTION__, __LINE__);
		return (mep_req_q_t*)NULL;
	}
}



//int mep_req_send(mep_req_handle hdl, char* data, int data_len, mep_req_cb cb, void* usr_data){
int mep_req_send(mep_req_cmd ossp_cmd, mep_req_handle hdl, char* data, int data_len, mep_cpreq_cb cb, void* usr_data)
{
	int offset = 0;
	MREQ* node = mep_req_search(hdl);
	void* tmp_data = NULL;
	if(!node){
		fprintf(stderr, "%s %s %d mep_req send failed.\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
		//return M_REQ_NOK;
	}else{
		node->data = malloc(data_len);
		if(node->data==NULL){
			fprintf(stderr, "[%s:%s:%d] malloc failed\n", __FILE__, __FUNCTION__, __LINE__);
			return -1;
		}
		memcpy(node->data,data,data_len);
		node->data_len = data_len;
		node->cb = cb;
		node->usr_data = usr_data;
		node->ok2Send = 1;
		
		//WW: I insert a mep_req_handle into the packet to fifo_ep. it will be return when the OSSP reply.
		//And then I can check the handle of each node in the request queue to figure out which one this
		//response belongs to.
		tmp_data = (void*)malloc(data_len + MREQ_HEAD_SZ);
		memcpy(tmp_data, &ossp_cmd, MREQ_CMD_SZ);
		offset += MREQ_CMD_SZ;
		memcpy(tmp_data + offset, &hdl, MREQ_HDL_SZ);
		offset += MREQ_HDL_SZ;
		memcpy(tmp_data + offset, data, data_len);
	}
	
	if(g_ep_hdl && node->ok2Send && tmp_data){
		event_proxy_send_event(g_ep_hdl, tmp_data, data_len+MREQ_HEAD_SZ);
		//fprintf(stderr, "%s %s %d data_len:%d.\n", __FILE__,__FUNCTION__,__LINE__, data_len);
		//DumpHex(tmp_data, data_len+MREQ_HEAD_SZ);
	}

	if(tmp_data)free(tmp_data);
	return hdl;
}

int mep_req_destory(mep_req_handle hdl)
{
	MREQ* node = mep_req_search(hdl);
	if(!node){
		fprintf(stderr, "%s %s %d mep_req search and destory failed.\n", __FILE__, __FUNCTION__, __LINE__);
		return M_REQ_NOK;
	}else{
		fprintf(stderr, "%s %s %d mep_req search and destory OK.\n", __FILE__, __FUNCTION__, __LINE__);
		return mep_req_del_hdl(hdl);
	}
}

int mep_req_cancel(mep_req_handle hdl) 
{
	mep_req_cmd ossp_cmd;
	int offset = 0;
	MREQ* node = mep_req_search(hdl);
	void* tmp_data = NULL;
	if(!node){
		fprintf(stderr, "%s %s %d mep_req search and Cancel failed.\n", __FILE__, __FUNCTION__, __LINE__);
		return M_REQ_NOK;
	}else{
		node->ok2Send = 1;
		
		tmp_data = (void*)malloc(MREQ_HEAD_SZ);
		ossp_cmd = MREQ_CMD_CANCEL;
		memcpy(tmp_data, &ossp_cmd, MREQ_CMD_SZ);
		offset += MREQ_CMD_SZ;
		memcpy(tmp_data + offset, &hdl, MREQ_HDL_SZ);
		offset += MREQ_HDL_SZ;
		node->isCanceled = 44;
	}
	if(g_ep_hdl && node->ok2Send && tmp_data){
		event_proxy_send_event(g_ep_hdl, tmp_data, MREQ_HEAD_SZ);
		//DumpHex(tmp_data, MREQ_HEAD_SZ+64);
	}
	mep_req_destory(hdl);

	if(tmp_data)free(tmp_data);
	return M_REQ_OK;
}

void mep_dump()
{
	mep_req_dump();
}



int mep_req_check(MREQ* mreq)
{
	MREQ* node = g_mreq_head;
	while(node && mreq){
		if(node == mreq) return 0;
		else{
			node = node->next;
		}
	}
	return -1;
}

MREQ* mep_get_head()
{
	return g_mreq_head;
}


//============= Static Link List functions ====================/
static MREQ* mep_req_new()
{
	MREQ* mreq = NULL;
	static unsigned int mreq_handle_counter = 1;
	if((mreq = (MREQ*) malloc(sizeof(MREQ)))){
		mreq->next 	= NULL;
		mreq->cb 	= NULL;
		mreq->hdl 	= mreq_handle_counter;
		mreq->isCanceled = 0;
		mreq->ok2Send	= 0;
		mreq_handle_counter+=2;
		
		//if(mep_req_add(mreq)){	//WW: Don't try to get mise being a dog.
		//else{
		//	fprintf(stderr, "%s %s %d mep_req Add failed.\n", __FILE__, __FUNCTION__, __LINE__);
		//	return NULL;
		//}
		return mreq;
	}
	fprintf(stderr, "%s %s %d mep_req new failed.\n", __FILE__, __FUNCTION__, __LINE__);
	return NULL;
}


static MREQ* mep_req_search(mep_req_handle hdl)
{
	MREQ* node = g_mreq_head;
	if(!node){
		fprintf(stderr, "%s %s %d mep_req link list is still empty.\n", __FILE__, __FUNCTION__, __LINE__);
		return NULL;
	}

	do{
		if(node->hdl == hdl){
			return node;
		}else if(node->next){
				node = node->next;
		}else{
			fprintf(stderr, "%s %s %d mreq handle %d not Found!!\n", __FILE__, __FUNCTION__, __LINE__, hdl);
			return NULL;
		}
	}while(node);	
	
	fprintf(stderr, "%s %s %d WRONG!!\n", __FILE__, __FUNCTION__, __LINE__);
	return NULL;
}

static MREQ* mep_req_search_ex(mep_req_handle hdl)
{
	MREQ* node = g_mreq_head;
	MREQ* elder = g_mreq_head;
	if(!node){
		fprintf(stderr, "%s %s %d mep_req link list is still empty.\n", __FILE__, __FUNCTION__, __LINE__);
		return NULL;
	}

	do{
		if(node->hdl == hdl){
			return elder;
		}else if(node->next){
				elder = node;
				node = node->next;
		}else{
			fprintf(stderr, "%s %s %d mreq elder handle %d not Found!!\n", __FILE__, __FUNCTION__, __LINE__, hdl);
			return NULL;
		}
	}while(node);	
	
	fprintf(stderr, "%s %s %d WRONG!!\n", __FILE__, __FUNCTION__, __LINE__);
	return NULL;
}

//WW: Add on the top.
static int	mep_req_add(MREQ* node)
{

	if(!g_mreq_head){
		g_mreq_head = node; //Empty, it's the first new head.
	}else{
		node->next = g_mreq_head;
		g_mreq_head = node;
	}

	return M_REQ_OK;
}

#if 0
static int mep_req_del(MREQ* node)
{
	//WW: Search and Destory !!
	return 0;
}
#endif

static int mep_req_del_hdl(mep_req_handle hdl)
{
//	MREQ* tmp 	= NULL;
	MREQ* node 	= NULL;
	MREQ* elder	= NULL;
	
	elder 	= mep_req_search_ex(hdl);
	node	= mep_req_search(hdl);
	fprintf(stderr, "%s %s %d Going to Delete node with hdl:%d elder:%p node:%p\n",
		__FILE__, __FUNCTION__, __LINE__, hdl, elder, node);
	
	if(!elder || !node){
		fprintf(stderr, "%s %s %d Can not find and del the handle %d!!\n", __FILE__, __FUNCTION__, __LINE__, hdl);
		return M_REQ_NOK;
	}else{
		if(elder == node){
			//Deleting head
			if(node->next) 
				g_mreq_head = node->next;	//1th node but not alone.
			else
				g_mreq_head = NULL;
		}else{
			elder->next = node->next;
		}
		mep_req_free(node);
		return M_REQ_OK;
	}
}



static int mep_req_free(MREQ* node)
{
	if(node){
		if(node->data)
			free(node->data);
		free(node);
	}else{
		fprintf(stderr, "%s %s %d Free failed node is NULL!!\n", __FILE__, __FUNCTION__, __LINE__);
		return M_REQ_NOK;
	}
	return M_REQ_OK;
}

#if 0
static int mep_req_free_hdl(mep_req_handle hdl)
{
	MREQ* node = NULL;
	if((node = mep_req_search(hdl))){
		free(node);		
	}else{
		fprintf(stderr, "%s %s %d Search and Free failed hdl is NULL!!\n", __FILE__, __FUNCTION__, __LINE__);
		return M_REQ_NOK;
	}
	return M_REQ_OK;
}
#endif

static void mep_req_dump()
{
	MREQ* node = g_mreq_head;	
	int counter = 0;
	fprintf(stderr,"\n");
	while(node){
		counter++;	
		fprintf(stderr, "Node %d: %p\thdl:%d\t next:%p\n",counter, node, node->hdl, node->next);
		node=node->next;
	}
	fprintf(stderr, "Total %d node in MReq Queue LL.\n", counter);
}



void DumpHex(char * buffer, int size)
{
	int i,j;
	int col=16;
	unsigned char * buf;
	unsigned char tmp_char;
	
	buf = (unsigned char *)buffer;
	printf("---------------------------------- Hex Dump -----------------------------------\n");
	for (i=0;i<size;i++){
		if ((i%col)==0)
			printf("0x%08X | ", (unsigned int)(buf+i));
		
		printf("%02x ",*(buf+i));

		if ((i%col)==(col-1)){
			printf("| ");
			for (j=i+1-col;j<=i;j++){
				tmp_char=*(buf+j);
				if (tmp_char<32 || tmp_char>=128)
					tmp_char=32;
				putchar(tmp_char);
			}
			printf("\n");
		}
		else if (i==(size-1)){
			for (j=0;j<col-size%col;j++)
				printf("   ");
			printf("| ");
			for (j=i-i%col;j<=i;j++){
				tmp_char=*(buf+j);
				if (tmp_char<32 || tmp_char>=128)
					tmp_char=32;
				putchar(tmp_char);
			}
			printf("\n");
		}
	}
	printf("-----------+-------------------------------------------------+-----------------\n");
}
