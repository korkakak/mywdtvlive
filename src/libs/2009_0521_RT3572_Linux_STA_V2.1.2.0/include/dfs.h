
/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    dfs.h

    Abstract:
    Support DFS function.

    Revision History:
    Who       When            What
    --------  ----------      ----------------------------------------------
    Fonchi    03-12-2007      created
*/

#define RADAR_PULSE 1
#define RADAR_WIDTH 2

#define WIDTH_RD_IDLE 0
#define WIDTH_RD_CHECK 1



/*************************************************************************
  *
  *	DFS Radar related definitions.
  *
  ************************************************************************/  
//#define CARRIER_DETECT_TASK_NUM	6
//#define RADAR_DETECT_TASK_NUM	7

// McuRadarState && McuCarrierState for 2880-SW-MCU
#define FREE_FOR_TX				0
#define WAIT_CTS_BEING_SENT		1
#define DO_DETECTION			2

// McuRadarEvent
#define RADAR_EVENT_CTS_SENT			0x01 // Host signal MCU that CTS has been sent
#define RADAR_EVENT_CTS_CARRIER_SENT	0x02 // Host signal MCU that CTS has been sent (Carrier)
#define RADAR_EVENT_RADAR_DETECTING		0x04 // Radar detection is on going, carrier detection hold back
#define RADAR_EVENT_CARRIER_DETECTING	0x08 // Carrier detection is on going, radar detection hold back
#define RADAR_EVENT_WIDTH_RADAR			0x10 // BBP == 2 radar detected
#define RADAR_EVENT_CTS_KICKED			0x20 // Radar detection need to sent double CTS, first CTS sent

// McuRadarCmd
#define DETECTION_STOP			0
#define RADAR_DETECTION			1
#define CARRIER_DETECTION		2



#ifdef RT305x
INT Set_CarrierCriteria_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
int Set_CarrierReCheck_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
INT Set_CarrierStopCheck_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
void NewCarrierDetectionStart(PRTMP_ADAPTER pAd);

#ifdef CARRIER_DETECTION_SUPPORT
void RTMPHandleRadarInterrupt(PRTMP_ADAPTER  pAd);
#endif // CARRIER_DETECTION_SUPPORT //

#endif // RT305x //


VOID BbpRadarDetectionStart(
	IN PRTMP_ADAPTER pAd);

VOID BbpRadarDetectionStop(
	IN PRTMP_ADAPTER pAd);

VOID RadarDetectionStart(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN CTS_Protect,
	IN UINT8 CTSPeriod);

VOID RadarDetectionStop(
	IN PRTMP_ADAPTER	pAd);

VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd);
	

BOOLEAN RadarChannelCheck(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			Ch);

ULONG JapRadarType(
	IN PRTMP_ADAPTER pAd);

ULONG RTMPBbpReadRadarDuration(
	IN PRTMP_ADAPTER	pAd);

ULONG RTMPReadRadarDuration(
	IN PRTMP_ADAPTER	pAd);

VOID RTMPCleanRadarDuration(
	IN PRTMP_ADAPTER	pAd);

VOID RTMPPrepareRDCTSFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pDA,
	IN	ULONG			Duration,
	IN  UCHAR           RTSRate,
	IN  ULONG           CTSBaseAddr,
	IN  UCHAR			FrameGap);

VOID RTMPPrepareRadarDetectParams(
	IN PRTMP_ADAPTER	pAd);


INT Set_ChMovingTime_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PSTRING arg);

INT Set_LongPulseRadarTh_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PSTRING arg);


