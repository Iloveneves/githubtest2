/**********************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
***********************************************************************
* FileName: Trd_enreach_init.c
* Description: 该文件实现进入、退出接口，对ui提供的接口
*
* History:
* Version   Date              Author       DefectNum      Description
**********************************************************************/

#include "hi_type.h"
#include "hi_base.h"

#include "enreach_erro.h"
#include "enreach_pub.h"
#include "ebrowser_key.h"
#include "ebrowser_api.h"
#include "enreach_oc_drv.h"
#include "enreach_oc.h"
#include "enreach_open.h"
#include "enreach_osd.h"

#include "gui_common.h"

#include "../tpversion/ui_com_api.h"

#include "trd_enreach_init.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"
#include "../tpmiddle/include/app_sys.h"

//#include "../tpmiddle/include/app_audio.h"

extern BU08 bfirst;

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct 
{
	HI_U8 	browser_buf[BROWSER_BUFF_SIZE];
	HI_U8 	brower_cache[BROWSER_CACHE_SIZE];
	HI_U8 	brower_js_cache[JS_BUFF_SIZE];
	
	HI_U8 	oc_buf[OC_INIT_SIZE];

	HI_U8 	evm_osd_buf[ENREACH_OSD_WIDTH*ENREACH_OSD_HEIGHT*2];
	HI_U8 	evm_osd_buf_tmp[ENREACH_OSD_WIDTH*ENREACH_OSD_HEIGHT*2];
	
} evm_mem_set;


//HI_BOOL 			g_bEnreachInited = HI_FALSE;
static HI_S32 			g_bDisplayVOI = -1;
static HI_U32 			g_exit = 0;
static evm_mem_set* 	all_mem = NULL;

/*------------------------------------------------------------------------------
0: 单向
1: 双向
2: VOD点播
------------------------------------------------------------------------------*/
static HI_U8 	g_evm_type_flag 	= 0;
static HI_S8 	*main_page 			= "http://172.16.3.230";
static HI_S8 	*vod_main_page 	= "http://172.16.3.230/11lxtx/lxtx05.htm";


static HI_S32 TRD_ENREACH_GetServiceInfo(HI_VOID)
{
    return HI_SUCCESS;
}

/**********************************************************************
 * Function:          HI_TRD_ENREACH_OSD_Init
 * Description:     enreach init
 * Data Accessed:
 * Data Updated:
 * Input:            HI_HANDLE hSurface  为stb传入的surface
                        trdtype 为初始化哪个第三方(ipanel,htld,enreach等)
 * Output:
 * Return:
 * Others:
***********************************************************************/
static HI_S32 HI_TRD_ENREACH_OSD_Init(void)
{
	EVM_INFO("Enter");
	return HI_SUCCESS;
}

/**********************************************************************
 * Function:        HI_TRD_ENREACH_PreInit
 * Description:     enreach pre init
 * Data Accessed:
 * Data Updated:
 * Input:
 * Output:
 * Return:
 * Others:
**********************************************************************/
static HI_BOOL HI_TRD_ENREACH_PreInit(HI_VOID)
{

	/* open key */
	EVM_DEBUG("EVM open key");
	bfirst = 0;
	
	/* static */
	EVM_DEBUG("set exit lable");
	g_exit = 0;

	EVM_DEBUG("allocation the evm use mem space");
	if (all_mem == NULL)
	{
		all_mem = (HI_U8*)malloc(sizeof(evm_mem_set));
		if (all_mem == NULL)
		{
			EVM_ERROR("create all mem error");
			return HI_FALSE;
		}
	}
	else
	{
		EVM_DEBUG("EVM all_mem has already allocation");
	}

	s_pu8Browser_buf 	= all_mem->browser_buf;
	s_pu8Browser_cach 	= all_mem->brower_cache;
	s_pu8JS_buf 		= all_mem->brower_js_cache;

	s_pu8Oc_addr 	= all_mem->oc_buf;

	evm_osd_buf 	= all_mem->evm_osd_buf;
	osd_tmp_buf	= all_mem->evm_osd_buf_tmp;

	EVM_DEBUG("EVM unlock display");
	OS_UnlockOSD();
	
	return HI_TRUE;

}


HI_VOID HI_TRD_ENREACH_SetEnterFlag(HI_U8 type)
{
	g_evm_type_flag = type;
}

HI_U8 HI_TRD_ENREACH_GetEnterFlag(HI_VOID)
{
	return g_evm_type_flag;
}


