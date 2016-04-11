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
#include "fyf_def.h"
#include "dev/fyf_osd.h"
#include "jpeg/fyf_jpeg.h"

HI_S32 RefreshLayer(HI_VOID)
{
	HI_RECT rect;
	rect.x = rect.y = 0;
	rect.h = 576;
	rect.w = 720;
	
    OSDGAL_Refresh(FYF_API_OSD_GetHandle(), &rect);

    return HI_SUCCESS;    
}

BS32 FYF_API_JPEG_Init()
{
	BS32 ret;
    ret = HI_Effect_Init(RefreshLayer, HI_FALSE);
	return ret;
}

BS32 FYF_API_JPEG_Effect_Play(BU32 srcSurface, BU32 desSurface, FYF_EFFECT_MODE_E effect_mode)
{
	HI_S32 s32Ret;
	s32Ret = HI_Effect_SetSpeed(1);
	s32Ret |= HI_Effect_Play(srcSurface, desSurface, effect_mode);

	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_Effect_Play fail 0x%x\n", s32Ret);
        return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_JPEG_Effect_Stop()
{
	HI_S32 s32Ret;
	s32Ret = HI_Effect_Stop();
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_Effect_Stop fail 0x%x\n", s32Ret);
        return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_JPEG_CreateDecoder(BU08 *filename, BU32 *pDecoder)
{
	HIGO_DEC_ATTR_S stSrcDesc;
	HIGO_DEC_IMGINFO_S imgInfo;
	HI_S32 s32Ret;

	stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = filename;
	s32Ret = HI_GO_CreateDecoder(&stSrcDesc, pDecoder);
    if (HI_SUCCESS != s32Ret)
    {
    	printf("HI_GO_CreateDecoder fail 0x%x\n", s32Ret);
        return FYF_ERR;
    }
/*
	HI_GO_DecImgInfo(*pDecoder, 0, &imgInfo);
	printf("imgInfo = %d %d %d\n", imgInfo.Format, imgInfo.Width, imgInfo.Height);
	*/
	return FYF_OK;
}


BS32 FYF_API_JPEG_CreateDecoderFromMem(BU32 *pDecoder, BU08 *pAddr, BU32 length)
{
	HIGO_DEC_ATTR_S stSrcDesc;
	HIGO_DEC_IMGINFO_S imgInfo;
	HI_S32 s32Ret;

	stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_MEM;
    stSrcDesc.SrcInfo.MemInfo.pAddr = pAddr;
	stSrcDesc.SrcInfo.MemInfo.Length = length;
	s32Ret = HI_GO_CreateDecoder(&stSrcDesc, pDecoder);
    if (HI_SUCCESS != s32Ret)
    {
    	printf("HI_GO_CreateDecoder fail 0x%x\n", s32Ret);
        return FYF_ERR;
    }
	return FYF_OK;
}

BS32 FYF_API_JPEG_DecImageData(BU32 hDecoder, BU32 width, BU32 height, FYF_OSD_PF_E PixelFormat, BU32 *pDecSurface)
{
	HI_S32 s32Ret;
	HIGO_DEC_IMGATTR_S imagetype;
	imagetype.Height = height;
	imagetype.Width = width;
	imagetype.Format = PixelFormat;
	s32Ret  = HI_GO_DecImgData(hDecoder, 0, &imagetype, pDecSurface);
	
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_GO_DecImgData fail\n");
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_JPEG_DestroyDecoder(BU32 hDecoder)
{
	HI_S32 s32Ret;
	s32Ret = HI_GO_DestroyDecoder(hDecoder);
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_GO_DestroyDecoder fail\n");
		return FYF_ERR;
	}
	return FYF_OK;
}
