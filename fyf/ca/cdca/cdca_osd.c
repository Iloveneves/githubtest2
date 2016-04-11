#include "CDCAS30.h"
#include "fyf_com.h"
#define CDCA_DEBUG printf


static CDCA_U8 MessageType[21][2]={
{CDCA_MESSAGE_BADCARD_TYPE,	 	255},	/* �޷�ʶ�� */
{CDCA_MESSAGE_EXPICARD_TYPE, 	255},	/* ���ܿ����ڣ�������¿� */
{CDCA_MESSAGE_INSERTCARD_TYPE,	4},		/* ���Ž�Ŀ����������ܿ� */
{CDCA_MESSAGE_NOOPER_TYPE,		19},	/* ���в����ڽ�Ŀ��Ӫ�� */
{CDCA_MESSAGE_BLACKOUT_TYPE,	6},		/* �������� */
{CDCA_MESSAGE_OUTWORKTIME_TYPE,	1}, 	/* ��ǰʱ�α��趨Ϊ���ܹۿ� */
{CDCA_MESSAGE_WATCHLEVEL_TYPE,	0},		/* ��Ŀ��������趨�Ĺۿ����� */
{CDCA_MESSAGE_PAIRING_TYPE,		2},		/* ���ܿ��뱾�����в���Ӧ */
{CDCA_MESSAGE_NOENTITLE_TYPE,	6},		/* û����Ȩ */
{CDCA_MESSAGE_DECRYPTFAIL_TYPE,	255},	/* ��Ŀ����ʧ�� */
{CDCA_MESSAGE_NOMONEY_TYPE,		9},		/* ���ڽ��� */
{CDCA_MESSAGE_ERRREGION_TYPE,	7},		/* ������ȷ */
{CDCA_MESSAGE_NEEDFEED_TYPE,	8},		/* �ӿ���Ҫ��ĸ����Ӧ�������ĸ�� */
{CDCA_MESSAGE_ERRCARD_TYPE,		255},	/* ���ܿ�У��ʧ�ܣ�����ϵ��Ӫ�� */
{CDCA_MESSAGE_UPDATE_TYPE,		255},	/* ���ܿ������У��벻Ҫ�ο����߹ػ� */
{CDCA_MESSAGE_LOWCARDVER_TYPE,	255},	/* ���������ܿ� */
{CDCA_MESSAGE_VIEWLOCK_TYPE,	255},	/* ����Ƶ���л�Ƶ�� */
{CDCA_MESSAGE_MAXRESTART_TYPE,	255},	/* ���ܿ���ʱ���ߣ���5���Ӻ����¿��� */
{CDCA_MESSAGE_FREEZE_TYPE,		20},	/* ���ܿ��Ѷ��ᣬ����ϵ��Ӫ�� */
{CDCA_MESSAGE_CALLBACK_TYPE,	255},	/* ���ܿ�����ͣ����ش����Ӽ�¼����Ӫ�� */
{CDCA_MESSAGE_STBLOCKED_TYPE,	255}	/* ������������ */
};
//-------------------- �ʼ�/OSD��ʾ���� --------------------
void CDSTBCA_ShowOSDMessage(CDCA_U8 byStyle,  char* szMessage)
{
	FYF_IPI_ca_notify (FYF_CA_NOTIFY_SCROLL_MSG, (BU32 *)szMessage, (BU32 *)&byStyle);
	CDCA_DEBUG ("CDSTBCA_ShowOSDMessage byStyle=%d\n",byStyle);
}
void CDSTBCA_HideOSDMessage(CDCA_U8 byStyle)
{
//CDCA_DEBUG	 
	FYF_IPI_ca_notify (FYF_CA_NOTIFY_CLEAR_MSG, NULL, (BU32 *)&byStyle);
	CDCA_DEBUG ("CDSTBCA_HideOSDMessage byStyle=%d\n",byStyle);
}
//-------------------- ��ʾ������� --------------------
void CDSTBCA_ShowProgressStrip(CDCA_U8 byProgress, CDCA_U8 byMark)
{
	CDCA_DEBUG ("CDSTBCA_ShowProgressStrip byProgress=%d\n",byProgress);

	FYF_IPI_ca_notify (FYF_CA_NOTIFY_PROGRESS_STRIP, (BU32 *)&byProgress, (BU32 *)&byMark);
}

int byMessageNo_four = 0;
void CDSTBCA_ShowBuyMessage(CDCA_U16 wEcmPID, CDCA_U8 byMessageType)
{
	static BU32 messageNo = 0;
	CDCA_DEBUG ("CDSTBCA_ShowBuyMessage wEcmPID=%d\n",wEcmPID);
#if 0
/*use DVT message string*/
	if(byMessageType==CDCA_MESSAGE_CANCEL_TYPE)
	{/*hide message*/
		FYF_IPI_ca_notify (FYF_CA_NOTIFY_HIDE_PROMPT, 0, 0);
		messageNo = 0xff;
		if(byMessageNo_four)
		{		
			byMessageNo_four = 0;
		}
	}
	else
	{/*show message*/
		if((messageNo!= MessageType[byMessageType-1][1])&&messageNo!=0xff)
		{
			FYF_IPI_ca_notify (FYF_CA_NOTIFY_HIDE_PROMPT, 0, 0);
			messageNo = 0xff;
			if(byMessageNo_four)
			{		
				byMessageNo_four = 0;
			}
		}
		if(byMessageType == CDCA_MESSAGE_STBLOCKED_TYPE)
		{
			messageNo = MessageType[20][1];
		}
		else
		{
			messageNo = MessageType[byMessageType-1][1];
		}
		//if(0 == FYF_CA_API_GetCardIn () || 4!=messageNo)
		{
			FYF_IPI_ca_notify (FYF_CA_NOTIFY_SHOW_PROMPT, (BU32 *)&messageNo, 0);
		}
		if(4==messageNo)
		{
			byMessageNo_four = 1;
		}
	}
#else
/*use cdca message string*/
	if(byMessageType==CDCA_MESSAGE_CANCEL_TYPE)
	{/*hide message*/
		printf("FYF_CA_NOTIFY_HIDE_PROMPT\n");
		FYF_IPI_ca_notify (FYF_CA_NOTIFY_HIDE_PROMPT, 0, 0);
		messageNo = 0;
		if(byMessageNo_four)
		{		
			byMessageNo_four = 0;
		}
	}
	else
	{/*show message*/
		if(messageNo)
		{
			printf("HIDE BEFOR SHOW~byMessageType=%d\n",byMessageType);
			FYF_IPI_ca_notify (FYF_CA_NOTIFY_HIDE_PROMPT, 0, 0);
			messageNo = 0;
			if(byMessageNo_four)
			{		
				byMessageNo_four = 0;
			}
		}
		if(CDSTBCA_IPI_VerifyEcm (wEcmPID) == FYF_FALSE)
		{
			return;		
		}
		if(byMessageType == CDCA_MESSAGE_STBLOCKED_TYPE)
		{
			byMessageType = 0x15;
		}
		printf("SHOW_PROMPT~byMessageType=%d\n",byMessageType);
		FYF_IPI_ca_notify (FYF_CA_NOTIFY_SHOW_PROMPT, (BU32 *)&byMessageType, 0);
		messageNo =0xff;
		if(4==CDCA_MESSAGE_INSERTCARD_TYPE)
		{
			byMessageNo_four = 1;
		}
	}

#endif
}
void CDSTBCA_ShowFingerMessage(CDCA_U16 wEcmPID, CDCA_U32 dwCardID)
{
	BU32 wDuration;

	wDuration = wEcmPID;
	CDCA_DEBUG ("TFSTBCA_ShowFingerMessage wEcmPID=%d\n",wEcmPID);
	
	FYF_IPI_ca_notify(FYF_CA_NOTIFY_FIGEIDENT, &dwCardID, &wDuration);
}
/*++
        ��byShow��CDCA_Email_New && dwEmailID!=0 ��ʱ��dwEmailID��һ�����ʼ���ID
         ���ʱ����Ե���        CDCASTB_GetEmailHead
                                                 CDCASTB_GetEmailContent ��ȡ����ʼ��ı��������
        ����������ж�����ʼ������ID�����е�ĳһ���ʼ���ID
         ������Ҳ���Բ��������ID����ΪCDCASTB_GetEmailHeads�᷵�����������е��ʼ��ı�����Ϣ���ݵ�
 -*/
void CDSTBCA_EmailNotifyIcon(CDCA_U8 byShow, CDCA_U32 dwEmailID)
{
	switch(byShow)
	{
		case CDCA_Email_IconHide:
			FYF_IPI_ca_notify(FYF_CA_EMAIL_NONE,0,0);
			break;
		case CDCA_Email_New:
			FYF_IPI_ca_notify(FYF_CA_EMAIL_NEW,0,0);
			break;
		case CDCA_Email_SpaceExhaust:
			FYF_IPI_ca_notify(FYF_CA_EMAIL_NEW_NO_ROOM,0,0);
			break;
		default:
			break;
	}

}



