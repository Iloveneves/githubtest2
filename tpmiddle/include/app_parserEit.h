#ifndef _APP_PARSEREIT_H_
#define _APP_PARSEREIT_H_

#define SAME_EPG_NUM_STOP 150
#define EVENT_NAME_LENGTH 50
#define DETAIL_INFO_LENGTH 1024



#define IS_SCH(tid)	(((tid&0xf0) == SI_EIT_TABLE_ID_ASCH)||((tid&0xf0) == SI_EIT_TABLE_ID_OSCH))
#define IS_PF(tid)	((tid == SI_EIT_TABLE_ID_APF)||(tid == SI_EIT_TABLE_ID_OPF))
#define SUCCESS         0       /* Success return */
#define ERR_FAILUE		-9		/* Common error, operation not success */
#define ERR_NO_MEM      -1      /* Not enough memory error */

#define SI_MAX_EPG_LANGUAGE_COUNT                        1

#define SI_EIT_TABLE_ID_APF		0x4E
#define SI_EIT_TABLE_ID_OPF		0x4F
#define	EXTENTED_EVENT_DESC 	        0x4E
#define CONTENT_DESC			         0x54
 /*========================================================
				
========================================================*/
typedef enum 
{
	Demux_Eit_Sate_Stop,
	Demux_Eit_Sate_PF,
	Demux_Eit_Sate_All,
	Demux_Eit_Sate_CurChn_All
}Demux_Eit_Sate_e;

typedef enum 
{
	PRESENT_EVENT 	= 0,
	FOLLOWING_EVENT = 1,
	SCHEDULE_EVENT 	= 2
	
}DB_EVENT_TYPE;

typedef struct _tagEitTempDateTime_S
{
	BU32 StartMjd;
	BU32 EndMjd;
	
	BU08 StartHour; 
	BU08 StartMin; 
	BU08 StartSec;
	BU08 StartWeekDay;
	
	BU08 EndHour; 
	BU08 EndMin; 
	BU08 EndSec;
	BU08 EndWeekDay;

}EitTempDateTime_S;



/**************************************************************
分析EIT表的结果
**************************************************************/
typedef struct _EIT_Content_s
{
	BU32 service_id;
	unsigned short event_id;
	BU08 eventType;
	BU08 week;
	BU32 day;
	BU08 eventName[EVENT_NAME_LENGTH];
	unsigned char startHour;
	unsigned char startMinute;
	unsigned char endHour;
	unsigned char endMinute;
	BU08 hasDetail;
	BU08 detailInfo[DETAIL_INFO_LENGTH];
} EIT_Content_s;
typedef EIT_Content_s * EIT_Content_sp;

struct DB_NODE
{
	BU16 service_id;
	BU16 event_id;
// 	BU08 event_type: 2;	//DB_EVENT_TYPE
// 	BU08 status: 2;		//NODE_STATUS
// 	BU08 nibble: 4;		//content descriptor: content_nibble1
	BU08 event_type;	//DB_EVENT_TYPE
	BU08 status;		//NODE_STATUS
	BU08 nibble;		//content descriptor: content_nibble1
	BU16 mjd_num;
	struct TIME time;
	struct TIME duration;
	
	struct EPG_MULTI_LANG
	{		
		BU08 *text_char;	//[event name] + [short event text char] + [extend event text char]
		BU16  text_length;
		BU16 lang_code2;	//!!!NOTE: converted from 3 byte of ISO_639_language_code
	}lang[SI_MAX_EPG_LANGUAGE_COUNT];
	
};


#endif










