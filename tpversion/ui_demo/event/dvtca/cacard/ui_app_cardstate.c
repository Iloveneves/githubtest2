#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"


//¿¨×´Ì¬
void UI_APP_CA_CardState_Enter(void)
{}
void UI_APP_CA_CardState_End(void)
{}
void UI_APP_CA_CardState_Key(BU32 key)
{
	if(key == GUI_VK_EXIT)
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
}
void UI_APP_CA_CardState_Timer(void)
{
	GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
}

