#ifndef __UI_APP_API__
#define __UI_APP_API__

#include "../../../tpmiddle/include/app_common.h"

//主菜单
void UI_APP_MainMenu_Enter(void);
void UI_APP_MainMenu_End(void);
void UI_APP_MainMenu_Key(BU32 key);
void UI_APP_MainMenu_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_MainMenu_OnKey(BU32 key);
void UI_APP_MainMenu_Timer(void);

//空对话框
void UI_APP_Null_Enter(void);
void UI_APP_Null_End(void);
void UI_APP_Null_Key(void);

//无节目自动搜索提示框
void UI_APP_AutoInfo_Enter(void);
void UI_APP_AutoInfo_Key(BU32 key);
void UI_APP_AutoInfo_Timer(void);

//节目管理
void UI_APP_ProgManagerMenu_Enter(void);
void UI_APP_ProgManagerMenu_Key(BU32 Key);

//节目列表
void UI_APP_ChnList_Enter(void);
void UI_APP_ChnList_End(void);
void UI_APP_ChnList_Key(BU32 key);
void UI_APP_ChnList_Change(void);
void UI_APP_ChnList_Timer(void);

//系统提示
void UI_APP_SysInfo_Enter(void);
void UI_APP_SysInfo_End(void);
void UI_APP_SysInfo_Key(BU32 key);
void UI_APP_SysInfo_Timer(void);

//全屏节目信息
void UI_APP_FullScreen_Enter(void);
void UI_APP_FullScreen_End(void);
void UI_APP_FullScreen_Key(BU32 key);
void UI_APP_FullScreen_TrackOnPaint(BU32 hdc,BU32 lparam);
void UI_APP_FullScreen_TrackKey(BU32 key);
void UI_APP_FullScreen_OffSetPaint(BU32 hdc,BU32 lparam);
void UI_APP_FullScreen_TrackSetPaint(BU32 hdc, BU32 lparam);
void UI_APP_FullScreen_Timer(void);
void UI_APP_Fullscreen_RenewEvent_Timer(void);
void UI_APP_NetState_Timer(void);
void UI_APP_FullScreen_ADVOnPaint(BU32 hdc,BU32 lparam);

//节目分类
void UI_APP_ChnSort_Enter(void);
void UI_APP_ChnSort_End(void);
void UI_APP_ChnSort_Key(BU32 key);
void UI_APP_ChnSort_Change(void);
void UI_APP_ChnSort_Timer(void);

//搜索
void UI_APP_Search_Enter(void);
void UI_APP_Search_End(void);
void UI_APP_Search_Key(BU32 key);

//搜索结束提示
void UI_APP_SearchInfo_Enter(void);
void UI_APP_SearchInfo_End(void);
void UI_APP_SearchInfo_Timer(void);
void UI_APP_SearchInfo_Key(BU32 key);

//搜索设置
void UI_APP_ManualSearchSet_Enter(void);
void UI_APP_ManualSearchSet_End(void);
void UI_APP_ManualSearchSet_Key(BU32 key);
void UI_APP_ManualSearchSet_Timer(void);

//菜单加锁
void UI_APP_MenuLock_Enter(void);
void UI_APP_MenuLock_End(void);
void UI_APP_MenuLock_Key(BU32 key);
void UI_APP_MenuLock_Timer(void);
void UI_APP_MenuLock_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_MenuLock_Onkey(BU32 key);

//节目加锁
void UI_APP_ChnLock_Enter(void);
void UI_APP_ChnLock_Key(BU32 Key);
void UI_APP_ChnLock_End(void);
void UI_APP_ChnLock_Timer(void);
void UI_APP_ChnLock_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_ChnLock_Onkey(BU32 key);

//开机锁
void UI_APP_StartUpLock_Enter(void);
void UI_APP_StartUpLock_Key(BU32 Key);
void UI_APP_StartUpLock_End(void);
void UI_APP_StartUpLock_Timer(void);
void UI_APP_StartUpLock_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_StartUpLock_Onkey(BU32 key);

//是否一次性解锁提示框
void UI_APP_ChUnLock_Key(BU32 key);

//主频点设置
void UI_APP_MainFreqSet_Enter(void);
void UI_APP_MainFreqSet_Key(BU32 key);

