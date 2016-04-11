#include <stdio.h>
#include <time.h>
#include "app_common.h"
#include "app_ipi_common.h"

static const unsigned long aCrc32_tab[] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

app_system_data_s app_system_data;

extern BU08 g_Week;
extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern BU32 videoOutput;

extern BU32 g_AllTypeChnNo[APP_CHN_AV_TYPE_RD_e + 1][APP_CHN_GROUP_TYPE_EDIT_e + 1];
extern BU08 isNeedReCreateChnList;


extern BU32 g_Year;
extern BU32 g_Month;
extern BU32 g_Date;
extern BU32 g_Hour;
extern BU32 g_Minute;
extern BU32 g_Sec;

static BU16 dba_db_freq = 0;

static BU16 dba_ota_freq = 0;
static BU16 dba_ota_symb = 0;
static BU32 dba_ota_pid = 0;
static BU08 dba_ota_qam = 0;
static BU08 dba_ota_flag = 0;
static BU08 dba_ota_type = 0;
static BU32 dba_ota_fac = 0;
static BU32 dba_ota_hwversion = 0;
static BU32 dba_ota_swversion = 0;

//下面的地址为保存在flash里手动设置的ip，
//一开机的时候就从flash中读出来
BU08 g_ip_address[16] = "000.000.000.000";
BU08 g_sub_net_Mark[16] = "000.000.000.000";
BU08 g_gateway_ip[16] = "000.000.000.000";
BU08 g_first_chose_dns[16] = "000.000.000.000";
BU08 g_reserve_dns[16] = "000.000.000.000";

extern OTA_Info_S	 g_OtaInfo;
extern BU32 semaphoreWriteE2;

/*
BS32 e2h_testE2;
BS32 e2h_g_MuteState;
BS32 e2h_g_AvType;
BS32 e2h_g_GroupType;
BS32 e2h_newVersion; 
BS32 e2h_sysVolume;
BS32 e2h_isFlashHasData;
BS32 e2h_tvMode;
BS32 e2h_transparence;
BS32 e2h_infoTimeOut;
BS32 e2h_munuLock;
BS32 e2h_chnLock;
BS32 e2h_password;
BS32 e2h_videoOut;
BS32 e2h_aspect;
BS32 e2h_language;
BS32 e2h_g_CurChnNo;
BS32 e2h_dba_main_freq;
BS32 e2h_dba_nvod_freq;
BS32 e2h_dba_area_lock;

BS32 e2h_dba_ota_freq;
BS32 e2h_dba_ota_symb;
BS32 e2h_dba_ota_pid;
BS32 e2h_dba_ota_qam;

BS32 e2h_dba_factory_year;
BS32 e2h_dba_factory_week;
BS32 e2h_dba_factory_num;
BS32 e2h_dba_factory_stbType;
BS32 e2h_dba_factory_sequence;

BS32 e2h_dba_flag;
*/
static BU32 g_update_type = APP_SYS_OTA_UD_NULL;

static unsigned int CRC32(unsigned int ulCrc, const char* pucData, unsigned int ulLen)
{
	if( !pucData )
	{
		return 0;
	}
    	ulCrc = ulCrc ^ 0xffffffffL;
	unsigned int ulI;
	unsigned int tmp0,tmp1;
	
    	for (ulI = 0;  ulI < ulLen;  ulI++)
    	{
        	ulCrc = aCrc32_tab[ (ulCrc ^ pucData[ulI]) & 0xff ] ^ (ulCrc >> 8);

    	}

	return ulCrc ^ 0xffffffffL;
}

BS32 asc_to_data(BU08 *strAscii , BU32 *u32Res , int len )
{
	int		i = 0 ;
	BU08	u8data  = 0 ;
	BU32	u32data = 0 ;

	if( (!strAscii) || (!u32Res) )
	{
		return	-1;
	}

	for(i = 0 ; i < len ; i++)
	{
		u8data = 0 ;
		if( (strAscii[i] >= '0') && (strAscii[i] <= '9') )
		{
			u8data = strAscii[i] - '0' ;
		}
		else if( (strAscii[i] >= 'a') && (strAscii[i] <= 'f') )
		{
			u8data = strAscii[i] - 'a'+ 10 ;
		}
		else if( (strAscii[i] >= 'A') && (strAscii[i] <= 'F') )
		{
			u8data  = strAscii[i] - 'A'+ 10 ;
		}
		
		u32data = ( u32data << 4 ) | u8data ;
	}

	*u32Res	=  u32data ;
	return	0;
}


/*-------------------------------------------------------------------------------

	Description:设置更新类型

	Parameters:

-------------------------------------------------------------------------------*/
void APP_SYS_API_SetUpdateType(BU32 type)
{
	g_update_type = type;
}
/*-------------------------------------------------------------------------------

	Description:得到更新类型

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_SYS_API_GetUpdateType(void)
{
	return g_update_type;
}

/*-------------------------------------------------------------------------------

	Description:获得本地日期，时间

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_SYS_API_GetLocalDateAndTime(APP_Date_S* DateAndTime)
{
	DateAndTime->m_day = (BU08)g_Date;
	DateAndTime->m_hour = (BU08)g_Hour;
	DateAndTime->m_min = (BU08)g_Minute;
	DateAndTime->m_month = (BU08)g_Month;
	DateAndTime->m_sec = (BU08)g_Sec;
	DateAndTime->m_weekday = (BU08)g_Week;
	DateAndTime->m_year = (BU16)g_Year;
	return APP_SUCCESS;
}


/*
系统统一音量大小数据(APP_Audio_API_SetVolume 改变音量)
*/
BU32 APP_SYS_IPI_GetVolume(void)
{
	ChnDataBase_ChnInfo_S chnInfo;
	if (app_system_data.volume_save_type == 1)
	{
		return app_system_data.sysVolume;	
	}
	else
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&chnInfo))
		{
			return app_system_data.sysVolume;
		}
		else
		{
			return chnInfo.m_AudioVolume;
		}
	}
}

void APP_SYS_IPI_SetVolume(BU32 Volume)
{
	ChnDataBase_ChnInfo_S chnInfo;
	if (Volume > 32)Volume = 32;
	if (app_system_data.volume_save_type == 1)
	{
		app_system_data.sysVolume = Volume;	
		APP_SYS_API_SaveSysInfo ();
	}
	else
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&chnInfo))
		{
			app_system_data.sysVolume = Volume;	
			APP_SYS_API_SaveSysInfo ();
		}
		else
		{
			chnInfo.m_AudioVolume = Volume;
			APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&chnInfo);
		}
	}
}

/*
设置电视制式
*/
APP_SYS_TVMODE_E APP_SYS_IPI_GetTVMode(void)
{
	return app_system_data.tvMode;
}

void APP_SYS_IPI_SetTVMode(APP_SYS_TVMODE_E TVMode)
{
	BU32 osdOutput;
	BU32 modeTemp = FYF_VOUT_TVENC_PAL;
	
	app_system_data.tvMode = TVMode;
	
	if(app_system_data.tvMode > APP_SYS_TVMODE_SECAM_e)
	{
		FYF_API_printf("SetTVMode over value = %d\n", app_system_data.tvMode);
		app_system_data.tvMode = APP_SYS_TVMODE_PAL_e;
	}
	
	switch(app_system_data.tvMode)
	{
	case APP_SYS_TVMODE_AUTO_e:
		modeTemp = FYF_VOUT_TVENC_AUTO;
		break;
	case APP_SYS_TVMODE_PAL_e:
		modeTemp = FYF_VOUT_TVENC_PAL;
		break;
	case APP_SYS_TVMODE_NTSC_e:
		modeTemp = FYF_VOUT_TVENC_NTSC;
		break;
	case APP_SYS_TVMODE_SECAM_e:
		modeTemp = FYF_VOUT_TVENC_SECAM;
		break;
	default:
		modeTemp = FYF_VOUT_TVENC_PAL;
		break;

	}	
	FYF_API_vout_ioctl(videoOutput, FYF_VOUT_SET_MODE, &modeTemp);
}

/*
透明度设置0~10
*/

BU32 APP_SYS_IPI_GetTransparence(void)
{
	return app_system_data.transparence;
}
void APP_SYS_IPI_SetTransparence(BU32 TransValue)
{
	BU32 osdOutput;
	if (TransValue >= 10)
	{
		FYF_API_printf("SetTransparence over value = %d\n", app_system_data.transparence);
		TransValue = 9;
	}
	app_system_data.transparence = TransValue;


	osdOutput = FYF_API_vout_open_window(videoOutput,1);	
	FYF_API_vout_ioctl(osdOutput,FYF_VOUT_SET_WIN_TRANSPARENT,&TransValue);

}

/*
设置屏幕比例
*/
APP_SYS_ASPECT_E APP_SYS_IPI_GetAspectRatio(void)
{
	return app_system_data.aspect;
}
void APP_SYS_IPI_SetAspectRatio(APP_SYS_ASPECT_E Aspect)
{
	BU32 tnsValue;
	app_system_data.aspect = Aspect;
	
	if(app_system_data.aspect > APP_SYS_ASPECT_4_3_e)
	{
		FYF_API_printf("SetAspectRatio over value = %d\n", app_system_data.aspect);
		app_system_data.aspect = APP_SYS_ASPECT_4_3_e;
	}

	if(app_system_data.aspect == APP_SYS_ASPECT_4_3_e)
	{
		tnsValue = FYF_VOUT_SET_AR_4TO3;
	}
	else
	{
		tnsValue = FYF_VOUT_SET_AR_16TO9 ;
	}
	
	FYF_API_vout_ioctl(videoOutput, FYF_VOUT_SET_ASPECT_RATIO, &tnsValue);
}

