#ifndef __RTL8712_EFUSE_H__
#define __RTL8712_EFUSE_H__

#include <drv_conf.h>
#include <osdep_service.h>

#define PG_STATE_HEADER 	0x01
#define PG_STATE_DATA		0x20

#define _REPEAT_THRESHOLD_  3
//------------------------------------------------------------------------------
typedef struct PG_PKT_STRUCT{
	u8 offset;
	u8 word_en;
	u8 data[8];	
}PGPKT_STRUCT,*PPGPKT_STRUCT;
//------------------------------------------------------------------------------
extern void 	efuse_access(_adapter * padapter, u8 bRead,u16 start_addr, u16 cnts, u8 *data);

extern void 	efuse_reg_init(_adapter * padapter);
extern void 	efuse_reg_uninit(_adapter * padapter);
extern void 	efuse_change_max_size(_adapter * padapter);
extern int 	efuse_pg_packet_read(_adapter * padapter,u8 offset,u8 *data);
extern int 	efuse_pg_packet_write(_adapter * padapter,const u8 offset,const u8 word_en,const u8 *data);
extern u16 	efuse_get_current_size(_adapter * padapter);
extern int 	efuse_get_max_size(_adapter * padapter);

#endif