//频道编辑
void UI_APP_ChnEdit_Enter(void);
void UI_APP_ChnEdit_End(void);
void UI_APP_ChnEdit_Key(BU32 key);
void UI_APP_ChnEdit_Change(void);
void UI_APP_ChnEdit_Timer(void);

//频道编辑退出

void UI_APP_EditInfo_Enter(void);
void UI_APP_EditInfo_End(void);
void UI_APP_EditInfo_Key(BU32 key);
void UI_APP_EditInfo_Timer(void);

//数据广播列表
void UI_APP_BrdcastList_Enter(void);
void UI_APP_BrdcastList_End(void);
void UI_APP_BrdcastList_Key(BU32 key);
void UI_APP_BrdcastList_Change(void);

//电视指南Epg
void UI_APP_EPG_Enter(void);
void UI_APP_EPG_End(void);
void UI_APP_EPG_Prog_Key(BU32 key);
void UI_APP_EPG_Prog_Change(void);
void UI_APP_EPG_Event_Key(BU32 key);
void UI_APP_EPG_Event_Change(void);
void UI_APP_EPG_Timer(void);
void UI_APP_EPG_Detail_Key(BU32 key);

//Epg数据保存提示框
void UI_APP_EpgInfo_Timer(void);

//预约节目的冲突提示框
void UI_APP_ConflictInfo_Enter(void);
void UI_APP_ConflictInfo_End(void);
void UI_APP_ConflictInfo_Key(BU32 key);

//Epg及不能预约的提示，如:节目正在播放，节目也播放完，预约列表已满
void UI_APP_EpgReserveInfo_Enter(void);
void UI_APP_EpgReserveInfo_Key(BU32 key);
void UI_APP_EpgReserveInfo_Timer(void);

//预约节目列表
void UI_APP_ReserveList_Enter(void);
void UI_APP_ReserveList_End(void);
void UI_APP_ReserveList_Key(BU32 key);

//预约列表管理提示框
void UI_APP_ReserveInfo_Enter(void);
void UI_APP_ReserveInfo_End(void);
void UI_APP_ReserveInfo_Key(BU32 key);

//预约节目播放提示框
void UI_APP_TimeOut_Callback(BU32 callback_type, Epg_Info_S *EpgInfo);
void UI_APP_TimeOut_Enter(void);
void UI_APP_TimeOut_End(void);
void UI_APP_TimeOut_OnPaint(BU32 hdc,BU32 lpara);
void UI_APP_TimeOut_Key(BU32 key);
void UI_APP_TimeOut_Timer(void);

//游戏天地
void UI_APP_Game_Enter(BU32 key);
void UI_APP_Game_End(BU32 key);
void UI_APP_Game_Key(BU32 key);

void UI_APP_Game_Russia_Enter(void);
void UI_APP_Game_Russia_End(void);
void UI_APP_Game_Snake_Enter(void);
void UI_APP_Game_Snake_End(void);
void UI_APP_Game_Box_Enter(void);

//俄罗斯方块

void Game_Russia_OnPaint(BU32 hdc,BU32 lparam);
void Game_Russia_OnKey(BU32 key);
void Game_Russia_Timing(void);

//贪吃蛇
void Game_Snake_OnPaint(BU32 hdc,BU32 lparam);
void Game_Snake_OnKey(BU32 KeyValue) ;
void Game_Snake_Timing(void);

//推箱子
void Game_Box_OnKey(BU32 key);
void Game_Box_OnPaint(BU32 hdc,BU32 lparam);

//扫雷
void Game_Landmine_OnKey(BU32 key);
void Game_Landmine_OnPaint(BU32 hdc,BU32 lparam);


//系统设置菜单
void UI_APP_SysSetMenu_Enter(void);
void UI_APP_SysSetMenu_End(void);
void UI_APP_SysSetMenu_Key(BU32 key);

//时区设置
void UI_APP_TimeZoneSet_Enter(void);
void UI_APP_TimeZoneSet_End(void);
void UI_APP_TimeZoneSet_Key(BU32 key);

//机顶盒信息
void UI_APP_STBInfo_Enter(void);
void UI_APP_STBInfo_Key(BU32 key);

//信号检测
void UI_APP_SignalDectec_Enter(void);
void UI_APP_SignalDectec_End(void);
void UI_APP_SignalDectec_Key(BU32 key);
void UI_APP_SignalDectec_Change(void);
void UI_APP_SignalDectec_Timer(void);