/*
设置信息显示时间
*/
BU32 APP_SYS_IPI_GetInfoTimeOut(void)
{
	return app_system_data.infoTimeOut;
}
void APP_SYS_IPI_SetInfoTimeOut(BU32 time)
{
	app_system_data.infoTimeOut = time;
	
	if(app_system_data.infoTimeOut > 10)
	{
		FYF_API_printf("SetInfoTimeOut over value = %d\n", app_system_data.infoTimeOut);
		app_system_data.infoTimeOut = 10;
	}
}

/*
设置视频输出
*/
APP_SYS_VIDEO_OUT_E APP_SYS_IPI_GetVideoOut(void)
{
	return app_system_data.videoOut;
}
void APP_SYS_IPI_SetVideoOut(APP_SYS_VIDEO_OUT_E VideoOut)
{
	FYF_VOUT_VIDEO_OUTPUT_e dev = FYF_VIDEO_OUTPUT_YUV;
	app_system_data.videoOut = VideoOut;
	
	if(app_system_data.videoOut > APP_SYS_VIDEO_OUT_YC_e)
	{
		FYF_API_printf("SetVideoOut over value = %d\n", app_system_data.videoOut);
		app_system_data.videoOut = APP_SYS_VIDEO_OUT_YUV_e;	
	}
	switch(app_system_data.videoOut)
	{
		case APP_SYS_VIDEO_OUT_YUV_e:
			dev = FYF_VIDEO_OUTPUT_YUV;
			break;
			
		case APP_SYS_VIDEO_OUT_YC_e:
			dev = FYF_VIDEO_OUTPUT_SVIDEO;
			break;
		default:
			dev = FYF_VIDEO_OUTPUT_YUV;
			break;
	}
	FYF_API_vout_ioctl(videoOutput,FYF_VOUT_SELECT_DEV,&dev);	
}

/*
描述：设置语言
*/
APP_SYS_LANGUAGE_E APP_SYS_IPI_GetLanguage(void)
{
	return app_system_data.language;
}
void APP_SYS_IPI_SetLanguage(APP_SYS_LANGUAGE_E LanguageType)
{
	app_system_data.language = LanguageType;
	
	if(app_system_data.language > APP_SYS_LANGUAGE_CHINESE_e)
	{
		FYF_API_printf("SetLanguage over value = %d\n", app_system_data.language);
		app_system_data.language = APP_SYS_LANGUAGE_CHINESE_e;
	}
}


BU32 APP_SYS_IPI_GetTimeZone()
{
	return app_system_data.timezone;
}

void APP_SYS_IPI_SetTimeZone(BU32 time_zone_type)
{
	app_system_data.timezone = time_zone_type;
	if (app_system_data.timezone > 1)
	{
		app_system_data.timezone = 0;
	}
}

/*
设置菜单加锁
*/
BU08 APP_SYS_IPI_GetMenuLock(void)
{
	return app_system_data.munuLock;
}
void APP_SYS_IPI_SetMenuLock(BU08 lock)
{
	app_system_data.munuLock = lock;
}

/*
设置频道加锁
*/
BU08 APP_SYS_IPI_GetChnLock(void)
{
	return app_system_data.chnLock;
}
void APP_SYS_IPI_SetChnLock(BU08 lock)
{
	app_system_data.chnLock = lock;
}

/*
设置菜单加锁
*/
void APP_SYS_IPI_GetPassword(PU32 Password)
{
	*Password = app_system_data.password;
} 
void APP_SYS_IPI_SetPassword(BU32 Password)
{
	app_system_data.password = Password;
}

/*
设置开机加锁
*/
BU08 APP_SYS_IPI_GetStartUpLock()
{
	return app_system_data.startupLock;
}
void APP_SYS_IPI_SetStartUpLock(BU08 lock)
{
	app_system_data.startupLock = lock;
}

/*
获取/设置主频点
*/
void APP_SYS_IPI_SetMainFreq(BU32 mainFreq)
{
	app_system_data.dba_main_freq = (BU16)mainFreq ;
	
	if(app_system_data.dba_main_freq < 51 || app_system_data.dba_main_freq > 859)
	{
		FYF_API_printf("SetMainFreq over value = %d\n", app_system_data.dba_main_freq);
		app_system_data.dba_main_freq = APP_MAIN_FREQ_MHZ;
	}
}
BU32 APP_SYS_IPI_GetMainFreq(void)
{
	if(app_system_data.dba_main_freq < 51 || app_system_data.dba_main_freq > 859)
	{
		FYF_API_printf("GetMainFreq over value = %d\n", app_system_data.dba_main_freq);
		app_system_data.dba_main_freq = APP_MAIN_FREQ_MHZ;
	}
	return (BU32)app_system_data.dba_main_freq;
}

/*
获取/设置数据广播频点
*/
void APP_SYS_IPI_SetDBFreq(BU32 DBFreq)
{
	app_system_data.dba_broadcast_freq = (BU16)DBFreq ;
	
	if(app_system_data.dba_broadcast_freq < 51 || app_system_data.dba_broadcast_freq > 859)
	{
		FYF_API_printf("SetbroadcastFreq over value = %d\n", app_system_data.dba_broadcast_freq);
		app_system_data.dba_broadcast_freq = APP_DB_FREQ_MHZ;
	}
}
BU32 APP_SYS_IPI_GetDBFreq(void)
{
	if(app_system_data.dba_broadcast_freq < 51 || app_system_data.dba_broadcast_freq > 859)
	{
		FYF_API_printf("GetbroadcastFreq over value = %d\n", app_system_data.dba_broadcast_freq);
		app_system_data.dba_broadcast_freq = APP_DB_FREQ_MHZ;
	}
	return (BU32)app_system_data.dba_broadcast_freq;
}

/*
获取/设置NVOD频点
*/
void APP_SYS_IPI_SetNvodFreq(BU32 NvodFreq)
{
	app_system_data.dba_nvod_freq = (BU16)NvodFreq ;
	
	if(app_system_data.dba_nvod_freq < 51 || app_system_data.dba_nvod_freq > 859)
	{
		FYF_API_printf("SetNvodFreq over value = %d\n", app_system_data.dba_nvod_freq);
		app_system_data.dba_nvod_freq = APP_NVOD_FREQ_MHZ;
	}
}
BU32 APP_SYS_IPI_GetNvodFreq(void)
{
	if(app_system_data.dba_nvod_freq < 51 || app_system_data.dba_nvod_freq > 859)
	{
		FYF_API_printf("GetNvodFreq over value = %d\n", app_system_data.dba_nvod_freq);
		app_system_data.dba_nvod_freq = APP_NVOD_FREQ_MHZ;
	}
	return (BU32)app_system_data.dba_nvod_freq;
}


/*
获取/设置广告频点
*/
void APP_SYS_IPI_SetADFreq(BU32 adFreq)
{
	app_system_data.dba_ad_freq = (BU16)adFreq ;
	
	if(app_system_data.dba_ad_freq < 51 || app_system_data.dba_ad_freq > 859)
	{
		FYF_API_printf("SetMainFreq over value = %d\n", app_system_data.dba_ad_freq);
		app_system_data.dba_ad_freq = APP_AD_FREQ_MHZ;
	}
}
BU32 APP_SYS_IPI_GetADFreq(void)
{
	if(app_system_data.dba_ad_freq < 51 || app_system_data.dba_ad_freq > 859)
	{
		FYF_API_printf("GetMainFreq over value = %d\n", app_system_data.dba_ad_freq);
		app_system_data.dba_ad_freq = APP_AD_FREQ_MHZ;
	}
	return (BU32)app_system_data.dba_ad_freq;
}

/*
获取/设置股票频点
*/
void APP_SYS_IPI_SetStockFreq(BU32 StockFreq)
{
	app_system_data.dba_stock_freq = StockFreq ;
	
	if(app_system_data.dba_stock_freq < 51 || app_system_data.dba_stock_freq > 859)
	{
		FYF_API_printf("SetMainFreq over value = %d\n", app_system_data.dba_stock_freq);
		app_system_data.dba_stock_freq = APP_STOCK_FREQ_MHZ;
	}
}
BU32 APP_SYS_IPI_GetStockFreq(void)
{
	if(app_system_data.dba_stock_freq < 51 || app_system_data.dba_stock_freq > 859)
	{
		FYF_API_printf("GetMainFreq over value = %d\n", app_system_data.dba_stock_freq);
		app_system_data.dba_stock_freq = APP_STOCK_FREQ_MHZ;
	}
	return app_system_data.dba_stock_freq;
}

/*
获取/设置BAT VER
*/
void APP_SYS_IPI_SetBatVer(BU08 batVer)
{
	app_system_data.batVersion= batVer ;
}
BU32 APP_SYS_IPI_GetBatVer(void)
{
	return app_system_data.batVersion;
}

