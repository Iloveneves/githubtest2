#ifndef _H_APP_PLAYER_H
#define _H_APP_PLAYER_H

#define APP_PLAYER_UNMUTE_STATE  0
#define APP_PLAYER_MUTE_STATE    1
#define APP_PLAYER_NAME_MAX 32

#define APP_PLAY_SPEED_NORMAL 1024
/*-----------------------------------------------
��������
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_TRACK_STEREO_e = 0, //������
	APP_PLAYER_TRACK_LEFT_e,       //������
	APP_PLAYER_TRACK_RIGHT_e,      //������
	APP_PLAYER_TRACK_SINGLE_e,      //������
	APP_PLAYER_TRACK_BUTT_e
}APP_PLAYER_TRACK_E;

/*-----------------------------------------------
��������
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_TYPE_NULL_e = 0,
	APP_PLAYER_TYPE_MP3_e, 		//MP3����
	APP_PLAYER_TYPE_PVR_e,     //PVR��Ƶ����
	APP_PLAYER_TYPE_AVI_e,     //AVI��Ƶ����
	APP_PLAYER_TYPE_BUTT_e
}APP_PLAYER_TYPE_E;

/*-----------------------------------------------
����������
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_GROUP_TYPE_ALL_e = 0, //����������
	APP_PLAYER_GROUP_TYPE_FAV_e     //ϲ��������
}APP_PLAYER_GROUP_TYPE_E;

/*-----------------------------------------------
����������
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_FILEATTR_TYPE_FILENAME_e = 0,//��ȡ�ļ����ļ���
	APP_PLAYER_FILEATTR_TYPE_TIMES_e,       //��ȡ�ļ����ܲ���ʱ��
	APP_PLAYER_FILEATTR_TYPE_CURTIME_e,     //��ȡ�ļ��ĵ�ǰ����ʱ��
	APP_PLAYER_FILEATTR_TYPE_ARTIST_e,      //��ȡ�ļ����ݳ���
	APP_PLAYER_FILEATTR_TYPE_SONGNAME_e,    //��ȡ������
	APP_PLAYER_FILEATTR_TYPE_SPECIAL_e,		//��ȡ������ר����
	APP_PLAYER_FILEATTR_TYPE_CURSTATUS_e		//��ǰMP3������״̬
}APP_PLAYER_FILEATTR_TYPE_E;

/*-----------------------------------------------
����״̬
-------------------------------------------------*/

typedef enum tagAPP_PLAYER_SPEED_E
{
	APP_PLAYER_SPEED_32X_FAST_BACKWORD = 0,
	APP_PLAYER_SPEED_16X_FAST_BACKWORD  = 1,
	APP_PLAYER_SPEED_8X_FAST_BACKWORD   ,
	APP_PLAYER_SPEED_4X_FAST_BACKWORD   ,
	APP_PLAYER_SPEED_2X_FAST_BACKWORD  ,
	APP_PLAYER_SPEED_NORMAL             ,
	APP_PLAYER_SPEED_2X_FAST_FORWORD    ,
	APP_PLAYER_SPEED_4X_FAST_FORWORD    ,
	APP_PLAYER_SPEED_8X_FAST_FORWORD    ,
	APP_PLAYER_SPEED_16X_FAST_FORWORD   ,
	APP_PLAYER_SPEED_32X_FAST_FORWORD   ,
	APP_PLAYER_SPEED_BUTT
} APP_PLAYER_SPEED_E;

typedef enum
{
	APP_PLAYER_STATUS_STOP_e = 0,
	APP_PLAYER_STATUS_PLAY_e,
	APP_PLAYER_STATUS_PAUSE_e,
	APP_PLAYER_STATUS_BUTT_e
}APP_PLAYER_STATUS_TYPE_E;


typedef struct _Player_Time_S
{
	BU08 hour;
	BU08 min;
	BU08 sec;
}Player_Time_S;

