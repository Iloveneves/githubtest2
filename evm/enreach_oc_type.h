/*------------------------------------------------------*/
/*     File     : EnReach_oc_type.h                                                                   */
/*     Date     : 2005/9/1                                                                   */
/*     PURPOSE  : define data type                                                    */
/*--------------------------------------------------------------------------------------*/
#ifndef _DSMCC_OC_TYPE_H
#define _DSMCC_OC_TYPE_H

#ifndef  BOOL
#define  BOOL           int
#endif

#ifndef  BYTE
#define  BYTE           unsigned char
#endif

#ifndef  UCHAR
#define  UCHAR           unsigned char
#endif

#ifndef  CHAR
#define  CHAR           char
#endif

#ifndef  INT
#define  INT            int
#endif

#ifndef  CINT
#define  CINT           const int
#endif

#ifndef  UINT
#define  UINT           unsigned int
#endif

#ifndef  INT8
#define  INT8           char
#endif

#ifndef  UINT8
#define  UINT8          unsigned char
#endif

#ifndef  INT16
#define  INT16          short
#endif

#ifndef  UINT16
#define  UINT16         unsigned short
#endif

#ifndef  INT32
#define  INT32           int
#endif

#ifndef	 UINT32
#define  UINT32         unsigned int
#endif

#ifndef  SINT32
#define  SINT32          signed int
#endif

#ifndef  LONG
#define  LONG           long 
#endif

#ifndef	 ULONG
#define  ULONG         unsigned long
#endif

#ifndef TRUE
#define  TRUE    (BOOL) (1)
#endif

#ifndef FALSE
#define  FALSE   (BOOL) (!TRUE ) 
#endif

/*机顶盒厂商设置OC的服务信息*/
typedef struct _OC_SERVICE_INFO
{
    /*所有参数均用10进制表示,*/
    char bShareVod;/*0:数据广播*/
    /*bUseFstParam ： １，第一组参数有效；０，第二组参数有效；其他值参数错误*/
    UINT8 bUseFstParam;
    UINT16 usSrvId;		/*service id，两组参数均使用该参数*/
    /*****第一组参数开始*****/
    UINT16 usOriNetId;	/*Original Network id*/
    UINT16 usTsId;		/*transport stream id*/
    /*****第一组参数结束*****/
    /*****第二组参数开始*****/
    UINT32 uiFreq;		/*OC所在物理频点,KHz为单位;*/
    UINT16 uiSymRat; /*符号率,如6875*/
    UINT8   ucQam;		/*调制值,如3,代表64QAM*/
    /*****第二组参数结束*****/
}OC_SERVICE_INFO;
#endif
