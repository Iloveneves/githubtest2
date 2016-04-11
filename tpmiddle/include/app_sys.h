#ifndef _APP_SYS_H_
#define _APP_SYS_H_

/*��ʶFLASH���Ƿ�������*/
#define IS_FLASH_HAS_DATA 0x77

/*OTA������ر�־*/
#ifdef BOX_ENCRYPT_E2PROM
#define APP_SYS_DATA_ADDR	0x80
#else
#define APP_SYS_DATA_ADDR	0x80//0x0
#endif
#define APP_SYS_SOFT_VERSION  (0x20003)	//����汾
#define APP_SYS_HARD_VERSION  (0x31100201)	//Ӳ���汾
#define APP_SYS_LOADER_VERSION (1)	
#define APP_SYS_SERIAL_NO  (5)			//���к�
#define APP_NIT_LY_MANU_ID 0x0040 //0x40			//NIT����ID
#define APP_NIT_LY_CABLE_ID 0x0044 
#define APP_NIT_FJ_MANU_ID 0x042f //0x40			//NIT����ID
#define APP_NIT_PUCSECTIONDATA 0xA1		//NIT˽���ֶ�������

#define APP_SYS_UPDATE_PID 0x1395	//5013

#define APP_NIT_MONITOR_SLEEP_TIME_SEC 5 // 2 sec

#define SYS_PRODUCT_DATE  "2009-02-25 14:59:00"
#define SYS_REMOTE_CODE   (0x01fe0000)

#define APP_SYS_STBTYPE    "NL-C5103"    //�������ͺ�
#define APP_SYS_STBCHIP    "HISI3110E"   //оƬ

#if defined(ENABLE_BROADCAST_IPANEL)
#define APP_SYS_BROADCAST  "��׳2.0"     //���ݹ㲥
#elif defined(ENABLE_BROADCAST_ENREACH)
#define APP_SYS_BROADCAST  "Ӱݰ��"     //���ݹ㲥
#endif

#if defined (CD_CA)
#define APP_SYS_CAINFO     "CDCAS3.0"    //CA��Ϣ
#elif defined (DVT_CA)
#define APP_SYS_CAINFO     "Sumavision CAS5.2"    //CA��Ϣ
#endif

#ifdef FLASH_8M
#define APP_SYS_FLASHSIZE  (8)           //FLASH��С
#else
#define APP_SYS_FLASHSIZE  (16)           //FLASH��С
#endif
#define APP_SYS_SDRAMSIZE  (64)          //SDRAM��С

typedef enum
{	
	APP_SYS_OTA_UD_NULL,//������
	APP_SYS_OTA_UD_MANUAL,//�ֶ�����
	APP_SYS_OTA_UD_FORCE//ǿ������
}APP_SYS_OTA_UD_E;


