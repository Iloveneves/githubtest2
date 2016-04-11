#ifndef _APP_SYS_H_
#define _APP_SYS_H_

/*标识FLASH中是否有数据*/
#define IS_FLASH_HAS_DATA 0x77

/*OTA升级相关标志*/
#ifdef BOX_ENCRYPT_E2PROM
#define APP_SYS_DATA_ADDR	0x80
#else
#define APP_SYS_DATA_ADDR	0x80//0x0
#endif
#define APP_SYS_SOFT_VERSION  (0x20003)	//软件版本
#define APP_SYS_HARD_VERSION  (0x31100201)	//硬件版本
#define APP_SYS_LOADER_VERSION (1)	
#define APP_SYS_SERIAL_NO  (5)			//序列号
#define APP_NIT_LY_MANU_ID 0x0040 //0x40			//NIT厂商ID
#define APP_NIT_LY_CABLE_ID 0x0044 
#define APP_NIT_FJ_MANU_ID 0x042f //0x40			//NIT厂商ID
#define APP_NIT_PUCSECTIONDATA 0xA1		//NIT私有字段描述符

#define APP_SYS_UPDATE_PID 0x1395	//5013

#define APP_NIT_MONITOR_SLEEP_TIME_SEC 5 // 2 sec

#define SYS_PRODUCT_DATE  "2009-02-25 14:59:00"
#define SYS_REMOTE_CODE   (0x01fe0000)

#define APP_SYS_STBTYPE    "NL-C5103"    //机顶盒型号
#define APP_SYS_STBCHIP    "HISI3110E"   //芯片

#if defined(ENABLE_BROADCAST_IPANEL)
#define APP_SYS_BROADCAST  "茁壮2.0"     //数据广播
#elif defined(ENABLE_BROADCAST_ENREACH)
#define APP_SYS_BROADCAST  "影莅驰"     //数据广播
#endif

#if defined (CD_CA)
#define APP_SYS_CAINFO     "CDCAS3.0"    //CA信息
#elif defined (DVT_CA)
#define APP_SYS_CAINFO     "Sumavision CAS5.2"    //CA信息
#endif

#ifdef FLASH_8M
#define APP_SYS_FLASHSIZE  (8)           //FLASH大小
#else
#define APP_SYS_FLASHSIZE  (16)           //FLASH大小
#endif
#define APP_SYS_SDRAMSIZE  (64)          //SDRAM大小

typedef enum
{	
	APP_SYS_OTA_UD_NULL,//不升级
	APP_SYS_OTA_UD_MANUAL,//手动升级
	APP_SYS_OTA_UD_FORCE//强制升级
}APP_SYS_OTA_UD_E;