//恢复出厂设置
void UI_APP_FactoryDefault_Enter(void);
void UI_APP_FactoryDefault_End(void);
void UI_APP_FactoryDefault_Key(BU32 key);
void UI_APP_FactoryDefault_Timer(void);

//信息检测(节目状态)
void UI_APP_ProgState_Enter(void);
void UI_APP_ProgState_End(void);
void UI_APP_ProgState_Key(BU32 key);
void UI_APP_ProgState_Timer(void);

//音视频设置
void UI_APP_AvSet_Enter(void);
void UI_APP_AvSet_End(void);
void UI_APP_AvSet_Key(BU32 key);

//机顶盒密码设置
void UI_APP_PasswordSet_Enter(void);
void UI_APP_PasswordSet_End(void);
void UI_APP_PasswordSet_Key(BU32 key);

//语言设置
void UI_APP_LangSet_Enter(void);
void UI_APP_LangSet_End(void);
void UI_APP_LangSet_Key(BU32 key);

//互动设置
void UI_APP_NetSet_Enter(void);
void UI_APP_NetSet_End(void);
void UI_APP_NetSet_Key(BU32 key);
void UI_APP_IPAddress_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_Subnetwork_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_GatewayIP_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_FirstChoseDNS_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_ReserveDNS_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_FirstpageAddr_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_ODCNetSet_Key(BU32 key);


#if defined (CD_CA)
void UI_APP_FullScreen_ipp_price_Key(BU32 key);
//条件接收
void UI_APP_CaMenu_Enter(void);
void UI_APP_CaMenu_End(void);
void UI_APP_CaMenu_Key(BU32 key);

//pin输入正确/错误提示
void UI_APP_CA_PinNote_Key(BU32 key);
void UI_APP_CA_PinNote_Timer(void);

//卡状态
void UI_APP_CA_CardState_Enter(void);
void UI_APP_CA_CardState_End(void);
void UI_APP_CA_CardState_Key(BU32 key);
void UI_APP_CA_CardState_Timer(void);

//成人级设置
void UI_APP_CA_RateSet_Enter(void);
void UI_APP_CA_RateSet_End(void);
void UI_APP_CA_RateSet_Key(BU32 key);

//PIN修改
void UI_APP_CA_PinSet_Enter(void);
void UI_APP_CA_PinSet_End(void);
void UI_APP_CA_PinSet_Key(BU32 key);

//工作时段设置
void UI_APP_CA_WorkTimeSet_Enter(void);
void UI_APP_CA_WorkTimeSet_End(void);
void UI_APP_CA_WorkTimeSet_Key(BU32 key);
void UI_APP_CA_WorkTimeSet_Start_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_CA_WorkTimeSet_End_OnPaint(BU32 hdc,BU32 lParam);

//机卡配对
void UI_APP_CA_CardPair_Enter(void);
void UI_APP_CA_CardPair_End(void);
void UI_APP_CA_CardPair_Key(BU32 key);
void UI_APP_CA_CardPair_Timer(void);

//邮件信息
void UI_APP_CA_Mail_Enter(void);
void UI_APP_CA_Mail_End(void);
void UI_APP_CA_Mail_Key(BU32 key);
void UI_APP_CA_Mail_Change(void);
void UI_APP_CA_ReadMail_Enter(void);
void UI_APP_CA_ReadMail_End(void);
void UI_APP_CA_ReadMail_Key(BU32 key);
void UI_APP_CA_DelMail_Enter(void);
void UI_APP_CA_DelMail_End(void);
void UI_APP_CA_DelMail_Key(BU32 key);

//智能卡信息
void UI_APP_CA_CardInfo_Enter(void);
void UI_APP_CA_CardInfo_End(void);
void UI_APP_CA_CardInfo_Key(BU32 key);

//获取所有运行商
void UI_APP_CA_GetServicer_Enter(void);
void UI_APP_CA_GetServicer_End(void);
void UI_APP_CA_GetServicer_Key(BU32 key);
void UI_APP_CA_GetServicer_Timer(void);

//电子钱包信息
void UI_APP_CA_Slot_Enter(void);
void UI_APP_CA_Slot_End(void);
void UI_APP_CA_Slot_Key(BU32 key);

