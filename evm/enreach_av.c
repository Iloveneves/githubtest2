/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_av.c
* Description: 该文件实现enreach浏览器定义的av播放接口
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
//#include "hi_unf_trd.h"
//#include "hi_svr_public.h"

#include "fyf_def.h"
#include "fyf_com.h"

#include "app_common.h"
#include "app_com.h"
#include "app_ipi_common.h"

#include "enreach_pub.h"
#include "enreach_erro.h"
#include "ebrowser_api.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 0
HI_S32 AV_SetVol(HI_S32 s32Vol)
{
    HI_S32 s32Ret = 0;

    s32Ret = HI_TRD_AV_SetVol(s32Vol);

    return s32Ret;
}

HI_S32 AV_GetVol(HI_S32 *ps32Vol)
{
    HI_S32 s32Ret = 0;
    
    ENREACH_RETURN((NULL == ps32Vol), HI_FAILURE);
    s32Ret = HI_TRD_AV_GetVol(ps32Vol);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), HI_FAILURE);

    return HI_SUCCESS;
}
#endif

/**************************** 浏览器定义的视频播放接口 *************************/

void Callback_setSmallWindow(unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h)
{
#if 0
    HI_TRD_RECT stWinRect;

    stWinRect.x = win_x;
    stWinRect.y = win_y;
    stWinRect.w = win_w;
    stWinRect.h = win_h;
    
    HI_TRD_AV_SetOutputWindow(&stWinRect);

    return;
#else

	FYF_Rect_s fyfRect;

	EVM_INFO("enter, x:%u, y:%u, w:%d, h:%d", win_x, win_y, win_w, win_h);

    	fyfRect.h = win_h;
	fyfRect.w = win_w;
	fyfRect.x = win_x;
	fyfRect.y = win_y;
	
    	FYF_API_vout_ioctl(0,FYF_VOUT_SET_WIN_LOCATION, (void *)&fyfRect);
		
#endif
}

char Callback_PlayTV(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int vid, unsigned int aid, unsigned int pcrid)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_U32 u32Qam = TUNER_MODULATION_QAM64;

    if (64 == qam)
    {
        u32Qam = TUNER_MODULATION_QAM64;
    }

    ENREACH_DEBUG("### freq = %d, symbolrate = %d, qam = %d, vid = %d, aid = %d, pcrpid = %d\n",
        freq, SymbolRate, qam, vid, aid, pcrid);
    
    s32Ret = HI_TRD_TUNER_Lock(0, freq, SymbolRate * 1000, u32Qam, NULL);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    s32Ret = HI_TRD_AV_StartByPid(aid, vid, pcrid);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    return HI_SUCCESS;
#else
	BS32 ret = 0;
	
	EVM_INFO("enter");

	#if 0	
	qam = FYF_TUNER_MOD_QAM64;
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	ret = FYF_API_tuner_lock_delivery(0, (BS32)(freq * 1000), (BS32)SymbolRate, (BS32)(qam), (BS32)freq);
	 if (ret != FYF_OK)
	 {
	     FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
	     return 1;
	 }
	 APP_IPI_SaveTunerInfo((BS32)freq, (BS32)SymbolRate, (BS32)(qam));
	 FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
	#else
	if (freq != 0)
	{
		ret = Ebrowser_SetTuner(freq, SymbolRate, qam);
		ENREACH_RETURN((ret != 0), ret);
	}
	#endif

	 ret = APP_IPI_PlayProg(vid, aid, pcrid, FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
	 ENREACH_RETURN((ret != 0), 1);

	return FYF_OK;
	 
#endif

}

char Callback_PlayTV_Srv(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int service_id)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_U32 u32Qam = TUNER_MODULATION_QAM64;

    ENREACH_DEBUG("### freq = %d, symbolrate = %d, qam = %d, service_id = %d \n",
        freq, SymbolRate, qam, service_id);

    if (64 == qam)
    {
        u32Qam = TUNER_MODULATION_QAM64;
    }

    s32Ret = HI_TRD_TUNER_Lock(0, freq, SymbolRate * 1000, u32Qam, NULL);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    s32Ret = HI_TRD_AV_StartBySvrId(service_id);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    return HI_SUCCESS;
#else
     BS32 ret = 0;

	EVM_DEBUG("Enter, freq:%u, symbolRate:%u, qam:%u, serId:%u", freq, SymbolRate, qam, service_id);


	if (freq != 0)
	{
		#if 0
		qam = FYF_TUNER_MOD_QAM64;
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
		ret = FYF_API_tuner_lock_delivery(0, (BS32)(freq * 1000), (BS32)SymbolRate, (BS32)(qam), (BS32)freq);
		if (ret != FYF_OK)
		{
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
			return 1;
		}
		APP_IPI_SaveTunerInfo((BS32)freq, (BS32)SymbolRate, (BS32)(qam));
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		#else
		Ebrowser_SetTuner(freq, SymbolRate, qam);
		#endif
		
	}

 
     return FYF_OK;
	 
#endif
}

char Callback_PlayTV_Pmt(unsigned int freq, unsigned int SymbolRate,unsigned int qam, unsigned int pmt_pid)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_U32 u32Qam = TUNER_MODULATION_QAM64;

    ENREACH_DEBUG("### freq = %d, symbolrate = %d, qam = %d, pmt_pid = %d \n",
        freq, SymbolRate, qam, pmt_pid);

    if (64 == qam)
    {
        u32Qam = TUNER_MODULATION_QAM64;
    }

    s32Ret = HI_TRD_TUNER_Lock(0, freq, SymbolRate * 1000, u32Qam, NULL);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    s32Ret = HI_TRD_AV_StartByPmtId(pmt_pid);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    return HI_SUCCESS;
#else
	EVM_DEBUG("Enter, freq:%u, symbolRate:%u, qam:%u, pmt_pid:%u", freq, SymbolRate, qam, pmt_pid);
	return FYF_OK;
#endif
}

char Callback_StopTV(unsigned char* url, int type)
{
#if 0
    HI_S32 s32Ret = 0;

    ENREACH_ERROR("### call Callback_StopTV func! ### \n");
    s32Ret = HI_TRD_AV_AVStop();
    return s32Ret;
#else
	EVM_INFO("Enter, url:%s, type:0x%x", url, type);
	APP_Com_IPI_StopProg(1);
	return FYF_OK;
#endif

}

int CallBack_TvFullScreen()
{
#if 0
    HI_S32 s32Ret = 0;
    HI_TRD_VID_E eMode;
    HI_TRD_RECT stWinRect;
    
    s32Ret = HI_TRD_AV_GetVideoMode(&eMode);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), s32Ret);

    if (HI_TRD_VID_NTSC == eMode)
    {
        stWinRect.x = UI_BASEVIDEO_WND_NTSC_X;
        stWinRect.y = UI_BASEVIDEO_WND_NTSC_Y;
        stWinRect.w = UI_BASEVIDEO_WND_NTSC_W;
        stWinRect.h = UI_BASEVIDEO_WND_NTSC_H;
    }
    else
    {
        stWinRect.x = UI_BASEVIDEO_WND_PAL_X;
        stWinRect.y = UI_BASEVIDEO_WND_PAL_Y;
        stWinRect.w = UI_BASEVIDEO_WND_PAL_W;
        stWinRect.h = UI_BASEVIDEO_WND_PAL_H;
    }

    s32Ret = HI_TRD_AV_SetOutputWindow(&stWinRect);

    return s32Ret;
#else
	EVM_INFO("enter");
	APP_Com_API_SetFullScreen();
	return FYF_OK;
	
#endif

}

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