typedef enum
{
	APP_SYS_VOLUME_e,		//音量 BU32
	APP_SYS_TV_MODE_e,		//电视制式 APP_SYS_TVMODE_E
	APP_SYS_VIDEOOUT_e,		//视频输出方式 APP_SYS_VIDEO_OUT_E
	APP_SYS_ASPECT_e,		//屏幕比例 APP_SYS_ASPECT_E
	APP_SYS_TimeZone_e,     //时区设置 0代表北京时间， 1代表标准时间
	APP_SYS_TRANSPARENCE_e,	//透明度 BU08
	APP_SYS_TIMEOUT_e,		//信息显示时间  BU08
	APP_SYS_LANGUAGE_e,		//语言 APP_SYS_LANGUAGE_E
	APP_SYS_STB_INFO_e,		//机顶盒基本信息 APP_SYS_STB_INFO_S(还没做)
	APP_SYS_MENU_LOCK_e,	//菜单锁 BU32
	APP_SYS_MENU_PSW_e,		//菜单密码 BU32
	APP_SYS_CHN_LOCK_e,		//频道锁 BU32
	APP_SYS_CHN_PSW_e,		//频道密码 BU32
	APP_SYS_STARTUP_LOCK_e,	//开机锁 BU32
    APP_SYS_STARTUP_PSW_e,  //开机密码BU32
	
	APP_SYS_MAIN_FREQ_e,	//主频点 BU32
	APP_SYS_AREA_LOCK_e,	//区域锁定 BU32	
	APP_SYS_NVOD_FREQ_e,	//NVOD频点
	APP_SYS_DB_FREQ_e,		//数据广播频点
    APP_SYS_AD_FREQ_e,      //广告频点
    APP_SYS_STOCK_FREQ_e,   //股票频点
	
	APP_SYS_OTA_FREQ_e, 	//OTA频点
	APP_SYS_OTA_SYMB,    	//OTA符号率
	APP_SYS_OTA_PID,     	//OTA PID
	APP_SYS_OTA_QAM, 		//OTA QAM调制模式
	APP_SYS_OTA_TYPE,		//ota 升级类型
	
	APP_SYS_FACTORY_SN,		//序列号
	APP_SYS_FACTORY_YEAR,	//生产年号
	APP_SYS_FACTORY_WEEK, 	//生产周号
	APP_SYS_FACTORY_STBTYPE,//型号
	APP_SYS_FACTORY_NUM, 	//厂商号
    APP_SYS_SwitchItem_e, 	//网络切换
    APP_SYS_AutoIP_e,       //自动IP
    APP_SYS_AutoDNS_e,	    //自动DNS
    APP_SYS_Region_SwitchItem_e,    //自动获取区域ID
    APP_SYS_RegionID_e,       //区域ID
    APP_SYS_StbID_e,//STBID
    APP_SYS_AreaIndicatorCode_e,
    APP_SYS_ManufactureCode_e,
	APP_SYS_StbFactoryIndicator_e,
	APP_SYS_StbType_e,
	APP_SYS_HwVersion_e,
	APP_SYS_StbIndicatorType_e,
	APP_SYS_MacAddressForward_e,
	APP_SYS_MacAddressBack_e,
	APP_SYS_DVTCAAreaCode_e,
	APP_SYS_AdvVersion_e,
	APP_SYS_AdvSaveFlag_e,
	APP_SYS_VolumeSave_e
	
}APP_SYS_TYPE_E;

/*电视制式*/
#if (APP_CONFIG_TVMODE == TVMODE_ALL)
typedef enum
{
	APP_SYS_TVMODE_AUTO_e = 0,//自动
	APP_SYS_TVMODE_PAL_e,
	APP_SYS_TVMODE_NTSC_e,
	APP_SYS_TVMODE_SECAM_e
}APP_SYS_TVMODE_E;
#else
#if (APP_CONFIG_TVMODE == TVMODE_ONLY_PAL)
typedef enum
{
	APP_SYS_TVMODE_PAL_e = 0,
	APP_SYS_TVMODE_AUTO_e,//自动
	APP_SYS_TVMODE_NTSC_e,
	APP_SYS_TVMODE_SECAM_e
}APP_SYS_TVMODE_E;
#else 
typedef enum
{
	APP_SYS_TVMODE_NTSC_e = 0,
	APP_SYS_TVMODE_PAL_e,
	APP_SYS_TVMODE_AUTO_e,//自动
	APP_SYS_TVMODE_SECAM_e
}APP_SYS_TVMODE_E;
#endif
#endif

/*屏幕比例*/
typedef enum
{
	APP_SYS_ASPECT_16_9_e=0,//16:9	
	APP_SYS_ASPECT_4_3_e//4:3
}APP_SYS_ASPECT_E;

/*视频输出方式*/

#if (APP_CONFIG_VIDEO_OUTPUT == VIDEO_OUTPUT_YC)
typedef enum
{
	APP_SYS_VIDEO_OUT_YC_e = 0,
	APP_SYS_VIDEO_OUT_YUV_e
}APP_SYS_VIDEO_OUT_E;
#else
typedef enum
{
	APP_SYS_VIDEO_OUT_YUV_e = 0,
	APP_SYS_VIDEO_OUT_YC_e	
}APP_SYS_VIDEO_OUT_E;
#endif

