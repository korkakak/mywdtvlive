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
	mlme_ex_def.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Fonchi		2007-06-25		Extend original mlme APIs to support multi-entries
	
*/
#ifndef __MLME_EX_DEF_H__
#define __MLME_EX_DEF_H__


typedef VOID (*STATE_MACHINE_FUNC_EX)(VOID *Adaptor, MLME_QUEUE_ELEM *Elem, PULONG pCurrState, USHORT Idx);

typedef struct _STA_STATE_MACHINE_EX
{
	ULONG					Base;
	ULONG					NrState;
	ULONG					NrMsg;
	ULONG					CurrState;
	STATE_MACHINE_FUNC_EX	*TransFunc;
} STATE_MACHINE_EX, *PSTA_STATE_MACHINE_EX;

#endif // __MLME_EX_DEF_H__ //