/*
获取/设置NIT VER
*/
void APP_SYS_IPI_SetNitVer(BU08 nitVer)
{
	app_system_data.nitVersion= nitVer;
}
BU32 APP_SYS_IPI_GetNitVer(void)
{
	return app_system_data.nitVersion;
}



#if 0
/*-------------------------------------------------------------------------------

	Description:

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_SYS_API_GetCurChno(void)
{
	return (app_system_data.g_CurChnNo);
}

void APP_SYS_API_SetCurChno(BU32 curChno)
{
	 app_system_data.g_CurChnNo = curChno;
}
#endif
/*-------------------------------------------------------------------------------

	Description:获得当前节目组类型

	Parameters:

-------------------------------------------------------------------------------*/
APP_CHN_GROUP_TYPE_E APP_SYS_API_GetCurGroupType(void)
{
	return (APP_CHN_GROUP_TYPE_E)(app_system_data.g_GroupType);
}

BU32 str_to_value(BU08 * str, BU32 len)
{
	printf("str1 = %s\n", str);
	BU32 value = 0;
	BS32 i;
	for (i = 0; i < len; i++)
		value = value * 10 + str[i] - 48;
	return value;
}

BU32 hexstr_to_value(BU08 * str, BU32 len)
{
	printf("str2 = %s\n", str);
	BS32 i;
	BU32 value = 0;
	for (i = 0; i < len ; i++)
		if (str[i] <= '9')value = value * 16 + str[i] - 48;
		else if (str[i] >= 'a')value = value * 16 + str[i] - 'a' + 10;
	return value;
}

void value_to_str(BU08 *str, BU32 value, BU32 len)
{
	printf("value1 = %d\n");
	BS32 i;
	for (i = len - 1; i >= 0; i--)
	{
		if (value == 0)
		{
			str[i] = '0';
		}
		else
		{
			str[i] = value % 10 + 48;
			value /= 10;
		}
			
	}
		
}


void hexvalue_to_str(BU08 *str, BU32 value, BU32 len)
{
	printf("value2 = %d\n");
	BS32 i;
	for (i = len - 1; i >= 0; i--)
	{
		if (value == 0)
		{
			str[i] = '0';
		}
		else
		{
			if ((value & 0xf) >= 10)
			{
				str[i] = (value & 0xf) + 'a' - 10;
			}
			else
			{
				str[i] = (value & 0xf) + '0';
			}
			value >>= 4;
		}
	}
		
}
/*-------------------------------------------------------------------------------

	Description:获得/设置系统参数

	Parameters:

-------------------------------------------------------------------------------*/
void APP_SYS_API_GetSysInfo(APP_SYS_TYPE_E type, BU32 *value)
{
	BU08 *stbid;
	MAC_BOX_INFO_STRUCT *mac_box_info;
	switch (type)
	{
	case APP_SYS_VOLUME_e:/*0~32 BU32*/
		*value = (BU32)APP_SYS_IPI_GetVolume();
		break;
	case APP_SYS_TV_MODE_e:
		*value = (BU32)APP_SYS_IPI_GetTVMode();
		break;
	case APP_SYS_VIDEOOUT_e:
		*value = (BU32)APP_SYS_IPI_GetVideoOut();
		break;
	case APP_SYS_ASPECT_e:
		*value =  (BU32)APP_SYS_IPI_GetAspectRatio();
		break;
	case APP_SYS_TRANSPARENCE_e:
		*value =  APP_SYS_IPI_GetTransparence();
		break;
	case APP_SYS_TIMEOUT_e:
		*value =  APP_SYS_IPI_GetInfoTimeOut();
		break;
	case APP_SYS_LANGUAGE_e:
		*value = (BU32)APP_SYS_IPI_GetLanguage();
		break;
	case APP_SYS_TimeZone_e:
		*value = (BU32)APP_SYS_IPI_GetTimeZone();
		break;
	case APP_SYS_STB_INFO_e:
		break;
	case APP_SYS_MENU_LOCK_e:
		*value =  (BU32)APP_SYS_IPI_GetMenuLock();
		break;
	case APP_SYS_MENU_PSW_e:
		APP_SYS_IPI_GetPassword(value);
		break;
	case APP_SYS_CHN_LOCK_e:
		*value =  (BU32)APP_SYS_IPI_GetChnLock();
		break;
	case APP_SYS_CHN_PSW_e:
		APP_SYS_IPI_GetPassword(value);
		break;
	case APP_SYS_STARTUP_LOCK_e:
		*value =  (BU32)APP_SYS_IPI_GetStartUpLock();
		break;
	case APP_SYS_STARTUP_PSW_e:
		APP_SYS_IPI_GetPassword(value);
		break;
	case APP_SYS_MAIN_FREQ_e://主频点 BU32
		*value = app_system_data.dba_main_freq;
		break;
	case APP_SYS_AREA_LOCK_e://区域锁定 BU32	
		*value = app_system_data.dba_area_lock;
		break;
	case APP_SYS_NVOD_FREQ_e://NVOD频点
		*value = APP_SYS_IPI_GetNvodFreq();
		break;	
	case APP_SYS_DB_FREQ_e:
		*value = APP_SYS_IPI_GetDBFreq();//数据广播频点
		break;
	case APP_SYS_AD_FREQ_e://广告频点
	    *value = APP_SYS_IPI_GetADFreq();
	    break;
	case APP_SYS_STOCK_FREQ_e://股票频点
	    *value = APP_SYS_IPI_GetStockFreq();
	    break;
	case APP_SYS_OTA_FREQ_e: //OTA频点
		*value = dba_ota_freq;
		break;
	case APP_SYS_OTA_SYMB:    //OTA符号率
		*value = dba_ota_symb;
		break;
	case APP_SYS_OTA_PID:     //OTA PID
		*value = dba_ota_pid;
		break;
	case APP_SYS_OTA_QAM: 	//OTA QAM调制模式
		*value = dba_ota_qam;
		break;
		case APP_SYS_FACTORY_SN:		//序列号
		APP_Printf("  factroy sn = %d \n",app_system_data.dba_factory_sequence);
		*value = app_system_data.dba_factory_sequence;
		if (*value > 2147483647)
		{
			app_system_data.dba_factory_sequence = 1;
			*value = 1;
		}
		break;
	case APP_SYS_FACTORY_YEAR:	//生产年号
		*value = app_system_data.dba_factory_year;
		if (*value > 9999)
		{
			*value = 2010;
			app_system_data.dba_factory_year = 2010;
		}
		break;
	case APP_SYS_FACTORY_WEEK: 	//生产周号
		*value = app_system_data.dba_factory_week;
		if (*value > 99)
		{
			*value = 10;
			app_system_data.dba_factory_week = 10;
		}
		break;
	case APP_SYS_FACTORY_STBTYPE://型号
		*value = app_system_data.dba_factory_stbType; 
		if (*value > 99)
		{
			*value = 1;
			app_system_data.dba_factory_stbType = 1;
		}
		break;
	case APP_SYS_FACTORY_NUM: 	//厂商号
		*value = app_system_data.dba_factory_num;
		if (*value > 99)
		{
			*value = 1;
			app_system_data.dba_factory_num = 1;
		}
		break;
	case APP_SYS_SwitchItem_e: 	//网络切换
		*value = app_system_data.SwitchItem;
		break;
	case APP_SYS_AutoIP_e://自动IP
		*value = app_system_data.AutoIP;
		break;
	case APP_SYS_AutoDNS_e: 	//自动DNS
		*value = app_system_data.AutoDNS;
		break;
	case APP_SYS_Region_SwitchItem_e: //自动获取区域ID
		*value = app_system_data.AutoRegionID;
		break;
	case APP_SYS_RegionID_e: 	//区域ID
		*value = app_system_data.RegionID;
		break;
	case APP_SYS_StbID_e:
		stbid = (BU08*)value;
		sprintf(stbid,"%s",app_system_data.g_mac_box_info.AreaIndicatorCode);
		break;
	case APP_SYS_AreaIndicatorCode_e:
		*value = str_to_value(app_system_data.g_mac_box_info.AreaIndicatorCode, 4);
		break;
	case APP_SYS_ManufactureCode_e:
		*value = str_to_value(app_system_data.g_mac_box_info.ManufactureCode, 2);
		break;
	case APP_SYS_StbFactoryIndicator_e:
		*value = str_to_value(app_system_data.g_mac_box_info.StbFactoryIndicator, 3);
		break;
	case APP_SYS_StbType_e:
		*value = str_to_value(app_system_data.g_mac_box_info.StbType, 2);
		break;
	case APP_SYS_HwVersion_e:
		*value = str_to_value(app_system_data.g_mac_box_info.HwVersion, 2);
		break;
	case APP_SYS_StbIndicatorType_e:
		*value = str_to_value(app_system_data.g_mac_box_info.StbIndicatorType, 1);
		break;
	case APP_SYS_MacAddressForward_e:
		*value = hexstr_to_value(app_system_data.g_mac_box_info.ucMacAddress, 6);
		break;
	case APP_SYS_MacAddressBack_e:
		*value = hexstr_to_value(&(app_system_data.g_mac_box_info.ucMacAddress[6]), 6);
		break;
#ifdef DVT_CA
	case APP_SYS_DVTCAAreaCode_e:
		memcpy((BU08 *)value,app_system_data.DVTAreaCode,4);
		break;
#endif
	case APP_SYS_AdvVersion_e:
		*value = app_system_data.advVersion;
		break;
	case APP_SYS_AdvSaveFlag_e:
		*value = app_system_data.advSaveFlag;
		break;
	case APP_SYS_VolumeSave_e:
		*value = app_system_data.volume_save_type;
		break;
	default:
		break;
	}
}

