#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"


extern CA_PIN_STATE_E g_pinState;

//PINºÏ≤ÈÃ· æ
void UI_APP_CA_PinNote_Key(BU32 key)
{
	if(g_pinState == CA_PIN_LOCKED_e)
	{
		if(key == GUI_VK_EXIT)
		{
			g_pinState = CA_PIN_NULL_e;
			GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		}
	}
}
void UI_APP_CA_PinNote_Timer(void)
{
	BU32 pre_state;
	if(g_pinState == CA_PIN_LOCKED_e)
	{
		g_pinState = CA_PIN_NULL_e;
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
	}
	else if(g_pinState == CA_PIN_ERROR_e)
	{
		g_pinState = CA_PIN_NULL_e;
		pre_state = UI_APP_API_GetPinNotePreState();
		GUI_ENC_API_SwitchState(pre_state, 1);
	}
	else if(g_pinState == CA_CHANGE_SUCESS_e)
	{
		g_pinState = CA_PIN_NULL_e;
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
	}
	else if(g_pinState == CA_CHANGE_FAIL_e)
	{
		pre_state = UI_APP_API_GetPinNotePreState();
		if(pre_state == ID_DLG_PinSet)
		{
			GUI_ENC_API_SwitchState(ID_DLG_PinSet, 1);
			GUI_ENC_API_SetFocus(ID_TBX_New_PinSet);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		}
	}

}

