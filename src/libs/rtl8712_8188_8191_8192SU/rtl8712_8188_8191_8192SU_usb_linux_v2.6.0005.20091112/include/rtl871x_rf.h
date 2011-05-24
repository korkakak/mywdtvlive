#ifndef	__RTL871X_RF_H_ 
#define __RTL871X_RF_H_

#include <drv_conf.h>
#include <rtl871x_cmd.h>

#define OFDM_PHY		1
#define MIXED_PHY		2
#define CCK_PHY		3

#ifndef CONFIG_80211N_HT
#define		NumRates	(13)
#else
//#define NumRates	(26)
#define NumRates	(13)
#endif

#define RTL8711_RF_MAX_SENS 6
#define RTL8711_RF_DEF_SENS 4


#define NUM_CHANNELS	15
//#define NUM_REGULATORYS	21
#define NUM_REGULATORYS	1

//Country codes
#define USA							0x555320
#define EUROPE						0x1 //temp, should be provided later	
#define JAPAN						0x2 //temp, should be provided later	

struct	regulatory_class {
	u32	starting_freq;					//MHz, 
	u8	channel_set[NUM_CHANNELS];
	u8	channel_cck_power[NUM_CHANNELS];//dbm
	u8	channel_ofdm_power[NUM_CHANNELS];//dbm
	u8	txpower_limit;  				//dbm
	u8	channel_spacing;				//MHz
	u8	modem;
};


enum	_REG_PREAMBLE_MODE{
	PREAMBLE_LONG	= 1,
	PREAMBLE_AUTO	= 2,
	PREAMBLE_SHORT	= 3,
};


enum _RTL8712_RF_MIMO_CONFIG_{
 RTL8712_RFCONFIG_1T=0x10,
 RTL8712_RFCONFIG_2T=0x20,
 RTL8712_RFCONFIG_1R=0x01,
 RTL8712_RFCONFIG_2R=0x02,
 RTL8712_RFCONFIG_1T1R=0x11,
 RTL8712_RFCONFIG_1T2R=0x12,
 RTL8712_RFCONFIG_TURBO=0x92,
 RTL8712_RFCONFIG_2T2R=0x22
};


struct setphyinfo_parm;
extern void init_phyinfo(_adapter  *adapter, struct setphyinfo_parm* psetphyinfopara);
extern u8 writephyinfo_fw(_adapter *padapter, u32 addr);
extern u32 ch2freq(u32 ch);
extern u32 freq2ch(u32 freq);

void SetBBReg(PADAPTER padapter, u32 addr, u32 bitmask, u32 data);
u32 QueryBBReg(PADAPTER padapter, u32 addr, u32 bitmask);

void SetRFReg(PADAPTER padapter, int rfpath, u32 addr, u32 bitmask, u32 data);
u32 QueryRFReg(PADAPTER padapter, int rfpath, u32 addr, u32 bitmask);

#endif //_RTL8711_RF_H_

