/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	auth_rsp.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John		2004-10-1		copy from RT2560
*/
#include "rt_config.h"

/*
    ==========================================================================
    Description:
        authentication state machine init procedure
    Parameters:
        Sm - the state machine
        
	IRQL = PASSIVE_LEVEL

    ==========================================================================
 */
VOID AuthRspStateMachineInit(
    IN PRTMP_ADAPTER pAd, 
    IN PSTATE_MACHINE Sm, 
    IN STATE_MACHINE_FUNC Trans[]) 
{
    StateMachineInit(Sm, Trans, MAX_AUTH_RSP_STATE, MAX_AUTH_RSP_MSG, (STATE_MACHINE_FUNC)Drop, AUTH_RSP_IDLE, AUTH_RSP_MACHINE_BASE);

    // column 1
    StateMachineSetAction(Sm, AUTH_RSP_IDLE, MT2_PEER_DEAUTH, (STATE_MACHINE_FUNC)PeerDeauthAction);

    // column 2
    StateMachineSetAction(Sm, AUTH_RSP_WAIT_CHAL, MT2_PEER_DEAUTH, (STATE_MACHINE_FUNC)PeerDeauthAction);

}

/*
    ==========================================================================
    Description:
        
	IRQL = DISPATCH_LEVEL

    ==========================================================================
*/
VOID PeerAuthSimpleRspGenAndSend(
    IN PRTMP_ADAPTER pAd, 
    IN PHEADER_802_11 pHdr80211, 
    IN USHORT Alg, 
    IN USHORT Seq, 
    IN USHORT Reason, 
    IN USHORT Status) 
{
    HEADER_802_11     AuthHdr;
    ULONG             FrameLen = 0;
    PUCHAR            pOutBuffer = NULL;
    NDIS_STATUS       NStatus;

    if (Reason != MLME_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Peer AUTH fail...\n"));
        return;
    }
    
	//Get an unused nonpaged memory
    NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  
    if (NStatus != NDIS_STATUS_SUCCESS) 
        return;

    DBGPRINT(RT_DEBUG_TRACE, ("Send AUTH response (seq#2)...\n"));
    MgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, pHdr80211->Addr2, pAd->MlmeAux.Bssid);
    MakeOutgoingFrame(pOutBuffer,               &FrameLen, 
                      sizeof(HEADER_802_11),    &AuthHdr, 
                      2,                        &Alg, 
                      2,                        &Seq, 
                      2,                        &Reason, 
                      END_OF_ARGS);
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);
}

/*
    ==========================================================================
    Description:
        
	IRQL = DISPATCH_LEVEL

    ==========================================================================
*/
VOID PeerDeauthAction(
    IN PRTMP_ADAPTER pAd, 
    IN PMLME_QUEUE_ELEM Elem) 
{
    UCHAR       Addr2[MAC_ADDR_LEN];
    USHORT      Reason;

    if (PeerDeauthSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &Reason)) 
    {
        if (INFRA_ON(pAd)
			&& MAC_ADDR_EQUAL(Addr2, pAd->CommonCfg.Bssid)
			)
        {
            DBGPRINT(RT_DEBUG_TRACE,("AUTH_RSP - receive DE-AUTH from our AP (Reason=%d)\n", Reason));


#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
		RtmpOSWrielessEventSend(pAd, SIOCGIWAP, -1, NULL, NULL, 0);
#endif // NATIVE_WPA_SUPPLICANT_SUPPORT //        
            

			// send wireless event - for deauthentication
			if (pAd->CommonCfg.bWirelessEvent)
				RTMPSendWirelessEvent(pAd, IW_DEAUTH_EVENT_FLAG, pAd->MacTab.Content[BSSID_WCID].Addr, BSS0, 0); 

            LinkDown(pAd, TRUE);
        }
    }
    else
    {
        DBGPRINT(RT_DEBUG_TRACE,("AUTH_RSP - PeerDeauthAction() sanity check fail\n"));
    }
}

