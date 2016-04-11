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

	if(*min+perAdd<60)//分未溢出
	{
		*min+=perAdd;
	}
	else//向时进位
	{
		*min = *min+perAdd-60;
		if(*hour+1<24)//时未溢出
		{
			*hour+=1;
		}
		else//向天进位
		{
			*hour=0;
			/******检查当前月份的总天数********/
			if(*month==2)//当前月为2月份
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
				if(bRunNian)//闰年
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
			if(*day+1<days)//日未溢出
			{
				*day+=1;
			}
			else//向月进位
			{
				*day = 1;
				if(*month+1<=12)//月未溢出
				{
					*month+=1;
				}
				else//向年进位
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
	
	if(*min - perSub>=0)//分钟够减
	{
		*min -=perSub;
	}
	else//向时借位
	{
		*min = 60+*min-perSub;
		if(*hour-1>=0)//时够减
		{
			*hour-=1;
		}
		else//向天借位
		{
			*hour = 23;
			if(*day-1>0)//天够减
			{
				*day-=1;
			}
			else//向月借位
			{
				/*****此时天数要算当前月的前一个月有几天,若前一个月为2月份,还要算年份看是不是闰年******/
				if(*month-1==2)//若果前个月为二月份
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
					if(bRunNian)//闰年
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
					else//12月份
					{
						*day = 31;
					}
				}
				/***************************/
				if(*month-1>0)//月份够减
				{
					*month -=1;
				}
				else//向年借位
				{
					*month = 12;
					
					if(*year-1>0)
					{
						*year-=1;
					}
					else//12月份
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
	printf("OW_DrawConnect: 正在连接,请稍后...\n");
	
	GUI_ENC_API_SwitchState(ID_DLG_INFOCONNECT, 1);
	
#if 0		/*hqx:do it later*/
	U16 x = 210,y = 200;
	U16 width = 300,hight = 60;
	
	//TPGRAPH_DrawRectangle(x, y,width, hight, BLUECOLOR);
	TP_OW_DrawGIF2BMP(x , y,ow_BPZhun,&width,&hight);
	TPGRAPH_DrawTextInRegion(x, y+20, width, 25, "正在连接,请稍后...", JCENTER, BLACKCOLOR);
#endif	
	return FYF_OK;
}

int OW_DrawSerErr(void)				//
{
	if (GUI_ENC_API_GetCurState() == ID_DLG_VOD)
	{
		OW_ERROR("OW_DrawSerErr\n");
		
		GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "服务器忙,请稍后再试", 0);
		
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
		
		GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "服务器忙,请稍后再试", 0);
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
		
		GUI_ENC_API_SetValue(ID_STA_INFOCONNECT, WDT_STA_SV_STRING, "服务器忙,请稍后再试", 0);
		GUI_ENC_API_Update(ID_STA_INFOCONNECT_BAK,1);
		GUI_ENC_API_Update(ID_STA_INFOCONNECT,1);
	}

	
	if (GUI_ENC_API_GetCurState() == ID_DLG_Null)
	{
		OW_ERROR("OW_DrawSerErr\n");
		
		GUI_ENC_API_SetValue(ID_STA_NULL_CHECKNET, WDT_STA_SV_STRING, "服务器忙,请稍后再试", 0);
		
		GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET_BAK, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_NULL_CHECKNET_BAK,1);
		GUI_ENC_API_Update(ID_STA_NULL_CHECKNET,1);
	}

	//TPGRAPH_SaveBmp(x, y, width, height);
	OW_DrawInfo("服务器忙,请稍后再试", FYF_TRUE, FYF_FALSE, FYF_FALSE);
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
	//TPGRAPH_DrawTextInRegion(30, 525, 120, 25, "电视回看", JCENTER, iRedColor);

	if (speednum < 0)
	{
		fu_tag = true;
		speednum *= (-1);
	}

	itoa((int)speednum, (char *)speedstring, 10);

	printf("stb_draw_speed speedstring =%s,speednum = %d\n", speedstring, speednum);

	if (fu_tag)
	{
		sprintf(drawString, "快退-%s", speedstring);
		TPGRAPH_DrawTextInRegion(x - 7, y, 100, 23, drawString, JCENTER, iBlackColor);
	}
	else
	{
		sprintf(drawString, "快进x%s", speedstring);
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
	printf("是否继续观看?\n");
	printf("按任意键继续\n");
	return FYF_OK;
}


int OW_DrawBPZhun(void)
{
	printf("回放准备	 秒 按任意键退出\n");


	return FYF_OK;

}

int OW_DrawInBP()
{
	printf("进入回看==================\n");

	GUI_ENC_API_SwitchState(ID_DLG_FULLTOTVOD,1);
	
	return FYF_OK;

}
int OW_DrawInBPText()
{

	printf("电视回看\n");


	return FYF_OK;
}


/*****type 0:直播状态下   1:时移状态下 ***********/

int OW_DrawBPJump(int ms, int type)	//画时间跳转
{

}

int RTSP_CheckPlayErr(void)
{
#if 0
	int x = 160;
	int y = 200;
	U16 width = 0, hight = 0;

	if (NULL != strstr(recvs, "Invalid Range"))	//在靠近时移起点位置长时间暂停后超出时移起点
	{
		if (NULL != strstr(sends, "PLAY"))
		{
			TP_OW_DrawGIF2BMP(x + 100, y, ow_PP, &width, &hight);
			TPGRAPH_DrawTextInRegion(x + 100, y + 25, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
			TPGRAPH_DrawTextInRegion(x + 100, y + 55, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
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

