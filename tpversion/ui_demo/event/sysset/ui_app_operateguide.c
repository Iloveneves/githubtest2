 #include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define OPERATIONGUIDE_CONTENT_NUM 40      //说明的行数
#define OPERATIONGUIDE_CONTENT_LENGTH 60   //每行的最长字节数 

#define OPERATIONGUIDE_CONTENT_NUMINPAGE 8 //num in page

static BU08 g_operationGuide_Content[OPERATIONGUIDE_CONTENT_NUM][OPERATIONGUIDE_CONTENT_LENGTH]=
{
     "01   机顶盒电源 通过遥控器开机和关机",
	 "02     喜爱     进入喜爱频道列表",
	 "03     静音     关闭声音输出和打开声音输出 ",
	 "04     高清     进入高清分类节目",
	 "05     点播     打开视频点播服务界面",
	 "06     时移     进入时移功能",
	 "07     资讯     打开数据广播服务界面",
	 "08   电视/广播  数字电视和音频广播之间切换",
	 "09   	 指南     打开节目指南服务界面",
	 "10     邮件     打开邮件列表",
	 "11     游戏     进入本地游戏",
	 "12    红色键    功能键1",
	 "13    绿色键    功能键2",
	 "14    黄色键    功能键3",
	 "15    蓝色键    功能键4/全屏下查看节目状态",
	 "16    节目表    进入节目列表",
	 "17     菜单     打开主菜单,或者关闭当前窗口",
	 "18     返回     返回上级菜单/最近观看的频道间切换",	
	 "19     退出     退出到全屏状态",
	 "20     上键     向上移动光标/增大频道",
	 "21     下键     向下移动光标/减小频道",
	 "22     左键     向左移动光标/减小音量",
	 "23     右键     向右移动光标/增大音量",
	 "24     确认     确定选中",
	 "25     音量+    增大音量",
	 "26     音量-    减小音量",
	 "27   上页/下页  上、下翻页",
	 "28    频道信息  显示节目信息,再按一次显示节目状态信息",
	 "29     切换     从数据广播切换到电视播放",
	 "30     声道     立体声,左声道,右声道选择切换键",
	 "31     帮助     进入使用帮助页面",
	 "32-40   0-9     数字键",
	 "灰色按键       学习键,具体使用方法如下:",
	 "               1.按住SET键至少3秒至红色指示灯长亮;",
	 "               2.按一下学习区要设置的键(音量+,音量-,",
	 "               AV/TV等),红色指示灯开始闪烁;",
	 "               3.将电视机遥控器对准机顶盒遥控器的发射",
	 "               管,按下要学习的按键,待机顶盒遥控停止闪",
	 "               烁后松开,此时表示该键功能已学习;",
	 "               4.重复2.3可设置其他按键,按下SET键学习完毕."
};

static GUI_Style_id g_operationGuide_Style[]=
{
    STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL
	
};

static BU08 g_operationGuide_contentInPage[OPERATIONGUIDE_CONTENT_NUMINPAGE][OPERATIONGUIDE_CONTENT_LENGTH];
static GUI_Style_id g_operationGuide_styleInPage[OPERATIONGUIDE_CONTENT_NUMINPAGE];

static BU08 g_operationGuide_curItem = 0; //当前选中的选项
static BU08 g_operationGuide_curPage = 0; //当前页
static BU08 g_operationGuide_PageNum;     //总页数
static BU08 g_operationGuide_rest;        //当前页中选项的数目