//查询授权信息
void UI_APP_CA_EntitleInfo_Enter(void);
void UI_APP_CA_EntitleInfo_End(void);
void UI_APP_CA_EntitleInfo_Key(BU32 key);

//查询IPP信息
void UI_APP_CA_ViewedIPP_Enter(void);
void UI_APP_CA_ViewedIPP_End(void);
void UI_APP_CA_ViewedIPP_Key(BU32 key);

//子母卡
void UI_APP_CA_MasterSlave_Enter(void);
void UI_APP_CA_MasterSlave_End(void);
void UI_APP_CA_MasterSlave_Key(BU32 key);

void UI_APP_CA_MasterSlaveManage_Enter(void);
void UI_APP_CA_MasterSlaveManage_End(void);
void UI_APP_CA_MasterSlaveManage_Key(BU32 key);
//void UI_APP_CA_RequestFeed_Timer(void);

//用户特征
void UI_APP_CA_acList_Enter(void);
void UI_APP_CA_acList_Key(BU32 key);

//反授权确认码
void DetitleChkNums_Enter(void);
void DetitleChkNums_End(void);
void DetitleChkNums_Key(BU32 key);
void UI_APP_CA_DelDetitle_Enter(void);
void UI_APP_CA_DelDetitle_End(void);
void UI_APP_CA_DelDetitle_Key(BU32 key);

//拔卡
void UI_APP_CA_CardOut_Enter(void);
void UI_APP_CA_CardOut_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_CA_CardOut_Timer(void);

//OSD
void UI_APP_CAOsd_ProgramInfo_Timer(void);
void UI_APP_CAOsd_ProgramInfo_Paint(BU32 hdc,BU32 lParam);

//
void UI_APP_Feeding_Timer(void);

#elif defined (DVT_CA)
//条件接收
void UI_APP_CaMenu_Enter(void);
void UI_APP_CaMenu_End(void);
void UI_APP_CaMenu_Key(BU32 key);

//pin输入正确/错误提示
void UI_APP_CA_PinNote_Key(BU32 key);
void UI_APP_CA_PinNote_Timer(void);

//卡状态
void UI_APP_CA_CardState_Enter(void);
void UI_APP_CA_CardState_End(void);
void UI_APP_CA_CardState_Key(BU32 key);
void UI_APP_CA_CardState_Timer(void);

//成人级设置
void UI_APP_CA_RateSet_Enter(void);
void UI_APP_CA_RateSet_End(void);
void UI_APP_CA_RateSet_Key(BU32 key);

//PIN修改
void UI_APP_CA_PinSet_Enter(void);
void UI_APP_CA_PinSet_End(void);
void UI_APP_CA_PinSet_Key(BU32 key);

//工作时段设置
void UI_APP_CA_WorkTimeSet_Enter(void);
void UI_APP_CA_WorkTimeSet_End(void);
void UI_APP_CA_WorkTimeSet_Key(BU32 key);
void UI_APP_CA_WorkTimeSet_Start_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_CA_WorkTimeSet_End_OnPaint(BU32 hdc,BU32 lParam);

//邮件信息
void UI_APP_CA_Mail_Enter(void);
void UI_APP_CA_Mail_End(void);
void UI_APP_CA_Mail_Key(BU32 key);
void UI_APP_CA_Mail_Change(void);
void UI_APP_CA_ReadMail_Enter(void);
void UI_APP_CA_ReadMail_End(void);
void UI_APP_CA_ReadMail_Key(BU32 key);
void UI_APP_CA_DelMail_Enter(void);
void UI_APP_CA_DelMail_End(void);
void UI_APP_CA_DelMail_Key(BU32 key);

//智能卡信息
void UI_APP_CA_CardInfo_Enter(void);
void UI_APP_CA_CardInfo_End(void);
void UI_APP_CA_CardInfo_Key(BU32 key);

//获取所有运行商
void UI_APP_CA_GetServicer_Enter(void);
void UI_APP_CA_GetServicer_End(void);
void UI_APP_CA_GetServicer_Key(BU32 key);
void UI_APP_CA_GetServicer_Timer(void);

//电子钱包信息
void UI_APP_CA_Slot_Enter(void);
void UI_APP_CA_Slot_End(void);
void UI_APP_CA_Slot_Key(BU32 key);

//查询授权信息
void UI_APP_CA_EntitleInfo_Enter(void);
void UI_APP_CA_EntitleInfo_End(void);
void UI_APP_CA_EntitleInfo_Key(BU32 key);

