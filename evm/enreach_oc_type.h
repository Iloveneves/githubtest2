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

/*�����г�������OC�ķ�����Ϣ*/
typedef struct _OC_SERVICE_INFO
{
    /*���в�������10���Ʊ�ʾ,*/
    char bShareVod;/*0:���ݹ㲥*/
    /*bUseFstParam �� ������һ�������Ч�������ڶ��������Ч������ֵ��������*/
    UINT8 bUseFstParam;
    UINT16 usSrvId;		/*service id�����������ʹ�øò���*/
    /*****��һ�������ʼ*****/
    UINT16 usOriNetId;	/*Original Network id*/
    UINT16 usTsId;		/*transport stream id*/
    /*****��һ���������*****/
    /*****�ڶ��������ʼ*****/
    UINT32 uiFreq;		/*OC��������Ƶ��,KHzΪ��λ;*/
    UINT16 uiSymRat; /*������,��6875*/
    UINT8   ucQam;		/*����ֵ,��3,����64QAM*/
    /*****�ڶ����������*****/
}OC_SERVICE_INFO;
#endif
