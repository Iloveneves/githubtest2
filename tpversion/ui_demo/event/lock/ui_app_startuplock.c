#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define LOCK_NULL      	0
#define LOCK_ERROR  	1
#define LOCK_SUCCESS  	2
#define LOCK_REFUSE     3

#define STARTUPLOCK_LENGTH 6
static BU08 g_startuplock_flag;
static BU08 g_startuplock_enterCnt; //输入密码次数
static BU32 g_startuplock_count;    //输入密码个数
static BU08 startuplock_pw[7];

//开机锁
void UI_APP_StartUpLock_Enter(void)
{	
	g_startuplock_flag = LOCK_NULL;
	g_startuplock_enterCnt = 0;
	g_startuplock_count	= 0;
	memset(startuplock_pw, 0, sizeof(startuplock_pw));

}
void UI_APP_StartUpLock_End(void)
{

}
void UI_APP_StartUpLock_Key(BU32 key)
{

}

void UI_APP_StartUpLock_Timer(void)
{
	BU32 curID;
	
	if(g_startuplock_flag == LOCK_ERROR)
	{
		g_startuplock_flag = LOCK_NULL;
		GUI_ENC_API_Update(ID_ODC_StartUpLock,1);
	}
	else if(g_startuplock_flag == LOCK_REFUSE)
	{
		GUI_ENC_API_SwitchState(ID_DLG_StartUpLock,1);
	}
	else if(g_startuplock_flag == LOCK_SUCCESS)
	{
		g_startuplock_flag = LOCK_NULL;
		UI_APP_FirstState();
	}
}

void UI_APP_StartUpLock_OnPaint(BU32 hdc,BU32 lparam)
{
	if(1 == lparam)//初始化
	{
		GUI_API_DrawStyle(hdc,32,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,67,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,102,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,137,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,172,73,STYLE_LockNo_W30_H30);
		GUI_API_DrawStyle(hdc,207,73,STYLE_LockNo_W30_H30);
		GUI_ENC_API_SetValue(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_DATAID,RS_PasswdInput,0);
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

void UI_APP_StartUpLock_Onkey(BU32 key)
{
	BU32 password;
	BU32 pwTemp;
	if(g_startuplock_flag)return;	
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
			startuplock_pw[g_startuplock_count++] = key + 48;//数组作字符串用，所以得将十进制数传化为字符
			GUI_ENC_API_Update(ID_ODC_StartUpLock,g_startuplock_count+1);
			if (g_startuplock_count == STARTUPLOCK_LENGTH)
			{
	 			g_startuplock_enterCnt += 1;
				g_startuplock_count = 0;
				GUI_ENC_API_SetValue(ID_TIM_StartUpLock, WDT_TIM_SV_REPLAY, 0, 0);
				APP_SYS_API_GetSysInfo(APP_SYS_STARTUP_PSW_e,&password);
				pwTemp = GUI_API_AddStrDec(startuplock_pw);
				if ( pwTemp == password)
				{
					GUI_ENC_API_SetValue(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_DATAID,RS_PasswdOK,0);
					g_startuplock_flag = LOCK_SUCCESS;
				}
#if 1 /* szhy:09-03-02 */
				else if(123456 == pwTemp)/* 使用龙岩的区号作为超级密码 */
				{
					GUI_ENC_API_SetFocus(ID_STA_InfoStr_StartUpLock);
					GUI_ENC_API_SetValueUnPaint(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_DATAID,RS_PasswdOK,0);
					g_startuplock_flag = LOCK_SUCCESS;
					APP_SYS_API_SetSysInfo (APP_SYS_MENU_PSW_e, 0);
					APP_SYS_API_SetSysInfo (APP_SYS_CHN_PSW_e,0);
					APP_SYS_API_SetSysInfo(APP_SYS_STARTUP_LOCK_e,0);
				}
#endif
				else if(g_startuplock_enterCnt == 3)
				{
					GUI_ENC_API_SetValue(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_DATAID,RS_Refuse,0);
					g_startuplock_flag = LOCK_REFUSE;
				}
				else
				{
					g_startuplock_count = 0;
					GUI_ENC_API_SetValue(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_DATAID,RS_PasswdErr,0);
					g_startuplock_flag = LOCK_ERROR;
				}
			}
		break;
	default:
		break;
	}
}


