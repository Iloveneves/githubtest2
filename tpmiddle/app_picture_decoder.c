/***************************************************************************************/
/*																					   */
/*                                                                                     */
/*                                                                                     */
/*                                                                                     */
/*  2009-08-25 cwd created                                                             */
/*	说明:图片解码的通用接口实现							                               */
/***************************************************************************************/


#include "app_common.h"
#include "app_ipi_common.h"
//#include "hi_stb_api.h"


//一个屏幕显示多少图片
#define APP_Pictures_NUM 9

#define LAYER_WIDTH 720
#define LAYER_HEIGHT 480
static BU32 g_cur_picture_no = 0xffff;
static Picture_Group_S g_picture_group;
static BU32 g_cur_picture_status = APP_PLAYER_STATUS_STOP_e;
FYF_Rect_s stRect;
static BU32 hLayerSurface;
FYF_EFFECT_MODE_E g_effect_mode = FYF_EFFECT_NONE;
static APP_PICTURE_SECONDS_TYPE_E g_order_mode = APP_PICTURE_SECONDS_3S_e;
static APP_PICTURE_SHOW_TYPE_E g_seconds_mode = APP_PICTURE_SHOW_ORDER_e;
static BU32 g_decoder_status = 0;
static BU32 g_effect_seconds[] = {0, 3, 6, 1, 1, 2, 2, 1, 3, 3};
/*
HI_S32 RefreshLayer(HI_VOID)
{
    OSDGAL_Refresh(FYF_API_OSD_GetHandle(), &stRect);

    return HI_SUCCESS;    
}
*/


BS32 MID_PictureDecoder_API_InitPicfile()
{
	g_picture_group.PicFileNum = 0;
	return APP_OK;
}
BS32 MID_PictureDecoder_API_AddPicFile(APP_File_Info_S	*picture_file)
{
	if (picture_file == NULL)
	{
		return APP_ERR;
	}
	memcpy(&(g_picture_group.PictureInfo[g_picture_group.PicFileNum++]), picture_file, sizeof(APP_File_Info_S));
	return APP_OK;
}


BS32 MID_PictureDecoder_API_Start(index)
{

	BS32 ret;
    BU32 hDecoder, hDecSurface;

	FYF_Rect_s RealRect;
	APP_Printf("index = %d, filenum = %d\n", index, g_picture_group.PicFileNum);

	//add by cwd 2009-12-03
	//防止退出全屏浏览后还再继续显示
	if (PictureShow_API_GetShowFlag() == 0)
	{
		APP_PRINT_DEBUG("cant not decodering picture!\n");
		return;
	}
	if (index >= g_picture_group.PicFileNum)
	{
		APP_PRINT_ERROR("picture index %d is over picture num\n", index, g_picture_group.PicFileNum);
		return APP_ERR;
	}
    /** 创建解码器 */
	g_decoder_status = 1;

	ret = FYF_API_JPEG_CreateDecoder(g_picture_group.PictureInfo[index].FileName, &hDecoder);
	ret |= FYF_API_JPEG_DecImageData(hDecoder, 720, 576, FYF_OSD_PF_1555, &hDecSurface);
	ret |= FYF_API_JPEG_DestroyDecoder(hDecoder);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("picture decoer error\n");
	}
	if (g_effect_mode > FYF_EFFECT_NONE)
	{
		//FYF_API_OSD_CreateSurface(720, 576, FYF_OSD_PF_1555, &hMemSurface);
		//FYF_API_OSD_Blit(hDecSurface, NULL, hMemSurface, NULL);
		FYF_API_JPEG_Effect_Play(hDecSurface, hLayerSurface, g_effect_mode);
		//FYF_API_OSD_DestroySurface(hMemSurface);
	}
	else
	{
		RealRect.x = 0;
		RealRect.y = 0;
		RealRect.w = 720;
		RealRect.h = 576;
		if (FYF_OK == FYF_API_OSD_Blit(hDecSurface, NULL, hLayerSurface, &RealRect))
		{
			FYF_API_OSD_Refresh(0, 0, 720, 576);
		}
	}
	FYF_API_OSD_DestroySurface(hDecSurface);

