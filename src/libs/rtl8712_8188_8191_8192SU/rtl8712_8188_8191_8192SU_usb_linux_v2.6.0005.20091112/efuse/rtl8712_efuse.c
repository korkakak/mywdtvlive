/******************************************************************************
* rtl8712_efuse.c                                                                                                                                 *
*                                                                                                                                          *
* Description :                                                                                                                       *
*                                                                                                                                           *
* Author :                                                                                                                       *
*                                                                                                                                         *
* History :                                                          
*
*                                        
*                                                                                                                                       *
* Copyright 2008, Realtek Corp.                                                                                                  *
*                                                                                                                                        *
* The contents of this file is the sole property of Realtek Corp.  It can not be                                     *
* be used, copied or modified without written permission from Realtek Corp.                                         *
*                                                                                                                                          *
*******************************************************************************/
#define _RTL8712_EFUSE_C_

#include <drv_conf.h>
#include <osdep_service.h>
#include <drv_types.h>

#include <rtl8712_efuse.h>
//------------------------------------------------------------------------------
#define _PRE_EXECUTE_READ_CMD_
//------------------------------------------------------------------------------
const u8 MAX_PGPKT_SIZE = 9; //header+ 2* 4 words (BYTES)
const u8 PGPKT_DATA_SIZE = 8; //BYTES sizeof(u8)*8
static int EFUSE_MAX_SIZE = 0x1FD; //reserve 3 bytes for HW stop read
#define PGPKG_MAX_WORDS	4

//------------------------------------------------------------------------------
static int efuse_one_byte_read(_adapter * padapter, u16 addr,u8 *data);
static int efuse_one_byte_write(_adapter * padapter,  u16 addr, u8 data);	
//------------------------------------------------------------------------------
static void efuse_reg_ctrl(_adapter * padapter, u8 bPowerOn)
{	

	struct mlme_priv	*pmlmepriv = &padapter->mlmepriv;	
	u8 tmpu8 = 0;
	if(_TRUE==bPowerOn){
		// -----------------SYS_FUNC_EN Digital Core Vdd enable ---------------------------------		

		tmpu8 = read8(padapter,SYS_FUNC_EN+1) ;
		
		if(!(tmpu8 & 0x20)){
			write8(padapter, SYS_FUNC_EN+1,( tmpu8|0x20) );
			msleep_os(10);
		}	

#if 1		
		//EE Loader to Retention path1: attach 0x1[0]=0
		write8(padapter, SYS_ISO_CTRL+1,(read8(padapter,SYS_ISO_CTRL+1) & 0xFE));	        
#endif

		// -----------------e-fuse pwr & clk reg ctrl ---------------------------------
		write8(padapter, EFUSE_TEST+3, (read8(padapter, EFUSE_TEST+3)|0x80));// 2.5v LDO
		
		// Change Efuse Clock for write action to 40MHZ
		//write8(padapter, EFUSE_CLK_CTRL, (read8(padapter, EFUSE_CLK_CTRL)|0x03));
		write8(padapter, EFUSE_CLK_CTRL, 0x03);
#ifdef _PRE_EXECUTE_READ_CMD_
		{
			unsigned char tmpdata;
			efuse_one_byte_read(padapter, 0,&tmpdata);
		}		
#endif
	}
	else{
		// -----------------e-fuse pwr & clk reg ctrl ---------------------------------
		write8(padapter, EFUSE_TEST+3, read8(padapter, EFUSE_TEST+3)&0x7F);
		
		// Change Efuse Clock for write action to 500K
		//write8(padapter, EFUSE_CLK_CTRL, read8(padapter, EFUSE_CLK_CTRL)&0xFE);
		write8(padapter, EFUSE_CLK_CTRL, 0x02);
#if 0
		// -----------------SYS_FUNC_EN Digital Core Vdd disable ---------------------------------
		if(check_fwstate(pmlmepriv, WIFI_MP_STATE) == _FALSE){ 
			write8(padapter, SYS_FUNC_EN+1,  read8(padapter,SYS_FUNC_EN+1)&0xDF);
		}
#endif

	}	
}
void efuse_reg_init(_adapter * padapter)
{
	efuse_reg_ctrl(padapter, _TRUE);	
}
void efuse_reg_uninit(_adapter * padapter)
{
	efuse_reg_ctrl(padapter, _FALSE);
}
//------------------------------------------------------------------------------
void efuse_change_max_size(_adapter * padapter)
{	
	u16 pre_pg_data_saddr = 0x1FB;
	
#if 1
	u16 i;
	u16 pre_pg_data_size = 5;
	u8 pre_pg_data[5];
	
	for(i=0;i <pre_pg_data_size;i++){
		efuse_one_byte_read(padapter, pre_pg_data_saddr+i,&pre_pg_data[i]);
	}
	if(	(pre_pg_data[0]==0x03) && (pre_pg_data[1]==0x00) && (pre_pg_data[2]==0x00) && 
		(pre_pg_data[3]==0x00) && (pre_pg_data[4]==0x0C) ){
		EFUSE_MAX_SIZE = 0x1F8;
	}
#else
	u8 efuse_data;

	efuse_one_byte_read(padapter, pre_pg_data_saddr,&efuse_data);
	if(efuse_data != 0xFF){
		EFUSE_MAX_SIZE = 0x1F8; //reserve :3 bytes
	}	
	RT_TRACE(_module_rtl8712_efuse_c_,_drv_alert_,("efuse_change_max_size , EFUSE_MAX_SIZE = %d\n",EFUSE_MAX_SIZE));		
#endif
	
}
int efuse_get_max_size(_adapter * padapter)
{
	return 	EFUSE_MAX_SIZE ;
}
//------------------------------------------------------------------------------
static int efuse_one_byte_read(_adapter * padapter, u16 addr,u8 *data)
{
	u8 tmpidx = 0;
	int bResult;
	
	// -----------------e-fuse reg ctrl ---------------------------------				
	write8(padapter, EFUSE_CTRL+1, (u8)(addr&0xFF));		//address
	write8(padapter, EFUSE_CTRL+2, ((u8)((addr>>8) &0x03) ) | (read8(padapter, EFUSE_CTRL+2)&0xFC ));	

	write8(padapter, EFUSE_CTRL+3,  0x72);//read cmd	

	while(!(0x80 &read8(padapter, EFUSE_CTRL+3)) && (tmpidx<100) ){
		tmpidx++;
	}
	if(tmpidx<100){			
		*data=read8(padapter, EFUSE_CTRL);
		RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Read Efuse at addr =0x%x, value=0x%x=====\n", addr,*data));
		bResult = _TRUE;
	}
	else{
		*data = 0xff;
		RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Read Efuse at addr =0x%x fail=====\n",addr));		
		bResult = _FALSE;
	}
	return bResult;
}
		

