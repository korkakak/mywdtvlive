/************************************************************************
;
;   (C) Copyright Arphic Technology Corp. 1993-2005
;
;    Title:     Arphic Mobile Font Rasterizer
;
;    Module:    gray render (include file)
;
;    Version:   1.00
;
;    Author:    Koffman Huang
;
;***********************************************************************/
/*EncryptOn*/

#ifndef bi_gray_renderH
#define bi_gray_renderH

#include "bi_render.h"
extern const BI_Raster_Funcs  bi_gray_raster;

#if BI_SUPPORT_K_RENDER
BI_INT32 LineToClosure( BI_INT32 x1, BI_INT32 y1,
                        BI_INT32 x2, BI_INT32 y2,
                        BI_INT32 x3, BI_INT32 y3,
                        BI_INT32 sr, BI_INT32 vr, BI_INT32 hr,
                        BI_INT32* points_x , BI_INT32* points_y,
			BI_INT32 flag,BI_INT32 phase, int oox, int ooy);
#endif

#endif  /* #ifndef bi_gray_renderH */

 
