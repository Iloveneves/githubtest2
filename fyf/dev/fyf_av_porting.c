/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author caiwende 2009/08/03                                           	  */
/******************************************************************************/

#include <pthread.h>
#include <stdlib.h>
#include "fyf_def.h"
#include "hi_stb_api.h"
#include "dev/fyf_adec.h"

#define __FYF_ADEC_PORTING_
#ifdef  __FYF_ADEC_PORTING_

HI_HANDLE s_AVPLAYHandle;
static BU32 fyf_avdec_semaphore;
static BU08 AdecOpenDone = FYF_FALSE;
static HI_HANDLE AoHandle;

/******************************************************************************/
/*Description: adec open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_CPI_adec_open_porting(void)
{

	HI_S32 s32ret;
	HI_UNF_AVPLAY_ATTR_S 	struAttrConfig;
	s32ret = HI_UNF_AVPLAY_GetDefaultConfig(&struAttrConfig, HI_UNF_AVPLAY_STREAM_TYPE_TSI);

	s32ret |= HI_UNF_AVPLAY_Create(&struAttrConfig, &s_AVPLAYHandle);
	if (HI_FAILURE == s32ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_Create failed +++\n");
	}
	s32ret = HI_UNF_AVPLAY_ChnOpen(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
	return FYF_OK;
}
/******************************************************************************/
/*Description: adec close                                                     */
/*Input      : decoder            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_adec_close_porting(BU32 decoder)
{
	/*关闭音频通道*/
    HI_UNF_AVPLAY_ChnClose(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
	return FYF_OK;
}
/******************************************************************************/
/*Description: adec ioctl                                                     */
/*Input      : decoder,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_adec_ioctl_porting(BU32 decoder, FYF_ADEC_IOCTL_e op, void *arg)
{
	BU32 * value = (BU32 *)arg;
	BS32 ret = FYF_OK;
	BU32 audio_pid;
	HI_UNF_AVPLAY_DEBUG_INFO_S stDebugInfo;
	BU32 audio_frame;
	
	switch(op)
	{
    case FYF_ADEC_SET_SOURCE:
		break;
    case FYF_ADEC_START:
		
		FYF_API_semaphore_wait (fyf_avdec_semaphore, FYF_WAIT_FOREVER);
		FYF_CPI_demux_get_pid(0, &audio_pid);
		if (audio_pid == 0)
			audio_pid = 0x1fff;

		ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &audio_pid);
		if (HI_SUCCESS != ret)
		{
			printf("\n +++ HI_UNF_AVPLAY_SetAttr audio pid f%d ailed %x +++\n", audio_pid,ret);
		}
		ret =HI_UNF_AVPLAY_Start(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
		if (HI_SUCCESS != ret)
		{
			printf("\n +++ HI_UNF_AVPLAY_Start audio failed %x +++\n", ret);
		}
		
		FYF_API_semaphore_release (fyf_avdec_semaphore);
		break;
		
	case FYF_ADEC_STOP:
		FYF_API_semaphore_wait (fyf_avdec_semaphore, FYF_WAIT_FOREVER);
		ret = HI_UNF_AVPLAY_Stop(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
		FYF_API_semaphore_release (fyf_avdec_semaphore);
		break;
    case FYF_ADEC_PAUSE:
		break;
    case FYF_ADEC_RESUME:
		break;
    case FYF_ADEC_CLEAR:
		break;
    case FYF_ADEC_SYNCHRONIZE:
		break;
    case FYF_ADEC_SET_CHANNEL_MODE:
		switch(*value)
		{
		case FYF_AUDIO_MODE_STEREO:
			ret = HI_UNF_SND_SetTrackMode(AoHandle, HI_UNF_TRACK_MODE_STEREO);
			break;
		case FYF_AUDIO_MODE_LEFT_MONO:
			ret = HI_UNF_SND_SetTrackMode(AoHandle, HI_UNF_TRACK_MODE_DOUBLE_LEFT);
			break;
		case FYF_AUDIO_MODE_RIGHT_MONO:
			ret = HI_UNF_SND_SetTrackMode(AoHandle, HI_UNF_TRACK_MODE_DOUBLE_RIGHT);
			break;
		case FYF_AUDIO_MODE_MIX_MONO:
			ret = HI_UNF_SND_SetTrackMode(AoHandle, HI_UNF_TRACK_MODE_DOUBLE_MONO);
			break;
		case FYF_AUDIO_MODE_STEREO_REVERSE:
			break;			
		default:
			break;
		}
		if (ret != HI_SUCCESS)
		{
			printf("HI_UNF_SND_SetTrackMode errcode = %x\n", ret);
		}
		break;
    case FYF_ADEC_GET_CHANNEL_MODE:
		ret = HI_UNF_SND_GetTrackMode(AoHandle,(HI_UNF_TRACK_MODE_E *)value);
		if (ret != HI_SUCCESS)
		{
			printf("HI_UNF_SND_GetTrackMode errcode = %x\n", ret);
		}

		break;
    case FYF_ADEC_SET_MUTE:
				
		if(FYF_DISABLE == *value)
		{
			HI_UNF_SND_SetAnalogMute(AoHandle, HI_FALSE);
			HI_UNF_SND_SetDigitalMute(AoHandle, HI_FALSE);
		}
		else if(FYF_ENABLE == *value)
		{
			HI_UNF_SND_SetAnalogMute(AoHandle, HI_TRUE);
			HI_UNF_SND_SetDigitalMute(AoHandle, HI_TRUE);
		}		
		break;
	
    case FYF_ADEC_GET_MUTE:
		HI_UNF_SND_GetAnalogMute(AoHandle, (HI_BOOL *)value);
		break;

    case FYF_ADEC_SET_PASS_THROUGH:
		break;
    case FYF_ADEC_SET_VOLUME:
		break;
    case FYF_ADEC_GET_BUFFER_RATE:
		break;
	case FYF_ADEC_STREAM_INPUT:
		/*ret = HI_UNF_AVPLAY_GetDebugInfo(s_AVPLAYHandle, &stDebugInfo);
		audio_frame = stDebugInfo.stAcodecInfo.u32DecFrameNum;
		FYF_API_thread_sleep(50);
		ret = HI_UNF_AVPLAY_GetDebugInfo(s_AVPLAYHandle, &stDebugInfo);
		
		if (audio_frame != stDebugInfo.stAcodecInfo.u32DecFrameNum)
		{
			*value = FYF_TRUE;
		}
		else
		{
			*value = FYF_FALSE;
		}
		printf("stSyncStatus %d %d\n", stDebugInfo.stAcodecInfo.u32DecFrameNum, audio_frame);
		*/
		break;
	default:
		ret = FYF_ERR;
		break;
	}
