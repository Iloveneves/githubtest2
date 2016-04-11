#ifndef _APP_PARSERSDT_H_
#define _APP_PARSERSDT_H_

/**************************************************************
分析SDT表的结果
**************************************************************/
typedef struct _SDT_Content_s
{
	BU32 service_id;
	unsigned char service_type;
	signed char service_name[51]; 
	BU16 OriNetId;		/* 原始网络标识符 */
}SDT_Content_s;
typedef SDT_Content_s * SDT_Content_sp;





#endif








