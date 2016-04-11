/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_demx.c
* Description: 该文件实现oc库定义的demux，flter接口
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
#include "fyf_def.h"
#include "fyf_demux.h"

//#include "hi_svr_dmxs.h"
//#include "hi_s2_mod.h"
#include "enreach_pub.h"
#include "enreach_oc_drv.h"



#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 0
#define CHN_MAX_NUM        (32)
#define CHN_INVALID_HDL    (-1)
#define CHN_INVALID_PID    (-1)
#define DMXS_POOL_SIZE     (64*1024)


typedef struct tagCHN_CALLBACK_PTR_S
{
    HI_U32 	u32ChnId;
    HI_U32 	u32Pid;
    DsmccOcSectionCallback pSectionCallback;
} CHN_CALLBACK_PTR_S;

/********************************** 静态变量定义 *********************/

static HI_BOOL 				s_bChnAttr_Ini  = HI_FALSE;        /* 通道属性初始化标记 */
static CHN_CALLBACK_PTR_S 	s_astChnCb_ptr[CHN_MAX_NUM] = { {CHN_INVALID_HDL, CHN_INVALID_PID, NULL}, };

static DsmccOcSectionCallback getSectionCallback(HI_U32 u32Chn)
{
    HI_S32 i = 0;

    for (i = 0; i < CHN_MAX_NUM; i++)
    {
        if (u32Chn != s_astChnCb_ptr[i].u32ChnId)
        {
            continue;
        }

        break;
    }

    /* 没有设置section数据接收回调函数 */

    if (CHN_MAX_NUM <= i)
    {
        return NULL;
    }

    return s_astChnCb_ptr[i].pSectionCallback;
}

static HI_S32 getChnNum(HI_U32 u32ChnId)
{
    HI_S32 i = 0;
    
    if (HI_FALSE == s_bChnAttr_Ini)
    {
        for (i = 0; i < CHN_MAX_NUM; i++)
        {
            s_astChnCb_ptr[i].u32ChnId         = CHN_INVALID_HDL;
            s_astChnCb_ptr[i].u32Pid           = CHN_INVALID_PID;
            s_astChnCb_ptr[i].pSectionCallback = NULL;
        }

        s_bChnAttr_Ini = HI_TRUE;
    }

    for (i = 0; i < CHN_MAX_NUM; i++)
    {
        if (u32ChnId != s_astChnCb_ptr[i].u32ChnId)
        {
            continue;
        }

        break;
    }

    return i;
}

static HI_S32 dmxCallback(HI_U32 u32ChHandle, HI_U32 u32FltHandle, 
                          HI_U8 *pu8DataAddr, HI_U32 u32Len, HI_U32 u32UserData)
{
    HI_U32 i = 0;
    DsmccOcSectionCallback pCallback;

    ENREACH_RETURN((NULL == pu8DataAddr || 0 == u32Len), HI_FAILURE);

    i = getChnNum(u32ChHandle);
    ENREACH_RETURN((CHN_MAX_NUM <= i), HI_FAILURE);

    pCallback = s_astChnCb_ptr[i].pSectionCallback;
    ENREACH_RETURN((NULL == pCallback), HI_FAILURE);

    pCallback(u32ChHandle, pu8DataAddr, u32Len);

    return HI_SUCCESS;
}
#else
static DsmccOcSectionCallback chn_callback_func[25];
#endif

/**
 * 创建通道
 * 
 * @param pChannelId        : 创建的通道ID
 * @param ulMaxFilterNumber : 该通道最多能邦定Filter个数
 * @param ulMaxFilterSize   : 最大Filter深度
 * @param ulBufferSize      : 通道接收Section数据的Buffer大小
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxAllocateSectionChannel(UINT32 *pChannelId, 
                                                      INT32  ulMaxFilterNumber,
                                                      INT32  ulMaxFilterSize,
                                                      UINT32 ulBufferSize)
{
#if 0
    HI_S32 s32Ret = 0;

    EVM_DEBUG("### allocate channel enter! ###\n");
    
    ENREACH_RETURN((HI_NULL == pChannelId || HI_NULL == ulBufferSize), OC_DEMUX_ERROR);

    s32Ret = HI_DMXS_AllocChn(0, DMXS_POOL_SIZE, HI_SVR_MODID_NULL, pChannelId);

	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

    EVM_DEBUG("### allocate channel leave, chanel id = %d! ### \n", *pChannelId);

    return OC_DEMUX_OK;    
#else
	HI_S32 s32Ret = 0;

	EVM_DEBUG("Enter");

	ENREACH_RETURN((HI_NULL == pChannelId || HI_NULL == ulBufferSize), OC_DEMUX_ERROR);
	s32Ret = FYF_API_demux_create_channel(ulBufferSize, FYF_DEMUX_DATA_PSI_CHANNEL, (BU32*)pChannelId, 0x1fff);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	EVM_INFO("allocate channel leave, chanel id = %d", *pChannelId);

	return OC_DEMUX_OK;
	
#endif

}

/**
 * 释放通道
 * 
 * @param ulChannelId : 通道ID
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxFreeSectionChannel(UINT32 ulChannelId)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_U32 i = 0;

    EVM_DEBUG("## DsmccOcDrvDemuxFreeSectionChannel enter ##\n");

    s32Ret = HI_DMXS_SetNofifyFun(HI_DMXS_NOTIFY_TYPE_CHN, ulChannelId, NULL, 0);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

    (HI_VOID)HI_DMXS_StopChn(ulChannelId);
    s32Ret = HI_DMXS_FreeChn(ulChannelId);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

    i = getChnNum(ulChannelId);
    ENREACH_RETURN((CHN_MAX_NUM <= i), OC_DEMUX_ERROR);

    s_astChnCb_ptr[i].u32ChnId = CHN_INVALID_HDL;
    s_astChnCb_ptr[i].pSectionCallback = NULL;
    s_astChnCb_ptr[i].u32Pid = CHN_INVALID_PID;

    EVM_DEBUG("## DsmccOcDrvDemuxFreeSectionChannel leave ##\n");

    return OC_DEMUX_OK;
#else

	HI_S32 s32Ret = 0;

	EVM_DEBUG("Enter, channelId:%u", ulChannelId);

	DsmccOcDrvDemuxRegisterChannelCallback(ulChannelId, 0);

	FYF_API_demux_stop_channel(ulChannelId);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	FYF_API_demux_destroy_channel(ulChannelId);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	chn_callback_func[ulChannelId] = 0;	
	
	EVM_INFO("Free ok, ChannelId:%d", ulChannelId);

	return OC_DEMUX_OK;
	
#endif

}

/**
 * Section数据回调函数
 * 
 * @param ulChannelId      : 创建的通道ID
 * @param CallbackFunction : section数据接收处理回调函数
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxRegisterChannelCallback(UINT32 ulChannelId,
                                                       DsmccOcSectionCallback CallbackFunction)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_U32 i = 0;

    EVM_DEBUG("## DsmccOcDrvDemuxRegisterChannelCallback enter ###\n");

    s32Ret = HI_DMXS_SetNofifyFun(HI_DMXS_NOTIFY_TYPE_CHN, ulChannelId, dmxCallback, 0);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

    i = getChnNum(CHN_INVALID_HDL);
    ENREACH_RETURN((CHN_MAX_NUM <= i), OC_DEMUX_ERROR);

    s_astChnCb_ptr[i].pSectionCallback = CallbackFunction;
    s_astChnCb_ptr[i].u32ChnId = ulChannelId;
    EVM_DEBUG("### set chan caback leave, chanel id = %d! ### \n", ulChannelId);

    return OC_DEMUX_OK;
#else
	EVM_DEBUG("Enter, channelId:%u", ulChannelId);
	chn_callback_func[ulChannelId] = CallbackFunction;
	return OC_DEMUX_OK;

#endif
}

void evm_data_callback(BU32 ch, BU32 filter, BU08 *buf, BS32 len)
{

//	EVM_ERROR("Enter, ch:%u, filter:%u, len:%d",ch,filter,len);

#if 0
	if (HI_TRD_ENREACH_IsExit())
	{
		for (i=0;i<25;i++)
		{
			if (chn_callback_func[i] != 0)
			{
				chn_callback_func[i](i,NULL, 0);		
			}
		}
	}
#endif

	if (chn_callback_func[ch] != 0)
	{
		chn_callback_func[ch](ch,buf,len);
	}
	
}


void HI_CleanCallback(void )
{
	int i = 0;

	EVM_ERROR("Enter");

	FYF_API_demux_set_notify(NULL);

#if 1
		for (i=0;i<25;i++)
		{
			if (chn_callback_func[i] != 0)
			{
				chn_callback_func[i](i, NULL, 0);		
				chn_callback_func[i] = 0;
			}
		}
	
#endif
}


/**
 * 分配Filter
 * 
 * @param ulChannelId      : 创建的通道ID
 * @param pFilterId        : 创建的Filter ID
 * @param ulFilterLength   : Filter深度
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxAllocateFilter(UINT32 ulChannelId,
                                              UINT32 *pFilterId,
                                              UINT32 ulFilterLength)
{
#if 0
    HI_S32 s32Ret = 0;
    EVM_DEBUG("### alloc filter enter, chanel id = %d, flt id = %d ! ### \n", ulChannelId, *pFilterId);

    s32Ret = HI_DMXS_AllocFilter(0, HI_SVR_MODID_NULL, pFilterId);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

    EVM_DEBUG("### allocate filter leave, chanel id = %d, flt id = %d ! ### \n", ulChannelId, *pFilterId);

    return OC_DEMUX_OK;
#else
	HI_S32 s32Ret = 0;

	EVM_INFO("channelId:%d", ulChannelId);
	s32Ret = FYF_API_demux_create_filter(ulChannelId, (BU32*)pFilterId);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
	EVM_INFO("allocate filter OK, channelId:%d, FilterId:%d", ulChannelId, *pFilterId);

	return OC_DEMUX_OK;
#endif
}

/**
 * 释放Filter
 * 
 * @param ulChannelId      : 创建的通道ID
 * @param ulFilterId       : 释放的Filter ID
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxFreeFilter(UINT32 ulChannelId,
                                          UINT32 ulFilterId)
{
#if 0
    HI_S32 s32Ret = 0;

    EVM_DEBUG("## DsmccOcDrvDemuxFreeFilter enter ###\n");

    s32Ret = HI_DMXS_DetachFilter(ulFilterId, ulChannelId);
    s32Ret |= HI_DMXS_FreeFilter(ulFilterId);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

    EVM_DEBUG("## DsmccOcDrvDemuxFreeFilter leave ###\n");

    return OC_DEMUX_OK;
#else
    HI_S32 s32Ret = 0;

    EVM_INFO("Enter, channelId:%d, FilterId:%d", ulChannelId, ulFilterId);

    s32Ret = FYF_API_demux_disable_filter(ulChannelId, ulFilterId);
    ENREACH_RETURN((FYF_ERR == s32Ret), OC_DEMUX_ERROR);

	s32Ret = FYF_API_demux_destroy_filter(ulChannelId, ulFilterId);
    ENREACH_RETURN((FYF_ERR == s32Ret), OC_DEMUX_ERROR);
	
    EVM_INFO("ok!");

    return OC_DEMUX_OK;
	
#endif

}

/**
 * 设置Filter
 * 
 * @param ulChannelId      : 创建的通道ID
 * @param ulFilterId       : 设置的Filter ID
 * @param pData            : Match
 * @param pMask            : Mask
 * @param ulDataLength     : depth
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxSetFilter(UINT32 ulChannelId,
                                         UINT32 ulFilterId,
                                         UINT8  *pData,
                                         UINT8  *pMask,
                                         UINT32 ulDataLength)
{
#if 0
	HI_S32 s32Ret = 0;
	HI_UNF_DMX_FILTER_ATTR_S stFiltAttr;

	EVM_DEBUG("### set filter enter, filter dep len = %d! ###\n", ulDataLength);

	ENREACH_RETURN((DMX_FILTER_MAX_DEPTH <= ulDataLength), OC_DEMUX_ERROR);

	memcpy(stFiltAttr.au8Match, pData, ulDataLength);
	memcpy(stFiltAttr.au8Mask, pMask, ulDataLength);
	memset(stFiltAttr.au8Negate, 0x0, DMX_FILTER_MAX_DEPTH);
	stFiltAttr.u32FilterDepth = ulDataLength;

	s32Ret = HI_DMXS_SetFilterAttr(ulFilterId, &stFiltAttr);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	s32Ret = HI_DMXS_AttatchFilter(ulFilterId, ulChannelId);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	EVM_DEBUG("### set filter leave, chanel id = %d, flt id = %d ! ### \n", ulChannelId, ulFilterId);

	return OC_DEMUX_OK;
#else
	HI_S32 				s32Ret = 0;
	HI_U8 				excl[EVM_FILTER_MAX_DEPTH] = {0};

	EVM_INFO("ChannelId:%d, FilterId:%d, data[0]:0x%x mask[0]:0x%x, data[1]:0x%x mask[1]:0x%x, data[2]:0x%x mask[2]:0x%x", 
		ulChannelId, ulFilterId, pData[0],pMask[0], pData[1],pMask[1], pData[2], pMask[2]);

	ENREACH_RETURN((EVM_FILTER_MAX_DEPTH <= ulDataLength), OC_DEMUX_ERROR);

	//memcpy(stFiltAttr.au8Match, pData, ulDataLength);
	//memcpy(stFiltAttr.au8Mask, pMask, ulDataLength);
	//memset(stFiltAttr.au8Negate, 0x0, DMX_FILTER_MAX_DEPTH);
	//stFiltAttr.u32FilterDepth = ulDataLength;

	//s32Ret = HI_DMXS_SetFilterAttr(ulFilterId, &stFiltAttr);

	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
	s32Ret = FYF_API_demux_set_filter(ulChannelId, ulFilterId, ulDataLength, pData, pMask, excl);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	s32Ret = FYF_API_demux_enable_filter(ulChannelId, ulFilterId);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	EVM_INFO("set OK, ChannelId:%d, FilterId:%d", ulChannelId, ulFilterId);

	return OC_DEMUX_OK;
	
#endif

}

/**
 * 设置pid
 * 
 * @param ulChannelId      : 创建的通道ID
 * @param usPid            : pid
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxSetChannelPid(UINT32 ulChannelId,
                                             UINT16 usPid)
{
#if 0
	HI_S32 s32Ret = 0;
	HI_U32 i = 0;

	s32Ret = HI_DMXS_SetChnPid(ulChannelId, usPid);
	ENREACH_ERROR("### set channel pid enter, chn id = %d, pid = %d, ret = 0x%x! ### \n", ulChannelId, usPid, s32Ret);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

	i = getChnNum(ulChannelId);
	ENREACH_RETURN((CHN_MAX_NUM <= i), OC_DEMUX_ERROR);

	s_astChnCb_ptr[i].u32Pid = usPid;

	EVM_DEBUG("### set channel pid leave! ### \n");

	return OC_DEMUX_OK;
#else
	HI_S32 s32Ret = 0;

	EVM_INFO("ChannelId:%d, Pid:%d", ulChannelId, usPid);
	s32Ret =FYF_API_demux_set_channel_pid(ulChannelId, usPid);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
	EVM_INFO("set OK");

	return OC_DEMUX_OK;

#endif

}

/**
 * 通道控制
 * 
 * @param ulChannelId      : 创建的通道ID
 * @param emChannelCmd     : 操作命令
 *
 * @return OC_DEMUX_STATUS
 */
OC_DEMUX_STATUS DsmccOcDrvDemuxControlChannel(UINT32 ulChannelId,
                                              OC_DEMUX_CMD emChannelCmd)
{
#if 0
    HI_S32 s32Ret = 0;
    HI_U32 i = 0;

    if (OC_DRV_DEMUX_ENABLE == emChannelCmd)
    {
        /* 接口内部自动绑定pid信息 */

        i = getChnNum(ulChannelId);
        ENREACH_RETURN((CHN_MAX_NUM <= i), OC_DEMUX_ERROR);
        
        s32Ret = HI_DMXS_SetChnPid(ulChannelId, s_astChnCb_ptr[i].u32Pid);
        ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);

        EVM_DEBUG("### open channel enter, channel id = %d ### \n", ulChannelId);

        s32Ret = HI_DMXS_StartChn(ulChannelId);
        EVM_DEBUG("### open channel leave, ret = 0x%x! ### \n", s32Ret);
        ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
    }
    else
    {
        EVM_DEBUG("### close channel enter! ### \n");
        s32Ret = HI_DMXS_StopChn(ulChannelId);
        EVM_DEBUG("### close channel leave! ### \n");

        ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
    }

    return OC_DEMUX_OK;
#else
    HI_S32 s32Ret = 0;
 
    if (OC_DRV_DEMUX_ENABLE == emChannelCmd)
    {
        EVM_INFO("Enable ChannelId:%d", ulChannelId);		
        s32Ret = FYF_API_demux_start_channel(ulChannelId);
        ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
    }
    else
    {
        EVM_INFO("Enable ChannelId:%d", ulChannelId);	
        s32Ret = FYF_API_demux_stop_channel(ulChannelId);
        ENREACH_RETURN((HI_SUCCESS != s32Ret), OC_DEMUX_ERROR);
    }

    return OC_DEMUX_OK;
	
#endif
}


#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

