#ifndef _APP_PARSER_H_
#define _APP_PARSER_H_



//最多一张表格中可以包含的信息量

#define MAX_DEMUX_PMT_CHANNEL 20 //最多可以同时多少个PMT并行过滤

#define SIE_CA_DESC    0x09
#define SIE_Search_GetSetionLen(x) (unsigned short)((((*(((unsigned char *)x)+1))&0x0f)<<8)|(*(((unsigned char *)x)+2)))

#define SDT_TABLE_NAME "SDT_TABLE"
#define PMT_PID_TABLE_NAME "PMT_PID"
#define NIT_TABLE_NAME "NIT_TABLE"
#define CAT_TABLE_NAME "CAT_TABLE"

typedef void (* APP_ParserFunc)(BU08 *section, BU32 channelId, BU32 filterId);

#ifndef DEFINED_ULONG
#define DEFINED_ULONG
typedef unsigned long ULONG;        /* 32 bits , range : 0 to 4294967295 */
#endif

#ifndef DEFINED_USHORT
#define DEFINED_USHORT
typedef unsigned short USHORT;      /* 16 bits , range : 0 to 65535 */
#endif

#ifndef BYTE //8bit
#define BYTE unsigned char
#endif


#ifndef DEFINED_BOOLEAN
#define DEFINED_BOOLEAN
typedef char BOOLEAN;               /* 8 bits , range : 0 to 255 */
#endif

#ifndef DEFINED_UINT
#define DEFINED_UINT
typedef unsigned int UINT;          /* 32 bits , range : 0 to 4294967295 */
#endif

#ifndef  INT
#define  INT            int
#endif

#ifndef DEFINED_SHORT
#define DEFINED_SHORT
typedef signed short SHORT;         /* 16 bits , range : -32768 to 32767 */
#endif

#ifndef DEFINED_CHAR
#define DEFINED_CHAR
typedef char CHAR;                  /* 8 bits , range : 0 to 255 */
#endif

typedef	union
{
	BU16	sWord16;
	struct
	{
		BU08	ucByte0;	/* LSB */
		BU08    ucByte1; 	/* MSB	*/
	} byte;
} SHORT2BYTE;

typedef	union 
{
   unsigned int	uiWord32;
   struct 
   {
		unsigned short   sLo16;
		unsigned short	 sHi16;
	} unShort;
} WORD2SHORT;

struct ParserFuncNode_s
{
	BU32 channelId;
	BU32 filterId;
	APP_ParserFunc ParserFunc;
	struct ParserFuncNode_s *next;
	BU08 isInUse;
};

/*
分析PMT表的结果
*/
typedef struct _PMT_Content_s
{
	BU08 m_ChnName[APP_CHN_NAME_LEN];
	BU32 videoPID;
	BU32 audioPID;
	BU32 bCA;
	BU32 avType;
	BU32 progNum;
	BU32 PCRPID;
	BU16 ecmPID;
	BU16 audioEcmPid;
	BU16 videoEcmPid;
	BU16 sCAT_Sys_ID;
	BU08 videotype;
	BU08 audiotype; 
} PMT_Content_s;
typedef PMT_Content_s * PMT_Content_sp;


/**************************************************************
分析CAT表的结果
**************************************************************/
typedef struct _CAT_Content_s
{
	BU16 wEMMPid;
	BU16 sCAT_Sys_ID;
} CAT_Content_s;
typedef CAT_Content_s * CAT_Content_sp;


struct TIME
{
// 	BU08 hour: 5;		//0-23
// 	BU08 minute: 6;	//0-59
// 	BU08 second: 6;	//0-59
	BU08 hour;		//0-23
	BU08 minute;	//0-59
	BU08 second;	//0-59
};

//eit short/extend event descriptor
struct EIT_EVENT_DESC
{
	// 1  short
	BU08 *lang;
	BU08 *sht_text;
	BU08  sht_len;
	
	//16 extend at most
	struct EIT_EXT_DESC
	{
		BU08 *text;
		BU08 len;
	} ext[16];
};

typedef struct _SectionNode_s
{
	BU08 isUsed;
	BU08 section;
	struct _SectionNode_s *next;
}SectionNode_s;


#endif





