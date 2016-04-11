#include "include/resource.h"
#include "../event/ui_app_api.h"

/**********事件表*********/
GUI_EventTable_s eventTable[]=
{
//主菜单
	DEF_DLG_EVE(ID_DLG_MainMenu, UI_APP_MainMenu_Enter, UI_APP_MainMenu_End, UI_APP_MainMenu_OnKey)
	DEF_ODC_EVE(ID_ODC_MainMenu,UI_APP_MainMenu_OnPaint,UI_APP_MainMenu_OnKey)
	DEF_TIM_EVE(ID_TIM_MainMenu,UI_APP_MainMenu_Timer)

#ifndef NO_BROADCAST
//空对话框
	//DEF_DLG_EVE(ID_DLG_Null, UI_APP_Null_Enter, UI_APP_Null_End, 0)
	DEF_DLG_EVE(ID_DLG_Null, UI_APP_Null_Enter, UI_APP_Null_End, UI_APP_Null_Key)
#endif
//无节目自动搜索提示框
	DEF_DLG_EVE(ID_DLG_AutoInfo, UI_APP_AutoInfo_Enter,0,UI_APP_AutoInfo_Key)
	DEF_TIM_EVE(ID_TIM_AutoInfo,UI_APP_AutoInfo_Timer)
	
//节目管理
	DEF_DLG_EVE(ID_DLG_ProgManager, UI_APP_ProgManagerMenu_Enter, 0, UI_APP_ProgManagerMenu_Key)
	DEF_MNU_EVE(ID_MNU_ProgManager, UI_APP_ProgManagerMenu_Key)
	
//节目列表
	DEF_DLG_EVE(ID_DLG_ProgList, UI_APP_ChnList_Enter, 0, 0)
	DEF_TIM_EVE(ID_TIM_ProgList, UI_APP_ChnList_Timer)
	DEF_LBP_EVE(ID_LBP_Prog_ProgList, UI_APP_ChnList_Key, UI_APP_ChnList_Change)
	
//系统提示
	DEF_DLG_EVE(ID_DLG_SysInfo,0,UI_APP_SysInfo_End,0)
	DEF_TIM_EVE(ID_TIM_SysInfo,UI_APP_SysInfo_Timer)
	
//全屏节目信息
	DEF_DLG_EVE(ID_DLG_ChnInfo, UI_APP_FullScreen_Enter,UI_APP_FullScreen_End, UI_APP_FullScreen_Key)
	DEF_TIM_EVE(ID_TIM_ChnInfo, UI_APP_FullScreen_Timer)
	DEF_TIM_EVE(ID_TIM_Event_ChnInfo, UI_APP_Fullscreen_RenewEvent_Timer)
	DEF_ODC_EVE(ID_ODC_ChName_ChnInfo, UI_APP_FullScreen_TrackOnPaint, UI_APP_FullScreen_TrackKey)
	DEF_CBX_EVE(ID_CBX_Track_ChnInfo, UI_APP_FullScreen_TrackKey)
	DEF_CBX_EVE(ID_CBX_Offset_ChnInfo, UI_APP_FullScreen_TrackKey)
	DEF_ODC_EVE(ID_ODC_ADV_ChnInfo, UI_APP_FullScreen_ADVOnPaint, 0)
	DEF_TIM_EVE(ID_TIM_NETSTATE, UI_APP_NetState_Timer)

	
//节目分类
	DEF_DLG_EVE(ID_DLG_SortProgram, UI_APP_ChnSort_Enter, UI_APP_ChnSort_End, 0)
	DEF_LBP_EVE(ID_LBP_SortProgram, UI_APP_ChnSort_Key, UI_APP_ChnSort_Change)
	DEF_TIM_EVE(ID_TIM_SortProgram, UI_APP_ChnSort_Timer)
	
//搜索节目
	DEF_DLG_EVE(ID_DLG_Search,UI_APP_Search_Enter,UI_APP_Search_End,UI_APP_Search_Key)
	
//搜索节目结束提示框
	DEF_DLG_EVE(ID_DLG_SearchInfo,UI_APP_SearchInfo_Enter, UI_APP_SearchInfo_End,UI_APP_SearchInfo_Key)
	DEF_TIM_EVE(ID_TIM_SearchInfo, UI_APP_SearchInfo_Timer)
	
//手动搜索设置
	DEF_DLG_EVE(ID_DLG_ManualSearchSet, UI_APP_ManualSearchSet_Enter, UI_APP_ManualSearchSet_End, 0)
	DEF_TIM_EVE(ID_TIM_ManualSearchSet,UI_APP_ManualSearchSet_Timer)
	DEF_TBX_EVE(ID_TBX_StartFreq_ManualSearchSet,UI_APP_ManualSearchSet_Key)
	DEF_TBX_EVE(ID_TBX_EndFreq_ManualSearchSet,UI_APP_ManualSearchSet_Key)
	DEF_TBX_EVE(ID_TBX_SymbRate_ManualSearchSet, UI_APP_ManualSearchSet_Key)
	DEF_CBX_EVE(ID_CBX_QAM_ManualSearchSet, UI_APP_ManualSearchSet_Key)
	DEF_CBX_EVE(ID_CBX_NetSearch_ManualSearchSet, UI_APP_ManualSearchSet_Key)
	
//菜单加锁
	DEF_DLG_EVE(ID_DLG_MenuLock,UI_APP_MenuLock_Enter,0,0)
	DEF_TIM_EVE(ID_TIM_MenuLock,UI_APP_MenuLock_Timer)
	DEF_ODC_EVE(ID_ODC_MenuLock,UI_APP_MenuLock_OnPaint,UI_APP_MenuLock_Onkey)

//节目加锁
	DEF_DLG_EVE(ID_DLG_ChnLock,UI_APP_ChnLock_Enter,UI_APP_ChnLock_End,UI_APP_ChnLock_Key)
	DEF_ODC_EVE(ID_ODC_ChnLock,UI_APP_ChnLock_OnPaint,UI_APP_ChnLock_Onkey)
	DEF_TIM_EVE(ID_TIM_ChnLock,UI_APP_ChnLock_Timer)

//开机锁
    DEF_DLG_EVE(ID_DLG_StartUpLock,UI_APP_StartUpLock_Enter,UI_APP_StartUpLock_End,UI_APP_StartUpLock_Key)
	DEF_ODC_EVE(ID_ODC_StartUpLock,UI_APP_StartUpLock_OnPaint,UI_APP_StartUpLock_Onkey)
	DEF_TIM_EVE(ID_TIM_StartUpLock,UI_APP_StartUpLock_Timer)

//是否一次性解锁提示框
	DEF_DLG_EVE(ID_DLG_Unlock, 0, 0, UI_APP_ChUnLock_Key)

//主频点设置
	DEF_DLG_EVE(ID_DLG_MainFreqSet,UI_APP_MainFreqSet_Enter, 0, 0)
	DEF_TBX_EVE(ID_CBX_Ctr_MainFreqSet, UI_APP_MainFreqSet_Key)
	DEF_TBX_EVE(ID_TBX_MainFreqSet_MainFreqSet, UI_APP_MainFreqSet_Key)

//频道编辑
	DEF_DLG_EVE(ID_DLG_ChnEdit, UI_APP_ChnEdit_Enter, UI_APP_ChnEdit_End, 0)
	DEF_TIM_EVE(ID_TIM_ChnEdit, UI_APP_ChnEdit_Timer)
	DEF_LBP_EVE(ID_LBP_ChnList_ChnEdit, UI_APP_ChnEdit_Key, UI_APP_ChnEdit_Change)

//频道编辑退出提示框
	DEF_DLG_EVE(ID_DLG_EditInfo, UI_APP_EditInfo_Enter, UI_APP_EditInfo_End, UI_APP_EditInfo_Key)
	DEF_TIM_EVE(ID_TIM_EditInfo, UI_APP_EditInfo_Timer)

//数据广播列表
	DEF_DLG_EVE(ID_DLG_BroadcastList, UI_APP_BrdcastList_Enter, 0, 0)
	DEF_LBP_EVE(ID_LBP_BroadcastList, UI_APP_BrdcastList_Key, UI_APP_BrdcastList_Change)

//Epg
	DEF_DLG_EVE(ID_DLG_EPG, UI_APP_EPG_Enter, UI_APP_EPG_End, 0)
	DEF_LBP_EVE(ID_LBP_Prog_EPG,UI_APP_EPG_Prog_Key,UI_APP_EPG_Prog_Change)
	DEF_LBP_EVE(ID_LBP_Event_EPG,UI_APP_EPG_Event_Key,UI_APP_EPG_Event_Change)
	DEF_TIM_EVE(ID_TIM_EPG, UI_APP_EPG_Timer)
	DEF_EDT_EVE(ID_EDT_Detail_EPG, UI_APP_EPG_Detail_Key)

//Epg数据保存
	DEF_TIM_EVE(ID_TIM_EpgInfo, UI_APP_EpgInfo_Timer)

//Epg预约节目的冲突提示框
	DEF_DLG_EVE(ID_DLG_ConflictInfo, UI_APP_ConflictInfo_Enter, UI_APP_ConflictInfo_End, UI_APP_ConflictInfo_Key)

//Epg中不能预约提示框
	DEF_DLG_EVE(ID_DLG_EpgReserveInfo,UI_APP_EpgReserveInfo_Enter,0,UI_APP_EpgReserveInfo_Key)
	DEF_TIM_EVE(ID_TIM_EpgReserveInfo, UI_APP_EpgReserveInfo_Timer)

//预约节目列表
	DEF_DLG_EVE(ID_DLG_ReserveList, UI_APP_ReserveList_Enter, UI_APP_ReserveList_End, 0)
	DEF_LBX_EVE(ID_LBX_ReserveList, UI_APP_ReserveList_Key)

//预约节目列表提示框
	DEF_DLG_EVE(ID_DLG_ReserveInfo,UI_APP_ReserveInfo_Enter,UI_APP_ReserveInfo_End,UI_APP_ReserveInfo_Key)

//预约节目播放提示框
	DEF_DLG_EVE(ID_DLG_TimeOut,UI_APP_TimeOut_Enter,UI_APP_TimeOut_End,UI_APP_TimeOut_Key)
	DEF_TIM_EVE(ID_TIM_TimeOut, UI_APP_TimeOut_Timer)
	DEF_ODC_EVE(ID_ODC_TimeOut,UI_APP_TimeOut_OnPaint,UI_APP_TimeOut_Key)

//游戏天地
	DEF_DLG_EVE(ID_DLG_Game,UI_APP_Game_Enter, UI_APP_Game_End, 0)
	DEF_MNU_EVE(ID_MNU_Game, UI_APP_Game_Key)

//俄罗斯方块
	DEF_DLG_EVE(ID_DLG_Russia,UI_APP_Game_Russia_Enter, UI_APP_Game_Russia_End, 0)
	DEF_ODC_EVE(ID_ODC_Russia, Game_Russia_OnPaint, Game_Russia_OnKey)
	DEF_TIM_EVE(ID_TIM_Russia, Game_Russia_Timing)
	
//贪吃蛇
	DEF_DLG_EVE(ID_DLG_Snake,UI_APP_Game_Snake_Enter, UI_APP_Game_Snake_End, 0)
	DEF_ODC_EVE(ID_ODC_Snake, Game_Snake_OnPaint, Game_Snake_OnKey)
	DEF_TIM_EVE(ID_TIM_Snake, Game_Snake_Timing)
	
//推箱子
	DEF_DLG_EVE(ID_DLG_Box,UI_APP_Game_Box_Enter, 0, 0)
	DEF_ODC_EVE(ID_ODC_Box, Game_Box_OnPaint, Game_Box_OnKey)

//系统设置
	DEF_DLG_EVE(ID_DLG_SysSet, UI_APP_SysSetMenu_Enter, UI_APP_SysSetMenu_End, 0)
	DEF_MNU_EVE(ID_MNU_SysSet, UI_APP_SysSetMenu_Key)

//时区设置
	DEF_DLG_EVE(ID_DLG_TimeZoneSet,UI_APP_TimeZoneSet_Enter,UI_APP_TimeZoneSet_End,0)
	DEF_CBX_EVE(ID_CBX_TimeZone_TimeZoneSet,UI_APP_TimeZoneSet_Key)
	
//本机信息
	DEF_DLG_EVE(ID_DLG_StbInfo, UI_APP_STBInfo_Enter,0,UI_APP_STBInfo_Key)

//信号检测
	DEF_DLG_EVE(ID_DLG_SignalCheck, UI_APP_SignalDectec_Enter,UI_APP_SignalDectec_End,0)
	DEF_LBP_EVE(ID_LBP_SignalCheck, UI_APP_SignalDectec_Key, UI_APP_SignalDectec_Change)
	DEF_TIM_EVE(ID_TIM_SignalCheck, UI_APP_SignalDectec_Timer)

//恢复出厂设置
	DEF_DLG_EVE(ID_DLG_Default, UI_APP_FactoryDefault_Enter,UI_APP_FactoryDefault_End,UI_APP_FactoryDefault_Key)
	DEF_TIM_EVE(ID_TIM_Default,UI_APP_FactoryDefault_Timer)

//信号检测(节目状态)
	DEF_DLG_EVE(ID_DLG_ProgState,UI_APP_ProgState_Enter,UI_APP_ProgState_End,UI_APP_ProgState_Key)
	DEF_TIM_EVE(ID_TIM_ProgState,UI_APP_ProgState_Timer)	

//音视频设置
	DEF_DLG_EVE(ID_DLG_AVSet,UI_APP_AvSet_Enter,UI_APP_AvSet_End,0)
	DEF_CBX_EVE(ID_CBX_Transparence_AvSet,UI_APP_AvSet_Key)
	DEF_CBX_EVE(ID_CBX_TVMode_AVSet,UI_APP_AvSet_Key)
	DEF_CBX_EVE(ID_CBX_DisplayFormat_AVSet,UI_APP_AvSet_Key)
	DEF_CBX_EVE(ID_CBX_TrackSet_AVSet,UI_APP_AvSet_Key)
	DEF_CBX_EVE(ID_CBX_VideoOut_AVSet,UI_APP_AvSet_Key)

//(机顶盒)密码设置
    DEF_DLG_EVE(ID_DLG_PasswordSet, UI_APP_PasswordSet_Enter,UI_APP_PasswordSet_End,0)
    DEF_TBX_EVE(ID_TBX_Old_PasswordSet, UI_APP_PasswordSet_Key)
	DEF_TBX_EVE(ID_TBX_New_PasswordSet, UI_APP_PasswordSet_Key)
	DEF_TBX_EVE(ID_TBX_Check_PasswordSet, UI_APP_PasswordSet_Key)

//语言设置
	DEF_DLG_EVE(ID_DLG_LanguageSet,UI_APP_LangSet_Enter,UI_APP_LangSet_End,0)
	DEF_CBX_EVE(ID_CBX_LangChoice_LanguageSet,UI_APP_LangSet_Key)

//互动设置
	DEF_DLG_EVE(ID_DLG_NetSet,UI_APP_NetSet_Enter,UI_APP_NetSet_End,0)
    DEF_CBX_EVE(ID_CBX_SwitchItem_NetSet,UI_APP_NetSet_Key)
    DEF_CBX_EVE(ID_CBX_AutoIP_NetSet,UI_APP_NetSet_Key)
    DEF_CBX_EVE(ID_CBX_AutoDNS_NetSet,UI_APP_NetSet_Key)

	DEF_ODC_EVE(ID_ODC_IPAddress_NetSet, UI_APP_IPAddress_OnPaint, UI_APP_ODCNetSet_Key)
	DEF_ODC_EVE(ID_ODC_Subnetwork_NetSet, UI_APP_Subnetwork_OnPaint, UI_APP_ODCNetSet_Key)
	DEF_ODC_EVE(ID_ODC_GatewayIP_NetSet, UI_APP_GatewayIP_OnPaint, UI_APP_ODCNetSet_Key)
	DEF_ODC_EVE(ID_ODC_FirstChoseDNS_NetSet, UI_APP_FirstChoseDNS_OnPaint, UI_APP_ODCNetSet_Key)
	DEF_ODC_EVE(ID_ODC_ReserveDNS_NetSet, UI_APP_ReserveDNS_OnPaint, UI_APP_ODCNetSet_Key)
	DEF_ODC_EVE(ID_ODC_FirstpageAddr_NetSet, UI_APP_FirstpageAddr_OnPaint, UI_APP_ODCNetSet_Key)
#if defined (CD_CA)
	DEF_STA_EVE(ID_STA_ipp_price_ChnInfo,UI_APP_FullScreen_ipp_price_Key)
	DEF_LBX_EVE(ID_TBX_ipp_pin_ChnInfo,UI_APP_FullScreen_ipp_price_Key)
//条件接收
	DEF_DLG_EVE(ID_DLG_Ca, UI_APP_CaMenu_Enter, UI_APP_CaMenu_End, 0)
	DEF_MNU_EVE(ID_MNU_Ca, UI_APP_CaMenu_Key)

//PIN检查提示
	DEF_DLG_EVE(ID_DLG_PinNote,0,0,UI_APP_CA_PinNote_Key)
	DEF_TIM_EVE(ID_TIM_PinNote, UI_APP_CA_PinNote_Timer)

//卡状态
	DEF_DLG_EVE(ID_DLG_CardState, 0, 0, UI_APP_CA_CardState_Key)
	DEF_TIM_EVE(ID_TIM_CardState, UI_APP_CA_CardState_Timer)

//成人级设置
	DEF_DLG_EVE(ID_DLG_Rating,UI_APP_CA_RateSet_Enter,UI_APP_CA_RateSet_End,0)
    DEF_TBX_EVE(ID_TBX_Value_Rating, UI_APP_CA_RateSet_Key)
	DEF_TBX_EVE(ID_TBX_Rating, UI_APP_CA_RateSet_Key)

//CA密码设置
	DEF_DLG_EVE(ID_DLG_PinSet,UI_APP_CA_PinSet_Enter,UI_APP_CA_PinSet_End,0)
    DEF_TBX_EVE(ID_TBX_Old_PinSet, UI_APP_CA_PinSet_Key)
	DEF_TBX_EVE(ID_TBX_New_PinSet, UI_APP_CA_PinSet_Key)
	DEF_TBX_EVE(ID_TBX_Check_PinSet, UI_APP_CA_PinSet_Key)
	
//设置工作时段
	DEF_DLG_EVE(ID_DLG_WorkTime,UI_APP_CA_WorkTimeSet_Enter,UI_APP_CA_WorkTimeSet_End,0)
    DEF_TBX_EVE(ID_TBX_PIN_WorkTime, UI_APP_CA_WorkTimeSet_Key)
	DEF_ODC_EVE(ID_ODC_StartTime_WorkTime, UI_APP_CA_WorkTimeSet_Start_OnPaint, UI_APP_CA_WorkTimeSet_Key)
	DEF_ODC_EVE(ID_ODC_EndTime_WorkTime, UI_APP_CA_WorkTimeSet_End_OnPaint, UI_APP_CA_WorkTimeSet_Key)

//机卡配对
    DEF_DLG_EVE(ID_DLG_CardPair, UI_APP_CA_CardPair_Enter,0,UI_APP_CA_CardPair_Key)

//邮件信息
	DEF_DLG_EVE(ID_DLG_Mail,UI_APP_CA_Mail_Enter,UI_APP_CA_Mail_End,0)
	DEF_LBP_EVE(ID_LBP_Mail, UI_APP_CA_Mail_Key, UI_APP_CA_Mail_Change)
	DEF_DLG_EVE(ID_DLG_ReadMail,UI_APP_CA_ReadMail_Enter,UI_APP_CA_ReadMail_End,0)
	DEF_EDT_EVE(ID_EDT_ReadMail, UI_APP_CA_ReadMail_Key)
	DEF_DLG_EVE(ID_DLG_DelCurrent,UI_APP_CA_DelMail_Enter,UI_APP_CA_DelMail_End,UI_APP_CA_DelMail_Key)

//智能卡基本信息
	DEF_DLG_EVE(ID_DLG_ICInfo,UI_APP_CA_CardInfo_Enter,0,UI_APP_CA_CardInfo_Key)

//获取所有运行商
	DEF_DLG_EVE(ID_DLG_GetServicer,UI_APP_CA_GetServicer_Enter,UI_APP_CA_GetServicer_End,0)
	DEF_LBX_EVE(ID_LBX_GetServicer, UI_APP_CA_GetServicer_Key)
	DEF_TIM_EVE(ID_TIM_GetServicer, UI_APP_CA_GetServicer_Timer)

//电子钱包信息
	DEF_DLG_EVE(ID_DLG_ePurse,UI_APP_CA_Slot_Enter,UI_APP_CA_Slot_End,UI_APP_CA_Slot_Key)

//查询授权信息
	DEF_DLG_EVE(ID_DLG_EntitleInfo,UI_APP_CA_EntitleInfo_Enter,UI_APP_CA_EntitleInfo_End,UI_APP_CA_EntitleInfo_Key)

//查询IPP信息
	DEF_DLG_EVE(ID_DLG_IPPInfo,UI_APP_CA_ViewedIPP_Enter,UI_APP_CA_ViewedIPP_End,UI_APP_CA_ViewedIPP_Key)


//子母卡信息
	DEF_DLG_EVE(ID_DLG_MasterSlave,UI_APP_CA_MasterSlave_Enter,0,UI_APP_CA_MasterSlave_Key)
	DEF_DLG_EVE(ID_DLG_MasterSlaveManage, UI_APP_CA_MasterSlaveManage_Enter, UI_APP_CA_MasterSlaveManage_End, 0)
	DEF_LBX_EVE(ID_LBX_MasterSlaveManage, UI_APP_CA_MasterSlaveManage_Key)
	//DEF_DLG_EVE(ID_TIM_RequestFeed,0, 0, UI_APP_CA_RequestFeed_Timer)

//用户特征
    DEF_DLG_EVE(ID_DLG_ACList,UI_APP_CA_acList_Enter,0,UI_APP_CA_acList_Key)

//反授权确认码
	DEF_DLG_EVE (ID_DLG_DetitleChkNums, DetitleChkNums_Enter, DetitleChkNums_End, 0)
	DEF_LBX_EVE (ID_LBX_DetitleChkNums, DetitleChkNums_Key)
	DEF_DLG_EVE(ID_DLG_DelDetitle,UI_APP_CA_DelDetitle_Enter,UI_APP_CA_DelDetitle_End,UI_APP_CA_DelDetitle_Key)

//拔卡
	DEF_DLG_EVE(ID_DLG_CardOut, UI_APP_CA_CardOut_Enter, 0, 0)
	DEF_ODC_EVE(ID_ODC_CardOut, UI_APP_CA_CardOut_OnPaint, 0)	
	DEF_TIM_EVE(ID_TIM_CardOut, UI_APP_CA_CardOut_Timer)

//OSD
	DEF_ODC_EVE(ID_ODC_CaOsd_ChInfo, UI_APP_CAOsd_ProgramInfo_Paint, 0)
	DEF_TIM_EVE(ID_TIM_CaOsd_ChInfo, UI_APP_CAOsd_ProgramInfo_Timer)

//
	DEF_TIM_EVE(ID_TIM_Feeding, UI_APP_Feeding_Timer)

#elif defined (DVT_CA)
//条件接收
	DEF_DLG_EVE(ID_DLG_Ca, UI_APP_CaMenu_Enter, UI_APP_CaMenu_End, 0)
	DEF_MNU_EVE(ID_MNU_Ca, UI_APP_CaMenu_Key)

//PIN检查提示
	DEF_DLG_EVE(ID_DLG_PinNote,0,0,UI_APP_CA_PinNote_Key)
	DEF_TIM_EVE(ID_TIM_PinNote, UI_APP_CA_PinNote_Timer)

//卡状态
	DEF_DLG_EVE(ID_DLG_CardState, 0, 0, UI_APP_CA_CardState_Key)
	DEF_TIM_EVE(ID_TIM_CardState, UI_APP_CA_CardState_Timer)

//成人级设置
	DEF_DLG_EVE(ID_DLG_Rating,UI_APP_CA_RateSet_Enter,UI_APP_CA_RateSet_End,0)
    DEF_TBX_EVE(ID_TBX_Value_Rating, UI_APP_CA_RateSet_Key)
	DEF_TBX_EVE(ID_TBX_Rating, UI_APP_CA_RateSet_Key)

//CA密码设置
	DEF_DLG_EVE(ID_DLG_PinSet,UI_APP_CA_PinSet_Enter,UI_APP_CA_PinSet_End,0)
    DEF_TBX_EVE(ID_TBX_Old_PinSet, UI_APP_CA_PinSet_Key)
	DEF_TBX_EVE(ID_TBX_New_PinSet, UI_APP_CA_PinSet_Key)
	DEF_TBX_EVE(ID_TBX_Check_PinSet, UI_APP_CA_PinSet_Key)
	
//设置工作时段
	DEF_DLG_EVE(ID_DLG_WorkTime,UI_APP_CA_WorkTimeSet_Enter,UI_APP_CA_WorkTimeSet_End,0)
    DEF_TBX_EVE(ID_TBX_PIN_WorkTime, UI_APP_CA_WorkTimeSet_Key)
	DEF_ODC_EVE(ID_ODC_StartTime_WorkTime, UI_APP_CA_WorkTimeSet_Start_OnPaint, UI_APP_CA_WorkTimeSet_Key)
	DEF_ODC_EVE(ID_ODC_EndTime_WorkTime, UI_APP_CA_WorkTimeSet_End_OnPaint, UI_APP_CA_WorkTimeSet_Key)

//邮件信息
	DEF_DLG_EVE(ID_DLG_Mail,UI_APP_CA_Mail_Enter,UI_APP_CA_Mail_End,0)
	DEF_LBP_EVE(ID_LBP_Mail, UI_APP_CA_Mail_Key, UI_APP_CA_Mail_Change)
	DEF_DLG_EVE(ID_DLG_ReadMail,UI_APP_CA_ReadMail_Enter,UI_APP_CA_ReadMail_End,0)
	DEF_EDT_EVE(ID_EDT_ReadMail, UI_APP_CA_ReadMail_Key)
	DEF_DLG_EVE(ID_DLG_DelCurrent,UI_APP_CA_DelMail_Enter,UI_APP_CA_DelMail_End,UI_APP_CA_DelMail_Key)

//智能卡基本信息
	DEF_DLG_EVE(ID_DLG_ICInfo,UI_APP_CA_CardInfo_Enter,0,UI_APP_CA_CardInfo_Key)

//获取所有运行商
	DEF_DLG_EVE(ID_DLG_GetServicer,UI_APP_CA_GetServicer_Enter,UI_APP_CA_GetServicer_End,0)
	DEF_LBX_EVE(ID_LBX_GetServicer, UI_APP_CA_GetServicer_Key)
	DEF_TIM_EVE(ID_TIM_GetServicer, UI_APP_CA_GetServicer_Timer)

//电子钱包信息
	DEF_DLG_EVE(ID_DLG_ePurse,UI_APP_CA_Slot_Enter,UI_APP_CA_Slot_End,UI_APP_CA_Slot_Key)

//查询授权信息
	DEF_DLG_EVE(ID_DLG_EntitleInfo,UI_APP_CA_EntitleInfo_Enter,UI_APP_CA_EntitleInfo_End,UI_APP_CA_EntitleInfo_Key)

//查询IPP信息
	DEF_DLG_EVE(ID_DLG_ViewedIPP,UI_APP_CA_ViewedIPP_Enter,UI_APP_CA_ViewedIPP_End,0)
	DEF_LBP_EVE(ID_LBP_ViewedIPP, UI_APP_CA_ViewedIPP_Key, UI_APP_CA_ViewedIPP_Change)

//子母卡信息
	DEF_DLG_EVE(ID_DLG_MasterSlave,UI_APP_CA_MasterSlave_Enter,0,UI_APP_CA_MasterSlave_Key)
	//DEF_DLG_EVE(ID_DLG_MasterSlaveManage, UI_APP_CA_MasterSlaveManage_Enter, UI_APP_CA_MasterSlaveManage_End, 0)
	//DEF_LBX_EVE(ID_LBX_MasterSlaveManage, UI_APP_CA_MasterSlaveManage_Key)
	//DEF_DLG_EVE(ID_TIM_RequestFeed,0, 0, UI_APP_CA_RequestFeed_Timer)
	DEF_DLG_EVE(ID_DLG_RequestFeed,UI_APP_CA_RequestFeed_Enter,0,UI_APP_CA_RequestFeed_Key)

//拔卡
	DEF_DLG_EVE(ID_DLG_CardOut, UI_APP_CA_CardOut_Enter, 0, 0)
	DEF_ODC_EVE(ID_ODC_CardOut, UI_APP_CA_CardOut_OnPaint, 0)	
	DEF_TIM_EVE(ID_TIM_CardOut, UI_APP_CA_CardOut_Timer)

//OSD
	DEF_ODC_EVE(ID_ODC_CaOsd_ChInfo, UI_APP_CAOsd_ProgramInfo_Paint, 0)
	DEF_TIM_EVE(ID_TIM_CaOsd_ChInfo, UI_APP_CAOsd_ProgramInfo_Timer)
	
//单频点
	DEF_DLG_EVE(ID_DLG_SingleSlot, UI_APP_AreaLock_Enter, 0, 0)
	DEF_TBX_EVE(ID_TBX_SingleSlot, UI_APP_AreaLock_Key)
	DEF_DLG_EVE(ID_DLG_AreaLockInfo, UI_APP_AreaLockInfo_Enter, 0, 0)

//订购/退订IPP节目
	DEF_DLG_EVE(ID_DLG_BookUnbook,UI_APP_CA_BookUnbookIPP_Enter,UI_APP_CA_BookUnbookIPP_End,0)
	DEF_LBP_EVE(ID_LBP_Ipp_BookUnbook, UI_APP_CA_BookUnbookIPP_Key, UI_APP_CA_BookUnbookIPP_Change)
	DEF_TBX_EVE(ID_TBX_BookUnbook, UI_APP_CA_BookUnbookIPP_TBXKey)
	DEF_DLG_EVE(ID_DLG_BookUnbookInfo,0,0,UI_APP_CA_BookUnbookInfo_Key)
	DEF_TIM_EVE(ID_TIM_BookUnbookInfo, UI_APP_CA_BookUnbookInfo_Timer)

//实时购买IPP节目
	DEF_DLG_EVE(ID_DLG_RealtimeBook,UI_APP_CA_RealTimeBook_Enter,UI_APP_CA_RealTimeBook_End,0)
	DEF_LBX_EVE(ID_LBX_RealtimeBook, UI_APP_CA_RealTimeBook_Key)
	DEF_TBX_EVE(ID_TBX_RealtimeBook, UI_APP_CA_RealTimeBook_PinKey)
	DEF_TBX_EVE(ID_TBX_Zhouqi_RealtimeBook, UI_APP_CA_RealTimeBook_ZhouqiKey)

//验证用户的PIN码
	DEF_DLG_EVE(ID_DLG_VerifyPin,UI_APP_CA_VerifyPin_Enter,UI_APP_CA_VerifyPin_End,0)
	DEF_TBX_EVE(ID_TBX_Pin_VerifyPin, UI_APP_CA_VerifyPin_Key)
#endif
	
#ifdef ENABLE_PVR
//播放器对话框
	DEF_DLG_EVE(ID_DLG_Player,UI_APP_Player_Enter,UI_APP_Player_End,UI_APP_Player_Key)
	DEF_LBP_EVE(ID_LBP_Player, UI_APP_PlayerFileList_OnKey, UI_APP_PlayerFileList_Change)
	DEF_LBP_EVE(ID_LBP_DisplaySet_Player, UI_APP_PlayerDisplay_OnKey,UI_APP_PlayerDisplay_Change)
    DEF_ODC_EVE(ID_ODC_CurTime_Player, UI_APP_CurTime_Player_OnPaint, 0)
    DEF_TIM_EVE(ID_TIM_Player, UI_APP_Player_Timer)
#endif

#ifdef ENABLE_JPEG
//图片浏览状态
	DEF_DLG_EVE(ID_DLG_PictureShow,UI_APP_PictureShow_Enter,UI_APP_PictureShow_End,UI_APP_PictureShow_Key)
	DEF_TIM_EVE(ID_TIM_PictureShow, UI_APP_PictureShow_Timer)
//	DEF_ODC_EVE(ID_ODC_Frame_PictureShow,UI_APP_PictureShow_OnPaint,UI_APP_PictureShow_Onkey)
//数码相框
	DEF_DLG_EVE(ID_DLG_PhotoFrame,UI_APP_PhotoFrame_Enter,UI_APP_PhotoFrame_End,UI_APP_PhotoFrame_Key)
	DEF_CBX_EVE(ID_CBX_Order_PhotoFrame,UI_APP_PhotoShowSet_Key)
	DEF_CBX_EVE(ID_CBX_Effect_PhotoFrame,UI_APP_PhotoShowSet_Key)
	DEF_CBX_EVE(ID_CBX_Seconds_PhotoFrame,UI_APP_PhotoShowSet_Key)	
	DEF_CBX_EVE(ID_CBX_FullScreen_PhotoFrame,UI_APP_PhotoShowSet_Key)
	DEF_ODC_EVE(ID_ODC_Frame_PhotoFrame,UI_APP_Frame_OnPaint,UI_APP_Frame_Onkey)
#endif

#ifdef ENABLE_USB
//特效菜单
	DEF_DLG_EVE(ID_DLG_Effects, UI_APP_EffectsMenu_Enter, 0, UI_APP_EffectsMenu_Key)
	DEF_MNU_EVE(ID_MNU_Effects, UI_APP_EffectsMenu_Key)
#endif
//特效提示框
	DEF_DLG_EVE(ID_DLG_EffectsInfo, UI_APP_EffectsInfo_Enter, UI_APP_EffectsInfo_End, UI_APP_EffectsInfo_Key)
	DEF_TIM_EVE(ID_TIM_EffectsInfo, UI_APP_EffectsInfo_Timer)

#ifdef ENABLE_PVR

//手动选时录制
	DEF_DLG_EVE(ID_DLG_ManualRec, UI_APP_ManualRec_Enter, UI_APP_ManualRec_End, UI_APP_ManualRec_Key)
	DEF_ODC_EVE(ID_ODC_StartTime_ManualRec, UI_APP_ManualRec_StartTime_OnPaint, UI_APP_ManualRec_Key)
	DEF_ODC_EVE(ID_ODC_EndTime_ManualRec, UI_APP_ManualRec_EndTime_OnPaint, UI_APP_ManualRec_Key)
	DEF_LBP_EVE(ID_LBP_Prog_ManualRec, UI_APP_ManualRec_Key, UI_APP_ManualRec_Prog_Change)
	DEF_LBP_EVE(ID_LBP_Week_ManualRec, UI_APP_ManualRec_Key, UI_APP_ManualRec_Week_Change)
//时移
	DEF_DLG_EVE(ID_DLG_TimeShift, UI_APP_TimeShift_Enter, UI_APP_TimeShift_End, UI_APP_TimeShift_key)
	DEF_ODC_EVE(ID_ODC_NowTime_TimeShift, UI_APP_NowTime_TimeShift_OnPaint, UI_APP_ODCNowTime_TimeShift_key)
    DEF_ODC_EVE(ID_ODC_JumpTO_TimeShift, UI_APP_JumpTO_TimeShift_OnPaint, UI_APP_ODCJumpTO_TimeShift_key)
    DEF_TIM_EVE(ID_TIM_TimeShift, UI_APP_TimeShift_Timer)
    DEF_TIM_EVE(ID_TIM_BrushScreen_TimeShift,UI_APP_BrushScreen_TimeShift_Timer)
#endif

//保存新的IP设置
	DEF_DLG_EVE(ID_DLG_Preserve_NetSet,UI_APP_PreserveNetset_Enter,UI_APP_PreserveNetset_End,UI_APP_PreserveNetset_Key)

//保存IP设置信息提示
	DEF_DLG_EVE(ID_DLG_Prompt_NetSet,UI_APP_PromptNetSet_Enter,UI_APP_PromptNetSet_End,0)
	DEF_TIM_EVE(ID_TIM_Prompt, UI_APP_PromptNetSet_Timer)

//Nvod影视点播
#ifdef ENABLE_NVOD
	DEF_DLG_EVE(ID_DLG_Nvod, UI_APP_Nvod_Enter, UI_APP_Nvod_End, UI_APP_Nvod_Key)
	DEF_LBP_EVE(ID_LBP_EventList_Nvod, UI_APP_Nvod_EventList_Key, UI_APP_Nvod_EventList_Change)
	DEF_LBP_EVE(ID_LBP_EventPlayTime_Nvod, UI_APP_Nvod_EventPlayTime_Key, UI_APP_Nvod_EventPlayTime_Change)
	DEF_TIM_EVE(ID_TIM_Nvod, UI_APP_Nvod_Timer)

//Nvod_Play
	DEF_DLG_EVE(ID_DLG_NvodPlay, UI_APP_NvodPlay_Enter,UI_APP_NvodPlay_End,UI_APP_NvodPlay_Key)
	DEF_TIM_EVE(ID_TIM_NvodPlay, UI_APP_NvodPlay_Timer)

//NvodPlay全屏播放退出提示框
	DEF_DLG_EVE(ID_DLG_NvodPlayInfo, UI_APP_NvodPlayInfo_Enter,UI_APP_NvodPlayInfo_End,UI_APP_NvodPlayInfo_Key)

//进入Nvod时等待界面
	DEF_DLG_EVE(ID_DLG_NvodWait, UI_APP_NvodWait_Enter,UI_APP_NvodWait_End,UI_APP_NvodWait_Key)
	DEF_TIM_EVE(ID_TIM_NvodWait, UI_APP_NvodWait_Timer)
#endif

#ifdef ENABLE_USB
//USB文件管理
	DEF_DLG_EVE(ID_DLG_UsbManager, UI_APP_UsbManager_Enter, UI_APP_UsbManager_End, 0)
	DEF_LBP_EVE(ID_LBP_FileList_UsbManager, UI_APP_UsbManager_LbpKey, UI_APP_UsbManager_Change)
#endif

#ifdef ENABLE_PVR
//Pvr播放列表
	DEF_DLG_EVE(ID_DLG_PvrList, UI_APP_PvrList_Enter, UI_APP_PvrList_End, 0)
	DEF_LBP_EVE(ID_LBP_FileList_PvrList, UI_APP_PvrList_LbpKey, UI_APP_PvrList_Change)

//Pvr全屏播放
    DEF_DLG_EVE(ID_DLG_PvrFullScreen, UI_APP_PvrFullScreen_Enter, UI_APP_PvrFullScreen_End, UI_APP_PvrFullScreen_Key)
    DEF_TIM_EVE(ID_TIM_PvrFullScreen, UI_APP_PvrFullScreen_Timer)
#endif

//工厂菜单
	DEF_DLG_EVE(ID_DLG_FacMenu, UI_APP_FactoryMenu_Enter, UI_APP_FactoryMenu_End, 0)
	DEF_MNU_EVE(ID_MNU_FacMenu, UI_APP_FactoryMenu_Key)

//回看跳转
	DEF_DLG_EVE(ID_DLG_FULLTOTVOD, UI_APP_FULLTOTVOD_Enter, UI_APP_FULLTOTVOD_End, 0)

#ifdef USE_ENREACH //if use the ENREACH VOD
//	DEF_ODC_EVE(ID_ODC_BPJUMP, UI_APP_BPJUMP_OnPaint, UI_APP_FULLTOTVOD_key)
	DEF_ODC_EVE(ID_ODC_BPZHUN, UI_APP_BPZHUN_OnPaint, UI_APP_BPZHUN_ODC_Key)
	DEF_ODC_EVE(ID_ODC_FAST, UI_APP_FAST_OnPaint, UI_APP_FULLTOTVOD_key)
//	DEF_ODC_EVE(ID_ODC_JUMPTO, UI_APP_JUMPTO_OnPaint, UI_APP_FULLTOTVOD_key)
	DEF_ODC_EVE(ID_ODC_JUMPBAR, UI_APP_JUMPBAR_OnPaint, UI_APP_JUMPBAR_key)
	DEF_TIM_EVE(ID_TIM_BPZHUN, UI_APP_BPZHUN_TIME)
	DEF_TIM_EVE(ID_TIM_CLEANSCREEN, UI_APP_TVOD_CLEANSCREEN_TIME)
	DEF_TIM_EVE(ID_TIM_TVOD, UI_APP_FULLTOTVOD_Timer)
	DEF_TIM_EVE(ID_TIM_SignalCheck_TVOD, UI_APP_FULLTOTVOD_SignalCheck_Timer)
#endif
#if 0
//VOD
	DEF_DLG_EVE(ID_DLG_VOD, UI_APP_VOD_Enter, UI_APP_VOD_End, UI_APP_VOD_key)
	DEF_ODC_EVE(ID_ODC_VOD_JUMPTO, UI_APP_VOD_JUMPTO_OnPaint, UI_APP_VOD_key)
	DEF_ODC_EVE(ID_ODC_VOD_JUMPBAR, UI_APP_VOD_JUMPBAR_OnPaint, UI_APP_VOD_key)	
	DEF_TIM_EVE(ID_TIM_VOD, UI_APP_VOD_TIME)

//CONNECT
	DEF_DLG_EVE(ID_DLG_INFOCONNECT, UI_APP_INFOCONNECT_Enter, UI_APP_INFOCONNECT_End, 0)
	DEF_TIM_EVE(ID_TIM_INFO_CONNECT,UI_APP_INFOCONNECT_TIME)
#endif
//null
	//DEF_TIM_EVE(ID_TIM_NULL, UI_APP_NULL_TIME)

//双向开机
//	DEF_DLG_EVE(ID_DLG_VOD_FIRST_STATE, UI_APP_VOD_FIRST_STATE_Enter, UI_APP_VOD_FIRST_STATE_End, 0)
//	DEF_TIM_EVE(ID_TIM_VOD_FIRST, UI_APP_VOD_FIRST_STATE_Timer)
//区域ID设置

	DEF_DLG_EVE(ID_DLG_RegionSet, UI_APP_RegionSet_Enter, UI_APP_RegionSet_End, 0)
	DEF_CBX_EVE(ID_CBX_AutoRegion_RegionSet, UI_APP_RegionSet_CBXKey)
	DEF_TBX_EVE(ID_TBX__RegionID_RegionSet, UI_APP_RegionSet_CBXKey)

//stbid setting 
	DEF_DLG_EVE(ID_DLG_StbIDSetting, UI_APP_StbIDSetting_Enter, UI_APP_StbIDSetting_End, UI_APP_StbIDSetting_Key)
	DEF_TIM_EVE(ID_TIM_StbIDSetting, UI_APP_StbIDSetting_Timer)

//使用操作指南
    DEF_DLG_EVE(ID_DLG_OperationGuide, UI_APP_OperationGuide_Enter, 0, 0)
    DEF_ODC_EVE(ID_ODC_OperationGuide, UI_APP_OperationGuide_OnPaint, UI_APP_OperationGuide_Key)

//输出设置
    DEF_DLG_EVE(ID_DLG_OutPutSet, UI_APP_OutPutSet_Enter,UI_APP_OutPutSet_End, 0)
    DEF_CBX_EVE(ID_CBX_TVMode_OutPutSet,UI_APP_OutPutSet_Key)
    DEF_CBX_EVE(ID_CBX_VideoOut_OutPutSet,UI_APP_OutPutSet_Key)

//显示设置
    DEF_DLG_EVE(ID_DLG_DisplaySet, UI_APP_DisplaySet_Enter,UI_APP_DisplaySet_End, 0)
    DEF_CBX_EVE(ID_CBX_TVMode_DisplaySet,UI_APP_DisplaySet_Key)
    DEF_CBX_EVE(ID_CBX_VideoOut_DisplaySet,UI_APP_DisplaySet_Key)
    DEF_CBX_EVE(ID_CBX_Trans_DisplaySet,UI_APP_DisplaySet_Key)
    DEF_CBX_EVE(ID_CBX_DisplayScale_DisplaySet,UI_APP_DisplaySet_Key)
    DEF_CBX_EVE(ID_CBX_InfoTime_DisplaySet,UI_APP_DisplaySet_Key)
    DEF_CBX_EVE(ID_CBX_VolumeSave_DisplaySet,UI_APP_DisplaySet_Key)

//频道设置
    DEF_DLG_EVE(ID_DLG_ChannelSet, UI_APP_ChannelSet_Enter,UI_APP_ChannelSet_End, 0)
    DEF_CBX_EVE(ID_CBX_Track_ChannelSet,UI_APP_ChannelSet_Key)
    DEF_CBX_EVE(ID_CBX_OffSet_ChannelSet,UI_APP_ChannelSet_Key)
    DEF_CBX_EVE(ID_CBX_ChannelSkip_ChannelSet,UI_APP_ChannelSet_Key)
    DEF_CBX_EVE(ID_CBX_ChannelFav_ChannelSet,UI_APP_ChannelSet_Key)

//频道设置保存提示框
    DEF_DLG_EVE(ID_DLG_ChannelSetInfo, UI_APP_ChannelSetInfo_Enter,0, UI_APP_ChannelSetInfo_Key)
    DEF_TIM_EVE(ID_TIM_ChannelSetInfo,UI_APP_ChannelSetInfo_Timer)

//父母控制
    DEF_DLG_EVE(ID_DLG_ParentControl, UI_APP_ParentControl_Enter,UI_APP_ParentControl_End, 0)
    DEF_CBX_EVE(ID_CBX_MenuLockSet_ParentControl,UI_APP_ParentControl_Key)
    DEF_CBX_EVE(ID_CBX_ChannelLockSet_ParentControl,UI_APP_ParentControl_Key)
    DEF_CBX_EVE(ID_CBX_BootLockSet_ParentControl,UI_APP_ParentControl_Key)

//系统软件升级

	DEF_DLG_EVE(ID_DLG_SysUpdate, UI_APP_SysUpgrade_Enter, 0, UI_APP_SysUpgrade_Key)
/***************************** 工厂菜单************************************/
//频点设置
    DEF_DLG_EVE(ID_DLG_FreqSet, UI_APP_FreqSet_Enter, 0, UI_APP_FreqSet_Key)
    DEF_TBX_EVE(ID_TBX_MainFreq_FreqSet,UI_APP_FreqSet_Key)
    DEF_TBX_EVE(ID_TBX_AdFreq_FreqSet,UI_APP_FreqSet_Key)
    DEF_TBX_EVE(ID_TBX_NvodFreq_FreqSet,UI_APP_FreqSet_Key)
    DEF_TBX_EVE(ID_TBX_BroadFreq_FreqSet,UI_APP_FreqSet_Key)
    DEF_TBX_EVE(ID_TBX_StockFreq_FreqSet,UI_APP_FreqSet_Key)

//序列号
    DEF_DLG_EVE(ID_DLG_Sequence, UI_APP_Sequence_Enter, 0, UI_APP_Sequence_Key)
    DEF_TBX_EVE(ID_TBX_Manufacture_Sequence, UI_APP_Sequence_Key)
    DEF_TBX_EVE(ID_TBX_StbType_Sequence, UI_APP_Sequence_Key)
    DEF_TBX_EVE(ID_TBX_Productyear_Sequence, UI_APP_Sequence_Key)
    DEF_TBX_EVE(ID_TBX_Productweek_Sequence, UI_APP_Sequence_Key)
    DEF_TBX_EVE(ID_TBX_Sequence_Sequence, UI_APP_Sequence_Key)

//空中升级
    DEF_DLG_EVE(ID_DLG_OTA, UI_APP_OTA_Enter, 0, UI_APP_OTA_Key)
    DEF_TBX_EVE(ID_TBX_PID_OTA, UI_APP_OTA_Key)
    DEF_TBX_EVE(ID_TBX_Freq_OTA, UI_APP_OTA_Key)
    DEF_TBX_EVE(ID_TBX_Symb_OTA, UI_APP_OTA_Key)
    DEF_CBX_EVE(ID_CBX_QAMMode_OTA,UI_APP_OTA_Key)
    DEF_TIM_EVE(ID_TIM_OTA,UI_APP_OTA_Timer)

//串口写序列号
    DEF_DLG_EVE(ID_DLG_ComSequence, UI_APP_ComSn_Enter, UI_APP_ComSn_End, 0)
    DEF_CBX_EVE(ID_CBX_DetectCard_ComSequence, UI_APP_ComSn_Smc_Key)
    DEF_CBX_EVE(ID_CBX_DetectE2_ComSequence, UI_APP_ComSn_E2_Key)
    DEF_CBX_EVE(ID_CBX_DetectCom_ComSequence, UI_APP_ComSn_Com_Key)
    DEF_CBX_EVE(ID_CBX_EthSet_ComSequence, UI_APP_ComSn_NetSet_Key)
    DEF_TIM_EVE(ID_TIM_ComSequence, UI_APP_ComSn_Timer)

//互动帮助
    DEF_DLG_EVE(ID_DLG_DoubleHelp,UI_APP_DoubleHelpInfo_Enter,UI_APP_DoubleHelpInfo_End,UI_APP_DoubleHelpInfo_Key)
   
 	DEF_DLG_EVE(ID_DLG_EPGDetail,UI_APP_EPGDetail_Enter,UI_APP_EPGDetail_End,UI_APP_EPGDetail_Key) 
};
BU32 eventTableNum = sizeof(eventTable)/sizeof(GUI_EventTable_s);

