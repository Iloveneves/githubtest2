#include "include/resource.h"


//��Ŀ����

BU32 ProgManager[]={RS_ProgManager,7,RS_TVList,RS_RadioList,RS_BatList,RS_TVSearch,RS_TVEdit,RS_RadioEdit,RS_ReserveManager,
					RS_TVSearch,3,RS_AllSearch,RS_AutoSearch,RS_ManualSearch,
					RS_TVEdit,4,RS_FavChn,RS_DelChn,RS_ChangeChn,RS_LockChn,
					RS_RadioEdit,4,RS_FavRadio,RS_DelRadio,RS_ChangeRadio,RS_LockRadio,
};
//��Ŀ�б�
BU32 ProgType[]={RS_AllProg,RS_FavProg};
GUI_lbpAttribute_s lbpa_ProgList[]=
{
	{STYLE_Faverite_W20_H20,STYLE_Faverite_W20_H20,268,6,-1},
	{STYLE_Lock_W20_H20,STYLE_Lock_W20_H20,238,6,-1},
};
//��������
BU32 cbx_Track[]={RS_Stereo, RS_Left, RS_Right, RS_Single};
BU32 cbx_Offset[]={RS_N15,RS_N14,RS_N13,RS_N12,RS_N11,RS_N10,RS_N9,RS_N8,RS_N7,RS_N6,RS_N5,RS_N4,RS_N3,RS_N2,RS_N1,
						RS_0,RS_1,RS_2,RS_3,RS_4,RS_5,RS_6,RS_7,RS_8,RS_9,RS_10,RS_11,RS_12,RS_13,RS_14,RS_15};
//��Ŀ��
BU32 ProgNumber[]={STYLE_Digital0_W30_H33,STYLE_Digital1_W30_H33,STYLE_Digital2_W30_H33,
				STYLE_Digital3_W30_H33,STYLE_Digital4_W30_H33,STYLE_Digital5_W30_H33,
				STYLE_Digital6_W30_H33,STYLE_Digital7_W30_H33,STYLE_Digital8_W30_H33,
				STYLE_Digital9_W30_H33,};
//�ֶ���������
BU32 QAM[]={ RS_16QAM, RS_32QAM,RS_64QAM,RS_128QAM,RS_256QAM};
BU32 cbx_OnOff[]={ RS_Off, RS_On };

GUI_lbpAttribute_s lbpa_ChEdit[]=
{
	{STYLE_Faverite_W20_H20,STYLE_Faverite_W20_H20,284,8,-1},
	{STYLE_Delete_W20_H20,STYLE_Delete_W20_H20,454,8,-1},
    {STYLE_Lock_W20_H20,STYLE_Lock_W20_H20,530,8,-1},
};
GUI_lbpAttribute_s lbpa_move=
{
   STYLE_Move_W20_H20,STYLE_Move_W20_H20,370,8, -1
};

//��Ŀ�б�
BU32 BroadcastType[]={RS_AllBroadcast};//���ݹ㲥����Ʊ������

//��Ŀָ��
GUI_lbpAttribute_s lbpa_EPG[]=
{
	{STYLE_Faverite_W20_H20, STYLE_Faverite_W20_H20, 190, 5, -1},
	{STYLE_Lock_W20_H20, STYLE_Lock_W20_H20, 160, 7, -1},
};
BU32 tab_WeekTable[]={RS_Monday,RS_Tuesday,RS_Wednesday,RS_Tursday,RS_Friday,RS_Saturday,RS_Sunday};
GUI_lbpAttribute_s _lbpa_event[]=
{
	{STYLE_EPGNow_W24_H24,STYLE_EPGNow_W24_H24,2,2,-1},
	{STYLE_EPGPass_W24_H24,STYLE_EPGPass_W24_H24,2,2,-1},
 	{STYLE_EPGDestine_W24_H24,STYLE_EPGDestine_W24_H24,2,2,-1},
  	{STYLE_EPGDetail_W30_H30,STYLE_EPGDetail_W30_H30,352,2,-1},
};

//������Ϸ
#ifdef ENABLE_USB
BU32 GameMenu[]=
{
		RS_EnterTainment,4,RS_Teltris,RS_Snake,RS_Sokoban,RS_EffectsMenu,
};
#else
BU32 GameMenu[]=
{
		RS_EnterTainment,3,RS_Teltris,RS_Snake,RS_Sokoban,
};
#endif
//ϵͳ����
BU32 SystemSetMenu[]=
	{
		RS_SystemSetting,9,RS_ProgManager,RS_TVSearch,RS_Games,RS_ChildLimit,RS_ParameterSet,RS_STBSetting,RS_SignalDetect,RS_NetSetting,RS_OperationGuide,
		RS_ProgManager,3,RS_TVEdit,RS_RadioEdit,RS_ReserveManager,
		RS_TVSearch,4,RS_AutoSearch,RS_ManualSearch,RS_AllSearch,RS_MainFreqSet,
	#ifdef ENABLE_USB
		RS_Games,4,RS_Teltris,RS_Snake,RS_Sokoban,RS_EffectsMenu,
	#else
		RS_Games,3,RS_Teltris,RS_Snake,RS_Sokoban,
	#endif
		RS_ChildLimit,4,RS_LockChn, RS_LockRadio, RS_ParentControl, RS_PasswordSetting,
	//	RS_ParameterSet,3,RS_LanguageSetting,RS_OutPutSet,RS_DisplaySet,
		RS_STBSetting,3,RS_Info, RS_FactoryDefault, RS_SoftWareUpdate,
		
	};
//�����˵�
BU32 FactoryMenu[]=
{
    RS_FactoryMenu,7,RS_FreqSet,RS_Sequence,RS_OTA,RS_CKSequence, RS_DetectLed, RS_ProduceResume,RS_DefaultDB,
};

//ʱ������
BU32 cbx_TimeZone[]={RS_GMT8,RS_GMT};
//����Ƶ����
BU32 cbx_Mix[]={RS_1,RS_2,RS_3,RS_4,RS_5,RS_6,RS_7,RS_8,RS_9,RS_10};

BU32 cbx_Aspect[]={RS_Audio_169TV,RS_Audio_43TV};
BU32 cbx_TvMode[]={RS_Audio_AUTO,RS_Audio_PAL,RS_Audio_NTSC};
BU32 cbx_videoout[]={RS_Audio_YUV,RS_Audio_YC};
BU32 cbx_volumesave[] = {RS_PART, RS_UNITE};
//Ƶ������
BU32 cbx_ChannelSet[]={RS_No,RS_Yes};
//��ĸ����
BU32 cbx_ParentControl[]={RS_UnLock,RS_Lock};
//ϵͳ����
BU32 cbx_QAM[]={ RS_16QAM,RS_32QAM,RS_64QAM,RS_128QAM,RS_256QAM};
//��������
//BU32 cbx_InfoTime[]={RS_OFF,RS_1sec,RS_2sec,RS_3sec,RS_4sec,RS_5sec,RS_6sec,RS_7sec,RS_8sec,RS_9sec,RS_10sec};
BU32 cbx_InfoTime[]={RS_3sec,RS_4sec,RS_5sec,RS_6sec,RS_7sec};
//�������
BU32 cbx_order[]={RS_Order, RS_Random};
BU32 cbx_effect[]={RS_NoEffect, RS_Effect1, RS_Effect2, RS_Effect3, RS_Effect4, RS_Effect5, RS_Effect6, RS_Effect7, RS_Effect8, RS_Effect9, RS_Effect10};
BU32 cbx_seconds[]={RS_3S,RS_5S,RS_8S,RS_10S};
BU32 cbx_fullscreen[] = {RS_FUllScreenScan};
#ifdef SYS_CONFIG_LANG_ENG
BU32 cbx_Lang[]={RS_English/*,RS_Chinese*/};
#else
BU32 cbx_Lang[]={RS_English,RS_China};
#endif
//��������
BU32 cbx_SwitchItem[]={RS_DoubleDirec,RS_SingalDirec};
BU32 cbx_AutoIPDNS[]={RS_On,RS_Off};

//��������
#if defined (CD_CA)
BU32 CaMenu[]=
	{
		RS_CA,8,RS_CaCardInfo,RS_CaCardSet,RS_CardPair,RS_EntitleInfo,RS_IPPInfo,RS_SlotsInfo,RS_MasterSlave,RS_Detitle_Ack_Code,
		RS_CaCardSet,3,RS_CaPasswdSet,RS_WorkTime,RS_Rating,
	};
