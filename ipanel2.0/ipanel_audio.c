/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment of the base Porting APIs needed by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/
//#include <stdio.h>
#include "trd_ipanel_init.h"
#include "ipanel_audio.h"
#include "../tpmiddle/include/app_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#define VOLUME_MAX 100

/******************************************************************/
/*ipanel porting set volume                                       */
/*Input:  vol                                                     */
/*Output&return: memory address pointer if success, other NULL    */
/******************************************************************/

int ipanel_porting_set_volume(int vol) 
{
	IPANEL_ERROR("ipanel set volume %d\n",vol);
	
	FYF_API_aout_ioctl(0, FYF_AOUT_SET_VOLUME, (void *)&vol);
	
	vol = vol / 3;
	if (vol > 32)vol = 32;
	if (vol != 0)
		APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, vol);

	if (vol == 0)
	{
		ipanel_SetAudMuteState(FYF_TRUE);
		APP_Audio_API_SetMuteState(1);
	}
	else
	{
		ipanel_SetAudMuteState(FYF_FALSE);
		APP_Audio_API_SetMuteState(0);
	}

    return FYF_OK;
}


/******************************************************************/
/*ipanel porting get volume                                       */
/*Input:  get audio volume                                        */
/*Output&return: memory address pointer if success, other NULL    */
/******************************************************************/
int ipanel_porting_get_volume(void)
{
   	unsigned int u32value =0;
/*	if (APP_Audio_API_GetMuteState())
	{
		printf("ipanel_porting_get_volume00\n");
		return 0;
	}
	else
	*/
	{
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &u32value);
		u32value = u32value * 3;
		if (u32value > 96)u32value = 100;
	}
	//FYF_API_aout_ioctl(0, FYF_AOUT_GET_VOLUME, (void *)&u32value);
	
	IPANEL_ERROR("ipanel_porting_get_volume, value = %d\n", u32value);
	
    return u32value;
}

/******************************************************************/
/*ipanel porting get volume                                       */
/*Input:  set audio decode mode                                   */
/*Output&return: memory address pointer if success, other NULL    */
/******************************************************************/
int ipanel_porting_set_audio_mode(int audiomode)
{
	FYF_ADEC_CHANNEL_OUT_MODE_e arg;
	
    IPANEL_DEBUG("audiomode %d\n", audiomode);    
	arg = (FYF_ADEC_CHANNEL_OUT_MODE_e)audiomode;
	FYF_API_adec_ioctl(0, FYF_ADEC_SET_CHANNEL_MODE, &arg);
    return FYF_OK;
}

/******************************************************************/
/*ipanel porting get volume                                       */
/*Input:  get audio decode mode                                   */
/*Output&return: memory address pointer if success, other NULL    */
/******************************************************************/
int ipanel_porting_get_audio_mode(void)
{
    BU32 audiomode = 0;
	IPANEL_DEBUG("ipanel_porting_get_audio_mode\n");
	
	FYF_API_adec_ioctl(0, FYF_ADEC_GET_CHANNEL_MODE, &audiomode);

    return (BS32)audiomode;
}

/******************************************************************/
/*ipanel porting get volume                                       */
/*Input:  set audio language                                      */
/*Output&return: memory address pointer if success, other NULL    */
/******************************************************************/
int ipanel_porting_audio_set_language(int track)
{
    /*目前我们没有语言接口*/
    return FYF_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */


