/*----------------------------------------------------------------------------
   File Name:ow_draw.c

   Description: 

   Version:REL_V1.0.0

   Copyright (c) 2006-2100 Triumph Corporation.  All rights reserved.
-----------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */

#include "ow_debug.h"


BU08 bConnectState = 0;
extern int optionSocket;
extern OW_PLAY_PARA VodPlayPara;
extern BU08 bPlayFromStart;

int OW_ParseSecToTimeStr(int secs,char* timeStr)
{
	char hourStr[3] = {0};
	char minStr[3] = {0};
	char secStr[3] = {0};
	BU08  hour = 0,min = 0,sec = 0;

	if (secs < 0)
	{
		OW_ERROR("secs < 0 \n");
		return FYF_ERR;
	}
	hour = secs/3600;
	secs%=3600;
	min = secs/60;
	secs%=60;
	sec = (BU08) secs;
	if(hour<10){
		sprintf(hourStr,"0%d",hour);
	}else{
		sprintf(hourStr,"%d",hour);
	}
	if(min<10){
		sprintf(minStr,"0%d",min);
	}else{
		sprintf(minStr,"%d",min);
	}
	if(sec<10){
		sprintf(secStr,"0%d",sec);
	}else{
		sprintf(secStr,"%d",sec);
	}

	sprintf(timeStr,"%s:%s:%s",hourStr,minStr,secStr);
	return FYF_OK;	
}

int OW_AddTime(BU16 *year,BU08 *month,BU08 *day,BU08 *hour,BU08 *min,BU08 *sec,BU08 perAdd)
{
	BU08 monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	BU08  bRunNian =FYF_FALSE;
	BU08 days = 0;

	//OW_DEBUG("OW_AddTime input year=%d,month =%d,day=%d,hour=%d,min=%d\n ",*year,*month,*day,*hour,*min);

	if(*sec+2<60)
	{
		*sec+=2;
	}
	else
	{
		*sec = 0;
	}

	if(*min+perAdd<60)//��δ���
	{
		*min+=perAdd;
	}
	else//��ʱ��λ
	{
		*min = *min+perAdd-60;
		if(*hour+1<24)//ʱδ���
		{
			*hour+=1;
		}
		else//�����λ
		{
			*hour=0;
			/******��鵱ǰ�·ݵ�������********/
			if(*month==2)//��ǰ��Ϊ2�·�
			{
				if(*year%4==0)
				{
					if(*year%100!=0)
					{
						bRunNian = FYF_TRUE;
					}
					else if(*year%400==0)
					{
						bRunNian = FYF_TRUE;
					}
				}
				if(bRunNian)//����
				{
					days = 29;
				}
				else
				{
					days = 28;
				}
			}
			else
			{
				days = monthDays[*month-1];
			}
			/***************************/
			if(*day+1<days)//��δ���
			{
				*day+=1;
			}
			else//���½�λ
			{
				*day = 1;
				if(*month+1<=12)//��δ���
				{
					*month+=1;
				}
				else//�����λ
				{
					*month = 1;
					*year+=1;
				}
			}
		}
	}
	//TP_Print_Debug(ow_main,ow_dos,"TP_OW_AddTime output year=%d,month =%d,day=%d,hour=%d,min=%d\n ",*year,*month,*day,*hour,*min);
	return FYF_OK;
}