//查询IPP信息
void UI_APP_CA_ViewedIPP_Enter(void);
void UI_APP_CA_ViewedIPP_End(void);
void UI_APP_CA_ViewedIPP_Key(BU32 key);
void UI_APP_CA_ViewedIPP_Change(void);

//子母卡
void UI_APP_CA_MasterSlave_Enter(void);
void UI_APP_CA_MasterSlave_End(void);
void UI_APP_CA_MasterSlave_Key(BU32 key);

//void UI_APP_CA_MasterSlaveManage_Enter(void);
//void UI_APP_CA_MasterSlaveManage_End(void);
//void UI_APP_CA_MasterSlaveManage_Key(BU32 key);
//void UI_APP_CA_RequestFeed_Timer(void);
void UI_APP_CA_RequestFeed_Enter(void);
void UI_APP_CA_RequestFeed_Key(BU32 key);

//拔卡
void UI_APP_CA_CardOut_Enter(void);
void UI_APP_CA_CardOut_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_CA_CardOut_Timer(void);

//OSD
void UI_APP_CAOsd_ProgramInfo_Timer(void);
void UI_APP_CAOsd_ProgramInfo_Paint(BU32 hdc,BU32 lParam);

//单频点
void UI_APP_AreaLock_Enter(void);
void UI_APP_AreaLock_End(void);
void UI_APP_AreaLock_Key(BU32 key);
void UI_APP_AreaLockInfo_Enter(void);

//退订订购IPP
void UI_APP_CA_BookUnbookIPP_Enter(void);
void UI_APP_CA_BookUnbookIPP_End(void);
void UI_APP_CA_BookUnbookIPP_Key(BU32 key);
void UI_APP_CA_BookUnbookIPP_TBXKey(BU32 key);
void UI_APP_CA_BookUnbookIPP_Change(BU32 key);
void UI_APP_CA_BookUnbookInfo_Key(BU32 key);
void UI_APP_CA_BookUnbookInfo_Timer(void);

//实时购买IPP节目
void UI_APP_CA_RealTimeBook_Enter(void);
void UI_APP_CA_RealTimeBook_End(void);
void UI_APP_CA_RealTimeBook_Key(BU32 key);
void UI_APP_CA_RealTimeBook_PinKey(BU32 key);
void UI_APP_CA_RealTimeBook_ZhouqiKey(BU32 key);

//验证用户的PIN码
void UI_APP_CA_VerifyPin_Enter(void);
void UI_APP_CA_VerifyPin_End(void);
void UI_APP_CA_VerifyPin_Key(BU32 key);
#endif

//播放器对话框
void UI_APP_Player_Enter(void);
void UI_APP_Player_End(void);
void UI_APP_Player_Key(BU32 key);
void UI_APP_PlayerFileList_OnKey(BU32 key);
void UI_APP_PlayerDisplay_OnKey(BU32 key);
void UI_APP_PlayerDisplay_Change();
void UI_APP_PlayerFileList_Change();
void UI_APP_CurTime_Player_OnPaint(BU32 hdc, BU32 lParam);
void UI_APP_Player_Timer(void);


//文件管理
void UI_APP_Usb_Enter(void);
void UI_APP_Usb_End(void);
void UI_APP_Usb_Key(BU32 key);
void UI_APP_Usb_LbpKey(BU32 key);
void UI_APP_Usb_CbxKey(BU32 key);
void UI_APP_Usb_EdtKey(BU32 key);
void UI_APP_Usb_Change(void);

//图片浏览状态
void UI_APP_PictureShow_Enter(void);
void UI_APP_PictureShow_End(void);
void UI_APP_PictureShow_Key(BU32 key);
void UI_APP_PictureShow_Timer(void);


//数码相框
void UI_APP_PhotoFrame_Enter(void);
void UI_APP_PhotoFrame_End(void);
void UI_APP_PhotoFrame_Key(BU32 key);
void UI_APP_PhotoShowSet_Key(BU32 key);
void UI_APP_Frame_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_Frame_Onkey(BU32 key);



//特效菜单
void UI_APP_EffectsMenu_Enter(void);
void UI_APP_EffectsMenu_End(void);
void UI_APP_EffectsMenu_Key(BU32 Key);

