#ifndef __ATE_H__
#define __ATE_H__

#ifdef VXWORKS
#define KERN_EMERG
#define ate_print printf
#define ATEDBGPRINT DBGPRINT
#ifdef RTMP_MAC_USB
#define EEPROM_SIZE								0x400
#ifdef CONFIG_STA_SUPPORT
#define EEPROM_BIN_FILE_NAME  "/tgtsvr/usb/e2p_sta.bin"
#endif // CONFIG_STA_SUPPORT //
#endif // RTMP_MAC_USB //
#endif // VXWORKS //

#ifdef LINUX
#define ate_print printk
#define ATEDBGPRINT DBGPRINT
#ifdef RTMP_MAC_USB
#define EEPROM_SIZE								0x400
#ifdef CONFIG_STA_SUPPORT
#define EEPROM_BIN_FILE_NAME  "/etc/Wireless/RT2870STA/e2p.bin"
#endif // CONFIG_STA_SUPPORT //
#endif // RTMP_MAC_USB //
#endif // LINUX //

#ifdef UCOS
/* RT2880_iNIC and RT288x router should define "RTMP_MAC_PCI" !!! */
/* iNIC has its own EEPROM_BIN_FILE_NAME */
#define fATE_LOAD_EEPROM						0x0C43
#ifdef CONFIG_PRINTK
extern INT ConsoleResponse(IN PUCHAR buff);
extern int (*remote_display)(char *);

/* specificly defined to redirect and show ate-related messages to host. */
/* Try to define ate_print as a macro. */
#define ate_print(fmt, args...)                 \
do{   int (*org_remote_display)(char *) = NULL;   \
	org_remote_display = remote_display;\
	/* Save original "remote_display" */\
	remote_display = (int (*)(char *))ConsoleResponse;           \
	printk(fmt, ## args);                       \
	/* Restore the remote_display function pointer */        \
	remote_display = org_remote_display; }while(0)

#define ATEDBGPRINT(Level, Fmt)    	\
{                                   \
    if ((Level) <= RTDebugLevel)      \
    {                               \
        ate_print Fmt;					\
    }                               \
}
#endif // CONFIG_PRINTK //
#endif // UCOS //

#define ATE_ON(_p)              (((_p)->ate.Mode) != ATE_STOP)


#ifdef RTMP_MAC_USB

#define ATE_BBP_IO_READ8_BY_REG_ID(_A, _I, _pV)    RTMP_BBP_IO_READ8_BY_REG_ID(_A, _I, _pV)
#ifndef RT35xx
#define ATE_BBP_IO_WRITE8_BY_REG_ID(_A, _I, _V)    RTMP_BBP_IO_WRITE8_BY_REG_ID(_A, _I, _V)
#endif // !RT35xx //

#define BULK_OUT_LOCK(pLock, IrqFlags)	\
		if(1 /*!(in_interrupt() & 0xffff0000)*/)	\
			RTMP_IRQ_LOCK((pLock), IrqFlags);
		
#define BULK_OUT_UNLOCK(pLock, IrqFlags)	\
		if(1 /*!(in_interrupt() & 0xffff0000)*/)	\
			RTMP_IRQ_UNLOCK((pLock), IrqFlags);

// Prototypes of completion funuc.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
#define ATE_RTUSBBulkOutDataPacketComplete(purb, pt_regs)    ATE_RTUSBBulkOutDataPacketComplete(purb)
#endif
		
VOID ATE_RTUSBBulkOutDataPacketComplete(
	IN purbb_t purb,
	OUT struct pt_regs *pt_regs);

VOID ATE_RTUSBBulkOutDataPacket(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR			BulkOutPipeId);

VOID ATE_RTUSBCancelPendingBulkInIRP(
	IN	PRTMP_ADAPTER	pAd);


VOID ATEResetBulkIn(
	IN PRTMP_ADAPTER	pAd);

INT ATEResetBulkOut(
	IN PRTMP_ADAPTER	pAd);
#endif // RTMP_MAC_USB //

#ifdef RT30xx
#define ATE_RF_IO_READ8_BY_REG_ID(_A, _I, _pV)     RTMP_RF_IO_READ8_BY_REG_ID(_A, _I, _pV)
#define ATE_RF_IO_WRITE8_BY_REG_ID(_A, _I, _V)     RTMP_RF_IO_WRITE8_BY_REG_ID(_A, _I, _V)
#endif // RT30xx //


VOID rt_ee_read_all(
	IN  PRTMP_ADAPTER   pAd,
	OUT USHORT *Data);

VOID rt_ee_write_all(
	IN  PRTMP_ADAPTER   pAd,
	IN  USHORT *Data);

INT Set_ATE_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_DA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_SA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_BSSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_CHANNEL_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_TX_POWER0_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_TX_POWER1_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg);

INT	Set_ATE_TX_Antenna_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg);

INT	Set_ATE_RX_Antenna_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg);
	
INT	Set_ATE_TX_FREQOFFSET_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_TX_BW_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_TX_LENGTH_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_TX_COUNT_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_TX_MCS_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg);

INT	Set_ATE_TX_MODE_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg);

INT	Set_ATE_TX_GI_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg);


INT	Set_ATE_RX_FER_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Read_RF_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Write_RF1_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Write_RF2_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Write_RF3_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Write_RF4_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Load_E2P_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_ATE_Read_E2P_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);


INT	Set_ATE_Show_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT	Set_ATE_Help_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

#ifdef RALINK_28xx_QA
VOID ATE_QA_Statistics(
	IN PRTMP_ADAPTER		pAd,
	IN PRXWI_STRUC			pRxWI,
	IN PRT28XX_RXD_STRUC    p28xxRxD,
	IN PHEADER_802_11		pHeader);
	
VOID RtmpDoAte(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq);

VOID BubbleSort(
	IN  INT32 n,
	IN  INT32 a[]);

VOID CalNoiseLevel(
	IN  PRTMP_ADAPTER   pAdapter,
	IN  UCHAR           channel,
	OUT INT32           buffer[3][10]);

BOOLEAN SyncTxRxConfig(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	USHORT			offset, 
	IN	UCHAR			value);

INT Set_TxStop_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_RxStop_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

#ifdef DBG
INT Set_EERead_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_EEWrite_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_BBPRead_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_BBPWrite_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT Set_RFWrite_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);
#endif // DBG // 
#endif // RALINK_28xx_QA //


VOID ATEAsicSwitchChannel(
	IN PRTMP_ADAPTER pAd); 

VOID ATEAsicAdjustTxPower(
	IN PRTMP_ADAPTER pAd);

VOID ATEDisableAsicProtect(
	IN		PRTMP_ADAPTER	pAd);

CHAR ATEConvertToRssi(
	IN PRTMP_ADAPTER  pAd,
	IN CHAR				Rssi,
	IN UCHAR    RssiNumber);

VOID ATESampleRssi(
	IN PRTMP_ADAPTER	pAd,
	IN PRXWI_STRUC		pRxWI);


#ifdef CONFIG_STA_SUPPORT
VOID RTMPStationStop(
    IN  PRTMP_ADAPTER   pAd);

VOID RTMPStationStart(
    IN  PRTMP_ADAPTER   pAd);
#endif // CONFIG_STA_SUPPORT //
#endif // __ATE_H__ //