#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define SANMING_VERSION
#define PHOTO_PAGE_NUM 9
#define COLOR_WHITE RGB(255, 255, 255)
#define COLOR_YELLOW RGB(255, 255, 0)// RGB(0, 0, 0)
#define COLOR_BLACK RGB(0, 0, 0)

static BU08 photo_total[10];
static BU08 photo_name[50];
static BU08 photo_index[10];
static BU32 g_photoframe_pagenum = 0;
static BU32 g_photoframe_curpage = 0;
static BU32 g_photoframe_restnum = 0;
static BU32 g_photoframe_curindex = 0;

static BU32 g_order_index = 0;
static BU32 g_effect_index = 0;
static BU32 g_seconds_index = 0;
static BU32 g_updateflag = 0;

static void PhotoFrame_IPI_DrawLine(BU32 hdc)
{
	BU32 i;
	for (i = 0; i < 2; i++)
	{
		GUI_API_DrawHLine(hdc, 0, (i+1)*FRAME_HEIGHT/3, FRAME_WIDTH, COLOR_WHITE);
		GUI_API_DrawVLine(hdc, (i+1)*FRAME_WIDTH/3, 0, FRAME_HEIGHT, COLOR_WHITE);
	}	
}
static void PhotoFrame_IPI_DrawRectangle(BU32 hdc, BU32 index, BU32 color)
{
	BU32 x, y;
	x = index % 3;
	if (index > 0)y = index / 3;
	else y = 0;
	y = y * (FRAME_HEIGHT / 3);
	x = x * (FRAME_WIDTH / 3);
	GUI_API_DrawRectangle(hdc, x + 3 , y + 3 , FRAME_WIDTH/3 - 6 , FRAME_HEIGHT/3 - 6, color);
	GUI_API_DrawRectangle(hdc, x + 4 , y + 4 , FRAME_WIDTH/3 - 8 , FRAME_HEIGHT/3 - 8, color);
}

void PhotoFrame_IPI_SetPageParam(void)
{
	BU32  count,curfile;
	count = APP_PictureDecoder_API_GetCount();
	APP_Printf("count = %d\n",count);
	g_photoframe_pagenum = (count +  PHOTO_PAGE_NUM -1) / PHOTO_PAGE_NUM;	
	g_photoframe_restnum = count % PHOTO_PAGE_NUM;
	g_photoframe_curpage= 0;
	if(g_photoframe_restnum == 0)
	{
		g_photoframe_restnum = PHOTO_PAGE_NUM;
	}
	
}

static void PhotoFrame_IPI_PrevPage()
{
	g_photoframe_curpage--;
	APP_PictureDecoder_API_InitRect();
	GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 1);	
}

static void PhotoFrame_IPI_NextPage()
{
	g_photoframe_curpage++;
	APP_PictureDecoder_API_InitRect();
	GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 1);	
}

static void PhotoFrame_IPI_ShowCurPage(void)
{
	Player_Mp3File_S mp3info;
	BU32 index,i;
	BU32 pagenum;

	if (g_photoframe_pagenum == 0)
	{
		return;
	}
	
	if (g_photoframe_curpage >= g_photoframe_pagenum)
	{
		g_photoframe_curpage = 0;
	}
	
	if (g_photoframe_curpage + 1 == g_photoframe_pagenum)
	{	
		pagenum = g_photoframe_restnum;
	}
	else
	{
		pagenum = PHOTO_PAGE_NUM;
	}
	
	for (i = 0; i < pagenum; i++)
	{	
		index = i + g_photoframe_curpage*PHOTO_PAGE_NUM;
		APP_PictureDecoder_API_ShowPicture(index);
	}	
	
	
}

