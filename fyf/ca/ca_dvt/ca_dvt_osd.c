/**********************************************************************************
 File Name:   ca_dvt_osd.c

 Description: STB�ṩ��CA���õĽӿ� 

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
���ܣ���ʾ�û����ܹۿ���Ŀ����ʾ��Ϣ����������������Ӧʹ�á�
������	
	byMesageNo:				Ҫ��ʾ����Ϣ��š�

#define	DVTCA_RATING_TOO_LOW					0			//�տ����𲻹�
#define DVTCA_NOT_IN_WATCH_TIME					1			//�����տ�ʱ����
#define	DVTCA_NOT_PAIRED						2			//û�л�����Ӧ
#define	DVTCA_IS_PAIRED_OTHER					3			//IC�������������ж�Ӧ
#define	DVTCA_PLEASE_INSERT_CARD				4			//��忨
#define	DVTCA_NO_ENTITLE						5			//û�й���˽�Ŀ
#define	DVTCA_PRODUCT_RESTRICT					6			//��Ӫ�����ƹۿ��ý�Ŀ
#define	DVTCA_AREA_RESTRICT						7			//��Ӫ����������ۿ�
#define	DVTCA_DECRYPT_FAILURE					0xFF		//STB�����κ���ʾ����ͨѶ���⣬ͨ������λ�ɽ��
//V2.1��������ʾ��Ϣ
#define DVTCA_MOTHER_RESTRICT					8			//�˿�Ϊ�ӿ����Ѿ��������տ�������ĸ�����
#define DVTCA_NO_MONEY							9			//���㣬���ܹۿ��˽�Ŀ���뼰ʱ��ֵ
#define DVTCA_IPPV_NO_CONFIRM					10			//�˽�ĿΪIPPV��Ŀ���뵽IPPV��Ŀȷ��/ȡ������˵���ȷ�Ϲ���˽�Ŀ
#define DVTCA_IPPV_NO_BOOK						11			//�˽�ĿΪIPPV��Ŀ����û��Ԥ����ȷ�Ϲ��򣬲��ܹۿ��˽�Ŀ
#define DVTCA_IPPT_NO_CONFIRM					12			//�˽�ĿΪIPPT��Ŀ���뵽IPPT��Ŀȷ��/ȡ������˵���ȷ�Ϲ���˽�Ŀ
#define DVTCA_IPPT_NO_BOOK						13			//�˽�ĿΪIPPT��Ŀ����û��Ԥ����ȷ�Ϲ��򣬲��ܹۿ��˽�Ŀ
#define DVTCA_IPPV_NO_MONEY						14			//�˽�ĿΪIPPV��Ŀ�����㣬���ܹۿ����뼰ʱ��ֵ
#define DVTCA_IPPT_NO_MONEY						15			//�˽�ĿΪIPPT��Ŀ�����㣬���ܹۿ����뼰ʱ��ֵ
//xb:20050617
#define DVTCA_DATA_INVALID						16			//������Ч��STB�����κ���ʾ������Կ���⡣
#define DVTCA_SLOT_NOT_FOUND					17			//Ǯ��������
#define DVTCA_SC_NOT_SERVER						18			//IC������ֹ����
#define DVTCA_TVS_NOT_FOUND						19			//��Ӫ�̲�����
#define DVTCA_KEY_NOT_FOUND						20			//�˿�δ���������ϵ��Ӫ��
#define DVTCA_IPPNEED_CALLBACK					21			//����ϵ��Ӫ�̻ش�IPP��Ŀ��Ϣ	
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

/*--------------------------- ����2�Ľ��棺�û�����ȡ����ʾ�Ľ��� -----------------------*/
/*++
���ܣ���ʾ���ʼ��ı�־��
������
	byShow:					��־���͡��������£�
							DVTCAS_NEW_EAMIL				���ʼ�֪ͨ��
							DVTCAS_NEW_EMAIL_NO_ROOM		�����ʼ�����Email�ռ䲻����
							DVTCAS_EMAIL_NONE				û�����ʼ���
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
���ܣ��������Ļ�ķ�ʽ��ʾһ��OSD��Ϣ��������ͬ����ʾ��Ҳ�������첽��ʾ��
����:	
	szOSD:					������ʾ��OSD��Ϣ��
˵����
	�û�����ͨ��ң����ȡ����ǰOSD��ʾ��
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
���ܣ���ʾָ����Ϣ��
������
	dwCardID:				���š�
	wDuration:				����������
--*/
void	DVTSTBCA_ShowFingerPrinting(DWORD dwCardID, WORD wDuration)
{
	FYF_IPI_ca_notify(FYF_CA_NOTIFY_FIGEIDENT, &dwCardID, &wDuration);
}

