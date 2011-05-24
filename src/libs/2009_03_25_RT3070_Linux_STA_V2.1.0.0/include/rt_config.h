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
	rt_config.h

	Abstract:
	Central header file to maintain all include files for all NDIS
	miniport driver routines.

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Paul Lin    08-01-2002    created

*/
#ifndef	__RT_CONFIG_H__
#define	__RT_CONFIG_H__

#include "rtmp_type.h"
#include "rtmp_os.h"

#include "rtmp_def.h"
#include "rtmp_chip.h"
#include "rtmp_timer.h"

#include "oid.h"
#include "mlme.h"
#include "wpa.h"
#include "crypt_md5.h"
#include "crypt_sha2.h"
#include "crypt_hmac.h"
#include "rtmp.h"
#include "ap.h"
#include "dfs.h"
#include "chlist.h"
#include "spectrum.h"

#ifdef MLME_EX
#include	"mlme_ex_def.h"
#include	"mlme_ex.h"
#endif // MLME_EX //

#include "eeprom.h"
#if defined(RTMP_PCI_SUPPORT) || defined(RTMP_USB_SUPPORT)
#include "rtmp_mcu.h"
#endif



#undef AP_WSC_INCLUDED
#undef STA_WSC_INCLUDED
#undef WSC_INCLUDED



#ifdef CONFIG_STA_SUPPORT
#ifdef WSC_STA_SUPPORT
#define STA_WSC_INCLUDED
#endif // WSC_STA_SUPPORT //
#endif // CONFIG_STA_SUPPORT //

#ifdef BLOCK_NET_IF
#include "netif_block.h"
#endif // BLOCK_NET_IF //

#ifdef IGMP_SNOOP_SUPPORT
#include "igmp_snoop.h"
#endif // IGMP_SNOOP_SUPPORT //

#ifdef RALINK_ATE
#include "rt_ate.h"
#endif // RALINK_ATE //

#ifdef RALINK_28xx_QA
#ifndef RALINK_ATE
#error "For supporting QA GUI, please set HAS_ATE=y and HAS_28xx_QA=y."
#endif // RALINK_ATE //
#endif // RALINK_28xx_QA //




#if defined(AP_WSC_INCLUDED) || defined(STA_WSC_INCLUDED)
#define WSC_INCLUDED
#endif


#ifdef CONFIG_STA_SUPPORT
#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
#ifndef WPA_SUPPLICANT_SUPPORT
#error "Build for being controlled by NetworkManager or wext, please set HAS_WPA_SUPPLICANT=y and HAS_NATIVE_WPA_SUPPLICANT_SUPPORT=y"
#endif // WPA_SUPPLICANT_SUPPORT //
#endif // NATIVE_WPA_SUPPLICANT_SUPPORT //

#endif // CONFIG_STA_SUPPORT //

#ifdef WSC_INCLUDED
// WSC security code
//#define OLD_DH_ALGORITHM //Change to old dh algorithm
//#define OLD_AES_ALGORITHM //Change to old aes algorithm
#include "crypt_biginteger.h"
#include "crypt_dh.h"
#include "crypt_aes.h"
#include "dh_key.h"
#include "evp_enc.h"
#include "wsc.h"
#include "wsc_tlv.h"
#endif // WSC_INCLUDED //


#ifdef IKANOS_VX_1X0
#include "vr_ikans.h"
#endif // IKANOS_VX_1X0 //



#endif	// __RT_CONFIG_H__

