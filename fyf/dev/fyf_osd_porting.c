/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*                                          								  */	
/******************************************************************************/ 

#include "hi_stb_api.h"
#include "hi_osdgal.h"

#include "fyf_def.h"

#include "dev/fyf_osd.h"
#include "dev/fyf_gdi.h"

static HI_OSDGAL_HANDLE OSD1_Handle;

HI_HANDLE hMemSurface = 0xffffffff;
HI_HANDLE hLaySurface = 0xffffffff;


BS32 FYF_API_OSD_Open()
{
	OSDGAL_OpenOSDLayer(OSDGAL_LAYER1, &OSD1_Handle);
	OSDGAL_Enable(OSD1_Handle,OSDGAL_ON);
}

BS32 FYF_API_OSD_Close()
{
	OSDGAL_CloseOSDLayer(OSD1_Handle);
}

HI_OSDGAL_HANDLE FYF_API_OSD_GetHandle()
{
	return OSD1_Handle;
}

BS32 FYF_API_OSD_DrawPixel(BU32 x, BU32 y, BU32 c)
{
	if (c != ((0xff>>3)<<10 | (0xff>>3)))
		c |= 0x8000;
	return OSDGAL_DrawPixel(OSD1_Handle, x, y, c);
}

BS32 FYF_API_OSD_ReadPixel(BU32 x, BU32 y, BU08 *data)
{
	return OSDGAL_GetPixel(OSD1_Handle, x, y, data);
}

BS32 FYF_API_OSD_DrawLine(BU32 x1, BU32 y1, BU32 x2, BU32 y2, BU32 c)
{
	c |= 0x8000;
	return OSDGAL_DrawLine(OSD1_Handle, x1, y1, x2, y2, c);
}


BS32 FYF_API_OSD_FillRect(BU32 x, BU32 y, BU32 w, BU32 h, BU32 c)
{
	HI_OSDGAL_RECT_S rect;	
	rect.u32x = x;
	rect.u32y = y;
	rect.u32Width = w;
	rect.u32Height = h;
	return OSDGAL_FillRectangle(OSD1_Handle, &rect, c);

}
BS32 FYF_API_OSD_WriteRectangleData(BU32 x, BU32 y, BU32 w, BU32 h, void *buf)
{
	HI_OSDGAL_RECT_S rect;
	rect.u32x = x;
	rect.u32y = y;
	rect.u32Width = w;
	rect.u32Height = h;
	return OSDGAL_WriteRectangleData(OSD1_Handle, &rect, w * h, buf);
}

BS32 FYF_API_OSD_ReadRectangleData(BU32 x, BU32 y, BU32 w, BU32 h, void* buf)
{
	HI_OSDGAL_RECT_S rect;
	rect.u32x = x;
	rect.u32y = y;
	rect.u32Width = w;
	rect.u32Height = h;
	return OSDGAL_ReadRectangleData(OSD1_Handle, &rect, w * h, buf);
}

BS32 FYF_API_OSD_WriteRectangleDataMask (BU32 x, BU32 y, BU32 w, BU32 h, void *buf, BU32 maskcolor)
{
	HI_OSDGAL_RECT_S rect;
	rect.u32x = x;
	rect.u32y = y;
	rect.u32Width = w;
	rect.u32Height = h;
	return OSDGAL_WriteRectangleDataMask(OSD1_Handle, &rect, w * h, buf, maskcolor);
}

BS32 FYF_API_OSD_Refresh(BU32 x, BU32 y, BU32 w, BU32 h)
{
	HI_OSDGAL_RECT_S rect;
	rect.u32x = x;
	rect.u32y = y;
	rect.u32Width = w;
	rect.u32Height = h;
	return OSDGAL_Refresh(OSD1_Handle,&rect);
}


