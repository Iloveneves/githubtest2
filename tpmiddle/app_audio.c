#include "app_common.h"
#include "app_ipi_common.h"

extern BU32 g_AudioDecoderID;
extern BU32 g_AudioOutput;

extern BS32 e2h_g_MuteState;

#define APP_Audio_Printf FYF_API_printf

BU32 APP_Audio_API_GetActualVolume(BS32 vol_offset, BS32 chn_vol)
{
	BS32 actual_vol;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	if (app_system_data.volume_save_type == 0)//分台
	{
		actual_vol = vol_offset + chn_vol;
	}
	else
	{
		actual_vol = vol_offset + app_system_data.sysVolume;
		chn_vol = app_system_data.sysVolume;
	}
	
	if (chn_vol)
	{
		if (actual_vol > 0)
		{
			if (actual_vol > 32)
			{
				actual_vol = 64;
			}
			else
			{
				actual_vol *= 2;
			}
		}
		else
		{
			actual_vol = 1;
		}
	}
	else
	{
		actual_vol = 0;
	}
	APP_PRINT_DEBUG("actual_vol = %d", actual_vol);
	return (BU32)actual_vol;
	
}
/*-------------------------------------------------------------------------------

	Description:设置声音volume音量大小(0~100)

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Audio_API_SetVolume(BU08 Volume)
{
	BU32 volume;
	volume = Volume;
	
	volume = volume*3/2;
	if(volume==96)
		volume=100;
		
	if(volume > 90)
	{
		APP_Audio_Printf("ERROR:APP_Audio_API_SetVolume = %d over\n",volume);
		volume = 90;
		
	}
	FYF_API_aout_ioctl(g_AudioOutput,FYF_AOUT_SET_VOLUME,&volume);
}

/*-------------------------------------------------------------------------------

	Description:设置声道

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Audio_API_SetTrack(APP_CHN_TRACK_E track)
{
	APP_CHN_TRACK_E Track;
	Track = track;
	if(Track > APP_CHN_TRACK_SINGLE_e)
	{
		APP_Audio_Printf("ERROR:APP_Audio_API_SetTrack = %d over\n",track);
		Track = APP_CHN_TRACK_SINGLE_e;		
	}
	FYF_API_adec_ioctl(g_AudioDecoderID, FYF_ADEC_SET_CHANNEL_MODE, &track);
}


/*-------------------------------------------------------------------------------

	Description:bMute !=0 静音

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Audio_API_SetMuteState(BU08 bMute)
{
	BU32 tnsValue = 0;
	static BU32 muteinit = 0;
#ifdef USE_FZVOD
	if (bMute == APP_AUDIO_MUTE_STATE)
	{
		ipanel_SetAudMuteState(FYF_TRUE);
	}
	else if (bMute == APP_AUDIO_UNMUTE_STATE)
	{
		ipanel_SetAudMuteState(FYF_FALSE);
	}
#endif
/*	if(muteinit == 0)
	{
		muteinit = 1;
	}
	else if(bMute == app_system_data.g_MuteState)
	{
		return;
	}
	*/
	if(bMute)
	{
		app_system_data.g_MuteState = APP_AUDIO_MUTE_STATE;
		tnsValue = app_system_data.g_MuteState;
		FYF_API_adec_ioctl(g_AudioDecoderID, FYF_ADEC_SET_MUTE, &tnsValue);
	}
	else
	{	
		app_system_data.g_MuteState = APP_AUDIO_UNMUTE_STATE;
		tnsValue = app_system_data.g_MuteState;
		FYF_API_adec_ioctl(g_AudioDecoderID, FYF_ADEC_SET_MUTE, &tnsValue);
	}

	APP_SYS_API_SaveSysInfo ();
}

/*-------------------------------------------------------------------------------

	Description:获得当前静音状态

	Parameters:

-------------------------------------------------------------------------------*/
BU08 APP_Audio_API_GetMuteState(void)
{
	return (BU08)(app_system_data.g_MuteState);
}


