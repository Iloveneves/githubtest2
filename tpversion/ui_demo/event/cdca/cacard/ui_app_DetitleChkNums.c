#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

static BU32 Detitle_Chk_Nums[FYF_CDCA_MAXNUM_DETITLE];
static BU32 Detitle_Chk_Nums_Count;
static BU32 Detitle_Chk_Delete;
extern BU08 g_ui_app_str[13][80];
static BU08 *g_ca_str2_p[5]=
{
	g_ui_app_str[6],
	g_ui_app_str[7],
	g_ui_app_str[8],
	g_ui_app_str[9],
	g_ui_app_str[10]
};

BU16* tvsid;
BU16* UI_APP_IPI_GetTvsId();

void DetitleChkNums_Enter(void)
{
	BS32 ret;
	BU32 index;

	tvsid = UI_APP_IPI_GetTvsId();
	GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &index, 0);
	Detitle_Chk_Nums_Count = 0;
	Detitle_Chk_Delete = 0;
	FYF_API_Memset (Detitle_Chk_Nums, 0, sizeof(Detitle_Chk_Nums));
	ret = FYF_API_ca_get_value (FYF_CA_SV_DETITLE_CHK_NUMS, (BU32 *)(&tvsid[index]), Detitle_Chk_Nums);
	if(FYF_CDCA_RC_OK == ret)
	{
		for(index = 0;index < FYF_CDCA_MAXNUM_DETITLE;index++)
		{
			if(Detitle_Chk_Nums[index])
			{
				sprintf (g_ca_str2_p[Detitle_Chk_Nums_Count], "%u", Detitle_Chk_Nums[index]);
				Detitle_Chk_Nums_Count++;
			}
		}
		if(Detitle_Chk_Nums_Count)
		{
			GUI_ENC_API_SetVisible (ID_STA_notebk_DetitleChkNums, WS_DISABLE);
			GUI_ENC_API_SetVisible (ID_STA_notetitle_DetitleChkNums, WS_DISABLE);
			GUI_ENC_API_SetVisible (ID_STA_note_DetitleChkNums, WS_DISABLE);
			GUI_ENC_API_SetValueUnPaint (ID_LBX_DetitleChkNums, WDT_LBX_SV_DATA, (BU32)g_ca_str2_p, Detitle_Chk_Nums_Count);
			GUI_ENC_API_GetValue(ID_LBX_DetitleChkNums, WDT_LBX_SV_INDEX, &index, 0);
			if(index>=Detitle_Chk_Nums_Count)index=0;
			GUI_ENC_API_SetValue(ID_LBX_DetitleChkNums, WDT_LBX_SV_INDEX, index, 0);
		}
		else
		{
			GUI_ENC_API_SetValueUnPaint (ID_LBX_DetitleChkNums, WDT_LBX_SV_ALLNUM, 0, 0);
			GUI_ENC_API_SetVisible (ID_STA_notebk_DetitleChkNums, WS_VISIBLE);
			GUI_ENC_API_SetVisible (ID_STA_notetitle_DetitleChkNums, WS_VISIBLE);
			GUI_ENC_API_SetVisible (ID_STA_note_DetitleChkNums, WS_VISIBLE);
			GUI_ENC_API_SetValueUnPaint (ID_STA_note_DetitleChkNums, WDT_STA_SV_DATAID, RS_No_Detitle_Ack_Code, 0);
		}
	}
	else if(FYF_CDCA_RC_CARD_INVALID == ret)
	{
		GUI_ENC_API_SetValueUnPaint (ID_LBX_DetitleChkNums, WDT_LBX_SV_ALLNUM, 0, 0);
		GUI_ENC_API_SetVisible (ID_STA_notebk_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_notetitle_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_note_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint (ID_STA_note_DetitleChkNums, WDT_STA_SV_DATAID, RS_CardPair_Invalid, 0);
		Detitle_Chk_Nums_Count = 0;
	}
	else if(FYF_CDCA_RC_POINTER_INVALID == ret)
	{
		GUI_ENC_API_SetValueUnPaint (ID_LBX_DetitleChkNums, WDT_LBX_SV_ALLNUM, 0, 0);
		GUI_ENC_API_SetVisible (ID_STA_notebk_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_notetitle_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_note_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint (ID_STA_note_DetitleChkNums, WDT_STA_SV_DATAID, RS_Pointer_Invalid, 0);
		Detitle_Chk_Nums_Count = 0;
	}
	else if(FYF_CDCA_RC_DATA_NOT_FIND == ret)
	{
		GUI_ENC_API_SetValueUnPaint (ID_LBX_DetitleChkNums, WDT_LBX_SV_ALLNUM, 0, 0);
		GUI_ENC_API_SetVisible (ID_STA_notebk_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_notetitle_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_note_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint (ID_STA_note_DetitleChkNums, WDT_STA_SV_DATAID, RS_Data_Not_Find, 0);		
		Detitle_Chk_Nums_Count = 0;
	}
	else if(FYF_CDCA_RC_UNKNOWN == ret)
	{
		GUI_ENC_API_SetValueUnPaint (ID_LBX_DetitleChkNums, WDT_LBX_SV_ALLNUM, 0, 0);
		GUI_ENC_API_SetVisible (ID_STA_notebk_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_notetitle_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetVisible (ID_STA_note_DetitleChkNums, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint (ID_STA_note_DetitleChkNums, WDT_STA_SV_DATAID, RS_UnknowErr, 0);
		Detitle_Chk_Nums_Count = 0;
	}	
}
void DetitleChkNums_End(void)
{

}
void DetitleChkNums_Key(BU32 key)
{
	if(GUI_VK_EXIT == key)
	{
		GUI_ENC_API_SwitchState (ID_DLG_GetServicer, 1);
	}
	else if(GUI_VK_RED == key)	/* 删除当前 */
	{
		if(Detitle_Chk_Nums_Count)
		{
			Detitle_Chk_Delete = 1;
			GUI_ENC_API_SwitchState (ID_DLG_DelDetitle, 1);			
		}
	}
	else if(GUI_VK_YELLOW == key)	/* 删除全部 */
	{
		if(Detitle_Chk_Nums_Count)
		{
			Detitle_Chk_Delete = 2;
			GUI_ENC_API_SwitchState (ID_DLG_DelDetitle, 1);	
		}
	}
}

void UI_APP_CA_DelDetitle_Enter(void)
{
	if(Detitle_Chk_Delete == 2)
	{
		GUI_ENC_API_SetValueUnPaint (ID_STA_Line1_DelDetitle, WDT_STA_SV_DATAID, RS_DelAll, 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_Line2_DelDetitle, WDT_STA_SV_DATAID, RS_Del_Exit, 0);		
	}
	else if(Detitle_Chk_Delete == 1)
	{
		GUI_ENC_API_SetValueUnPaint (ID_STA_Line1_DelDetitle, WDT_STA_SV_DATAID, RS_DelOne, 0);		
		GUI_ENC_API_SetValueUnPaint (ID_STA_Line2_DelDetitle, WDT_STA_SV_DATAID, RS_Del_Exit, 0);		
	}
}
void UI_APP_CA_DelDetitle_End(void)
{

}
void UI_APP_CA_DelDetitle_Key(BU32 key)
{
	BU32 index;
	BU16 curTvsId;
	BU32 temp;
	BS32 ret;
	
	GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &index, 0);
	curTvsId = tvsid[index];
	
	if(key == GUI_VK_SELECT)
	{
		if(Detitle_Chk_Delete == 2)
		{
			temp = 0;
			FYF_API_ca_set_value(FYF_CA_SV_DEL_DETITLE_CHK,curTvsId,temp);
			GUI_ENC_API_SwitchState(ID_DLG_DetitleChkNums, 1);
		}
		else if(Detitle_Chk_Delete == 1)
		{
			GUI_ENC_API_GetValue(ID_LBX_DetitleChkNums, WDT_LBX_SV_INDEX, &index, 0);
			temp = Detitle_Chk_Nums[index];
			ret = FYF_API_ca_set_value(FYF_CA_SV_DEL_DETITLE_CHK,curTvsId,temp);
			if(ret == FYF_TRUE)
			{
				GUI_ENC_API_SwitchState(ID_DLG_DetitleChkNums, 1);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_DelDetitle, WDT_STA_SV_DATAID, RS_DelFail, 0);
				GUI_ENC_API_SetValue(ID_STA_Line2_DelDetitle, WDT_STA_SV_DATAID, RS_Exit, 0);
			}
		}
	}
	else if(key ==GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_DetitleChkNums, 1);
	}
}


