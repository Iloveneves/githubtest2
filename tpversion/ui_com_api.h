
//��Ŀ�б���ؽṹ�嶨��
typedef struct CHN_LIST_S
{
	BU08 *chnName;
	BU08 fav;
	BU08 lock;
}CHN_LIST_S;

//ȫ����Ŀ��Ϣ
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
//��Ŀָ��Epg
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

//��Ŀ�༭��ؽṹ�嶨��

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

//��������
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

//nvodȫ������״̬����
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

//ʱ�Ʋ���״̬
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
��������ʾ��ǰ����һ����ĿԤ��
-----------------------------------------------------------------------------*/
void UI_APP_API_ShowCurAndNextEPG(BU32 EventID1,BU32 EventID2,BU32 PbrID, BU32 PbrLength, BU32 Param);
/*----------------------------------------------------------------------------
��������ʾ��ǰ����һ����ĿԤ��
-----------------------------------------------------------------------------*/
void UI_APP_API_ShowCurAndNextEPGWithEndTime(BU32 EventID1,BU32 EventID2,BU32 PbrID, BU32 PbrLength,BU32 Param);
/*----------------------------------------------------------------------------
���������µ�ǰ����һ����ĿԤ��
-----------------------------------------------------------------------------*/
void UI_APP_API_RenewEvent(BU32 curID,BU32 nextID, BU32 PbrID, BU32 PbrLength, BU32 bEndtime);
/*----------------------------------------------------------------------------
���������ã��ͻ�ȡ��Ŀ�б�dialog��ǰ״̬
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnListPreState(BU32 dlg_id);
BU32 UI_APP_API_GetChnListPreState(void);
/*----------------------------------------------------------------------------
����������������С
-----------------------------------------------------------------------------*/
void UI_APP_API_SetVolume(void);
/*----------------------------------------------------------------------------
������
-----------------------------------------------------------------------------*/
void UI_APP_API_SetLockServiceId(BU32 serviceid);
BU32 UI_APP_API_GetLockServiceId(void);
/*----------------------------------------------------------------------------
������
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurChnbPlay(BU32 bplay);
BU32 UI_APP_API_GetCurChnbPlay(void);
/*----------------------------------------------------------------------------
������
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnBUnlock(BU32 bunlock);
BU32 UI_APP_API_GetChnBUnlock(void);
/*----------------------------------------------------------------------------
������
-----------------------------------------------------------------------------*/
void UI_APP_API_SetLockCurState(BU32 curstate);
BU32 UI_APP_API_GetLockCurState(void);
/*----------------------------------------------------------------------------
���������Ž�Ŀ
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckPlay_PlayChn(BU16 curChn, BU08 bForce);
/*----------------------------------------------------------------------------
������ֹͣ���Ž�Ŀ
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_StopChn(BU32 isNeedBlkScrn);
/*----------------------------------------------------------------------------
���������Ž�Ŀ�����������ʾ��Ŀ��
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckPlay_PlayChn_led(BU16 curChn, BU08 bForce);
/*----------------------------------------------------------------------------
����������Ŀ�Ƿ���������������ѡ���Ƿ񲥷Ž�Ŀ
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_PlayChn(BU16 curChn,BU32 checkLock);
/*----------------------------------------------------------------------------
����������Ŀ�Ƿ���������������ѡ���Ƿ񲥷Ž�Ŀ,�����������ʾ��Ŀ��
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_PlayChn_led(BU16 curChn,BU32 ledIndex,BU32 checkLock);
/*----------------------------------------------------------------------------
���������á���������Ƿ񱣴浽Flash�ı�־
-----------------------------------------------------------------------------*/
void UI_APP_API_SetbSaveFlag(BU32 bsave);
BU32 UI_APP_API_GetbSaveFlag(void);
/*----------------------------------------------------------------------------
���������á������ȫ�����Ž�Ŀʱ��ǰ��ʾ״̬
		��:	��ʾ��Ŀ��Ϣ״̬,
			��ʾ����״̬,
			��������״̬,
			��ʾû�н�Ŀ״̬,
			��ʾ��Ŀ��״̬,
			��ʾû�з���״̬
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnInfoCurState(BU32 curstate);
BU32 UI_APP_API_GetChnInfoCurState(void);
/*----------------------------------------------------------------------------
���������á������ȫ�����Ž�Ŀʱ������ʾ����ʾ�������������ֵ��е�ID
		��:"���ܽ�Ŀ����忨"
			"��Ŀ��ͣ"
			"�޽�Ŀ"
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnInfoSaveID(BU32 rs_id);
BU32 UI_APP_API_GetChnInfoSaveID(void);

/*----------------------------------------------------------------------------
����������ͻ�õ�ǰ��Ŀ��progid
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurChnProgID(BU32 prog_id);
BU32 UI_APP_API_GetCurChnProgID(void);
/*----------------------------------------------------------------------------
���������ã����epg�Ƿ񱣴棬����ľ���epg�е�ԤԼ��Ŀ
-----------------------------------------------------------------------------*/
void UI_APP_API_SetEpgSaveFlag(BU08 savefalg);
BU08 UI_APP_API_GetEpgSaveFlag(void);
/*----------------------------------------------------------------------------
���������á���ȡepg Dialog��ǰһ��״̬
-----------------------------------------------------------------------------*/
void UI_APP_API_SetEpgPreState(BU32 dlg_id);
BU32 UI_APP_API_GetEpgPreState(void);
/*----------------------------------------------------------------------------
���������á���ȡSortProg Dialog��ǰһ��״̬
-----------------------------------------------------------------------------*/
void UI_APP_API_SetSortProgPreState(BU32 dlg_id);
BU32 UI_APP_API_GetSortProgPreState(void);
/*----------------------------------------------------------------------------
���������á���ȡԤԼ�б�Dialog��ǰһ��״̬����:�����˵����Epg����ԤԼ�б�Ի���
-----------------------------------------------------------------------------*/
void UI_APP_API_SetReserveListPreState(BU32 id);
BU32 UI_APP_API_GetReserveListPreState(void);
/*----------------------------------------------------------------------------
������
-----------------------------------------------------------------------------*/
void UI_APP_SetDefaultState(BU32 dlg_id);
BU32 UI_APP_GetDefaultState(void);
/*----------------------------------------------------------------------------
���������á���ȡ�ʼ�ǰ״̬
-----------------------------------------------------------------------------*/
void UI_APP_CA_SetMailPreState(BU32 id);
BU32 UI_APP_CA_getMailPreState(void);
/*----------------------------------------------------------------------------
���������á���ȡ�ʼ��Ƿ�����״̬
-----------------------------------------------------------------------------*/
void UI_APP_SetMailFullState(BU08 id);
BU08 UI_APP_GetMailFullState(void);
/*----------------------------------------------------------------------------
���������û��ȡPIN�����ʾ���ǰ״̬����:��������,����ʱ�����õ�
-----------------------------------------------------------------------------*/
void UI_APP_API_SetPinNotePreState(BU32 pre_state);
BU32 UI_APP_API_GetPinNotePreState(void);
/*----------------------------------------------------------------------------
���������ã��ͻ�ȡ���ݹ㲥�б�dialog��ǰ״̬
-----------------------------------------------------------------------------*/
void UI_APP_API_SetBrdcastListPreState(BU32 dlg_id);
BU32 UI_APP_API_GetBrdcastListPreState(void);
/*----------------------------------------------------------------------------
���������ã��ͻ�ȡ���ݹ㲥�б��е�ǰ���ݹ㲥
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurBrdcastNo(BU32 curno);
BU32 UI_APP_API_GetCurBrdcastNo(void);
/*ʱ��ת������,����CDCA����ת��Ϊ����*/
BU08 UI_APP_GetDateStrFromValC_TF(unsigned short wInputDate,char * szOut); 
/*ʱ��ת������,����CDCA����ת��Ϊʱ��*/
BU08 UI_APP_GetTimeStrFromValC_TF(unsigned int wInputDate,char * szOut);
/*ʱ��ת������,����������Ѷ����ת��Ϊ����*/
/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22.*/
BU08 UI_APP_GetDateStrFromValC(unsigned short wInputDate,char * szOut);
/*ʱ��ת������,����������Ѷ����ת��Ϊ���ڼ�ʱ��*/
/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22 12:3:5.*/
BU08 UI_APP_GetTimeStrFromValC(int nInputTime,char * szOut);
#if 0
void UI_APP_convertSecIntoDay (BU32 secInTotal, caInfo_time_s * outPut);
void UI_APP_convertFengIntoYuan (BU32 fengInTotal, caInfo_currency_s * outPut);
#endif
/******************************
���������ã��ͻ�ȡɾ����ǰĿ¼�µ��ļ������
*******************************/
void UI_APP_API_SetDelFileNo(BU32 FileNo);
BU32 UI_APP_API_GetDelFileNo(void);
/******************************
���������ã��ͻ�ȡNvod�¼����
*******************************/
void UI_APP_API_SetNvodEventIndex(BU32 EventIndex);
BU32 UI_APP_API_GetNvodEventIndex(void);
/******************************
���������ã��ͻ�ȡ��ͻ���
*******************************/
void UI_APP_API_SetConflictIndex(BU32 ConflictIndex);
BU32 UI_APP_API_GetConflictIndex(void);

/*************************************
����:���úͻ�ȡtimeshiftȫ���µĸ�״̬
*************************************/
void UI_APP_API_SetTimeShiftCurState(BU32 CurState);
BU32 UI_APP_API_GetTimeShiftCurState(void);


