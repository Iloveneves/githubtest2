#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

static BU08 g_show_info_type = 0;
void UI_APP_SetShowInfoType(BU08 type)
{
	g_show_info_type = type;
}
//,精彩编排
static BU08	doublehelpinfo[1025]="    如需要开通互动点播业务,您需要配置标清互动型或高清互动型的机顶盒。互动点播里有直播全频道、家庭电影院、强档电视剧、新闻天天看、娱乐全接触、体育最前线、栏目大拼盘、教育充电馆海量内容,让您领略家庭娱乐魅力,坐拥雄厚节目资源,打造您的数字视觉盛宴!南平数字互动电视让您成为电视的真正主人!资费标准:互动基本包为35元/月.卡,影视大包月为48元/月.卡。详情到广电营业厅或拨打24小时客服热线968839咨询。"; 
static BU08	time_shift_info[1025]="   我们想为您做的是把错过的精彩找回来！目前我们提供16个直播频道，72小时内节目任意回放，它们分别是中央1套、中央2套、中央3套、中央5套、中央6套、中央8套、中央10套、中央12套、中央新闻频道、福建体育、福建新闻、福建综合、南平1套、湖南卫视、江苏卫视、浙江卫视。资费标准：20元/月.卡。"; 
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
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
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


