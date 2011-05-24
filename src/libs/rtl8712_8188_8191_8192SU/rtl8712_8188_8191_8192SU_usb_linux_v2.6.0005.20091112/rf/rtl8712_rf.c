/******************************************************************************
* rtl8712_rf.c                                                                                                                                 *
*                                                                                                                                          *
* Description :                                                                                                                       *
*                                                                                                                                           *
* Author :                                                                                                                       *
*                                                                                                                                         *
* History :                                                          
*
*                                        
*                                                                                                                                       *
* Copyright 2007, Realtek Corp.                                                                                                  *
*                                                                                                                                        *
* The contents of this file is the sole property of Realtek Corp.  It can not be                                     *
* be used, copied or modified without written permission from Realtek Corp.                                         *
*                                                                                                                                          *
*******************************************************************************/
#define _RTL8712_RF_C_

#include <drv_conf.h>
#include <osdep_service.h>
#include <drv_types.h>

#define	bMaskDWord	0xffffffff

#define	bMask20Bits        0xfffff	// RF Reg mask bits T65 RF

#define	HST_RDBUSY		BIT(0)

// The following two definition are only used for USB interface.
#define		RF_BB_CMD_ADDR				0x02c0	// RF/BB read/write command address.
#define		RF_BB_CMD_DATA				0x02c4	// RF/BB read/write command data.


/**
* Function:	phy_CalculateBitShift
*
* OverView:	Get shifted position of the BitMask
*
* Input:
*			u4Byte		BitMask,	
*
* Output:	none
* Return:		u4Byte		Return the shift bit bit position of the mask
*/
static u32 phy_CalculateBitShift(u32 BitMask)
{
	u32 i;

	for(i=0; i<=31; i++)
	{
		if ( ((BitMask>>i) &  0x1 ) == 1)
			break;
	}

	return (i);
}


u32 phy_QueryBBReg(IN PADAPTER	Adapter, IN u32 RegAddr)
{	
	u32	ReturnValue = 0xffffffff;
	u8	PollingCnt = 50;	
	
	read32(Adapter, RegAddr);	

	do
	{
		// Make sure that access could be done.
		if((read8(Adapter, PHY_REG_RPT)&HST_RDBUSY) == 0)
			break;
		
	}while( --PollingCnt );

	if(PollingCnt == 0)
	{
		//ERR_8712 ("Fail!!!phy_QueryBBReg(): RegAddr(%#x) = %#x\n", RegAddr, ReturnValue);
	}
	else
	{
		// Data FW read back.
		ReturnValue = read32(Adapter, PHY_REG_DATA);		
	}

	return ReturnValue;
	
}

void phy_SetBBReg(IN	PADAPTER Adapter, IN	u32 RegAddr, IN	u32 Data)
{
	write32(Adapter, RegAddr, Data);	
}

u32 phy_QueryRFReg(
	IN	PADAPTER	Adapter,
	IN	int			eRFPath,
	IN	u32			Offset
	)
{	
	u32	ReturnValue = 0;	
	u8	PollingCnt = 50;


	Offset &= 0x3f; //RF_Offset= 0x00~0x3F		
	
	write32(Adapter, RF_BB_CMD_ADDR, 0xF0000002|(Offset<<8)|//RF_Offset= 0x00~0x3F
											(eRFPath<<16)); 	//RF_Path = 0(A) or 1(B)
	
	do
	{
		// Make sure that access could be done.
		if(read32(Adapter, RF_BB_CMD_ADDR) == 0)
			break;
		
	}while( --PollingCnt );

	// Data FW read back.
	ReturnValue = read32(Adapter, RF_BB_CMD_DATA);	
	
	return ReturnValue;

}

void phy_SetRFReg(
	IN	PADAPTER	Adapter,
	IN	int			eRFPath,
	IN	u32			RegAddr,
	IN	u32			Data
	)
{
	u8	PollingCnt = 50;
	
	
	RegAddr &= 0x3f; //RF_Offset= 0x00~0x3F
	
	write32(Adapter, RF_BB_CMD_DATA, Data);	
	write32(Adapter, RF_BB_CMD_ADDR, 0xF0000003|(RegAddr<<8)| //RF_Offset= 0x00~0x3F
												(eRFPath<<16));  //RF_Path = 0(A) or 1(B)
	
	do
	{
		// Make sure that access could be done.
		if(read32(Adapter, RF_BB_CMD_ADDR) == 0)
				break;
		
	}while( --PollingCnt );		

	if(PollingCnt == 0)
	{		
		//ERR_8712("phy_SetRFReg(): Set RegAddr(%#x) = %#x Fail!!!\n", RegAddr, Data);
	}
	

}

void SetBBReg(PADAPTER padapter, u32 addr, u32 bitmask, u32 data)
{
	u32	OriginalValue, BitShift, NewValue;

	if(bitmask!= bMaskDWord)//if not "double word" write
	{		
		OriginalValue = phy_QueryBBReg(padapter, addr);
		BitShift = phy_CalculateBitShift(bitmask);
            	NewValue = ((OriginalValue & (~bitmask)) | (data << BitShift));
		phy_SetBBReg(padapter, addr, NewValue);	
	}
	else
	{
		phy_SetBBReg(padapter, addr, data);	
	}	

}

u32 QueryBBReg(PADAPTER padapter, u32 addr, u32 bitmask)
{
  	u32	ReturnValue = 0, OriginalValue, BitShift;

	//
	// <Roger_Notes> Due to 8051 operation cycle (limitation cycle: 6us) and 1-Byte access issue, we should use 
	// 4181 to access Base Band instead of 8051 on USB interface to make sure that access could be done in 
	// infinite cycle.
	// 2008.09.06.
	//

	OriginalValue = phy_QueryBBReg(padapter, addr);
	
	if(bitmask!= bMaskDWord)//if not "double word" write
	{		
		//OriginalValue = phy_QueryBBReg(padapter, addr);

		BitShift = phy_CalculateBitShift(bitmask);
		ReturnValue = (OriginalValue & bitmask) >> BitShift;
	}
	else
	{
		//ReturnValue = phy_QueryBBReg(padapter, addr);
		ReturnValue = OriginalValue;
	}

	return (ReturnValue);	
	
}

void SetRFReg(PADAPTER padapter, int rfpath, u32 addr, u32 bitmask, u32 data)
{	
	u32 	Original_Value, BitShift, New_Value;

#if 0//gtest
	if (!Adapter->HalFunc.PHYCheckIsLegalRfPathHandler(Adapter, eRFPath))
	{
		return;
	}
#endif

	//
	// <Roger_Notes> Due to 8051 operation cycle (limitation cycle: 6us) and 1-Byte access issue, we should use 
	// 4181 to access Base Band instead of 8051 on USB interface to make sure that access could be done in 
	// infinite cycle.
	// 2008.09.06.
	//

	if (bitmask != bMask20Bits) // RF data is 12 bits only
	{
		Original_Value = phy_QueryRFReg(padapter, rfpath, addr);
		BitShift =  phy_CalculateBitShift(bitmask);
		New_Value = ((Original_Value & (~bitmask)) | (data<< BitShift));
		phy_SetRFReg(padapter, rfpath, addr, New_Value);
	}
	else
	{
		phy_SetRFReg(padapter, rfpath, addr, data);
	}	
	
}


