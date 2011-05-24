/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2007, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  * 
 * it under the terms of the GNU General Public License as published by  * 
 * the Free Software Foundation; either version 2 of the License, or     * 
 * (at your option) any later version.                                   * 
 *                                                                       * 
 * This program is distributed in the hope that it will be useful,       * 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         * 
 * GNU General Public License for more details.                          * 
 *                                                                       * 
 * You should have received a copy of the GNU General Public License     * 
 * along with this program; if not, write to the                         * 
 * Free Software Foundation, Inc.,                                       * 
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             * 
 *                                                                       * 
 *************************************************************************
*/

#ifndef __RTMP_USB_H__
#define __RTMP_USB_H__


#include "rtusb_io.h"


#ifdef LINUX
#include <linux/usb.h>

typedef struct usb_device	* PUSB_DEV;
typedef struct urb *purbb_t;
typedef struct usb_ctrlrequest devctrlrequest;
#endif // LINUX //

extern UCHAR EpToQueue[6];


#define RXBULKAGGRE_ZISE			12
#define MAX_TXBULK_LIMIT			(LOCAL_TXBUF_SIZE*(BULKAGGRE_ZISE-1))
#define MAX_TXBULK_SIZE			(LOCAL_TXBUF_SIZE*BULKAGGRE_ZISE)
#define MAX_RXBULK_SIZE			(LOCAL_TXBUF_SIZE*RXBULKAGGRE_ZISE)
#define MAX_MLME_HANDLER_MEMORY 20


// Flags for Bulkflags control for bulk out data
//
#define	fRTUSB_BULK_OUT_DATA_NULL				0x00000001
#define	fRTUSB_BULK_OUT_RTS						0x00000002
#define	fRTUSB_BULK_OUT_MLME						0x00000004

#define	fRTUSB_BULK_OUT_PSPOLL					0x00000010
#define	fRTUSB_BULK_OUT_DATA_FRAG				0x00000020
#define	fRTUSB_BULK_OUT_DATA_FRAG_2				0x00000040
#define	fRTUSB_BULK_OUT_DATA_FRAG_3				0x00000080
#define	fRTUSB_BULK_OUT_DATA_FRAG_4				0x00000100

#define	fRTUSB_BULK_OUT_DATA_NORMAL				0x00010000
#define	fRTUSB_BULK_OUT_DATA_NORMAL_2			0x00020000
#define	fRTUSB_BULK_OUT_DATA_NORMAL_3			0x00040000
#define	fRTUSB_BULK_OUT_DATA_NORMAL_4			0x00080000

// TODO:move to ./ate/include/iface/ate_usb.h
#ifdef RALINK_ATE
#define	fRTUSB_BULK_OUT_DATA_ATE				0x00100000
#endif // RALINK_ATE //


#define FREE_HTTX_RING(_pCookie, _pipeId, _txContext)			\
{										\
	if ((_txContext)->ENextBulkOutPosition == (_txContext)->CurWritePosition)	\
	{																	\
		(_txContext)->bRingEmpty = TRUE;			\
	}																	\
	/*NdisInterlockedDecrement(&(_p)->TxCount); */\
}



/******************************************************************************

  	USB Bulk operation related definitions

******************************************************************************/

#ifdef LINUX
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#define BULKAGGRE_ZISE          100
#define RT28XX_PUT_DEVICE							usb_put_dev
#define RTUSB_ALLOC_URB(iso)							usb_alloc_urb(iso, GFP_ATOMIC)
#define RTUSB_SUBMIT_URB(pUrb)							usb_submit_urb(pUrb, GFP_ATOMIC)
#define RTUSB_URB_ALLOC_BUFFER(pUsb_Dev, BufSize, pDma_addr)			usb_buffer_alloc(pUsb_Dev, BufSize, GFP_ATOMIC, pDma_addr)
#define RTUSB_URB_FREE_BUFFER(pUsb_Dev, BufSize, pTransferBuf, Dma_addr)	usb_buffer_free(pUsb_Dev, BufSize, pTransferBuf, Dma_addr)
#else
#define BULKAGGRE_ZISE          60
#define RT28XX_PUT_DEVICE(dev_p)
#define RTUSB_ALLOC_URB(iso)                                               usb_alloc_urb(iso)
#define RTUSB_SUBMIT_URB(pUrb)                                             usb_submit_urb(pUrb)
#define RTUSB_URB_ALLOC_BUFFER(pUsb_Dev, BufSize, pDma_addr)               kmalloc(BufSize, GFP_ATOMIC)
#define RTUSB_URB_FREE_BUFFER(pUsb_Dev, BufSize, pTransferBuf, Dma_addr)   kfree(pTransferBuf)
#endif

#define RTUSB_FREE_URB(pUrb)	usb_free_urb(pUrb)

// unlink urb
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,7)
#define RTUSB_UNLINK_URB(pUrb)		usb_kill_urb(pUrb)
#else
#define RTUSB_UNLINK_URB(pUrb)		usb_unlink_urb(pUrb)
#endif

// Prototypes of completion funuc.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
#define RTUSBBulkOutDataPacketComplete(purb, pt_regs)    RTUSBBulkOutDataPacketComplete(purb)
#define RTUSBBulkOutMLMEPacketComplete(pUrb, pt_regs)    RTUSBBulkOutMLMEPacketComplete(pUrb)
#define RTUSBBulkOutNullFrameComplete(pUrb, pt_regs)     RTUSBBulkOutNullFrameComplete(pUrb)
#define RTUSBBulkOutRTSFrameComplete(pUrb, pt_regs)      RTUSBBulkOutRTSFrameComplete(pUrb)
#define RTUSBBulkOutPsPollComplete(pUrb, pt_regs)        RTUSBBulkOutPsPollComplete(pUrb)
#define RTUSBBulkRxComplete(pUrb, pt_regs)               RTUSBBulkRxComplete(pUrb)
#endif // end of "#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)" //

extern void dump_urb(struct urb* purb);

#define InterlockedIncrement 	 	atomic_inc
#define NdisInterlockedIncrement 	atomic_inc
#define InterlockedDecrement		atomic_dec
#define NdisInterlockedDecrement 	atomic_dec
#define InterlockedExchange		atomic_set

#endif // LINUX //



#define NT_SUCCESS(status)			(((status) >=0) ? (TRUE):(FALSE))



#define USBD_TRANSFER_DIRECTION_OUT		0
#define USBD_TRANSFER_DIRECTION_IN		0
#define USBD_SHORT_TRANSFER_OK			0
#define PURB			purbb_t

#define PIRP		PVOID
#define NDIS_OID	UINT
#ifndef USB_ST_NOERROR
#define USB_ST_NOERROR     0
#endif

// vendor-specific control operations
#define CONTROL_TIMEOUT_JIFFIES ( (100 * OS_HZ) / 1000)
#define UNLINK_TIMEOUT_MS		3


VOID RTUSBBulkOutDataPacketComplete(purbb_t purb, struct pt_regs *pt_regs);
VOID RTUSBBulkOutMLMEPacketComplete(purbb_t pUrb, struct pt_regs *pt_regs);
VOID RTUSBBulkOutNullFrameComplete(purbb_t pUrb, struct pt_regs *pt_regs);
VOID RTUSBBulkOutRTSFrameComplete(purbb_t pUrb, struct pt_regs *pt_regs);
VOID RTUSBBulkOutPsPollComplete(purbb_t pUrb, struct pt_regs *pt_regs);
VOID RTUSBBulkRxComplete(purbb_t pUrb, struct pt_regs *pt_regs);


#ifdef KTHREAD_SUPPORT
#define RTUSBMlmeUp(pAd) \
	do{								    \
		RTMP_OS_TASK	*_pTask = &((pAd)->mlmeTask);\
		if (_pTask->kthread_task) \
        { \
			_pTask->kthread_running = TRUE; \
	        wake_up(&_pTask->kthread_q); \
		} \
	}while(0)
#else
#define RTUSBMlmeUp(pAd)	        \
	do{								    \
		RTMP_OS_TASK	*_pTask = &((pAd)->mlmeTask);\
		CHECK_PID_LEGALITY(_pTask->taskPID)		    \
		{ \
			RTMP_SEM_EVENT_UP(&(_pTask->taskSema)); \
		}\
	}while(0)
#endif

#ifdef KTHREAD_SUPPORT
#define RTUSBCMDUp(pAd) \
	do{	\
		RTMP_OS_TASK	*_pTask = &((pAd)->cmdQTask);	\
		{ \
			_pTask->kthread_running = TRUE; \
	        wake_up(&_pTask->kthread_q); \
		} \
	}while(0)

#else
#define RTUSBCMDUp(pAd)	                \
	do{									    \
		RTMP_OS_TASK	*_pTask = &((pAd)->cmdQTask);	\
		CHECK_PID_LEGALITY(_pTask->taskPID)	    \
		{\
			RTMP_SEM_EVENT_UP(&(_pTask->taskSema)); \
		}\
	}while(0)
#endif

#define DEVICE_VENDOR_REQUEST_OUT       0x40
#define DEVICE_VENDOR_REQUEST_IN        0xc0
//#define INTERFACE_VENDOR_REQUEST_OUT    0x41
//#define INTERFACE_VENDOR_REQUEST_IN     0xc1

#define BULKOUT_MGMT_RESET_FLAG		0x80

#define RTUSB_SET_BULK_FLAG(_M, _F)	((_M)->BulkFlags |= (_F))
#define RTUSB_CLEAR_BULK_FLAG(_M, _F)	((_M)->BulkFlags &= ~(_F))
#define RTUSB_TEST_BULK_FLAG(_M, _F)	(((_M)->BulkFlags & (_F)) != 0)

#define RTMP_IRQ_REQUEST(net_dev)		do{}while(0)
#define RTMP_IRQ_RELEASE(net_dev)		do{}while(0)


#endif // __RTMP_USB_H__ //
