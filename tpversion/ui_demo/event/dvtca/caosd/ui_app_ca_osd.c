#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define TIMER_PERIODS 100
static BU32 print_time_start = 0;
static BU08 __glb_caosd_Finger_ID[11];
static BU32 __glb_caosd_printingProgID=0;

typedef struct
{
	BU32 full;
	BU32 half;
}ui_app_osd_s;
static ui_app_osd_s ui_app_osd={0,0};
UI_APP_CAOsd_s __glb_caosd_show={0,30,520/*540*/,30/*0*/,0,0,0,0,0,0,100,0,0,0,0,0,1,0,0};

#define MSG_LEN_IN_LINE 60
char __glb_caosd_show_osdMesage[210+60+1];
char __glb_caosd_round_show[MSG_LEN_IN_LINE+1];
BU32 __glb_caosd_show_osd_i=0;

char __glb_caosd_show_osdMesageTop[210+60+1];
char __glb_caosd_round_showTop[MSG_LEN_IN_LINE+1];
BU32 __glb_caosd_show_osd_Top_i=0;
BU08 g_caosd_show_osd_flag = 0;
BU08 g_caosd_show_osd_top_flag = 0;
extern ui_detitle_received_info_s ui_detitle_received_info;
static BU32 offset_x = 0;

void UI_APP_CAOsd_ShowOsdMsg(const char * szOSD)//滚动文字_底部显示
{	
	BU32 strlen,i; 
	__glb_caosd_show.bu32ShowInfo = 1;

  	APP_Printf("__glb_caosd_show.bu32ShowInfo = 1, szOSD=%s start osd show\n",szOSD);
	memset(__glb_caosd_show_osdMesage, 0, 271);//clear
//	memset(__glb_caosd_round_show, 0, MSG_LEN_IN_LINE+1);//clear
//	memset( __glb_caosd_show_osdMesage,  32, MSG_LEN_IN_LINE);
	GUI_API_Strcpy(__glb_caosd_show_osdMesage/*+60*/, (PU08)szOSD);
//	GUI_API_Strcpy(__glb_caosd_show_osdMesage/*+60*/, "testtesttesttesttesttest");
	strlen = GUI_API_Strlen(__glb_caosd_show_osdMesage);
	for(i=0;i<strlen;i++)
	{
		if((__glb_caosd_show_osdMesage[i] >= 0)&&(__glb_caosd_show_osdMesage[i] <= 31 ))
		{
			__glb_caosd_show_osdMesage[i] = ' ';
		}
	}

	__glb_caosd_show_osdMesage[strlen] = ' ';
	__glb_caosd_show_osdMesage[strlen+1] = 0;

	__glb_caosd_show.szOSD_len = GUI_API_GetTextLenght (__glb_caosd_show_osdMesage, GUI_API_Strlen (__glb_caosd_show_osdMesage))/*GUI_API_Strlen(__glb_caosd_show_osdMesage)*/;
	__glb_caosd_show.time_out = 0;//计时清零 
	__glb_caosd_show_osd_i = 0;
}

void UI_APP_CAOsd_ShowOsdMsgTop(const char * szOSD)//滚动文字_顶部显示
{	
	BU32 strlen,i; 
	__glb_caosd_show.bu32ShowInfo_top= 1;

  	APP_Printf("__glb_caosd_show.bu32ShowInfo_top = 1, start osd show\n");
	memset(__glb_caosd_show_osdMesageTop, 0, 271);//clear
	GUI_API_Strcpy(__glb_caosd_show_osdMesageTop/*+60*/, (PU08)szOSD);
	strlen = GUI_API_Strlen(__glb_caosd_show_osdMesageTop);
	for(i=0;i<strlen;i++)
	{
		if((__glb_caosd_show_osdMesageTop[i] >= 0)&&(__glb_caosd_show_osdMesageTop[i] <=31 ))
		{
			__glb_caosd_show_osdMesageTop[i] = ' ';
		}
	}
	__glb_caosd_show_osdMesageTop[strlen] = ' ';
	__glb_caosd_show_osdMesageTop[strlen+1] = 0;

	__glb_caosd_show.szOSD_len = GUI_API_GetTextLenght (__glb_caosd_show_osdMesageTop, GUI_API_Strlen (__glb_caosd_show_osdMesageTop))/*GUI_API_Strlen(__glb_caosd_show_osdMesage)*/;
	__glb_caosd_show.time_out_top = 0;//计时清零 
	__glb_caosd_show_osd_Top_i = 0;
}

