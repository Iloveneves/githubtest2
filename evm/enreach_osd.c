/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
*-------------------------------------------------------------------------------
* FileName: enreach_osd.c
* Description: 该文件实现enreach浏览器定义的osd接口
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
#include "hi_base.h"
//#include "hi_unf_trd.h"
#include "enreach_pub.h"
#include "ebrowser_api.h"

#include "../tpversion/ui_demo/osd/include/wndId.h"
#include "trd_enreach_init.h"
#include "../tpmiddle/include/app_sys.h"
#include "../tpversion/ui_com_api.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 1

//extern HI_S32 g_Enreach_hWnd;

HI_U8 *evm_osd_buf = NULL;
HI_U8  *osd_tmp_buf = NULL;//[ENREACH_OSD_WIDTH*ENREACH_OSD_HEIGHT*2];
static HI_U8  lock_osd = 0;


HI_VOID OSD_LockOSD(HI_VOID)
{
	lock_osd = 1;
}

HI_VOID OS_UnlockOSD(HI_VOID)
{
	lock_osd = 0;
}

HI_S32 OSD_EnreachGetBufRect(HI_U32 *pu32Width, HI_U32 *pu32Height)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_TRD_OSD_GRAPHICS_S stGraphics;

    ENREACH_RETURN((NULL == pu32Width || NULL == pu32Height), HI_FAILURE);

    s32Ret = HI_TRD_OSD_GetGraphicsData(g_Enreach_hWnd, &stGraphics);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    *pu32Width  = stGraphics.stPixel.u32Width;
    *pu32Height = stGraphics.stPixel.u32Height;
#else
	EVM_INFO("Enter");
	*pu32Width = ENREACH_OSD_WIDTH;
	*pu32Height = ENREACH_OSD_HEIGHT;
#endif

    return HI_SUCCESS;
}

HI_S32 OSD_EnreachGetBufAddr(HI_U32 *pu32ScreenBuf)
{
#if 1
	EVM_INFO("Enter");
	//evm_osd_buf = (HI_U8*)malloc(1024 * 1024 * 2);
	if (evm_osd_buf == NULL)
	{
		//evm_osd_buf = (HI_U8*)malloc(ENREACH_OSD_WIDTH*ENREACH_OSD_HEIGHT*2);
		*pu32ScreenBuf = 0;	
		EVM_ERROR("evm_osd_buf, ERROR");
	}

	EVM_DEBUG("get Buf addr:0x%x", evm_osd_buf);
	*pu32ScreenBuf = (HI_U32)evm_osd_buf;

#endif

	return HI_SUCCESS;
}

void OSD_EnreachFreeBufAddr(void )
{
	EVM_DEBUG("Enter");

#if 0
	EVM_DEBUG("Free evm_osd_buf:0x%x", evm_osd_buf);
	ENREACH_SAFE_FREE(evm_osd_buf);

	EVM_DEBUG("Free osd_tmp_buf:0x%x", osd_tmp_buf);
	ENREACH_SAFE_FREE(osd_tmp_buf);
#endif

}

#endif

/**************************** 浏览器定义的清屏、刷屏接口 ************************/

void Callback_cleanWindow(unsigned char* framebuffer, unsigned short win_x,
                          unsigned short win_y, unsigned short win_w, unsigned short win_h)
{
	EVM_INFO("Enter, x:%d, y:%d, w:%d, h:%d",win_x, win_y, win_w,win_h);

	// (HI_VOID)HI_TRD_OSD_ClearScreen(g_Enreach_hWnd, win_x, win_y, win_w, win_h, 0x0);
	//FYF_API_OSD_FillRect(win_x, win_y, win_w, win_h, (0x1f>>3)<<10|(0x1f>>3)<<5|(0x1f>>3));
	GUI_API_FillRect(GUI_API_GetDesktopWindow(), win_x, win_y, win_w,win_h, (0x1f>>3)<<10|(0x1f>>3)<<5|(0x1f>>3));


	return;
}


