/**********************************************************************************
 File Name:   ca_dvt_osd.c

 Description: STB提供给CA调用的接口 

 History:           
**********************************************************************************/
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"

BU32 requestfeeding_status = FYF_FALSE;
void APP_Com_API_Stop_RequestFeeding(void)
{
	requestfeeding_status = FYF_FALSE;
}
void DVTSTBCA_API_RequestFeeding_thread(void * param)
{
	while(1)
	{
		FYF_API_thread_sleep(200);
		if(requestfeeding_status == FYF_TRUE)
		{
			FYF_IPI_ca_notify(FYF_CA_NOFITY_SONCARD, 0,0);
		}
	}
}
void DVTSTBCA_API_InitRequestFeeding(void)
{	
	FYF_API_thread_create("RequestFeeding",DVTSTBCA_API_RequestFeeding_thread,FYF_NULL, 15, 10 * 1024);
}
/*------------------------------------------------------------
功能：显示用户不能观看节目等提示信息，下面两个函数对应使用。
参数：	
	byMesageNo:				要显示的消息编号。

#define	DVTCA_RATING_TOO_LOW					0			//收看级别不够
#define DVTCA_NOT_IN_WATCH_TIME					1			//不在收看时段内
#define	DVTCA_NOT_PAIRED						2			//没有机卡对应
#define	DVTCA_IS_PAIRED_OTHER					3			//IC卡与其它机顶盒对应
#define	DVTCA_PLEASE_INSERT_CARD				4			//请插卡
#define	DVTCA_NO_ENTITLE						5			//没有购买此节目
#define	DVTCA_PRODUCT_RESTRICT					6			//运营商限制观看该节目
#define	DVTCA_AREA_RESTRICT						7			//运营商限制区域观看
#define	DVTCA_DECRYPT_FAILURE					0xFF		//STB不做任何提示。卡通讯问题，通过卡复位可解决
//V2.1新增的提示信息
#define DVTCA_MOTHER_RESTRICT					8			//此卡为子卡，已经被限制收看，请与母卡配对
#define DVTCA_NO_MONEY							9			//余额不足，不能观看此节目，请及时充值
#define DVTCA_IPPV_NO_CONFIRM					10			//此节目为IPPV节目，请到IPPV节目确认/取消购买菜单下确认购买此节目
#define DVTCA_IPPV_NO_BOOK						11			//此节目为IPPV节目，您没有预订和确认购买，不能观看此节目
#define DVTCA_IPPT_NO_CONFIRM					12			//此节目为IPPT节目，请到IPPT节目确认/取消购买菜单下确认购买此节目
#define DVTCA_IPPT_NO_BOOK						13			//此节目为IPPT节目，您没有预订和确认购买，不能观看此节目
#define DVTCA_IPPV_NO_MONEY						14			//此节目为IPPV节目，余额不足，不能观看，请及时充值
#define DVTCA_IPPT_NO_MONEY						15			//此节目为IPPT节目，余额不足，不能观看，请及时充值
//xb:20050617
#define DVTCA_DATA_INVALID						16			//数据无效，STB不做任何提示。卡密钥问题。
#define DVTCA_SLOT_NOT_FOUND					17			//钱包不存在
#define DVTCA_SC_NOT_SERVER						18			//IC卡被禁止服务
#define DVTCA_TVS_NOT_FOUND						19			//运营商不存在
#define DVTCA_KEY_NOT_FOUND						20			//此卡未被激活，请联系运营商
#define DVTCA_IPPNEED_CALLBACK					21			//请联系运营商回传IPP节目信息	
------------------------------------------------------------*/
BYTE Showmessage = 0;
void DVTSTBCA_ShowPromptMessage(BYTE byMesageNo)
{
	printf("---------------DVTSTBCA_ShowPromptMessage--------------byMesageNo=%d\n",byMesageNo);
	if(byMesageNo == DVTCA_DECRYPT_FAILURE)
	{
//		if( DVTSTBCA_SCReset() == DVTCA_OK)
//		{
//			DVTCASTB_SCInsert(0) ;
//		}
		return;
	}
//	if(APP_CA_API_GetCardIn()==1 && byMesageNo == DVTCA_PLEASE_INSERT_CARD)
//		return;
	if(Showmessage)	
	{
		Showmessage = 0;
		FYF_IPI_ca_notify (FYF_CA_NOTIFY_HIDE_PROMPT, 0, 0);
	}
	Showmessage = 1;
	if(byMesageNo == DVTCA_MOTHER_RESTRICT)
		requestfeeding_status = FYF_TRUE;
	else
		FYF_IPI_ca_notify (FYF_CA_NOTIFY_SHOW_PROMPT, (BU32 *)&byMesageNo, 0);
}

void DVTSTBCA_HidePromptMessage(void)
{
	Showmessage = 0;
	FYF_IPI_ca_notify (FYF_CA_NOTIFY_HIDE_PROMPT, 0, 0);
}

/*--------------------------- 类型2的界面：用户可以取消显示的界面 -----------------------*/
/*++
功能：显示新邮件的标志。
参数：
	byShow:					标志类型。含义如下：
							DVTCAS_NEW_EAMIL				新邮件通知。
							DVTCAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够。
							DVTCAS_EMAIL_NONE				没有新邮件。
--*/
void DVTSTBCA_EmailNotify(BYTE byShow)
{
	if(byShow==DVTCAS_EMAIL_NONE)
	{
		FYF_IPI_ca_notify(FYF_CA_EMAIL_NONE, 0, 0);
	}
	else if (byShow == DVTCAS_EMAIL_NEW)
	{
		FYF_IPI_ca_notify(FYF_CA_EMAIL_NEW, 0, 0);
	}
	else if (byShow == DVTCAS_EMAIL_NEW_NO_ROOM)
	{
		FYF_IPI_ca_notify(FYF_CA_EMAIL_NEW_NO_ROOM, 0, 0);
	}
	else
	{
	}
}

/*
功能：以左滚字幕的方式显示一次OSD消息，可以是同步显示，也可以是异步显示。
参数:	
	szOSD:					用于显示的OSD信息。
说明：
	用户可以通过遥控器取消当前OSD显示。
--*/
void	DVTSTBCA_ShowOSDMsg(BYTE byPriority,const char * szOSD)
{ 
	BU32 style = 2;
	if(szOSD == NULL) 
		printf("---------------DVTSTBCA_ShowOSDMsg--------------szOSD=NULL\n");
	else
	{
		printf("---------------DVTSTBCA_ShowOSDMsg--------------szOSD=%s\n",szOSD);
		FYF_IPI_ca_notify (FYF_CA_NOTIFY_SCROLL_MSG, (BU32 *)szOSD, &style);
	}
}

/*
功能：显示指纹信息。
参数：
	dwCardID:				卡号。
	wDuration:				持续秒数。
--*/
void	DVTSTBCA_ShowFingerPrinting(DWORD dwCardID, WORD wDuration)
{
	FYF_IPI_ca_notify(FYF_CA_NOTIFY_FIGEIDENT, &dwCardID, &wDuration);
}

