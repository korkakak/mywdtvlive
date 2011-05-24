/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Module Name:
	rt3070.h
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */

#ifndef __RT35XX_H__
#define __RT35XX_H__

#ifdef RT35xx

#ifdef RTMP_USB_SUPPORT
#include "chip/mac_usb.h"
#endif


#ifndef RTMP_RF_RW_SUPPORT
#error "For RT3062/3562/3572/3592, you should define the compile flag -DRTMP_RF_RW_SUPPORT"
#endif

#include "chip/rt30xx.h"

extern REG_PAIR   RF3572_RegTable[];

//
// Device ID & Vendor ID, these values should match EEPROM value
//
#define NIC3062_PCI_DEVICE_ID	0x3062		// 2T/2R miniCard
#define NIC3562_PCI_DEVICE_ID	0x3562		// 2T/2R miniCard

#endif // RT35xx //

#endif //__RT35XX_H__ //

