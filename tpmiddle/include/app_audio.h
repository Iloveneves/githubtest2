#ifndef _APP_AUDIO_API_H_
#define _APP_AUDIO_API_H_



#define APP_AUDIO_UNMUTE_STATE  0
#define APP_AUDIO_MUTE_STATE    1



/*
��������volume������С(0~100)
*/
void APP_Audio_API_SetVolume(BU08 volume);
/*
��������
*/
void APP_Audio_API_SetTrack(APP_CHN_TRACK_E track);
/*
bMute !=0 ����
*/
void APP_Audio_API_SetMuteState(BU08 bMute);
/*
*��õ�ǰ����״̬
*/
BU08 APP_Audio_API_GetMuteState(void);
#endif