void Callback_updateScreen(unsigned char* src, unsigned char* dst, int srcX,
                           int srcY, int srcW, int srcH, int dstX, int dstY)
{

	EVM_ERROR("Enter, src:0x%x, dst:0x%x, srcX:%d, srcY:%d, srcW:%d, srcH:%d, dstX:%d, dstY:%d", src,dst,srcX,srcY,srcW,srcH,dstX,dstY);


#if 0
	FYF_API_gdi_draw_image(dstX, dstY, srcW, srcH, src+ENREACH_OSD_WIDTH*2+srcX*2, ENREACH_OSD_WIDTH);
	FYF_API_OSD_Refresh(dstX, dstY, srcW, srcH);
#endif

#if 0
	{
		FILE*  file;
		file = fopen("/root/bmp.dat", "wb");
		fwrite(src, 720*576*2, 1, file);		
		fclose(file);
	}
#endif

#if 0	
	FYF_API_OSD_WriteRectangleData(0, 0, ENREACH_OSD_WIDTH, ENREACH_OSD_HEIGHT, src);
	FYF_API_OSD_Refresh(0, 0, ENREACH_OSD_WIDTH, ENREACH_OSD_HEIGHT);
#endif
	
#if 0
    EVM_INFO("### call Callback_updateScreen fun! ### \n");

    (HI_VOID)HI_TRD_OSD_UpdateSrceen(g_Enreach_hWnd, HI_FALSE, srcX, srcY, srcW, srcH);

    return;
#else
	#if 1
	HI_U32 i = 0;
	HI_U8 *p = src + ENREACH_OSD_WIDTH*2*srcY + srcX*2;
	HI_U8 *temp;
	HI_U32 x_offset = (720 - ENREACH_OSD_WIDTH)/2;
	HI_U32 y_offset = (576 - ENREACH_OSD_HEIGHT)/2;

	if (HI_TRD_ENREACH_IsExit() || lock_osd)
	{
		EVM_DEBUG("Exit process");
		return;
	}

	if (src == NULL)
	{
		if (evm_osd_buf == NULL)
		{
			Callback_cleanWindow(NULL, dstX-x_offset,  dstY-y_offset, srcW, srcH);
		}
		else
		{
			Callback_updateScreen(evm_osd_buf, NULL, srcX-x_offset, srcY-y_offset,  srcW, srcH, dstX-x_offset, dstY-y_offset);
		}
		
		return;
	}

	if (srcW == ENREACH_OSD_WIDTH && srcH == ENREACH_OSD_HEIGHT)
	{
		EVM_DEBUG("Full Display");
		GUI_API_DrawBitmap( GUI_API_GetDesktopWindow(), dstX+x_offset, dstY+y_offset, srcW, srcH, src);	
		FYF_API_OSD_Refresh(dstX+x_offset, dstY+y_offset, srcW, srcH);

		if (APP_Audio_API_GetMuteState())
		{
			EVM_DEBUG("mute status");
			GUI_ENC_API_SetVisible(ID_STA_MUTE_EVM, 0x00000001L);
			GUI_ENC_API_Update(ID_STA_MUTE_EVM,  1);
		}

		return;
	}
	

	if (osd_tmp_buf == NULL)
	{
		EVM_ERROR("no enough memory");
		return ;
	}

	temp = osd_tmp_buf;
	
	for (i=0; i<srcH; i++)
	{
		memcpy(temp, p, srcW*2);
		temp += srcW*2;
		p += ENREACH_OSD_WIDTH*2;
	}
	
	//FYF_API_OSD_WriteRectangleData(dstX+x_offset, dstY+y_offset, srcW, srcH, osd_tmp_buf);
	GUI_API_DrawBitmap( GUI_API_GetDesktopWindow(), dstX+x_offset, dstY+y_offset, srcW, srcH, osd_tmp_buf);	
	//free(osd_tmp_buf);
	FYF_API_OSD_Refresh(dstX+x_offset, dstY+y_offset, srcW, srcH);

	/* set mute icon*/
	if ((dstX+x_offset >= 573 && dstX+x_offset <= 573+51) 
		|| (dstX + x_offset + srcW >= 573 && dstX + x_offset + srcW <= 573+51)
		|| (dstY + y_offset >= 63 && dstY+y_offset <= 63+34)
		|| (dstY + y_offset + srcH >= 63 && dstY + y_offset + srcH <= 63+34))
	{
		if (APP_Audio_API_GetMuteState())
		{
			EVM_DEBUG("mute status");
			GUI_ENC_API_SetVisible(ID_STA_MUTE_EVM, 0x00000001L);
			GUI_ENC_API_Update(ID_STA_MUTE_EVM,  1);
		}
	}


	#endif
#endif

}

HI_VOID OSD_DrawLogo(HI_VOID)
{
	EVM_DEBUG("OSD_DrawLogo");
	
	GUI_API_FillRect(GUI_API_GetDesktopWindow(), 0, 0, 720,572, 0);

}

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

