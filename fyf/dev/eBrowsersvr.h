/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: eBrowsersvr.h
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-09-01   diaoqiangwei/d60770                
 ******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define EBROWSER_MAX_CHANNEL 20
#define EBROWSER_MAX_FILTER 15
#define EBROWSER_TIMEOUT    (10)          
#define EBROWSER_CHANNEL_BUFFLEN  4*1024

typedef void (*EBROWSER_CHANNEL_CALLBACK)(HI_U32 ch, HI_U32 filter, HI_U8 *buf, HI_S32 len); 


typedef enum hieBrowser_Filter_Status
{
	EBROWSER_FILTER_FREE = 0,
	EBROWSER_FILTER_ALLOCATE,
	EBROWSER_FILTER_RUN,	
}EBROWSER_FILTER_STATUS;


typedef enum hieBrowser_Channel_Status
{
	EBROWSER_CHANNEL_FREE = 0,
	EBROWSER_CHANNEL_ALLOCATE,
	EBROWSER_CHANNEL_PID_SET,
	EBROWSER_CHANNEL_OPEN
}EBROWSER_CHANNEL_STATUS;

typedef struct hi_eBrowser_Filter_S
{
	HI_U32 filterflag;
	SECTIONSVR_DATA_FILTER_S filter;
}EBROWSER_FILTER_S;

typedef struct hi_eBrowser_Channel_S
{
	HI_U32 channelflag;
	HI_U32 channelpid;
	HI_U32 frontendid;
	HI_U32 u32SectionType;			/*sec or pes*/
	EBROWSER_FILTER_S filtergrop[EBROWSER_MAX_FILTER];
	EBROWSER_CHANNEL_CALLBACK CallbackFunction;	
}EBROWSER_CHANNEL_S;


HI_S32 eBrowserSvr_Init();

HI_S32 eBrowserSvr_AllocateChannel(HI_U32 frontendid,HI_U32 *pChannelId,HI_U32 u32SectionType);

HI_S32 eBrowserSvr_FreeChannel(HI_U32 ulChannelId);

HI_S32 eBrowserSvr_RegisterChannelCallback(HI_U32 ulChannelId,EBROWSER_CHANNEL_CALLBACK CallbackFunction);

HI_S32 eBrowserSvr_RegisterCallback(EBROWSER_CHANNEL_CALLBACK CallbackFunction);

HI_S32 eBrowserSvr_AllocateFilter(HI_U32 ulChannelId,HI_U32 *pFilterId);

HI_S32 eBrowserSvr_FreeFilter(HI_U32 ulChannelId,HI_U32 ulFilterId);

HI_S32 eBrowserSvr_SetFilter(HI_U32 ulChannelId,HI_U32 ulFilterId,HI_U8  *u8Match,HI_U8  *pMask, HI_U8 *u8Negate,HI_U32 ulDataLength);

HI_S32 eBrowserSvr_EnableFilter(HI_U32 ulChannelId,HI_U32 ulFilterId);

HI_S32 eBrowserSvr_DisableFilter(HI_U32 ulChannelId,HI_U32 ulFilterId);

HI_S32 eBrowserSvr_SetChannelPid(HI_U32 ulChannelId,HI_U16 usPid);

HI_S32 eBrowserSvr_GetChannelByPid(HI_U32 *ulChannelId,HI_U16 usPid);

HI_S32 eBrowserSvr_OpenChannel(HI_U32 ulChannelId);

HI_S32 eBrowserSvr_CloseChannel(HI_U32 ulChannelId);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

