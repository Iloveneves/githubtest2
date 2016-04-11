#ifndef __FYF_CA_H__
#define __FYF_CA_H__
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
/*
#define CA_NO  (0)
#define CA_DVT (1)
#define CA_MG  (2)
#define CA_TF  (3)
#define CA_CD  (4)
#define CA_TYPE CA_CD
*/
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
#if defined (CD_CA)
#include "../../ca/cdca/cdca.h"
#elif defined (DVT_CA)
#include "../../ca/ca_dvt/ca_dvt.h"
#endif
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef enum
{
	FYF_CA_SV_MAIL_HEAD,	/* ��ȡ�ʼ�ͷ */
	FYF_CA_SV_MAIL_BODY,	/* ��ȡ�ʼ�����,������512����Ч�ֽ�,������NULL */
	FYF_CA_SV_MSG,			/* ��ȡCA��Ϣ,������128����Ч�ֽ�,������NULL */
	FYF_CA_SV_PPV,          /* �������˶���ǰPPV,����ʹ��FYF_TRUE,�˶�ʹ��FYF_FALSE */
	FYF_CA_SV_OPERATOR_INFO,/* ���������Ӫ����Ϣ */
	FYF_CA_SV_MONEY_INFO,	/* ��õ���Ǯ����Ϣ */
	FYF_CA_SV_CORRESPOND_INFO, /* ��ĸ�������Ϣ */	
	FYF_CA_SV_EMAIL_COUNT,	/* ��ȡ�ʼ����� */
	FYF_CA_SV_EMAIL_READED, /* �ʼ��Ѷ� */
	FYF_CA_SV_EMAIL_DEL,	/* ɾ���ʼ� */
	FYF_CA_SV_WORK_TIME,	/* ����ʱ�� */
	FYF_CA_SV_VERIFY_PIN,	/* ���PIN */
	FYF_CA_SV_GET_BOOK_IPP,	/* ��ȡIPP��Ŀ��Ϣ */
	FYF_CA_SV_VIEWED_PPV,	/* �ѹۿ�IPPV*/
	FYF_CA_SV_IPP_OVER,     /* ʵʱ��������*/
	FYF_CA_SV_RATING,		/* �޸ĳ��˼� */
	FYF_CA_SV_SEVICE_ENTITLE, /*��Ȩ��Ϣ*/
	FYF_CA_SV_CARD_INFO, 		/*��������Ϣ*/
	FYF_CA_SV_PIN_LOCK, 		/*pin�Ƿ���*/
	FYF_CA_SV_AREA_INFO, 		/*������Ϣ*/
	FYF_CA_SV_MOTHER_INFO, 		/*ĸ������*/
	FYF_CA_SV_CHANGE_PIN,	/*�޸�PIN*/
	FYF_CA_SV_CARD_STATE,	/* ��ÿ�״̬ */
	FYF_CA_SV_MSG_OVER,		/* ��Ϣ��ʾ���֪ͨ */
	FYF_CA_SV_BOOK_UNBOOK,	/* �������˶�*/
	FYF_CA_SV_CAS_ID_VERIRY,	/* ���CASID */
	FYF_CA_SV_SWITCH_CHANNEL_OVER, /* ��Ƶ�������������� */
	FYF_CA_SV_CARD_IN,		/* �жϿ���û�в��� */
	FYF_CA_SV_Card_Pair,	/*��û��������Ϣ*/
	FYF_CA_SV_DETITLE_READED,	/* ��ȡ����Ȩȷ�����ȡ״̬ */
	FYF_CA_SV_DETITLE_CHK_NUMS,	/* ��ѯ����Ȩȷ������Ϣ */
	FYF_CA_SV_DEL_DETITLE_CHK,	/* ɾ������Ȩȷ���� */
	FYF_CA_SV_ACLIST_INFO, /*���AC List��Ϣ*/
	FYF_CA_SV_STBID,			/* ���STBID */
	FYF_CA_SV_TVS_NAME,			/* �����Ӫ������  */
	FYF_CA_SV_DebugMsgSign			/* ���ƻ��Դ�ӡ  */
}FYF_CA_SV_e;
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef enum
{
	FYF_CA_NOTIFY_CARD_OUT,		/* �ο�֪ͨ */
	FYF_CA_NOTIFY_CARD_IN,		/* �忨֪ͨ */
	FYF_CA_NOTIFY_MAIL,			/* �ʼ�֪ͨ */
	FYF_CA_NOTIFY_MSG,			/* ��Ϣ֪ͨ */
	FYF_CA_NOTIFY_PPV,			/* PPV֪ͨ  */

	FYF_CA_NOTIFY_APP_LOCKED,   /* �������� */
	FYF_CA_NOTIFY_CARD_LOCKED,  /* ���ܿ������� */
	FYF_CA_NOTIFY_NOT_AUTHORED,	/* ����Ŀû����Ȩ */
	FYF_CA_NOTIFY_CARD_NOTFOUND,/* �Ҳ������ܿ� */
	FYF_CA_NOTIFY_NO_MONEY,		/* ���ܿ����� */
	FYF_CA_NOTIFY_EXTER_AUTHEN,	/* ���ܿ���֤���� */
	FYF_CA_NOTIFY_PIN_AUTHEN,	/* ��Դ��� */
	FYF_CA_NOTIFY_NOREG_CARD,	/* ���ܿ�δע�� */
	FYF_CA_NOTIFY_ILLEGAL_CARD,	/* ���ܿ����� */
	FYF_CA_NOTIFY_SCARD_NOT_ACT,/* �����ӿ� */
	FYF_CA_NOTIFY_SCARD_DISABLE,/* �ӿ�����ʱ�䵽 */
	FYF_CA_NOTIFY_ZONE_CODE,	/* ��������� */

	FYF_CA_NOTIFY_APP_STOP,		/* ���ܿ���ͣ���� */
	FYF_CA_NOTIFY_APP_RESUME,	/* ���ܿ��ָ����� */
	FYF_CA_NOTIFY_PROG_UNAUTH,  /* ȡ����Ŀ���� */
	FYF_CA_NOTIFY_PROG_AUTH,	/* ���ӽ�Ŀ���� */
	FYF_CA_NOTIFY_EN_PARTNER,	/* ���û������ģʽ */
	FYF_CA_NOTIFY_DIS_PARTNER,	/* ȡ���������ģʽ */
	FYF_CA_NOTIFY_UPDATE_GNO,	/* ���ܿ�״̬���޸� */
	FYF_CA_NOTIFY_UPDATE_GK,	/* ���ܿ�����,���ο� */
	FYF_CA_NOTIFY_CHARGE,		/* �ʻ�Զ�̳�ֵ */
	FYF_CA_NOTIFY_FIGEIDENT,	/* ָ��ʶ�� */
	FYF_CA_NOFITY_SONCARD,		/* ������ĸ�� */
	FYF_CA_NOFITY_URGENT_SERVICE,/*��������֪ͨ */
	FYF_CA_NOTIFY_MODIFY_ZONECODE,/*���ܿ����������޸� */
	FYF_CA_NOTIFY_LOADER,		/* �յ��������� */
	FYF_CA_EMAIL_NEW,			/* ���ʼ�֪ͨ */
	FYF_CA_EMAIL_NEW_NO_ROOM,	/* ��������֪ͨ */
	FYF_CA_EMAIL_NONE,			/* û���ʼ�֪ͨ */
	FYF_CA_NOTIFY_SCROLL_MSG,	/* ������Ϣ֪ͨ */
	FYF_CA_NOTIFY_CLEAR_MSG,	/* ������Ϣ֪ͨ */
	
	FYF_CA_NOTIFY_SHOW_PROMPT,  /* ��ʾ��ʾ��Ϣ*/
	FYF_CA_NOTIFY_HIDE_PROMPT,   /* ������ʾ��Ϣ*/
	FYF_CA_NOTIFY_SWITCH_CHANNEL, /* ��Ƶ���������� */
	FYF_CA_NOTIFY_AREA_LOCK_OK,		/* ���������ɹ� */
	FYF_CA_NOTIFY_DETITLE_RECEIVED,	/* ����Ȩȷ����֪ͨ */
	FYF_CA_NOTIFY_PROGRESS_STRIP,	/* ������ʾ */
	FYF_CA_NOFITY_FEEDING_HIDE		/* ����ι����ʾ */
}FYF_CA_NOTIFY_e;


