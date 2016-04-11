#ifndef _APP_PARSERNIT_H_
#define _APP_PARSERNIT_H_

#define TPOTA_SERIAL_NUM_MAX		5
#define	DVT_DL_DESC							0x85
#define DVT_NL_ID						    0x0040
#define	CABLE_DELIVERY_DESC				0x44

#ifndef BYTE //8bit
#define BYTE unsigned char
#endif

#ifndef DEFINED_UINT
#define DEFINED_UINT
typedef unsigned int UINT;          /* 32 bits , range : 0 to 4294967295 */
#endif

#ifndef  INT
#define  INT            int
#endif


/**************************************************************
分析NIT表的结果
**************************************************************/
typedef struct _NIT_Content_s	
{
	BU08 version;
	BU32 freqMHz;
	BU32 symKbps;
	BU32 qam;
	BU32 downloadPid;
	BU32 downloadType;
	BU16 hardwareVersion;
	BU16 softwareVersion;
	BU08 tableId;
	BU32 startSerialNo;
	BU32 endSerialNo;
}NIT_Content_s;
typedef NIT_Content_s * NIT_Content_sp;


typedef void (*Fun_HandleNitInfo)(NIT_Content_s *);
typedef BU32 (*Fun_ParserNit)(BU08 * section, Fun_HandleNitInfo HandleNitInfo);

#endif





