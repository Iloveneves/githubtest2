/***********************************************************************************
 *             Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sectionsvr_pub.h
 * Description: SECTIONSVR模块对外输出
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


/***************************** 宏定义 ******************************/

#define CHANNEL_MAX						(1*32)	/* 硬件通道最大数*/

#define SECTION_DATA_CHANNEL_SIZE  		(20)  	/* 数据通道的个数*/
#define SECTION_DATA_FILTER_SIZE  			(20)  	/* 数据过滤器的个数*/

#define SECTION_FILTER_DEPTH_MAX 			(16)		/* 过滤器最大过滤深度*/
#define SECTION_DATA_SECTIONLEN			(256)	/* PSI/SI  最大SECTION 个数*/
#define SECTION_DATA_BUFFERSIZE			(10)		/* SETCION 最大缓存个数空间*/
#define SECTION_DATA_CHFILTER_MAX		(15)		/* 单CHANNEL可挂接FILTER个数*/


/*********************************公共数据结构********************/    

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
  标识：SECTIONSVR_DATA_FILTER_S
  类型：数据结构
  目的：标示SECTION数据过滤器内容，应用通过这个提出具体的过滤要求
  定义：
 **********************************************************************/
typedef struct  hiSECTIONSVR_DATA_FILTER_S
{
	HI_S32 s32DataFilterID;     	/* 过滤ID */
	HI_U32 u32TSPID;     			/* TSPID */
	HI_U16 u16FilterDepth;    		/* 过滤深度，0xff - 数据全部用用户设置，否则使用DVB算法（fixme） */
	
	HI_U16 frontendid;

	HI_U8  u8UseFlag;    			/* 占用标志，0 - 未占用 1 - 占用 */
	HI_U8  u8RunFlag;			/* 过滤标记，0 - 没有在过滤 1 - 正在过滤*/
	HI_U8  u8DirTransFlag;		/* pre 前直接透传标记，0 - 不透传 1 - 透传 */
	HI_U8  u8SectionType;		/* SECTION，0 - SEC 1 -  PES*/
	
	HI_U8 u8Match[SECTION_FILTER_DEPTH_MAX];     	
	HI_U8 u8Mask[SECTION_FILTER_DEPTH_MAX];    	
	HI_U8 u8Negate[SECTION_FILTER_DEPTH_MAX];    

	HI_U32 u32TimeOut;     	 /* 超时，单位-秒 0- 永远等待*/

	HI_U32	u32FilterHandle;	/*申请的物理滤波器句柄*/
	HI_U32	u32ChannelHandle; /*申请的物理通道句柄*/

	T_CommReadyCallback    funReadyFunCallback;    	/* 过滤准备回调函数，SECTION主任务在接收到MESSAGE_SECTIONSVR_START消息后调用  */
	T_CommSectionCallback  funSectionFunCallback; 	/* section完成回调函数 */
	T_CommTableCallback     funTableFunCallback; 		/* table完成回调函数 */
    
} SECTIONSVR_DATA_FILTER_S;


/*****************************函数声明*******************************/

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