/*-------------------------------------------------------------------------------

	Description:

	Parameters:

-------------------------------------------------------------------------------*/
void APP_SYS_API_SetSysInfo(APP_SYS_TYPE_E type, BU32 value)
{
	switch (type)
	{
	case APP_SYS_VOLUME_e:
		APP_SYS_IPI_SetVolume(value);
		break;
	case APP_SYS_TV_MODE_e:
		APP_SYS_IPI_SetTVMode((APP_SYS_TVMODE_E)value);
		APP_SYS_IPI_SetTransparence(app_system_data.transparence);
		break;
	case APP_SYS_VIDEOOUT_e:
		APP_SYS_IPI_SetVideoOut((APP_SYS_VIDEO_OUT_E)value);
		break;
	case APP_SYS_ASPECT_e:
		APP_SYS_IPI_SetAspectRatio((APP_SYS_ASPECT_E)value);
		break;
	case APP_SYS_TRANSPARENCE_e:
		APP_SYS_IPI_SetTransparence(value);
		break;
	case APP_SYS_TIMEOUT_e:
		APP_SYS_IPI_SetInfoTimeOut(value);
		break;
	case APP_SYS_LANGUAGE_e:
		APP_SYS_IPI_SetLanguage((APP_SYS_LANGUAGE_E)value);
		break;
	case APP_SYS_STB_INFO_e:
		break;
	case APP_SYS_TimeZone_e:
		APP_SYS_IPI_SetTimeZone(value);
		break;
	case APP_SYS_MENU_LOCK_e:
		APP_SYS_IPI_SetMenuLock((BU08)value);
		break;
	case APP_SYS_MENU_PSW_e:
		APP_SYS_IPI_SetPassword(value);
		break;
	case APP_SYS_CHN_LOCK_e:
		APP_SYS_IPI_SetChnLock((BU08)value);
		break;
	case APP_SYS_STARTUP_LOCK_e:
		APP_SYS_IPI_SetStartUpLock((BU08)value);
		break;
	case APP_SYS_CHN_PSW_e:
		APP_SYS_IPI_SetPassword(value);
		break;
	case APP_SYS_MAIN_FREQ_e://主频点 BU32
		APP_SYS_IPI_SetMainFreq(value);
		break;
	case APP_SYS_AREA_LOCK_e://区域锁定 BU32	
		app_system_data.dba_area_lock = value ;
		break;
	case APP_SYS_NVOD_FREQ_e://NVOD频点
		app_system_data.dba_nvod_freq = value;
		break;	
	case APP_SYS_DB_FREQ_e:
		APP_SYS_IPI_SetDBFreq(value);
		break;
	case APP_SYS_AD_FREQ_e:
		APP_SYS_IPI_SetADFreq(value);
		break;
	case APP_SYS_STOCK_FREQ_e:
		APP_SYS_IPI_SetStockFreq(value);
		break;
	case APP_SYS_OTA_FREQ_e: //OTA频点
		dba_ota_freq = (BU16)(value/1000);
		break;
	case APP_SYS_OTA_SYMB:    //OTA符号率
		dba_ota_symb = (BU16)(value/1000);
		break;
	case APP_SYS_OTA_PID:     //OTA PID
		dba_ota_pid = value;
		break;
	case APP_SYS_OTA_QAM: 	//OTA QAM调制模式
		dba_ota_qam = (BU08)value;
		break;
	case APP_SYS_OTA_TYPE:
		dba_ota_type= (BU08)value;
		break;
	case APP_SYS_FACTORY_SN:		//序列号
		app_system_data.dba_factory_sequence = value;
		break;
	case APP_SYS_FACTORY_YEAR:	//生产年号
		app_system_data.dba_factory_year = value;
		break;
	case APP_SYS_FACTORY_WEEK: 	//生产周号
		app_system_data.dba_factory_week = value;
		break;
	case APP_SYS_FACTORY_STBTYPE://型号
		app_system_data.dba_factory_stbType = value;
		break;
	case APP_SYS_FACTORY_NUM: 	//厂商号
		app_system_data.dba_factory_num = value;
		break;
	case APP_SYS_SwitchItem_e: 	//网络切换
		app_system_data.SwitchItem = value;
		break;
	case APP_SYS_AutoIP_e://自动IP
		app_system_data.AutoIP = value;
		break;
	case APP_SYS_AutoDNS_e: 	//自动DNS
		app_system_data.AutoDNS = value;
		break;
	case APP_SYS_Region_SwitchItem_e: //自动获取区域ID
		app_system_data.AutoRegionID = value;
		break;
	case APP_SYS_RegionID_e: 	//区域ID
		app_system_data.RegionID = value;
		break;
	case APP_SYS_StbID_e:
		//mac_box_info = (MAC_BOX_INFO_STRUCT *)value;
		//memcpy(mac_box_info, &(app_system_data.g_mac_box_info), sizeof(MAC_BOX_INFO_STRUCT));
		break;
	case APP_SYS_AreaIndicatorCode_e:
		value_to_str(app_system_data.g_mac_box_info.AreaIndicatorCode, value, 4);
		break;
	case APP_SYS_ManufactureCode_e:
		value_to_str(app_system_data.g_mac_box_info.ManufactureCode, value, 2);
		break;
	case APP_SYS_StbFactoryIndicator_e:
		value_to_str(app_system_data.g_mac_box_info.StbFactoryIndicator, value, 3);
		break;
	case APP_SYS_StbType_e:		
		value_to_str(app_system_data.g_mac_box_info.StbType, value, 2);
		break;
	case APP_SYS_HwVersion_e:
		value_to_str(app_system_data.g_mac_box_info.HwVersion, value, 2);
		break;
	case APP_SYS_StbIndicatorType_e:
		value_to_str(app_system_data.g_mac_box_info.StbIndicatorType, value, 1);
		break;
	case APP_SYS_MacAddressForward_e:
		hexvalue_to_str(app_system_data.g_mac_box_info.ucMacAddress, value, 6);
		break;
	case APP_SYS_MacAddressBack_e:
		hexvalue_to_str(&(app_system_data.g_mac_box_info.ucMacAddress[6]), value, 6);
		break;
#ifdef DVT_CA
	case APP_SYS_DVTCAAreaCode_e:
		memcpy(app_system_data.DVTAreaCode,(BU08 *)value,4);
		break;
#endif
	case APP_SYS_AdvVersion_e:
		app_system_data.advVersion = value;
		break;
	case APP_SYS_AdvSaveFlag_e:
		app_system_data.advSaveFlag = value;
		break;
	case APP_SYS_VolumeSave_e:
		app_system_data.volume_save_type = value;
		break;
	default:
		break;
	}
}

void APP_SYS_API_SetStbID(MAC_BOX_INFO_STRUCT *mac_box_info)
{
	memcpy(&(app_system_data.g_mac_box_info), mac_box_info, sizeof(MAC_BOX_INFO_STRUCT));
}

void APP_SYS_API_GetStbID(MAC_BOX_INFO_STRUCT *mac_box_info)
{
	memcpy(mac_box_info, &(app_system_data.g_mac_box_info), sizeof(MAC_BOX_INFO_STRUCT));
}

APP_BOOT_INFO fyf_boot_info;
BS32 e2pHandle = 0;
/*-------------------------------------------------------------------------------

	Description:

	Parameters:

-------------------------------------------------------------------------------*/

void APP_SYS_IPI_WriteStbInfo(void)
{
	unsigned int crc_result;
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;

	LOADER_DB_INFO_S LOADER_DB;
	FYF_FLS_IOCTL_param.buffer = &LOADER_DB;
	FYF_FLS_IOCTL_param.bufferSize = sizeof(LOADER_DB_INFO_S);
	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_STBINFO, (BU32)&FYF_FLS_IOCTL_param);
/*
	crc_result = CRC32(0,&LOADER_DB,sizeof(LOADER_DB_INFO_S)-4);
	if(LOADER_DB.magic_number != LOADER_MAGIC_NUMBER)
	{
		APP_Printf(" magic numer  failure \n");
		return;	
	}

	if(crc_result != LOADER_DB.u32Crc)
	{
		APP_Printf(" crc check failure \n");
		return;
	}
	*/

	dba_ota_fac = APP_NIT_FJ_MANU_ID;
	dba_ota_hwversion = APP_SYS_HARD_VERSION;
	dba_ota_swversion = 6001;

    APP_Printf("==================stb info================\n");

	APP_Printf("ota_frequency = %d \n",dba_ota_freq);
	APP_Printf("ota_symbol_rate = %d \n",dba_ota_symb);
	APP_Printf("ota_modulation = %d \n",dba_ota_qam);
	APP_Printf("ota_pid = 0x%x \n",dba_ota_pid);
	APP_Printf("ota_type = 0x%x \n",dba_ota_type);
	APP_Printf("ota_fac = 0x%x \n",dba_ota_fac);
	APP_Printf("ota_hwversion = 0x%x \n",dba_ota_hwversion);

    APP_Printf("==================stb info================\n");

