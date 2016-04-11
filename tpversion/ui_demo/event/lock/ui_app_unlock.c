#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define LOCK_NULL      	0
#define LOCK_ERROR  	1
#define LOCK_SUCCESS  	2
#define LOCK_REFUSE     3

//是否一次性解锁提示框
void UI_APP_ChUnLock_Key(BU32 key)
{
	BU32 prestate;
	if(key == GUI_VK_SELECT)
	{
		UI_APP_API_SetChnBUnlock(1);
	}
	prestate = UI_APP_API_GetLockCurState();
	if (prestate != ID_DLG_Null)
		GUI_ENC_API_SwitchState(prestate,1);
	else
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	//GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK_END, 0, 0);
}

