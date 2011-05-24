/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	wapi.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Albert		2008-4-3      	Supoort WAPI protocol
*/

#ifndef __WAPI_H__
#define __WAPI_H__

#define LENGTH_WAI_H	12
#define LEN_TX_IV		16
#define LEN_WPI_MIC		16
#define LEN_WPI_IV_HDR	18

// trigger message from driver
#define WAI_MLME_CERT_AUTH_START	1
#define WAI_MLME_KEY_HS_START		2
#define WAI_MLME_UPDATE_BK			3
#define WAI_MLME_UPDATE_USK			4
#define WAI_MLME_UPDATE_MSK			5
#define WAI_MLME_DISCONNECT			0xff

#define WAPI_KEY_UPDATE_EXEC_INTV   1000				// 1 sec

// WAPI rekey method
#define REKEY_METHOD_DISABLE        0
#define REKEY_METHOD_TIME           1
#define REKEY_METHOD_PKT            2
//#define REKEY_METHOD_TIME_PKT     3

extern UCHAR AE_BCAST_PN[LEN_TX_IV];
extern UCHAR ASUE_UCAST_PN[LEN_TX_IV];
extern UCHAR AE_UCAST_PN[LEN_TX_IV];

// WAPI authentication mode
typedef enum _WAPI_AUTH_MODE
{
   WAPI_AUTH_DISABLE,
   WAPI_AUTH_PSK,
   WAPI_AUTH_CERT,
} WAPI_AUTH_MODE, *PWAPI_AUTH_MODE;

// WAPI authentication mode
typedef enum _KEY_TYPE_MODE
{
   HEX_MODE,
   ASCII_MODE
} KEY_TYPE_MODE, *PKEY_TYPE_MODE;


// Increase TxIV value for next transmission
// Todo - When overflow occurred, do re-key mechanism
#define	INC_TX_IV(_V, NUM)					\
{											\
	UCHAR	cnt = LEN_TX_IV;				\
	do										\
	{										\
		cnt--;								\
		_V[cnt] = _V[cnt] + NUM;			\
		if (cnt == 0)						\
		{									\
			DBGPRINT(RT_DEBUG_TRACE, ("PN overflow!!!!\n"));	\
			break;							\
		}									\
	}while (_V[cnt] == 0);					\
}

#define IS_WAPI_CAPABILITY(a)       (((a) >= Ndis802_11AuthModeWAICERT) && ((a) <= Ndis802_11AuthModeWAIPSK))

// the defintion of WAI header
typedef	struct PACKED _HEADER_WAI	{    
    USHORT          version;
	UCHAR			type;
	UCHAR			sub_type;
	USHORT			reserved;
	USHORT			length;
	USHORT			pkt_seq;
	UCHAR			frag_seq;
	UCHAR			flag;
}	HEADER_WAI, *PHEADER_WAI;

// For WAPI
typedef struct PACKED _WAPIIE {
    USHORT  version;    
    USHORT  acount;
    struct PACKED {
        UCHAR oui[4];
    }auth[1];
} WAPIIE, *PWAPIIE;

// unicast key suite
typedef struct PACKED _WAPIIE_UCAST {
    USHORT ucount;
    struct PACKED {
        UCHAR oui[4];
    }ucast[1];
} WAPIIE_UCAST,*PWAPIIE_UCAST;

// multi-cast key suite and capability
typedef struct PACKED _WAPIIE_MCAST {
    UCHAR   mcast[4];
    USHORT  capability;
} WAPIIE_MCAST,*PWAPIIE_MCAST;

// the relative to wapi daemon
typedef struct PACKED _COMMON_WAPI_INFO
{	
	UINT8			wapi_ifname[IFNAMSIZ];		// wai negotiation
	UINT8			wapi_ifname_len;			
	UINT8 			preauth_ifname[IFNAMSIZ];	// pre-authentication
	UINT8			preauth_ifname_len;
	UINT8			as_cert_path[128];			// the path of as certification
	UINT8			as_cert_path_len;
	UINT8			user_cert_path[128];		// the path of local user certification 
	UINT8			user_cert_path_len;		
	UINT32			wapi_as_ip;					// the ip address of authentication server
	UINT32			wapi_as_port;				// the port of authentication server
} COMMON_WAPI_INFO, *PCOMMON_WAPI_INFO;

typedef struct PACKED _MBSS_WAPI_INFO
{	
	UINT8			ifname[IFNAMSIZ];
	UINT8			ifname_len;
	UINT8			auth_mode;	
    UINT8       	psk[64];
	UINT8			psk_len;	
	UINT8			wie[128];
	UINT8			wie_len;
} MBSS_WAPI_INFO, *PMBSS_WAPI_INFO;

// It's used by wapi daemon to require relative configuration
typedef struct PACKED _WAPI_CONF
{
    UINT8				mbss_num;					// indicate multiple BSS number 
	COMMON_WAPI_INFO	comm_wapi_info;		
	MBSS_WAPI_INFO		mbss_wapi_info[MAX_MBSSID_NUM];
} WAPI_CONF, *PWAPI_CONF;

INT SMS4_TEST(void);

#endif // __WAPI_H__ //
