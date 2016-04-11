#ifndef _APP_AUDIO_API_H_
#define _APP_AUDIO_API_H_



#define APP_AUDIO_UNMUTE_STATE  0
#define APP_AUDIO_MUTE_STATE    1



/*
设置声音volume音量大小(0~100)
*/
void APP_Audio_API_SetVolume(BU08 volume);
/*
设置声道
*/
void APP_Audio_API_SetTrack(APP_CHN_TRACK_E track);
/*
bMute !=0 静音
*/
void APP_Audio_API_SetMuteState(BU08 bMute);
/*
*获得当前静音状态
*/
BU08 APP_Audio_API_GetMuteState(void);
#endif












