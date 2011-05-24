
#ifndef __HAL_INIT_H__
#define __HAL_INIT_H__

#include <drv_conf.h>
#include <osdep_service.h>
#include <drv_types.h>

uint rtl871x_hal_init(_adapter *padapter);
uint rtl871x_hal_deinit(_adapter *padapter);
void rtl871x_hal_stop(_adapter *padapter);

#ifdef CONFIG_RTL8711
#include "rtl8711_hal.h"
#endif

#ifdef CONFIG_RTL8712
#include "rtl8712_hal.h"
#endif

#endif //__HAL_INIT_H__

