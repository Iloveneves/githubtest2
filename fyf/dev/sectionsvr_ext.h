/***********************************************************************************
 *             Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sectionsvr_pub.h
 * Description: SECTIONSVRģ��������
 *
 * History:
 * Version   Date             Author     DefectNum    Description
 * main\1    2006-12-04  g47171     NULL         Create this file.
 ***********************************************************************************/


#ifndef  _SECTIONSVR_PUB_H
#define  _SECTIONSVR_PUB_H

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */


/***************************** �궨�� ******************************/

#define CHANNEL_MAX						(1*32)	/* Ӳ��ͨ�������*/

#define SECTION_DATA_CHANNEL_SIZE  		(20)  	/* ����ͨ���ĸ���*/
#define SECTION_DATA_FILTER_SIZE  			(20)  	/* ���ݹ������ĸ���*/

#define SECTION_FILTER_DEPTH_MAX 			(16)		/* ���������������*/
#define SECTION_DATA_SECTIONLEN			(256)	/* PSI/SI  ���SECTION ����*/
#define SECTION_DATA_BUFFERSIZE			(10)		/* SETCION ��󻺴�����ռ�*/
#define SECTION_DATA_CHFILTER_MAX		(15)		/* ��CHANNEL�ɹҽ�FILTER����*/


/*********************************�������ݽṹ********************/    

typedef HI_VOID (*T_CommReadyCallback) (HI_S32 s32DataFilterid );
typedef HI_VOID (*T_CommSectionCallback) (HI_S32 s32DataFilterid, HI_U8 *pu8Buffer, HI_U32 u32BufferLength, HI_U8 u8tableid);
typedef HI_VOID (*T_CommTableCallback) (HI_S32 s32DataFilterid, HI_U8 u8VersionNumber, HI_BOOL bIsSuccess);

typedef struct hiMsG_MESSAGE_S
{
    HI_U32  u32MessageID;
    HI_S32  s32Para1; 
    HI_S32  s32Para2; 
    HI_S32  s32Para3; 
}MSG_MESSAGE_S;

/********************************************************************
  ��ʶ��SECTIONSVR_DATA_FILTER_S
  ���ͣ����ݽṹ
  Ŀ�ģ���ʾSECTION���ݹ��������ݣ�Ӧ��ͨ������������Ĺ���Ҫ��
  ���壺
 **********************************************************************/
typedef struct  hiSECTIONSVR_DATA_FILTER_S
{
	HI_S32 s32DataFilterID;     	/* ����ID */
	HI_U32 u32TSPID;     			/* TSPID */
	HI_U16 u16FilterDepth;    		/* ������ȣ�0xff - ����ȫ�����û����ã�����ʹ��DVB�㷨��fixme�� */
	
	HI_U16 frontendid;

	HI_U8  u8UseFlag;    			/* ռ�ñ�־��0 - δռ�� 1 - ռ�� */
	HI_U8  u8RunFlag;			/* ���˱�ǣ�0 - û���ڹ��� 1 - ���ڹ���*/
	HI_U8  u8DirTransFlag;		/* pre ǰֱ��͸����ǣ�0 - ��͸�� 1 - ͸�� */
	HI_U8  u8SectionType;		/* SECTION��0 - SEC 1 -  PES*/
	
	HI_U8 u8Match[SECTION_FILTER_DEPTH_MAX];     	
	HI_U8 u8Mask[SECTION_FILTER_DEPTH_MAX];    	
	HI_U8 u8Negate[SECTION_FILTER_DEPTH_MAX];    

	HI_U32 u32TimeOut;     	 /* ��ʱ����λ-�� 0- ��Զ�ȴ�*/

	HI_U32	u32FilterHandle;	/*����������˲������*/
	HI_U32	u32ChannelHandle; /*���������ͨ�����*/

	T_CommReadyCallback    funReadyFunCallback;    	/* ����׼���ص�������SECTION�������ڽ��յ�MESSAGE_SECTIONSVR_START��Ϣ�����  */
	T_CommSectionCallback  funSectionFunCallback; 	/* section��ɻص����� */
	T_CommTableCallback     funTableFunCallback; 		/* table��ɻص����� */
    
} SECTIONSVR_DATA_FILTER_S;


/*****************************��������*******************************/

HI_S32  SECSVR_SectionCreateTask(HI_VOID); 
HI_BOOL SECSVR_SectionTaskReady(HI_VOID);
HI_S32 SECSVR_SectionDestroyTask(HI_VOID);
HI_S32 SECSVR_SectionInitDataFilter(HI_VOID);
HI_S32 SECSVR_SectionAllocDataFilter(HI_VOID);
HI_VOID SECSVR_SectionFreeDataFilter(HI_S32 s32DataFilterID);
HI_S32 SECSVR_SectionSetDataFilter(HI_S32 s32DataFilterID, SECTIONSVR_DATA_FILTER_S  *pstruDataFilter);
HI_S32 SECSVR_SectionGetDataFilter (HI_S32 s32DataFilterID, SECTIONSVR_DATA_FILTER_S    *pstruDataFilter);
HI_S32 SECSVR_SectionStartDataFilter(HI_S32 s32DataFilterID);
HI_S32 SECSVR_SectionStopDataFilter(HI_S32 s32DataFilterID);

/************************************************************************/



#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /* _SECTIONSVR_PUB_H*/