#elif defined (DVT_CA)
BU32 CaMenu[]=
	{
		RS_CA,7,RS_CaCardInfo,RS_EntitleInfo,RS_Email,RS_SlotsInfo,RS_IPPInfo,RS_MasterSlave,RS_CaCardSet,
		RS_CaCardSet,3,RS_CaPasswdSet,RS_Rating,RS_WorkTime
	};
#elif defined (NO_CA)
BU32 CaMenu[]=
	{
		RS_Null
	};
#endif

//�ļ��б�
GUI_lbpAttribute_s lbpa_FileList1[]=
{
	{STYLE_Faverite_W20_H20,STYLE_Faverite_W20_H20,2,1,-1},
};
//�ļ��б�
GUI_lbpAttribute_s lbpa_FileList[] =
{
	{STYLE_FolderBack_W28_H28,STYLE_FolderBack_W28_H28,1,6,-1},
  	{STYLE_FolderOpen_W28_H28,STYLE_Folder_W28_H28,1,6,-1},
	{STYLE_Picture_W28_H28,STYLE_Picture_W28_H28,1,6,-1},
	{STYLE_Mp3_W28_H28,STYLE_Mp3_W28_H28,1,5,-1},

};
//�ļ��б�
GUI_lbpAttribute_s lbpa_DisplaySet =
{
	STYLE_DisplaySelectIcon_W28_H28,STYLE_DisplaySelectIcon_W28_H28,0,2,-1
};


//��Ŀ�б�
BU32 DelFile[]={RS_DelFile};
//��Ч�˵�
BU32 EffectsMenu[]={RS_EffectsMenu,4,RS_FileManager,RS_MusicPlay,RS_PhotoFrame,RS_RecordPlay};//,RS_ManualRec};


BU32 led_detect[] = {RS_Detect};
BU32 e2p_normal[] = {RS_E2pNormal};
BU32 push_enter[] = {RS_PushEnter};
BU32 cbx_eth[] = {RS_ETH0, RS_ETH1};
BU32 gui_msg_handle(BU32 ID,BU32 message,BU32 wParam,BU32 lParam);
void osdInit(void)
{
//ע����Ϣ������
	GUI_ENC_API_RegisterMsgHander(gui_msg_handle);

//���˵�

	GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_MainMenu, WDT_STA_SV_FGC,COLOR_WHITE,0);

//�޽�Ŀ�Զ�������ʾ��
	GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_BGC, COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_BGC, COLOR_BK_S, 0);

//��Ŀ����
	GUI_ENC_API_SetValue(ID_MNU_ProgManager, WDT_MNU_SV_DATA, (BU32)ProgManager,sizeof(ProgManager)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_MNU_ProgManager, WDT_MNU_SV_ITEM_INTERVAL, 46,0);
	GUI_ENC_API_SetValue(ID_MNU_ProgManager, WDT_MNU_SV_BKSTYLE, STYLE_MenuBk_W700_H480,0);
	GUI_ENC_API_SetValue(ID_MNU_ProgManager,WDT_MNU_SV_BKSTYLE_POS,0,0);
	GUI_ENC_API_SetValue(ID_MNU_ProgManager,WDT_MNU_SV_TITLE_POSITION,62,70);
	GUI_ENC_API_SetValue(ID_MNU_ProgManager,WDT_MNU_SV_TITLE_RANGE,200,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_ProgManager,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_MNU_ProgManager,WDT_MNU_SV_EXIT_OR_RECALL,0,0);

//��Ŀ�б�
	GUI_ENC_API_SetValue(ID_STA_ProgList_ProgList,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_ProgList, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_LBP_Prog_ProgList,WDT_LBP_SV_STYLE,(BU32)lbpa_ProgList,2);
	GUI_ENC_API_SetValue(ID_STA_CardState_ProgList,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ProgList,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ProgList,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_LBP_Prog_ProgList,WDT_LBP_SV_DATAX,6,0);
//	GUI_ENC_API_SetValue(ID_CBX_ProgList,WDT_CBX_SV_DATA, (BU32)ProgType,sizeof(ProgType)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_STA_Time_ProgList,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_event1_ProgList,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_event2_ProgList,WDT_STA_SV_BGC,COLOR_INFOBK,0);

//ȫ����Ŀ��Ϣ
	GUI_ENC_API_SetValue(ID_STAText1_ChnInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STAText1_ChnInfo, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CHN_ChnInfo, WDT_CHN_SV_DATA, (BU32)ProgNumber, 10);
	GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, WDT_CBX_SV_DATA, (BU32)cbx_Track, 4);	
	GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, WDT_CBX_SV_DATA, (BU32)cbx_Offset, 31);
	GUI_ENC_API_SetValue(ID_VOI_ChnInfo, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
	GUI_ENC_API_SetValue(ID_VOI_ChnInfo, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_ChnInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_ChnInfo, WDT_STA_SV_FGC, COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Info1_ChnInfo, WDT_STA_SV_BGC, COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_Info2_ChnInfo, WDT_STA_SV_BGC, COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_BGC, COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_Text1_ChInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Text1_ChInfo,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_Text2_ChInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Text2_ChInfo,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_PBR_ChInfo,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
	GUI_ENC_API_SetValue(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_ipp_prompt_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ipp_prompt2_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ipp_tvsid_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ipp_slotid_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ipp_productid_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ipp_ExpiredDate_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ipp_pin_prompt_ChnInfo, WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue (ID_STA_ipp_pin_prompt_ChnInfo, WDT_STA_SV_FGC, (BU32)(UNLOCK_COLOR), 0);
	GUI_ENC_API_SetValue (ID_STA_ipp_title_ChnInfo, WDT_STA_SV_BGS, STYLE_ipp_title_bk_W220_H44, 0);
	GUI_ENC_API_SetValue(ID_STA_ipp_title_ChnInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
    GUI_ENC_API_SetValue(ID_STA_Name_ChnInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	

//�����б�
	GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_LBP_SortProgram, WDT_LBP_SV_STYLE, (BU32)lbpa_ProgList, 2);
	GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_ProgList_SortProgram,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Note_SortProgram,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_event1_SortProgram,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_event2_SortProgram,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_Time_SortProgram,WDT_STA_SV_FGC,COLOR_WHITE,0);


//������Ŀ
	GUI_ENC_API_SetValue(ID_LBX_TV_Search,WDT_LBX_SV_BKSTYLE,STYLE_SearchBk_W284_H180,0);
	GUI_ENC_API_SetValue(ID_LBX_Radio_Search,WDT_LBX_SV_BKSTYLE,STYLE_SearchBk_W284_H180,0);
	GUI_ENC_API_SetValue(ID_STA_Freq_Search,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_StrengthNum_Search,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_ProgressNum_Search,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_Search,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Search,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_TVNum_Search, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_RadioNum_Search, WDT_STA_SV_FGC, COLOR_WHITE, 0);

//������Ŀ������ʾ
	GUI_ENC_API_SetValue(ID_STA_Line1_SearchInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_SearchInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_SearchInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_SearchInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_SearchInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_SearchInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
//�ֶ��������� 
	GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet,WDT_CBX_SV_DATA, (long)QAM, 5);
	GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet,WDT_CBX_SV_DATA,(BU32)cbx_OnOff,2);
	GUI_ENC_API_SetValue(ID_STA_SnrStr_ManualSearchSet,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_StrengthStr_ManualSearchSet,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_ManualSearchSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_ManualSearchSet,WDT_STA_SV_FGC,COLOR_WHITE,0);

//�˵���
	GUI_ENC_API_SetValue(ID_STA_Tex_MenuLock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_MenuLock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Tex_MenuLock,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_MenuLock,WDT_STA_SV_BGC,COLOR_BK_S,0);

//��Ŀ��
	GUI_ENC_API_SetValue(ID_STA_Menu_ChnLock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Tex_ChnLock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_ChnLock,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Tex_ChnLock,WDT_STA_SV_BGC,COLOR_BK_S,0);

//������
    GUI_ENC_API_SetValue(ID_STA_Title_StartUpLock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Title_StartUpLock,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_InfoStr_StartUpLock,WDT_STA_SV_BGC,COLOR_BK_S,0);

//�Ƿ�һ���Խ�����ʾ��
	GUI_ENC_API_SetValue(ID_STA_Unlock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_Unlock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_Unlock,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Unlock,WDT_STA_SV_FGC,COLOR_WHITE,0);

//��Ƶ������
	GUI_ENC_API_SetValue(ID_CBX_OnOff_MainFreqSet, WDT_CBX_SV_LABEL, RS_MainFreqSwitch, 0);
	GUI_ENC_API_SetValue(ID_CBX_FreqSet_MainFreqSet, WDT_CBX_SV_LABEL, RS_MainFreqSet, 0);
	GUI_ENC_API_SetValue(ID_CBX_Ctr_MainFreqSet, WDT_CBX_SV_DATA, (BU32)cbx_OnOff, 2);
	GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_MainFreqSet,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_MainFreqSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//Ƶ���༭
    GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_STYLE, (BU32)lbpa_ChEdit, 3);
	GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_MOVESTYLE, (BU32)&lbpa_move, 1);
	GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_DATAX,2,0);
	GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_SortName_ChnEdit, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	/*
	GUI_ENC_API_SetValue(ID_LBP_ChannelEdit, WDT_LBP_SV_STYLE, (BU32)lbpa_ChEdit, 3);
	GUI_ENC_API_SetValue(ID_LBP_ChannelEdit, WDT_LBP_SV_MOVESTYLE, (BU32)&lbpa_move, 1);
	GUI_ENC_API_SetValue(ID_LBP_ChannelEdit,WDT_LBP_SV_DATAX,24,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_EditSort_ChannelEdit,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_ChannelEdit,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_ChannelEdit,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_event1_ChannelEdit,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_event2_ChannelEdit,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	*/