/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef struct
{
	unsigned short 	m_wCaSysID;				//CA ID
	union
	{
		struct
		{
			unsigned short 	m_wVideoEcmPid;			//Video Ecm Pid
			unsigned short  m_wAudioEcmPid;			//Audio Ecm Pid
			unsigned short 	m_wServiceID;			//service id
		}dvt_ca_ecm;
		struct
		{
			unsigned short 	m_wVideoEcmPid;			//Video Ecm Pid
			unsigned short  m_wAudioEcmPid;			//Audio Ecm Pid
			unsigned short 	m_wServiceID;			//service id			
		}tf_ca_ecm;
	}ca_ecm;
}FYF_Ecm_Info_s;

/*-------------------------------------------------------------------------------
not use
-------------------------------------------------------------------------------*/
typedef enum _FYF_CA_CurEcm
{
	FYF_CA_ECM_VIDEO_START,		/* ��ʼ������Ƶ��ECM */
	FYF_CA_ECM_VIDEO_END,		/* ����������Ƶ��ECM */
	FYF_CA_ECM_AUDIO_START,		/* ��ʼ������Ƶ��ECM */
	FYF_CA_ECM_AUDIO_END		/* ����������Ƶ��ECM */
}FYF_CA_CurEcm_e;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_API_CaInit(void);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_API_SetCurEmmInfo(int emmpid,int caid);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_API_SetCurEcmInfo(const FYF_Ecm_Info_s * pServiceInfo);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return: 	  None	

-------------------------------------------------------------------------------*/
void FYF_API_SetProgramChange(void);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
BS32 FYF_API_ca_set_value(FYF_CA_SV_e type,BU32 para1,BU32 para2);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
BS32 FYF_API_ca_get_value(FYF_CA_SV_e type,BU32 *para1,BU32 *para2);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_API_ca_register_notify(void (*notify)(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2));

typedef struct
{
	void (*FYF_CPI_CaInit)(void);
	void (*FYF_CPI_SetCurEmmInfo)(int emmpid,int caid);
	void (*FYF_CPI_SetCurEcmInfo)(const FYF_Ecm_Info_s * pServiceInfo);
	void (*FYF_CPI_SetProgramChange)(void);
	BS32 (*FYF_CPI_ca_set_value)(FYF_CA_SV_e type,BU32 para1,BU32 para2);
	BS32 (*FYF_CPI_ca_get_value)(FYF_CA_SV_e type,BU32 *para1,BU32 *para2);
	void (*FYF_CPI_ca_register_notify)(void (*notify)(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2));
}FYF_CA_PORTING_s;
typedef FYF_CA_PORTING_s * FYF_CA_PORTING_sp;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_API_ca_register(FYF_CA_PORTING_sp porting);
#endif