//特效提示框
void UI_APP_RecDiskFull_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
void UI_APP_RecError_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
void UI_APP_PlayEndOfFile_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
void UI_APP_PlayStartOfFile_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
void UI_APP_PlayError_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
void UI_APP_PlayReachRec_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
void UI_APP_EffectsInfo_Enter(void);
void UI_APP_EffectsInfo_End(void);
void UI_APP_EffectsInfo_Key(BU32 key);
void UI_APP_EffectsInfo_Timer(void);

//手动选时录制
void UI_APP_ManualRec_Enter(void);
void UI_APP_ManualRec_End(void);
void UI_APP_ManualRec_StartTime_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_ManualRec_EndTime_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_ManualRec_Key(BU32 key);
void UI_APP_ManualRec_Prog_Change(void);
void UI_APP_ManualRec_Week_Change(void);

//时移
void UI_APP_TimeShift_Enter(void);
void UI_APP_TimeShift_End(void);
void UI_APP_TimeShift_key(BU32 key);
void UI_APP_NowTime_TimeShift_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_ODCNowTime_TimeShift_key(BU32 key);
void UI_APP_JumpTO_TimeShift_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_ODCJumpTO_TimeShift_key(BU32 key);
void UI_APP_TimeShift_Timer(void);
void UI_APP_BrushScreen_TimeShift_Timer(void);

//保存新的IP设置
void UI_APP_PreserveNetset_Enter(void);
void UI_APP_PreserveNetset_End(void);
void UI_APP_PreserveNetset_Key(BU32 key);

//保存IP设置信息提示
void UI_APP_PromptNetSet_Enter(void);
void UI_APP_PromptNetSet_End(void);
void UI_APP_PromptNetSet_Timer(void);

//Nvod影视点播
void UI_APP_Nvod_Enter(void);
void UI_APP_Nvod_End(void);
void UI_APP_Nvod_Key(BU32 key);
void UI_APP_Nvod_EventList_Key(BU32 key);
void UI_APP_Nvod_EventList_Change(void);
void UI_APP_Nvod_EventPlayTime_Key(BU32 key);
void UI_APP_Nvod_EventPlayTime_Change(void);
void UI_APP_Nvod_Timer(void);

//Nvod全屏播放
void UI_APP_NvodPlay_Enter(void);
void UI_APP_NvodPlay_End(void);
void UI_APP_NvodPlay_Key(BU32 key);
void UI_APP_NvodPlay_Timer(void);

//NvodPlay全屏播放退出提示框
void UI_APP_NvodPlayInfo_Enter(void);
void UI_APP_NvodPlayInfo_End(void);
void UI_APP_NvodPlayInfo_Key(BU32 key);

//进入Nvod时等待界面
void UI_APP_NvodWait_Enter(void);
void UI_APP_NvodWait_End(void);
void UI_APP_NvodWait_Key(BU32 key);
void UI_APP_NvodWait_Timer(void);

//工厂菜单
void UI_APP_FactoryMenu_Enter(void);
void UI_APP_FactoryMenu_End(void);
void UI_APP_FactoryMenu_CBXKey(BU32 key);
void UI_APP_SnInput_Timer(void);


//full to tvod
void UI_APP_BPJUMP_OnPaint(BU32 hdc,BU32 lParam);
//void UI_APP_BPJUMP_ODC_Key(BU32 key);
void UI_APP_FULLTOTVOD_Enter(void);
void UI_APP_FULLTOTVOD_End(void);
void UI_APP_FULLTOTVOD_key(BU32 key);
void UI_APP_BPZHUN_OnPaint(BU32 hdc,BU32 lParam);
void UI_APP_BPZHUN_ODC_Key(BU32 key);
void UI_APP_BPZHUN_TIME(void);
void UI_APP_FAST_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_JUMPTO_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_JUMPBAR_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_TVOD_CLEANSCREEN_TIME(void);
void UI_APP_FULLTOTVOD_Timer(void);
void UI_APP_FULLTOTVOD_SignalCheck_Timer(void);
void UI_APP_FULLTOTVOD_CheckError_Timer();


void UI_APP_VOD_Enter(void);
void UI_APP_VOD_End(void);
void UI_APP_VOD_key(BU32 key);
void UI_APP_VOD_JUMPTO_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_VOD_TIME(void);
void UI_APP_VOD_JUMPBAR_OnPaint(BU32 hdc,BU32 lparam);
void UI_APP_JUMPBAR_key(BU32 key);

