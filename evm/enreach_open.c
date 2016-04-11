/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_open.c
* Description: 该文件实现enreach浏览器定义的打开，退出接口，实现基本进出流程
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
#include "hi_base.h"
#include "enreach_erro.h"
#include "enreach_pub.h"
#include "ebrowser_key.h"
#include "ebrowser_api.h"
#include "enreach_oc_drv.h"
#include "enreach_osd.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_U8* 	s_pu8Browser_buf  	= NULL;
HI_U8* 	s_pu8Browser_cach 	= NULL;
HI_U8* 	s_pu8JS_buf 		= NULL;


//static eBrowserHandler s_pBrowser_hdl = NULL;  /* 用来记录浏览器句柄 */
static HI_U32  	s_pBrowser_hdl 		= 0;

static HI_BOOL 	s_bBrowser_open 	= HI_FALSE;
extern HI_S32 	g_Enreach_hWnd;


HI_VOID HI_TRD_ENREACH_Post_Exit(void *para);

HI_VOID CallBack_ExitEBrowser(HI_S8* exitFlag)
{
	HI_S32 s32Ret = 0;

	EVM_DEBUG("call CallBack_ExitEBrowser fun");
	HI_TRD_ENREACH_SetExit();

	return;
}

HI_S32 HI_StartBrowser(HI_U32 u32Freq, HI_U32 u32SymbolRate,
                       HI_U32 u32Qam, HI_U32 u32Service_id)
{
	HI_S32 		s32Ret = 0;
	HI_U8  		u8Ret  = 0;
	HI_U32 		u32VideoBuffer = HI_NULL, u32Width = 0, u32Height = 0;

/*------------------------------------------------------------------------------
	ebrowser memory alloc
------------------------------------------------------------------------------*/
#if 0
	s_pu8Browser_buf = malloc(BROWSER_BUFF_SIZE);
	if (NULL == s_pu8Browser_buf)
	{
		EVM_ERROR("malloc s_pu8Browser_buf error");
		return HI_FAILURE;
	}
	memset(s_pu8Browser_buf, 0 , BROWSER_BUFF_SIZE);

	s_pu8Browser_cach = malloc(BROWSER_CACHE_SIZE);
	if (NULL == s_pu8Browser_cach)
	{
		EVM_ERROR(" malloc enreach buffer fail!");
		return HI_FAILURE;
	}
	memset(s_pu8Browser_cach, 0, BROWSER_CACHE_SIZE);
	
	s_pu8JS_buf  = malloc(JS_BUFF_SIZE);
	if (s_pu8JS_buf == NULL)
	{
		EVM_ERROR("malloc enreach JS buffer fail!");
		return HI_FAILURE;
	}
	memset(s_pu8JS_buf, 0, JS_BUFF_SIZE);
#else
	if (NULL == s_pu8Browser_buf)
	{
		EVM_ERROR("malloc s_pu8Browser_buf error");
		return HI_FAILURE;
	}
	memset(s_pu8Browser_buf, 0 , BROWSER_BUFF_SIZE);

	if (NULL == s_pu8Browser_cach)
	{
		EVM_ERROR(" malloc enreach buffer fail!");
		return HI_FAILURE;
	}
	memset(s_pu8Browser_cach, 0, BROWSER_CACHE_SIZE);

	if (s_pu8JS_buf == NULL)
	{
		EVM_ERROR("malloc enreach JS buffer fail!");
		return HI_FAILURE;
	}
	memset(s_pu8JS_buf, 0, JS_BUFF_SIZE);	
#endif

	/*--------------------------------------------------------------------------
		ebrowser init 
	--------------------------------------------------------------------------*/
	s32Ret = eBrowser_setup((eBrowserHandler*)&s_pBrowser_hdl, NULL);
	if (HI_SUCCESS != s32Ret)
	{
		EVM_ERROR("eBrowser_setup fail");
		return HI_FAILURE;
	}
		
	EVM_DEBUG("eBrowser_setup ok!")

	/* set destory type */
	regist_callback_task_destroyself(HI_TaskDestroySelf);
	//regist_callback_task_destroy(HI_TaskDestroy);
	
	EVM_DEBUG("Set destory call back ok");

	u8Ret = eBrowser_setCacheMemory((eBrowserHandler*)&s_pBrowser_hdl,
	        					(HI_VOID*)s_pu8Browser_cach,
	        					BROWSER_CACHE_SIZE);
	if (HI_SUCCESS != u8Ret)
	{
		EVM_ERROR("eBrowser_setCacheMemory fail");
		return HI_FAILURE;
	}
	EVM_DEBUG("eBrowser_setCacheMemory ok!")

	s32Ret = eBrowser_setMemory((eBrowserHandler*)&s_pBrowser_hdl, s_pu8Browser_buf);
	if (s32Ret != HI_SUCCESS)
	{
		EVM_ERROR("eBrowser_setMemory fail!");
		return HI_FAILURE;
	}
	EVM_DEBUG("eBrowser_setMemory ok!")

#if 1 // set the js buf cache
	EnRich_GetMemory(s_pu8JS_buf, JS_BUFF_SIZE);//设置 js 部分内存
#endif

	EVM_DEBUG("!set buffer and cache ok!!");


	/* 需要获取机顶盒显示区域的显存地址pVideoBuffer */

	s32Ret = OSD_EnreachGetBufAddr(&u32VideoBuffer);
	s32Ret |= OSD_EnreachGetBufRect(&u32Width, &u32Height);

	if (HI_SUCCESS != s32Ret)
	{
		EVM_ERROR("OSD_EnreachGetBufRect or OSD_EnreachGetBufAddr fail");
		return HI_FAILURE;
	}

	EVM_DEBUG("get OSD memory OK")

	s32Ret = eBrowser_setVideoInfo((eBrowserHandler*)&s_pBrowser_hdl, (HI_VOID*)u32VideoBuffer,
		u32Width, u32Height);
	

	EVM_DEBUG("eBrowser_setVideoInfo, h = %d, w = %d, addr = 0x%x , ret:%u", 
		u32Height, u32Width, u32VideoBuffer,s32Ret);

	/* 向浏览器注册相关函数 */

	eBrowser_setCustomizedStartScreen(OSD_DrawLogo);

	regist_callback_updateScreen(Callback_updateScreen);
	regist_callback_cleanWindow(Callback_cleanWindow);

	regist_callback_PlayTV(Callback_PlayTV);
	regist_callback_PlayTV_Srv(Callback_PlayTV_Srv);
	regist_callback_PlayTV_Pmt(Callback_PlayTV_Pmt);

	regist_callback_setSmallWindow(Callback_setSmallWindow);

	regist_callback_setTuner(Callback_setTuner);
	regist_callback_StopTV(Callback_StopTV);
	regist_callback_TvFullScreen(CallBack_TvFullScreen);

	regist_callback_Customized_exit_eb(CallBack_ExitEBrowser);

	regist_callback_post_exit_browser(HI_TRD_ENREACH_Post_Exit);

	EVM_DEBUG("regist ebrowser callback ok");

	return HI_SUCCESS;
	
}