static BS32 PhotoFrame_IPI_ChangePictureNameAndIndex()
{
	BU32 index = g_photoframe_curindex + g_photoframe_curpage * PHOTO_PAGE_NUM;

	APP_PictureDecoder_API_GetPicName(photo_name, index);
	sprintf(photo_index,"%d", index + 1);
	GUI_ENC_API_SetValue(ID_STA_PhotoName_PhotoFrame, WDT_STA_SV_STRING, (BU32)photo_name, 0);
	GUI_ENC_API_SetValue(ID_STA_PhotoIndex_PhotoFrame, WDT_STA_SV_STRING, (BU32)photo_index, 0);
	
}
void UI_APP_PhotoFrame_Enter(void)
{
	BU32 photocount;
	APP_EPG_API_StopGetEpg();
	GUI_ENC_API_SetFocus(ID_ODC_Frame_PhotoFrame);
	if(APP_OK != APP_PictureDecoder_API_Init())
	{
		APP_PRINT_ERROR("PICTUR DECODER      ERROR!!\n");
	}
	photocount = APP_PictureDecoder_API_GetCount();
	sprintf(photo_total, "%d", photocount);
	GUI_ENC_API_SetValue(ID_STA_PhotoTotal_PhotoFrame, WDT_STA_SV_STRING, (BU32)photo_total, 0);
	GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 1);
	PhotoFrame_IPI_SetPageParam();
	PhotoFrame_IPI_ChangePictureNameAndIndex();

	g_order_index = APP_PictureDecoder_API_GetOrder();
	g_effect_index = APP_PictureDecoder_API_GetEffect();
	g_seconds_index = APP_PictureDecoder_API_GetSeconds();
	
	GUI_ENC_API_SetValue(ID_CBX_Order_PhotoFrame,WDT_CBX_SV_INDEX,(BU32)g_order_index,0);
	GUI_ENC_API_SetValue(ID_CBX_Effect_PhotoFrame,WDT_CBX_SV_INDEX,(BU32)g_effect_index,0);
	GUI_ENC_API_SetValue(ID_CBX_Seconds_PhotoFrame,WDT_CBX_SV_INDEX,(BU32)g_seconds_index,0);
	
}
void UI_APP_PhotoFrame_End(void)
{
	
}
void UI_APP_PhotoFrame_Key(BU32 key)
{
	switch(key)
	{
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(ID_DLG_ProgManager, 1);
			break;
		defalut:
			break;
	}
	
}

void UI_APP_PhotoShowSet_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_SELECT:
		if (GUI_ENC_API_IsFocus(ID_CBX_FullScreen_PhotoFrame))
		{
			PictureShow_API_SetShowFlag(2);
			GUI_ENC_API_SwitchState(ID_DLG_PictureShow, 1);
			
			APP_PictureDecoder_API_Start(g_photoframe_curindex + g_photoframe_curpage * PHOTO_PAGE_NUM);
		}
		break;
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if (GUI_ENC_API_IsFocus(ID_CBX_Order_PhotoFrame))
		{
			GUI_ENC_API_GetValue(ID_CBX_Order_PhotoFrame,WDT_CBX_SV_INDEX,&g_order_index,0);
			APP_PictureDecoder_API_SetOrder(g_order_index);
	
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_Effect_PhotoFrame))
		{
			GUI_ENC_API_GetValue(ID_CBX_Effect_PhotoFrame,WDT_CBX_SV_INDEX,&g_effect_index,0);
			APP_PictureDecoder_API_SetEffect(g_effect_index);
	
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_Seconds_PhotoFrame))
		{
			GUI_ENC_API_GetValue(ID_CBX_Seconds_PhotoFrame,WDT_CBX_SV_INDEX,&g_seconds_index,0);
			APP_PictureDecoder_API_SetSeconds(g_seconds_index);
			
		}
		break;
	case GUI_VK_DOWN:
		if (GUI_ENC_API_IsFocus(ID_CBX_Order_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Effect_PhotoFrame);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_Effect_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Seconds_PhotoFrame);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_Seconds_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_FullScreen_PhotoFrame);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_FullScreen_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Order_PhotoFrame);
		}
		break;
	case GUI_VK_UP:
		if (GUI_ENC_API_IsFocus(ID_CBX_Order_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_FullScreen_PhotoFrame);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_Effect_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Order_PhotoFrame);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_Seconds_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Effect_PhotoFrame);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_FullScreen_PhotoFrame))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Seconds_PhotoFrame);
		}
		break;
#ifdef SANMING_VERSION
	case GUI_VK_F10:
#else
	case GUI_VK_F9:
#endif
		g_updateflag = 1;
		GUI_ENC_API_SetFocus(ID_ODC_Frame_PhotoFrame);
		g_updateflag = 0;
		GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 7);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		APP_Com_API_StopProg(0);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		break;
	default:
		break;
	}
	
}