void UI_APP_CAOsd_ClearOsdMsg(BU32 byStyle)//clear滚动文字
{
	switch(byStyle)
	{
		case 1:
			__glb_caosd_show.bu32ShowInfo_top = 0;
			g_caosd_show_osd_top_flag = 0;
			GUI_ENC_API_Update(ID_ODC_CaOsd_ChInfo, 5);
			break;
		case 2:
			__glb_caosd_show.bu32ShowInfo = 0;
			g_caosd_show_osd_flag = 0;
			GUI_ENC_API_Update(ID_ODC_CaOsd_ChInfo, 6);
			break;

		default:
			break;
	}
}
void UI_APP_CAOsd_ShowFingerPrinting(BU32 dwCardID, BU16 wDuration)//指纹显示
{
	/* 数码视讯显示指纹
	   dwCardID :指纹显示的卡号
	   wDuration:要显示指纹的持续时间，单位秒 
	*/	
	ChnDataBase_ChnInfo_S chnInfo;
	
	//FYF_API_printf("show print time %d sec\n",wDuration);
	
  	APP_Printf("__glb_caosd_show.bu32ShowID = 1, start print show\n");
	__glb_caosd_show.bu32ShowID = 1;
	__glb_caosd_show.bu32IDFirst = 1;
	__glb_caosd_show.bu32CardID = dwCardID;
	__glb_caosd_show.bu32IDCnt = wDuration*(1000/TIMER_PERIODS);
/*
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
	{
		return;
	}
	__glb_caosd_printingProgID = chnInfo.m_ProgId;
*/	
	print_time_start =	FYF_API_time_ms();
	__glb_caosd_show.printDuration = wDuration;
	
	offset_x = GUI_API_Random(600);
	/*避免指纹前三位不会显示不出来*/
	if(offset_x < 30)
	{
		offset_x = 30;
	}
}





BU32 UI_APP_API_IsOsdShow(void)
{
	if(ui_app_osd.full == 1 || ui_app_osd.half == 1)
	{
		return 1;
	}
	return 0;
}

void _CA_OSD_ShowPrinting(BU32 hdc)
{
	int len = 0;
	BU32 time_end = 0;
	
	time_end =	FYF_API_time_ms();

	GUI_API_Memset(__glb_caosd_Finger_ID, 0, 11);
	
	if((time_end - print_time_start) < __glb_caosd_show.printDuration*1000)
	{
		//FYF_API_printf("print %d s\n",time_end - print_time_start);
		if(UI_APP_API_GetChnInfoCurState() == TRACKSET_STATE)
		{
			//return;
			__glb_caosd_show.bu32IDYPos = 65;
		}
		else
		{
			__glb_caosd_show.bu32IDYPos = 65;
		}
		sprintf(__glb_caosd_Finger_ID,"%d",__glb_caosd_show.bu32CardID);
		len = GUI_API_Strlen(__glb_caosd_Finger_ID);
		
		GUI_API_FillRect(hdc, __glb_caosd_show.old_offset_x, __glb_caosd_show.bu32IDYPos, __glb_caosd_show.old_len*11+10, 30, COLOR_TRANS);	
	//	if(__glb_caosd_show.bu32IDCnt > 1)
		{  
			GUI_API_FillRect(hdc, offset_x, __glb_caosd_show.bu32IDYPos, len*11+10, 30, COLOR_INFOBK);
			GUI_API_TextOut(hdc, offset_x+4, __glb_caosd_show.bu32IDYPos+4, __glb_caosd_Finger_ID, len, COLOR_BLACK, COLOR_WHITE, 1);
			__glb_caosd_show.old_offset_x = offset_x;
			__glb_caosd_show.old_len = len;
	
			__glb_caosd_show.bu32IDFirst = 0;
		}

	}
	else
	{		
		GUI_API_FillRect(hdc, __glb_caosd_show.old_offset_x, __glb_caosd_show.bu32IDYPos, __glb_caosd_show.old_len*11+10, 30, COLOR_TRANS);	
		//FYF_API_printf("print duration %d s\n",time_end - print_time_start);
		__glb_caosd_show.bu32ShowID = 0;
	}
}