/*语言*/
typedef enum
{
	APP_SYS_LANGUAGE_ENGLISH_e=0,
	APP_SYS_LANGUAGE_CHINESE_e	
}APP_SYS_LANGUAGE_E;

/*机顶盒信息*/
typedef struct
{

	PU08 hwVer;			//硬件版本
	PU08 swVer;			//软件版本
	PU08 browser;	//浏览器版本
	PU08 remote;		//遥控器版本
	PU08 manufacturer;	//制造商
	PU08 CAVer;			//CA版本
	PU08 date;			//出厂日期	
	PU08 loader;		//
	PU08 broadcast;		//数据广播
	PU08 guiVer;		//GUI版本


}APP_SYS_STB_INFO_S;

typedef struct _tag_DATE_S
{
    BU32 m_mjd;//日期编码
	BU16 m_year;
	BU08 m_month;
	BU08 m_day;
	BU08 m_weekday;
	BU08 m_hour;
	BU08 m_min;
	BU08 m_sec;
	
}APP_Date_S;

#define APP_OTA_TYPE_FAC 		0x55
#define APP_OTA_TYPE_NONE 		0x28
#define APP_OTA_TYPE_AUTO   	0x93
#define APP_OTA_TYPE_MANUAL 	0x47
#define APP_OTA_RECOGNITION 	0x74625342

#define APP_OTA_UPDATE_FLAG_NONE	0x49
#define APP_OTA_UPDATE_FLAG_START	0x67
typedef struct _APP_BOOT_INFO
{
	BU08	ota_flag;		
	BU08	ota_qam;
	BU08 	table_id; 	
	BU08	download_type;			
	BU16	ota_pid;
	BU16  	stb_id;

	BU32	ota_frequency;
	BU32	ota_symbol;

	BU32  	hardware_version; 	
	BU32 	software_version;

	BU08	vdac_out[6];
	BU08	tv_mode;
	BU08	reserved[25];
	BU32	ota_count;		
	BU32	recognition;
}APP_BOOT_INFO;

/*------
OTA
--------*/
typedef struct _OTA_Info_S
{
	BU32 downloadPid;
	BU32 downloadType;
	BU32 hardwareVersion;
	BU32 softwareVersion;
	BU08 tableId;
	BU32 startSerialNo;
	BU32 endSerialNo;
	BU32 freqMHz;
	BU32 symKbps;
	BU32 qam;
}OTA_Info_S;

/*海思LOADER*/
#define LOADER_MAGIC_NUMBER 0x4C4F4144  /* LOAD */

typedef struct tagLoaderDbInfo
{
    /* 应用和loader 间交互 的变量 */
    BU32 magic_number; /* Loader DB Magic Number 名称 */
    BU32 ota_type; /* 升级类型*/
    BU32 ota_pid; /* 升级的pid */
    BU32 ota_frequency; /* 升级的频点, 单位为10KHz */
    BU32 ota_symbol_rate; /* 符号率KHz */
    BU32 ota_modulation; /* 调制模式 */
    BU32 u32NewSoftware; /* 被升级的软件版本号 */
    BU32 u32CurSoftware; /* 当前软件版本号 */
    BU32 download_data; /* 下载日期 */
    BU32 u32FailedCnt; /* 升级失败次数 */
    BU32 u32HardwareVersion; /* 硬件版本号 */
    BU32 u32DestroyFlag;  /* 主程损坏标志 */
    BU32 u32Crc; /* crc 校验值 */
} LOADER_DB_INFO_S;



void APP_SYS_IPI_ReadSysInfo(void);
void APP_IPI_StartWriteE2Thread(void);
BU32 APP_SYS_IPI_GetMainFreq(void);



