/*

*/


#ifndef __RTUSB_IO_H__
#define __RTUSB_IO_H__

#include "rtmp_type.h"

// New for MeetingHouse Api support
#define CMDTHREAD_VENDOR_RESET                      0x0D730101	// cmd
#define CMDTHREAD_VENDOR_UNPLUG                     0x0D730102	// cmd
#define CMDTHREAD_VENDOR_SWITCH_FUNCTION            0x0D730103	// cmd
#define CMDTHREAD_MULTI_WRITE_MAC                   0x0D730107	// cmd
#define CMDTHREAD_MULTI_READ_MAC                    0x0D730108	// cmd
#define CMDTHREAD_VENDOR_EEPROM_WRITE               0x0D73010A	// cmd
#define CMDTHREAD_VENDOR_EEPROM_READ                0x0D73010B	// cmd
#define CMDTHREAD_VENDOR_ENTER_TESTMODE             0x0D73010C	// cmd
#define CMDTHREAD_VENDOR_EXIT_TESTMODE              0x0D73010D	// cmd
#define CMDTHREAD_VENDOR_WRITE_BBP                  0x0D730119	// cmd
#define CMDTHREAD_VENDOR_READ_BBP                   0x0D730118	// cmd
#define CMDTHREAD_VENDOR_WRITE_RF                   0x0D73011A	// cmd
#define CMDTHREAD_VENDOR_FLIP_IQ                    0x0D73011D	// cmd
#define CMDTHREAD_RESET_BULK_OUT                    0x0D730210	// cmd
#define CMDTHREAD_RESET_BULK_IN                     0x0D730211	// cmd
#define CMDTHREAD_SET_PSM_BIT                  		0x0D730212	// cmd
#define CMDTHREAD_SET_RADIO                         0x0D730214	// cmd
#define CMDTHREAD_UPDATE_TX_RATE                    0x0D730216	// cmd
#define CMDTHREAD_802_11_ADD_KEY_WEP                0x0D730218	// cmd
#define CMDTHREAD_RESET_FROM_ERROR                  0x0D73021A	// cmd
#define CMDTHREAD_LINK_DOWN                         0x0D73021B	// cmd
#define CMDTHREAD_RESET_FROM_NDIS                   0x0D73021C	// cmd
#define CMDTHREAD_CHECK_GPIO                        0x0D730215	// cmd
#define CMDTHREAD_FORCE_WAKE_UP                     0x0D730222	// cmd
#define CMDTHREAD_SET_BW                            0x0D730225	// cmd
#define CMDTHREAD_SET_ASIC_WCID                     0x0D730226	// cmd
#define CMDTHREAD_SET_ASIC_WCID_CIPHER              0x0D730227	// cmd
#define CMDTHREAD_QKERIODIC_EXECUT                  0x0D73023D	// cmd
#define RT_CMD_SET_KEY_TABLE                        0x0D730228  // cmd
#define RT_CMD_SET_RX_WCID_TABLE                    0x0D730229  // cmd
#define CMDTHREAD_SET_CLIENT_MAC_ENTRY              0x0D73023E	// cmd
#define CMDTHREAD_SET_GROUP_KEY						0x0D73023F	// cmd
#define CMDTHREAD_SET_PAIRWISE_KEY					0x0D730240	// cmd

#define CMDTHREAD_802_11_QUERY_HARDWARE_REGISTER    0x0D710105	// cmd
#define CMDTHREAD_802_11_SET_PHY_MODE               0x0D79010C	// cmd
#define CMDTHREAD_802_11_SET_STA_CONFIG             0x0D790111	// cmd
#define CMDTHREAD_802_11_SET_PREAMBLE               0x0D790101	// cmd
#define CMDTHREAD_802_11_COUNTER_MEASURE			0x0D790102	// cmd
// add by johnli, fix "in_interrupt" error when call "MacTableDeleteEntry" in Rx tasklet
#define CMDTHREAD_UPDATE_PROTECT					0x0D790103	// cmd
// end johnli


//CMDTHREAD_MULTI_READ_MAC
//CMDTHREAD_MULTI_WRITE_MAC
//CMDTHREAD_VENDOR_EEPROM_READ
//CMDTHREAD_VENDOR_EEPROM_WRITE
typedef	struct	_CMDHandler_TLV	{
	USHORT		Offset;
	USHORT		Length;
	UCHAR		DataFirst;
}	CMDHandler_TLV, *PCMDHandler_TLV;


typedef	struct _CmdQElmt	{
	UINT				command;
	PVOID				buffer;
	ULONG				bufferlength;
	BOOLEAN				CmdFromNdis;
	BOOLEAN				SetOperation;
	struct _CmdQElmt	*next;
}	CmdQElmt, *PCmdQElmt;

typedef	struct	_CmdQ	{
	UINT		size;
	CmdQElmt	*head;
	CmdQElmt	*tail;
	UINT32		CmdQState;
}CmdQ, *PCmdQ;


