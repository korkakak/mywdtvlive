/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rtusb_data.c

	Abstract:
	Ralink USB driver Tx/Rx functions.

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Jan            03-25-2006    created

*/

#ifdef RTMP_MAC_USB


#include	"rt_config.h"

extern  UCHAR Phy11BGNextRateUpward[]; // defined in mlme.c
extern UCHAR	EpToQueue[];


VOID REPORT_AMSDU_FRAMES_TO_LLC(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pData,
	IN	ULONG			DataSize)
{	
	PNDIS_PACKET	pPacket;
	UINT			nMSDU;
	struct			sk_buff *pSkb;

	nMSDU = 0;
	/* allocate a rx packet */
	pSkb = dev_alloc_skb(RX_BUFFER_AGGRESIZE);
	pPacket = (PNDIS_PACKET)OSPKT_TO_RTPKT(pSkb);
	if (pSkb)
	{

		/* convert 802.11 to 802.3 packet */
		pSkb->dev = get_netdev_from_bssid(pAd, BSS0);		
		RTMP_SET_PACKET_SOURCE(pPacket, PKTSRC_NDIS);
		deaggregate_AMSDU_announce(pAd, pPacket, pData, DataSize);
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR,("Can't allocate skb\n"));
	}
}


/*
	========================================================================

	Routine	Description:
		This subroutine will scan through releative ring descriptor to find
		out avaliable free ring descriptor and compare with request size.
		
	Arguments:
		pAd	Pointer	to our adapter
		RingType	Selected Ring
		
	Return Value:
		NDIS_STATUS_FAILURE		Not enough free descriptor
		NDIS_STATUS_SUCCESS		Enough free descriptor

	Note:
	
	========================================================================
*/
NDIS_STATUS	RTUSBFreeDescriptorRequest(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR			BulkOutPipeId,
	IN	UINT32			NumberRequired)
{
//	UCHAR			FreeNumber = 0;
//	UINT			Index;
	NDIS_STATUS		Status = NDIS_STATUS_FAILURE;
	unsigned long   IrqFlags;
	HT_TX_CONTEXT	*pHTTXContext;


	pHTTXContext = &pAd->TxContext[BulkOutPipeId];
	RTMP_IRQ_LOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags);
	if ((pHTTXContext->CurWritePosition < pHTTXContext->NextBulkOutPosition) && ((pHTTXContext->CurWritePosition + NumberRequired + LOCAL_TXBUF_SIZE) > pHTTXContext->NextBulkOutPosition))
	{
		
		RTUSB_SET_BULK_FLAG(pAd, (fRTUSB_BULK_OUT_DATA_NORMAL << BulkOutPipeId));
	}
	else if ((pHTTXContext->CurWritePosition == 8) && (pHTTXContext->NextBulkOutPosition < (NumberRequired + LOCAL_TXBUF_SIZE)))
	{
		RTUSB_SET_BULK_FLAG(pAd, (fRTUSB_BULK_OUT_DATA_NORMAL << BulkOutPipeId));
	}
	else if (pHTTXContext->bCurWriting == TRUE)
	{
		DBGPRINT(RT_DEBUG_TRACE,("RTUSBFreeD c3 --> QueIdx=%d, CWPos=%ld, NBOutPos=%ld!\n", BulkOutPipeId, pHTTXContext->CurWritePosition, pHTTXContext->NextBulkOutPosition));
		RTUSB_SET_BULK_FLAG(pAd, (fRTUSB_BULK_OUT_DATA_NORMAL << BulkOutPipeId));
	}
	else
	{
		Status = NDIS_STATUS_SUCCESS;
	}
	RTMP_IRQ_UNLOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags);

	
	return (Status);
}


NDIS_STATUS RTUSBFreeDescriptorRelease(
	IN RTMP_ADAPTER *pAd, 
	IN UCHAR		BulkOutPipeId)
{
	unsigned long   IrqFlags;
	HT_TX_CONTEXT	*pHTTXContext;
	
	pHTTXContext = &pAd->TxContext[BulkOutPipeId];
	RTMP_IRQ_LOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags);
	pHTTXContext->bCurWriting = FALSE;
	RTMP_IRQ_UNLOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags);

	return (NDIS_STATUS_SUCCESS);
}