//�༭�˳���ʾ��
	GUI_ENC_API_SetValue(ID_STA_Menu_EditInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_EditInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_EditInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);

//���ݹ㲥�б�
	GUI_ENC_API_SetValue(ID_STA_Menu_BroadcastList,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_CBX_BroadcastList,WDT_CBX_SV_DATA, (BU32)BroadcastType,sizeof(BroadcastType)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_LBP_BroadcastList,WDT_LBP_SV_DATAX,24,0);
	GUI_ENC_API_SetValue(ID_STA_Time_BroadcastList,WDT_STA_SV_FGC,COLOR_WHITE,0);

//��Ŀָ��EPG
	GUI_ENC_API_SetValue(ID_STA_Epg_EPG,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_TAB_EPG, WDT_TAB_SV_DATA, (BU32)tab_WeekTable, 7);
	GUI_ENC_API_SetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_STYLE, (BU32)lbpa_EPG, 2);
	GUI_ENC_API_SetValue(ID_LBP_Event_EPG, WDT_LBP_SV_STYLE, (BU32)_lbpa_event, 4);
	GUI_ENC_API_SetValue(ID_LBP_Event_EPG, WDT_LBP_SV_DATAX,30,0);
	GUI_ENC_API_SetValue(ID_STA_Time_EPG, WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_DetailTitle_Epg,   WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_EDT_Detail_EPG, WDT_EDT_SV_SCROLL_BGS, (BU32)COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_EPG,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_EPG,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_EPG,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
//	GUI_ENC_API_SetValue(ID_STA_SortName_EPG, WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//Epg���ݱ�����ʾ��
	GUI_ENC_API_SetValue(ID_STAInfoText_EpgInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STAInfoTitle_EpgInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STAInfoTitle_EpgInfo, WDT_STA_SV_FGC, COLOR_WHITE, 0);


//EpgԤԼ��Ŀ�ĳ�ͻ��ʾ��
	GUI_ENC_API_SetValue(ID_STATitle_ConflictInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_ConflictInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STATitle_ConflictInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);


//Epg�в���ԤԼ��ʾ��
	GUI_ENC_API_SetValue(ID_STA_EpgReserveInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_EpgReserveInfo, WDT_STA_SV_FGC, COLOR_WHITE, 0);


//ԤԼ��Ŀ�б�
	GUI_ENC_API_SetValue(ID_STA_Note_ReserveList ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Title_ReserveList ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_LBX_ReserveList, WDT_LBX_SV_BKSTYLE, STYLE_ReserveBk_W640_304,0);
	GUI_ENC_API_SetValue(ID_STA_Title_ReserveList, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	
//ԤԼ��Ŀ�б���ʾ��
	GUI_ENC_API_SetValue(ID_STA_ReserveInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_ReserveInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ReserveInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_ReserveInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Title_TimeOut, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_TimeOut, WDT_STA_SV_FGC, COLOR_WHITE, 0);


//ϵͳ��ʾ
	GUI_ENC_API_SetValue(ID_STA_Menu_SysInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_SysInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);


//��Ϸ���
	GUI_ENC_API_SetValue(ID_MNU_Game, WDT_MNU_SV_ITEM_INTERVAL, 50,0);
	GUI_ENC_API_SetValue(ID_MNU_Game, WDT_MNU_SV_DATA, (BU32)GameMenu,sizeof(GameMenu)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_MNU_Game, WDT_MNU_SV_BKSTYLE, STYLE_MenuBk_W700_H480,0);
	GUI_ENC_API_SetValue(ID_MNU_Game,WDT_MNU_SV_BKSTYLE_POS,0,0);
	GUI_ENC_API_SetValue(ID_MNU_Game,WDT_MNU_SV_TITLE_POSITION,62,70);
	GUI_ENC_API_SetValue(ID_MNU_Game,WDT_MNU_SV_TITLE_RANGE,200,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Game,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Game,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Game,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Game,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//����˹����
	GUI_ENC_API_SetValue(ID_STA_Russia_Russia,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Russia,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Russia,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Russia,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Russia_Russia,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Russia,WDT_STA_SV_FGC,COLOR_WHITE,0);


//̰����
	GUI_ENC_API_SetValue(ID_STA_Snake_Snake,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Snake,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Snake,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Snake,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Snake_Snake,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Snake,WDT_STA_SV_FGC,COLOR_WHITE,0);


//������
	GUI_ENC_API_SetValue(ID_STA_Box_Box,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Box,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Box,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Box,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Box_Box,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Box,WDT_STA_SV_FGC,COLOR_WHITE,0);


//ϵͳ����
	GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_ITEM_INTERVAL, 39,0);
	GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_DATA, (BU32)SystemSetMenu,sizeof(SystemSetMenu)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_BKSTYLE, STYLE_MenuBk_W700_H480,0);
	GUI_ENC_API_SetValue(ID_MNU_SysSet,WDT_MNU_SV_BKSTYLE_POS,0,0);
	GUI_ENC_API_SetValue(ID_MNU_SysSet,WDT_MNU_SV_TITLE_POSITION,62,70);
	GUI_ENC_API_SetValue(ID_MNU_SysSet,WDT_MNU_SV_TITLE_RANGE,200,0);
	GUI_ENC_API_SetValue(ID_STA_Time_SysSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_SysSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_SysSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_SysSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_Enable(ID_STA_Event1_SysSet, 0);
	GUI_ENC_API_SetValue(ID_MNU_SysSet,WDT_MNU_SV_EXIT_OR_RECALL,0,0);


//ʱ������
	GUI_ENC_API_SetValue(ID_STA_Title_TimeZoneSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_CBX_TimeZone_TimeZoneSet, WDT_CBX_SV_DATA, (BU32)cbx_TimeZone, 2);
	GUI_ENC_API_SetValue(ID_CBX_TimeZone_TimeZoneSet, WDT_CBX_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_TimeZoneSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_TimeZoneSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_InNumber_TimeZoneSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_CBX_CurDate_TimeZoneSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_CurTime_TimeZoneSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_TimeZoneTitle_TimeZoneSet, WDT_CBX_SV_LABEL, RS_TimeZone, 0);
	GUI_ENC_API_SetValue(ID_CBX_CurDateTitle_TimeZoneSet, WDT_CBX_SV_LABEL, RS_Date, 0);
	GUI_ENC_API_SetValue(ID_CBX_CurTimeTitle_TimeZoneSet, WDT_CBX_SV_LABEL, RS_CurrentTime, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//������Ϣ
	GUI_ENC_API_SetValue(ID_STA_StbInfo_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_CBX_SoftNum_StbInfo, WDT_CBX_SV_LABEL, RS_SWVer, 0);
	GUI_ENC_API_SetValue(ID_CBX_HardNum_StbInfo, WDT_CBX_SV_LABEL, RS_HWVer, 0);
	GUI_ENC_API_SetValue(ID_CBX_CardNum_StbInfo, WDT_CBX_SV_LABEL, RS_CardNum, 0);
	GUI_ENC_API_SetValue(ID_CBX_StbType_StbInfo, WDT_CBX_SV_LABEL, RS_StbType, 0);
	GUI_ENC_API_SetValue(ID_CBX_Chip_StbInfo,    WDT_CBX_SV_LABEL, RS_Chip, 0);
	GUI_ENC_API_SetValue(ID_CBX_DataRadio_StbInfo, WDT_CBX_SV_LABEL, RS_BroadCast, 0);
	GUI_ENC_API_SetValue(ID_CBX_CaInfo_StbInfo, WDT_CBX_SV_LABEL, RS_CAINFO, 0);
	GUI_ENC_API_SetValue(ID_CBX_FlashSize_StbInfo, WDT_CBX_SV_LABEL, RS_FLASHSIZE, 0);
	GUI_ENC_API_SetValue(ID_CBX_SDRamSize_StbInfo, WDT_CBX_SV_LABEL, RS_SDRAMSIZE, 0);
	GUI_ENC_API_SetValue(ID_STA_Line0_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line4_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line5_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line6_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line7_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line8_StbInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line0_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line1_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line2_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line3_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line4_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line5_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line6_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line7_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Line8_StbInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Time_StbInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);



//�ָ���������
	GUI_ENC_API_SetValue(ID_STA_Default_Default,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Attention_Default,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_Default,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_Default,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_Default,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Attention_Default,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_Default,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_Default,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_Default,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Default,WDT_STA_SV_FGC,COLOR_WHITE,0);


//��Ϣ���
	GUI_ENC_API_SetValue(ID_STA_Title_ProgState,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Title_ProgState,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line6_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line7_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line8_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line9_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line10_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line11_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line12_ProgState,WDT_STA_SV_BGC,COLOR_BK_S,0);

//�źż��
    GUI_ENC_API_SetValue(ID_STA_Title_SignalCheck,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_STA_Title_SignalCheck,WDT_STA_SV_FGC, COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Illuminate_SignalCheck,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//����Ƶ����
	GUI_ENC_API_SetValue(ID_STA_AVSet_AVSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_AVSet_AVSet,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet, WDT_CBX_SV_DATA, (BU32)cbx_Mix, 10);
	GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet, WDT_CBX_SV_DATA, (BU32)cbx_Track, 4);
	GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet, WDT_CBX_SV_DATA, (BU32)cbx_Aspect, 2);
	GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet, WDT_CBX_SV_DATA, (BU32)cbx_TvMode, 3);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet, WDT_CBX_SV_DATA, (BU32)cbx_videoout, 2);
	GUI_ENC_API_SetValue(ID_CBX_TransparenceTitle_AvSet, WDT_CBX_SV_LABEL, RS_Audio_Diaphaneity, 0);
	GUI_ENC_API_SetValue(ID_CBX_TrackSetTitle_AVSet, WDT_CBX_SV_LABEL, RS_Audio_AudioSetting, 0);
	GUI_ENC_API_SetValue(ID_CBX_DisplayFormatTitle_AVSet, WDT_CBX_SV_LABEL, RS_Audio_TVForamt, 0);
	GUI_ENC_API_SetValue(ID_CBX_TVModeTitle_AVSet, WDT_CBX_SV_LABEL, RS_Audio_TVMode, 0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOutTitle_AVSet, WDT_CBX_SV_LABEL, RS_Audio_TVOutput, 0);