#if 0
/*
	0x4c表示有效升级信息
	其他为无效升级请求
*/
	LOADER_DB.ota_type = 0x4c; 
	LOADER_DB.ota_pid = 0x1b58;//dba_ota_pid;
/*
	单位10KHZ
*/	
	LOADER_DB.ota_frequency = 31500;
/*
	单位KHZ
*/	
	LOADER_DB.ota_symbol_rate = 6875;
/*
	QAM64
*/
	LOADER_DB.ota_modulation = 2;
/*
	32位	
*/	
	LOADER_DB.u32NewSoftware = 0x01110101;
/*
	32位	
*/	
	LOADER_DB.u32HardwareVersion = 0x31100101;
	LOADER_DB.u32FailedCnt = 0;
#else
	LOADER_DB.magic_number = LOADER_MAGIC_NUMBER;
	LOADER_DB.ota_type = 0x4c; 
	LOADER_DB.ota_pid = dba_ota_pid;//dba_ota_pid;
	LOADER_DB.ota_frequency = dba_ota_freq*100;
	LOADER_DB.ota_symbol_rate = dba_ota_symb;
	LOADER_DB.ota_modulation = dba_ota_qam;
	LOADER_DB.u32NewSoftware = dba_ota_swversion;
	LOADER_DB.u32HardwareVersion = dba_ota_hwversion;
	LOADER_DB.u32DestroyFlag = 0;
	LOADER_DB.u32FailedCnt = 0;
#endif
	LOADER_DB.u32Crc = CRC32(0,&LOADER_DB,sizeof(LOADER_DB_INFO_S)-4);

	FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_STBINFO, (BU32)&FYF_FLS_IOCTL_param);

    APP_PRINT_DEBUG("==================set stb inf success ================\n");

//    FYF_API_thread_sleep(1000);

//    system("reboot");


}



BS32 APP_SYS_API_SaveSysInfo(void)
{
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	BU16 crc,index;

	APP_PRINT_DEBUG("APP_SYS_API_SaveSysInfo");
	fyf_e2p_ioctl_param.buffer = (PU08)&app_system_data;
	fyf_e2p_ioctl_param.bufferSize = sizeof(app_system_data);
	APP_PRINT_DEBUG("fyf_e2p_ioctl_param.bufferSize = %d\n",sizeof(app_system_data));
#if 0
	crc = 0;
	for(index = 0;index<sizeof(app_system_data)-2;index++)
	{
		crc += fyf_e2p_ioctl_param.buffer[index];
	}
	app_system_data.crc = crc;
	
	APP_PRINT_DEBUG("app_system_data.crc = %d\n", app_system_data.crc);
#else
	app_system_data.crc = E2P_CRC_DATA;
#endif
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, APP_SYS_DATA_ADDR, (BU32)&fyf_e2p_ioctl_param);

	//APP_SYS_IPI_WriteStbInfo();


	return APP_SUCCESS;

}

void APP_SYS_IPI_SetSysValueToDefault(void)
{
	APP_Printf("----APP_SYS_IPI_SetSysValueToDefault---\n");
	app_system_data.nitVersion = 0xFF;
	app_system_data.batVersion = 0xFF;
	app_system_data.advSaveFlag = 0;
	app_system_data.advVersion = 0xff;
	app_system_data.volume_save_type = 1;
#if (APP_CONFIG_TVMODE == TVMODE_ONLY_NTSC)
	app_system_data.tvMode 			= APP_SYS_TVMODE_NTSC_e;
#else
	app_system_data.tvMode 			= APP_SYS_TVMODE_PAL_e;
#endif
	app_system_data.transparence 	= 2;
	app_system_data.infoTimeOut 	= 5;
	app_system_data.munuLock 		= 0;
	app_system_data.chnLock 		= 1;
	app_system_data.startupLock     = 0;
	app_system_data.password 		= 0;
	app_system_data.sysVolume 		= 16;
#if (APP_CONFIG_VIDEO_OUTPUT == VIDEO_OUTPUT_YC)
	app_system_data.videoOut 		= APP_SYS_VIDEO_OUT_YC_e;
#else
	app_system_data.videoOut 		= APP_SYS_VIDEO_OUT_YUV_e;
#endif
	app_system_data.timezone        = 0;
	app_system_data.aspect 			= APP_SYS_ASPECT_4_3_e;
#ifdef APP_CONFIG_LANGUAGE_ENGLISH
	app_system_data.language 		= APP_SYS_LANGUAGE_ENGLISH_e;
#else
	app_system_data.language 		= APP_SYS_LANGUAGE_CHINESE_e;
#endif
	app_system_data.g_AvType 		= APP_CHN_AV_TYPE_TV_e;
 	app_system_data.g_GroupType 	= APP_CHN_GROUP_TYPE_ALL_e;
	app_system_data.g_CurChnNo 		= 0;
//	g_ChnNum 		= 0;
 	app_system_data.g_MuteState 	= APP_AUDIO_UNMUTE_STATE;
 	isNeedReCreateChnList = 1;

	app_system_data.dba_main_freq 	= APP_MAIN_FREQ_MHZ;
	app_system_data.dba_nvod_freq 	= APP_NVOD_FREQ_MHZ;
	app_system_data.dba_broadcast_freq = APP_DB_FREQ_MHZ;
	app_system_data.dba_ad_freq = APP_AD_FREQ_MHZ;
	app_system_data.dba_stock_freq = APP_STOCK_FREQ_MHZ;
	
	dba_db_freq 	= APP_MAIN_FREQ_MHZ;
	app_system_data.dba_area_lock 	= 0;

	dba_ota_freq 	= APP_OTA_FREQ_MHZ;
	dba_ota_symb 	= 6875;
	dba_ota_pid 	= APP_SYS_UPDATE_PID;//6000;//256;
	dba_ota_qam 	= 2;
	dba_ota_flag    = APP_OTA_UPDATE_FLAG_NONE;
	dba_ota_type 	= APP_OTA_TYPE_NONE;
	dba_ota_fac 	=  APP_NIT_FJ_MANU_ID;
	dba_ota_hwversion = APP_SYS_HARD_VERSION;
	dba_ota_swversion = APP_SYS_SOFT_VERSION;

/*这些值是不能被恢复的*/	
	//app_system_data.dba_factory_year = 2009;
	//app_system_data.dba_factory_week = 1;
	//app_system_data.dba_factory_num  = 0; 
	//app_system_data.dba_factory_stbType = 0;
	app_system_data.dba_flag = 0xff;
	/*if (app_system_data.SwitchItem == 1)
		app_system_data.SwitchItem = 1;
	else if (app_system_data.SwitchItem == 0)
		app_system_data.SwitchItem = 0;
	else
	*/
	app_system_data.SwitchItem = 0;
	app_system_data.AutoIP = 1;
	app_system_data.AutoDNS = 1;

	app_system_data.AutoRegionID= 0;
    app_system_data.RegionID= 255;

	app_system_data.g_mac_box_info.Reserved[0] = 0;
	app_system_data.g_mac_box_info.Reserved[1] = 0;
#ifdef DVT_CA
	memset(app_system_data.DVTAreaCode,0,4);
#endif
}

void APP_SYS_IPI_ReadStbInfo(void)
{
#if 0
	unsigned int crc_result;
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;

	LOADER_DB_INFO_S LOADER_DB;
	FYF_FLS_IOCTL_param.buffer = &LOADER_DB;
	FYF_FLS_IOCTL_param.bufferSize = sizeof(LOADER_DB_INFO_S);
	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_STBINFO, (BU32)&FYF_FLS_IOCTL_param);
	crc_result = CRC32(0,&LOADER_DB,sizeof(LOADER_DB_INFO_S)-4);
	if(crc_result == LOADER_DB.u32Crc)
	{
		APP_Printf(" crc check success \n");
	}

    APP_Printf("==================stb info================\n");
	APP_Printf("magic_number = 0x%x \n",LOADER_DB.magic_number);
	APP_Printf("u32CurSoftware = 0x%x \n",LOADER_DB.u32CurSoftware);
	APP_Printf("u32NewSoftware = 0x%x \n",LOADER_DB.u32NewSoftware);
	APP_Printf("u32HardwareVersion = 0x%x \n",LOADER_DB.u32HardwareVersion);
	APP_Printf("ota_frequency = %d \n",LOADER_DB.ota_frequency);
	APP_Printf("ota_symbol_rate = %d \n",LOADER_DB.ota_symbol_rate);
	APP_Printf("ota_modulation = %d \n",LOADER_DB.ota_modulation);
	APP_Printf("ota_pid = 0x%x \n",LOADER_DB.ota_pid);
	APP_Printf("ota_type = 0x%x \n",LOADER_DB.ota_type);
	dba_ota_freq = LOADER_DB.ota_frequency / 100;
	dba_ota_symb = LOADER_DB.ota_symbol_rate;
	dba_ota_pid = LOADER_DB.ota_pid;
	dba_ota_qam = LOADER_DB.ota_modulation;

    APP_Printf("==================stb info================\n");
