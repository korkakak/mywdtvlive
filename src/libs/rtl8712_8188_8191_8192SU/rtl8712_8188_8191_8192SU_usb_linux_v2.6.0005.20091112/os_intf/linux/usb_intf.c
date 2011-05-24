#define _HCI_INTF_C_

#include <drv_conf.h>
#include <osdep_service.h>
#include <drv_types.h>
#include <recv_osdep.h>
#include <xmit_osdep.h>
#include <hal_init.h>
#include <rtl8712_efuse.h>

#ifndef CONFIG_USB_HCI

#error "CONFIG_USB_HCI shall be on!\n"

#endif

#include <usb_vendor_req.h>
#include <usb_ops.h>
#include <usb_osintf.h>
#include <usb_hal.h>

#if defined (PLATFORM_LINUX) && defined (PLATFORM_WINDOWS)

#error "Shall be Linux or Windows, but not both!\n"

#endif

static struct usb_interface *pintf;

extern u32 start_drv_threads(_adapter *padapter);
extern void stop_drv_threads (_adapter *padapter);
extern u8 init_drv_sw(_adapter *padapter);
extern u8 free_drv_sw(_adapter *padapter);
extern struct net_device *init_netdev(void);

void r871x_dev_unload(_adapter *padapter);

static int r871xu_drv_init(struct usb_interface *pusb_intf,const struct usb_device_id *pdid);
static void r871xu_dev_remove(struct usb_interface *pusb_intf);

static struct usb_device_id rtl871x_usb_id_tbl[] ={
	//92SU
	// Realtek */
	{USB_DEVICE(0x0bda, 0x8171)},
	{USB_DEVICE(0x0bda, 0x8172)},
	{USB_DEVICE(0x0bda, 0x8173)},
	{USB_DEVICE(0x0bda, 0x8174)},
	{USB_DEVICE(0x0bda, 0x8712)},
	{USB_DEVICE(0x0bda, 0x8713)},
	// Corega */
	{USB_DEVICE(0x07aa, 0x0047)},
	// Dlink */
	{USB_DEVICE(0x07d1, 0x3303)},
	{USB_DEVICE(0x07d1, 0x3302)},
	{USB_DEVICE(0x07d1, 0x3300)},
	{USB_DEVICE(0x07D1, 0x3304)},
	// EnGenius */
	{USB_DEVICE(0x1740, 0x9603)},
	{USB_DEVICE(0x1740, 0x9605)},
	// Belkin */
	{USB_DEVICE(0x050d, 0x815F)},
	{USB_DEVICE(0x050D, 0x945A)},
	{USB_DEVICE(0x050D, 0x845A)}, // Belkin F7D2101	
	// Guillemot */
	{USB_DEVICE(0x06f8, 0xe031)},
	// Edimax */
	{USB_DEVICE(0x7392, 0x7611)},
	{USB_DEVICE(0x7392, 0x7612)},
	{USB_DEVICE(0x7392, 0x7622)},
	// Sitecom */	
	{USB_DEVICE(0x0DF6, 0x0045)},
	// Hawking */
	{USB_DEVICE(0x0E66, 0x0015)},
	{USB_DEVICE(0x0E66, 0x0016)},
	{USB_DEVICE(0x0b05, 0x1786)},
	{USB_DEVICE(0x0BDA, 0x8174)}, /* Hornetek HT-H10DN JANUS ver. 2.00 */
	//*/
	{USB_DEVICE(0x13D3, 0x3306)},
	{USB_DEVICE(0x13D3, 0x3309)},
	{USB_DEVICE(0x13D3, 0x3310)},
	{}
};

typedef struct _driver_priv{
      
	struct usb_driver r871xu_drv;
	int drv_registered;	

}drv_priv, *pdrv_priv;


static drv_priv drvpriv = {	
		
	.r871xu_drv.name="r871x_usb_drv",
	.r871xu_drv.id_table=rtl871x_usb_id_tbl,
	.r871xu_drv.probe=r871xu_drv_init,
	.r871xu_drv.disconnect=r871xu_dev_remove,
	.r871xu_drv.suspend=NULL,
	.r871xu_drv.resume=NULL,
	
};	