#if 0
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = g_picture_group.PictureInfo[index].FileName;
	APP_Printf("%s\n", g_picture_group.PictureInfo[index].FileName);
    s32Ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != s32Ret)
    {
    	APP_PRINT_ERROR("HI_GO_CreateDecoder fail 0x%x\n", s32Ret);
        return APP_ERR;
    }


    /** 解码到Surface */
    s32Ret  = HI_GO_DecImgData(hDecoder, 0, NULL, &hDecSurface);
    s32Ret |= HI_GO_DestroyDecoder(hDecoder);

	if (s32Ret != HI_SUCCESS)
	{
		APP_PRINT_ERROR("HI_GO_DecImgData fail\n");
		return APP_ERR;
	}

	if (g_effect_mode > HI_EFFECT_NONE)
	{
		s32Ret = HI_Effect_SetSpeed(1);
		s32Ret |= HI_Effect_Play(hDecSurface, hLayerSurface, g_effect_mode);
	}
	else
	{
		RealRect.x = 0;
		RealRect.y = 0;
		RealRect.w = 720;
		RealRect.h = 576;
		s32Ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &RealRect, &stBltOpt);
	    if (HI_SUCCESS == s32Ret)
	    {
	        /** 刷新显示 */
	       OSDGAL_Refresh(FYF_API_OSD_GetHandle(), &stRect);

	    }
		else
		{
			APP_PRINT_ERROR("HI_GO_Blit fail 0x%x\n", s32Ret);
		}
	}
    /** 释放Surface */
    HI_GO_FreeSurface(hDecSurface);
#endif
	g_decoder_status = 0;
	return APP_OK;
	
}

BS32 APP_PictureDecoder_API_Init()
{
	BS32 ret = APP_OK;
	g_cur_picture_no = 0xffff;
	stRect .x = stRect.y = 0;
	stRect.h = FRAME_HEIGHT / 3 - 10;
	stRect.w = FRAME_WIDTH / 3 - 10;
	OSDGAL_GetSurface(FYF_API_OSD_GetHandle(), &hLayerSurface);
    ret = FYF_API_JPEG_Init();

	return ret;
}

BS32 APP_PictureDecoder_API_InitRect()
{
	stRect.x = stRect.y = 0;
	stRect.h = FRAME_HEIGHT / 3 - 10;
	stRect.w = FRAME_WIDTH / 3 - 10;
	return APP_OK;
}

BS32 APP_PictureDecoder_API_Deinit()
{
	HI_Effect_Deinit();
}


BS32 APP_PictureDecoder_API_ShowPicture(index)
{
	BU32 hDecoder, hDecSurface;
	BS32 ret; 
	FYF_Rect_s RealRect;
	if (index >= g_picture_group.PicFileNum)
	{
		APP_PRINT_ERROR("picture index %d is over picture num\n", index, g_picture_group.PicFileNum);
		return APP_ERR;
	}


	ret = FYF_API_JPEG_CreateDecoder(g_picture_group.PictureInfo[index].FileName, &hDecoder);
	ret |= FYF_API_JPEG_DecImageData(hDecoder, 720, 576, FYF_OSD_PF_1555, &hDecSurface);
	ret |= FYF_API_JPEG_DestroyDecoder(hDecoder);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("picture decoer error\n");
	}
	RealRect.x = stRect.x + 45;
	RealRect.y = stRect.y + 131;
	RealRect.w = stRect.w;
	RealRect.h = stRect.h;
	if (FYF_OK == FYF_API_OSD_Blit(hDecSurface, NULL, hLayerSurface, &RealRect))
	{
		FYF_API_OSD_Refresh(0, 0, 720, 576);

		stRect.x = (stRect.x + FRAME_WIDTH / 3) % FRAME_WIDTH;
		if (0 == stRect.x)
		{
		   stRect.y = (stRect.y + FRAME_HEIGHT / 3) % FRAME_HEIGHT;
		}
	}

	FYF_API_OSD_DestroySurface(hDecSurface);
