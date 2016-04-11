/*===============================================================================

 File Name:

 Description:

 History:

===============================================================================*/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"
#include "../../../tpversion/ui_demo/osd/include/resource.h"
#include "../../tpmiddle/include/app_common.h"
/*----------------------------------------------------------------------
	Static Valiables
----------------------------------------------------------------------*/
BU16 CurChnNO;//Ӧ���㲥ǰ��Ƶ����
APP_CHN_AV_TYPE_E av_type;
bool g_bUrgency=false;//��¼�Ƿ���Ӧ���㲥��true�� false��
struct 
{
	BU32	u32waittime;
	BU32	u32tick;
} g_bUrgencyTime;


static BU32 dvtstbca_UrgentBrct_flag = FYF_FALSE;
void DVTSTBCA_SetUrgentBrctFlag(BU32 flag)
{
	dvtstbca_UrgentBrct_flag = flag;
}

BU32 DVTSTBCA_GetUrgentBrctFlag(void)
{
	return dvtstbca_UrgentBrct_flag;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:           

-------------------------------------------------------------------------------*/
void JudgeUrgencyTime(void)
{
	BU32 times;
	if(g_bUrgency)
	{
		times = FYF_API_time_ms();
		if (times - g_bUrgencyTime.u32tick > g_bUrgencyTime.u32waittime)
		{
			DVTSTBCA_CancelUrgencyBroadcast();
		}
	}
		
}

/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:           

-------------------------------------------------------------------------------*/
void CancelUrgencyBroadcast(bool bSwitch)
{
	if(!g_bUrgency)    
		return;
	g_bUrgencyTime.u32waittime=0;
	g_bUrgencyTime.u32tick=0;
	GUI_API_SetKeyReceiveMode (KEY_RECEIVE_OPEN);
	APP_EPG_API_SetRemindReserve(1);
	APP_Com_API_Start_Signal_Dectect ();	
	g_bUrgency=false;
	DVTSTBCA_SetUrgentBrctFlag(FYF_FALSE);
	
	if(bSwitch)
	{
		APP_ChnDatabase_API_SetCurChnNO(CurChnNO);
		if(APP_CHN_AV_TYPE_RD_e == av_type)
		{
			APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);//�㲥�б�
		}
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);	
	}	
}

/*-------------------------------------------------------------------------------

���ܣ�ȡ��Ӧ���㲥���л���Ӧ���㲥ǰ�û��ۿ���Ƶ����
�������ޡ�
-------------------------------------------------------------------------------*/
void DVTSTBCA_CancelUrgencyBroadcast( void )
{
	CancelUrgencyBroadcast(true);
}
/*-------------------------------------------------------------------------------
        
���ܣ�Ӧ���㲥, �л���ָ��Ƶ����
������
	wOriNetID:				ԭʼ����ID��
	wTSID:					������ID��
	wServiceID:				ҵ��ID��
	wDuration:				����������
-------------------------------------------------------------------------------*/
void DVTSTBCA_UrgencyBroadcast(WORD wOriNetID, WORD wTSID, WORD wServiceID, WORD wDuration)
{
	BU32 Chn = 0,i;
	BS32 ret;
	ChnDataBase_ChnInfo_S chnInfo;
	
	if(!g_bUrgency)
		CurChnNO=APP_ChnDatabase_API_GetCurChnNO();
	CancelUrgencyBroadcast(false);
	av_type = APP_ChnDatabase_API_GetCurChnType ();
	if(APP_CHN_AV_TYPE_TV_e != av_type)
	{
		APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);//�����б�
	}
	for(i=0;i<APP_ChnDatabase_API_GetCurTypeChnNum();i++)
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), i, &chnInfo))
		{
			if(APP_CHN_AV_TYPE_RD_e == av_type)
			{
				APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);//�㲥�б�
			}

			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			return;
		}
		if(wOriNetID == chnInfo.m_OriNetId && wTSID == chnInfo.m_TsId && wServiceID == SeparateSevId(chnInfo.m_ProgId))
		{
			break;
		}
	}	
	if(i<APP_ChnDatabase_API_GetCurTypeChnNum())
	{
		Chn = chnInfo.m_ChnIndex;
		APP_ChnDatabase_API_SetCurChnNO(Chn);
		g_bUrgency=true;
		g_bUrgencyTime.u32waittime=wDuration*1000;
		g_bUrgencyTime.u32tick=FYF_API_time_ms();
		GUI_API_SetKeyReceiveMode (KEY_RECEIVE_CLOSE);
		APP_EPG_API_SetRemindReserve(0);
		APP_Com_API_Stop_Signal_Dectect ();	
		DVTSTBCA_SetUrgentBrctFlag(FYF_TRUE);
	}
	else
	{
		if(APP_CHN_AV_TYPE_RD_e == av_type)
		{
			APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);//�㲥�б�
		}

	}
	GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);	
	return ;
	
}