ADEC_ERR:
	return ret;
}
FYF_ADEC_PORTING FYF_adec_porting=
{
	FYF_CPI_adec_open_porting,
	FYF_CPI_adec_close_porting,
	FYF_CPI_adec_ioctl_porting	
};
#endif


#define __FYF_AOUT_PORTING_
#ifdef  __FYF_AOUT_PORTING_
#include "dev/fyf_aout.h"


/******************************************************************************/
/*Description: aout open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_CPI_aout_open_porting(void)
{

	HI_UNF_SND_INIT_PARA_S sndinitpara;

	if (HI_SUCCESS != HI_UNF_SND_Create(&sndinitpara, &AoHandle))
	{
		printf("\n +++ HI_UNF_VO_CreateLayer failed +++\n");
		
	}


	return HI_SUCCESS;
}
/******************************************************************************/
/*Description: aout close                                                     */
/*Input      : handle            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_aout_close_porting(BU32 handle)
{
}
/******************************************************************************/
/*Description: aout ioctl                                                     */
/*Input      : handle,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_aout_ioctl_porting(BU32 handle, FYF_AOUT_IOCTL_e op, void *arg)
{
	BU32 * value = (BU32 *)arg;
	BU32 ret;

	switch(op)
	{
    case FYF_AOUT_SET_OUTPUT:
		
		break;
    case FYF_AOUT_SET_VOLUME:
		ret = HI_UNF_SND_SetVolume(AoHandle, *value);
		if (ret != HI_SUCCESS)
		{
			printf("HI_UNF_SND_SetVolume error 0x%x\n", ret);
		}
		
		break;
	
    case FYF_AOUT_GET_VOLUME:
		ret = HI_UNF_SND_GetVolume(AoHandle, value);
		if (ret != HI_SUCCESS)
		{
			printf("HI_UNF_SND_GetVolume error 0x%x\n", ret);
		}
		
		break;

    case FYF_AOUT_SET_BALANCE:
		break;
	default:
		ret = FYF_ERR;
		break;
	}
AOUT_ERR:
	return ret;
}
FYF_AOUT_PORTING FYF_aout_porting=
{
	FYF_CPI_aout_open_porting,
	FYF_CPI_aout_close_porting,
	FYF_CPI_aout_ioctl_porting
};
#endif

#define __FYF_VOUT_PORTING_
#ifdef  __FYF_VOUT_PORTING_
#include "dev/fyf_vout.h"
static BU08 VoutOpendone = FYF_FALSE;
static HI_HANDLE VoHandle = 0, DispHandle = 0;
BU08 bPAL = FYF_TRUE;
/******************************************************************************/
/*Description: vout open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_CPI_vout_open_porting(void)
{
	BS32 s32ret;
	HI_UNF_DISP_INIT_PARA_S sDispPara;
	HI_UNF_VO_INIT_PARA_S	sVoInitPara;
	HI_UNF_VO_ATTACH_PARA_S para;
	HI_UNF_AVPLAY_ATTR_S 	struAttrConfig;
	HI_UNF_SND_INIT_PARA_S sndinitpara;
	if (!VoutOpendone)
	{

		if (HI_SUCCESS != HI_UNF_VO_CreateLayer(&sVoInitPara, &VoHandle))
		{
			printf("\n +++ HI_UNF_VO_Create failed +++\n");
		}
		HI_UNF_VO_SetEnable(VoHandle, FYF_TRUE);
		
		if (HI_SUCCESS != HI_UNF_DISP_Create(&sDispPara, &DispHandle))
		{
			printf("\n +++ HI_UNF_DISP_Create failed +++\n");
		}
		
		//HI_UNF_SND_SetVolume(AoHandle, 255);
		if (HI_SUCCESS != HI_UNF_SND_Attach(AoHandle, s_AVPLAYHandle, 100))
		{
			printf("\n +++ HI_UNF_SND_Attach failed +++\n");
		}
		para.eSourceType = HI_UNF_VO_SOURCE_TYPE_AVPLAY;
   	 	HI_UNF_VO_Attach(VoHandle, s_AVPLAYHandle, &para);
		HI_UNF_VO_SetAspectCvrs(VoHandle, HI_UNF_ASPECT_CVRS_IGNORE);
		VoutOpendone = FYF_TRUE;
	
	}
	return  FYF_OK;
}
/******************************************************************************/
/*Description: vout close                                                     */
/*Input      : display            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_vout_close_porting(BU32 display)
{
}
/******************************************************************************/
/*Description: vout open window                                               */
/*Input      : display,type        										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_CPI_vout_open_window_porting(BU32 display, BS32 type)
{
	
}
/******************************************************************************/
/*Description: vout close window                                              */
/*Input      : display,window      										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_vout_close_window_porting(BU32 display, BU32 window)
{
	
}

/******************************************************************************/
/*Description: vout ioctl                                                     */
/*Input      : display,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_vout_ioctl_porting(BU32 display, FYF_VOUT_IOCTL_e op, void *arg)
{
	BU32 * value = (BU32 *)arg;
	FYF_Rect_s * rect;
	HI_UNF_WINDOW_S eWindow;
	BS32 ret = FYF_OK;
	HI_HANDLE hlayer;
	HI_UNF_OUTPUT_INTERFACE_S 	stDacMode;
	BU32 Alpha;
	DMX_CHAN_HANDLE hChannelHandle;
	switch(op)
	{
    case FYF_VOUT_SELECT_DEV:
		#if 1
		switch(* value)
		{
		case FYF_VIDEO_OUTPUT_YUV:
			stDacMode.enDacMode[0] = HI_UNF_DAC_MODE_PB;
			stDacMode.enDacMode[1] = HI_UNF_DAC_MODE_Y;
			stDacMode.enDacMode[2] = HI_UNF_DAC_MODE_PR;
			stDacMode.enDacMode[3] = HI_UNF_DAC_MODE_CVBS;
				
			ret =  HI_UNF_DISP_SetDacMode(DispHandle, &stDacMode);
			break;
		case FYF_VIDEO_OUTPUT_SVIDEO:
			stDacMode.enDacMode[0] = HI_UNF_DAC_MODE_SILENCE;
			stDacMode.enDacMode[1] = HI_UNF_DAC_MODE_SVIDEO_Y;
			stDacMode.enDacMode[2] = HI_UNF_DAC_MODE_SVIDEO_C;
			stDacMode.enDacMode[3] = HI_UNF_DAC_MODE_CVBS;
				
			ret =  HI_UNF_DISP_SetDacMode(DispHandle, &stDacMode);
			break;
		default:
			break;
		}
		#endif
		break;
    case FYF_VOUT_ENABLE_DEV:
		break;
    case FYF_VOUT_SET_MODE:
		switch(*value)
		{
		
		case FYF_VOUT_TVENC_AUTO:
			ret = HI_UNF_DISP_SetFormat(DispHandle, HI_UNF_ENC_FMT_AUTO);
			OSDGAL_GetOSDLayerHandle(FYF_API_OSD_GetHandle(), &hlayer);
			ret = HI_GO_SetScreenSize(hlayer, 720, 576);
			printf("==================HI_UNF_ENC_FMT_AUTO=========================\n");
			bPAL = 1;
			break;
		case FYF_VOUT_TVENC_NTSC:
			ret = HI_UNF_DISP_SetFormat(DispHandle, HI_UNF_ENC_FMT_NTSC);
			OSDGAL_GetOSDLayerHandle(FYF_API_OSD_GetHandle(), &hlayer);
			ret = HI_GO_SetScreenSize(hlayer, 720, 480);
			printf("==================HI_UNF_ENC_FMT_NTSC=========================\n");
			bPAL = 0;
			break;
		case FYF_VOUT_TVENC_PAL:
			ret = HI_UNF_DISP_SetFormat(DispHandle, HI_UNF_ENC_FMT_PAL);
			OSDGAL_GetOSDLayerHandle(FYF_API_OSD_GetHandle(), &hlayer);
			ret = HI_GO_SetScreenSize(hlayer, 720, 576);
			printf("==================HI_UNF_ENC_FMT_PAL=========================\n");
			bPAL = 1;
			break;
		case FYF_VOUT_TVENC_SECAM:
			ret = HI_UNF_DISP_SetFormat(DispHandle, HI_UNF_ENC_FMT_SECAM_SIN);
			printf("==================HI_UNF_ENC_FMT_SECAM_SIN=========================\n");
			break;
		defalut:
			printf("Error vout mode");
			break;
					
		}
		if (ret != HI_SUCCESS)
		{
			printf("HI_UNF_DISP_SetFormat errorcode = %x\n", ret);
			return FYF_ERR;
		}
		break;
		
    case FYF_VOUT_SET_VISABLE:
		switch(*value)
		{
		case FYF_ENABLE:
			//ret = HI_UNF_VO_SetEnable(VoHandle, HI_TRUE);

			break;
		case FYF_DISABLE:
			HI_UNF_DMX_GetVideoChannelHandle(0, &hChannelHandle);
			HI_UNF_DMX_CloseChannel(hChannelHandle);
			HI_VID_FlushBuffer(VID_FLUSH_DISPLAY_BUFFER);
			//ret = HI_UNF_VO_SetEnable(VoHandle, HI_FALSE);
			break;
		default:
			break;
		}
		break;
		
    case FYF_VOUT_SET_ASPECT_RATIO:
		switch(*value)
		{
		case FYF_VOUT_SET_AR_16TO9:
			ret = HI_UNF_VO_SetAspectRatio(VoHandle, HI_UNF_ASPECT_RATIO_16TO9);
			break;
		case FYF_VOUT_SET_AR_4TO3:
			ret = HI_UNF_VO_SetAspectRatio(VoHandle, HI_UNF_ASPECT_RATIO_4TO3);
			break;
		default:
			break;
		}
		
		if (ret != HI_SUCCESS)
		{
			printf("HI_UNF_VO_SetAspectRatio errorcode = %x\n", ret);
		}
		break;
		
    case FYF_VOUT_SET_WIN_LOCATION:
		rect = (FYF_Rect_s *)arg;
    	//	printf("===================x = %d y = %d w = %d h = %d \n ",rect->x,rect->y,rect->w,rect->h);

		if(rect->x > 720 || rect->y > 576 || rect->w > 720 || rect->h > 576)
		{
			return FYF_ERR;
		}	

		if(((rect->x + rect->w) > 720) || ((rect->y + rect->h) > 576))
		{
			return FYF_ERR;
		}
		 HI_UNF_VO_GetInputWindow(VoHandle, &eWindow);
		// printf("===================input x = %d y = %d w = %d h = %d %d %d\n ",eWindow.u32X,eWindow.u32Y,eWindow.u32Width,eWindow.u32Height, eWindow.u32BaseHeight, eWindow.u32BaseWidth);
		/*if (rect->h == 252)
		{
			eWindow.u32X = rect->x;
		    eWindow.u32Y = rect->y;

			eWindow.u32Width = 338;
		    eWindow.u32Height = 270;
		    eWindow.u32BaseWidth = 720;
		    eWindow.u32BaseHeight = 576;

		}
		else
		*/
		{
			eWindow.u32X = rect->x;
		    eWindow.u32Y = rect->y;

			eWindow.u32Width = rect->w;
		    eWindow.u32Height = rect->h;// * 4 / 5;
		    eWindow.u32BaseWidth = 720;
		    eWindow.u32BaseHeight = 576;
		}
		if (rect->x == 0 && rect->y == 0 && rect->w == 720 && rect->h == 576)
		{
			HI_UNF_VO_SetAspectCvrs(VoHandle, HI_UNF_ASPECT_CVRS_LETTERBOX);
		}
		else
		{
			HI_UNF_VO_SetAspectCvrs(VoHandle, HI_UNF_ASPECT_CVRS_IGNORE);
		}