int OW_SubTime(BU16 *year,BU08 *month,BU08 *day,BU08 *hour,BU08 *min,BU08 *sec,BU08 perSub)
{
	BU08  monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	BU08  bRunNian = FYF_FALSE;

	//OW_DEBUG("OW_SubTime input year=%d,month =%d,day=%d,hour=%d,min=%d\n ",*year,*month,*day,*hour,*min);
	if(*sec - 2 >= 0)
	{
		*sec-=2;
	}
	else
	{
		*sec = 59;
	}
	
	if(*min - perSub>=0)//���ӹ���
	{
		*min -=perSub;
	}
	else//��ʱ��λ
	{
		*min = 60+*min-perSub;
		if(*hour-1>=0)//ʱ����
		{
			*hour-=1;
		}
		else//�����λ
		{
			*hour = 23;
			if(*day-1>0)//�칻��
			{
				*day-=1;
			}
			else//���½�λ
			{
				/*****��ʱ����Ҫ�㵱ǰ�µ�ǰһ�����м���,��ǰһ����Ϊ2�·�,��Ҫ����ݿ��ǲ�������******/
				if(*month-1==2)//����ǰ����Ϊ���·�
				{
					if(*year%4==0)
					{
						if(*year%100!=0)
						{
							bRunNian = FYF_TRUE;
						}
						else if(*year%400==0)
						{
							bRunNian = FYF_TRUE;
						}
					}
					if(bRunNian)//����
					{
						*day = 29;
					}
					else
					{
						*day = 28;
					}
				}
				else
				{
					if(*month-1>0)
					{
						*day = monthDays[*month-2];
					}
					else//12�·�
					{
						*day = 31;
					}
				}
				/***************************/
				if(*month-1>0)//�·ݹ���
				{
					*month -=1;
				}
				else//�����λ
				{
					*month = 12;
					
					if(*year-1>0)
					{
						*year-=1;
					}
					else//12�·�
					{
						 *year = 0;
						 *month= 0;
						 *day= 0;
						 *hour= 0;
						 *min= 0;
					}
				}
			}
		}
	}
	//OW_DEBUG("OW_SubTime output year=%d,month =%d,day=%d,hour=%d,min=%d\n ",*year,*month,*day,*hour,*min);
	return FYF_OK;	
}

int OW_DrawConnect(void)
{	
	printf("OW_DrawConnect: ��������,���Ժ�...\n");
	
	GUI_ENC_API_SwitchState(ID_DLG_INFOCONNECT, 1);
	
#if 0		/*hqx:do it later*/
	U16 x = 210,y = 200;
	U16 width = 300,hight = 60;
	
	//TPGRAPH_DrawRectangle(x, y,width, hight, BLUECOLOR);
	TP_OW_DrawGIF2BMP(x , y,ow_BPZhun,&width,&hight);
	TPGRAPH_DrawTextInRegion(x, y+20, width, 25, "��������,���Ժ�...", JCENTER, BLACKCOLOR);
#endif	
	return FYF_OK;
}

int OW_DrawSerErr(void)				//
{
	if (GUI_ENC_API_GetCurState() == ID_DLG_VOD)
	{
		OW_ERROR("OW_DrawSerErr\n");
		
		GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "������æ,���Ժ�����", 0);
		
		GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK,1);
		GUI_ENC_API_Update(ID_STA_VOD_CHECKNET,1);

		FYF_API_thread_sleep(1500);
		
		GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK,1);
		GUI_ENC_API_Update(ID_STA_VOD_CHECKNET,1);

		
	}
	
	if (GUI_ENC_API_GetCurState() == ID_DLG_FULLTOTVOD)
	{
		OW_ERROR("OW_DrawSerErr22\n");
		
		GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "������æ,���Ժ�����", 0);
		GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
		GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET,1);
		
		FYF_API_thread_sleep(1500);
		
		GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
		GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET,1);
	}

	if (GUI_ENC_API_GetCurState() == ID_DLG_INFOCONNECT)
	{
		OW_ERROR("OW_DrawSerErr33\n");
		
		GUI_ENC_API_SetValue(ID_STA_INFOCONNECT, WDT_STA_SV_STRING, "������æ,���Ժ�����", 0);
		GUI_ENC_API_Update(ID_STA_INFOCONNECT_BAK,1);
		GUI_ENC_API_Update(ID_STA_INFOCONNECT,1);
	}

	
	if (GUI_ENC_API_GetCurState() == ID_DLG_Null)
	{
		OW_ERROR("OW_DrawSerErr\n");
		
		GUI_ENC_API_SetValue(ID_STA_NULL_CHECKNET, WDT_STA_SV_STRING, "������æ,���Ժ�����", 0);
		
		GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET_BAK, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_NULL_CHECKNET_BAK,1);
		GUI_ENC_API_Update(ID_STA_NULL_CHECKNET,1);
	}

	//TPGRAPH_SaveBmp(x, y, width, height);
	OW_DrawInfo("������æ,���Ժ�����", FYF_TRUE, FYF_FALSE, FYF_FALSE);
	//TPGRAPH_ReleaseBmp(x, y, width, height);
	return FYF_OK;
}

