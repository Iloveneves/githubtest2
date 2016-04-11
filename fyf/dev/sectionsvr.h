/***********************************************************************************
 *             Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sectionsvr.h
 * Description: SECTIONSVR模块内部使用的头文件，不对外输出
 *
 * History:
 * Version   Date             Author     DefectNum    Description
 * main1     2008-09-01   diaoqiangwei/d60770                
 ***********************************************************************************/


#ifndef  _SECTIONSVR_PRI_H
#define  _SECTIONSVR_PRI_H

#include "hi_type.h"

#include "sectionsvr_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */


/**************************公共数据结构****************************/    


typedef struct hiSECTIONSVR_FILTER_S
{
	HI_S32	s32datafilterID;
	HI_U32	u32filterID;											/*申请的物理滤波器句柄*/

	HI_U8	u8SectionType;
	HI_U8 	u8Useflag;
	HI_U8 	u8Runflag;
	
	HI_U8 	u8VersionNum;
	HI_U8 	u8VersionBack;

	HI_U32	u32TimeOut;										/*  TIMEOUT 超时时间*/
	HI_U32	u32TimeCount;									/*  时间计数*/

	HI_U32  	u32SecNum;
	HI_U32  	u32GotSecNum;
	
	HI_U8 	u8GotSecFlag[SECTION_DATA_SECTIONLEN];    		/* section索引，每一byte为1时代表相应的section已经收到*/

	HI_S16	 s16section_length[SECTION_DATA_BUFFERSIZE];	/* buffer index 记号列表所对应的长度 */
	HI_S16    s16section_buffindex[SECTION_DATA_BUFFERSIZE];	/* buffer index 记号列表*/
	
	HI_U16    u16section_next;
	HI_U8   	pu8section_buffer[4096*SECTION_DATA_BUFFERSIZE];	
	
}SECTIONSVR_FILTER_S;


typedef struct hiSECTIONSVR_SECTIONDATA_S
{
	HI_U16 	u16frontendid;

	HI_U32 	u32TSPID;     	
	HI_U32 	u32channelID;							/*申请的物理通道句柄*/
	
	HI_U8 	u8Runflag;										/* 0 - 没有过滤 1- 正在过滤*/
	HI_U8 	u8Channeltype;								/* 0 - SEC  1-  PES*/

	HI_U32	u32filternum;
	SECTIONSVR_FILTER_S  u32filtermatrix[SECTION_DATA_CHFILTER_MAX];
	
}SECTIONSVR_SECTIONDATA_S;



/*****************************函数声明*******************************/
static HI_VOID SectionTimerProc(HI_VOID *pArg);
static HI_S32 SectionStartTimer(HI_VOID);

static HI_VOID SectionStart(HI_S32 s32DataFilterIndex) ;
static HI_VOID SectionStop(HI_S32 s32DataFilterIndex) ;

static HI_VOID SectionTimeProcess(HI_VOID);
static HI_VOID SectionPostProcess(HI_S32 MsgID, HI_S32 channelID, HI_S32 filterid, HI_S32 sectionnum);
static HI_VOID SectionProProcess( HI_U32 SectionID,  HI_U32 FilterID,  HI_U8 * pu8buffer, HI_U32 u32len);
static HI_VOID SectionMainThread(HI_VOID);
static HI_VOID SectionReadThread(HI_S32 s32ReadID);

/*****************************外部函数*******************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /* _SECTIONSVR_PRI_H*/