#endif
	LOADER_DB_INFO_S LOADER_DB;
	BU08 u8stbID[17];
	BU32 u32sn;
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;
	FYF_FLS_IOCTL_param.buffer = &LOADER_DB;
	FYF_FLS_IOCTL_param.bufferSize = sizeof(LOADER_DB_INFO_S);

	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_LOADREINFO, (BU32)&FYF_FLS_IOCTL_param);

	if (LOADER_DB.u32Crc != CRC32(0,&LOADER_DB,sizeof(LOADER_DB_INFO_S)-4) 
	|| (LOADER_DB.u32FailedCnt != 0)
	|| (LOADER_DB.u32HardwareVersion != APP_SYS_HARD_VERSION)
	|| (LOADER_DB.u32DestroyFlag != 0)
	|| (LOADER_DB.ota_type != 0xff)
	|| (LOADER_DB.ota_pid != APP_SYS_UPDATE_PID)
	|| (LOADER_DB.magic_number != 0x4C4F4144)
	|| (LOADER_DB.ota_frequency != APP_MAIN_FREQ_MHZ * 100))
	{
		dba_ota_freq 	= APP_MAIN_FREQ_MHZ;
		dba_ota_symb 	= 6875;
		dba_ota_pid 	= APP_SYS_UPDATE_PID;//7000;//6000;//256;
		dba_ota_qam 	= 2;
		dba_ota_flag    = APP_OTA_UPDATE_FLAG_NONE;
		dba_ota_type 	= APP_OTA_TYPE_NONE;
		dba_ota_fac 	=  APP_OTA_TYPE_FAC;
		dba_ota_hwversion = APP_SYS_HARD_VERSION;
		dba_ota_swversion = APP_SYS_SOFT_VERSION;

		LOADER_DB.magic_number = 0x4C4F4144;
		LOADER_DB.ota_type = 0xff; 
		LOADER_DB.ota_pid = dba_ota_pid;
		LOADER_DB.ota_frequency = dba_ota_freq*100;
		LOADER_DB.ota_symbol_rate = dba_ota_symb;
		LOADER_DB.ota_modulation = dba_ota_qam;
		LOADER_DB.u32CurSoftware = APP_SYS_SOFT_VERSION;//0x01110101;
		LOADER_DB.u32HardwareVersion = APP_SYS_HARD_VERSION;//0x31100101;
		LOADER_DB.u32DestroyFlag = 0;
		LOADER_DB.u32FailedCnt = 0;

		LOADER_DB.u32Crc = CRC32(0,&LOADER_DB,sizeof(LOADER_DB_INFO_S)-4);

		FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_LOADREINFO, (BU32)&FYF_FLS_IOCTL_param);

	    APP_Printf("==================set stb inf success ================\n");
		APP_Printf("magic_number = 0x%x \n",LOADER_DB.magic_number);
		APP_Printf("u32CurSoftware = 0x%x \n",LOADER_DB.u32CurSoftware);
		APP_Printf("u32NewSoftware = 0x%x \n",LOADER_DB.u32NewSoftware);
		APP_Printf("u32HardwareVersion = 0x%x \n",LOADER_DB.u32HardwareVersion);
		APP_Printf("ota_frequency = %d \n",LOADER_DB.ota_frequency);
		APP_Printf("ota_symbol_rate = %d \n",LOADER_DB.ota_symbol_rate);
		APP_Printf("ota_modulation = %d \n",LOADER_DB.ota_modulation);
		APP_Printf("ota_pid = 0x%x \n",LOADER_DB.ota_pid);
		APP_Printf("ota_type = 0x%x \n",LOADER_DB.ota_type);
	}


	FYF_FLS_IOCTL_param.buffer = &u8stbID;
	FYF_FLS_IOCTL_param.bufferSize = sizeof(u8stbID);

   /*
      AABBCCDDEEFFFFFFF
      AA: OUI
      FFFFFFF SN
    */
   FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_STBINFO, (BU32)&FYF_FLS_IOCTL_param);

   if ((u8stbID[0] * 10 + u8stbID[1] != APP_NIT_FJ_MANU_ID)
   || (app_system_data.dba_factory_sequence !=  u8stbID[7] * 1000000000 + u8stbID[8] * 100000000 + u8stbID[9] * 10000000 + u8stbID[10] * 1000000 + u8stbID[11] * 100000 + u8stbID[12] * 10000 \
        + u8stbID[13] * 1000 +  u8stbID[14] * 100 + u8stbID[15] * 10 + u8stbID[16]))
   {
	   u8stbID[0] = (BU08)(APP_NIT_FJ_MANU_ID / 10);
	   u8stbID[1] = (BU08)(APP_NIT_FJ_MANU_ID % 10);
	   
	   u32sn = app_system_data.dba_factory_sequence;
	   u8stbID[7] = (BU08)(u32sn / 1000000000);
	   u32sn %= 1000000000;
	   u8stbID[8] = (BU08)(u32sn / 100000000);
	   u32sn %= 100000000;
	   u8stbID[9] = (BU08)(u32sn / 10000000);
	   u32sn %= 10000000;
	   u8stbID[10] = (BU08)(u32sn / 1000000);
	   u32sn %= 1000000;
	   u8stbID[11] = (BU08)(u32sn / 100000);
	   u32sn %= 100000;
	   u8stbID[12] = (BU08)(u32sn / 10000);
	   u32sn %= 10000;
	   u8stbID[13] = (BU08)(u32sn / 1000);
	   u32sn %= 1000;
	   u8stbID[14] = (BU08)(u32sn / 100);
	   u32sn %= 100;
	   u8stbID[15] = (BU08)(u32sn / 10);
	   u32sn %= 10;
	   u8stbID[16] = (BU08)(u32sn);
	   
	   FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_STBINFO, (BU32)&FYF_FLS_IOCTL_param);
	}

}



void APP_SYS_API_WirteUpdateFlag()
{

	LOADER_DB_INFO_S LOADER_DB;
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;
	FYF_FLS_IOCTL_param.buffer = &LOADER_DB;
	FYF_FLS_IOCTL_param.bufferSize = sizeof(LOADER_DB_INFO_S);
	LOADER_DB.magic_number = 0x4C4F4144;
	LOADER_DB.ota_type = 0x4c; 
	LOADER_DB.ota_pid = dba_ota_pid;
	LOADER_DB.ota_frequency = dba_ota_freq*100;
	LOADER_DB.ota_symbol_rate = dba_ota_symb;
	LOADER_DB.ota_modulation = dba_ota_qam;
	if (g_OtaInfo.softwareVersion <= APP_SYS_SOFT_VERSION)
		LOADER_DB.u32NewSoftware = APP_SYS_SOFT_VERSION + 1;//0x01110101;
	else
		LOADER_DB.u32NewSoftware = g_OtaInfo.softwareVersion;
	LOADER_DB.u32CurSoftware = APP_SYS_SOFT_VERSION;
	LOADER_DB.u32HardwareVersion = APP_SYS_HARD_VERSION;//0x31100101;
	LOADER_DB.u32DestroyFlag = 0;
	LOADER_DB.u32FailedCnt = 0;

	LOADER_DB.u32Crc = CRC32(0,&LOADER_DB,sizeof(LOADER_DB_INFO_S)-4);

	FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_LOADREINFO, (BU32)&FYF_FLS_IOCTL_param);

    APP_Printf("==================set stb inf success ================\n");
	APP_Printf("magic_number = 0x%x \n",LOADER_DB.magic_number);
	APP_Printf("u32CurSoftware = 0x%x \n",LOADER_DB.u32CurSoftware);
	APP_Printf("u32NewSoftware = 0x%x \n",LOADER_DB.u32NewSoftware);
	APP_Printf("u32HardwareVersion = 0x%x \n",LOADER_DB.u32HardwareVersion);
	APP_Printf("ota_frequency = %d \n",LOADER_DB.ota_frequency);
	APP_Printf("ota_symbol_rate = %d \n",LOADER_DB.ota_symbol_rate);
	APP_Printf("ota_modulation = %d \n",LOADER_DB.ota_modulation);
	APP_Printf("ota_pid = 0x%x \n",LOADER_DB.ota_pid);
	APP_Printf("ota_type = 0x%x \n",LOADER_DB.ota_type);

	//system("reboot");					
	system("reboot");
	FYF_API_thread_sleep(2000);	
	
}

void APP_SYS_IPI_ReadSysInfo(void)
{
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	BU16 crc,index;
	BU32 tnsValue;


	fyf_e2p_ioctl_param.buffer = (PU08)&app_system_data;
	fyf_e2p_ioctl_param.bufferSize = sizeof(app_system_data);
	APP_PRINT_DEBUG("fyf_e2p_ioctl_param.bufferSize = %d\n",sizeof(app_system_data));
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, APP_SYS_DATA_ADDR, (BU32)&fyf_e2p_ioctl_param);
#if 0
	crc = 0;
	for(index = 0;index<sizeof(app_system_data)-2;index++)
	{
		crc += fyf_e2p_ioctl_param.buffer[index];
	}
	APP_PRINT_DEBUG("crc = %d %d\n", crc, app_system_data.crc);
	if(crc != app_system_data.crc)//crc校验正确
#else
	if(E2P_CRC_DATA != app_system_data.crc)//crc校验正确
