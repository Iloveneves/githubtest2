#include "include/resource.h"

GUI_StateTable_s stateTable[]=
{

//机顶盒信息
	{ID_DLG_StbInfo,  IDC_ALL, IDI_ALL, GUI_VK_EXIT, ID_DLG_ChnInfo, STF_NORMAL},
	{ID_DLG_StbInfo,  IDC_ALL, IDI_ALL, GUI_VK_RECALL, ID_DLG_SysSet, STF_NORMAL},
	{ID_DLG_StbInfo,  IDC_ALL, IDI_ALL, GUI_VK_MENU, ID_DLG_SysSet, STF_NORMAL},
	
//密码修改成功
	{ID_DLG_PasswordSetInfo, ID_TIM_PasswordSetInfo, IDI_ALL, GUI_VK_NULL, ID_DLG_PasswordSet,	STF_NORMAL},

//俄罗斯方块
	{ID_DLG_Russia,  IDC_ALL, IDI_ALL, GUI_VK_RECALL, ID_DLG_SysSet, STF_NORMAL}, 
	{ID_DLG_Russia,  IDC_ALL, IDI_ALL, GUI_VK_MENU, ID_DLG_SysSet, STF_NORMAL},	
	{ID_DLG_Russia,  IDC_ALL, IDI_ALL, GUI_VK_EXIT, ID_DLG_ChnInfo, STF_NORMAL},	

//贪吃蛇
	{ID_DLG_Snake,	IDC_ALL, IDI_ALL, GUI_VK_RECALL, ID_DLG_SysSet, STF_NORMAL},
	{ID_DLG_Snake,	IDC_ALL, IDI_ALL, GUI_VK_MENU, ID_DLG_SysSet, STF_NORMAL},
	{ID_DLG_Snake,	IDC_ALL, IDI_ALL, GUI_VK_EXIT, ID_DLG_ChnInfo, STF_NORMAL},

//推箱子
	{ID_DLG_Box,  IDC_ALL, IDI_ALL, GUI_VK_RECALL, ID_DLG_SysSet, STF_NORMAL},
	{ID_DLG_Box,  IDC_ALL, IDI_ALL, GUI_VK_MENU, ID_DLG_SysSet, STF_NORMAL},
    {ID_DLG_Box,  IDC_ALL, IDI_ALL, GUI_VK_EXIT, ID_DLG_ChnInfo, STF_NORMAL},

//智能卡基本信息 
	{ID_DLG_ICInfo, IDC_ALL, IDI_ALL,GUI_VK_EXIT, ID_DLG_Ca, STF_NORMAL},

};

BU32 stateTableNum = sizeof(stateTable)/sizeof(GUI_StateTable_s);