void OW_DrawSpeed(int speednum)
{

	printf("OW_DrawSpeed, speednum = %d\n", speednum);
#if 0
	char speedstring[4] = { 0 };
	char drawString[10] = { 0 };
	int speednumToStringLen = 0;
	BOOLEAN fu_tag = false;
	int x = 500, y = 120;

	//TPGXOBJ_Bitmap_t bitmap;
	if (speednum == 0)
	{
		TP_OW_DrawPP();
		return;
	}
	else if (speednum == 1)
	{
		TP_OW_ClearPP();
		return;
	}

	bitmap.ColorType = STGXOBJ_COLOR_TYPE_ARGB1555;

	TP_Print_Debug(ow_draw, ow_com, "============stb_draw_speed speednum =%d\n", speednum);

	bitmap.Width = 30;
	bitmap.Height = 25;
	bitmap.Data_p = (void *)Data_Bitmap_Speed;

	//iPanel_graphics_dprintf("dstX:%d  dstY:%d   srcW:%d   srcH:%d\n", dstX ,dstY ,srcW,srcH);

	TPGRAPH_DrawRectangle(x, y, 250, 150, ibackgroundColor);
	TPGRAPH_DrawBMP(x, y, bitmap);
	TPGRAPH_DrawBMP(x + 30, y, bitmap);
	TPGRAPH_DrawBMP(x + 60, y, bitmap);
	//TPGRAPH_DrawTextInRegion(30, 525, 120, 25, "���ӻؿ�", JCENTER, iRedColor);

	if (speednum < 0)
	{
		fu_tag = true;
		speednum *= (-1);
	}

	itoa((int)speednum, (char *)speedstring, 10);

	printf("stb_draw_speed speedstring =%s,speednum = %d\n", speedstring, speednum);

	if (fu_tag)
	{
		sprintf(drawString, "����-%s", speedstring);
		TPGRAPH_DrawTextInRegion(x - 7, y, 100, 23, drawString, JCENTER, iBlackColor);
	}
	else
	{
		sprintf(drawString, "���x%s", speedstring);
		TPGRAPH_DrawTextInRegion(x - 7, y, 100, 23, drawString, JCENTER, iBlackColor);
	}
#endif
}

int OW_DrawInfo(char *info,BU08 bClearSelf,BU08 bOnlyClear,BU08 bPressOut)
{
	printf("OW_DrawInfo: %s\n", info);

	return FYF_OK;
}


int OW_ClearConnect(void)
{
	printf("OW_ClearConnect.\n");
	
	//TPGRAPH_DrawRectangle(x, y,300,60, TRANSPARENTCOLOR);
	return FYF_OK;
}


int OW_DrawExit5(void)
{
	printf("�Ƿ�����ۿ�?\n");
	printf("�����������\n");
	return FYF_OK;
}


int OW_DrawBPZhun(void)
{
	printf("�ط�׼��	 �� ��������˳�\n");


	return FYF_OK;

}

int OW_DrawInBP()
{
	printf("����ؿ�==================\n");

	GUI_ENC_API_SwitchState(ID_DLG_FULLTOTVOD,1);
	
	return FYF_OK;

}
int OW_DrawInBPText()
{

	printf("���ӻؿ�\n");


	return FYF_OK;
}


/*****type 0:ֱ��״̬��   1:ʱ��״̬�� ***********/

int OW_DrawBPJump(int ms, int type)	//��ʱ����ת
{

}

int RTSP_CheckPlayErr(void)
{
#if 0
	int x = 160;
	int y = 200;
	U16 width = 0, hight = 0;

	if (NULL != strstr(recvs, "Invalid Range"))	//�ڿ���ʱ�����λ�ó�ʱ����ͣ�󳬳�ʱ�����
	{
		if (NULL != strstr(sends, "PLAY"))
		{
			TP_OW_DrawGIF2BMP(x + 100, y, ow_PP, &width, &hight);
			TPGRAPH_DrawTextInRegion(x + 100, y + 25, width, 25, "�ѳ���ʱ�����", JCENTER, BLACKCOLOR);
			TPGRAPH_DrawTextInRegion(x + 100, y + 55, width, 25, "��ʱ����㿪ʼ����", JCENTER, BLACKCOLOR);
			MILLI_DELAY(2000);
			TPGRAPH_DrawRectangle(x + 100, y, width, hight, TRANSPARENTCOLOR);
			owPlayPara.reqScale = 1;
			owPlayPara.bBOS = true;
			if (RTSP_CreatePLAY(optionSocket, &owPlayPara) == TP_FAIL)
			{
				TP_Print_Debug(ow_main, ow_dos, "TP_OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
			}
			else
			{
				TP_OW_PassBadFrame();
				return TP_SUCCESS;
			}

		}
	}
#endif	
	return FYF_OK;
}

