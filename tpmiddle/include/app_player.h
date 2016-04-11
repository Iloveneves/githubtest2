#ifndef _H_APP_PLAYER_H
#define _H_APP_PLAYER_H

#define APP_PLAYER_UNMUTE_STATE  0
#define APP_PLAYER_MUTE_STATE    1
#define APP_PLAYER_NAME_MAX 32

#define APP_PLAY_SPEED_NORMAL 1024
/*-----------------------------------------------
声道类型
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_TRACK_STEREO_e = 0, //立体声
	APP_PLAYER_TRACK_LEFT_e,       //左声道
	APP_PLAYER_TRACK_RIGHT_e,      //右声道
	APP_PLAYER_TRACK_SINGLE_e,      //单声道
	APP_PLAYER_TRACK_BUTT_e
}APP_PLAYER_TRACK_E;

/*-----------------------------------------------
播放类型
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_TYPE_NULL_e = 0,
	APP_PLAYER_TYPE_MP3_e, 		//MP3类型
	APP_PLAYER_TYPE_PVR_e,     //PVR视频类型
	APP_PLAYER_TYPE_AVI_e,     //AVI视频类型
	APP_PLAYER_TYPE_BUTT_e
}APP_PLAYER_TYPE_E;

/*-----------------------------------------------
播放组类型
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_GROUP_TYPE_ALL_e = 0, //所有类型组
	APP_PLAYER_GROUP_TYPE_FAV_e     //喜爱类型组
}APP_PLAYER_GROUP_TYPE_E;

/*-----------------------------------------------
播放组类型
-------------------------------------------------*/
typedef enum
{
	APP_PLAYER_FILEATTR_TYPE_FILENAME_e = 0,//获取文件的文件名
	APP_PLAYER_FILEATTR_TYPE_TIMES_e,       //获取文件的总播放时间
	APP_PLAYER_FILEATTR_TYPE_CURTIME_e,     //获取文件的当前播放时间
	APP_PLAYER_FILEATTR_TYPE_ARTIST_e,      //获取文件的演唱者
	APP_PLAYER_FILEATTR_TYPE_SONGNAME_e,    //获取歌曲名
	APP_PLAYER_FILEATTR_TYPE_SPECIAL_e,		//获取歌曲的专辑名
	APP_PLAYER_FILEATTR_TYPE_CURSTATUS_e		//当前MP3播放器状态
}APP_PLAYER_FILEATTR_TYPE_E;

/*-----------------------------------------------
播放状态
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
	BU32 TimeLen;//毫秒为单位
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
设置播放器声音volume音量大小(0~100)
*/
void APP_Player_API_SetVolume(BU08 volume);
/*
设置声道
*/
void APP_Player_API_SetTrack(APP_PLAYER_TRACK_E track);
/*
bMute !=0 静音
*/
void APP_Player_API_SetMuteState(BU08 bMute);
/*
*获得当前静音状态
*/
BU08 APP_Player_API_GetMuteState(void);
/*
获取该播放类型的文件
*/
BU32 APP_Player_API_GetPlayerFile(APP_PLAYER_TYPE_E player_type,APP_PLAYER_GROUP_TYPE_E group_type,BU32 index,void *str);
/*
获得当前的播放类型
*/
APP_PLAYER_TYPE_E APP_Player_API_GetCurPlayerType(void);
/*
设置当前的播放类型
*/
BS32 APP_Player_API_SetCurPlayerType(APP_PLAYER_TYPE_E PlayType);
/*
获得当前的播放组类型
*/
APP_PLAYER_GROUP_TYPE_E APP_Player_API_GetCurPlayerGroupType(void);
/*
根据索引号播放文件
*/
BU32 APP_Player_API_Play(BU32 player_no);
/*
停止播放
*/
BU32 APP_Player_API_Stop(void);
/*
暂停播放
*/
BU32 APP_Player_API_Pause(void);

/*
恢复播放
*/
BU32 APP_Player_API_Resume(void);

/*
播放当前播放组类型，当前播放类型的下个文件
*/
BU32 APP_Player_API_PlayNext(void);
/*
播放当前播放组类型，当前播放类型的上个文件
*/
BU32 APP_Player_API_PlayPre(void);
/*
快进播放文件
*/
BU32 APP_Player_API_Speed(void);
/*
快退播放文件
*/
BU32 APP_Player_API_Rewind(void);
/*
获取播放文件相关属性
*/
BU32 APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_E file_attr_type, void *str);


/*
从绝对路径的文件名获得相对路径的文件名
*/
BU32 APP_PLAYER_API_GetRelativePath(BU08 *abspath, BU08* relpath);


/*
获取当前类型的文件总数
*/

BU32 APP_Player_API_GetCurTypeCount(void);

BS32 APP_Player_API_AddVolume();


BS32 APP_Player_API_ReduceVolume();

BU32 APP_Player_API_GetVolume();

/*获取Pvr当前播放速度*/
BU32 APP_Player_API_GetPvrCurSpeedIndex();

/*设置Pvr当前播放速度*/
void APP_Player_API_SetPvrCurSpeedIndex(BU32 speed);

#endif