#define EnqueueCmd(cmdq, cmdqelmt)		\
{										\
	if (cmdq->size == 0)				\
		cmdq->head = cmdqelmt;			\
	else								\
		cmdq->tail->next = cmdqelmt;	\
	cmdq->tail = cmdqelmt;				\
	cmdqelmt->next = NULL;				\
	cmdq->size++;						\
}


/******************************************************************************

  	USB Cmd to ASIC Related MACRO
  	
******************************************************************************/
// reset MAC of a station entry to 0xFFFFFFFFFFFF
#define RTMP_STA_ENTRY_MAC_RESET(pAd, Wcid)					\
	{	RT_SET_ASIC_WCID	SetAsicWcid;						\
		SetAsicWcid.WCID = Wcid;								\
		SetAsicWcid.SetTid = 0xffffffff;						\
		SetAsicWcid.DeleteTid = 0xffffffff;						\
		RTUSBEnqueueInternalCmd(pAd, CMDTHREAD_SET_ASIC_WCID, 	\
				&SetAsicWcid, sizeof(RT_SET_ASIC_WCID));	}

// add this entry into ASIC RX WCID search table
#define RTMP_STA_ENTRY_ADD(pAd, pEntry)							\
	RTUSBEnqueueInternalCmd(pAd, CMDTHREAD_SET_CLIENT_MAC_ENTRY, 	\
							pEntry, sizeof(MAC_TABLE_ENTRY));

// add by johnli, fix "in_interrupt" error when call "MacTableDeleteEntry" in Rx tasklet
// Set MAC register value according operation mode
#define RTMP_UPDATE_PROTECT(pAd)	\
 	RTUSBEnqueueInternalCmd(pAd, CMDTHREAD_UPDATE_PROTECT, NULL, 0);
// end johnli

// remove Pair-wise key material from ASIC
// yet implement
#define RTMP_STA_ENTRY_KEY_DEL(pAd, BssIdx, Wcid)

// add Client security information into ASIC WCID table and IVEIV table
#define RTMP_STA_SECURITY_INFO_ADD(pAd, apidx, KeyID, pEntry)						\
	{	RTMP_STA_ENTRY_MAC_RESET(pAd, pEntry->Aid);								\
		if (pEntry->Aid >= 1) {														\
			RT_SET_ASIC_WCID_ATTRI	SetAsicWcidAttri;								\
			SetAsicWcidAttri.WCID = pEntry->Aid;									\
			if ((pEntry->AuthMode <= Ndis802_11AuthModeAutoSwitch) &&				\
				(pEntry->WepStatus == Ndis802_11Encryption1Enabled))				\
			{																		\
				SetAsicWcidAttri.Cipher = pAd->SharedKey[apidx][KeyID].CipherAlg;	\
			}																		\
			else if (pEntry->AuthMode == Ndis802_11AuthModeWPANone)					\
			{																		\
				SetAsicWcidAttri.Cipher = pAd->SharedKey[apidx][KeyID].CipherAlg;	\
			}																		\
			else SetAsicWcidAttri.Cipher = 0;										\
            DBGPRINT(RT_DEBUG_TRACE, ("aid cipher = %ld\n",SetAsicWcidAttri.Cipher));       \
			RTUSBEnqueueInternalCmd(pAd, CMDTHREAD_SET_ASIC_WCID_CIPHER, 			\
							&SetAsicWcidAttri, sizeof(RT_SET_ASIC_WCID_ATTRI)); } }

// Insert the BA bitmap to ASIC for the Wcid entry
#define RTMP_ADD_BA_SESSION_TO_ASIC(_pAd, _Aid, _TID)					\
		do{																\
			RT_SET_ASIC_WCID	SetAsicWcid;							\
			SetAsicWcid.WCID = (_Aid);									\
			SetAsicWcid.SetTid = (0x10000<<(_TID));						\
			SetAsicWcid.DeleteTid = 0xffffffff;							\
			RTUSBEnqueueInternalCmd((_pAd), CMDTHREAD_SET_ASIC_WCID, &SetAsicWcid, sizeof(RT_SET_ASIC_WCID));	\
		}while(0)

// Remove the BA bitmap from ASIC for the Wcid entry
#define RTMP_DEL_BA_SESSION_FROM_ASIC(_pAd, _Wcid, _TID)				\
		do{																\
			RT_SET_ASIC_WCID	SetAsicWcid;							\
			SetAsicWcid.WCID = (_Wcid);									\
			SetAsicWcid.SetTid = (0xffffffff);							\
			SetAsicWcid.DeleteTid = (0x10000<<(_TID) );					\
			RTUSBEnqueueInternalCmd((_pAd), CMDTHREAD_SET_ASIC_WCID, &SetAsicWcid, sizeof(RT_SET_ASIC_WCID));	\
		}while(0)


#endif // __RTUSB_IO_H__ //
