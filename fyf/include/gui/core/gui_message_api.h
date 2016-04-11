
#ifndef __GUI_MESSAGE_API_H_
#define __GUI_MESSAGE_API_H_


//GUI_API_SetTimer mode para
#define TM_ONLY					(0)
#define TM_CYCLE				(1)
//Key Message
#define WM_KEY						0x4003

//common message

#define WM_QUIT						0x5000
#define WM_PAINT					0x5001
#define WM_NCPAINT					0x5002
#define WM_SETFOCUS					0x5003
#define WM_KILLFOCUS				0x5004
#define WM_CREATE					0x5005
#define WM_DESTROY					0x5006
#define WM_COMMAND					0x5007
#define WM_SHOWWINDOW				0x5008

#define WM_TIMER					0x500D

#define WM_USER						0xFF00

#define KEY_RECEIVE_OPEN			0x1100
#define KEY_RECEIVE_CLOSE			0x0011

#define TIMER_RUN_OPEN				0x2200
#define TIMER_RUN_CLOSE				0x0022

/**********struct***************/
//定义了画图样式的子结构体
typedef struct	_tagGUI_VKeyTable_s				
{
	BU08 GUI_VK_0 ;
	BU08 GUI_VK_1;
	BU08 GUI_VK_2;
	BU08 GUI_VK_3;
	BU08 GUI_VK_4;
	BU08 GUI_VK_5;
	BU08 GUI_VK_6;
	BU08 GUI_VK_7;
	BU08 GUI_VK_8;
	BU08 GUI_VK_9;

	BU08 GUI_VK_LEFT;
	BU08 GUI_VK_RIGHT;
	BU08 GUI_VK_UP;
	BU08 GUI_VK_DOWN;
	BU08 GUI_VK_SELECT;

	BU08 GUI_VK_MENU;
	BU08 GUI_VK_EXIT;
	
	BU08 GUI_VK_POWER;
	
	BU08 GUI_VK_MUTE;
	BU08 GUI_VK_TVRADIO;
	BU08 GUI_VK_RECALL;
	BU08 GUI_VK_AUDIO;
	
	//extend up/down control key
	BU08 GUI_VK_C_UP;//ch+
	BU08 GUI_VK_C_DOWN;//ch-
	BU08 GUI_VK_V_UP;//vol+
	BU08 GUI_VK_V_DOWN;//vol-
	BU08 GUI_VK_P_UP;//page+
	BU08 GUI_VK_P_DOWN;//page-
	
	BU08 GUI_VK_EPG;
	
	//color keys
	BU08 GUI_VK_RED;
	BU08 GUI_VK_GREEN;
	BU08 GUI_VK_YELLOW;
	BU08 GUI_VK_BLUE;	
}GUI_VKeyTable_s,* GUI_VKeyTable_sp;

typedef struct	_tagGUI_VKeyValue_s				
{
	GUI_VKeyTable_s GUI_VKey_Value;
	BU32 			GUI_Key_Value;
	
}GUI_VKeyValue_s,* GUI_VKeyValue_sp;

typedef BU32	(*SENDMSGHOOK)(BU32 hWnd,BU32 type,BU32 * value,BU32 * Param);

BU32 GUI_API_SendMessage(BU32 hWnd,BU32 iMsg,BU32 wParam,BU32 lParam);
BU32 GUI_API_PostMessage(BU32 hWnd,BU32 Msg,BU32 wParam,BU32 lParam);
void GUI_API_ProcessMsg(void);
BU32 GUI_API_PostKeyMessage(BU32 value,BU32 state);
void GUI_API_SetTimerTick(BU32 nms);
void GUI_API_SetTimer(BU32 hWnd,BU32 ID,BU32 valuems,BU32 mode);
void GUI_API_KillTimer(BU32 hWnd,BU32 ID);
void GUI_API_PauseTimer(BU32 hWnd,BU32 ID);
void GUI_API_ResumeTimer(BU32 hWnd,BU32 ID);
void GUI_API_SetTimerValue(BU32 hWnd,BU32 ID,BU32 value);
void GUI_API_TimerProcess(void);
//old
GUI_VKeyTable_sp GUI_API_GetKeyTable(void);
void GUI_API_RegisterKeyTable(void * table,BU32 num);
BU32 GUI_API_GetKeyNumValue(BU08 keyValue);
//sys driver
void GUI_API_SetSYSDevice(GUI_SystemDevice_sp sysdevice);//3.00
void GUI_API_SetSYSDevice_Semaphore(GUI_Semaphore_sp guisemaphore);

BU32 GUI_API_SetSendMessageHookPre(SENDMSGHOOK sendmsghookpre);
BU32 GUI_API_SetSendMessageHookEnd(SENDMSGHOOK sendmsghookend);

void GUI_API_WaitMsgProtect(void);
void GUI_API_ReleaseMsgProtect(void);

void GUI_API_CreateProtect(void);
void GUI_API_SetKeyReceiveMode(BU32 mode);
//设置按键接收模式 KEY_RECEIVE_OPEN为打开接收模式 KEY_RECEIVE_CLOSE为关闭接收模式
BU32 GUI_API_GetKeyReceiveMode(void);

#endif