void UI_APP_Frame_OnPaint(BU32 hdc,BU32 lparam)
{
	APP_Printf("lparam = %d\n", lparam);
	if (g_updateflag) return;
	switch(lparam)
	{
	case 1:
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_PhotoFrameBK2_W441_H330);
		PhotoFrame_IPI_DrawLine(hdc);
		PhotoFrame_IPI_ShowCurPage();
		PhotoFrame_IPI_DrawRectangle(hdc, 0, COLOR_YELLOW);
		g_photoframe_curindex = 0;
		break;
	case 2:
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex, COLOR_BLACK);
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex - 3, COLOR_YELLOW);
		g_photoframe_curindex -= 3;		
		break;
	case 3:
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex, COLOR_BLACK);
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex + 3, COLOR_YELLOW);
		g_photoframe_curindex += 3;
		break;
	case 4:
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex, COLOR_BLACK);
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex - 1, COLOR_YELLOW);
		g_photoframe_curindex--;		
		break;
	case 5:
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex, COLOR_BLACK);
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex + 1, COLOR_YELLOW);
		g_photoframe_curindex ++;
		break;
	case 6:
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex, COLOR_BLACK);
		break;
	case 7:
		PhotoFrame_IPI_DrawRectangle(hdc, g_photoframe_curindex, COLOR_YELLOW);
		break;
	case 8:
	case 9:
	case 10:
		break;
	default:
		break;
	}

	
}
void UI_APP_Frame_Onkey(BU32 key)
{
	BU08 flag = 0;
	APP_PRINT_INFO("%d %d %d\n", g_photoframe_curindex, g_photoframe_curpage, g_photoframe_restnum);
	switch(key)
	{
	case GUI_VK_SELECT:
		PictureShow_API_SetShowFlag(1);
		GUI_ENC_API_SwitchState(ID_DLG_PictureShow, 1);	
		APP_PictureDecoder_API_Start(g_photoframe_curindex + g_photoframe_curpage * PHOTO_PAGE_NUM);
		break;
	case GUI_VK_UP:
		if (g_photoframe_curindex >= 3)//向上
		{
			flag = 1;
			GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 2);	
		}
		else if (g_photoframe_curindex < 3 && g_photoframe_curpage != 0)//上一页
		{
			flag = 1;
			PhotoFrame_IPI_PrevPage();
		}
		break;
	case GUI_VK_DOWN:
		if (g_photoframe_curindex >= 6 && g_photoframe_curpage < g_photoframe_pagenum)
		{
			flag = 1;
			PhotoFrame_IPI_NextPage();
		}
		else if ((g_photoframe_curpage + 1 < g_photoframe_pagenum && g_photoframe_curindex + 3 < 9)
			|| (g_photoframe_curpage + 1== g_photoframe_pagenum && g_photoframe_curindex + 3 < g_photoframe_restnum))
		{
			flag = 1;
			GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 3);	
		}
		break;
	case GUI_VK_LEFT:
		if (g_photoframe_curindex != 0)
		{
			flag = 1;
			GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 4);	
		}
		else if (g_photoframe_curindex == 0 && g_photoframe_curpage != 0)
		{
			flag = 1;
			PhotoFrame_IPI_PrevPage();
		}
		break;
	case GUI_VK_RIGHT:
		if ((g_photoframe_curpage + 1 < g_photoframe_pagenum && g_photoframe_curindex + 1 < 9)
			|| (g_photoframe_curpage + 1 == g_photoframe_pagenum && g_photoframe_curindex + 1 < g_photoframe_restnum))
		{
			flag = 1;
			GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 5);	
		}
		else if (g_photoframe_curindex >= 8 && g_photoframe_curpage < g_photoframe_pagenum)
		{
			flag =1;
			PhotoFrame_IPI_NextPage();
		}
		break;
	case GUI_VK_P_UP:
		if (g_photoframe_curpage > 0)
		{
			flag = 1;
			PhotoFrame_IPI_PrevPage();
		}
	
		break;
	case GUI_VK_P_DOWN:
		if (g_photoframe_curpage + 1 < g_photoframe_pagenum)
		{
			flag = 1;
			PhotoFrame_IPI_NextPage();
		}
		break;
#ifdef SANMING_VERSION
	case GUI_VK_F10:
#else
	case GUI_VK_F9:
#endif
		GUI_ENC_API_SetFocus(ID_CBX_Order_PhotoFrame);
		GUI_ENC_API_Update(ID_ODC_Frame_PhotoFrame, 6);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		APP_Com_API_StopProg(0);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
	//	UI_APP_API_SetVolume();
		break;
	default:
		break;
	}

	if (flag)
	{
		PhotoFrame_IPI_ChangePictureNameAndIndex();
	}
}