MODULE_DEVICE_TABLE(usb, rtl871x_usb_id_tbl);

uint usb_dvobj_init(_adapter * padapter)
{
	int i;
	u8 val8;
	u32 	blocksz;
	uint	status=_SUCCESS;
	
	struct	usb_device_descriptor 		*pdev_desc;

	struct	usb_host_config			*phost_conf;
	struct	usb_config_descriptor 		*pconf_desc;

	struct	usb_host_interface		*phost_iface;
	struct	usb_interface_descriptor	*piface_desc;
	
	struct	usb_host_endpoint		*phost_endp;
	struct	usb_endpoint_descriptor		*pendp_desc;
	
	
	struct dvobj_priv *pdvobjpriv=&padapter->dvobjpriv;
	struct usb_device *pusbd=pdvobjpriv->pusbdev;

	PURB urb = NULL;

_func_enter_;
	
	pdvobjpriv->padapter=padapter;
	padapter->EepromAddressSize = 6;

	//pdvobjpriv->nr_endpoint = 6;
	//
	pdev_desc = &pusbd->descriptor;

#if 0
	printk("\n8712_usb_device_descriptor:\n");
	printk("bLength=%x\n", pdev_desc->bLength);
	printk("bDescriptorType=%x\n", pdev_desc->bDescriptorType);
	printk("bcdUSB=%x\n", pdev_desc->bcdUSB);
	printk("bDeviceClass=%x\n", pdev_desc->bDeviceClass);
	printk("bDeviceSubClass=%x\n", pdev_desc->bDeviceSubClass);
	printk("bDeviceProtocol=%x\n", pdev_desc->bDeviceProtocol);
	printk("bMaxPacketSize0=%x\n", pdev_desc->bMaxPacketSize0);
	printk("idVendor=%x\n", pdev_desc->idVendor);
	printk("idProduct=%x\n", pdev_desc->idProduct);
	printk("bcdDevice=%x\n", pdev_desc->bcdDevice);	
	printk("iManufacturer=%x\n", pdev_desc->iManufacturer);
	printk("iProduct=%x\n", pdev_desc->iProduct);
	printk("iSerialNumber=%x\n", pdev_desc->iSerialNumber);
	printk("bNumConfigurations=%x\n", pdev_desc->bNumConfigurations);
#endif	
	
	phost_conf = pusbd->actconfig;
	pconf_desc = &phost_conf->desc;
	
#if 0	
	printk("\n8712_usb_configuration_descriptor:\n");
	printk("bLength=%x\n", pconf_desc->bLength);
	printk("bDescriptorType=%x\n", pconf_desc->bDescriptorType);
	printk("wTotalLength=%x\n", pconf_desc->wTotalLength);
	printk("bNumInterfaces=%x\n", pconf_desc->bNumInterfaces);
	printk("bConfigurationValue=%x\n", pconf_desc->bConfigurationValue);
	printk("iConfiguration=%x\n", pconf_desc->iConfiguration);
	printk("bmAttributes=%x\n", pconf_desc->bmAttributes);
	printk("bMaxPower=%x\n", pconf_desc->bMaxPower);
#endif

	//printk("\n/****** num of altsetting = (%d) ******/\n", pintf->num_altsetting);
		
	phost_iface = &pintf->altsetting[0];
	piface_desc = &phost_iface->desc;

#if 0
	printk("\n8712_usb_interface_descriptor:\n");
	printk("bLength=%x\n", piface_desc->bLength);
	printk("bDescriptorType=%x\n", piface_desc->bDescriptorType);
	printk("bInterfaceNumber=%x\n", piface_desc->bInterfaceNumber);
	printk("bAlternateSetting=%x\n", piface_desc->bAlternateSetting);
	printk("bNumEndpoints=%x\n", piface_desc->bNumEndpoints);
	printk("bInterfaceClass=%x\n", piface_desc->bInterfaceClass);
	printk("bInterfaceSubClass=%x\n", piface_desc->bInterfaceSubClass);
	printk("bInterfaceProtocol=%x\n", piface_desc->bInterfaceProtocol);
	printk("iInterface=%x\n", piface_desc->iInterface);	
#endif
	
	pdvobjpriv->nr_endpoint = piface_desc->bNumEndpoints;
	

	//printk("\ndump 8712_usb_endpoint_descriptor:\n");

	for(i=0; i<pdvobjpriv->nr_endpoint; i++)
	{
		phost_endp = phost_iface->endpoint+i;
		if(phost_endp)
		{
			pendp_desc = &phost_endp->desc;
		
			printk("\n8712_usb_endpoint_descriptor(%d):\n", i);
			printk("bLength=%x\n",pendp_desc->bLength);
			printk("bDescriptorType=%x\n",pendp_desc->bDescriptorType);
			printk("bEndpointAddress=%x\n",pendp_desc->bEndpointAddress);
			//printk("bmAttributes=%x\n",pendp_desc->bmAttributes);
			//printk("wMaxPacketSize=%x\n",pendp_desc->wMaxPacketSize);
			printk("wMaxPacketSize=%x\n",le16_to_cpu(pendp_desc->wMaxPacketSize));
			printk("bInterval=%x\n",pendp_desc->bInterval);
			//printk("bRefresh=%x\n",pendp_desc->bRefresh);
			//printk("bSynchAddress=%x\n",pendp_desc->bSynchAddress);	
		}	

	}

	printk("\n");
	
	if (pusbd->speed==USB_SPEED_HIGH)
	{
                pdvobjpriv->ishighspeed = _TRUE;
                printk("8712u : USB_SPEED_HIGH\n");
	}
	else
	{
		 pdvobjpriv->ishighspeed = _FALSE;
                 printk("8712u: NON USB_SPEED_HIGH\n");
	}
	  	
			
	printk("nr_endpoint=%d\n", pdvobjpriv->nr_endpoint);
	  	
	if ( (alloc_io_queue(padapter)) == _FAIL)
	{
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,(" \n Can't init io_reqs\n"));
		status = _FAIL;			
	}	
	
	_init_sema(&(padapter->dvobjpriv.usb_suspend_sema), 0);


