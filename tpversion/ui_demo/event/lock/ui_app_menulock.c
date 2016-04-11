#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define LOCK_NULL      	0
#define LOCK_ERROR  	1
#define LOCK_SUCCESS  	2
#define LOCK_REFUSE     3

#define MENULOCK_LENGTH 6
static BU08 g_menulock_flag;
static BU08 g_menulock_enterCnt; //输入密码次数
static BU32 g_menulock_count;    //输入密码个数
static BU08 menulock_pw[7];

//菜单加锁
void UI_APP_MenuLock_Enter(void)
{	
	g_menulock_flag = LOCK_NULL;
	g_menulock_enterCnt = 0;
	g_menulock_count	= 0;
	memset(menulock_pw, 0, sizeof(menulock_pw));

}
void UI_APP_MenuLock_End(void)
{

}
void UI_APP_MenuLock_Key(BU32 key)
{

}
void UI_APP_MenuLock_Timer(void)
{

	BU32 curID;
	BU32 preDlgID;
	BU32 avType = APP_ChnDatabase_API_GetCurChnType();
	if(g_menulock_flag == LOCK_ERROR)
	{
		g_menulock_flag = LOCK_NULL;
		GUI_ENC_API_Update(ID_ODC_MenuLock,1);
	}
	else if(g_menulock_flag == LOCK_REFUSE)
	{
		if(GUI_ENC_API_GetPrevState() == ID_DLG_SysSet)
		{
			GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
		}
		else if(GUI_ENC_API_GetPrevState() == ID_DLG_ProgManager)
		{
			GUI_ENC_API_SwitchState(ID_DLG_ProgManager,1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_Ca,1);
		}
	}
	else if(g_menulock_flag == LOCK_SUCCESS)
	{
		g_menulock_flag = LOCK_NULL;
       preDlgID = GUI_ENC_API_GetPrevState();
	   if(preDlgID==ID_DLG_SysSet)
	   {
	   		GUI_ENC_API_GetValue(ID_MNU_SysSet,WDT_MNU_SV_CURDATAID,&curID,0);
			switch(curID)
			{
				case RS_FactoryDefault:
					//UI_APP_SetDefaultState(ID_DLG_SysSet);
					UI_APP_FactoryDefault_SetPrevState(ID_DLG_SysSet);
					GUI_ENC_API_SwitchState(ID_DLG_Default,1);
					break;
				case RS_NetSetting:
					GUI_ENC_API_SwitchState(ID_DLG_NetSet,1);
					break;
				case RS_ParentControl:
					GUI_ENC_API_SwitchState(ID_DLG_ParentControl,1);
					break;
				case RS_TVEdit:
					if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
					APP_ChnDatabase_API_GetCurTypeChnNum())
					{
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
						UI_APP_API_SetChnEditPreState(0);
						GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit,WDT_STA_SV_DATAID, RS_TVEdit, 0);
						GUI_ENC_API_SwitchState(ID_DLG_ChnEdit,1);
					}
					else
					{
						APP_Com_API_StopProg(1);
						if(avType == APP_CHN_AV_TYPE_RD_e)
						{
							if(!UI_APP_API_bShowNoSignal())
							{
								APP_Com_API_ShowBmp(0);//黑屏
							}
						}
						APP_Com_API_ShowLED("C000");
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
						APP_Com_API_SetEcmNULL();
						GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
						GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
					}
					break;
				case RS_RadioEdit:
					if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
					APP_ChnDatabase_API_GetCurTypeChnNum())
					{
						UI_APP_API_SetChnEditPreState(0);
						GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit,WDT_STA_SV_DATAID, RS_RadioEdit, 0);
						GUI_ENC_API_SwitchState(ID_DLG_ChnEdit,1);
					}
					else
					{
						APP_Com_API_StopProg(1);
						if(avType == APP_CHN_AV_TYPE_TV_e)
						{
							if(!UI_APP_API_bShowNoSignal())
							{
								APP_Com_API_ShowBmp(1);//广播LOGO
							}
						}
						APP_Com_API_ShowLED("R000");
						APP_Com_API_SetEcmNULL();
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
						GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
						GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
					}
					break;
				case RS_LockChn:
					if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
					APP_ChnDatabase_API_GetCurTypeChnNum())
					{
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
						UI_APP_API_SetChnEditPreState(1);
						GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit,WDT_STA_SV_DATAID, RS_TVEdit, 0);
						GUI_ENC_API_SwitchState(ID_DLG_ChnEdit,1);
					}
					else
					{
						APP_Com_API_StopProg(1);
						if(avType == APP_CHN_AV_TYPE_RD_e)
						{
							if(!UI_APP_API_bShowNoSignal())
							{
								APP_Com_API_ShowBmp(0);//黑屏
							}
						}
						APP_Com_API_ShowLED("C000");
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
						APP_Com_API_SetEcmNULL();
						GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
						GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
					}
					break;
				case RS_LockRadio:
					if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
					APP_ChnDatabase_API_GetCurTypeChnNum())
					{
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
						UI_APP_API_SetChnEditPreState(1);
						GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit,WDT_STA_SV_DATAID, RS_RadioEdit, 0);
						GUI_ENC_API_SwitchState(ID_DLG_ChnEdit,1);
					}
					else
					{
						APP_Com_API_StopProg(1);
						if(avType == APP_CHN_AV_TYPE_TV_e)
						{
							if(!UI_APP_API_bShowNoSignal())
							{
								APP_Com_API_ShowBmp(1);//广播LOGO
							}
						}
						APP_Com_API_ShowLED("R000");
						APP_Com_API_SetEcmNULL();
						GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
						GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
						GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
					}
					break;
				case RS_PasswordSetting:
					GUI_ENC_API_SwitchState(ID_DLG_PasswordSet,1);
					break;
				case RS_AutoSearch:
				case RS_AllSearch:
					GUI_ENC_API_SwitchState(ID_DLG_Search,1);
					break;		
				case RS_ManualSearch:
					GUI_ENC_API_SwitchState(ID_DLG_ManualSearchSet,1);
					break;
				case RS_MainFreqSet:
					GUI_ENC_API_SwitchState(ID_DLG_MainFreqSet,1);
					break;
				case RS_SoftWareUpdate:
					GUI_ENC_API_SwitchState(ID_DLG_SysUpdate, 1);
					break;
				default:
					GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
					break;
			}
	   }
	   else if(preDlgID==ID_DLG_ProgManager)
	   {
		   	GUI_ENC_API_GetValue(ID_MNU_ProgManager,WDT_MNU_SV_CURDATAID,&curID,0);
			switch(curID)
			{
				case RS_AutoSearch:
				case RS_AllSearch:
					GUI_ENC_API_SwitchState(ID_DLG_Search,1);
					break;		
				case RS_ManualSearch:
					GUI_ENC_API_SwitchState(ID_DLG_ManualSearchSet,1);
					break;
				case RS_MainFreqSet:
					GUI_ENC_API_SwitchState(ID_DLG_MainFreqSet,1);
					break;
				case RS_DelChn:
				case RS_ChangeChn:
				case RS_LockChn:
					APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
					GUI_ENC_API_SwitchState(ID_DLG_ChannelEdit,1);
					break;
				case RS_DelRadio:
				case RS_ChangeRadio:
				case RS_LockRadio:
					APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
					GUI_ENC_API_SwitchState(ID_DLG_ChannelEdit,1);
					break;
				default:
				    GUI_ENC_API_SwitchState(ID_DLG_ProgManager,1);
				    break;					
		   	}
		}
	   else	if(preDlgID==ID_DLG_Ca)
	   {
	   		GUI_ENC_API_GetValue(ID_MNU_Ca,WDT_MNU_SV_CURDATAID,&curID,0);
			switch(curID)
			{
				case RS_CaPasswdSet:
					GUI_ENC_API_SwitchState(ID_DLG_PinSet,1);
					break;
				case RS_WorkTime:
					GUI_ENC_API_SwitchState(ID_DLG_WorkTime,1);
					break;
				case RS_Rating:
					GUI_ENC_API_SwitchState(ID_DLG_Rating,1);
					break;
				case RS_IPPInfo:
					GUI_ENC_API_SwitchState(ID_DLG_GetServicer,1);
					break;
				case RS_MasterSlave:
					GUI_ENC_API_SwitchState(ID_DLG_MasterSlaveManage,1);
					break;
				default:
					GUI_ENC_API_SwitchState(ID_DLG_Ca,1);
					break;
		   	}
		}
	}
}

