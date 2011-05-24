/****************************************************************************/
/*  Copyright (c) 2007 Atheros Communications Inc. All rights reserved.     */
/*  Http://www.atheros.com/                                                 */
/*                                                                          */
/*  Module Name : cwm.h                                                     */
/*                                                                          */
/*  Abstract                                                                */
/*      This module contains channel width relatived functions.             */
/*                                                                          */
/*  NOTES                                                                   */
/*      None                                                                */
/*                                                                          */
/****************************************************************************/
/*Revision History:                                                         */
/*    Who         When        What                                          */
/*    --------    --------    ----------------------------------------------*/
/*                                                                          */
/*    Honda       3-19-07     created                                       */
/*                                                                          */
/****************************************************************************/

#ifndef _CWM_H
#define _CWM_H

#define ATH_CWM_EXTCH_BUSY_THRESHOLD  30  /* Extension Channel Busy Threshold (0-100%) */

void zfCwmInit(zdev_t* dev);
void zfCoreCwmBusy(zdev_t* dev, u16_t busy);
u16_t zfCwmIsExtChanBusy(u32_t ctlBusy, u32_t extBusy);



#endif /* #ifndef _CWM_H */