void OperationGuide_IPI_SetParam(void)
{
    g_operationGuide_curItem = 0;
	g_operationGuide_curPage = 0;
	g_operationGuide_PageNum = (OPERATIONGUIDE_CONTENT_NUM - 1)/OPERATIONGUIDE_CONTENT_NUMINPAGE +1;
	g_operationGuide_rest = OPERATIONGUIDE_CONTENT_NUM  %  OPERATIONGUIDE_CONTENT_NUMINPAGE;

	if(g_operationGuide_rest == 0)
	{
	    g_operationGuide_rest = OPERATIONGUIDE_CONTENT_NUMINPAGE;
	}
	
    GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_MAX, (BU32)OPERATIONGUIDE_CONTENT_NUM - 1,0);
	GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_MIN, (BU32)0,0);
	GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_VALUE, (BU32)g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE + g_operationGuide_curItem,0);

}
void OperationGuide_IPI_ShowPage(void)
{ 
    BU08 curPageNum;
	BU08 i;
    if(g_operationGuide_curPage  >=  g_operationGuide_PageNum)
    {
        g_operationGuide_curPage = 0;
		g_operationGuide_curItem = 0;
    }

	if(g_operationGuide_curPage + 1 == g_operationGuide_PageNum)
	{
	    curPageNum = g_operationGuide_rest;
	}
	else
	{
	    curPageNum = OPERATIONGUIDE_CONTENT_NUMINPAGE;
	}
	/*清空缓存*/
	for(i = 0; i < OPERATIONGUIDE_CONTENT_NUMINPAGE; i++)
	{
         GUI_API_Memset(g_operationGuide_contentInPage[i], 0, GUI_API_Strlen(g_operationGuide_contentInPage[i]));
		 g_operationGuide_styleInPage[i] = STYLE_NULL;
	}
	for(i = 0; i< curPageNum; i++)
	{
	    GUI_API_Memcpy(g_operationGuide_contentInPage[i], g_operationGuide_Content[i + g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE],
			           GUI_API_Strlen(g_operationGuide_Content[i + g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE]));
        g_operationGuide_styleInPage[i] = g_operationGuide_Style[i + g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE];
	}
	GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_VALUE, (BU32)g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE + g_operationGuide_curItem,0);
    GUI_ENC_API_Update(ID_ODC_OperationGuide, g_operationGuide_curItem);
}
void UI_APP_OperationGuide_Enter(void)
{
    OperationGuide_IPI_SetParam();
	OperationGuide_IPI_ShowPage();
	APP_Com_API_SetFullScreen();
}

void UI_APP_OperationGuide_Key(BU32 key)
{
    BU08 curPageNum;
	
	
    switch(key)
    {
    case GUI_VK_UP:
		if(g_operationGuide_curItem == 0)
		{
		     if(g_operationGuide_curPage == 0)
		     {
		          g_operationGuide_curPage = g_operationGuide_PageNum -1;
				  g_operationGuide_curItem = g_operationGuide_rest - 1;
		     }
			 else
			 {    
			      g_operationGuide_curPage--;
			      g_operationGuide_curItem = OPERATIONGUIDE_CONTENT_NUMINPAGE - 1;
			 }
		}
		else
		{
		    g_operationGuide_curItem--;
		}
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_DOWN:
		curPageNum = (g_operationGuide_curPage + 1 == g_operationGuide_PageNum)?
			          g_operationGuide_rest : OPERATIONGUIDE_CONTENT_NUMINPAGE;
		if(g_operationGuide_curItem == curPageNum - 1)
		{
		     g_operationGuide_curPage++;
			 g_operationGuide_curItem = 0;
		}
        else
        {
             g_operationGuide_curItem++;
        }
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_P_UP:
		if(g_operationGuide_curPage == 0)
		{
		     g_operationGuide_curPage = g_operationGuide_PageNum - 1;
		}
		else
		{
		     g_operationGuide_curPage--;
		}
		curPageNum = (g_operationGuide_curPage + 1 == g_operationGuide_PageNum)?
			          g_operationGuide_rest : OPERATIONGUIDE_CONTENT_NUMINPAGE;
		g_operationGuide_curItem = curPageNum - 1;
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_P_DOWN:
		g_operationGuide_curPage++;
		g_operationGuide_curItem = 0;
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	    GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_SELECT, RS_SystemSetting, 0);
		 GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_INDEX, 8, 0);
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
    }
}

void UI_APP_OperationGuide_OnPaint(BU32 hdc,BU32 lParam)
{
    BU32 i;
	BU08 curPageNum;
    lParam = g_operationGuide_curItem;

	GUI_API_DrawStyle(hdc, 0, 0, STYLE_OperationGuideBk_W620_H304);
	GUI_API_DrawStyle(hdc, 0, 38*lParam,STYLE_OperationGuideBar_W620_H36);

    if(g_operationGuide_curPage + 1 == g_operationGuide_PageNum)
	{
	    curPageNum = g_operationGuide_rest;
	}
	else
	{
	    curPageNum = OPERATIONGUIDE_CONTENT_NUMINPAGE;
	}
	
	for(i = 0; i < curPageNum; i++)
	{
	    if(g_operationGuide_styleInPage[i] != STYLE_NULL)
	    {
	        GUI_API_DrawStyle(hdc, 56, 38*i+3, g_operationGuide_styleInPage[i]);
	    }
		GUI_API_TextOut(hdc,2,38*i + 10,g_operationGuide_contentInPage[i],
			            GUI_API_Strlen(g_operationGuide_contentInPage[i]), COLOR_BLACK, COLOR_BLACK, 1);
	}
}