/*		if (!bPAL)
		{
			eWindow.u32Y = eWindow.u32Y * 480 / 576;
			eWindow.u32Height = eWindow.u32Height * 480 / 576;
			eWindow.u32BaseHeight = 480;
		}
*/
		ret = HI_UNF_VO_SetOutputWindow(VoHandle, &eWindow);
	    if(HI_SUCCESS != ret)
	    {
	        printf("HI_UNF_VO_SetInputWindow error:%x\n",ret);
	        return FYF_ERR;
	    }

		break;
    case FYF_VOUT_SET_WIN_TRANSPARENT:
		printf("Alpha value = %d\n", *value);
		HI_UNF_VO_GetAlpha(VoHandle, &Alpha);
	//	HI_UNF_VO_SetAlpha(VoHandle, 100 - (*value)*10);
	//	printf("VoAlpha = %d\n", Alpha);
		if (HI_SUCCESS != OSDGAL_SetAlpha(FYF_API_OSD_GetHandle(), 255 - 15 * (*value)))
		{
			printf("HI_UNF_VO_SetAlpha errorcode = %x\n", ret);
			return FYF_ERR;
		}

		break;
    case FYF_VOUT_SET_CONTRAST:
		break;
    case FYF_VOUT_SET_HUE:
		break;
    case FYF_VOUT_SET_BRIGHTNESS:
		break;
   	case FYF_VOUT_SET_SATURATION:
		break;
    case FYF_VOUT_SET_SHARPNESS:
		break;
	default:
		break;
	}

	return FYF_OK;
}
FYF_VOUT_PORTING FYF_vout_porting=
{
	FYF_CPI_vout_open_porting,
	FYF_CPI_vout_close_porting,
	FYF_CPI_vout_open_window_porting,
	FYF_CPI_vout_close_window_porting,
	FYF_CPI_vout_ioctl_porting
};
#endif