static HI_VOID HI_TRD_ENREACH_Exit(void)
{
	EVM_DEBUG("Enter");

	HI_DestroyOC();	
	HI_DestroyBrowser();

	HI_FreeBrowserRes();
	HI_FreeOCRes();	
}


HI_VOID HI_TRD_ENREACH_Post_Exit(void *para)
{
	EVM_DEBUG("Enter");

	g_exit = 0;
}


static HI_VOID HI_TRD_ENREACH_DeInit(void)
{
	HI_S32  prevstat;
	HI_S32 	i ;
	HI_BOOL browser_status;

	
	EVM_DEBUG("Enter");

	HI_CleanCallback();


	browser_status = HI_IsBrowserOpen();
	HI_TRD_ENREACH_Exit();


	EVM_DEBUG("Wait for evm lib to finish the exit");

	if (browser_status)
	{
		/* open browser wait */
		while (g_exit)
		{
			sleep(1);
		}
	}

	ENREACH_SAFE_FREE(all_mem);
	HI_FreeOsRes();
	
	EVM_DEBUG(
		"\n-------------------------------------------------------------------------------\n"
		"\t\t HI_TRD_ENREACH deinit  ok \n"
		"-------------------------------------------------------------------------------\n"
		);	
	

	/* status convert */
	prevstat = UI_APP_Null_GetPrevStat();
	if (prevstat != ID_DLG_MainMenu && prevstat != ID_DLG_ChnInfo && prevstat != ID_DLG_BroadcastList)
	{
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	}
	else
	{
		GUI_ENC_API_SwitchState(prevstat, 1);
	}

	/* release the key loop to ui */
	bfirst = 1; 
		
	EVM_DEBUG(
		"\n-------------------------------------------------------------------------------\n"
		"\t\t GUI API Switch  ok \n"
		"-------------------------------------------------------------------------------\n"
		);	
	
    	return;
}

#if 0
static void in_dead(void)
{
	while(1){};
}
#endif

