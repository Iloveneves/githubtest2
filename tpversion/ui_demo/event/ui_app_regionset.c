#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

#ifdef ENABLE_BROADCAST_IPANEL
extern BS32 RegionID;
#endif
static BU32 g_sys_Region_SwitchItem=0;
static BU32 g_sys_RegionID=0;
void UI_APP_RegionSet_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	APP_EPG_API_StopGetEpg();
	hwnd = GUI_API_GetDlgItem(ID_VFM_RegionSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_RegionSet,ID_STA_Event2_RegionSet,ID_PBR_Epg_RegionSet,260,1);

	APP_SYS_API_GetSysInfo(APP_SYS_Region_SwitchItem_e,&g_sys_Region_SwitchItem);
	APP_SYS_API_GetSysInfo(APP_SYS_RegionID_e,&g_sys_RegionID);


	GUI_ENC_API_SetValue(ID_CBX_AutoRegion_RegionSet,WDT_CBX_SV_INDEX,(BU32)g_sys_Region_SwitchItem,0);
	if(g_sys_Region_SwitchItem)
	{
	#ifdef ENABLE_BROADCAST_IPANEL
		Ipanel_DLRegionId();
		GUI_ENC_API_SetValue(ID_TBX__RegionID_RegionSet, WDT_TBX_SV_DATA,(BU32) RegionID, 0);
	#endif
	}
	else
		GUI_ENC_API_SetValue(ID_TBX__RegionID_RegionSet, WDT_TBX_SV_DATA, g_sys_RegionID, 0);

	
}

void UI_APP_RegionSet_End(void)
{
	
}

void UI_APP_RegionSet_CBXKey(BU32 key)
{
	BU32 index;
	BU32 region_ID=0;
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:		
		if(GUI_ENC_API_IsFocus(ID_TBX__RegionID_RegionSet))
		{
		    GUI_ENC_API_GetValue(ID_TBX__RegionID_RegionSet,WDT_TBX_SV_DATA,&region_ID,0);
		    if(region_ID>65535)
			region_ID=65535;
		
		}
		break;
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if (GUI_ENC_API_IsFocus(ID_CBX_AutoRegion_RegionSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_AutoRegion_RegionSet,WDT_CBX_SV_INDEX,&index,0);
			if(index)
			{	
			    //自动获取
				GUI_ENC_API_Enable(ID_CBX_AutoRegion_RegionSet, 0);
				GUI_ENC_API_Enable(ID_TBX__RegionID_RegionSet, 0);
				#ifdef ENABLE_BROADCAST_IPANEL
				GUI_ENC_API_SetValue(ID_TBX__RegionID_RegionSet, WDT_TBX_SV_DATA, RegionID, 0);
				#endif
				GUI_ENC_API_Update(ID_CBX_AutoRegion_RegionSet,1);
				GUI_ENC_API_Update(ID_TBX__RegionID_RegionSet,1);
			}
			else
			{
			    //手动设置
				GUI_ENC_API_Enable(ID_CBX_AutoRegion_RegionSet, 1);
				GUI_ENC_API_Enable(ID_TBX__RegionID_RegionSet, 1);

				GUI_ENC_API_Update(ID_CBX_AutoRegion_RegionSet,1);
				GUI_ENC_API_Update(ID_TBX__RegionID_RegionSet,1);
			}
		}



		break;
	case GUI_VK_UP:
	case GUI_VK_DOWN:
		if (GUI_ENC_API_IsFocus(ID_CBX_AutoRegion_RegionSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_AutoRegion_RegionSet,WDT_CBX_SV_INDEX,&index,0);
			if(index)
			{	
			    //自动获取

			}
			else
			{
			    //手动设置
                            GUI_ENC_API_SetFocus(ID_TBX__RegionID_RegionSet);
				GUI_ENC_API_Update(ID_CBX_AutoRegion_RegionSet,1);
				GUI_ENC_API_Update(ID_TBX__RegionID_RegionSet,1);
			}
		}
		else if (GUI_ENC_API_IsFocus(ID_TBX__RegionID_RegionSet))
		{
                      GUI_ENC_API_SetFocus(ID_CBX_AutoRegion_RegionSet);
		}

		break;

	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_CBX_AutoRegion_RegionSet,WDT_CBX_SV_INDEX,&index,0);
		APP_SYS_API_SetSysInfo(APP_SYS_Region_SwitchItem_e,index);

		GUI_ENC_API_GetValue(ID_TBX__RegionID_RegionSet,WDT_TBX_SV_DATA,&region_ID,0);
		APP_SYS_API_SetSysInfo(APP_SYS_RegionID_e,region_ID);
		APP_SYS_API_SaveSysInfo();
		#ifdef ENABLE_BROADCAST_IPANEL
		RegionID = region_ID;
		#endif
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);

	}


	
}