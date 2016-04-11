#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define LOCK_NULL      	0
#define LOCK_ERROR  	1
#define LOCK_SUCCESS  	2
#define LOCK_REFUSE     3

#define CHNLOCK_LENGTH 6
static BU08 g_chnlock_flag;
static BU08 g_chnlock_enterCnt; //输入密码次数
static BU32 g_chnlock_count;	//输入密码个数
static BU08 chnlock_pw[7];
//节目加锁
void UI_APP_ChnLock_Enter(void)
{
	g_chnlock_flag = LOCK_NULL;
	g_chnlock_enterCnt = 0;
	g_chnlock_count	= 0;
	memset(chnlock_pw, 0, sizeof(chnlock_pw));
}
void UI_APP_ChnLock_End(void)
{

}
void UI_APP_ChnLock_Key(BU32 key)
{

}

void UI_APP_ChnLock_OnPaint(BU32 hdc,BU32 lparam)
{
	if(1 == lparam)//初始化
	{
	    GUI_API_DrawStyle(hdc,32,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
		GUI_ENC_API_SetValue(ID_STA_Tex_ChnLock,WDT_STA_SV_DATAID,RS_PasswdInput,0);
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
void UI_APP_ChnLock_Onkey(BU32 key)
{

	BU32 password,pwTemp;
	BU32 prestate=0;
	BU16 curChn, chnNum;
	if(g_chnlock_flag)	return;
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
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
			chnlock_pw[g_chnlock_count++] = key + 48;//数组作字符串用，所以得将十进制数传化为字符
			GUI_ENC_API_Update(ID_ODC_ChnLock,g_chnlock_count+1);
			if (g_chnlock_count == CHNLOCK_LENGTH)
			{
				g_chnlock_enterCnt++;
				g_chnlock_count = 0;
				GUI_ENC_API_SetValue(ID_TIM_ChnLock, WDT_TIM_SV_REPLAY, 0, 0);
				APP_SYS_API_GetSysInfo(APP_SYS_CHN_PSW_e,&password);
				pwTemp = GUI_API_AddStrDec(chnlock_pw);
				if (pwTemp == password)
				{
					GUI_ENC_API_SetFocus(ID_STA_Tex_ChnLock);
					GUI_ENC_API_SetValue(ID_STA_Tex_ChnLock,WDT_STA_SV_DATAID,RS_PasswdOK,0);
					g_chnlock_flag = LOCK_SUCCESS;
				}
#if 1 /* szhy:09-03-02 */
				else if(123456 == pwTemp)/* 使用龙岩的区号作为超级密码 */
				{
					GUI_ENC_API_SetFocus(ID_STA_Tex_ChnLock);
					GUI_ENC_API_SetValueUnPaint(ID_STA_Tex_ChnLock,WDT_STA_SV_DATAID,RS_PasswdOK,0);
					g_chnlock_flag = LOCK_SUCCESS;
					APP_SYS_API_SetSysInfo (APP_SYS_MENU_PSW_e, 0);
					APP_SYS_API_SetSysInfo (APP_SYS_CHN_PSW_e,0);
				}
#endif
				else if(g_chnlock_enterCnt == 3)
				{
					g_chnlock_enterCnt = 0;
					GUI_ENC_API_SetValue(ID_STA_Tex_ChnLock,WDT_STA_SV_DATAID,RS_Refuse,0);
					g_chnlock_flag = LOCK_REFUSE;
				}
				else
				{
					g_chnlock_count = 0;
					GUI_ENC_API_SetValue(ID_STA_Tex_ChnLock,WDT_STA_SV_DATAID,RS_PasswdErr,0);
					g_chnlock_flag = LOCK_ERROR;
				}
			}
			break;
		case GUI_VK_UP:
			prestate = UI_APP_API_GetLockCurState();
			if (prestate == ID_DLG_Null)
				return;
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(prestate == ID_DLG_ChnInfo)
			{
				curChn++;
				if(curChn > chnNum-1)
					curChn=0;
			}
			else
			{
				if(curChn == 0)
					curChn = chnNum-1;
				else
					curChn--;
			}
			APP_ChnDatabase_API_SetCurChnNO(curChn);
			GUI_ENC_API_SwitchState(prestate,1);
			//GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK_END, 0, 0);
			break;
		case GUI_VK_DOWN:
			prestate = UI_APP_API_GetLockCurState();	
			if (prestate == ID_DLG_Null)
				return;
			
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(prestate == ID_DLG_ChnInfo)
			{
				if(curChn == 0)
					curChn = chnNum-1;
				else
					curChn--;
			}
			else
			{
				curChn++;
				if(curChn > chnNum-1)
					curChn=0;
			}
			APP_ChnDatabase_API_SetCurChnNO(curChn);
			GUI_ENC_API_SwitchState(prestate,1);
			//GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK_END, 0, 0);
			break;
		case GUI_VK_RECALL:
		case GUI_VK_MENU:
			prestate = UI_APP_API_GetLockCurState();
			if (prestate == ID_DLG_Null)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
			}
			else
			{
				GUI_ENC_API_SwitchState(prestate,1);
			}
			UI_APP_API_SetCurChnbPlay(2);
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK_END, 0, 0);
			break;
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_SetCurChnbPlay(2);
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK_END, 0, 0);
			break;
		default:
			break;
	}
}
void UI_APP_ChnLock_Timer(void)
{
	BU32 preDlgID;
	BU32 prestate;
	if(g_chnlock_flag == LOCK_ERROR)
	{
		g_chnlock_flag = LOCK_NULL;
		GUI_ENC_API_Update(ID_ODC_ChnLock,1);
	}
	else if(g_chnlock_flag == LOCK_REFUSE)
	{
		prestate = UI_APP_API_GetLockCurState();
		GUI_ENC_API_SwitchState(prestate,1);
	}
	else if(g_chnlock_flag == LOCK_SUCCESS)
	{
		g_chnlock_flag = LOCK_NULL;
		GUI_ENC_API_SwitchState(ID_DLG_Unlock,1);
		UI_APP_API_SetCurChnbPlay(1);

		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 0);
	//	UI_APP_API_CheckPlay_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),1);

	}

}

