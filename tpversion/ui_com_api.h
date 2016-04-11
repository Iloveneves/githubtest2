
//节目列表相关结构体定义
typedef struct CHN_LIST_S
{
	BU08 *chnName;
	BU08 fav;
	BU08 lock;
}CHN_LIST_S;

//全屏节目信息
enum FULLSCREEN_STATE
{
	NULL_STATE = 0,
	INFOBAR_STATE,
	VOICE_STATE,
	OFFSET_STATE,
	TRACKSET_STATE,
	NOPROG_STATE,
	CHNUM_STATE,
	NO_SERVICE,
	IPP_STATE,
	NET_STATE
};
//节目指南Epg
typedef struct _EPG_CHN_LIST_S
{
	BU08 *chnName;
	BU08 fav;
	BU08 lock;
}EPG_CHN_LIST_S;

typedef struct _EPG_EVENT_LIST_S
{
	BU08 *eventName;
	BU08 now;
	BU08 pass;
	BU08 reserved;
	BU08 info;
}EPG_EVENT_LIST_S;
typedef enum _RESERVE_TYPE_E
{
	RESERVE_NULL,
	RESERVE_PLAYED,
	RESERVE_PLAYING,
	RESERVE_PLAYSOON,
	RESERVE_CONFLICT,
	RESERVE_CONFLICTREC,
	RESERVE_CONFLICT_NVOD,
	RESERVE_FULL,
	RESERVE_OK,
	RESERVE_CANCEL

}RESERVE_TYPE_E;

//节目编辑相关结构体定义

typedef struct _CHN_EDIT_LIST_S
{
	BU08 *chnName;
	BU08 fav;
	BU08 del;
	BU08 lock;
	BU08 move;
}CHN_EDIT_LIST_S;


typedef struct _CHN_EDIT_NODE_S
{
	BU32 serviceId;
	BU08 fav;
	BU08 lock;
	struct _CHN_EDIT_NODE_S *next;
}EDIT_NODE_S,*EDIT_NODE_SP;

//条件接收
//ca
typedef enum _PIN_STATE_E
{
	CA_PIN_NULL_e,
	CA_PIN_LOCKED_e,
	CA_PIN_ERROR_e,
	CA_PIN_OK_e,
	CA_CHANGE_SUCESS_e,
	CA_CHANGE_FAIL_e,
}CA_PIN_STATE_E;

typedef struct
{
	BU16 wTvsID;
	BU08 bstatus;
	BU32 xPos;
	BU32 yPos;
	BU32 show;
}ui_detitle_received_info_s;

//nvod全屏播放状态类型
typedef enum _NVOD_FULLPLAY_STATE
{
	NVOD_FULLPLAY_NULL_STATE = 0,
	NVOD_FULLPLAY_INFO_STATE,
	NVOD_FULLPLAY_VOICE_STATE,
	NVOD_FULLPLAY_CHNUM_STATE,
	NVOD_FULLPLAY_PROG_INFO_STATE,
	NVOD_FULLPLAY_PROG_DETAIL_STATE,
	NVOD_FULLPLAY_NO_SERVICE
}NVOD_FULLPLAY_STATE_E;
typedef struct _tagUI_APP_CAOsd_s
{
	BU32 bu32XPos;
	BU32 bu32YPos_top;
	BU32 bu32YPos;
	BU32 bu32IDYPos;
	PU08 szOSD_top;
	PU08 szOSD;
	BU32 szOSD_len_top;
	BU32 szOSD_len;
	BU32 time_out_top;
	BU32 time_out;
	BU32 bu32CardID;
	BU32 bu32ShowInfo_top;
	BU32 bu32ShowInfo;
	BU32 bu32ShowID;
	BU32 old_offset_x;	
	BU32 bu32IDCnt;
	BU32 printDuration;
	BU32 bu32IDFirst;
	BU08 old_len;//add by jx080512
} UI_APP_CAOsd_s;