_func_exit_;
	
	return status;


}

void usb_dvobj_deinit(_adapter * padapter){
	
	struct dvobj_priv *pdvobjpriv=&padapter->dvobjpriv;

	_func_enter_;


	_func_exit_;
}

void rtl871x_intf_stop(_adapter *padapter)
{
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+rtl871x_intf_stop\n"));
	
	//disabel_hw_interrupt
	if(padapter->bSurpriseRemoved == _FALSE)
	{
		//device still exists, so driver can do i/o operation
		//TODO:
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("SurpriseRemoved==_FALSE\n"));
	}
	
	//cancel in irp
	if(padapter->dvobjpriv.inirp_deinit !=NULL)
	{	
		padapter->dvobjpriv.inirp_deinit(padapter);	
	}	

	//cancel out irp
	usb_write_port_cancel(padapter);


	//todo:cancel other irps

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-rtl871x_intf_stop\n"));

}

#if 0
void r871x_dev_unload(_adapter *padapter)
{
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+r871x_dev_unload\n"));

	if(padapter->bSurpriseRemoved == _TRUE)
	{		
		padapter->bDriverStopped = _TRUE;
	
		RT_TRACE(_module_os_intfs_c_, _drv_info_, ("padapter->bSurpriseRemoved==_TRUE\n"));
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("unload -> surprise removed\n"));
		
		free_drv_sw(padapter);
		
		return;

	}
					
       padapter->bDriverStopped = _TRUE;
	padapter->bSurpriseRemoved = _TRUE;   

	rtl871x_intf_stop(padapter);

	stop_drv_threads(padapter);

	rtl871x_hal_deinit(padapter);

	
	if(padapter->dvobj_deinit)
	{
		padapter->dvobj_deinit(padapter);
		
	}else
	{
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("Initialize hcipriv.hci_priv_init error!!!\n"));
	}			
	
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-r871x_dev_unload\n"));
}
#else
void r871x_dev_unload(_adapter *padapter)
{
	struct net_device *pnetdev= (struct net_device*)padapter->pnetdev;

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+r871x_dev_unload\n"));

	if(padapter->bup == _TRUE)
	{
		printk("+r871x_dev_unload\n");
		//s1.
/*		if(pnetdev)   
     		{
        		netif_carrier_off(pnetdev);
     	  		netif_stop_queue(pnetdev);
     		}
		
		//s2.
		//s2-1.  issue disassoc_cmd to fw
		disassoc_cmd(padapter);	
		//s2-2.  indicate disconnect to os
		indicate_disconnect(padapter);				
		//s2-3. 
	       free_assoc_resources(padapter);	
		//s2-4.
		free_network_queue(padapter);*/

		padapter->bDriverStopped = _TRUE;
	
		//s3.
		rtl871x_intf_stop(padapter);

		//s4.
		stop_drv_threads(padapter);


		//s5.
		if(padapter->bSurpriseRemoved == _FALSE)
		{
			printk("r871x_dev_unload()->rtl871x_hal_deinit()\n");
			rtl871x_hal_deinit(padapter);

			padapter->bSurpriseRemoved = _TRUE;
		}	

		//s6.	
		if(padapter->dvobj_deinit)
		{
			padapter->dvobj_deinit(padapter);
		
		}
		else
		{
			RT_TRACE(_module_hci_intfs_c_,_drv_err_,("Initialize hcipriv.hci_priv_init error!!!\n"));
		}			
		
		padapter->bup = _FALSE;

	}
	else
	{
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("r871x_dev_unload():padapter->bup == _FALSE\n" ));
	}
				
	printk("-r871x_dev_unload\n");		
	
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-r871x_dev_unload\n"));
	
}
#endif
/*
 * drv_init() - a device potentially for us
 *
 * notes: drv_init() is called when the bus driver has located a card for us to support.
 *        We accept the new device by returning 0.
*/
static int r871xu_drv_init(struct usb_interface *pusb_intf,const struct usb_device_id *pdid)
{  	
  	uint status;	 
	_adapter *padapter = NULL;
  	struct dvobj_priv *pdvobjpriv;
	struct net_device *pnetdev;

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+871x - drv_init\n"));
	//printk("+871x - drv_init\n");

	//2009.8.13, by Thomas
	// In this probe function, O.S. will provide the usb interface pointer to driver.
	// We have to increase the reference count of the usb device structure by using the usb_get_dev function.
	usb_get_dev(interface_to_usbdev(pusb_intf));

	pintf = pusb_intf;

	//step 1.
	pnetdev = init_netdev();
	if (!pnetdev)
		goto error;	
	
	padapter = netdev_priv(pnetdev);
	pdvobjpriv = &padapter->dvobjpriv;	
	pdvobjpriv->padapter = padapter;

	padapter->dvobjpriv.pusbdev=interface_to_usbdev(pusb_intf);
	usb_set_intfdata(pusb_intf, pnetdev);	
	SET_NETDEV_DEV(pnetdev, &pusb_intf->dev);

	//step 2.
	padapter->dvobj_init=&usb_dvobj_init;
	padapter->dvobj_deinit=&usb_dvobj_deinit;
	padapter->halpriv.hal_bus_init=&usb_hal_bus_init;
	padapter->halpriv.hal_bus_deinit=&usb_hal_bus_deinit;	
	padapter->dvobjpriv.inirp_init=&usb_inirp_init;
	padapter->dvobjpriv.inirp_deinit=&usb_inirp_deinit;

	//step 3.
	//initialize the dvobj_priv 		
	if(padapter->dvobj_init ==NULL){
			RT_TRACE(_module_hci_intfs_c_,_drv_err_,("\n Initialize dvobjpriv.dvobj_init error!!!\n"));
			goto error;
	}else{
	
		status=padapter->dvobj_init(padapter);
		if (status != _SUCCESS) {
			RT_TRACE(_module_hci_intfs_c_,_drv_err_,("\n initialize device object priv Failed!\n"));			
			goto error;
		} 
	}

	//step 4.
	status = init_drv_sw(padapter);	
	if(status ==_FAIL){
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("Initialize driver software resource Failed!\n"));	
		goto error;					
	}	

