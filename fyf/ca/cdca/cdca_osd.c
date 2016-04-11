#include "CDCAS30.h"
#include "fyf_com.h"
#define CDCA_DEBUG printf


static CDCA_U8 MessageType[21][2]={
{CDCA_MESSAGE_BADCARD_TYPE,	 	255},	/* 无法识别卡 */
{CDCA_MESSAGE_EXPICARD_TYPE, 	255},	/* 智能卡过期，请更换新卡 */
{CDCA_MESSAGE_INSERTCARD_TYPE,	4},		/* 加扰节目，请插入智能卡 */
{CDCA_MESSAGE_NOOPER_TYPE,		19},	/* 卡中不存在节目运营商 */
{CDCA_MESSAGE_BLACKOUT_TYPE,	6},		/* 条件禁播 */
{CDCA_MESSAGE_OUTWORKTIME_TYPE,	1}, 	/* 当前时段被设定为不能观看 */
{CDCA_MESSAGE_WATCHLEVEL_TYPE,	0},		/* 节目级别高于设定的观看级别 */
{CDCA_MESSAGE_PAIRING_TYPE,		2},		/* 智能卡与本机顶盒不对应 */
{CDCA_MESSAGE_NOENTITLE_TYPE,	6},		/* 没有授权 */
{CDCA_MESSAGE_DECRYPTFAIL_TYPE,	255},	/* 节目解密失败 */
{CDCA_MESSAGE_NOMONEY_TYPE,		9},		/* 卡内金额不足 */
{CDCA_MESSAGE_ERRREGION_TYPE,	7},		/* 区域不正确 */
{CDCA_MESSAGE_NEEDFEED_TYPE,	8},		/* 子卡需要和母卡对应，请插入母卡 */
{CDCA_MESSAGE_ERRCARD_TYPE,		255},	/* 智能卡校验失败，请联系运营商 */
{CDCA_MESSAGE_UPDATE_TYPE,		255},	/* 智能卡升级中，请不要拔卡或者关机 */
{CDCA_MESSAGE_LOWCARDVER_TYPE,	255},	/* 请升级智能卡 */
{CDCA_MESSAGE_VIEWLOCK_TYPE,	255},	/* 请勿频繁切换频道 */
{CDCA_MESSAGE_MAXRESTART_TYPE,	255},	/* 智能卡暂时休眠，请5分钟后重新开机 */
{CDCA_MESSAGE_FREEZE_TYPE,		20},	/* 智能卡已冻结，请联系运营商 */
{CDCA_MESSAGE_CALLBACK_TYPE,	255},	/* 智能卡已暂停，请回传收视记录给运营商 */
{CDCA_MESSAGE_STBLOCKED_TYPE,	255}	/* 请重启机顶盒 */
};
//-------------------- 邮件/OSD显示管理 --------------------
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
//-------------------- 显示界面管理 --------------------
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
        当byShow＝CDCA_Email_New && dwEmailID!=0 的时候dwEmailID是一封新邮件的ID
         这个时候可以调用        CDCASTB_GetEmailHead
                                                 CDCASTB_GetEmailContent 获取这封邮件的标题和内容
        如果邮箱内有多封新邮件，这个ID是其中的某一封邮件的ID
         机顶盒也可以不关心这个ID，因为CDCASTB_GetEmailHeads会返回邮箱内所有的邮件的标题信息内容的
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