//------------------------------------------------------------------------------
static int efuse_one_byte_write(_adapter * padapter,  u16 addr, u8 data)
{
	u8 tmpidx = 0;
	int bResult;
	
	// -----------------e-fuse reg ctrl ---------------------------------				
	write8(padapter, EFUSE_CTRL+1, (u8)(addr&0xFF));		//address
	write8(padapter, EFUSE_CTRL+2, (read8(padapter, EFUSE_CTRL+2)&0xFC )|(u8)((addr>>8)&0x03) );

	write8(padapter, EFUSE_CTRL, data);//data		
	write8(padapter, EFUSE_CTRL+3, 0xF2);//write cmd		
		
	while((0x80 &  read8(padapter, EFUSE_CTRL+3)) && (tmpidx<100) ){
		tmpidx++;
	}
	
	if(tmpidx<100){			
		RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Write Efuse at addr =0x%x, value=0x%x=====\n", addr,data));
		bResult = _TRUE;
	}
	else{		
		RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Write Efuse at addr =0x%x fail=====\n",addr));		
		bResult = _FALSE;
	}		
	
	return bResult;	
}
//------------------------------------------------------------------------------
static u8 calculate_word_cnts(const u8 word_en){
	u8 word_cnts = 0;	

	u8 word_idx;
	for(word_idx = 0;word_idx<PGPKG_MAX_WORDS;word_idx++){
		if(!(word_en & BIT(word_idx)))	word_cnts++; // 0 : write enable		
	}
	return word_cnts;
}
//------------------------------------------------------------------------------
static void pgpacket_copy_data(const u8 word_en,const u8 *sourdata,u8 *targetdata)
{	
	u8 tmpindex = 0;
	u8 word_idx,byte_idx;
	
	for(word_idx = 0;word_idx<4;word_idx++){
		if(!(word_en&BIT(word_idx))){			
			byte_idx = word_idx*2 ;
			targetdata[byte_idx] 	  = sourdata[tmpindex++];
			targetdata[byte_idx+1] = sourdata[tmpindex++];			
		}
	}
}
//------------------------------------------------------------------------------
u16 efuse_get_current_size(_adapter * padapter)
{
	int bContinual = _TRUE;

	u16 efuse_addr = 0;
	u8 hoffset=0,hworden=0;	
	u8 efuse_data,word_cnts=0;
		
	while(bContinual && efuse_one_byte_read(padapter, efuse_addr ,&efuse_data) && (efuse_addr  < EFUSE_MAX_SIZE) ){			
		if(efuse_data!=0xFF){					
			hoffset = (efuse_data>>4) & 0x0F;
			hworden =  efuse_data & 0x0F;									
			word_cnts = calculate_word_cnts(hworden);
			//read next header							
			efuse_addr = efuse_addr + (word_cnts*2)+1;						
		}
		else{
			bContinual = _FALSE ;			
		}
	}	
	
	return efuse_addr;
	
}
//------------------------------------------------------------------------------
int efuse_pg_packet_read(_adapter * padapter,u8 offset,u8 *data)
{	
	u8 ReadState = PG_STATE_HEADER;	
	
	int bContinual = _TRUE;	

	u8 efuse_data,word_cnts=0;
	u16 efuse_addr = 0;
	u8 hoffset=0,hworden=0;	
	u8 tmpidx=0;
	u8 tmpdata[8];
	
	if(data==NULL)	return _FALSE;
	if(offset>0x0f)	return _FALSE;	
	
	_memset(data, 0xFF, sizeof(u8)*PGPKT_DATA_SIZE);	
	_memset(tmpdata, 0xFF, sizeof(u8)*PGPKT_DATA_SIZE);		
	
	while(bContinual && (efuse_addr  < EFUSE_MAX_SIZE) ){			
		//-------  Header Read -------------
		if(ReadState & PG_STATE_HEADER){
			if(efuse_one_byte_read(padapter, efuse_addr ,&efuse_data)&&(efuse_data!=0xFF)){				
				hoffset = (efuse_data>>4) & 0x0F;
				hworden =  efuse_data & 0x0F;									
				word_cnts = calculate_word_cnts(hworden);
				
				if(hoffset==offset){
					
					for(tmpidx = 0;tmpidx< word_cnts*2 ;tmpidx++){
						if(efuse_one_byte_read(padapter, efuse_addr+1+tmpidx ,&efuse_data) ){
							tmpdata[tmpidx] = efuse_data;						
						}					
					}
					ReadState = PG_STATE_DATA;	
					
				}
				else{//read next header	
					efuse_addr = efuse_addr + (word_cnts*2)+1;
					ReadState = PG_STATE_HEADER; 
				}
				
			}
			else{
				bContinual = _FALSE ;
			}
		}		
		//-------  Data section Read -------------
		else if(ReadState & PG_STATE_DATA){
			pgpacket_copy_data(hworden,tmpdata,data);
			efuse_addr = efuse_addr + (word_cnts*2)+1;
			ReadState = PG_STATE_HEADER; 
		}		
	}			
	
	
	if(	(data[0]==0xff) &&(data[1]==0xff) && (data[2]==0xff)  && (data[3]==0xff) &&
		(data[4]==0xff) &&(data[5]==0xff) && (data[6]==0xff)  && (data[7]==0xff))
		return _FALSE;
	else
		return _TRUE;

}
//------------------------------------------------------------------------------
static u8 pgpacket_write_data(_adapter * padapter,const u16 efuse_addr,const u8 word_en,const u8 *data)
{		
	u16 tmpaddr = 0;
	u16 start_addr = efuse_addr;

	u8 badworden = 0x0F;
	
	
	u8 tmpdata[8]; 

	u8 word_idx,byte_idx;

	memset(tmpdata,0xff,PGPKT_DATA_SIZE);

	for(word_idx = 0;word_idx<4;word_idx++){
		if(!(word_en&BIT(word_idx))){
			tmpaddr = start_addr;
			byte_idx = word_idx*2 ;
			efuse_one_byte_write(padapter,start_addr++, data[byte_idx]);
			efuse_one_byte_write(padapter,start_addr++, data[byte_idx+1]);

			efuse_one_byte_read(padapter,tmpaddr,     &tmpdata[byte_idx]);
			efuse_one_byte_read(padapter,tmpaddr+1, &tmpdata[byte_idx+1]);
			if((data[byte_idx]!=tmpdata[byte_idx])||(data[byte_idx+1]!=tmpdata[byte_idx+1])){
				badworden &=( ~BIT(word_idx));
			}
		}
	}

	return badworden;
}

//------------------------------------------------------------------------------
int efuse_pg_packet_write(_adapter * padapter,const u8 offset,const u8 word_en,const u8 *data)
{
	u8 WriteState = PG_STATE_HEADER;		

	int bContinual = _TRUE,bDataEmpty=_TRUE,bResult = _TRUE;
	u16 efuse_addr = 0;
	u8 efuse_data;

	u8 pg_header = 0;
	u16 remain_size = 0,curr_size = 0;
	u16 tmp_addr=0;
	
	u8 tmp_word_cnts=0,target_word_cnts=0;
	u8 tmp_header,match_word_en,tmp_word_en;
	
	u8 word_idx;

	PGPKT_STRUCT target_pkt;	
	PGPKT_STRUCT tmp_pkt;
	
	u8 originaldata[sizeof(u8)*8];
	u8 tmpindex = 0,badworden = 0x0F;

	static int repeat_times = 0;

	if( (curr_size= efuse_get_current_size(padapter)) >= EFUSE_MAX_SIZE){
		return _FALSE;
	}
	remain_size = EFUSE_MAX_SIZE - curr_size;

	
	
	target_pkt.offset = offset;
	target_pkt.word_en= word_en;
	memset(target_pkt.data,0xFF,sizeof(u8)*8);
	pgpacket_copy_data(word_en,data,target_pkt.data);
	target_word_cnts = calculate_word_cnts(target_pkt.word_en);	
	RT_TRACE(_module_rtl8712_efuse_c_,_drv_alert_,
		("====efuse_pg_packet_write EFUSE_MAX_SIZE=%d remain_size =%d, target_word_cnts=%d=====\n", EFUSE_MAX_SIZE,remain_size,(target_word_cnts*2+1)));
	if( remain_size <  (target_word_cnts*2+1)){
		RT_TRACE(_module_rtl8712_efuse_c_,_drv_alert_,("====efuse size isnot enough !!!!\n"));
		return _FALSE; //target_word_cnts + pg header(1 byte)
	}
	
	
	while( bContinual && (efuse_addr  < EFUSE_MAX_SIZE) ){
		
		if(WriteState==PG_STATE_HEADER){	
			bDataEmpty=_TRUE;
			badworden = 0x0F;		
			//************  so *******************	
			if( efuse_one_byte_read(padapter, efuse_addr ,&efuse_data) &&(efuse_data!=0xFF)){ 	
				tmp_header  =  efuse_data;
				
				tmp_pkt.offset 	= (tmp_header>>4) & 0x0F;
				tmp_pkt.word_en 	= tmp_header & 0x0F;					
				tmp_word_cnts 	= calculate_word_cnts(tmp_pkt.word_en);

				//************  so-1 *******************
				if(tmp_pkt.offset  != target_pkt.offset){
					efuse_addr = efuse_addr + (tmp_word_cnts*2) +1; //Next pg_packet
					WriteState = PG_STATE_HEADER;
				}
				else{	
					//************  so-2 *******************
					for(tmpindex=0 ; tmpindex<(tmp_word_cnts*2) ; tmpindex++){
						if(efuse_one_byte_read(padapter, (efuse_addr+1+tmpindex) ,&efuse_data)&&(efuse_data != 0xFF)){
							bDataEmpty = _FALSE;	
						}
					}	
					//************  so-2-1 *******************
					if(bDataEmpty == _FALSE){						
						efuse_addr = efuse_addr + (tmp_word_cnts*2) +1; //Next pg_packet	
						WriteState=PG_STATE_HEADER;
					}
					else{//************  so-2-2 *******************
						match_word_en = 0x0F;					
						for(word_idx=0;word_idx<4;word_idx++){
							if(   !( (target_pkt.word_en&BIT(word_idx))|(tmp_pkt.word_en&BIT(word_idx))  )){
								 match_word_en &= (~BIT(word_idx));
							}	
						}					
						//************  so-2-2-A *******************
						if((match_word_en&0x0F)!=0x0F){							
							badworden = pgpacket_write_data(padapter,efuse_addr+1, tmp_pkt.word_en ,target_pkt.data);
							
							//************  so-2-2-A-1 *******************
							//############################
							if(0x0F != (badworden&0x0F)){														
								u8 reorg_offset = offset;
								u8 reorg_worden=badworden;								
								efuse_pg_packet_write(padapter,reorg_offset,reorg_worden,originaldata);	
							}	
							//############################						

							tmp_word_en = 0x0F;						
							for(word_idx=0;word_idx<4;word_idx++){
								if(  (target_pkt.word_en&BIT(word_idx))^(match_word_en&BIT(word_idx))  ){
									tmp_word_en &= (~BIT(word_idx));
								}
							}				
							//************  so-2-2-A-2 *******************	
							if((tmp_word_en&0x0F)!=0x0F){
								//reorganize other pg packet														
								efuse_addr = efuse_get_current_size(padapter);								
								//===========================
								target_pkt.offset = offset;
								target_pkt.word_en= tmp_word_en;					
								//===========================
							}else{								
								bContinual = _FALSE;								
							}	
							WriteState=PG_STATE_HEADER;
							repeat_times++;
							if(repeat_times>_REPEAT_THRESHOLD_){
								bContinual = _FALSE;
								bResult = _FALSE;
							}
						}
						else{//************  so-2-2-B *******************
							//reorganize other pg packet						
							efuse_addr = efuse_addr + (2*tmp_word_cnts) +1;//next pg packet addr							
							//===========================
							target_pkt.offset = offset;
							target_pkt.word_en= target_pkt.word_en;					
							//===========================	
							WriteState=PG_STATE_HEADER;
						}
						
					}				
					
				}				
				
			}
			else	{ //************  s1: header == oxff  *******************	
				curr_size = efuse_get_current_size(padapter);
				remain_size = EFUSE_MAX_SIZE - curr_size;			
				RT_TRACE(_module_rtl8712_efuse_c_,_drv_alert_,("====efuse write header state remain_size =%d, target_word_cnts=%d=====\n", remain_size,(target_word_cnts*2+1)));
				if( remain_size <  (target_word_cnts*2+1)) //target_word_cnts + pg header(1 byte)
				{
					RT_TRACE(_module_rtl8712_efuse_c_,_drv_alert_,("====efuse size isnot enough !!!!\n"));
					bContinual = _FALSE;
					bResult = _FALSE;	
				}
				else{
					pg_header = ((target_pkt.offset << 4)&0xf0) |target_pkt.word_en;							

					efuse_one_byte_write(padapter,efuse_addr, pg_header);
					efuse_one_byte_read(padapter,efuse_addr, &tmp_header);		
			
					if(tmp_header == pg_header){ //************  s1-1 *******************								
						WriteState = PG_STATE_DATA;						
					}
					else if(tmp_header == 0xFF){//************  s1-3: if Write or read func doesn't work *******************		
						//efuse_addr doesn't change
						WriteState = PG_STATE_HEADER;					
						repeat_times++;
						if(repeat_times>_REPEAT_THRESHOLD_){
							bContinual = _FALSE;
							bResult = _FALSE;
						}
					}
					else{//************  s1-2 : fixed the header procedure *******************							
						tmp_pkt.offset = (tmp_header>>4) & 0x0F;
						tmp_pkt.word_en=  tmp_header & 0x0F;					
						tmp_word_cnts =  calculate_word_cnts(tmp_pkt.word_en);
																												
						//************  s1-2-A :cover the exist data *******************
						memset(originaldata,0xff,sizeof(u8)*8);
						
						if(efuse_pg_packet_read( padapter, tmp_pkt.offset,originaldata)){	//check if data exist 											
							badworden = pgpacket_write_data(padapter,efuse_addr+1,tmp_pkt.word_en,originaldata);	
							//############################
							if(0x0F != (badworden&0x0F)){														
								u8 reorg_offset = tmp_pkt.offset;
								u8 reorg_worden=badworden;								
								efuse_pg_packet_write(padapter,reorg_offset,reorg_worden,originaldata);	
								efuse_addr = efuse_get_current_size(padapter);							
							}
							//############################
							else{
								efuse_addr = efuse_addr + (tmp_word_cnts*2) +1; //Next pg_packet							
							}						
						}
						 //************  s1-2-B: wrong address*******************
						else{
							efuse_addr = efuse_addr + (tmp_word_cnts*2) +1; //Next pg_packet						
						}	
						WriteState=PG_STATE_HEADER;	
						repeat_times++;
						if(repeat_times>_REPEAT_THRESHOLD_){
							bContinual = _FALSE;
							bResult = _FALSE;
						}
					}
				}

			}

		}
		//write data state
		else if(WriteState==PG_STATE_DATA) {//************  s1-1  *******************
			badworden = 0x0f;
			badworden = pgpacket_write_data(padapter,efuse_addr+1,target_pkt.word_en,target_pkt.data);	
			if((badworden&0x0F)==0x0F){ //************  s1-1-A *******************
				bContinual = _FALSE;
			}
			else{//reorganize other pg packet //************  s1-1-B *******************
				efuse_addr = efuse_addr + (2*target_word_cnts) +1;//next pg packet addr				
						
				//===========================
				target_pkt.offset = offset;
				target_pkt.word_en= badworden;	
				target_word_cnts = calculate_word_cnts(target_pkt.word_en);
				//===========================			
				WriteState=PG_STATE_HEADER;	
				repeat_times++;
				if(repeat_times>_REPEAT_THRESHOLD_){
					bContinual = _FALSE;
					bResult = _FALSE;
				}
			}
		}
	}

	return bResult;
}
//------------------------------------------------------------------------------
static int efuse_one_byte_rw(_adapter * padapter, u8 bRead, u16 addr, u8 *data)
{
	UCHAR tmpidx = 0;
	int bResult;
	u8 tmpv8=0;
	
	// -----------------e-fuse reg ctrl ---------------------------------				

	write8(padapter, EFUSE_CTRL+1, (u8)(addr&0xFF));		//address
	tmpv8 = ((u8)((addr>>8) &0x03) ) | (read8(padapter, EFUSE_CTRL+2)&0xFC );
	write8(padapter, EFUSE_CTRL+2, tmpv8);

	if(_TRUE==bRead){
		
		write8(padapter, EFUSE_CTRL+3,  0x72);//read cmd		

		while(!(0x80 & read8(padapter, EFUSE_CTRL+3)) && (tmpidx<100) ){
			tmpidx++;
		}
		if(tmpidx<100){			
			*data=read8(padapter, EFUSE_CTRL);
			bResult = _TRUE;
		}
		else{
			*data = 0;
			RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Read Efuse at addr =0x%x fail=====\n",addr));		
			bResult = _FALSE;
		}
		
	}
	else{
		write8(padapter, EFUSE_CTRL, *data);//data
		
		write8(padapter, EFUSE_CTRL+3, 0xF2);//write cmd		
		
		while((0x80 & read8(padapter, EFUSE_CTRL+3)) && (tmpidx<100) ){
			tmpidx++;
		}
		if(tmpidx<100){			
			*data=read8(padapter, EFUSE_CTRL);
			RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Write Efuse at addr =0x%x, value=0x%x=====\n", addr,*data));
			bResult = _TRUE;
		}
		else{
			*data = 0;
			RT_TRACE(_module_rtl8712_efuse_c_,_drv_err_,("====Write Efuse at addr =0x%x fail=====\n",addr));		
			bResult = _FALSE;
		}
		
	}
	return bResult;	
}
//------------------------------------------------------------------------------
void efuse_access(_adapter * padapter, u8 bRead,u16 start_addr, u16 cnts, u8 *data)
{	
	int 	i = 0;

	if(start_addr>0x200) return;
	
	efuse_reg_ctrl(padapter, _TRUE);
	//-----------------e-fuse one byte read / write ------------------------------	
	for(i=0;i<cnts;i++){		
		efuse_one_byte_rw(padapter,bRead, start_addr+i , data+i);		
		//RT_TRACE(_module_rtl871x_mp_ioctl_c_,_drv_err_,("==>efuse_access addr:0x%02x value:0x%02x\n",data+i,*(data+i)));
	}
	efuse_reg_ctrl(padapter, _FALSE);
	
}	
//------------------------------------------------------------------------------