typedef enum
{
	APP_SYS_VOLUME_e,		//���� BU32
	APP_SYS_TV_MODE_e,		//������ʽ APP_SYS_TVMODE_E
	APP_SYS_VIDEOOUT_e,		//��Ƶ�����ʽ APP_SYS_VIDEO_OUT_E
	APP_SYS_ASPECT_e,		//��Ļ���� APP_SYS_ASPECT_E
	APP_SYS_TimeZone_e,     //ʱ������ 0������ʱ�䣬 1�����׼ʱ��
	APP_SYS_TRANSPARENCE_e,	//͸���� BU08
	APP_SYS_TIMEOUT_e,		//��Ϣ��ʾʱ��  BU08
	APP_SYS_LANGUAGE_e,		//���� APP_SYS_LANGUAGE_E
	APP_SYS_STB_INFO_e,		//�����л�����Ϣ APP_SYS_STB_INFO_S(��û��)
	APP_SYS_MENU_LOCK_e,	//�˵��� BU32
	APP_SYS_MENU_PSW_e,		//�˵����� BU32
	APP_SYS_CHN_LOCK_e,		//Ƶ���� BU32
	APP_SYS_CHN_PSW_e,		//Ƶ������ BU32
	APP_SYS_STARTUP_LOCK_e,	//������ BU32
    APP_SYS_STARTUP_PSW_e,  //��������BU32
	
	APP_SYS_MAIN_FREQ_e,	//��Ƶ�� BU32
	APP_SYS_AREA_LOCK_e,	//�������� BU32	
	APP_SYS_NVOD_FREQ_e,	//NVODƵ��
	APP_SYS_DB_FREQ_e,		//���ݹ㲥Ƶ��
    APP_SYS_AD_FREQ_e,      //���Ƶ��
    APP_SYS_STOCK_FREQ_e,   //��ƱƵ��
	
	APP_SYS_OTA_FREQ_e, 	//OTAƵ��
	APP_SYS_OTA_SYMB,    	//OTA������
	APP_SYS_OTA_PID,     	//OTA PID
	APP_SYS_OTA_QAM, 		//OTA QAM����ģʽ
	APP_SYS_OTA_TYPE,		//ota ��������
	
	APP_SYS_FACTORY_SN,		//���к�
	APP_SYS_FACTORY_YEAR,	//�������
	APP_SYS_FACTORY_WEEK, 	//�����ܺ�
	APP_SYS_FACTORY_STBTYPE,//�ͺ�
	APP_SYS_FACTORY_NUM, 	//���̺�
    APP_SYS_SwitchItem_e, 	//�����л�
    APP_SYS_AutoIP_e,       //�Զ�IP
    APP_SYS_AutoDNS_e,	    //�Զ�DNS
    APP_SYS_Region_SwitchItem_e,    //�Զ���ȡ����ID
    APP_SYS_RegionID_e,       //����ID
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

/*������ʽ*/
#if (APP_CONFIG_TVMODE == TVMODE_ALL)
typedef enum
{
	APP_SYS_TVMODE_AUTO_e = 0,//�Զ�
	APP_SYS_TVMODE_PAL_e,
	APP_SYS_TVMODE_NTSC_e,
	APP_SYS_TVMODE_SECAM_e
}APP_SYS_TVMODE_E;
#else
#if (APP_CONFIG_TVMODE == TVMODE_ONLY_PAL)
typedef enum
{
	APP_SYS_TVMODE_PAL_e = 0,
	APP_SYS_TVMODE_AUTO_e,//�Զ�
	APP_SYS_TVMODE_NTSC_e,
	APP_SYS_TVMODE_SECAM_e
}APP_SYS_TVMODE_E;
#else 
typedef enum
{
	APP_SYS_TVMODE_NTSC_e = 0,
	APP_SYS_TVMODE_PAL_e,
	APP_SYS_TVMODE_AUTO_e,//�Զ�
	APP_SYS_TVMODE_SECAM_e
}APP_SYS_TVMODE_E;
#endif
#endif

/*��Ļ����*/
typedef enum
{
	APP_SYS_ASPECT_16_9_e=0,//16:9	
	APP_SYS_ASPECT_4_3_e//4:3
}APP_SYS_ASPECT_E;

/*��Ƶ�����ʽ*/

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

/*����*/
typedef enum
{
	APP_SYS_LANGUAGE_ENGLISH_e=0,
	APP_SYS_LANGUAGE_CHINESE_e	
}APP_SYS_LANGUAGE_E;

/*��������Ϣ*/
typedef struct
{

	PU08 hwVer;			//Ӳ���汾
	PU08 swVer;			//����汾
	PU08 browser;	//������汾
	PU08 remote;		//ң�����汾
	PU08 manufacturer;	//������
	PU08 CAVer;			//CA�汾
	PU08 date;			//��������	
	PU08 loader;		//
	PU08 broadcast;		//���ݹ㲥
	PU08 guiVer;		//GUI�汾


}APP_SYS_STB_INFO_S;

typedef struct _tag_DATE_S
{
    BU32 m_mjd;//���ڱ���
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

/*��˼LOADER*/
#define LOADER_MAGIC_NUMBER 0x4C4F4144  /* LOAD */

typedef struct tagLoaderDbInfo
{
    /* Ӧ�ú�loader �佻�� �ı��� */
    BU32 magic_number; /* Loader DB Magic Number ���� */
    BU32 ota_type; /* ��������*/
    BU32 ota_pid; /* ������pid */
    BU32 ota_frequency; /* ������Ƶ��, ��λΪ10KHz */
    BU32 ota_symbol_rate; /* ������KHz */
    BU32 ota_modulation; /* ����ģʽ */
    BU32 u32NewSoftware; /* ������������汾�� */
    BU32 u32CurSoftware; /* ��ǰ����汾�� */
    BU32 download_data; /* �������� */
    BU32 u32FailedCnt; /* ����ʧ�ܴ��� */
    BU32 u32HardwareVersion; /* Ӳ���汾�� */
    BU32 u32DestroyFlag;  /* �����𻵱�־ */
    BU32 u32Crc; /* crc У��ֵ */
} LOADER_DB_INFO_S;



void APP_SYS_IPI_ReadSysInfo(void);
void APP_IPI_StartWriteE2Thread(void);
BU32 APP_SYS_IPI_GetMainFreq(void);



/*
���ø�������
*/
void APP_SYS_API_SetUpdateType(BU32 type);
/*
�õ���������
*/

/*
��������ñ������ڣ�ʱ��
*/
BU32 APP_SYS_API_GetLocalDateAndTime(APP_Date_S* DateAndTime);
/*
���/����ϵͳ����
*/
void APP_SYS_API_GetSysInfo(APP_SYS_TYPE_E type, BU32 *value);
void APP_SYS_API_SetSysInfo(APP_SYS_TYPE_E type, BU32 value);
/*
����: ����ϵͳ��ز�����ֵ
*/
BS32 APP_SYS_API_SaveSysInfo(void);
/*
�ָ���ʼֵ
*/
void APP_SYS_API_SetToDefault(void);
/*
����������
*/
void APP_SYS_API_Restart(void);
/*��̨�������TUNER��ֵ*/
void APP_SYS_API_SetTunerAfterSearch(BS32 Frequency, BS32 symbol_rate, BS32 Modulation);

/*
	(1)stbidΪ26 λ�ַ���ȫ����������ɣ���ʽ˵�����£�
	stbid  ��ʽ��AAAA  BB  CCC  DD  EE  X  FFFF  FFFF  FFFF
	AAAA				��λ����ָʾ��
	BB					�����г�����֤���
	CCC					��������������
	DD					����������
	EE					Ӳ���汾��
	X					ָʾ��12λ������ʶ������
	FFFF  FFFF  FFFF	����ǰһλ��X������ľ�����ֵ
	(2)Ϊ��ȡ���ݷ���ͱ�֤�洢��ʽ��һ���ԣ������ַ���ASII��ķ�ʽ�洢
	(3)��STBID = "21040010102052544c1e643a11"

	2104001010205200606e56bf83
*/
typedef struct mac_box_info_t 
{
	BU08    AreaIndicatorCode[4];/*"2104",'2'�Ƿ��ڵ�һ���ֽ�*/
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
	BU08 advVersion;//���İ汾
	BU08 advSaveFlag;//���ı���
	
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
	BU16 crc;			//crcУ��
}app_system_data_s;
extern app_system_data_s app_system_data;
#endif










