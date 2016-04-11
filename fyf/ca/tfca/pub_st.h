/* =========================================================== *\
	Title:				STB���̺�CASϵͳĩ�ˣ����ܿ����õ����ݶ��壬���Ͷ����
	Version:			3.0.0.0
	Create Date:		2003.07.28
	Author:				GZY
	Description:		���ļ�����ANSI C���﷨	
\* =========================================================== */
#ifndef _PUB21EX_ST_H
#define _PUB21EX_ST_H
#define  INCLUDE_ADDRESSING_APP	1  /*OSD���ʼ�ģ��*/
#define  INCLUDE_IPPV_APP			1  /*IPPVģ��*/
#define  INCLUDE_LOCK_SERVICE		0  /*VOD�㲥ģ��*/
#ifdef  __cplusplus
extern "C" {
#endif
#define true           1
#define false          0

/*���к������õ�ָ��ָ��Ŀռ��ɵ��õĹ���*/
#define		OUT				/*�����ó�ʼ�������������������*/
#define		IN				/*�����߳�ʼ�������������������*/
#define		INOUT			/*�����������룬�������������*/	

#define		ULONG			unsigned  long   
#define		WORD			unsigned  short  
#define		BYTE			unsigned  char 
#ifndef     bool
	#define		bool		BYTE
#endif
#define		TFDATE			WORD
#define		TFDATETIME		ULONG
#ifndef		NULL
#define		NULL			0
#endif

/*˽�б�PID List_Management*/
#define		TFCAS_LIST_OK						0x00
#define		TFCAS_LIST_FIRST					0x01	
#define		TFCAS_LIST_ADD					0x02
#define		TFCAS_LIST_UPDATE					0x03


/*������CAS������*/
#define		TFCA_MAXLEN_PROGRAMNUM			4		/*һ������������Ľ�Ŀ��*/
#define		TFCA_MAXLEN_ECM					8		/*ͬʱ���մ����ECMPID���������*/

/*���ܿ�������*/
#define		TFCA_MAXLEN_PRICE					2		/*����IPPV�۸����*/
#define		TFCA_MAXLEN_OPERATOR				4		/*������Ӫ�̸���*/
#define		TFCA_MAXLEN_PINCODE		   		6		/*PIN��ĳ���*/     
#define		TFCA_MAXLEN_ACLIST				18		/*���ܿ��ڱ���ÿ����Ӫ�̵�AC�ĸ���*/
#define		TFCA_MAXLEN_SLOT					20		/*���洢�����Ǯ����*/
#define		TFCA_MAXLEN_TVSPRIINFO			20		/*��Ӫ��˽����Ϣ*/
#define		TFCA_MAXLEN_IPPVP					300		/*���ܿ��������IPPV��Ŀ�ĸ���*/
#define		TFCA_MAXLEN_ENTITL				300		/*���ܿ����������Ȩ��Ʒ�ĸ���*/
	
/*�ʼ�*/
#define		TFCA_MAXLEN_EMAIL_TITLE			30		/*�ʼ�����ĳ���*/
#define		TFCA_MAXLEN_EMAIL				100		/*�����б��������ʼ�����*/
#define		TFCA_MAXLEN_EMAIL_CONTENT		160		/*�ʼ����ݵĳ���*/
/*�ʼ�ͼ����ʾ��ʽ*/
#define		Email_IconHide						0x00	/*�����ʼ�֪ͨͼ��*/
#define		Email_New							0x01	/*���ʼ�֪ͨ����ʾ���ʼ�ͼ��*/
#define		Email_SpaceExhaust					0x02	/*���̿ռ�������ͼ����˸��*/

/*OSD�ĳ���*/
#define		TFCA_MAXLEN_OSD					180		/*OSD���ݵ���󳤶�*/
/*OSD����*/
#define		OSD_TOP							0x01	/*OSD�����ʾ����Ļ�Ϸ�*/
#define		OSD_BOTTOM						0x02	/*OSD�����ʾ����Ļ�·�*/

/*��Ŀ�޷����ŵ���ʾ*/
#define		MESSAGE_CANCEL_TYPE				0x00	/*ȡ����ǰ����ʾ*/
#define		MESSAGE_BADCARD_TYPE			0x01	/*�޷�ʶ�𿨣�����ʹ��*/
#define		MESSAGE_EXPICARD_TYPE			0x02	/*���ܿ��Ѿ����ڣ�������¿�*/
#define		MESSAGE_INSERTCARD_TYPE			0x03	/*���Ž�Ŀ����������ܿ�*/
#define		MESSAGE_NOOPER_TYPE				0x04	/*���в����ڽ�Ŀ��Ӫ��*/
#define		MESSAGE_BLACKOUT_TYPE			0x05	/*��������*/
#define		MESSAGE_OUTWORKTIME_TYPE		0x06	/*���ڹ���ʱ����*/
#define		MESSAGE_WATCHLEVEL_TYPE			0x07	/*��Ŀ��������趨�ۿ�����*/
#define		MESSAGE_PAIRING_TYPE				0x08	/*��ĿҪ�������Ӧ*/
#define		MESSAGE_NOENTITLE_TYPE			0x09	/*û����Ȩ*/
#define		MESSAGE_DECRYPTFAIL_TYPE			0x0A	/*��Ŀ����ʧ��*/
#define		MESSAGE_NOMONEY_TYPE			0x0B	/*���ڽ���*/
#define		MESSAGE_ERRREGION_TYPE			0x0C	/*������ȷ*/
/*��Ŀ����׶���ʾ*/
#define		IPPV_FREEVIEWED_SEGMENT			0x00        /*���Ԥ���׶Σ��Ƿ���*/
#define		IPPV_PAYEVIEWED_SEGMENT			0x01        /*�շѽ׶Σ��Ƿ���*/


/************************************************************************/
/*                       Product,IPPV���                               */
/************************************************************************/
/*IPPV�۸�����*/
#define TFCA_IPPVPRICETYPE_TPPVVIEW			0X0
#define TFCA_IPPVPRICETYPE_TPPVVIEWTAPING	0X1
/*IPPV��Ŀ��״̬*/
#define TFCA_IPPVSTATUS_TOKEN_BOOKING		0x01
#define TFCA_IPPVSTATUS_TOKEN_VIEWED			0x03

/*--------------------------------����ֵ����-------------------------------------------------------------*/
#define TFCAS_OK								0x00
#define TFCAS_UNKNOWN							0x01	/*- δ֪����*/
#define TFCAS_POINTER_INVALID					0x02	/*- ָ����Ч*/
#define TFCAS_CARD_INVALID						0x03	/*- ���ܿ���Ч*/
#define TFCAS_PIN_INVALID						0x04	/*- PIN����Ч*/
#define TFCAS_PIN_LOCK							0x05	/*- PIN������*/
#define TFCAS_DATASPACE_SMALL					0x06	/*- �����Ŀռ䲻��*/
#define TFCAS_CARD_PAIROTHER					0x07	/*- ���ܿ��Ѿ���Ӧ��Ļ�����*/
#define TFCAS_DATA_NOT_FIND					0x08	/*- û���ҵ���Ҫ������*/
#define TFCAS_PROG_STATUS_INVALID				0x09	/*- Ҫ����Ľ�Ŀ״̬��Ч*/
#define TFCAS_CARD_NO_ROOM					0x0A	/*- ���ܿ�û�пռ��Ź���Ľ�Ŀ*/
#define TFCAS_WORKTIME_INVALID				0x0B	/*- �趨�Ĺ���ʱ����Ч 0��24*/
#define TFCAS_IPPV_CANNTDEL					0x0C	/*- IPPV��Ŀ���ܱ�ɾ��*/
#define TFCAS_CARD_NOPAIR						0x0D	/*- ���ܿ�û�ж�Ӧ�κεĻ�����*/
#define TFCAS_WATCHRATING_INVALID			0x0E	/*- �趨�Ĺۿ�������Ч 4��18*/
/*-----------------------------------�ź�������  ������ϵͳ��һ�� -----------------------------------*/

typedef ULONG  TFCA_Semaphore ;

typedef struct _TFCAOperatorInfo{
	char	m_szTVSPriInfo[TFCA_MAXLEN_TVSPRIINFO+1];   /*��Ӫ��˽����Ϣ*/
}STFCAOperatorInfo;

typedef struct _TFCASServiceInfo{
	WORD	m_wEcmPid;
	BYTE	m_byServiceNum;
	WORD 	m_wServiceID[TFCA_MAXLEN_PROGRAMNUM];
}STFCASServiceInfo;

#if INCLUDE_IPPV_APP 
	typedef struct _TFCASTVSSlotInfo{
		WORD	m_wCreditLimit;						/*���ö�*/
		WORD	m_wBalance;						/*�ѻ��ĵ���*/
	}STFCATVSSlotInfo;

	typedef struct _TFCAIPPVPrice{
		BYTE	m_byPriceCode;	
		WORD	m_wPrice;		
	}STFCAIPPVPrice; 

	typedef struct _TFCAIppvBuyInfo{
		WORD			m_wTVSID;					/*��Ӫ��ID*/
		BYTE			m_bySlotID;					/*Ǯ��ID*/
		ULONG			m_dwProductID;				/*��Ŀ��ID*/
		BYTE			m_byPriceNum;				/*��Ŀ�۸����*/
		STFCAIPPVPrice	m_Price[TFCA_MAXLEN_PRICE];	/*��Ŀ�۸�*/
		TFDATE			m_wExpiredDate;			/*��Ŀ����ʱ��*/
	}STFCAIppvBuyInfo;

	typedef struct _TFCAIppvfo{
		ULONG 			m_dwProductID;		/*��Ŀ��ID*/
		BYTE			m_byBookEdFlag;	/*��Ʒ״̬*/ /*BOOKING VIEWED EXPIRED*/ 
		BYTE			m_bCanTape;		/*�Ƿ����¼��1������¼��0��������¼��*/
		WORD			m_wPrice;			/*��Ŀ�۸�*/
		TFDATE			m_wExpiredDate;	/*��Ŀ����ʱ��*/
	}STFCAIppvInfo;
#endif

typedef struct _TFCAEntitle{
	ULONG 		m_dwProductID;
	BYTE		m_bCanTape;
	TFDATE		m_tExpriData;
}STFCAEntitle;

typedef struct _TFCAEntitles{
	WORD		m_wProductCount;
	STFCAEntitle	m_Entitles[TFCA_MAXLEN_ENTITL];
}STFCAEntitles;

#if INCLUDE_ADDRESSING_APP
	typedef struct _TFCAEmailHead{
		ULONG		m_dwActionID;		/*Email ID  */
		BYTE		m_bNewEmail;		/*0 �������ʼ� 1�����ʼ�*/
		char   		m_szEmailHead[TFCA_MAXLEN_EMAIL_TITLE+1];/*�ʼ����⣬�Ϊ30*/
		WORD 		m_wImportance;		/*��Ҫ��, 0����ͨ��1����Ҫ*/
		ULONG		m_tCreateTime;		/*EMAIL������ʱ��*/
	}STFCAEmailHead;

	typedef struct _TFCAEmailContent{
		char		m_szEmail[TFCA_MAXLEN_EMAIL_CONTENT+1];	/*Email������*/
	}STFCAEmailContent;
#endif

#ifdef  __cplusplus
}
#endif
#if 0
/**************************************wxf define *******************************************************/
typedef struct TP_TFIPPV
{
	WORD   TVSId ;
	BYTE   SoltID ;
	STFCAIppvInfo  IppvInfo;
}TP_IPPv_t;



typedef struct TFCA_OSD_STRUCT
{
	BOOLEAN  bOsdon ;
	char 	OsdMsg[TFCA_MAXLEN_OSD] ;
}TFCA_Osd_t;


typedef struct  FZ_SLOT
{
	WORD   TVSId ;
	BYTE    SlotId ;
}FZ_Slot_t;

typedef struct _FZTFCAEntitle{
	
	WORD   		TVSId ;
	ULONG 		m_dwProductID;
	BYTE		m_bCanTape;
	TFDATE		m_tExpriData;
}FZ_TFCAEntitle;

typedef struct _FZTFCAEntitles
{
	WORD		m_wProductCount;
	FZ_TFCAEntitle	m_Entitles[TFCA_MAXLEN_ENTITL];
}FZ_TFCAEntitles;


typedef struct tfca_error_code_t
{
	WORD  EcmPid ;
	BYTE   Message ;
	BOOLEAN bFgpOn ;
}TFCA_Message_t ;
#endif
#endif