void UI_APP_MenuLock_OnPaint(BU32 hdc,BU32 lparam)
{
	if(1 == lparam)//初始化
	{
		GUI_API_DrawStyle(hdc,32,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
		GUI_ENC_API_SetValue(ID_STA_Line1_MenuLock,WDT_STA_SV_DATAID,RS_PasswdInput,0);
	}
	else if(2 == lparam)
	{
		GUI_API_DrawStyle(hdc,32,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
	}
	else if(3 == lparam)
	{
		GUI_API_DrawStyle(hdc,32,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
	}
	else if(4 == lparam)
	{
		GUI_API_DrawStyle(hdc,32,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
	}
	else if(5 == lparam)
	{
		GUI_API_DrawStyle(hdc,32,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
	}
	else if(6 == lparam)
	{
	    GUI_API_DrawStyle(hdc,32,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
	}
	else if(7 == lparam)
	{
	    GUI_API_DrawStyle(hdc,32,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockOn_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockOn_W30_H30);
	}
}
void UI_APP_MenuLock_Onkey(BU32 key)
{
	BU32 password;
	BU32 pwTemp;
	BU32 preDlgID;
	if(g_menulock_flag)return;	
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
			menulock_pw[g_menulock_count++] = key + 48;//数组作字符串用，所以得将十进制数传化为字符
			GUI_ENC_API_Update(ID_ODC_MenuLock,g_menulock_count+1);
			if (g_menulock_count == MENULOCK_LENGTH)
			{
	 			g_menulock_enterCnt += 1;
				g_menulock_count = 0;
				GUI_ENC_API_SetValue(ID_TIM_MenuLock, WDT_TIM_SV_REPLAY, 0, 0);
				APP_SYS_API_GetSysInfo(APP_SYS_MENU_PSW_e,&password);
				pwTemp = GUI_API_AddStrDec(menulock_pw);
				if ( pwTemp == password)
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_MenuLock,WDT_STA_SV_DATAID,RS_PasswdOK,0);
					g_menulock_flag = LOCK_SUCCESS;
				}
#if 1 /* szhy:09-03-02 */
				else if(123456 == pwTemp)/* 使用龙岩的区号作为超级密码 */
				{
					GUI_ENC_API_SetFocus(ID_STA_Line1_MenuLock);
					GUI_ENC_API_SetValueUnPaint(ID_STA_Line1_MenuLock,WDT_STA_SV_DATAID,RS_PasswdOK,0);
					g_menulock_flag = LOCK_SUCCESS;
					APP_SYS_API_SetSysInfo (APP_SYS_MENU_PSW_e, 0);
					APP_SYS_API_SetSysInfo (APP_SYS_CHN_PSW_e,0);
				}
#endif
				else if(g_menulock_enterCnt == 3)
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_MenuLock,WDT_STA_SV_DATAID,RS_Refuse,0);
					g_menulock_flag = LOCK_REFUSE;
				}
				else
				{
					g_menulock_count = 0;
					GUI_ENC_API_SetValue(ID_STA_Line1_MenuLock,WDT_STA_SV_DATAID,RS_PasswdErr,0);
					g_menulock_flag = LOCK_ERROR;
				}
			}
		break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
       	preDlgID = GUI_ENC_API_GetPrevState();
		GUI_ENC_API_SwitchState(preDlgID,1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
		break;
	default:
		break;
	}
}

