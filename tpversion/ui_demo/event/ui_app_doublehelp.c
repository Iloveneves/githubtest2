#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

static BU08 g_show_info_type = 0;
void UI_APP_SetShowInfoType(BU08 type)
{
	g_show_info_type = type;
}
//,���ʱ���
static BU08	doublehelpinfo[1025]="    ����Ҫ��ͨ�����㲥ҵ��,����Ҫ���ñ��廥���ͻ���廥���͵Ļ����С������㲥����ֱ��ȫƵ������ͥ��ӰԺ��ǿ�����Ӿ硢�������쿴������ȫ�Ӵ���������ǰ�ߡ���Ŀ��ƴ�̡��������ݺ�������,�������Լ�ͥ��������,��ӵ�ۺ��Ŀ��Դ,�������������Ӿ�ʢ��!��ƽ���ֻ�������������Ϊ���ӵ���������!�ʷѱ�׼:����������Ϊ35Ԫ/��.��,Ӱ�Ӵ����Ϊ48Ԫ/��.�������鵽���Ӫҵ���򲦴�24Сʱ�ͷ�����968839��ѯ��"; 
static BU08	time_shift_info[1025]="   ������Ϊ�������ǰѴ���ľ����һ�����Ŀǰ�����ṩ16��ֱ��Ƶ����72Сʱ�ڽ�Ŀ����طţ����Ƿֱ�������1�ס�����2�ס�����3�ס�����5�ס�����6�ס�����8�ס�����10�ס�����12�ס���������Ƶ���������������������š������ۺϡ���ƽ1�ס��������ӡ��������ӡ��㽭���ӡ��ʷѱ�׼��20Ԫ/��.����"; 
void UI_APP_DoubleHelpInfo_Enter(void)
{
	BU32 len;
	BU32 hwnd;
	GUI_Rect_s rect;
	if (g_show_info_type == 1)
	{
		len = GUI_API_Strlen(time_shift_info);
		printf("len = %d \n",len);
		GUI_ENC_API_SetValue(ID_EDT_DoubleHelp, WDT_EDT_SV_VALUE, (BU32)time_shift_info, len);
		GUI_ENC_API_SetValue(ID_EDT_DoubleHelp, WDT_EDT_SV_PAGE_INDEX, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_Title_DoubleHelp,WDT_STA_SV_DATAID,RS_TimeShiftTV,0);
	}
	else
	{
		len = GUI_API_Strlen(doublehelpinfo);
		printf("len = %d \n",len);
		GUI_ENC_API_SetValue(ID_EDT_DoubleHelp, WDT_EDT_SV_VALUE, (BU32)doublehelpinfo, len);
		GUI_ENC_API_SetValue(ID_EDT_DoubleHelp, WDT_EDT_SV_PAGE_INDEX, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_Title_DoubleHelp,WDT_STA_SV_DATAID,RS_DoubleVod,0);
	}
	hwnd = GUI_API_GetDlgItem(ID_VFM_DoubleHelp);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//������Ƶ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_DoubleHelp,ID_STA_Event2_DoubleHelp,ID_PBR_Epg_DoubleHelp,260,1);
}

void UI_APP_DoubleHelpInfo_End(void)
{

}

void UI_APP_DoubleHelpInfo_Key(BU32 key)
{
	switch(key)
	{
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
			break;
		case GUI_VK_MENU:
		case GUI_VK_RECALL:
			GUI_ENC_API_SwitchState(UI_APP_API_GetDoubleHelpPreState(),1);
			break;
	}
}