BOOLEAN	RTUSBNeedQueueBackForAgg(
	IN RTMP_ADAPTER *pAd, 
	IN UCHAR		BulkOutPipeId)
{
	unsigned long   IrqFlags;
	HT_TX_CONTEXT	*pHTTXContext;
	BOOLEAN			needQueBack = FALSE;
	
	pHTTXContext = &pAd->TxContext[BulkOutPipeId];
	
	RTMP_IRQ_LOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags);
	if ((pHTTXContext->IRPPending == TRUE)  /*&& (pAd->TxSwQueue[BulkOutPipeId].Number == 0) */)
	{
#if 0
		if ((pHTTXContext->CurWritePosition <= 8) && 
			(pHTTXContext->NextBulkOutPosition > 8 && (pHTTXContext->NextBulkOutPosition+MAX_AGGREGATION_SIZE) < MAX_TXBULK_LIMIT))
		{
			needQueBack = TRUE;
		}
		else if ((pHTTXContext->CurWritePosition < pHTTXContext->NextBulkOutPosition) &&
				 ((pHTTXContext->NextBulkOutPosition + MAX_AGGREGATION_SIZE) < MAX_TXBULK_LIMIT))
		{
			needQueBack = TRUE;
		}
#else
		if ((pHTTXContext->CurWritePosition < pHTTXContext->ENextBulkOutPosition) && 
			(((pHTTXContext->ENextBulkOutPosition+MAX_AGGREGATION_SIZE) < MAX_TXBULK_LIMIT) || (pHTTXContext->CurWritePosition > MAX_AGGREGATION_SIZE)))
		{
			needQueBack = TRUE;
		}
#endif
		else if ((pHTTXContext->CurWritePosition > pHTTXContext->ENextBulkOutPosition) && 
				 ((pHTTXContext->ENextBulkOutPosition + MAX_AGGREGATION_SIZE) < pHTTXContext->CurWritePosition))
		{
			needQueBack = TRUE;
		}
	}
	RTMP_IRQ_UNLOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags);

	return needQueBack;

}


/*
	========================================================================
	
	Routine Description:

	Arguments:

	Return Value:

	IRQL = 
	
	Note:
	
	========================================================================
*/
VOID	RTUSBRejectPendingPackets(
	IN	PRTMP_ADAPTER	pAd)
{
	UCHAR			Index;
	PQUEUE_ENTRY	pEntry;
	PNDIS_PACKET	pPacket;
	PQUEUE_HEADER	pQueue;
	

	for (Index = 0; Index < 4; Index++)
	{
		NdisAcquireSpinLock(&pAd->TxSwQueueLock[Index]);
		while (pAd->TxSwQueue[Index].Head != NULL)
		{
			pQueue = (PQUEUE_HEADER) &(pAd->TxSwQueue[Index]);
			pEntry = RemoveHeadQueue(pQueue);
			pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
		}
		NdisReleaseSpinLock(&pAd->TxSwQueueLock[Index]);

	}

}


/*
	========================================================================
	
	Routine	Description:
		Calculates the duration which is required to transmit out frames 
	with given size and specified rate.
		
	Arguments:
		pTxD		Pointer to transmit descriptor
		Ack			Setting for Ack requirement bit
		Fragment	Setting for Fragment bit
		RetryMode	Setting for retry mode
		Ifs			Setting for IFS gap
		Rate		Setting for transmit rate
		Service		Setting for service
		Length		Frame length
		TxPreamble  Short or Long preamble when using CCK rates
		QueIdx - 0-3, according to 802.11e/d4.4 June/2003
		
	Return Value:
		None
		
	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	========================================================================
*/


VOID RTMPWriteTxInfo(
	IN	PRTMP_ADAPTER	pAd,
	IN	PTXINFO_STRUC 	pTxInfo,
	IN	  USHORT		USBDMApktLen,
	IN	  BOOLEAN		bWiv,
	IN	  UCHAR			QueueSel,
	IN	  UCHAR			NextValid,
	IN	  UCHAR			TxBurst)
{
	pTxInfo->USBDMATxPktLen = USBDMApktLen;
	pTxInfo->QSEL = QueueSel;
	if (QueueSel != FIFO_EDCA)
		DBGPRINT(RT_DEBUG_TRACE, ("====> QueueSel != FIFO_EDCA<============\n"));
	pTxInfo->USBDMANextVLD = FALSE; //NextValid;  // Need to check with Jan about this.
	pTxInfo->USBDMATxburst = TxBurst;
	pTxInfo->WIV = bWiv;
	pTxInfo->SwUseLastRound = 0;
	pTxInfo->rsv = 0;
	pTxInfo->rsv2 = 0;
}

#endif // RTMP_MAC_USB //