#endif
	{
		APP_SYS_IPI_SetSysValueToDefault ();
		APP_SYS_API_SaveSysInfo ();
	}

	APP_SYS_IPI_SetTVMode(app_system_data.tvMode);
	APP_SYS_IPI_SetVideoOut(app_system_data.videoOut);
	APP_SYS_IPI_SetAspectRatio(app_system_data.aspect);
	APP_SYS_IPI_SetLanguage(app_system_data.language);

	if(app_system_data.g_AvType > APP_CHN_AV_TYPE_RD_e)
		app_system_data.g_AvType = APP_CHN_AV_TYPE_TV_e;
	
	if(app_system_data.g_GroupType > APP_CHN_GROUP_TYPE_ALL_e)
		app_system_data.g_GroupType = APP_CHN_GROUP_TYPE_ALL_e; 
	g_AllTypeChnNo[app_system_data.g_AvType][app_system_data.g_GroupType] = app_system_data.g_CurChnNo;

	APP_SYS_IPI_SetMainFreq(app_system_data.dba_main_freq);
	APP_SYS_IPI_SetTransparence (app_system_data.transparence);
	tnsValue = app_system_data.g_MuteState;
	APP_Printf("app_system_data.g_MuteState = %d, app_system_data.g_AvType = %d,%d, app_system_data.g_CurChnNo = %d\n", app_system_data.g_MuteState, app_system_data.g_AvType, app_system_data.g_GroupType, app_system_data.g_CurChnNo);
	FYF_API_adec_ioctl(0, FYF_ADEC_SET_MUTE, &tnsValue);

	APP_SYS_IPI_ReadStbInfo();
	
}


/*-------------------------------------------------------------------------------

	Description:恢复初始值

	Parameters:

-------------------------------------------------------------------------------*/
void APP_SYS_API_SetToDefault(void)
{
	BS32 i = 0;
	FYF_API_printf("APP_SYS_API_SetToDefault()\n");
	APP_Com_API_StopProg(1);
	APP_ChnDatabase_API_ClearAllTypeChnNO();
//	ota_nit_monitor_disable();
	APP_SYS_IPI_SetSysValueToDefault();
	APP_SYS_API_SaveSysInfo();
	APP_SYS_IPI_ReadSysInfo();

	SQL_API_Semaphore_Wait();


//	SQL_API_Exec("DELETE FROM program;");//清空节目库
//	SQL_API_Exec("DELETE FROM eventReserved;");//清空预约
//	SQL_API_Exec("DELETE FROM broadcast;");//清空CAT
//	SQL_API_Exec("DELETE FROM sortTable;");//清空节目分类

	SQL_API_Exec("DROP TABLE program;");
	SQL_API_Exec("DROP TABLE eventReserved;");//清空预约
	SQL_API_Exec("DROP TABLE broadcast;");//清空CAT
	SQL_API_Exec("DROP TABLE sortTable;");//清空节目分类

	SQL_API_Exec("CREATE TABLE flash.sortTable \
(bouquetId INTEGER, bouquetName TEXT, PRIMARY KEY(bouquetId));");

	SQL_API_Exec("CREATE TABLE flash.program \
(service_id INTEGER(32) primary key, 编号 INTEGER, 节目名 TEXT, 加密 INTEGER(8), 加锁 INTEGER(8), \
喜爱 INTEGER(8), 删除 INTEGER(8), 跳过 INTEGER(8), 移动 INTEGER(8), \
声道 INTEGER(8), 音量补偿 INTEGER(8), 频率 INTEGER, 调制方式 TEXT, \
符号率 INTEGER, 服务类型 INTEGER, 视频PID INTEGER, \
音频PID INTEGER, PCRPID INTEGER, ecmPID INTEGER(16), audioEcmPID INTEGER(16), videoEcmPID INTEGER(16), \
sCAT_Sys_ID INTEGER(16), TSID INTEGER(32), \
分类ID TEXT, PID INTEGER, OriNetId INTEGER(16), 视频类型 INTEGER(16), 音频类型 INTEGER(16), 音量大小 INTEGER(8));");
	SQL_API_Exec("CREATE INDEX IF NOT EXISTS flash.iPro ON program(service_id, 编号);");
		
	SQL_API_Exec("CREATE TABLE flash.eventReserved \
(编号 INTEGER, 节目名 TEXT, 星期 INTEGER(8), 事件号 INTEGER, \
显示信息状态 INTEGER(8), 详细信息 TEXT, 事件内容 TEXT, \
预约状态 INTEGER(8), 开始小时 INTEGER(8), 开始分钟 INTEGER(8), \
结束小时 INTEGER(8), 结束分钟 INTEGER(8), 日期 INTEGER(32), 类型 INTEGER, PRIMARY KEY(编号, 事件号,日期,开始小时,开始分钟));");

	SQL_API_Exec("CREATE TABLE flash.broadcast \
(service_id INTEGER(32) primary key, 编号 INTEGER, 节目名 TEXT, \
声道 INTEGER(8), 频率 INTEGER, 调制方式 TEXT, \
符号率 INTEGER, 服务类型 INTEGER, 视频PID INTEGER, \
音频PID INTEGER, PCRPID INTEGER, TSID INTEGER(32), PID INTEGER, OriNetId INTEGER(16));");
	SQL_API_Exec("CREATE INDEX IF NOT EXISTS flash.iBroad ON program(service_id, 编号);");
	
	SQL_API_Semaphore_Release();
#ifdef ENABLE_NVOD
	APP_Nvod_IPI_ClearNvodInfo();
#endif
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
	for (i = APP_CHN_GROUP_TYPE_1_e; i < APP_CHN_GROUP_TYPE_26_e; i++)
	{
			APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, i);
	}
#ifndef NO_BROADCAST
	DBA_API_CreateBroadCastList();
#endif
	APP_ChnDatabase_IPI_SetSortCount(0);
//	APP_BAT_IPI_GetBatInfoFromDbs();
	//擦除两块节目库

#if (GUI_PLATFORM == Hisi)
	APP_Printf("Hisi\n");
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_SYSDATA, 1);
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_SYSDATABK, 1);
	//FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_VODINFO, 1);
#else
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, 0x1a0000, 0x20000);
#endif
//	app_system_data.dba_flag = 0xaa;

	APP_IPI_SetTime(2010, 1, 1, 5, 8, 0, 0);
	FYF_Ecm_Info_s serviceInfo;
	memset(&serviceInfo, 0, sizeof(FYF_Ecm_Info_s));
	FYF_API_SetCurEcmInfo(&serviceInfo);
//	ota_nit_monitor_enable(10/APP_NIT_MONITOR_SLEEP_TIME_SEC);//restart ota monitor in 20s
//	APP_TDT_IPI_StartDemuxTdt();
}


void APP_SYS_API_SaveTime()
{
	BU08 time[2];
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	fyf_e2p_ioctl_param.buffer = (PU08)time;
	fyf_e2p_ioctl_param.bufferSize = sizeof(time);
	APP_Printf("fyf_e2p_ioctl_param.bufferSize = %d\n",sizeof(time));
	time[0] = g_Hour;
	time[1] = g_Minute;
	if (time[0] >= 24)time[0] = 8;
	if (time[1] >= 60)time[1] = 0;
	printf("time 1 2 =%d %d\n", time[0], time[1]);
#ifdef BOX_ENCRYPT_E2PROM
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, 32, (BU32)&fyf_e2p_ioctl_param);
#else
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, 32, (BU32)&fyf_e2p_ioctl_param);
#endif

}


struct APP_E2_Info_s
{
	void *buffer;
	BU32 size;
	BU32 count;
	BS32 handle;
};

//数据保存在FLASH位置标志

void APP_IPI_OpenE2Handle(void)
{

}


/*
重启机顶盒
*/
void APP_SYS_API_Restart(void)
{
	FYF_API_power_on_process();
}

/*搜台结束后对TUNER设值*/
void APP_SYS_API_SetTunerAfterSearch(BS32 Frequency, BS32 symbol_rate, BS32 Modulation)
{
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);	
	APP_IPI_SaveTunerInfo(Frequency, symbol_rate, Modulation);
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
}

void APP_SYS_API_NotifyNitUpdated(void)
{
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_NIT_VER_UPDATED, 0, 0);
}

void APP_SYS_API_NotifyOTA_Detected(void)
{
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_OTA_DESC_DETECTED, 0, 0);
}

#if 0
ChnDataBase_ChnInfo_S defaultdbase[] =
{
	{26280714, "奥妙世界", 			26280714, 0, 0, 0, 0, 0, 0, 0, 4, 5, 80, 8190, 8190, 1, 778, 6875, 2, 0, 0, 0, 0, 1,0,2,0},
	{65763,    "1K0dB_PULSE BAR2",  65763,    0, 0, 0, 0, 0, 0, 0, 1, 6,769, 768, 768,  1, 227, 6875, 2, 0, 0, 0, 0, 1,0,2,1},
	{131299,   "1K-10dB_100BARS1",  131299,   0, 0, 0, 0, 0, 0, 0, 1, 7,789, 788, 788,  1, 227, 6875, 2, 0, 0, 0, 0, 1,0,2,1},
	{393443,   "龙岩公共",          393443,   0, 0, 1, 0, 0, 0, 0, 1, 8,870, 869, 869,  1, 227, 6875, 2, 19154, 868, 0, 0, 1,0,2,1},
	{852195,   "NINE DIGITAL ",     852195,   0, 0, 0, 0, 0, 0, 0, 1, 9,1009,1008, 524, 1, 227, 6875, 2, 0, 0, 0, 0, 1,0,2,1},
	{459226,   "CCTV-1",            459226,   0, 0, 0, 0, 0, 0, 0, 3, 10,0, 33, 33,  1, 443, 6875, 2, 0, 0, 0, 0, 1,0,2,1},
};
//	{459226,   "CCTV-1",            459226,   0, 0, 0, 0, 0, 0, 0, 3, 768, 769, 512,  1, 474, 6875, 2, 0, 0, 0, 0, 1,},
#endif

