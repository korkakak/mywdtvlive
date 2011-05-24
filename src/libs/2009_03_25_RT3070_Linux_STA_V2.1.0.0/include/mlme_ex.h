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
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mlme_ex.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Fonchi		2007-06-25		Extend original mlme APIs to support multi-entries
	
*/
#ifndef __MLME_EX_H__
#define __MLME_EX_H__

#include "mlme_ex_def.h"


VOID StateMachineInitEx(
	IN STATE_MACHINE_EX *S, 
	IN STATE_MACHINE_FUNC_EX Trans[], 
	IN ULONG StNr,
	IN ULONG MsgNr,
	IN STATE_MACHINE_FUNC_EX DefFunc, 
	IN ULONG InitState, 
	IN ULONG Base);

VOID StateMachineSetActionEx(
	IN STATE_MACHINE_EX *S, 
	IN ULONG St, 
	IN ULONG Msg, 
	IN STATE_MACHINE_FUNC_EX Func);

BOOLEAN isValidApCliIf(
	SHORT Idx);

VOID StateMachinePerformActionEx(
	IN PRTMP_ADAPTER pAd, 
	IN STATE_MACHINE_EX *S, 
	IN MLME_QUEUE_ELEM *Elem,
	USHORT Idx,
	PULONG pCurrState);

BOOLEAN MlmeEnqueueEx(
	IN	PRTMP_ADAPTER pAd,
	IN ULONG Machine, 
	IN ULONG MsgType, 
	IN ULONG MsgLen, 
	IN VOID *Msg,
	IN USHORT Idx);

VOID DropEx(
    IN PRTMP_ADAPTER pAd,
    IN MLME_QUEUE_ELEM *Elem,
	PULONG pCurrState,
	USHORT Idx);

#endif /* __MLME_EX_H__ */

