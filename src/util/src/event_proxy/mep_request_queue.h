
#ifndef _H_MEP_REQ_Q_
#define _H_MEP_REQ_Q_

#include "event_proxy.h"
#include "ossp.h"

typedef ptr_event_object_handler EP_HDL_P;
typedef unsigned int mep_req_handle;
typedef unsigned int mep_req_cmd;
typedef struct _mep_req_q mep_req_q_t;
typedef void (*mep_cpreq_cb)(char* rsp, int rsp_len, void* cp_cb, void* cp_data, void* usr_data);

struct _mep_req_q{
	mep_req_handle		hdl;

	void* 			data;
	unsigned int		data_len;
	mep_cpreq_cb		cb;
	void* 			usr_data;
	struct _mep_req_q* 	next;
	int			isCanceled;
	int			ok2Send;
	unsigned int 		counter;
	void*			cp_cb;
	void*			cp_usr_data;
	struct timeval		time_stamp;
};

#define MREQ_CMD_SZ	sizeof(mep_req_cmd)
#define MREQ_HDL_SZ	sizeof(mep_req_handle)
#define MREQ_HEAD_SZ	(MREQ_HDL_SZ + MREQ_CMD_SZ)
#define MREQ mep_req_q_t
#define M_REQ_OK	88
#define M_REQ_NOK	0

/*
 *  Send http get data request from OSD CP to OSSP
 */
#define MREQ_CMD_GETDATA	0x01

/*
 *  Send cancel request from OSD CP.
 *  OSD CP will delete the mreq node from link list,
 *  OSSP will cancel the curl request.
 */
#define MREQ_CMD_CANCEL		0x02

/*
 *  Send file name path from OSSP to OSD CP
 *  OSD CP will open and read the file then remove it.
 */
#define MREQ_CMD_FILE		0x04

/*
 *  Send data with xml format request from OSD CP to OSSP
 */
#define MREQ_CMD_XML		0x08

/*
 * Check connection.
 */
#define MREQ_CMD_CONNECT	0X10

/*
 *  It's a dummy definition
 */
#define MREQ_CMD_XXX		0xffffffff


/*
 *  Command type of xml format
 */
#define MREQ_CMD_XML_SUB_POST		0x01

//Multi Event Proxy Request Queue Link List APIs.
EP_HDL_P mep_req_init(init_callback cb);
mep_req_q_t* mep_req_create();
mep_req_q_t* mep_req_create_ex(char* data, char* usr_data);
int mep_req_send(mep_req_cmd ossp_cmd, mep_req_handle hdl, char* data, int data_len, mep_cpreq_cb cb, void* usr_data);
int mep_req_destory(mep_req_handle hdl);
int mep_req_cancel(mep_req_handle hdl);
void mep_dump();
int mep_req_check(MREQ* mreq); //OK, retuen 0 .  mreq not found in Mreq LL returns -1.
MREQ* mep_get_head();
//int mep_req_set(mep_req_q_t* mreq, char* data, int data_len, void* cb, void* usr_data);

void DumpHex(char * buffer, int size);

//Multi Event Proxy Request Queue Link List Manipulation functions.

#endif //_H_MEP_REQ_Q_