#define __FYF_VDEC_PORTING_
#ifdef  __FYF_VDEC_PORTING_
#include "dev/fyf_vdec.h"

/******************************************************************************/
/*Description: vdec open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_CPI_vdec_open_porting(void)
{

	HI_UNF_AVPLAY_ChnOpen(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
    return 0;

}
/******************************************************************************/
/*Description: vdec close                                                     */
/*Input      : decoder            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_vdec_close_porting(BU32 decoder)
{
	

    /*关闭视频通道*/
    HI_UNF_AVPLAY_ChnClose(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

}


#define MENULOGO_ID 0x02FD0100
BU32 fyf_vdec_start_flag = 0;
BU08 fyf_adec_start = 0;
extern unsigned char acLogo_radio[];
extern int logo_len;
extern unsigned char aclogo_nosignal[];
extern int logo_nosignal_len;
extern unsigned char music_logo[];
extern unsigned int music_logo_len;
extern BU32 	 LogoFileTotalLength;
extern BU08     *data_bitmap_Logo;
//extern BU08 data_logo_buf[1024*64];
extern BU32 OldLogoFileTotalLength;
extern unsigned char aclogo_start[];
extern unsigned int logo_start_len;


/******************************************************************************/
/*Description: vdec ioctl                                                     */
/*Input      : decoder,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_CPI_vdec_ioctl_porting(BU32 decoder, FYF_VDEC_IOCTL_e op, void *arg)
{
	BU32 * value = (BU32 *)arg;
	BS32 ret = FYF_OK;
	HI_UNF_AVPLAY_SYNC_ATTR_S syncPara;
	HI_UNF_AVPLAY_STOP_OPT_S StopAttr;

	BU32 video_pid;
	BU32 addr=0, len=0;
	HI_UNF_AVPLAY_I_FRAME_S stFrame;
	HI_UNF_AVPLAY_DEBUG_INFO_S stDebugInfo;
	BU32 video_frame;
	    DMX_CHAN_HANDLE hChannelHandle;


	switch(op)
	{
   	case FYF_VDEC_SET_SOURCE:
		break;
    case FYF_VDEC_START:

		FYF_API_semaphore_wait (fyf_avdec_semaphore, FYF_WAIT_FOREVER);

		FYF_CPI_demux_get_pid(1, &video_pid);
		if (video_pid == 0)
			video_pid = 0x1fff;
		//printf("video_pid = %d,\n", video_pid);
		ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &video_pid);
		if (HI_SUCCESS != ret)
		{
			printf("\n +++ HI_UNF_AVPLAY_SetAttr video pid f%d ailed %x +++\n", video_pid,ret);
		}
		
		//ret |= HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_PCR_PID, &pcr_pid);
	    ret = HI_UNF_AVPLAY_GetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &syncPara);
		syncPara.enSyncRef = HI_UNF_AVPLAY_SYNC_REF_AUDIO;
    	ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &syncPara);
		if (HI_SUCCESS != ret)
		{
			printf("\n +++ HI_UNF_AVPLAY_SetAttr failed %x +++\n",ret);
		}


		ret = HI_UNF_AVPLAY_Start(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
		if (HI_SUCCESS != ret)
		{
			printf("\n +++ HI_UNF_AVPLAY_Start video failed %x+++\n", ret);
		}
		FYF_API_semaphore_release (fyf_avdec_semaphore);
		break;
    case FYF_VDEC_STOP:
		FYF_API_semaphore_wait (fyf_avdec_semaphore, FYF_WAIT_FOREVER);
		printf("value = %d\n", *value);
		StopAttr.s32Timeout = 0;
		if (*value == 0)
			StopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
		else 
			StopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
		ret |=HI_UNF_AVPLAY_Stop(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &StopAttr);
		if (HI_SUCCESS != ret)
		{
			printf("\n +++ HI_UNF_AVPLAY_Stop failed +++\n");
		}
		FYF_API_semaphore_release (fyf_avdec_semaphore);
		break;
    case FYF_VDEC_PAUSE:
		break;
    case FYF_VDEC_RESUME:
		break;
    case FYF_VDEC_CLEAR:
		break;
    case FYF_VDEC_SYNCHRONIZE:
		break;
    case FYF_VDEC_GET_BUFFER_RATE:
		break;
	case FYF_VDEC_BITMAP:
		FYF_API_semaphore_wait (fyf_avdec_semaphore, FYF_WAIT_FOREVER);
		if (*value == 0)
		{
			
			//HI_UNF_DMX_GetAudioChannelHandle(0, &hChannelHandle);
			//HI_UNF_DMX_CloseChannel(hChannelHandle);
			HI_UNF_DMX_GetVideoChannelHandle(0, &hChannelHandle);
			HI_UNF_DMX_CloseChannel(hChannelHandle);
			ret = HI_VID_FlushBuffer(VID_FLUSH_DISPLAY_BUFFER);
			printf("HI_UNF_AVPLAY_Reset ret = 0x%x\n", ret);
			ret = HI_UNF_AVPLAY_Reset(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
		}
		else
		{
			if (*value == 1)
			{
				stFrame.enType = HI_UNF_VCODEC_TYPE_MPEG2;
				if (OldLogoFileTotalLength == 0)
				{
					stFrame.pu8Addr = acLogo_radio;
					stFrame.u32BufSize = logo_len;
				}
				else
				{
					stFrame.pu8Addr = acLogo_radio;
					stFrame.u32BufSize = logo_len;
					//stFrame.pu8Addr = data_logo_buf;
					//stFrame.u32BufSize = OldLogoFileTotalLength;
				}

			}
			else if (*value == 2)
			{
				stFrame.enType = HI_UNF_VCODEC_TYPE_MPEG2;
				stFrame.pu8Addr = aclogo_nosignal;
				stFrame.u32BufSize = logo_nosignal_len;
			}
			else if (*value == 3)
			{
				stFrame.enType = HI_UNF_VCODEC_TYPE_MPEG2;
				stFrame.pu8Addr = aclogo_start;
				stFrame.u32BufSize = logo_start_len;
			}
			else if (*value == 4)
			{
				stFrame.enType = HI_UNF_VCODEC_TYPE_MPEG2;
				stFrame.pu8Addr = music_logo;
				stFrame.u32BufSize = music_logo_len;
			//	printf("music_logo_len = %d %d \n", logo_len, logo_nosignal_len);
			}
			else if (*value == 5)//显示新的开机LOGO
			{
				stFrame.enType = HI_UNF_VCODEC_TYPE_MPEG2;
				stFrame.pu8Addr = acLogo_radio;
				stFrame.u32BufSize = logo_len;
			//	stFrame.pu8Addr = data_bitmap_Logo;
			//	stFrame.u32BufSize = LogoFileTotalLength;
			//	printf("logo_len = %d\n",LogoFileTotalLength);
			}
			else
			{
				stFrame.pu8Addr = 0;
				stFrame.u32BufSize = 0;
			}

			if (stFrame.pu8Addr != 0 && stFrame.u32BufSize != 0)
			{
				HI_UNF_ENC_FMT_E fmt;
				HI_UNF_DMX_GetVideoChannelHandle(0, &hChannelHandle);
				HI_UNF_DMX_CloseChannel(hChannelHandle);
				ret = HI_UNF_DISP_GetFormat(DispHandle,&fmt);
				printf("HI_UNF_DISP_GetFormat: %x----%d\n", ret, fmt);
				ret = HI_UNF_DISP_SetFormat(DispHandle, fmt);
				ret = HI_UNF_AVPLAY_DecodeIFrame(s_AVPLAYHandle, &stFrame);					
				printf("HI_UNF_AVPLAY_DecodeIFrame ret = 0x%x\n", ret); 
				//printf("HI_UNF_DISP_SetFormat ret = 0x%x\n", ret); 
			}

		}
		FYF_API_semaphore_release (fyf_avdec_semaphore);
		break;
	case FYF_VDEC_STATE:
		
		break;
	case FYF_VDEC_STREAM_INPUT:
		ret = HI_UNF_AVPLAY_GetDebugInfo(s_AVPLAYHandle, &stDebugInfo);
		video_frame = stDebugInfo.stVcodecInfo.u32TotalFrame;
		FYF_API_thread_sleep(50);
		ret = HI_UNF_AVPLAY_GetDebugInfo(s_AVPLAYHandle, &stDebugInfo);
		
		if (video_frame != stDebugInfo.stVcodecInfo.u32TotalFrame)
		{
			*value = FYF_TRUE;
		}
		else
		{
			*value = FYF_FALSE;
		}
		//printf("stSyncStatus %d %d\n", stDebugInfo.stVcodecInfo.u32TotalFrame, video_frame);
		
		break;

	default:
		ret = FYF_ERR;
		break;
	}
VDEC_ERR:
	return ret;
}
FYF_VDEC_PORTING FYF_vdec_porting=
{
	FYF_CPI_vdec_open_porting,
	FYF_CPI_vdec_close_porting,
	FYF_CPI_vdec_ioctl_porting	
};
#endif

static BU64 ptsValue = 0;

#define USE_HI3110

BU64 FYF_API_GetPtsValue(void)
{
#ifdef USE_HI3110
	return ptsValue;
#else
	HI_UNF_AVPLAY_STATUS_INFO_S pstStatusInfo;
	BU32 pts = 0;

	HI_UNF_AVPLAY_GetStatusInfo(s_AVPLAYHandle, &pstStatusInfo);
	pts = pstStatusInfo.stSyncStatus.u32LastVidPts / 1000;
	//printf("pts = %d\n", pts);
	return pts;
#endif

}

void FYF_API_SetPtsValue(BU64 pts)
{
	ptsValue = pts;
}

BU64 FYF_API_GetPts_CB(HI_HANDLE Handle, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para)
{
    HI_UNF_VO_FRAMEINFO_S *frame_info;
    static int i = 0;
	BU64 pts = 0;
    frame_info = (HI_UNF_VO_FRAMEINFO_S *)u32Para;

    if (HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME ==  enEvent)
    {
		pts = (BU64)((frame_info->u32Pts)/90000);
		
		FYF_API_SetPtsValue(pts);
        //printf("\033[0;31m======video frame %d: pts:%lld, y:0x%x\033[0m\n",i++, frame_info->u32Pts,  frame_info->u32YAddr);
		//return (pts);
    }


	//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  pts = %x\n", infopara.u32Pts);
    return 0;

}

void FYF_API_GetPts()
{
#ifdef USE_HI3110
	HI_UNF_AVPLAY_RegisterEvent(s_AVPLAYHandle, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME, FYF_API_GetPts_CB);
#else

#endif

}


BS32 FYF_API_AV_Freeze(void)
{
#ifdef USE_HI3110
	return HI_VID_Freeze();
#else
	return HI_UNF_VO_SetFreeze(FYF_TRUE);
#endif
}


BS32 FYF_API_AV_Resume(void)
{
#ifdef USE_HI3110
	return HI_VID_Resume();
#else
	return HI_UNF_VO_SetFreeze(FYF_FALSE);
#endif

}

HI_U8		mp4Head[] = {	0x00 , 0x00 , 0x01 , 0xb0 , 0xf4 , 0x00 , 0x00 , 0x01 , 0xb5 , 0x09 , 0x00 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00 , 0x01 , 0x20 , 0x00 , 0x86 , 0xc4 , 0x00 , 0x67 , 0x0c , 0x5a , 0x11 , 0x20 , 0x51 , 0x8f, 0x00};
HI_U8		mp4aHead[] = {  0x12,  0x10	};

void FYF_API_InitRegist()
{
	HI_UNF_AVPLAY_UnRegisterEvent(s_AVPLAYHandle,HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);

}


void FYF_API_av_register(void)
{
	BU32 index;

#ifdef  __FYF_ADEC_PORTING_
	FYF_API_adec_register(&FYF_adec_porting);
#endif

#ifdef  __FYF_AOUT_PORTING_
	FYF_API_aout_register(&FYF_aout_porting);
#endif

#ifdef  __FYF_VDEC_PORTING_
	fyf_avdec_semaphore = FYF_API_semaphore_create ("vdec", 1, FYF_THREAD_WAIT_FIFO);

	FYF_API_vdec_register(&FYF_vdec_porting);
#endif

#ifdef  __FYF_VOUT_PORTING_
	FYF_API_vout_register(&FYF_vout_porting);
#endif

}

BS32 FYF_API_av_detach()
{
	HI_S32 ret;
	HI_UNF_DMX_ATTR_S stDmxAttr;
#if 0
    ret = HI_UNF_SND_Detach(AoHandle, s_AVPLAYHandle);
	if (ret != HI_SUCCESS)
	{
		printf("\n +++ FYF_API_av_detach  HI_UNF_SND_Detach failed +++ 0x%x\n", ret);
	}
    /*VO解绑定*/
    ret = HI_UNF_VO_Detach(VoHandle, s_AVPLAYHandle);

	if (ret != HI_SUCCESS)
	{
		printf("\n +++ FYF_API_av_detach  HI_UNF_VO_Detach failed +++ 0x%x\n", ret);
	}