/*
设置更新类型
*/
void APP_SYS_API_SetUpdateType(BU32 type);
/*
得到更新类型
*/

/*
描述：获得本地日期，时间
*/
BU32 APP_SYS_API_GetLocalDateAndTime(APP_Date_S* DateAndTime);
/*
获得/设置系统参数
*/
void APP_SYS_API_GetSysInfo(APP_SYS_TYPE_E type, BU32 *value);
void APP_SYS_API_SetSysInfo(APP_SYS_TYPE_E type, BU32 value);
/*
功能: 保存系统相关参数的值
*/
BS32 APP_SYS_API_SaveSysInfo(void);
/*
恢复初始值
*/
void APP_SYS_API_SetToDefault(void);
/*
重启机顶盒
*/
void APP_SYS_API_Restart(void);
/*搜台结束后对TUNER设值*/
void APP_SYS_API_SetTunerAfterSearch(BS32 Frequency, BS32 symbol_rate, BS32 Modulation);

/*
	(1)stbid为26 位字符，全部由数字组成，格式说明如下：
	stbid  格式：AAAA  BB  CCC  DD  EE  X  FFFF  FFFF  FFFF
	AAAA				四位地区指示码
	BB					机顶盒厂家认证编号
	CCC					机顶盒生产批次
	DD					机顶盒类型
	EE					硬件版本号
	X					指示后12位机顶盒识别类型
	FFFF  FFFF  FFFF	根据前一位（X）定义的具体数值
	(2)为读取数据方便和保证存储格式的一致性，采用字符串ASII码的方式存储
	(3)如STBID = "21040010102052544c1e643a11"

	2104001010205200606e56bf83
*/
typedef struct mac_box_info_t 
{
	BU08    AreaIndicatorCode[4];/*"2104",'2'是放在第一个字节*/
	BU08    ManufactureCode[2];/*"00"*/
	BU08    StbFactoryIndicator[3];/*"101"*/
	BU08    StbType[2];/*"02"*/
	BU08    HwVersion[2];/*"05"*/
	BU08    StbIndicatorType[1];/*"2"*/
	BU08   	ucMacAddress[12];/*"54 4c 1e 64 3a 11"*/
	BU08    Reserved[2];/*"00"*/
} MAC_BOX_INFO_STRUCT;/*zsy:080719*/

typedef struct
{
	BU08 g_MuteState;
	BU08 g_AvType;
	BU08 g_GroupType;
	BU08 volume_save_type;
	
	BU08 nitVersion;
	BU08 batVersion;
	BU08 advVersion;//广告的版本
	BU08 advSaveFlag;//广告的保存
	
	BU32 timezone;
	BU32 sysVolume;
	BU32 transparence;
	BU32 password;
	BU32 dba_area_lock;
	BU32 dba_factory_num;
	BU32 dba_factory_stbType;
	BU32 dba_factory_sequence;
	BU32 g_CurChnNo;

	BU08 munuLock;
	BU08 chnLock;
	BU08 videoOut;
	BU08 aspect;
	
	BU32 startupLock;
	
	BU16 dba_main_freq;
	BU16 dba_nvod_freq;
    BU16 dba_ad_freq;
	BU16 dba_broadcast_freq;

	BU32 dba_stock_freq;
	BU16 dba_factory_year;
	BU08 dba_factory_week;
	BU08 language;

	BU32 AutoRegionID;
	BU32 RegionID;

	MAC_BOX_INFO_STRUCT g_mac_box_info;
#ifdef DVT_CA
	BU08 DVTAreaCode [4];
#endif
	BU08 tvMode;
	BU08 dba_flag;
	BU08 infoTimeOut;
	BU08 SwitchItem;
	BU08 AutoIP;
	BU08 AutoDNS;
	BU16 crc;			//crc校验
}app_system_data_s;
extern app_system_data_s app_system_data;
#endif