//ϵͳ����(��������)
	GUI_ENC_API_SetValue(ID_STA_Title_SysUpdate,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_SysUpdate,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_SysUpdate,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_SysUpdate,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_SysUpdate,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_SysUpdate,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Ok_SysUpdate,	WDT_STA_SV_FGC,COLOR_WHITE,0);
    GUI_ENC_API_SetValue(ID_CBX_QAMMode_SysUpdate, WDT_CBX_SV_DATA, (BU32)cbx_QAM, 5);
	GUI_ENC_API_SetValue(ID_CBX_PID_SysUpdate, WDT_CBX_SV_LABEL, RS_PID, 0);
	GUI_ENC_API_SetValue(ID_CBX_TableID_SysUpdate, WDT_CBX_SV_LABEL, RS_TableID, 0);
	GUI_ENC_API_SetValue(ID_CBX_Fre_SysUpdate, WDT_CBX_SV_LABEL, RS_Freq, 0);
	GUI_ENC_API_SetValue(ID_CBX_SymbolRate_SysUpdate, WDT_CBX_SV_LABEL, RS_SymbolRate, 0);
	GUI_ENC_API_SetValue(ID_CBX_QAMModeTitle_SysUpdate, WDT_CBX_SV_LABEL, RS_QAMMode, 0);
	GUI_ENC_API_SetValue(ID_TBX_PID_SysUpdate,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_TableID_SysUpdate,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_QAMMode_SysUpdate,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
//	GUI_ENC_API_Enable(ID_TBX_PID_SysUpdate, 0);
//	GUI_ENC_API_Enable(ID_TBX_TableID_SysUpdate, 0)	;


//�����������޸�
	GUI_ENC_API_SetValue(ID_STA_Title_PasswordSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_PasswordSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_CBX_Old_PasswordSet, WDT_CBX_SV_LABEL, RS_OldPasswd, 0);
	GUI_ENC_API_SetValue(ID_CBX_New_PasswordSet, WDT_CBX_SV_LABEL, RS_NewPasswd, 0);
	GUI_ENC_API_SetValue(ID_CBX_Check_PasswordSet, WDT_CBX_SV_LABEL, RS_CheckPasswd, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//�����������޸ĳɹ�
	GUI_ENC_API_SetValue(ID_STA_Title_PasswordSetInfo ,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Title_PasswordSetInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_PasswordSetInfo ,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//��������
	GUI_ENC_API_SetValue(ID_STA_Title_LanguageSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_CBX_LangChoice_LanguageSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
#ifdef SYS_CONFIG_LANG_ENG
	GUI_ENC_API_SetValue(ID_CBX_LangChoice_LanguageSet, WDT_CBX_SV_DATA, (BU32)cbx_Lang, 1);
#else
	GUI_ENC_API_SetValue(ID_CBX_LangChoice_LanguageSet, WDT_CBX_SV_DATA, (BU32)cbx_Lang, 2);
#endif
	GUI_ENC_API_SetValue(ID_STA_Time_LanguageSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_CBX_LangChoiceTitle_LanguageSet, WDT_CBX_SV_LABEL, RS_Language, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//��������
    GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet, WDT_CBX_SV_DATA, (BU32)cbx_SwitchItem, 2);
    GUI_ENC_API_SetValue(ID_CBX_AutoIP_NetSet, WDT_CBX_SV_DATA, (BU32)cbx_AutoIPDNS, 2);
	GUI_ENC_API_SetValue(ID_CBX_AutoDNS_NetSet, WDT_CBX_SV_DATA, (BU32)cbx_AutoIPDNS, 2);

	GUI_ENC_API_SetValue(ID_STA_Title_NetSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_NetSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_NetSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_InNumber_NetSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	
	GUI_ENC_API_SetValue(ID_CBX_SwitchItemTitle_NetSet, WDT_CBX_SV_LABEL, RS_NetSwitch, 0);
	GUI_ENC_API_SetValue(ID_CBX_AutoIPTitle_NetSet, WDT_CBX_SV_LABEL, RS_AutoIP, 0);
	GUI_ENC_API_SetValue(ID_CBX_IPAddressTitle_NetSet, WDT_CBX_SV_LABEL, RS_IPAddr, 0);
	GUI_ENC_API_SetValue(ID_CBX_SubnetworkTitle_NetSet, WDT_CBX_SV_LABEL, RS_SubNetMark, 0);
	GUI_ENC_API_SetValue(ID_CBX_GatewayIPTitle_NetSet, WDT_CBX_SV_LABEL, RS_GatewayIP, 0);
	GUI_ENC_API_SetValue(ID_CBX_AutoDNSTitle_NetSet, WDT_CBX_SV_LABEL, RS_AutoDNS, 0);
	GUI_ENC_API_SetValue(ID_CBX_FirstChoseDNSTitle_NetSet, WDT_CBX_SV_LABEL, RS_FirstChoseDNS, 0);
	GUI_ENC_API_SetValue(ID_CBX_ReserveDNSTitle_NetSet, WDT_CBX_SV_LABEL, RS_ReserveDNS, 0);
	GUI_ENC_API_SetValue(ID_CBX_FirstpageAddrTitle_NetSet, WDT_CBX_SV_LABEL, RS_FirstPageAddr, 0);
	
	GUI_ENC_API_SetValue(ID_STA_CardState_NetSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_NetSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_NetSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//��������
	GUI_ENC_API_SetValue(ID_MNU_Ca, WDT_MNU_SV_ITEM_INTERVAL, 39,0);
	GUI_ENC_API_SetValue(ID_MNU_Ca, WDT_MNU_SV_DATA, (BU32)CaMenu,sizeof(CaMenu)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_MNU_Ca, WDT_MNU_SV_BKSTYLE, STYLE_MenuBk_W700_H480,0);
	GUI_ENC_API_SetValue(ID_MNU_Ca,WDT_MNU_SV_BKSTYLE_POS,0,0);
	GUI_ENC_API_SetValue(ID_MNU_Ca,WDT_MNU_SV_TITLE_POSITION,62,70);
	GUI_ENC_API_SetValue(ID_MNU_Ca,WDT_MNU_SV_TITLE_RANGE,200,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Ca,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Ca,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Ca,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Ca,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_MNU_Ca,WDT_MNU_SV_EXIT_OR_RECALL,0,0);

//PIN�����ʾ
	GUI_ENC_API_SetValue(ID_STA_Title_PinNote,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Title_PinNote,	WDT_STA_SV_FGC,COLOR_WHITE,0);


//��״̬
	GUI_ENC_API_SetValue(ID_STA_Title_CardState,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_CardState,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Title_CardState,	WDT_STA_SV_FGC,COLOR_WHITE,0);


//��������
	GUI_ENC_API_SetValue(ID_STA_Title_Rating,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Rating,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_Rating,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_InNumber_Rating,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Ok_Rating,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_CBX_Rating, WDT_CBX_SV_LABEL, RS_EnterPin, 0);
	GUI_ENC_API_SetValue(ID_CBX_Value_Rating, WDT_CBX_SV_LABEL, RS_RateLevel, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Rating,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Rating,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Rating,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//CA��������
	GUI_ENC_API_SetValue(ID_STA_Title_PinSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_PinSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_PinSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_InNumber_PinSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Ok_PinSet,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_CBX_Old_PinSet, WDT_CBX_SV_LABEL, RS_OldPasswd, 0);
	GUI_ENC_API_SetValue(ID_CBX_New_PinSet, WDT_CBX_SV_LABEL, RS_NewPasswd, 0);
	GUI_ENC_API_SetValue(ID_CBX_Check_PinSet, WDT_CBX_SV_LABEL, RS_CheckPasswd, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_PinSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_PinSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_PinSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//���ù���ʱ��
	GUI_ENC_API_SetValue(ID_STA_Title_WorkTime,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_WorkTime,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_WorkTime,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_InNumber_WorkTime,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Ok_WorkTime,	WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_CBX_PIN_WorkTime, WDT_CBX_SV_LABEL, RS_EnterPin, 0);
	GUI_ENC_API_SetValue(ID_CBX_StartTime_WorkTime, WDT_CBX_SV_LABEL, RS_StartTime, 0);
	GUI_ENC_API_SetValue(ID_CBX_EndTime_WorkTime, WDT_CBX_SV_LABEL, RS_EndTime, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);


//�������
	GUI_ENC_API_SetValue(ID_STA_Msg_CardPair,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_CardPair,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_CardPair,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Msg_CardPair,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time_CardPair,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Exit_CardPair,WDT_STA_SV_FGC,COLOR_WHITE,0);


	
//��Ч�˵�
	GUI_ENC_API_SetValue(ID_MNU_Effects, WDT_MNU_SV_DATA, (BU32)EffectsMenu,sizeof(EffectsMenu)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_MNU_Effects, WDT_MNU_SV_ITEM_INTERVAL, 46,0);
	GUI_ENC_API_SetValue(ID_MNU_Effects, WDT_MNU_SV_BKSTYLE, STYLE_MenuBk_W700_H480,0);
	GUI_ENC_API_SetValue(ID_MNU_Effects,WDT_MNU_SV_BKSTYLE_POS,0,0);
	GUI_ENC_API_SetValue(ID_MNU_Effects,WDT_MNU_SV_TITLE_POSITION,62,70);
	GUI_ENC_API_SetValue(ID_MNU_Effects,WDT_MNU_SV_TITLE_RANGE,200,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Effects,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Effects,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Effects,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_Effects,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_EnterMenu_Effects,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_Effects,WDT_STA_SV_FGC,COLOR_WHITE,0);

//��Ч��ʾ��
	GUI_ENC_API_SetValue(ID_STA_Menu_EffectsInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Menu_EffectsInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);



//�ʼ�
    GUI_ENC_API_SetValue(ID_STA_Time_ReadMail,WDT_STA_SV_FGC,COLOR_WHITE,0);
    GUI_ENC_API_SetValue(ID_STA_MailInfo_ReadMail, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
    GUI_ENC_API_SetValue(ID_EDT_ReadMail, WDT_EDT_SV_SCROLL_BGS, COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_2_ReadMail,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_ReadMail,WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Page_ReadMail,WDT_STA_SV_BGC,COLOR_BK_S,0);	
	GUI_ENC_API_SetValue(ID_STA_MailInfo_Mail, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Note_Mail, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Notetitle_Mail, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Notetitle_Mail,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time1_Mail,WDT_STA_SV_FGC,COLOR_WHITE,0);

	GUI_ENC_API_SetValue(ID_STA_Menu_ICInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Exit_ICInfo,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time_ICInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
	GUI_ENC_API_SetValue(ID_STA_Menu_GetServicer, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Note_GetServicer, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Note_GetServicer,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_GetServicer, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_GetServicer,WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_OK_GetServicer,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Exit_GetServicer,WDT_STA_SV_FGC,COLOR_WHITE,0);

	GUI_ENC_API_SetValue(ID_STA_Menu_EntitleInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Text_EntitleInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_UD_EntitleInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Exit_EntitleInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time_EntitleInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
	GUI_ENC_API_SetValue(ID_STA_Menu_ePurse, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Text_ePurse, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

	GUI_ENC_API_SetValue(ID_STA_Exit_ePurse,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time_ePurse,WDT_STA_SV_FGC,COLOR_WHITE,0);

	GUI_ENC_API_SetValue(ID_STA_Menu_IPPInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Text_IPPInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_UD_IPPInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Exit_IPPInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time_IPPInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
	GUI_ENC_API_SetValue(ID_STA_Servicer_ACList, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Exit_ACList,WDT_STA_SV_FGC,COLOR_WHITE,0);	
	GUI_ENC_API_SetValue(ID_STA_Text_ACList, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_ACList,WDT_STA_SV_FGC,COLOR_WHITE,0);

	GUI_ENC_API_SetValue(ID_STA_ReserveList_DetitleChkNums, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_note_DetitleChkNums, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_CancelPre_DetitleChkNums,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_DeleteAll_DetitleChkNums,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_ExitMenu_DetitleChkNums,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Time_DetitleChkNums,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
	GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

	GUI_ENC_API_SetValue(ID_STA_Title_MasterSlaveManage, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Note_MasterSlaveManage, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Note_MasterSlaveManage,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlaveManage, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_MasterSlaveManage,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_OK_MasterSlaveManage,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Exit_MasterSlaveManage,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
	GUI_ENC_API_SetValue(ID_STA_MasterSlave, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_OK_MasterSlave,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Exit_MasterSlave,WDT_STA_SV_FGC,COLOR_WHITE,0);

//�ֶ�¼��
	GUI_ENC_API_SetValue(ID_LBP_Prog_ManualRec,WDT_LBP_SV_STYLE,(BU32)lbpa_ProgList,2);
	GUI_ENC_API_SetValue(ID_LBP_Prog_ManualRec,WDT_LBP_SV_DATAX,24,0);
	GUI_ENC_API_SetValue(ID_CBX_StartTime_ManualRec, WDT_CBX_SV_LABEL, RS_StartTime, 0);
	GUI_ENC_API_SetValue(ID_CBX_EndTime_ManualRec, WDT_CBX_SV_LABEL, RS_EndTime, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_ManualRec, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

//ʱ��
	GUI_ENC_API_SetValue(ID_STA_TimeShift_StartTime, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_TimeShift_StartTime, WDT_STA_SV_FGC, COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_STA_TimeShift_StartTime,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_TimeShift_EndTime, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_TimeShift_EndTime, WDT_STA_SV_FGC, COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_STA_TimeShift_EndTime,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_HelpTitle_TimeShift, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_HelpTitle_TimeShift, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_PauseStr_TimeShift, WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PlayStr_TimeShift, WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
		GUI_ENC_API_SetValue(ID_VOI_TimeShift,WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);

	GUI_ENC_API_SetValue(ID_VOI_TimeShift,WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
//�����µ�IP����
	GUI_ENC_API_SetValue(ID_STA_PreserveTitle,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PreserveTitle,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_PreserveTitle,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_PreserveTitle,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_PreserveTitle,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_PreserveTitle,WDT_STA_SV_BGC,COLOR_BK_S,0);

//����IP������Ϣ��ʾ
	GUI_ENC_API_SetValue(ID_STA_Prompt,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Prompt,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_Prompt,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_Prompt,WDT_STA_SV_BGC,COLOR_BK_S,0);
//Nvod��Ƶ�㲥
	GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_SCROLL_BGS, (BU32)COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_TEXT_BKCOLOR, (BU32)COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_SCROLL_CYCLE, 200, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_Nvod, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_LR_Nvod, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_EventDetail_Nvod, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_ReserveList_Nvod, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_Nvod, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_EventListTitle_Nvod, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_EventPlayTimeTitle_Nvod, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_MovieNameContent_Nvod, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_MovieTimeLengContent_Nvod, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Nvod,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Nvod,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_Nvod,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//NvodPlayȫ������
	GUI_ENC_API_SetValue(ID_VOI_NvodPlay, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
	GUI_ENC_API_SetValue(ID_VOI_NvodPlay, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CHN_NvodPlay, WDT_CHN_SV_DATA, (BU32)ProgNumber, 10);
	GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_NvodPlay, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_NvodPlay, WDT_STA_SV_FGC, COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_BGC, COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_NvodPlay, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_NvodPlay,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Time_NvodPlay,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_ChnInfoTitle_NvodPlay, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_ChnInfoTitle_NvodPlay, WDT_STA_SV_FGC, COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_EventDetai_Title_NvodPlay, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_EventDetai_Title_NvodPlay, WDT_STA_SV_FGC, COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_EventDetail_MovieName_NvodPlay, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_EDT_NvodPlay, WDT_EDT_SV_SCROLL_BGS, (BU32)COLOR_BK_S, 0);

//NvodPlayȫ�������˳���ʾ��
	GUI_ENC_API_SetValue(ID_STA_Line1_NvodPlayInfo, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_NvodPlayInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_NvodPlayInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_NvodPlayInfo, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

//����Nvodʱ�ȴ�����
	GUI_ENC_API_SetValue(ID_STA_Line1_NvodWait, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_NvodWait, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_NvodWait, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);


//ι����ĸ��
	GUI_ENC_API_SetValue(ID_STA_Text_Feeding,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Text_Feeding,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Title_Feeding,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Title_Feeding,WDT_STA_SV_FGC,COLOR_WHITE,0);
	
//ɾ������Ȩȷ����
	GUI_ENC_API_SetValue(ID_STA_DelDetitle,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_DelDetitle,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_DelDetitle,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_DelDetitle,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_DelDetitle,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_DelDetitle,WDT_STA_SV_BGC,COLOR_BK_S,0);

//��ѯ����Ȩȷ����
	GUI_ENC_API_SetValue(ID_STA_notetitle_DetitleChkNums,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_STA_notetitle_DetitleChkNums,WDT_STA_SV_FGC,COLOR_WHITE,0);

//��ѯ�û�����
	GUI_ENC_API_SetValue(ID_STA_Title_ACList,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_STA_Title_ACList,WDT_STA_SV_FGC,COLOR_WHITE,0);

//����Ǯ����Ϣ
	GUI_ENC_API_SetValue(ID_STA_Title_ePurse,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_STA_Title_ePurse,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line0_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line5_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line6_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line7_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line8_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line9_ePurse,WDT_STA_SV_BGC,COLOR_BK_S,0);


//IPP������ѯ
	
	GUI_ENC_API_SetValue(ID_STA_Line0_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line4_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line5_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line6_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
    GUI_ENC_API_SetValue(ID_STA_Line7_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line8_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line9_IPPInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
		

//ɾ����ǰ�ʼ�
	GUI_ENC_API_SetValue(ID_STA_DelCurrent,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_DelCurrent,	WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent,WDT_STA_SV_BGC,COLOR_BK_S,0);

//��ѯ��Ȩ��Ϣ
	GUI_ENC_API_SetValue(ID_STA_Title_EntitleInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_STA_Title_EntitleInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Line0_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line4_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line5_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line6_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line7_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line8_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line9_EntitleInfo,WDT_STA_SV_BGC,COLOR_BK_S,0);

//�����ĸ��

	GUI_ENC_API_SetValue(ID_STA_Time_MasterSlave,WDT_STA_SV_FGC,COLOR_WHITE,0);

//��ѯIPP��Ϣ
	GUI_ENC_API_SetValue(ID_STA_Title_IPPInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    GUI_ENC_API_SetValue(ID_STA_Title_IPPInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);

//�ļ����
    GUI_ENC_API_SetValue(ID_STA_FileScan_UsbManager, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
    GUI_ENC_API_SetValue(ID_STA_Time_UsbManager,     WDT_STA_SV_FGC,   COLOR_WHITE,  0);
    GUI_ENC_API_SetValue(ID_STA_Line1_UsbManager,    WDT_STA_SV_BGC,   COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_UsbManager,    WDT_STA_SV_BGC,   COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_STYLE, (BU32)lbpa_FileList, 4);
	GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_DATAX, 40, 0);
    GUI_ENC_API_SetValue(ID_STA_Enter_UsbManager,     WDT_STA_SV_FGC,   COLOR_WHITE,  0);
    GUI_ENC_API_SetValue(ID_STA_Return_UsbManager,     WDT_STA_SV_FGC,   COLOR_WHITE,  0);
    GUI_ENC_API_SetValue(ID_STA_Delete_UsbManager,     WDT_STA_SV_FGC,   COLOR_WHITE,  0);

//Pvr�����б�
    GUI_ENC_API_SetValue(ID_STA_FileList_PvrList, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
    GUI_ENC_API_SetValue(ID_STA_Time_PvrList,     WDT_STA_SV_FGC,   COLOR_WHITE,  0);
    GUI_ENC_API_SetValue(ID_STA_Line1_PvrList,    WDT_STA_SV_BGC,   COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_PvrList,    WDT_STA_SV_BGC,   COLOR_INFOBK, 0);
	GUI_ENC_API_SetValue(ID_LBP_FileList_PvrList, WDT_LBP_SV_DATAX, 20, 0);
    GUI_ENC_API_SetValue(ID_STA_Enter_PvrList,    WDT_STA_SV_FGC,   COLOR_WHITE,  0);
    GUI_ENC_API_SetValue(ID_STA_Return_PvrList,   WDT_STA_SV_FGC,   COLOR_WHITE,  0);
	GUI_ENC_API_SetValue(ID_STA_AllFile_PvrList,  WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

//Pvrȫ������
	GUI_ENC_API_SetValue(ID_VOI_PvrFullScreen, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
    GUI_ENC_API_SetValue(ID_VOI_PvrFullScreen, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);


//�����˵�
	GUI_ENC_API_SetValue(ID_CBX_Led_FactoryMenu, WDT_CBX_SV_LABEL, RS_DetectLed, 0);
	GUI_ENC_API_SetValue(ID_CBX_E2P_FactoryMenu, WDT_CBX_SV_LABEL, RS_DetectE2p, 0);
	GUI_ENC_API_SetValue(ID_CBX_ProResume_FactoryMenu, WDT_CBX_SV_LABEL, RS_ProduceResume, 0);
	GUI_ENC_API_SetValue(ID_CBX_SN_FactoryMenu, WDT_CBX_SV_LABEL, RS_InputSn, 0);

	GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory, WDT_CBX_SV_DATA, (BU32)led_detect, 1);
	GUI_ENC_API_SetValue(ID_CBX_E2p_Factory, WDT_CBX_SV_DATA, (BU32)e2p_normal, 1);
	GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory, WDT_CBX_SV_DATA, (BU32)push_enter, 1);
//	GUI_ENC_API_SetValue(ID_CBX_InputSN_Factory, WDT_CBX_SV_LABEL, RS_PushEnter, 0);

	GUI_ENC_API_SetValue(ID_STA_Title_FactoryMenu, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
//	GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory, WDT_CBX_SV_ALIGN, WDT_CBX_ALIGN_CENTER, 0);
//	GUI_ENC_API_SetValue(ID_CBX_E2p_Factory, WDT_CBX_SV_ALIGN, WDT_CBX_ALIGN_CENTER, 0);
//	GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory, WDT_CBX_SV_ALIGN, WDT_CBX_ALIGN_CENTER, 0);
//	GUI_ENC_API_SetValue(ID_CBX_InputSN_Factory, WDT_CBX_SV_ALIGN, WDT_CBX_ALIGN_CENTER, 0);

//	GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory, WDT_CBX_SV_LABEL_X, 60, 0);
//	GUI_ENC_API_SetValue(ID_CBX_E2p_Factory, WDT_CBX_SV_LABEL_X, 60, 0);
//	GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory, WDT_CBX_SV_LABEL_X, 60, 0);
//	GUI_ENC_API_SetValue(ID_CBX_InputSN_Factory, WDT_CBX_SV_LABEL_X, 60, 0);


	//����ID����
	GUI_ENC_API_SetValue(ID_CBX_AutoRegionTitle_RegionSet, WDT_CBX_SV_LABEL, RS_AutoRegionID, 0);
	GUI_ENC_API_SetValue(ID_CBX_RegionIDTitle_RegionSet, WDT_CBX_SV_LABEL, RS_RegionID, 0);
	GUI_ENC_API_SetValue(ID_CBX_AutoRegion_RegionSet, WDT_CBX_SV_DATA, (BU32)cbx_OnOff, 2);
	GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_RegionSet,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_Title_RegionSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	
//stbid setting
	GUI_ENC_API_SetValue(ID_STA_Title_StbIDSetting, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_AreaIndicatorCode, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_ManufactureCode, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_StbType, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_HwVersion, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_StbIndicatorType, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_MacAddress, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_StbFactoryIndicator, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	
	GUI_ENC_API_SetValue(ID_STA_AreaIndicatorCode, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_ManufactureCode, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_StbType, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_HwVersion, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_StbIndicatorType, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_MacAddress, WDT_STA_SV_FGC, COLOR_WHITE, 0);	
	GUI_ENC_API_SetValue(ID_STA_StbFactoryIndicator, WDT_STA_SV_FGC, COLOR_WHITE, 0);

	GUI_ENC_API_SetValue(ID_CBX_AreaIndicatorCode_StbIDSetting, WDT_CBX_SV_LABEL, RS_AreaIndicatorCode, 0);
	GUI_ENC_API_SetValue(ID_CBX_ManufactureCode_StbIDSetting, WDT_CBX_SV_LABEL, RS_ManufactureCode, 0);
	GUI_ENC_API_SetValue(ID_CBX_StbFactoryIndicator_StbIDSetting, WDT_CBX_SV_LABEL, RS_StbFactoryIndicator, 0);
	GUI_ENC_API_SetValue(ID_CBX_StbType_StbIDSetting, WDT_CBX_SV_LABEL, RS_StbType, 0);
	GUI_ENC_API_SetValue(ID_CBX_HwVersion_StbIDSetting, WDT_CBX_SV_LABEL, RS_HWVer, 0);
	GUI_ENC_API_SetValue(ID_CBX_StbIndicatorType_StbIDSetting, WDT_CBX_SV_LABEL, RS_StbIndicatorType, 0);
	GUI_ENC_API_SetValue(ID_CBX_MacAddress_StbIDSetting, WDT_CBX_SV_LABEL, RS_MACAddress, 0);

//ʹ�ò���ָ��
    GUI_ENC_API_SetValue(ID_STA_Title_OperationGuide, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
    GUI_ENC_API_SetValue(ID_STA_Time_OperationGuide, WDT_STA_SV_FGC, COLOR_WHITE, 0);

//�������
    GUI_ENC_API_SetValue(ID_CBX_TVMode_OutPutSet, WDT_CBX_SV_DATA, (BU32)cbx_TvMode, 3);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_OutPutSet, WDT_CBX_SV_DATA, (BU32)cbx_videoout, 2);
	GUI_ENC_API_SetValue(ID_CBX_TVModeTitle_OutPutSet, WDT_CBX_SV_LABEL, RS_Audio_TVMode, 0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOutTitle_OutPutSet, WDT_CBX_SV_LABEL, RS_Audio_TVOutput, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_OutPutSet, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_OutPutSet, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//��ʾ����
	
	GUI_ENC_API_SetValue(ID_CBX_VolumeSaveTitle_DisplaySet, WDT_CBX_SV_LABEL, RS_VOLUMESAVE, 0);
    GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet, WDT_CBX_SV_DATA, (BU32)cbx_volumesave, 2);
    GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet, WDT_CBX_SV_DATA, (BU32)cbx_TvMode, 3);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet, WDT_CBX_SV_DATA, (BU32)cbx_videoout, 2);
	GUI_ENC_API_SetValue(ID_CBX_TVModeTitle_DisplaySet, WDT_CBX_SV_LABEL, RS_Audio_TVMode, 0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOutTitle_DisplaySet, WDT_CBX_SV_LABEL, RS_Audio_TVOutput, 0);
    GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet, WDT_CBX_SV_DATA, (BU32)cbx_Mix, 10);
	GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet, WDT_CBX_SV_DATA, (BU32)cbx_Aspect, 2);
	GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet, WDT_CBX_SV_DATA, (BU32)cbx_InfoTime, 5);
	GUI_ENC_API_SetValue(ID_CBX_TransTitle_DisplaySet, WDT_CBX_SV_LABEL, RS_Audio_Diaphaneity, 0);
	GUI_ENC_API_SetValue(ID_CBX_DisplayScaleTitle_DisplaySet, WDT_CBX_SV_LABEL, RS_DisplayScale, 0);
	GUI_ENC_API_SetValue(ID_CBX_InfoTimeTitle_DisplaySet, WDT_CBX_SV_LABEL, RS_OSDTime, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_DisplaySet, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_DisplaySet, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//Ƶ������
    GUI_ENC_API_SetValue(ID_CBX_Track_ChannelSet, WDT_CBX_SV_DATA, (BU32)cbx_Track, 4);
    GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet, WDT_CBX_SV_DATA, (BU32)cbx_Offset, 31);
	GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet, WDT_CBX_SV_DATA, (BU32)cbx_ChannelSet, 2);
	GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet, WDT_CBX_SV_DATA, (BU32)cbx_ChannelSet, 2);
	GUI_ENC_API_SetValue(ID_CBX_TrackTitle_ChannelSet, WDT_CBX_SV_LABEL, RS_Track, 0);
	GUI_ENC_API_SetValue(ID_CBX_OffSetTitle_ChannelSet, WDT_CBX_SV_LABEL, RS_Offset, 0);
	GUI_ENC_API_SetValue(ID_CBX_ChannelSkipTitle_ChannelSet, WDT_CBX_SV_LABEL, RS_ChannelSkip, 0);
	GUI_ENC_API_SetValue(ID_CBX_ChannelFavTitle_ChannelSet, WDT_CBX_SV_LABEL, RS_ChannelFavSet, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_ChannelSet, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_ChannelSet, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//Ƶ�����ñ�����ʾ��
    GUI_ENC_API_SetValue(ID_STA_Title_ChannleSetInfo,WDT_STA_SV_FGC,COLOR_WHITE,0);
    GUI_ENC_API_SetValue(ID_STA_Title_ChannleSetInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Content_ChannelSetInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//��ĸ����
    GUI_ENC_API_SetValue(ID_CBX_MenuTitle_ParentControl, WDT_CBX_SV_LABEL, RS_Menu, 0);
    GUI_ENC_API_SetValue(ID_CBX_ChannelTitle_ParentControl, WDT_CBX_SV_LABEL, RS_Channel, 0);
	GUI_ENC_API_SetValue(ID_CBX_BootTitle_ParentControl, WDT_CBX_SV_LABEL, RS_Boot, 0);
    
	GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl, WDT_CBX_SV_DATA, (BU32)cbx_ParentControl, 2);
	GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl, WDT_CBX_SV_DATA, (BU32)cbx_ParentControl, 2);
	GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl, WDT_CBX_SV_DATA, (BU32)cbx_ParentControl, 2);
	GUI_ENC_API_SetValue(ID_STA_Title_ParentControl, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_Time_ParentControl, WDT_STA_SV_FGC, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//�����˵�
	GUI_ENC_API_SetValue(ID_MNU_FacMenu, WDT_MNU_SV_DATA, (BU32)FactoryMenu,sizeof(FactoryMenu)/sizeof(BU32));
	GUI_ENC_API_SetValue(ID_MNU_FacMenu, WDT_MNU_SV_ITEM_INTERVAL, 46,0);
	GUI_ENC_API_SetValue(ID_MNU_FacMenu, WDT_MNU_SV_BKSTYLE, STYLE_MenuBk_W700_H480,0);
	GUI_ENC_API_SetValue(ID_MNU_FacMenu, WDT_MNU_SV_BKSTYLE_POS,0,0);
	GUI_ENC_API_SetValue(ID_MNU_FacMenu, WDT_MNU_SV_TITLE_POSITION,62,70);
	GUI_ENC_API_SetValue(ID_MNU_FacMenu, WDT_MNU_SV_TITLE_RANGE,200,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_FacMenu,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_FacMenu,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_FacMenu,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_FacMenu,WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_MNU_FacMenu,WDT_MNU_SV_EXIT_OR_RECALL,0,0);

//Ƶ������
    GUI_ENC_API_SetValue(ID_CBX_MainFreqTitle_FreqSet, WDT_CBX_SV_LABEL, RS_MainFreq, 0);
    GUI_ENC_API_SetValue(ID_CBX_ADFreqTitle_FreqSet, WDT_CBX_SV_LABEL, RS_ADFreq, 0);
	GUI_ENC_API_SetValue(ID_CBX_NvodFreqTitle_FreqSet, WDT_CBX_SV_LABEL, RS_NvodFreq, 0);
	GUI_ENC_API_SetValue(ID_CBX_BroadCastFreq_FreqSet, WDT_CBX_SV_LABEL, RS_BROADFreq, 0);
	GUI_ENC_API_SetValue(ID_CBX_StockFreqTitle_FreqSet, WDT_CBX_SV_LABEL, RS_StockFreq, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_FreqSet, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

//���к�
    GUI_ENC_API_SetValue(ID_CBX_ManufactureTitle_Sequence, WDT_CBX_SV_LABEL, RS_ManufacNum, 0);
    GUI_ENC_API_SetValue(ID_CBX_StbTypeTitle_Sequence, WDT_CBX_SV_LABEL, RS_StbType, 0);
	GUI_ENC_API_SetValue(ID_CBX_ProductyearTitle_Sequence, WDT_CBX_SV_LABEL, RS_ProductYear, 0);
	GUI_ENC_API_SetValue(ID_CBX_ProductweekTitle_Sequence, WDT_CBX_SV_LABEL, RS_ProductWeek, 0);
	GUI_ENC_API_SetValue(ID_CBX_SequenceTitle_Sequence, WDT_CBX_SV_LABEL, RS_Sequence, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_Sequence, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

//��������
    GUI_ENC_API_SetValue(ID_CBX_PIDTitle_OTA, WDT_CBX_SV_LABEL, RS_PID, 0);
	GUI_ENC_API_SetValue(ID_CBX_FreqTitle_OTA, WDT_CBX_SV_LABEL, RS_Freq, 0);
	GUI_ENC_API_SetValue(ID_CBX_SymbTitle_OTA, WDT_CBX_SV_LABEL, RS_SymbolRate, 0);
	GUI_ENC_API_SetValue(ID_CBX_QAMModeTitle_OTA, WDT_CBX_SV_LABEL, RS_QAMMode, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_OTA, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);

	GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, WDT_CBX_SV_DATA, (BU32)cbx_QAM, 5);

//����д���к�
    GUI_ENC_API_SetValue(ID_CBX_DetectCardTitle_ComSequence, WDT_CBX_SV_LABEL, RS_CardDetected, 0);
    GUI_ENC_API_SetValue(ID_CBX_DetectE2Title_ComSequence, WDT_CBX_SV_LABEL, RS_DetectE2p, 0);
	GUI_ENC_API_SetValue(ID_CBX_DetectComTitle_ComSequence, WDT_CBX_SV_LABEL, RS_DetectCom, 0);
	GUI_ENC_API_SetValue(ID_CBX_EthSetTitle_ComSequence, WDT_CBX_SV_LABEL, RS_NetSet, 0);

	GUI_ENC_API_SetValue(ID_CBX_DetectCard_ComSequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_DetectE2_ComSequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_DetectCom_ComSequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_ComSequence, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_CBX_EthSet_ComSequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_EthSet_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_eth, 2);

//�ѹۿ�IPP��Ŀ��Ϣ
	GUI_ENC_API_SetValue(ID_STA_ViewedIPP,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
    	GUI_ENC_API_SetValue(ID_STA_Title_ViewedIPP,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Text_ViewedIPP,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_ViewedIPP,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ViewedIPP,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ViewedIPP,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ViewedIPP,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Prompt_ViewedIPP,WDT_STA_SV_BGC,COLOR_BK_S,0);

//ʵʱ����
	GUI_ENC_API_SetValue(ID_STA_RealtimeBook,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Booked_RealtimeBook,WDT_STA_SV_BGC,COLOR_BK_S,0);

//����/�˶�IPP��Ŀ
	GUI_ENC_API_SetValue(ID_STA_BookUnbook,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_TextTitle_BookUnbook,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Text_BookUnbook,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line1_BookUnbook,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_BookUnbook,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_BookUnbook,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line4_BookUnbook,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

//��֤�û���PIN��
	GUI_ENC_API_SetValue(ID_STA_Title_VerifyPin,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Note_VerifyPin,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Note_VerifyPin,WDT_STA_SV_BGC,COLOR_BK_S,0);

//��Ƶ������
	GUI_ENC_API_SetValue(ID_STA_Title_SingleSlot,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_SingleSlot,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
//��������
	GUI_ENC_API_SetValue(ID_EDT_DoubleHelp, WDT_EDT_SV_SCROLL_BGS, COLOR_BK_S, 0);
	GUI_ENC_API_SetValue(ID_STA_Title_DoubleHelp,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp,WDT_STA_SV_BGC,COLOR_INFOBK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp,WDT_STA_SV_FGC,COLOR_BLACK,0);
	GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_Time_DoubleHelp, WDT_STA_SV_FGC, COLOR_WHITE, 0);

	// epg��ϸ��Ϣ
   	GUI_ENC_API_SetValue(ID_STA_Title_EPGDetail,   WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
   	GUI_ENC_API_SetValue(ID_STA_Title_EPGDetail,   WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_EventName_EPGDetail,  WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_EDT_Detail_EPGDetail, WDT_EDT_SV_SCROLL_BGS, (BU32)COLOR_BK_S, 0);

	//enreachʱ�ƽ���
	
	GUI_ENC_API_SetValue(ID_STA_PP_PLAYTIME,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_PAUSE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PLAYCHN,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_INBP,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE3,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

	GUI_ENC_API_SetValue(ID_STA_TVOD_HELP, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_TVOD_HELP, WDT_STA_SV_FGC, COLOR_WHITE, 0);

	GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
	GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);

	GUI_ENC_API_SetValue(ID_STA_Line10_FULLTOTVOD,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line11_FULLTOTVOD,WDT_STA_SV_BGC,COLOR_BK_S,0);
	GUI_ENC_API_SetValue(ID_STA_Line12_FULLTOTVOD,WDT_STA_SV_BGC,COLOR_BK_S,0);



	//�������
	GUI_ENC_API_SetValue(ID_STA_Title_PhotoFrame, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_PhotoIndex_PhotoFrame, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_PhotoTotal_PhotoFrame, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_STA_PhotoName_PhotoFrame, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_CBX_Order_PhotoFrame, WDT_CBX_SV_DATA, (BU32)cbx_order, 2);
	GUI_ENC_API_SetValue(ID_CBX_Effect_PhotoFrame, WDT_CBX_SV_DATA, (BU32)cbx_effect, 11);
	GUI_ENC_API_SetValue(ID_CBX_Seconds_PhotoFrame, WDT_CBX_SV_DATA, (BU32)cbx_seconds, 4);
	GUI_ENC_API_SetValue(ID_CBX_FullScreen_PhotoFrame, WDT_CBX_SV_DATA, (BU32)cbx_fullscreen, 1);
	GUI_ENC_API_SetValue(ID_STA_Exit_PhotoFrame, WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_OK_PhotoFrame, WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_LR_PhotoFrame, WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_7, WDT_STA_SV_FGC,COLOR_WHITE,0);


	
//������
	GUI_ENC_API_SetValue(ID_STA_Title_Player, WDT_STA_SV_FGC,COLOR_WHITE,0);
    GUI_ENC_API_SetValue(ID_STA_Title_Player, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
    GUI_ENC_API_SetValue(ID_STA_DisplaySetTitle_Player, WDT_STA_SV_FGC, COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_DisplaySetTitle_Player, WDT_STA_SV_ALIGN, WDT_STA_ALIGN_CENTER, 0);
	GUI_ENC_API_SetValue(ID_LBP_DisplaySet_Player, WDT_LBP_SV_STYLE, (BU32)&lbpa_DisplaySet, 1);
	GUI_ENC_API_SetValue(ID_LBP_DisplaySet_Player, WDT_LBP_SV_DATAX, 34, 0);
	GUI_ENC_API_SetValue(ID_STA_DisplaySetIcon_Player, WDT_STA_SV_FGC,COLOR_WHITE,0);
	GUI_ENC_API_SetValue(ID_STA_DisplaySetText_Player, WDT_STA_SV_FGC,COLOR_WHITE,0);

	

	
	

	

}