#endif
    /*关闭视频通道*/
    ret = HI_UNF_AVPLAY_ChnClose(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

	 /*关闭视频通道*/
    ret |= HI_UNF_AVPLAY_ChnClose(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

//	ret |= HI_UNF_AVPLAY_Destroy(s_AVPLAYHandle);
	if (ret != HI_SUCCESS)
	{
		printf("\n +++ FYF_API_av_detach failed +++ 0x%x\n", ret);
	}

	stDmxAttr.u32PortId = HI_UNF_DMX_PORT_MODE_RAM;
	ret = HI_UNF_DMX_SetAttr(0, &stDmxAttr);	
	if (HI_FAILURE == ret)
	{
		printf("\n +++ HI_UNF_DMX_SetAttr failed +++ 0x%x\n", ret);
	}

}

BS32 FYF_API_av_attach()
{
	HI_S32 s32ret, ret;
	HI_UNF_AVPLAY_ATTR_S 	struAttrConfig;
	HI_UNF_VO_ATTACH_PARA_S para;
	HI_UNF_DMX_ATTR_S stDmxAttr;
	BU32 video_pid = 0x1fff;
	BU32 audio_pid = 0x1fff;
	HI_UNF_AVPLAY_SYNC_ATTR_S syncPara;
	HI_UNF_AVPLAY_STOP_OPT_S StopAttr;
    syncPara.enSyncRef = HI_UNF_AVPLAY_SYNC_REF_AUDIO;
	
	stDmxAttr.u32PortId = HI_UNF_DMX_PORT_MODE_EXTERNAL;
	s32ret = HI_UNF_DMX_SetAttr(0, &stDmxAttr);

/*
	s32ret = HI_UNF_AVPLAY_GetDefaultConfig(&struAttrConfig, HI_UNF_AVPLAY_STREAM_TYPE_TSI);

	s32ret |= HI_UNF_AVPLAY_Create(&struAttrConfig, &s_AVPLAYHandle);
	if (HI_FAILURE == s32ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_Create failed +++\n");
	}
	printf("s_AVPLAYHandle = %d\n", s_AVPLAYHandle);
*/
    /*关闭视频通道*/
    ret = HI_UNF_AVPLAY_ChnOpen(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);

	 /*关闭视频通道*/
    ret |= HI_UNF_AVPLAY_ChnOpen(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);

	 if (ret != HI_SUCCESS)
	{
		printf("\n +++ FYF_API_av_attach HI_UNF_AVPLAY_ChnOpen failed +++ 0x%x\n", ret);
	}

	HI_UNF_SND_Attach(AoHandle, s_AVPLAYHandle, 100);
	para.eSourceType = HI_UNF_VO_SOURCE_TYPE_AVPLAY;
    /*VO解绑定*/
    HI_UNF_VO_Attach(VoHandle, s_AVPLAYHandle, &para);


#if 0 
	ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &audio_pid);
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_SetAttr audio pid f%d ailed %x +++\n", audio_pid,ret);
	}
	ret =HI_UNF_AVPLAY_Start(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_Start audio failed %x +++\n", ret);
	}

 	ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &video_pid);
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_SetAttr video pid f%d ailed %x +++\n", video_pid,ret);
	}
	
	//ret |= HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_PCR_PID, &pcr_pid);
	ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &syncPara);
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_SetAttr failed %x +++\n",ret);
	}


	ret = HI_UNF_AVPLAY_Start(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_AVPLAY_Start video failed %x+++\n", ret);
	}
#endif
	
	ret = HI_UNF_AVPLAY_Stop(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
	StopAttr.s32Timeout = 0;
	StopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
	ret |=HI_UNF_AVPLAY_Stop(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &StopAttr);
	
	
	 


	
}

