/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    rtmp_type.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Paul Lin    1-2-2004
*/

#ifndef __RTMP_TYPE_H__
#define __RTMP_TYPE_H__

#ifdef VXWORKS
#ifdef VXWORKS_5X
#include <types/vxTypesOld.h>
#endif // VXWORKS_5X //
#include <sys/types.h>
#endif // VXWORKS //

#define PACKED  __attribute__ ((packed))

#ifdef LINUX
// Put platform dependent declaration here
// For example, linux type definition
typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned int			UINT32;
typedef unsigned long long		UINT64;
typedef int					INT32;
typedef long long 				INT64;
#endif // LINUX //

typedef unsigned char *		PUINT8;
typedef unsigned short *		PUINT16;
typedef unsigned int *			PUINT32;
typedef unsigned long long *	PUINT64;
typedef int	*				PINT32;
typedef long long * 			PINT64;

// modified for fixing compile warning on Sigma 8634 platform
typedef char 					STRING;
typedef signed char			CHAR;	

typedef signed short			SHORT;
typedef signed int				INT;
typedef signed long			LONG;
typedef signed long long		LONGLONG;	


#ifdef LINUX
typedef unsigned char			UCHAR;
typedef unsigned short			USHORT;
typedef unsigned int			UINT;
typedef unsigned long			ULONG;
#endif // LINUX //
typedef unsigned long long		ULONGLONG;
 
typedef unsigned char			BOOLEAN;
#ifdef LINUX
typedef void					VOID;
#endif // LINUX //

typedef char *				PSTRING;
typedef VOID *				PVOID;
typedef CHAR *				PCHAR;
typedef UCHAR * 				PUCHAR;
typedef USHORT *			PUSHORT;
typedef LONG *				PLONG;
typedef ULONG *				PULONG;
typedef UINT *				PUINT;

typedef unsigned int			NDIS_MEDIA_STATE;

typedef union _LARGE_INTEGER {
    struct {
        UINT LowPart;
        INT32 HighPart;
    } u;
    INT64 QuadPart;
} LARGE_INTEGER;


//
// Register set pair for initialzation register set definition
//
typedef struct  _RTMP_REG_PAIR
{
	ULONG   Register;
	ULONG   Value;
} RTMP_REG_PAIR, *PRTMP_REG_PAIR;

typedef struct  _REG_PAIR
{
	UCHAR   Register;
	UCHAR   Value;
} REG_PAIR, *PREG_PAIR;

//
// Register set pair for initialzation register set definition
//
typedef struct  _RTMP_RF_REGS
{
	UCHAR   Channel;
	ULONG   R1;
	ULONG   R2;
	ULONG   R3;
	ULONG   R4;
} RTMP_RF_REGS, *PRTMP_RF_REGS;

typedef struct _FREQUENCY_ITEM {
	UCHAR	Channel;
	UCHAR	N;
	UCHAR	R;
	UCHAR	K;
} FREQUENCY_ITEM, *PFREQUENCY_ITEM;


typedef int				NTSTATUS;


#define STATUS_SUCCESS				0x00
#define STATUS_UNSUCCESSFUL 		0x01

#endif  // __RTMP_TYPE_H__ //

