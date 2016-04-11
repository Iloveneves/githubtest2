#ifndef _APP_PARSERPAT_H_
#define _APP_PARSERPAT_H_


/**************************************************************
分析PAT表的结果
**************************************************************/
#define START_DEMUX_PAT 1
#define START_DEMUX_SDT 2
#define START_DEMUX_PMT 3
#define START_DEMUX_EIT 4
#define START_DEMUX_NIT 5



typedef struct _PAT_Content_s
{
	BU32 service_id;
	unsigned short service_PID;
	unsigned long  ts_id;
}PAT_Content_s;
typedef PAT_Content_s * PAT_Content_sp;


typedef struct _PAT_AreaCode
{
	BU32 area_code;
	BU32 service_id;
}PAT_AreaCode;




#endif