//时移播放状态
typedef enum _TIMESHIFT_STATE_E
{
    TIMESHIFT_NULL_STATE = 0,
	TIMESHIFT_HELP_STATE,
	TIMESHIFT_JUMPBAR_STATE,
	TIMESHIFT_JUMPTO_STATE,
	TIMESHIFT_PAUSE_STATE,
	TIMESHIFT_SPEED_STATE,
	TIMESHIFT_VOIBAR_STATE
}TIMESHIFT_STATE_E;

/*----------------------------------------------------------------------------
描述：显示当前、下一个节目预告
-----------------------------------------------------------------------------*/
void UI_APP_API_ShowCurAndNextEPG(BU32 EventID1,BU32 EventID2,BU32 PbrID, BU32 PbrLength, BU32 Param);
/*----------------------------------------------------------------------------
描述：显示当前、下一个节目预告
-----------------------------------------------------------------------------*/
void UI_APP_API_ShowCurAndNextEPGWithEndTime(BU32 EventID1,BU32 EventID2,BU32 PbrID, BU32 PbrLength,BU32 Param);
/*----------------------------------------------------------------------------
描述：更新当前、下一个节目预告
-----------------------------------------------------------------------------*/
void UI_APP_API_RenewEvent(BU32 curID,BU32 nextID, BU32 PbrID, BU32 PbrLength, BU32 bEndtime);
/*----------------------------------------------------------------------------
描述：设置，和获取节目列表dialog的前状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnListPreState(BU32 dlg_id);
BU32 UI_APP_API_GetChnListPreState(void);
/*----------------------------------------------------------------------------
描述：设置音量大小
-----------------------------------------------------------------------------*/
void UI_APP_API_SetVolume(void);
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetLockServiceId(BU32 serviceid);
BU32 UI_APP_API_GetLockServiceId(void);
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurChnbPlay(BU32 bplay);
BU32 UI_APP_API_GetCurChnbPlay(void);
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnBUnlock(BU32 bunlock);
BU32 UI_APP_API_GetChnBUnlock(void);
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetLockCurState(BU32 curstate);
BU32 UI_APP_API_GetLockCurState(void);
/*----------------------------------------------------------------------------
描述：播放节目
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckPlay_PlayChn(BU16 curChn, BU08 bForce);
/*----------------------------------------------------------------------------
描述：停止播放节目
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_StopChn(BU32 isNeedBlkScrn);
/*----------------------------------------------------------------------------
描述：播放节目并在面板上显示节目号
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckPlay_PlayChn_led(BU16 curChn, BU08 bForce);
/*----------------------------------------------------------------------------
描述：检测节目是否加锁，并根据情况选择是否播放节目
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_PlayChn(BU16 curChn,BU32 checkLock);
/*----------------------------------------------------------------------------
描述：检测节目是否加锁，并根据情况选择是否播放节目,并在面板上显示节目号
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_PlayChn_led(BU16 curChn,BU32 ledIndex,BU32 checkLock);
/*----------------------------------------------------------------------------
描述：设置、获得数据是否保存到Flash的标志
-----------------------------------------------------------------------------*/
void UI_APP_API_SetbSaveFlag(BU32 bsave);
BU32 UI_APP_API_GetbSaveFlag(void);
/*----------------------------------------------------------------------------
描述：设置、获得在全屏播放节目时当前显示状态
		如:	显示节目信息状态,
			显示音量状态,
			声道设置状态,
			显示没有节目状态,
			显示节目号状态,
			显示没有服务状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnInfoCurState(BU32 curstate);
BU32 UI_APP_API_GetChnInfoCurState(void);
/*----------------------------------------------------------------------------
描述：设置、获得在全屏播放节目时，在提示框显示的内容在数字字典中的ID
		如:"加密节目，请插卡"
			"节目暂停"
			"无节目"
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnInfoSaveID(BU32 rs_id);
BU32 UI_APP_API_GetChnInfoSaveID(void);

/*----------------------------------------------------------------------------
描述：保存和获得当前节目的progid
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurChnProgID(BU32 prog_id);
BU32 UI_APP_API_GetCurChnProgID(void);
/*----------------------------------------------------------------------------
描述：设置，获得epg是否保存，保存的就是epg中的预约节目
-----------------------------------------------------------------------------*/
void UI_APP_API_SetEpgSaveFlag(BU08 savefalg);
BU08 UI_APP_API_GetEpgSaveFlag(void);
/*----------------------------------------------------------------------------
描述：设置、获取epg Dialog的前一个状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetEpgPreState(BU32 dlg_id);
BU32 UI_APP_API_GetEpgPreState(void);
/*----------------------------------------------------------------------------
描述：设置、获取SortProg Dialog的前一个状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetSortProgPreState(BU32 dlg_id);
BU32 UI_APP_API_GetSortProgPreState(void);
/*----------------------------------------------------------------------------
描述：设置、获取预约列表Dialog的前一个状态，如:从主菜单或从Epg进入预约列表对话框
-----------------------------------------------------------------------------*/
void UI_APP_API_SetReserveListPreState(BU32 id);
BU32 UI_APP_API_GetReserveListPreState(void);
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_SetDefaultState(BU32 dlg_id);
BU32 UI_APP_GetDefaultState(void);
/*----------------------------------------------------------------------------
描述：设置、获取邮件前状态
-----------------------------------------------------------------------------*/
void UI_APP_CA_SetMailPreState(BU32 id);
BU32 UI_APP_CA_getMailPreState(void);
/*----------------------------------------------------------------------------
描述：设置、获取邮件是否已满状态
-----------------------------------------------------------------------------*/
void UI_APP_SetMailFullState(BU08 id);
BU08 UI_APP_GetMailFullState(void);
/*----------------------------------------------------------------------------
描述：设置或获取PIN检测提示框的前状态框，如:成人设置,工作时段设置等
-----------------------------------------------------------------------------*/
void UI_APP_API_SetPinNotePreState(BU32 pre_state);
BU32 UI_APP_API_GetPinNotePreState(void);
/*----------------------------------------------------------------------------
描述：设置，和获取数据广播列表dialog的前状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetBrdcastListPreState(BU32 dlg_id);
BU32 UI_APP_API_GetBrdcastListPreState(void);
/*----------------------------------------------------------------------------
描述：设置，和获取数据广播列表中当前数据广播
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurBrdcastNo(BU32 curno);
BU32 UI_APP_API_GetCurBrdcastNo(void);
/*时间转换函数,来自CDCA整型转化为日期*/
BU08 UI_APP_GetDateStrFromValC_TF(unsigned short wInputDate,char * szOut); 
/*时间转换函数,来自CDCA整型转化为时间*/
BU08 UI_APP_GetTimeStrFromValC_TF(unsigned int wInputDate,char * szOut);
/*时间转换函数,来自数码视讯整型转化为日期*/
/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22.*/
BU08 UI_APP_GetDateStrFromValC(unsigned short wInputDate,char * szOut);
/*时间转换函数,来自数码视讯整型转化为日期兼时间*/
/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22 12:3:5.*/
BU08 UI_APP_GetTimeStrFromValC(int nInputTime,char * szOut);
#if 0
void UI_APP_convertSecIntoDay (BU32 secInTotal, caInfo_time_s * outPut);
void UI_APP_convertFengIntoYuan (BU32 fengInTotal, caInfo_currency_s * outPut);
#endif
/******************************
描述：设置，和获取删除当前目录下的文件的序号
*******************************/
void UI_APP_API_SetDelFileNo(BU32 FileNo);
BU32 UI_APP_API_GetDelFileNo(void);
/******************************
描述：设置，和获取Nvod事件序号
*******************************/
void UI_APP_API_SetNvodEventIndex(BU32 EventIndex);
BU32 UI_APP_API_GetNvodEventIndex(void);
/******************************
描述：设置，和获取冲突序号
*******************************/
void UI_APP_API_SetConflictIndex(BU32 ConflictIndex);
BU32 UI_APP_API_GetConflictIndex(void);

/*************************************
描述:设置和获取timeshift全屏下的各状态
*************************************/
void UI_APP_API_SetTimeShiftCurState(BU32 CurState);
BU32 UI_APP_API_GetTimeShiftCurState(void);