typedef struct _Player_Mp3_S
{
	BU08 filename[FILE_FULLNAME_LEN];
	BU08 mp3name[APP_PLAYER_NAME_MAX];
	BU08 mp3singer[APP_PLAYER_NAME_MAX];
	BU08 mp3special[APP_PLAYER_NAME_MAX];
	FYF_Time_Data_S filemtime; 
	BU32 Filelen;	/* B */
	
}Player_Mp3File_S;

typedef struct _Player_Film_Info_S
{
	BU08 FileName[FILE_FULLNAME_LEN];
	BU08 FilmName[APP_PLAYER_NAME_MAX];
	BU32 TimeLen;//����Ϊ��λ
	BU32 FileLen;
	BU32 FrameNum;
}Player_Film_Info_S;

typedef struct _PLAYER_FILM_GROUP_S
{
	BU32 FilmFileNum;
	Player_Film_Info_S FilmInfo[100];
}Player_Film_Group_S;


typedef struct _PLAYER_MP3_GROUP_S
{
	BU32 MP3FileNum;
	Player_Mp3File_S MP3Info[1024];
}Player_MP3_Group_S;



/*
���ò���������volume������С(0~100)
*/
void APP_Player_API_SetVolume(BU08 volume);
/*
��������
*/
void APP_Player_API_SetTrack(APP_PLAYER_TRACK_E track);
/*
bMute !=0 ����
*/
void APP_Player_API_SetMuteState(BU08 bMute);
/*
*��õ�ǰ����״̬
*/
BU08 APP_Player_API_GetMuteState(void);
/*
��ȡ�ò������͵��ļ�
*/
BU32 APP_Player_API_GetPlayerFile(APP_PLAYER_TYPE_E player_type,APP_PLAYER_GROUP_TYPE_E group_type,BU32 index,void *str);
/*
��õ�ǰ�Ĳ�������
*/
APP_PLAYER_TYPE_E APP_Player_API_GetCurPlayerType(void);
/*
���õ�ǰ�Ĳ�������
*/
BS32 APP_Player_API_SetCurPlayerType(APP_PLAYER_TYPE_E PlayType);
/*
��õ�ǰ�Ĳ���������
*/
APP_PLAYER_GROUP_TYPE_E APP_Player_API_GetCurPlayerGroupType(void);
/*
���������Ų����ļ�
*/
BU32 APP_Player_API_Play(BU32 player_no);
/*
ֹͣ����
*/
BU32 APP_Player_API_Stop(void);
/*
��ͣ����
*/
BU32 APP_Player_API_Pause(void);

/*
�ָ�����
*/
BU32 APP_Player_API_Resume(void);

/*
���ŵ�ǰ���������ͣ���ǰ�������͵��¸��ļ�
*/
BU32 APP_Player_API_PlayNext(void);
/*
���ŵ�ǰ���������ͣ���ǰ�������͵��ϸ��ļ�
*/
BU32 APP_Player_API_PlayPre(void);
/*
��������ļ�
*/
BU32 APP_Player_API_Speed(void);
/*
���˲����ļ�
*/
BU32 APP_Player_API_Rewind(void);
/*
��ȡ�����ļ��������
*/
BU32 APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_E file_attr_type, void *str);


/*
�Ӿ���·�����ļ���������·�����ļ���
*/
BU32 APP_PLAYER_API_GetRelativePath(BU08 *abspath, BU08* relpath);


/*
��ȡ��ǰ���͵��ļ�����
*/

BU32 APP_Player_API_GetCurTypeCount(void);

BS32 APP_Player_API_AddVolume();


BS32 APP_Player_API_ReduceVolume();

BU32 APP_Player_API_GetVolume();

/*��ȡPvr��ǰ�����ٶ�*/
BU32 APP_Player_API_GetPvrCurSpeedIndex();

/*����Pvr��ǰ�����ٶ�*/
void APP_Player_API_SetPvrCurSpeedIndex(BU32 speed);

#endif