#if 1
	//step 5. read efuse/eeprom data and get mac_addr
	{
		int i, offset;
		u8 mac[6];
		u8 *pdata = padapter->eeprompriv.efuse_eeprom_data;		
		

		efuse_reg_init(padapter);

	
		for(i=0, offset=0 ; i<128; i+=8, offset++)
		{
			efuse_pg_packet_read(padapter, offset, &pdata[i]);
			
		}
	
		efuse_reg_uninit(padapter);
	
		
		//for(i=0; i<128; j++)
		//	printk("%d=0x%x\n", i, pdata[i]);


	 	_memcpy(mac, &pdata[0x12], 6);//offset = 0x12 for usb in efuse 

		if(	((mac[0]==0xff) &&(mac[1]==0xff) && (mac[2]==0xff)  && (mac[3]==0xff) &&
			(mac[4]==0xff) &&(mac[5]==0xff) ) || 
			((mac[0]==0x0) &&(mac[1]==0x0) && (mac[2]==0x0)  && (mac[3]==0x0) &&
			(mac[4]==0x0) &&(mac[5]==0x0)))
		{
		mac[0]=0x00;
		mac[1]=0xe0;
		mac[2]=0x4c;
		mac[3]=0x87;
		mac[4]=0x00;
		mac[5]=0x00;
		}
	
		_memcpy(pnetdev->dev_addr, mac/*padapter->eeprompriv.mac_addr*/, ETH_ALEN);

		printk("MAC Address from efuse= %x-%x-%x-%x-%x-%x\n", 
				 mac[0],	mac[1],  mac[2],	mac[3], mac[4], mac[5]);				 
			
		
	}	