HI_S32 HI_OpenBrowser(HI_U8 *pu8WebUrl)
{
	HI_S32 s32Ret = 0;
	
	EVM_DEBUG("Enter, open url:%s", pu8WebUrl);	

	//ENREACH_RETURN((pu8WebUrl == NULL), HI_FAILURE);
	
	s32Ret = eBrowser_open((eBrowserHandler*)&s_pBrowser_hdl, pu8WebUrl);
	EVM_DEBUG("eBrowser_open, handle:%x s32Ret:0x%x",
		(eBrowserHandler*)&s_pBrowser_hdl , s32Ret);

	s_bBrowser_open = HI_TRUE;
	
	return HI_SUCCESS;
}

HI_S32 HI_BrowserInput(HI_U32 u32Key)
{   
    HI_U16 u16Code  = 0;
    HI_U8  u8Status = 0;

#if 0
    if (KEY_VOL_UP == u32Key)
    {
        s32Ret = AV_GetVol(&s32Vol);
        ENREACH_RETURN((HI_SUCCESS != s32Ret), HI_FAILURE);

        s32Vol = ((s32Vol + AV_VOL_STEP_SIZE) > 100) ? 100 : (s32Vol + AV_VOL_STEP_SIZE);
        EVM_ERROR("### enreach set vol = %d ### \n", s32Vol);
        (HI_VOID)AV_SetVol(s32Vol);
    }
    else if (KEY_VOL_DOWN == u32Key)
    {
        s32Ret = AV_GetVol(&s32Vol);
        ENREACH_RETURN((HI_SUCCESS != s32Ret), HI_FAILURE);
        
        s32Vol = ((s32Vol - AV_VOL_STEP_SIZE) < 0) ? 0 : (s32Vol - AV_VOL_STEP_SIZE);
        EVM_ERROR("### enreach set vol = %d ### \n", s32Vol);
        (HI_VOID)AV_SetVol(s32Vol);
    }
#endif

	eBrowser_keyToScancode(&u8Status, (HI_U8*)&u16Code, u32Key);
	EVM_DEBUG("eBrowser_keyToScancode, status:0x%x, code:0x%x", u8Status, u16Code);

	eBrowser_event(u8Status, u16Code);

	return HI_SUCCESS;
}

HI_S32 HI_DestroyBrowser(void)
{
	HI_S32 s32Ret = 0;

	EVM_DEBUG("call HI_DestroyBrowser fun enter");

	if (HI_TRUE == s_bBrowser_open)
	{
		EVM_DEBUG("call eBrowser_close fun");
		s32Ret = eBrowser_close((eBrowserHandler*)&s_pBrowser_hdl);
		EVM_DEBUG("eBrowser_close, ret:0x%x", s32Ret);
		ENREACH_RETURN((s32Ret != 0), HI_FALSE);

		/* 如果在播放状态,停止播放 */
		//Callback_StopTV(NULL, 0);
	}
	else 
	{
		EVM_DEBUG("s_bBrowser_open is HI_FALSE");
	}
	
	s_bBrowser_open = HI_FALSE;
	
	EVM_ERROR("Destory ebrowser ok");

    	return HI_SUCCESS;
}




HI_BOOL HI_IsBrowserOpen(HI_VOID)
{
	return s_bBrowser_open;
}


void HI_FreeBrowserRes(void)
{
	/* 释放浏览器的内存 */
#if 0
	EVM_DEBUG("Free s_pu8Browser_buf:0x%x", s_pu8Browser_buf);
	ENREACH_SAFE_FREE(s_pu8Browser_buf);

	EVM_DEBUG("Free s_pu8Browser_cach:0x%x", s_pu8Browser_cach);
	ENREACH_SAFE_FREE(s_pu8Browser_cach);

	EVM_DEBUG("Free s_pu8JS_buf:0x%x", s_pu8JS_buf);
	ENREACH_SAFE_FREE(s_pu8JS_buf);
#endif

	/* free OSD display buffer*/
	OSD_EnreachFreeBufAddr();
}

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif


