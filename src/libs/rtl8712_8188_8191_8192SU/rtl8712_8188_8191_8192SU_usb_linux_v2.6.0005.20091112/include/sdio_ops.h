#ifndef __SDIO_OPS_H_
#define __SDIO_OPS_H_

#include <drv_conf.h>
#include <osdep_service.h>
#include <drv_types.h>
#include <osdep_intf.h>

#ifdef PLATFORM_LINUX
#include <sdio_ops_linux.h>
#endif



#ifdef PLATFORM_WINDOWS

#ifdef PLATFORM_OS_XP
#include <sdio_ops_xp.h>
struct async_context
{
	PMDL pmdl;
	PSDBUS_REQUEST_PACKET sdrp;
	unsigned char* r_buf;
	unsigned char* padapter;
};
#endif
#ifdef PLATFORM_OS_CE
#include <sdio_ops_ce.h>
#endif
#endif



extern void sdio_set_intf_option(u32 *poption);

extern void sdio_set_intf_funs(struct intf_hdl *pintf_hdl);

extern uint sdio_init_intf_priv(struct intf_priv *pintfpriv);

extern void sdio_unload_intf_priv(struct intf_priv *pintfpriv);

extern void sdio_intf_hdl_init(u8 *priv);

extern void sdio_intf_hdl_unload(u8 *priv);

extern void sdio_intf_hdl_open(u8 *priv);

extern void sdio_intf_hdl_close(u8 *priv);

extern void sdio_set_intf_ops(struct _io_ops *pops);
	
//extern void sdio_set_intf_callbacks(struct _io_callbacks *pcallbacks);
extern void sdio_func1cmd52_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *rmem);
extern void sdio_func1cmd52_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *wmem);

extern	uint __inline _cvrt2ftaddr(const u32 addr, u32 *pftaddr)  ;


#endif