#endif

	//step 6.
	/* Tell the network stack we exist */
	if (register_netdev(pnetdev) != 0) {
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("register_netdev() failed\n"));
		goto error;
	}
	
	
  	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-drv_init - Adapter->bDriverStopped=%d, Adapter->bSurpriseRemoved=%d\n",padapter->bDriverStopped, padapter->bSurpriseRemoved));
  	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-871x_drv - drv_init, success!\n"));
	//printk("-871x_drv - drv_init, success!\n");

  	return 0;

error:	      

	usb_put_dev(interface_to_usbdev(pusb_intf));//decrease the reference count of the usb device structure if driver fail on initialzation

	usb_set_intfdata(pusb_intf, NULL);

   	if(padapter->dvobj_deinit==NULL){
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("\n Initialize dvobjpriv.dvobj_deinit error!!!\n"));
	}else{
		padapter->dvobj_deinit(padapter);
	} 	  

      if(pnetdev)
      {	 
          //unregister_netdev(pnetdev);
          free_netdev(pnetdev);
      }

      RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-871x_sdio - drv_init, fail!\n"));
      //printk("-871x_sdio - drv_init, fail!\n");
	  
	return -ENODEV;
	  
}

/*
 * dev_remove() - our device is being removed
*/
#if 0
static void r871xu_dev_remove(struct usb_interface *pusb_intf)
{	
	_irqL irqL;
	struct net_device *pnetdev=usb_get_intfdata(pusb_intf);	
     _adapter *padapter = (_adapter*)netdev_priv(pnetdev);
	 struct	mlme_priv *pmlmepriv = &padapter->mlmepriv;

_func_exit_;

	if(padapter)	
	{
       		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+dev_remove()\n"));
		
		pnetdev= (struct net_device*)padapter->pnetdev;	
      		
		padapter->bSurpriseRemoved = _TRUE;	 
     
    		if(pnetdev)   
     		{
        		netif_carrier_off(pnetdev);
     	  		netif_stop_queue(pnetdev);
     		}

		rtl871x_intf_stop(padapter);			
		stop_drv_threads(padapter);

		// indicate-disconnect if necssary (free all assoc-resources)
		// dis-assoc from assoc_sta (optional)			
		_enter_critical(&pmlmepriv->lock, &irqL);
		if(check_fwstate(pmlmepriv, _FW_LINKED)== _TRUE) 
		{
			indicate_disconnect(padapter); //will clr Linked_state; before this function, we must have chked whether  issue dis-assoc_cmd or not		
		}
		_exit_critical(&pmlmepriv->lock, &irqL);
			
		//todo:wait until fw has process dis-assoc cmd		


		r871x_dev_unload(padapter);		
			
	}
	
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-dev_remove()\n"));

_func_exit_;	

	return;
	
}
#else
//rmmod module & unplug(SurpriseRemoved) will call r871xu_dev_remove() => how to recognize both
static void r871xu_dev_remove(struct usb_interface *pusb_intf)
{		
	struct net_device *pnetdev=usb_get_intfdata(pusb_intf);	
       _adapter *padapter = (_adapter*)netdev_priv(pnetdev);
   
_func_exit_;

	if(padapter)	
	{
		printk("+r871xu_dev_remove\n");
       	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+dev_remove()\n"));		
      		
		if(drvpriv.drv_registered == _TRUE)
		{
			//printk("r871xu_dev_remove():padapter->bSurpriseRemoved == _TRUE\n");
		        padapter->bSurpriseRemoved = _TRUE;	 
		}
		/*else
		{
			//printk("r871xu_dev_remove():module removed\n");
			padapter->hw_init_completed = _FALSE;
		}*/

		if(pnetdev)
			unregister_netdev(pnetdev); //will call netdev_close()

		r871x_dev_unload(padapter);

		usb_put_dev(interface_to_usbdev(pusb_intf));//decrease the reference count of the usb device structure when disconnect

		free_drv_sw(padapter);
		
		//If we didn't unplug usb dongle and remove/insert modlue, driver fails on sitesurvey for the first time when device is up . 
		//Reset usb port for sitesurvey fail issue. 2009.8.13, by Thomas
		if(interface_to_usbdev(pusb_intf)->state != USB_STATE_NOTATTACHED)
			usb_reset_device(interface_to_usbdev(pusb_intf));

		usb_set_intfdata(pusb_intf, NULL);
	}
	
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-dev_remove()\n"));
	printk("-r871xu_dev_remove, hw_init_completed=%d\n", padapter->hw_init_completed);

_func_exit_;	

	return;
	
}
#endif

static int __init r8712u_drv_entry(void)
{
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+r8712u_drv_entry\n"));
	drvpriv.drv_registered = _TRUE;
	return usb_register(&drvpriv.r871xu_drv);	
}

static void __exit r8712u_drv_halt(void)
{
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+r8712u_drv_halt\n"));
	printk("+r8712u_drv_halt\n");
	drvpriv.drv_registered = _FALSE;
	usb_deregister(&drvpriv.r871xu_drv);
	printk("-r8712u_drv_halt\n");
}


module_init(r8712u_drv_entry);
module_exit(r8712u_drv_halt);


/*
init (driver module)-> r8712u_drv_entry
probe (sd device)-> r871xu_drv_init(dev_init)
open (net_device) ->netdev_open
close (net_device) ->netdev_close
remove (sd device) ->r871xu_dev_remove
exit (driver module)-> r8712u_drv_halt
*/


/*
r8711s_drv_entry()
r8711u_drv_entry()
r8712s_drv_entry()
r8712u_drv_entry()
*/