void _CA_OSD_ShowMessage(BU32 hdc)
{
	BU32 Len=0;
	BU32 second = 0;
	unsigned char first_char;
	GUI_Rect_s disprect;
	static BU32 time_start = 0,time_end = 0;

	if(!__glb_caosd_show_osd_i)
	{
		time_start = FYF_API_time_ms();
	}
	
	if(__glb_caosd_show_osd_i >= __glb_caosd_show.szOSD_len+660)
	{
		GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos, 720, 30, COLOR_TRANS);
		__glb_caosd_show_osd_i = 0;
		time_end = FYF_API_time_ms();
		//second = __glb_caosd_show.time_out / 1000;
		second = (time_end - time_start)/1000;
		FYF_API_printf("show osd time %d sec\n",second);
		FYF_API_ca_set_value(FYF_CA_SV_MSG_OVER,second,0);//通知CA已滚动一遍
		__glb_caosd_show.bu32ShowInfo = 0;
		g_caosd_show_osd_flag = 0;
		return;
	}
	else
	{
		if(__glb_caosd_show_osd_i<660)
		{
			disprect.xpos =690-__glb_caosd_show_osd_i; 
			disprect.ypos = __glb_caosd_show.bu32YPos;
			disprect.width =__glb_caosd_show_osd_i;
			disprect.height = 30;
			GUI_API_TextSetValue(TEXT_SVL_RECT,&disprect,0); 
			if(!__glb_caosd_show_osd_i || g_caosd_show_osd_flag)
			{
				GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos, 720, 30, COLOR_INFOBK);
				g_caosd_show_osd_flag = 0;
			}
			GUI_API_TextOut(hdc,690-__glb_caosd_show_osd_i,__glb_caosd_show.bu32YPos+3,__glb_caosd_show_osdMesage,GUI_API_Strlen(__glb_caosd_show_osdMesage),COLOR_BLACK,COLOR_INFOBK,0); 
		}
		else
		{
			disprect.xpos =30; 
			disprect.ypos = __glb_caosd_show.bu32YPos;
			disprect.width =660;
			disprect.height = 30;			
			GUI_API_TextSetValue(TEXT_SVL_RECT,&disprect,0); 
			GUI_API_TextSetValue(TEXT_SVL_PASSLEN,__glb_caosd_show_osd_i-660,0); 
			if(g_caosd_show_osd_flag)
			{
				GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos, 720, 30, COLOR_INFOBK);
				g_caosd_show_osd_flag = 0;
			}
			GUI_API_TextOut(hdc,30,__glb_caosd_show.bu32YPos+3,__glb_caosd_show_osdMesage,GUI_API_Strlen(__glb_caosd_show_osdMesage),COLOR_BLACK,COLOR_INFOBK,0); 
			
		}
		__glb_caosd_show_osd_i+=4;
	}
#if 0
	if(__glb_caosd_show_osd_i+MSG_LEN_IN_LINE >= __glb_caosd_show.szOSD_len)
	{
		Len = __glb_caosd_show.szOSD_len - __glb_caosd_show_osd_i;
	}
	else
	{
		Len = MSG_LEN_IN_LINE;
	}
	
	GUI_API_Memcpy(__glb_caosd_round_show, __glb_caosd_show_osdMesage+__glb_caosd_show_osd_i,Len);
	__glb_caosd_round_show[MSG_LEN_IN_LINE]=0;
	GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos, 720, 26, 136);
	GUI_API_TextOut(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos, __glb_caosd_round_show, Len, 0, 1, 1);

	first_char = __glb_caosd_round_show[0];

	if(first_char <= 127)
	{
		__glb_caosd_show_osd_i += 1;	
	}
	else if(first_char > 127)
	{
		__glb_caosd_show_osd_i += 2;	
	}
#endif
}