void UI_APP_INFOCONNECT_Enter(void);
void UI_APP_INFOCONNECT_TIME(void);
void UI_APP_INFOCONNECT_End(void);

//null
void UI_APP_NULL_TIME(void);


void UI_APP_VOD_FIRST_STATE_Enter(void);
void UI_APP_VOD_FIRST_STATE_End(void);
void UI_APP_VOD_FIRST_STATE_Timer(void);
//文件管理
void UI_APP_UsbManager_Enter(void);
void UI_APP_UsbManager_End(void);
void UI_APP_UsbManager_LbpKey(BU32 key);
void UI_APP_UsbManager_Change(void);

//Pvr播放列表
void UI_APP_PvrList_Enter(void);
void UI_APP_PvrList_End(void);
void UI_APP_PvrList_LbpKey(BU32 key);
void UI_APP_PvrList_Change(void);

//Pvr全屏播放
void UI_APP_PvrFullScreen_Enter(void);
void UI_APP_PvrFullScreen_End(void);
void UI_APP_PvrFullScreen_Key(BU32 key);
void UI_APP_PvrFullScreen_Timer(void);


//区域ID设置
void UI_APP_RegionSet_Enter(void);
void UI_APP_RegionSet_End(void);
void UI_APP_RegionSet_CBXKey(BU32 key);
//工厂菜单
void UI_APP_FactoryMenu_Enter(void);
void UI_APP_FactoryMenu_End(void);
void UI_APP_FactoryMenu_Key(BU32 key);


//stbid 设置
void UI_APP_StbIDSetting_Enter(void);
void UI_APP_StbIDSetting_End(void);
void UI_APP_StbIDSetting_Key(BU32 key);
void UI_APP_StbIDSetting_Timer(void);

//使用操作指南
void UI_APP_OperationGuide_Enter(void);
void UI_APP_OperationGuide_Key(BU32 key);
void UI_APP_OperationGuide_OnPaint(BU32 hdc, BU32 lParam);

//输出设置
void UI_APP_OutPutSet_Enter(void);
void UI_APP_OutPutSet_End(void);
void UI_APP_OutPutSet_Key(BU32 key);

//显示设置
void UI_APP_DisplaySet_Enter(void);
void UI_APP_DisplaySet_End(void);
void UI_APP_DisplaySet_Key(BU32 key);

//频道设置
void UI_APP_ChannelSet_Enter(void);
void UI_APP_ChannelSet_End(void);
void UI_APP_ChannelSet_Key(BU32 key);

//频道设置保存提示框
void UI_APP_ChannelSetInfo_Enter(void);
void UI_APP_ChannelSetInfo_Key(BU32 key);
void UI_APP_ChannelSetInfo_Timer(void);

//父母控制
void UI_APP_ParentControl_Enter(void);
void UI_APP_ParentControl_End(void);
void UI_APP_ParentControl_Key(BU32 key);

void UI_APP_SysUpgrade_Enter();
void UI_APP_SysUpgrade_Key(BU32 key);
//频点设置
void UI_APP_FreqSet_Enter(void);
void UI_APP_FreqSet_Key(BU32 key);

//序列号
void UI_APP_Sequence_Enter(void);
void UI_APP_Sequence_Key(BU32 key);

//空中升级
void UI_APP_OTA_Enter(void);
void UI_APP_OTA_Key(BU32 key);
void UI_APP_OTA_Timer(void);
//串口写序列号
void UI_APP_ComSn_Enter(void);
void UI_APP_ComSn_End(void);
void UI_APP_ComSn_Smc_Key(BU32 key);
void UI_APP_ComSn_E2_Key(BU32 key);
void UI_APP_ComSn_Com_Key(BU32 key);
void UI_APP_ComSn_NetSet_Key(BU32 key);
void UI_APP_ComSn_Timer(void);

//互动帮助
void UI_APP_DoubleHelpInfo_Enter(void);
void UI_APP_DoubleHelpInfo_End(void);
void UI_APP_DoubleHelpInfo_Key(BU32 key);

void UI_APP_EPGDetail_Enter(void);
void UI_APP_EPGDetail_End(void);
void UI_APP_EPGDetail_Key(BU32 key);

#endif