#if 0
    /** 创建解码器 */
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = g_picture_group.PictureInfo[index].FileName;
	APP_Printf("%s\n", g_picture_group.PictureInfo[index].FileName);
    s32Ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != s32Ret)
    {
    	APP_PRINT_ERROR("HI_GO_CreateDecoder fail 0x%x\n", s32Ret);
        return APP_ERR;
    }


    /** 解码到Surface */
    s32Ret  = HI_GO_DecImgData(hDecoder, 0, NULL, &hDecSurface);
    s32Ret |= HI_GO_DestroyDecoder(hDecoder);

	if (s32Ret != HI_SUCCESS)
	{
		APP_PRINT_ERROR("HI_GO_DecImgData fail\n");
		return APP_ERR;
	}
	RealRect.x = stRect.x + 45;
	RealRect.y = stRect.y + 131;
	RealRect.w = stRect.w;
	RealRect.h = stRect.h;
	s32Ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &RealRect, &stBltOpt);
    if (HI_SUCCESS == s32Ret)
    {
        /** 刷新显示 */
       OSDGAL_Refresh(FYF_API_OSD_GetHandle(), &stRect);

  	   stRect.x = (stRect.x + FRAME_WIDTH / 3) % FRAME_WIDTH;
       if (0 == stRect.x)
       {
           stRect.y = (stRect.y + FRAME_HEIGHT / 3) % FRAME_HEIGHT;
       }
    }
	else
	{
		APP_PRINT_ERROR("HI_GO_Blit fail 0x%x\n", s32Ret);
	}

    /** 释放Surface */
    HI_GO_FreeSurface(hDecSurface);
#endif
	g_cur_picture_no = index;
	return APP_OK;
}

BS32 APP_PictureDecoder_API_Stop()
{
	BS32 ret = 0;
	g_decoder_status = 0;
	if (g_effect_mode > FYF_EFFECT_NONE)
	{
		ret =  FYF_API_JPEG_Effect_Stop();
	}
	FYF_API_OSD_FillRect(0, 0, 720, 576, 0x8000);
	return ret;
	
}

BS32 APP_PictureDecoder_API_Start(BU32 index)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (g_decoder_status == 1)
	{
		APP_PictureDecoder_API_Stop();
	}
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ShowPhoto;
	msgSend.q2ndWordOfMsg= index;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	g_cur_picture_no = index;
	
	return APP_OK;
}



BS32 APP_PictureDecoder_API_NextPicture()
{
	if (++g_cur_picture_no >= g_picture_group.PicFileNum)
		g_cur_picture_no = 0;
	return APP_PictureDecoder_API_Start(g_cur_picture_no);
}


BS32 APP_PictureDecoder_API_PrePicture()
{
	if (g_cur_picture_no == 0)
		g_cur_picture_no = g_picture_group.PicFileNum - 1;
	else
		g_cur_picture_no--;
	return APP_PictureDecoder_API_Start(g_cur_picture_no);
	
}

BU32 APP_PictureDecoder_API_GetCount()
{
	return g_picture_group.PicFileNum;
}

BS32 APP_PictureDecoder_API_GetPicName(BU08 *name, BU32 index)
{
	BU32 len;
	BU32 i;
	if (name == NULL)
	{
		return APP_ERR;
	}

	if (index >= g_picture_group.PicFileNum)
	{
		return APP_ERR;
	}
	len = strlen(g_picture_group.PictureInfo[index].FileName);
	for (i = len - 1; i >= 0 ; i--)
		if (g_picture_group.PictureInfo[index].FileName[i] == '/')
			break;
	
	strcpy(name, &(g_picture_group.PictureInfo[index].FileName[i+1]));
	
	//memcpy(name, g_picture_group.PictureInfo[*index].FileName, len);
	return APP_OK;
}


void APP_PictureDecoder_API_SetEffect(BU32 effect_mode)
{
	g_effect_mode = effect_mode;
}


void APP_PictureDecoder_API_SetOrder(BU32 order_mode)
{
	g_order_mode = order_mode;
}

void APP_PictureDecoder_API_SetSeconds(BU32 seconds)
{
	g_seconds_mode = seconds;
}


BU32 APP_PictureDecoder_API_GetEffect()
{
	return g_effect_mode;
}


BU32 APP_PictureDecoder_API_GetOrder()
{
	return g_order_mode ;
}

BU32 APP_PictureDecoder_API_GetSeconds()
{
	return g_seconds_mode;
}

BU32 APP_PictureDecoder_API_GetEffectSeconds()
{
	return (g_seconds_mode + 1) * 3 + g_effect_seconds[g_effect_mode];
}