BS32 FYF_API_OSD_CreateSurface(BS32 width, BS32 height, FYF_OSD_PF_E PixelFormat, BU32 *hMemSurface)
{
	BS32 ret;
	ret = HI_GO_CreateSurface(width, height, PixelFormat, hMemSurface);
	if(ret != HI_SUCCESS)
	{
		printf(" HI_GO_CreateSurface error %x \n",ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_OSD_DestroySurface(BU32 hMemSurface)
{
	BS32 ret;
	ret = HI_GO_FreeSurface(hMemSurface);
	if(ret != HI_SUCCESS)
	{
		printf(" HI_GO_DestroyLayer error %x \n",ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_OSD_Blit(BU32 handleSrc, FYF_Rect_s *prectSrc, BU32 handleDst, FYF_Rect_s *prectDst)
{
	HI_S32 s32Ret;
	HIGO_BLTOPT_S stBltOpt = {HI_FALSE, HIGO_COMPOPT_NONE, HIGO_CKEY_NONE,
                              HI_FALSE, HIGO_ROP_BLACK,    HI_TRUE,        HIGO_ROTATE_NONE,HIGO_MIRROR_NONE};
	s32Ret = HI_GO_Blit(handleSrc, prectSrc, handleDst, prectDst, &stBltOpt);
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_GO_Blit error %x\n", s32Ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_IPI_Get_SurfaceBuff(HI_U32 * SurfaceBuffAddr)
{
	BS32 ret = 0;
	HI_PIXELDATA data;
	
	if(SurfaceBuffAddr == 0)
		return HI_FAILURE;

	if(hLaySurface == 0xffffffff)
		OSDGAL_GetSurface(OSD1_Handle, &hLaySurface);

	
	if(hMemSurface == 0xffffffff)
	{
		ret = HI_GO_CreateSurface(640, 526, HIGO_PF_1555, &hMemSurface);
		if(ret != 0)
		{
			printf(" HI_GO_CreateSurface error %x \n",ret);
			return HI_FAILURE;
		}
	    ret = HI_GO_LockSurface(hMemSurface, data);
	    if(ret)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
	        ret = HI_GO_FreeSurface (hMemSurface);
			hMemSurface = 0xffffffff;
	        return HI_FAILURE;
	    }
		*SurfaceBuffAddr= data[0].pData;
	    ret = HI_GO_UnlockSurface(hMemSurface);
	    if(ret)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
	        ret = HI_GO_FreeSurface (hMemSurface);
			hMemSurface = 0xffffffff;
	        return HI_FAILURE;
	    }

	}
	else
	{

	    ret = HI_GO_LockSurface(hMemSurface, data);
	    if(ret)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
	        ret = HI_GO_FreeSurface (hMemSurface);
			hMemSurface = 0xffffffff;
	        return HI_FAILURE;
	    }
	    *SurfaceBuffAddr= data[0].pData;
	    ret = HI_GO_UnlockSurface(hMemSurface);
	    if(ret)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
	        ret = HI_GO_FreeSurface (hMemSurface);
			hMemSurface = 0xffffffff;
	        return HI_FAILURE;
	    }	

	}
	
	return HI_SUCCESS;
}

BS32 FYF_CPI_gdi_get_info_porting(BS32 *width,BS32 *height,void **pBuffer,BS32 *bufWidth,BS32 *bufHeight)
{
	BU32 Buf;
	*width  = 640;
    *bufWidth  = 640;
    *height = 526;
    *bufHeight = 526;
	FYF_IPI_Get_SurfaceBuff(&Buf);
	*pBuffer = (void*)Buf;
	
}

BS32 FYF_CPI_gdi_draw_image_porting(BS32 x, BS32 y, BS32 w, BS32 h, BU08 *bits, BS32 w_src)
{
	HI_RECT stRect;
	HI_RECT dstRect;
	HIGO_BLTOPT_S stBlitOpt;
	BU32 ret;
	HI_PIXELDATA data;

	stRect.x = x;
	stRect.y = y;   
	stRect.w = w;
	stRect.h = h;

	dstRect.x = x + 35;
	dstRect.y = y + 25;   
	dstRect.w = w;
	dstRect.h = h;

	memset(&stBlitOpt, 0, sizeof(HIGO_BLTOPT_S));
	ret = HI_GO_Blit(hMemSurface, &stRect, hLaySurface, &dstRect, &stBlitOpt);
	if (HI_SUCCESS != ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
        return HI_FAILURE;
    }
//	HI_GO_LockSurface(hLaySurface, data);
//	HI_GO_UnlockSurface(hLaySurface);

	return OSDGAL_Refresh(OSD1_Handle,&stRect);
}

BS32 FYF_CPI_gdi_ioctl_porting(FYF_GDI_IOCTL_e op, void *arg)
{

}

FYF_GDI_PORTING FYF_gdi_porting =
{
	FYF_CPI_gdi_get_info_porting,
	NULL,
	FYF_CPI_gdi_draw_image_porting,
	NULL,
	FYF_CPI_gdi_ioctl_porting		
};


BS32 FYF_API_OSD_Init()
{
	FYF_API_gdi_register(&FYF_gdi_porting);

	return OSDGAL_Init();
}





