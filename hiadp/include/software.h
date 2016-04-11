

#ifndef _SOFTWARE_H
#define _SOFTWARE_H

#include "hi_type.h"
#include "hardware.h"


#define	USE_OTA					(0)	//0:no ota		1:use ota		
#define	USE_MTD_FILE				(1)	//0:use mtd	1:use file
#define 	APP_MTD					(3)

#define	DMX_TSPORT				(0)
#define	DMX_IDNUM					(0)

#define	SCI_PORT					(0)
#define 	SCI_PROTYPE				(0)

#define 	MAX_PROGNAME_LENGTH		(17)
#define 	MAX_AUDIO_LANGUAGE	 	(5)


#define	MAX_FRONTEND				(100)
#define 	MAX_SPEC					(8)
#define	MAX_INDEX					(8)
#define	MAX_TV						(100)
#define 	MAX_RADIO					(50)

#define 	MAX_URL_NUM  				(0)
#define 	MAX_FILE_NUM  				(0)

#define	DATAFLAG					(0x11)
#define	EPPROMADDRESS				(0x00)

/* system para struct */
typedef struct hiDB_Language_S
{
	HI_U8 menulanguage;
	HI_U8 audiolanguage;
	HI_U8 subtitlelanguage;
	HI_U8 epglanguage;
	
	HI_U8 languageuser1;
	HI_U8 languageuser2;
	HI_U8 languageuser3;
	HI_U8 languageuser4;
	
}DB_Language_S;

typedef struct hiDB_Video_S
{
	HI_U8 outputmodel;  
	HI_U8 videomode;		
	HI_U8 screenmode;	
	HI_U8 panscan_letterbox;	
	HI_U8 brightness;
	HI_U8 contrase;
	HI_U8 saturation;
}DB_Video_S;

typedef struct hiDB_UHF_S
{
	HI_U8 UHF_channelnum;
	HI_U8 UHF_palmode;
	HI_U8 UHF_type;
}DB_UHF_S;

typedef struct hiDB_Time_S
{
	HI_U8 timeoffset;				
	HI_U8 osdtimeout;				
	HI_U8 summertime;
	
}DB_Time_S;


typedef struct hiDB_Other_S
{
	HI_U8  opacity;
	HI_U8  audiosys;

	HI_U32 supportlanguage;			

	HI_U8 syspasswordenable;
	HI_U8 syspassword[8];			

	HI_U8 parentpasswordenable;	
	HI_U8 parentlevel;
	HI_U8 parentpassword[8];			
}DB_Other_S;


typedef struct hiDB_SystemParameter_S
{
	DB_Language_S 	sys_language;
	DB_Time_S 		sys_time;
	DB_Video_S 		sys_video;
	DB_UHF_S		sys_uhf;
	DB_Other_S 		sys_oth;
}DB_SystemParameter_S;



/* channel dot struct */
typedef struct hiDB_FRONTEND_S
{
	HI_U32	u32Frequency;            	
	HI_S32	s32OffsetFrequency;        
	HI_U32	u32SymbolRate;            	
	HI_S32	s32OffsetSymbolRate;      
	
	HI_U8    	u8IQflag;                		
	
	HI_U8    	u8Modulation;            	
	
	HI_U16    u16NetworkID;            	
	HI_U16    u16TsID;                		
	HI_U16    u16Reserved;            	
	
} DB_FRONTEND_S;



/* program struct */
typedef struct hiDB_VIDEOEX_S
{
	HI_U16 	u16VideoPid;
	HI_U8 	u8VideoEncType;
}DB_VIDEOEX_S;


typedef struct hiDB_AUDIOEX_S
{
	HI_U16 	u16audiopid;
	HI_U16	u16audiolan;
	HI_U8 	u8AudioEncType;
}DB_AUDIOEX_S;


typedef struct hiDB_TELETEXTEX_S
{
	HI_U16 	u16TeletextPid;
	HI_U16 	u16Telepage;
	
}DB_TELETEXTEX_S;


typedef struct hiDB_SUBTITLEEX_S
{
	HI_U16 	u16SubtitlePid;
	HI_U16 	u16Subpage;
	
}DB_SUBTITLEEX_S;



typedef struct  hiDB_PROGRAM_S
{
	HI_U16    u16FrontendID;           
	HI_U16    u16NetworkID;           
	HI_U16    u16TsID;               
	HI_U16    u16ServiceID;          

	HI_U8 	enServiceType; 			

	HI_S8   	ProgramName[MAX_PROGNAME_LENGTH];    

	HI_U16    u16PmtPid;           
	HI_U16    u16PcrPid;            

	HI_U32    u16ProgProperty;        

	HI_U16    u16AudioVol;            	
	HI_U8    	u8AudioChannel;    	

	DB_VIDEOEX_S 		VideoEX;
	DB_TELETEXTEX_S 	TeleTextEX;
	DB_SUBTITLEEX_S 	SubTitleEX;
	DB_AUDIOEX_S 		AudioEX[MAX_AUDIO_LANGUAGE];
	
	HI_U16    Reserved;
} DB_PROGRAM_S;

/* ip broadcast struct */
typedef struct hiDB_IPFRONTEND_S
{
	HI_U8    u8ProgName[256];       	
	HI_U8    u8ProgURL[256];        	
	HI_S32  s32port;

} DB_IPFRONTEND_S;


/* file struct */
typedef  struct hiDB_FILE_INFO_S
{
	HI_S8 FileName[256]; 
	HI_U8 FileType;	
}DB_FILE_INFO_S;

typedef  struct hiDB_CURRENT_INFO_S
{
	HI_U8 	u8progtype;
	HI_U8	u8favid;
	HI_U16	u16progid;
	HI_U16	u16progidradio;

	HI_U8 	u8progtyperecall;
	HI_U8	u8favidrecall;
	HI_U16	u16progidrecall;

	HI_U8	audiolevel;
	HI_U8	audiotrack;
	HI_U8	nitversionnum;
}DB_CURRENT_INFO_S;



typedef  struct hiDB_UPDATE_INFO_S
{
	HI_U16	u16downloadpid;
	HI_U16	u16downloadfreq;
	HI_U16	u16symbolrate;
	HI_U8	u8qam;

	HI_U8	u16stbid[2];
	HI_U16	u16stbtype;
	HI_U8	u32hwversion[4];
	HI_U8	u32swversion[4];
	
}DB_UPDATE_INFO_S;


#endif
