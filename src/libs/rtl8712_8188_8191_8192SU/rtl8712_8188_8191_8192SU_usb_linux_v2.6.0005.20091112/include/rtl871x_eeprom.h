#ifndef __RTL871X_EEPROM_H__
#define __RTL871X_EEPROM_H__

#include <drv_conf.h>
#include <osdep_service.h>

#define	RTL8712_EEPROM_ID			0x8712
#define	EEPROM_MAX_SIZE			256
#define	CLOCK_RATE					50			//100us		

//- EEPROM opcodes
#define EEPROM_READ_OPCODE		06
#define EEPROM_WRITE_OPCODE		05
#define EEPROM_ERASE_OPCODE		07
#define EEPROM_EWEN_OPCODE		19      // Erase/write enable
#define EEPROM_EWDS_OPCODE		16      // Erase/write disable

//Country codes
#define USA							0x555320
#define EUROPE						0x1 //temp, should be provided later	
#define JAPAN						0x2 //temp, should be provided later

#ifdef CONFIG_SDIO_HCI
#define eeprom_cis0_sz	17
#define eeprom_cis1_sz	50
#endif

struct eeprom_priv 
{    
	u8		bautoload_fail_flag;
	u8		bempty;
	u8		sys_config;
	u8		mac_addr[6];	
	u8		config0;
	u16	channel_plan;
	u8		country_string[3];	
	u8		tx_power_b[15];
	u8		tx_power_g[15];
	u8		tx_power_a[201];

	u8		efuse_eeprom_data[EEPROM_MAX_SIZE];

	#ifdef CONFIG_SDIO_HCI
	u8		sdio_setting;	
	u32		ocr;
	u8		cis0[eeprom_cis0_sz];
	u8		cis1[eeprom_cis1_sz];	
	#endif

};


extern void eeprom_write16(_adapter *padapter, u16 reg, u16 data);
extern u16 eeprom_read16(_adapter *padapter, u16 reg);
extern void read_eeprom_content(_adapter *padapter);
extern void eeprom_read_sz(_adapter * padapter, u16 reg,u8* data, u32 sz); 

extern void read_eeprom_content_by_attrib(_adapter *	padapter	);

#endif  //__RTL871X_EEPROM_H__