HI_S32  HI_TRD_ENREACH_Init(HI_S32 freq, HI_S32 sym, HI_S32 qam, HI_S32 servid)
{
	HI_S32 		s32Ret = 0;
	HI_CHAR 		szRet = OS_OK;
	HI_U8 		*pu8WebIndex = NULL;

	

#if 0 //for test msg queue
	OC_OS_STATUS ret = 0;
	char temp[] = "hello word";
	char revtmp[20];
	UINT32 hmsg = 0;
	ret = OcOsCreateQueue(5, 20, &hmsg);
	if (ret != 0)
	{
		EVM_ERROR("create queue error");
		in_dead();
	}
	EVM_DEBUG("Create queue OK");

	OcOsDeleteQueue(hmsg);

	while (1)
	{
		//ret = OcOsQueueSend(hmsg, temp, sizeof(temp));
		//if (ret != 0)
		//{
		//	EVM_ERROR("send queue error");
		//	in_dead();
		//}
		//sleep(2);
		//ret = OcOsQueueReceiveTimeout(hmsg, revtmp, 20, OC_INFINITE_WAIT);
		ret = OcOsQueueReceiveTimeout(hmsg, revtmp, 20, 3000);
		if (ret != 0)
		{
			EVM_ERROR("recv queue error");
			in_dead();
		}
		printf("queue msg info :%s\n", revtmp);
	}
#endif

#if 0 //for test sem 
	UINT32  hsem;
	OC_OS_STATUS ret = 0;

	ret = DsmccOcDrvOsCreateSemaphore(0, &hsem);
	if (ret != 0)
	{
		EVM_ERROR("create sem error");
		in_dead();
	}
	EVM_DEBUG("Create sem OK");

	ret = DsmccOcDrvOsWaitSemaphore(hsem, 2000);
	EVM_DEBUG("Wait sem OK");
	in_dead();

#endif

#if 0 //for demux test
#endif


	EVM_DEBUG("Enter, freq = %d, symbol = %d, qam = %d, serid = %d ", 
    		freq, sym, qam, servid);

	HI_TRD_ENREACH_PreInit();

/*------------------------------------------------------------------------------
	browser init
------------------------------------------------------------------------------*/
	s32Ret = HI_StartBrowser(freq, sym, qam, servid);
	if (HI_SUCCESS != s32Ret)
	{
		EVM_ERROR("HI_StartBrowser failed");
		g_exit = 1;
		
		return HI_FAILURE;
	}
	else
	{
		EVM_DEBUG(
		"\n-------------------------------------------------------------------------------\n"
		"\t\t\t HI_StartBrowser ok \n"
		"-------------------------------------------------------------------------------\n");
	}

/*------------------------------------------------------------------------------
	OC start
------------------------------------------------------------------------------*/
	if (g_evm_type_flag == 0)
	{
		s32Ret = HI_StartEnreachOC(freq, sym, qam, servid);
		if (HI_SUCCESS != s32Ret)
		{
			EVM_ERROR("HI_StartEnreachOC failed %x ", s32Ret);
			OSD_LockOSD(); /*Lock the osd undisplay the NULL page*/
			HI_OpenBrowser(0); /*for the browser resource reclaim */
			
			g_exit = 1;
			
			return HI_FAILURE;
		}
		else
		{		
			EVM_DEBUG(
			"\n-------------------------------------------------------------------------------\n"
			"\t\t HI_StartEnreachOC ok \n"
			"-------------------------------------------------------------------------------\n");
		}
	}

	
/*------------------------------------------------------------------------------
	start page get 
------------------------------------------------------------------------------*/
	if (g_evm_type_flag == 0)
	{
		s32Ret = OC_EnreachGetWebUrl(&pu8WebIndex);
		if (s32Ret != HI_SUCCESS)
		{
			EVM_ERROR("Get OC start page error");
			OSD_LockOSD();

			HI_OpenBrowser(0);
			g_exit = 1;
			
			return HI_FAILURE;
		}
	}
	else if (g_evm_type_flag == 1)
	{
		pu8WebIndex = main_page;
	}
	else 
	{
		pu8WebIndex = vod_main_page;
	}

	EVM_DEBUG(
"\n-------------------------------------------------------------------------------\n"
"\t\t\t Get web URL ok \n"
"-------------------------------------------------------------------------------\n");

	 //pu8WebIndex = "http://172.16.3.39";

	
/*------------------------------------------------------------------------------
	browser open
------------------------------------------------------------------------------*/
	s32Ret = HI_OpenBrowser(pu8WebIndex);
	if (HI_SUCCESS != s32Ret)
	{
		EVM_ERROR("## HI_StartBrowser failed %x \n", s32Ret);
		g_exit = 1;
		return HI_FAILURE;
	}
	else
	{
		EVM_DEBUG(
			"\n-------------------------------------------------------------------------------\n"
			"\t\t HI_OpenBrowser ok \n"
			"-------------------------------------------------------------------------------\n");	
	}
	
    return HI_SUCCESS;
}


static HI_VOID TRD_ENREACH_SET_MUTE_ICON(HI_VOID)
{
	if (APP_Audio_API_GetMuteState())
	{
		EVM_DEBUG("mute status");
		GUI_ENC_API_SetVisible(ID_STA_MUTE_EVM, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_MUTE_EVM,  1);
	
	}
	else
	{
		EVM_DEBUG("unmute");
		GUI_ENC_API_SetVisible(ID_STA_MUTE_EVM, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_MUTE_EVM,  1);
		Callback_updateScreen(NULL, NULL, 573, 63, 51, 34, 573, 63);	
	}		
}

/*******************************************************************************
* Function:     HI_TRD_INPUT_GetKey
* Description:  HI_TRD_INPUT_GetKey
* Input:        eTrdType 第三方定义
                    s32Key 输入的原始按键值
* Output:
* Return:       HI_FAILURE:失败，HI_SUCCESS:成功
********************************************************************************/
static HI_S32 TRD_ENREACH_GetKey(HI_S32 s32Key)
{
	HI_S32 evm_key = 0;;

	switch (s32Key)
	{
		case GUI_VK_0:			return KEY_0;
		case GUI_VK_1:			return KEY_1;
		case GUI_VK_2:			return KEY_2;
		case GUI_VK_3:			return KEY_3;
		case GUI_VK_4:			return KEY_4;
		case GUI_VK_5:			return KEY_5;
		case GUI_VK_6:			return KEY_6;
		case GUI_VK_7:			return KEY_7;
		case GUI_VK_8:			return KEY_8;
		case GUI_VK_9:	             return KEY_9;
		case GUI_VK_SELECT:		return KEY_ENTER;
		case GUI_VK_LEFT:		return KEY_LEFT;
		case GUI_VK_UP:			return KEY_UP;
		case GUI_VK_RIGHT:		return KEY_RIGHT;
		case GUI_VK_DOWN:		return KEY_DOWN;
		case GUI_VK_P_UP:		return KEY_PGUP;
		case GUI_VK_P_DOWN:		return KEY_PGDN;
		case GUI_VK_MENU:	      return KEY_MENU;
		case GUI_VK_EXIT:		return KEY_EXIT;
		case GUI_VK_RECALL:		return KEY_F2;
		
		default: 	return HI_FAILURE;		
	}

}

