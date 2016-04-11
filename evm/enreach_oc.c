/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_oc.c
* Description: 该文件实现oc库定义的oc接口
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
#include "hi_base.h"
#include "fyf_def.h"
#include "enreach_erro.h"
#include "enreach_pub.h"
#include "enreach_oc_api.h"
#include "enreach_oc_drv.h"
#include "ebrowser_api.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static DSMCC_OC_HANDLE s_hOc_hdl = INVALID_HANDLE;

HI_U8*    s_pu8Oc_addr    = NULL;

static HI_U8*    s_pWebIndex_Ptr = HI_NULL;           /* 网站入口首地址 */
static HI_BOOL  s_oc_open = HI_FALSE;

void evm_data_callback(BU32 ch, BU32 filter, BU08 *buf, BS32 len);

HI_S32 OC_EnreachGetWebUrl(HI_U8 **ppszWebUrl)
{
	HI_U8 	u8Ret;
	
	ENREACH_RETURN((NULL == ppszWebUrl), HI_FAILURE);

	u8Ret = DSMCC_FindWebEntry(&s_pWebIndex_Ptr);
	EVM_DEBUG("url = %s \n", s_pWebIndex_Ptr);
	if (0 == u8Ret)
	{	
		return HI_FAILURE;
	}
	
	EVM_DEBUG("get OC main page:%s", s_pWebIndex_Ptr);
    	*ppszWebUrl = s_pWebIndex_Ptr;

	return HI_SUCCESS;
}


void HI_DestroyOC(void)
{
	EVM_DEBUG("Enter");
	
	if (s_oc_open == HI_TRUE)
	{
		DSMCC_OC_Close(s_hOc_hdl);
	}
	else
	{
		EVM_DEBUG("s_oc_init is HI_FALSE");
	}

	s_hOc_hdl = INVALID_HANDLE;
	
	if (s_pWebIndex_Ptr != HI_NULL)
	{
		s_pWebIndex_Ptr = HI_NULL;
	}

	s_oc_open = HI_FALSE;

	EVM_DEBUG("Destory Oc OK");
}

HI_S32 HI_StartEnreachOC(HI_U32 u32Freq, HI_U32 u32Symbol, HI_U32 u32Qam, HI_U32 u32SerID)
{
	HI_U8  				u8Ret  = 0;
	DSMCC_OC_STATUES 	OcStatus = DSMCC_OC_OK;
	OC_SERVICE_INFO 	OcSrvInfo = {0};
	HI_U8  u8OcRet = 0;

	/* OC初始化 */
	ENREACH_RETURN((INVALID_HANDLE != s_hOc_hdl), HI_SUCCESS);

	if (u32Freq != 0)
	{
		OC_SERVICE_INFO tune_info;
		tune_info.uiFreq 		= u32Freq;
		tune_info.uiSymRat 	= u32Symbol;
		tune_info.ucQam 		= u32Qam;
		
		OcStatus = OC_SetTuner(tune_info);
		ENREACH_RETURN((OcStatus != HI_SUCCESS), HI_FAILURE);
		EVM_DEBUG("set TUNE ok");
	}
	

	ENREACH_RETURN((NULL == s_pu8Oc_addr), HI_FAILURE);
	memset(s_pu8Oc_addr, 0, OC_INIT_SIZE);


	OcStatus = DSMCC_OC_LowLevelInit(s_pu8Oc_addr, OC_INIT_SIZE);
	if (DSMCC_OC_OK != OcStatus)
	{	
		EVM_ERROR("### DSMCC_OC_LowLevelInit fail! ###\n");
		return HI_FAILURE;
	}

	EVM_DEBUG("DSMCC_OC_LowLevelInit ok");


	/* 注册线程退出方式，采用自杀方式 */
	Regist_Task_DestroySelf(HI_TaskDestroySelf);
	//Regist_Task_Destroy(HI_TaskDestroy);


	FYF_API_demux_set_notify(evm_data_callback);


	/* 设置机顶盒的OC Info */
	OcSrvInfo.bShareVod    = 0;
	OcSrvInfo.bUseFstParam = 0;
	OcSrvInfo.usSrvId      = u32SerID;

	/* to do 各个参数需要待定 */

	OcSrvInfo.uiFreq   		= u32Freq;   /* 618 */
	OcSrvInfo.uiSymRat 	= u32Symbol; /* 6875 */
	OcSrvInfo.ucQam    	= u32Qam;    /* 3 */

	u8OcRet = DsmccOcSetCurServiceID(OcSrvInfo);
	if (u8OcRet != DSMCC_OC_OK)
	{
		EVM_ERROR("DsmccOcSetCurServiceID fail!");
		return HI_FAILURE;
	}
	EVM_DEBUG("DsmccOcSetCurServiceID ok!");


	/* 打开一个OC */
	OcStatus = DSMCC_OC_Open(&s_hOc_hdl, OC_OPEN_WAITE_TIME); /* 4000代表4秒 */
	EVM_ERROR("DSMCC_OC_Open, OcStatus:0x%x, s_hOc_hdl:0x:%x", OcStatus, s_hOc_hdl);

	if (DSMCC_OC_OK != OcStatus)
	{
		EVM_ERROR("DSMCC_OC_Open fail, status = %d", OcStatus);
		//sleep(2);
		return HI_FAILURE;
	}

	s_oc_open = HI_TRUE;

	EVM_DEBUG("OC open OK!");	

	return HI_SUCCESS;
}

char Callback_StartOC(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int service_id)
{
	HI_S32 s32Ret = 0;

	s32Ret = HI_StartEnreachOC(freq, SymbolRate, qam, service_id);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OS_ERROR);

	HI_TRD_ENREACH_SetEnterFlag(0);
	
	/* TODO: 是否要考虑清屏处理 */

	return HI_SUCCESS;
}

char Callback_StopOC()
{
    DSMCC_OC_STATUES OcStatus = DSMCC_OC_OK;

#if 0
    ENREACH_RETURN((INVALID_HANDLE == s_hOc_hdl), HI_SUCCESS);

    OcStatus  = DSMCC_OC_Close(s_hOc_hdl);
    s_hOc_hdl = INVALID_HANDLE;

    /* 释放分配给OC的内存地址 */

    ENREACH_SAFE_FREE(s_pu8Oc_addr);
    EVM_ERROR("### free oc addr! ### \n");
#else
	HI_DestroyOC();
#endif

    return HI_SUCCESS;    
}

void HI_FreeOCRes(void)
{
	EVM_DEBUG("Free s_pu8Oc_addr:0x%x", s_pu8Oc_addr);
	//ENREACH_SAFE_FREE(s_pu8Oc_addr);
}


#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