ChnDataBase_ChnInfo_S defaultdbase[] =
{
	{0x2ca076f, "导视频道", 		0x2ca076f,  0, 0, 0, 0, 0, 0, 0, 4,1024, 4097, 4096, 4096, 1, 

714, 6875, 2, 0, 0, 0, 0, 1, 2, 0, 1, 0, 16},
	{0xe30600,   "1K0dB_PULSE BAR2",  0xe30600,     0, 0, 0, 0, 0, 0, 0, 1,261, 869, 868, 868,  1, 227, 6875, 2, 

0, 0, 0, 0, 1, 2, 0, 2, 0, 16},
	{0xe30500,   "1K-10dB_100BARS1",  0xe30500,     0, 0, 0, 0, 0, 0, 0, 1,260, 849, 848, 848,  1, 227, 6875, 2, 

0, 0, 0, 0, 1, 2, 0, 3, 0,16},
	{0xe30400,   "NINE DIGITAL",      0xe30400,     0, 0, 0, 0, 0, 0, 0, 1,259, 829, 828, 515,  1, 227, 6875, 2, 

0, 0, 0, 0, 1, 2, 0, 4, 0, 16},
	{0xe30900,   "CCTV-7",            0xe30900,     0, 0, 1, 0, 0, 0, 0, 1,264, 930,929, 520, 1, 227, 6875, 2, 

18946, 928, 0, 0, 1, 2, 0, 5, 0, 16},
	{0x1da03ef, "CCTV-音乐",         0x1da03ef,   0, 0, 1, 0, 0, 1, 0, 10,263, 710, 518, 8190,  1, 474, 6875, 

2, 19154, 55, 0, 0, 1, 2, 0, 6, 0, 16},
};

BU32 defaultdbpmtpid[] =
{
	1024,
	256,
	257,
	261,
	268,
	258,
};

BU32 defaultdb_num  = sizeof(defaultdbase)/sizeof(ChnDataBase_ChnInfo_S);
void APP_SYS_API_SetDefaultDbase(void)
{
	BU32 i = 0;
	APP_Com_API_StopProg(1);
	APP_ChnDatabase_API_ClearAllTypeChnNO();
	SQL_API_Semaphore_Wait();


//	SQL_API_Exec("DELETE FROM program;");//清空节目库
//	SQL_API_Exec("DELETE FROM eventReserved;");//清空预约
//	SQL_API_Exec("DELETE FROM broadcast;");//清空CAT
//	SQL_API_Exec("DELETE FROM sortTable;");//清空节目分类

	SQL_API_Exec("DROP TABLE program;");
	SQL_API_Exec("DROP TABLE eventReserved;");//清空预约
	SQL_API_Exec("DROP TABLE broadcast;");//清空CAT
	SQL_API_Exec("DROP TABLE sortTable;");//清空节目分类

	SQL_API_Exec("CREATE TABLE flash.sortTable \
(bouquetId INTEGER, bouquetName TEXT, PRIMARY KEY(bouquetId));");

	SQL_API_Exec("CREATE TABLE flash.program \
(service_id INTEGER(32) primary key, 编号 INTEGER, 节目名 TEXT, 加密 INTEGER(8), 加锁 INTEGER(8), \
喜爱 INTEGER(8), 删除 INTEGER(8), 跳过 INTEGER(8), 移动 INTEGER(8), \
声道 INTEGER(8), 音量补偿 INTEGER(8), 频率 INTEGER, 调制方式 TEXT, \
符号率 INTEGER, 服务类型 INTEGER, 视频PID INTEGER, \
音频PID INTEGER, PCRPID INTEGER, ecmPID INTEGER(16), audioEcmPID INTEGER(16), videoEcmPID INTEGER(16), \
sCAT_Sys_ID INTEGER(16), TSID INTEGER(32), \
分类ID TEXT, PID INTEGER, OriNetId INTEGER(16), 视频类型 INTEGER(16), 音频类型 INTEGER(16), 音量大小 INTEGER(8));");
	SQL_API_Exec("CREATE INDEX IF NOT EXISTS flash.iPro ON program(service_id, 编号);");
		
	SQL_API_Exec("CREATE TABLE flash.eventReserved \
(编号 INTEGER, 节目名 TEXT, 星期 INTEGER(8), 事件号 INTEGER, \
显示信息状态 INTEGER(8), 详细信息 TEXT, 事件内容 TEXT, \
预约状态 INTEGER(8), 开始小时 INTEGER(8), 开始分钟 INTEGER(8), \
结束小时 INTEGER(8), 结束分钟 INTEGER(8), 日期 INTEGER(32), 类型 INTEGER, PRIMARY KEY(编号, 事件号,日期,开始小时,开始分钟));");

	SQL_API_Exec("CREATE TABLE flash.broadcast \
(service_id INTEGER(32) primary key, 编号 INTEGER, 节目名 TEXT, \
声道 INTEGER(8), 频率 INTEGER, 调制方式 TEXT, \
符号率 INTEGER, 服务类型 INTEGER, 视频PID INTEGER, \
音频PID INTEGER, PCRPID INTEGER, TSID INTEGER(32), PID INTEGER, OriNetId INTEGER(16));");
	SQL_API_Exec("CREATE INDEX IF NOT EXISTS flash.iBroad ON program(service_id, 编号);");

	for(i=0;i<defaultdb_num;i++)
	{
		sprintf(g_StrSQL,"INSERT INTO program \
				(service_id, 编号, 节目名, 加密, 加锁, \
				喜爱, 删除, 跳过, 移动, 声道, \
				音量补偿, 频率, 调制方式, 符号率, 服务类型, \
				视频PID, 音频PID, PCRPID, ecmPID, audioEcmPID, \
				videoEcmPID, sCAT_Sys_ID, \
				TSID, PID,OriNetId, 视频类型, 音频类型, 音量大小) \
				VALUES(%d, %d, '%s', %d, 0, \
				0, 0, 0, 0, 1, \
				0, %d, %d, %d, %d, \
				%d, %d, %d, %d, %d, \
				%d, %d, \
				%d, %d,%d,%d,%d,16);",		
		defaultdbase[i].m_ProgId, defaultdbase[i].m_ProgLCN, defaultdbase[i].m_ChnName,defaultdbase[i].m_Ca,
		defaultdbase[i].m_FreqMHz, defaultdbase[i].m_QAM,defaultdbase[i].m_SymKbps, defaultdbase[i].m_style,
		defaultdbase[i].m_VideoPid,defaultdbase[i].m_AudioPid, defaultdbase[i].m_PcrPid, defaultdbase[i].m_EcmPID,
		defaultdbase[i].m_audioEcmPID, defaultdbase[i].m_videoEcmPID, defaultdbase[i].m_CAT_Sys_ID,
		defaultdbase[i].m_TsId, defaultdbase[i].m_pmtpid,defaultdbase[i].m_OriNetId, defaultdbase[i].m_VideoType, defaultdbase[i].m_AudioType);
		SQL_API_Exec(g_StrSQL);
	}
		
	
	SQL_API_Semaphore_Release();
#ifdef ENABLE_NVOD
	APP_Nvod_IPI_ClearNvodInfo();
#endif
	
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
	for (i = APP_CHN_GROUP_TYPE_1_e; i < APP_CHN_GROUP_TYPE_26_e; i++)
	{
			APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, i);
	}
#ifndef NO_BROADCAST
	DBA_API_CreateBroadCastList();
#endif
	APP_ChnDatabase_IPI_SetSortCount(0);
//	APP_BAT_IPI_GetBatInfoFromDbs();
	//擦除两块节目库

#if (GUI_PLATFORM == Hisi)
	APP_Printf("Hisi\n");
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_SYSDATA, 1);
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_SYSDATABK, 1);
	//FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_VODINFO, 1);
#else
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, 0x1a0000, 0x20000);
#endif
//	app_system_data.dba_flag = 0xaa;
	SQL_API_SaveToFlash("flash.db");
 

	APP_IPI_SetTime(2010, 1, 1, 5, 8, 0, 0);
	FYF_Ecm_Info_s serviceInfo;
	memset(&serviceInfo, 0, sizeof(FYF_Ecm_Info_s));
	FYF_API_SetCurEcmInfo(&serviceInfo);
	
}


void APP_SYS_API_ClearCAData(void)
{
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_CA, 1);
	FYF_API_fls_ioctl (FYF_FLS_IOCTL_ERASE, FYF_FLASH_DEVID_CABK, 1);
}