HI_VOID HI_TRD_ENREACH_Input(HI_U32 u32Key)
{
	HI_S32 key = 0;
	HI_S32 VoiceIndex; 


	EVM_DEBUG("Enter, key:0x%x", u32Key);

	if (g_exit == 1)
	{
		EVM_DEBUG("退出过程中，不再响应按键，以免按键堵塞而挂死");
		return ;
	}


/*------------------------------------------------------------------------------
	vol control
------------------------------------------------------------------------------*/
	if (u32Key == GUI_VK_V_UP)
	{
		UI_APP_API_VoiceControl(GUI_VK_V_UP);
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, (BU32 *)&VoiceIndex);	
		GUI_ENC_API_SetVisible(ID_VOI_EVM, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_VOI_EVM,WDT_VOI_SV_RIGHT,VoiceIndex,0);
		GUI_ENC_API_Update(ID_VOI_EVM, 1);

		TRD_ENREACH_SET_MUTE_ICON();

		g_bDisplayVOI = 1;
		
		return;

	}
	else if (u32Key == GUI_VK_V_DOWN)
	{
		UI_APP_API_VoiceControl(GUI_VK_V_DOWN);
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e,(BU32 *)&VoiceIndex);
		GUI_ENC_API_SetVisible(ID_VOI_EVM, WS_VISIBLE);
		
		GUI_ENC_API_SetValue(ID_VOI_EVM,WDT_VOI_SV_LEFT,VoiceIndex, 0);
		GUI_ENC_API_Update(ID_VOI_EVM, 1);

		TRD_ENREACH_SET_MUTE_ICON();

		
		g_bDisplayVOI = 1;

		return ;
	} 

	if (g_bDisplayVOI > 0)
	{
		EVM_DEBUG(" Hidden VOI ");
		GUI_ENC_API_SetVisible(ID_VOI_EVM, WS_DISABLE);
		Callback_updateScreen(NULL, NULL, 138, 388, 450, 44, 138, 388);
		g_bDisplayVOI = -1;
	}
	
	if (u32Key == GUI_VK_MUTE)
	{
		EVM_ERROR("Press the 'mute' key");

		UI_APP_API_VoiceControl(GUI_VK_MUTE);

		TRD_ENREACH_SET_MUTE_ICON();

		return;
	}

	key = TRD_ENREACH_GetKey(u32Key);
	EVM_DEBUG("key:0x%x, convert key :0x%x", u32Key, key);

	if (KEY_EXIT == key)
	{
		EVM_DEBUG("Get the EXIT key !");
		g_exit = 1;
		return;
	}
	
/*------------------------------------------------------------------------------
	post key Event
------------------------------------------------------------------------------*/
	(HI_VOID)HI_BrowserInput(key);

	return;
}

HI_VOID HI_TRD_ENREACH_TimeWork(HI_VOID)
{
	EVM_DEBUG("Enter");

	if (g_exit == 1)
	{
		EVM_DEBUG(
		"\n-------------------------------------------------------------------------------\n"
		"\t\t HI_TRD_ENREACH_TimeWork get in Exit process\n"
		"-------------------------------------------------------------------------------\n");	

		HI_TRD_ENREACH_DeInit();
		GUI_ENC_API_SetValue(ID_TIM_NULL, WDT_TIM_SV_STOP, 0, 0);

		return;
	}

	if (g_bDisplayVOI > 0)
	{
		if (g_bDisplayVOI > 5)
		{
			/* Hidden VOI */
			EVM_DEBUG(" Hidden VOI ");
			GUI_ENC_API_SetVisible(ID_VOI_EVM, WS_DISABLE);
			Callback_updateScreen(NULL, NULL, 138, 388, 450, 44, 138, 388);
			g_bDisplayVOI = -1;

		}
		else
		{
			g_bDisplayVOI++;
		}
	}
}

HI_U32 HI_TRD_ENREACH_IsExit(HI_VOID)
{
	return g_exit;
}

HI_VOID HI_TRD_ENREACH_SetExit(HI_VOID)
{
	 g_exit  = 1;
}


#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