void _CA_OSD_ShowMessageTop(BU32 hdc)
{
	BU32 Len=0;
	unsigned char first_char;
	GUI_Rect_s disprect;
	
	if(__glb_caosd_show_osd_Top_i >= __glb_caosd_show.szOSD_len+660)
	{
	//	GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos_top, 720, 30, 255);
		__glb_caosd_show_osd_Top_i = 0;
	//	__glb_caosd_show.bu32ShowInfo_top= 0;
		return;
	}
	else
	{
		if(__glb_caosd_show_osd_Top_i<660)
		{
			disprect.xpos =690-__glb_caosd_show_osd_Top_i; 
			disprect.ypos = __glb_caosd_show.bu32YPos_top;
			disprect.width =__glb_caosd_show_osd_Top_i;
			disprect.height = 30;
			GUI_API_TextSetValue(TEXT_SVL_RECT,&disprect,0); 
			if(!__glb_caosd_show_osd_Top_i || g_caosd_show_osd_top_flag)
			{
				GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos_top, 720, 30, COLOR_INFOBK);
				g_caosd_show_osd_top_flag = 0;
			}
			GUI_API_TextOut(hdc,690-__glb_caosd_show_osd_Top_i,__glb_caosd_show.bu32YPos_top+3,__glb_caosd_show_osdMesageTop,GUI_API_Strlen(__glb_caosd_show_osdMesageTop),COLOR_BLACK,COLOR_INFOBK,0); 
		}
		else
		{
			disprect.xpos =30; 
			disprect.ypos = __glb_caosd_show.bu32YPos_top;
			disprect.width =660;
			disprect.height = 30;			
			GUI_API_TextSetValue(TEXT_SVL_RECT,&disprect,0); 
			GUI_API_TextSetValue(TEXT_SVL_PASSLEN,__glb_caosd_show_osd_Top_i-660,0); 
			if(g_caosd_show_osd_top_flag)
			{
				GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos_top, 720, 30, COLOR_INFOBK);
				g_caosd_show_osd_top_flag = 0;
			}
			GUI_API_TextOut(hdc,30,__glb_caosd_show.bu32YPos_top+3,__glb_caosd_show_osdMesageTop,GUI_API_Strlen(__glb_caosd_show_osdMesageTop),COLOR_BLACK,COLOR_INFOBK,0); 
			
		}
		__glb_caosd_show_osd_Top_i+=4;
	}
}

void UI_APP_CAOsd_ProgramInfo_Paint(BU32 hdc,BU32 lParam)
{



	switch(lParam)
	{
	case 1:
		break;
	case 2:
/*--------finger---------*/
		if(UI_APP_API_IsOsdShow ())
		{
			return;
		}
		_CA_OSD_ShowPrinting(hdc);
		break;
	case 3:
/*--------both finger & osd_msg---------*/
		_CA_OSD_ShowMessage(hdc);
		break;
	case 4:
/*--------osd_msg top---------*/
		_CA_OSD_ShowMessageTop(hdc);
		break;
	case 5:
		GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos_top, 720, 30, COLOR_TRANS);
		break;
	case 6:
		GUI_API_FillRect(hdc, __glb_caosd_show.bu32XPos, __glb_caosd_show.bu32YPos, 720, 30, COLOR_TRANS);
		break;
	default:
		break;
	}
}

void UI_APP_CAOsd_ProgramInfo_Timer(void)
{
	static BU08 cnt = 0;
	static BU08 mail_cnt = 0;
	if(UI_APP_GetMailFullState())
	{
		mail_cnt ++;
		if(mail_cnt == 1)
		{
			GUI_ENC_API_Update(ID_STA_HideMail_ChInfo ,1);	
		}
		else if(mail_cnt == 3)
		{
			GUI_ENC_API_Update(ID_STA_Mail_ChInfo ,1);	
		}
		if(mail_cnt >= 8)
		{
			mail_cnt = 0;
		}
	}
	else
	{
		mail_cnt = 0;
	}

	if(__glb_caosd_show.bu32ShowInfo)
	{
		GUI_ENC_API_Update(ID_ODC_CaOsd_ChInfo, 3);
		__glb_caosd_show.time_out = __glb_caosd_show.time_out + TIMER_PERIODS;//毫秒计时OSD左滚屏

	}

	if(__glb_caosd_show.bu32ShowInfo_top)
	{
		GUI_ENC_API_Update(ID_ODC_CaOsd_ChInfo, 4);
//		__glb_caosd_show.bu32ShowInfo_top = __glb_caosd_show.bu32ShowInfo_top + TIMER_PERIODS;//毫秒计时OSD左滚屏

	}

	if(__glb_caosd_show.bu32ShowID)
	{
		cnt ++;
		if(cnt == 10)
		{
			cnt =0;
			GUI_ENC_API_Update(ID_ODC_CaOsd_ChInfo, 2);
			if(__glb_caosd_show.bu32IDCnt)
			{
				__glb_caosd_show.bu32IDCnt--;
			}
		}
	}	

}




