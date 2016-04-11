#ifndef CDCAS_CALIB_APIEX_H
#define CDCAS_CALIB_APIEX_H

#ifdef  __cplusplus
extern "C" {
#endif


/*-------------------------------------������������---------------------------------------*/
typedef unsigned  long  CDCA_U32;
typedef unsigned  short CDCA_U16;
typedef unsigned  char  CDCA_U8;

typedef CDCA_U8 CDCA_BOOL;

#define CDCA_TRUE    ((CDCA_BOOL)1)
#define CDCA_FALSE   ((CDCA_BOOL)0)

/*----------------------------------------�궨��-----------------------------------------*/

//---------- 1. ���ܿ��������
#define CDCA_MAXLEN_SN                16U   /* ���ܿ����кŵĳ��� */
#define CDCA_MAXLEN_PINCODE           6U    /* PIN��ĳ��� */
#define CDCA_MAXLEN_TVSPRIINFO        32U   /* ��Ӫ��˽����Ϣ�ĳ��� */
#define CDCA_MAXNUM_OPERATOR          4U    /* ������Ӫ�̸��� */
#define CDCA_MAXNUM_ACLIST            18U   /* ���ܿ��ڱ����ÿ����Ӫ�̵��û��������� */
#define CDCA_MAXNUM_SLOT              20U   /* ���洢�����Ǯ���� */

#define CDCA_MAXNUM_IPPVP             300U  /* ���ܿ��������IPPV��Ŀ�ĸ��� */
#define CDCA_MAXNUM_PRICE             2U    /* ����IPPV�۸���� */

#define CDCA_MAXNUM_ENTITLE           300U  /* ���ܿ����������Ȩ��Ʒ�ĸ��� */

//---------- 2. �������������
#define CDCA_MAXNUM_PROGRAMBYCW       4U    /* һ������������Ľ�Ŀ�� */
#define CDCA_MAXNUM_ECM               8U    /* ͬʱ���մ����ECMPID��������� */

#define CDCA_MAXNUM_DETITLE            5U    /* ÿ����Ӫ���¿ɱ���ķ���Ȩ����� */

//---------- 3. CAS��ʾ��Ϣ
#define CDCA_MESSAGE_CANCEL_TYPE      0x00  /* ȡ����ǰ����ʾ */
#define CDCA_MESSAGE_BADCARD_TYPE     0x01  /* �޷�ʶ�� */
#define CDCA_MESSAGE_EXPICARD_TYPE    0x02  /* ���ܿ����ڣ�������¿� */
#define CDCA_MESSAGE_INSERTCARD_TYPE  0x03  /* ���Ž�Ŀ����������ܿ� */
#define CDCA_MESSAGE_NOOPER_TYPE      0x04  /* ���в����ڽ�Ŀ��Ӫ�� */
#define CDCA_MESSAGE_BLACKOUT_TYPE    0x05  /* �������� */
#define CDCA_MESSAGE_OUTWORKTIME_TYPE 0x06  /* ��ǰʱ�α��趨Ϊ���ܹۿ� */
#define CDCA_MESSAGE_WATCHLEVEL_TYPE  0x07  /* ��Ŀ��������趨�Ĺۿ����� */
#define CDCA_MESSAGE_PAIRING_TYPE     0x08  /* ���ܿ��뱾�����в���Ӧ */
#define CDCA_MESSAGE_NOENTITLE_TYPE   0x09  /* û����Ȩ */
#define CDCA_MESSAGE_DECRYPTFAIL_TYPE 0x0A  /* ��Ŀ����ʧ�� */
#define CDCA_MESSAGE_NOMONEY_TYPE     0x0B  /* ���ڽ��� */
#define CDCA_MESSAGE_ERRREGION_TYPE   0x0C  /* ������ȷ */
#define CDCA_MESSAGE_NEEDFEED_TYPE    0x0D  /* �ӿ���Ҫ��ĸ����Ӧ�������ĸ�� */
#define CDCA_MESSAGE_ERRCARD_TYPE     0x0E  /* ���ܿ�У��ʧ�ܣ�����ϵ��Ӫ�� */
#define CDCA_MESSAGE_UPDATE_TYPE      0x0F  /* ���ܿ������У��벻Ҫ�ο����߹ػ� */
#define CDCA_MESSAGE_LOWCARDVER_TYPE  0x10  /* ���������ܿ� */
#define CDCA_MESSAGE_VIEWLOCK_TYPE    0x11  /* ����Ƶ���л�Ƶ�� */
#define CDCA_MESSAGE_MAXRESTART_TYPE  0x12  /* ���ܿ���ʱ���ߣ���5���Ӻ����¿��� */
#define CDCA_MESSAGE_FREEZE_TYPE      0x13  /* ���ܿ��Ѷ��ᣬ����ϵ��Ӫ�� */
#define CDCA_MESSAGE_CALLBACK_TYPE    0x14  /* ���ܿ�����ͣ����ش����Ӽ�¼����Ӫ�� */
#define CDCA_MESSAGE_STBLOCKED_TYPE   0x20  /* ������������ */

//---------- 4. ���ܵ��÷���ֵ����
#define CDCA_RC_OK                    0x00  /* �ɹ� */
#define CDCA_RC_UNKNOWN               0x01  /* δ֪���� */
#define CDCA_RC_POINTER_INVALID       0x02  /* ָ����Ч */
#define CDCA_RC_CARD_INVALID          0x03  /* ���ܿ���Ч */
#define CDCA_RC_PIN_INVALID           0x04  /* PIN����Ч */
#define CDCA_RC_DATASPACE_SMALL       0x06  /* �����Ŀռ䲻�� */
#define CDCA_RC_CARD_PAIROTHER        0x07  /* ���ܿ��Ѿ���Ӧ��Ļ����� */
#define CDCA_RC_DATA_NOT_FIND         0x08  /* û���ҵ���Ҫ������ */
#define CDCA_RC_PROG_STATUS_INVALID   0x09  /* Ҫ����Ľ�Ŀ״̬��Ч */
#define CDCA_RC_CARD_NO_ROOM          0x0A  /* ���ܿ�û�пռ��Ź���Ľ�Ŀ */
#define CDCA_RC_WORKTIME_INVALID      0x0B  /* �趨�Ĺ���ʱ����Ч */
#define CDCA_RC_IPPV_CANNTDEL         0x0C  /* IPPV��Ŀ���ܱ�ɾ�� */
#define CDCA_RC_CARD_NOPAIR           0x0D  /* ���ܿ�û�ж�Ӧ�κεĻ����� */
#define CDCA_RC_WATCHRATING_INVALID   0x0E  /* �趨�Ĺۿ�������Ч */
#define CDCA_RC_CARD_NOTSUPPORT       0x0F  /* ��ǰ���ܿ���֧�ִ˹��� */
#define CDCA_RC_DATA_ERROR            0x10  /* ���ݴ������ܿ��ܾ� */
#define CDCA_RC_FEEDTIME_NOT_ARRIVE   0x11  /* ι��ʱ��δ�����ӿ����ܱ�ι�� */
#define CDCA_RC_CARD_TYPEERROR        0x12  /* ��ĸ��ι��ʧ�ܣ��������ܿ����ʹ��� */

//---------- 5. ECM_PID���õĲ�������
#define CDCA_LIST_OK                  0x00
#define CDCA_LIST_FIRST               0x01
#define CDCA_LIST_ADD                 0x02

//---------- 6. �ʼ�Ӧ����ض���
//------------ 6.1. �ʼ���С����������
#define CDCA_MAXNUM_EMAIL             100U   /* �����б��������ʼ����� */
#define CDCA_MAXLEN_EMAIL_TITLE       30U    /* �ʼ�����ĳ��� */
#define CDCA_MAXLEN_EMAIL_CONTENT     160U   /* �ʼ����ݵĳ��� */

//------------ 6.2. �ʼ�ͼ����ʾ��ʽ
#define CDCA_Email_IconHide           0x00  /* �����ʼ�֪ͨͼ�� */
#define CDCA_Email_New                0x01  /* ���ʼ�֪ͨ����ʾ���ʼ�ͼ�� */
#define CDCA_Email_SpaceExhaust       0x02  /* ���̿ռ�������ͼ����˸�� */

//---------- 7. OSDӦ����ض���
//------------ 7.1. OSD�ĳ�������
#define CDCA_MAXLEN_OSD               180U  /* OSD���ݵ���󳤶� */

//------------ 7.2. OSD��ʾ����
#define CDCA_OSD_TOP                  0x01  /* OSD�����ʾ����Ļ�Ϸ� */
#define CDCA_OSD_BOTTOM               0x02  /* OSD�����ʾ����Ļ�·� */
#define CDCA_OSD_FULLSCREAN           0x03  /* OSD���������ʾ */
#define CDCA_OSD_HALFSCREAN           0x04  /* OSD��񣺰�����ʾ */


//---------- 8. IPPV/IPPTӦ����ض���
//------------ 8.1. IPPV/IPPT��ͬ����׶���ʾ
#define CDCA_IPPV_FREEVIEWED_SEGMENT  0x00  /* IPPV���Ԥ���׶Σ��Ƿ��� */
#define CDCA_IPPV_PAYVIEWED_SEGMENT   0x01  /* IPPV�շѽ׶Σ��Ƿ��� */
#define CDCA_IPPT_PAYVIEWED_SEGMENT   0x02  /* IPPT�շѶΣ��Ƿ��� */

//------------ 8.2. IPPV�۸�����
#define CDCA_IPPVPRICETYPE_TPPVVIEW       0x0  /* ���ش�����¼������ */
#define CDCA_IPPVPRICETYPE_TPPVVIEWTAPING 0x1  /* ���ش�����¼������ */

//------------ 8.3. IPPV��Ŀ��״̬
#define CDCA_IPPVSTATUS_BOOKING       0x01  /* Ԥ�� */
#define CDCA_IPPVSTATUS_VIEWED        0x03  /* �ѿ� */


//---------- 9. Ƶ������Ӧ����ض���
#define CDCA_MAXNUM_COMPONENT         5U    /* ��Ŀ��������� */
#define CDCA_MAXLEN_LOCKMESS          40U

//---------- 10. ����Ȩȷ����Ӧ����ض���
#define CDCA_Detitle_All_Readed       0x00  /* ���з���Ȩȷ�����Ѿ�����������ͼ�� */
#define CDCA_Detitle_Received         0x01  /* �յ��µķ���Ȩ�룬��ʾ����Ȩ��ͼ�� */
#define CDCA_Detitle_Space_Small      0x02  /* ����Ȩ��ռ䲻�㣬�ı�ͼ��״̬��ʾ�û� */
#define CDCA_Detitle_Ignore           0x03  /* �յ��ظ��ķ���Ȩ�룬�ɺ��ԣ��������� */
	
//---------- 10. ��������ʾ��Ϣ
#define CDCA_SCALE_RECEIVEPATCH       1     /* �������ݽ����� */
#define CDCA_SCALE_PATCHING           2     /* ���ܿ������� */

/*-------------------------------------end of �궨��--------------------------------------*/



/*----------------------------------------���ݽṹ----------------------------------------*/

//---------- 1. ϵͳʱ��
typedef CDCA_U32  CDCA_TIME;
typedef CDCA_U16  CDCA_DATE;

//---------- 2. �ź������壨��ͬ�Ĳ���ϵͳ���ܲ�һ����
typedef CDCA_U32  CDCA_Semaphore;

//---------- 3. ��Ӫ����Ϣ
typedef struct {
	char  m_szTVSPriInfo[CDCA_MAXLEN_TVSPRIINFO+1];  /* ��Ӫ��˽����Ϣ */
}SCDCAOperatorInfo;

//---------- 4. ��Ŀ��Ϣ
typedef struct {
	CDCA_U16	m_wEcmPid;														/*��Ŀ��Ӧ������Ϣ��PID*/				
	CDCA_U8		m_byServiceNum;												/*��ǰPID�µĽ�Ŀ����*/
	CDCA_U8   m_byReserved;                         /* ���� */
	CDCA_U16 	m_wServiceID[CDCA_MAXNUM_PROGRAMBYCW];	/*��ǰPID�µĽ�ĿID�б�*/	
}SCDCASServiceInfo;

//---------- 5. ��ͨ��Ȩ��Ϣ
//------------ 5.1. ��Ȩ��Ϣ
typedef struct {
	CDCA_U32 		m_dwProductID;		/*��ͨ��Ȩ�Ľ�ĿID*/
	CDCA_DATE		m_tBeginDate;		/*��Ȩ����ʼʱ��*/
	CDCA_DATE		m_tExpireDate;		/*��Ȩ�Ĺ���ʱ��*/
	CDCA_U8   	m_bCanTape;     /* �û��Ƿ���¼��1������¼��0��������¼�� */
	CDCA_U8   	m_byReserved[3];/* ���� */
}SCDCAEntitle;

//------------ 5.2. ��Ȩ��Ϣ����
typedef struct {
	CDCA_U16      m_wProductCount;
  CDCA_U8       m_m_byReserved[2];               /* ���� */
  SCDCAEntitle  m_Entitles[CDCA_MAXNUM_ENTITLE]; /* ��Ȩ�б� */
}SCDCAEntitles;

//---------- 6. IPPV��Ϣ
//------------ 6.1. Ǯ����Ϣ
typedef struct {
  CDCA_U32  m_wCreditLimit; /* ���öȣ�������*/
  CDCA_U32  m_wBalance;     /* �ѻ��ĵ��� */
}SCDCATVSSlotInfo;

//------------ 6.2. IPPV��Ŀ�ļ۸�
typedef struct {
  CDCA_U16  m_wPrice;       /* ��Ŀ�۸񣨵�����*/
  CDCA_U8   m_byPriceCode;  /* ��Ŀ�۸����� */
  CDCA_U8   m_byReserved;   /* ���� */
}SCDCAIPPVPrice; 

//------------ 6.3. IPPV��Ŀ������ʾ��Ϣ
typedef struct {
  CDCA_U32        m_dwProductID;              /* ��Ŀ��ID */
  CDCA_U16        m_wTvsID;                   /* ��Ӫ��ID */
  CDCA_U8         m_bySlotID;                 /* Ǯ��ID */
  CDCA_U8         m_byPriceNum;               /* ��Ŀ�۸���� */
  SCDCAIPPVPrice  m_Price[CDCA_MAXNUM_PRICE]; /* ��Ŀ�۸� */
	CDCA_DATE       m_wExpiredDate;             /* ��Ŀ����ʱ�� */
}SCDCAIppvBuyInfo;

//------------ 6.4. IPPV��Ŀ��Ϣ
typedef struct {
	CDCA_U32   m_dwProductID;       /* ��Ŀ��ID */
  CDCA_U8    m_byBookEdFlag;      /* ��Ʒ״̬��BOOKING��VIEWED */ 
  CDCA_U8    m_bCanTape;          /* �Ƿ����¼��1������¼��0��������¼�� */
  CDCA_U16   m_wPrice;            /* ��Ŀ�۸� */
  CDCA_DATE  m_wExpiredDate;      /* ��Ŀ����ʱ�� */
  CDCA_U8    m_bySlotID;          /* Ǯ��ID */
}SCDCAIppvInfo;


//---------- 7. �ʼ���Ϣ
//------------ 7.1. �ʼ�ͷ
typedef struct {
  CDCA_U32   m_dwActionID;                             /* Email ID */
  CDCA_U8    m_bNewEmail;                              /* ���ʼ���ǣ�0���������ʼ���1�����ʼ� */
  char       m_szEmailHead[CDCA_MAXLEN_EMAIL_TITLE+1]; /* �ʼ����⣬�Ϊ30 */        
	CDCA_U32   m_tCreateTime;                            /* EMAIL������ʱ�� */
  CDCA_U16   m_wImportance;                            /* ��Ҫ�ԣ� 0����ͨ��1����Ҫ */
}SCDCAEmailHead;

//------------ 7.2. �ʼ�����
typedef struct {
  char    m_szEmail[CDCA_MAXLEN_EMAIL_CONTENT+1];      /* Email������ */
}SCDCAEmailContent;


//---------- 8. Ƶ��������Ϣ
//------------ 8.1. ��Ŀ�����Ϣ
typedef struct {    /* �������֪ͨ�����н�Ŀ���ͼ�PID����Ϣ��һ����Ŀ���ܰ��������� */
  CDCA_U16   m_wCompPID;     /* ���PID */
  CDCA_U16   m_wECMPID;      /* �����Ӧ��ECM����PID���������ǲ����ŵģ���Ӧȡ0�� */
  CDCA_U8    m_CompType;     /* ������� */
  CDCA_U8    m_byReserved[3];/* ���� */
}SCDCAComponent;

//------------ 8.2. Ƶ��������Ϣ
typedef struct {        
  CDCA_U32       m_dwFrequency;                          /* Ƶ�ʣ�BCD�� */
  CDCA_U32       m_symbol_rate;                          /* �����ʣ�BCD�� */
  CDCA_U16       m_wPcrPid;                              /* PCR PID */
  CDCA_U8        m_fec_outer;                            /* ǰ��������� */
  CDCA_U8        m_fec_inner;                            /* ǰ��������� */
  CDCA_U8        m_Modulation;                           /* ���Ʒ�ʽ */
  CDCA_U8        m_ComponentNum;                         /* ��Ŀ������� */
  SCDCAComponent m_CompArr[CDCA_MAXNUM_COMPONENT];       /* ��Ŀ����б� */
  char           m_szBeforeInfo[CDCA_MAXLEN_LOCKMESS+1]; /* ���� */
  char           m_szQuitInfo[CDCA_MAXLEN_LOCKMESS+1];   /* ���� */
  char           m_szEndInfo[CDCA_MAXLEN_LOCKMESS+1];    /* ���� */
}SCDCALockService;
	
/*-----------------------------------------------------------------------------------
a. ��ϵͳ�У�����m_dwFrequency��m_symbol_rateʹ��BCD�룬����ǰȡMHzΪ��λ��
   ����ʱ��ǰ4��4-bit BCD���ʾС����ǰ��ֵ����4��4-bit BCD���ʾС������ֵ��
   ���磺
		��Ƶ��Ϊ642000KHz����642.0000MHz�����Ӧ��m_dwFrequency��ֵӦΪ0x06420000��
		��������Ϊ6875KHz����6.8750MHz�����Ӧ��m_symbol_rate��ֵӦΪ0x00068750��

b. ��ϵͳ�У�m_Modulation��ȡֵ���£�
	0		Reserved
	1		QAM16
	2		QAM32
	3		QAM64
	4		QAM128
	5		QAM256
	6��255	Reserved
------------------------------------------------------------------------------------*/ 


/*------------------------------------end of ���ݽṹ-------------------------------------*/



/*---------------------------���½ӿ���CA_LIB�ṩ��STB-------------------------------------*/

//-------------------- CA_LIB���ȹ��� --------------------
/*++
 * ��CA�н�������������byThreadPrior�����⼸����������ȼ�
-*/
extern CDCA_BOOL  CDCASTB_Init(CDCA_U8 byThreadPrior);

/*++
 * ����������������ж��Ƿ���CASYSTEMID�ܹ�����ǰCAʶ��	
-*/
extern CDCA_BOOL  CDCASTB_IsCDCa(CDCA_U16 wCaSystemID);

//-------------------- Flash���� --------------------
/*+
 *  ��������������¸�ʽ�������CA������flash 
 *  ���flash���������ڲ������ػ���������ԭ���´���ɵ�������������¸�ʽ��flash
 *  Ŀǰֻ֧��FormatTypeȡֵΪ0��Ӧ�ã���0ʱCALIB����flash���κβ���
-*/
extern void  CDCASTB_FormatBuffer(CDCA_U8 byFormatType);
extern void  CDCASTB_RequestMaskBuffer(void);
extern void  CDCASTB_RequestUpdateBuffer(void);

//-------------------- TS������ --------------------
/*++
	if(�û��л�Ƶ��||���ߵ�ǰƵ����ECMPID�������ӻ��߼���)
	{
		CDCASTB_SetEcmPid(CDCA_LIST_FIRST,NULL);
		if(�����ǰƵ������ECMPID,�Ƚ�Ŀ�Ǽ��ŵ�) //�м���ECMPID�ͼ��Σ�ECMPID��ͬ��һ�ξͿ���
			CDCASTB_SetEcmPid(CDCA_LIST_ADD,pServiceInfo);
		CDCASTB_SetEcmPid(CDCA_LIST_OK,NULL);
	}
--*/
extern void  CDCASTB_SetEcmPid(CDCA_U8 byType,  SCDCASServiceInfo* pServiceInfo);
extern void  CDCASTB_SetEmmPid(CDCA_U16 wEmmPid);
extern void  CDCASTB_PrivateDataGot(CDCA_U8 byReqID, CDCA_BOOL bOK, CDCA_U16 wPid, const CDCA_U8* pbyReceiveData, CDCA_U16 wLen);

//-------------------- ���ܿ����� --------------------
extern CDCA_BOOL  CDCASTB_SCInsert(void);
extern void  			CDCASTB_SCRemove(void);
/*
 ˵�� ��
	��ȡ���ܿ����кţ����ܿ����к���16���ֽ���ɵ��ַ���
���ӣ�
	char			sn[CDCA_MAXLEN_SN+1];
	CDCA_U16	wRet = 0;
	wRet= CDCASTB_GetCardSN(sn);
	if(wRet==CDCA_RC_OK)
		printf("SN:%s",sn);
	}
 */
extern CDCA_U16  CDCASTB_GetCardSN(char* pCardSN);

extern CDCA_U16  CDCASTB_ChangePin( const CDCA_U8* pbyOldPin, const CDCA_U8* pbyNewPin);
/*++

����˵����
   byRating�ķ�Χ��[4,18],������ 
*/
extern CDCA_U16  CDCASTB_SetRating(const CDCA_U8* pbyPin, CDCA_U8 byRating);
extern CDCA_U16  CDCASTB_GetRating(CDCA_U8* pbyRating);

extern CDCA_U16  CDCASTB_SetWorkTime(const CDCA_U8* pbyPin, CDCA_U8 byStartHour, CDCA_U8 byStartMin, CDCA_U8 byStartSec, CDCA_U8 byEndHour, CDCA_U8 byEndMin, CDCA_U8 byEndSec);
extern CDCA_U16  CDCASTB_GetWorkTime(CDCA_U8* pbyStartHour, CDCA_U8* pbyStartMin, CDCA_U8* pbyStartSec, CDCA_U8* pbyEndHour, CDCA_U8* pbyEndMin, CDCA_U8* pbyEndSec);
//-------------------- ������Ϣ��ѯ --------------------
extern CDCA_U32 CDCASTB_GetVer(void);
/*++
˵�� ��
	���pwTVSID����ָ��CDCA_MAXNUM_OPERATOR��CDCA_U16����ĵ�ַ�����û����Ӫ��pwTVSID[i]ֵ��0
���ӣ�
	CDCA_U16  wArrTvsID[CDCA_MAXNUM_OPERATOR];
	CDCA_U8   index=0;
	SCDCAOperatorInfo OperatorInfo;
	if(CDCASTB_GetOperatorIds(wArrTvsID) == CDCA_RC_OK)
	{
		for(index=0,index<CDCA_MAXNUM_OPERATOR;index++)
		{	
			if(wArrTvsID[index]==0)
				break;
			if(CDCASTB_GetOperatorInfo(wArrTvsID[index],&OperatorInfo) == CDCA_RC_OK)
			{
				printf("��Ӫ��ID�� %d ��Ӫ�����ƣ�%s \r\n",wArrTvsID[i],OperatorInfo.m_szTVSPriInfo);
			}
		}
	}
--*/
extern CDCA_U16 CDCASTB_GetOperatorIds(CDCA_U16* pwTVSID);
extern CDCA_U16 CDCASTB_GetOperatorInfo(CDCA_U16 wTVSID,  SCDCAOperatorInfo* pOperatorInfo);
/*++
˵�� ��
	���pACArray����ָ��CDCA_MAXNUM_ACLIST��CDCA_U32����ĵ�ַ
���ӣ�
		CDCA_U32  ACArray[CDCA_MAXNUM_ACLIST];
    CDCA_U16  wOperatorID = 12345;
    if(CDCASTB_GetACList(wOperatorID,ACArray) == CDCA_RC_OK)
	{
		printf("Get operator AC OK!);
	}
--*/
extern CDCA_U16 CDCASTB_GetACList(CDCA_U16 wTVSID, CDCA_U32* pACArray);
/*++
˵�� ��
	���ĳһ����Ӫ�̵���Ȩ��Ϣ
���ӣ�
	 SCDCAEntitles Entitle;
        CDCA_U16        wRet = 0;
        CDCA_U16        wIndex;
        wRet= CDCASTB_GetServiceEntitles(1,&Entitle);
        if(wRet == CDCA_RC_OK)
        {
                printf("������Ȩ %d��",Entitle.m_wProductCount);
                for(wIndex =0 ; wIndex < Entitle.m_wProductCount ; wIndex++)
                {
                        printf("��ĿID %d\n:",Entitle.m_Entitles[wIndex].m_dwProductID);
                        printf("��Ŀ�Ƿ�����¼�� %d\n:",Entitle.m_Entitles[wIndex].m_bCanTape);
                        printf("��Ȩ����ʱ��\n:",Entitle.m_Entitles[wIndex].m_tExpireDate);
                }
        }
        else
                printf("��Ӧ�Ĵ���");
*/
extern CDCA_U16 CDCASTB_GetServiceEntitles(CDCA_U16 wTVSID, SCDCAEntitles* pServiceEntitles);

//-------------------- ��Ȩ��Ϣ���� --------------------
/*++
˵�� ��
	���ĳһ����Ӫ�̵���ȨID��Ϣ
���ӣ�
	          CDCA_U32   EntitleIds[CDCA_MAXNUM_ENTITLE];
         CDCA_U16 wRet = 0,wOperatorID=1;
         wRet = CDCASTB_GetEntitleIDs(wOperatorID,EntitleIds);
         if(wRet == CDCA_RC_OK)
         {
          CDCA_U16 wIndex=0;
          for(wIndex =0 ; wIndex < CDCA_MAXNUM_ENTITLE ; wIndex++)
          {
           if(EntitleIds[wIndex]==0) break;
           printf("��ĿID %d\n:",EntitleIds[wIndex]);
          }
         }
         else
          printf("��Ӧ�Ĵ���");
*/
extern CDCA_U16 CDCASTB_GetEntitleIDs(CDCA_U16 wTVSID, CDCA_U32* pdwEntitleIds);

extern CDCA_U16  CDCASTB_GetDetitleChkNums(CDCA_U16 wTvsID,  CDCA_U32 *pdwDetitleChkNums);
extern CDCA_BOOL CDCASTB_GetDetitleReaded(CDCA_U16 wTvsID);
extern CDCA_BOOL CDCASTB_DelDetitleChkNum(CDCA_U16 wTvsID, CDCA_U32 dwDetitleChkNum);

//-------------------- ������Ӧ --------------------

/*++
˵�� ��
	��鵱ǰ�����к����ܿ���Ӧ��������������ܿ���Ӧ�Ļ����е�STBID�б�
		pbyNum����ǰ���ܿ���Ӧ�Ļ����е����������Ϊ5
		pbySTBID_List��ָ��һ�����ݻ��������ڷ���STBID�б�ÿ��STBIDΪ6�ֽ�
���ӣ�
 				CDCA_U16  wRet = 0;
        wRet = CDCASTB_IsPaired(pbyNum, pbySTBID_List);
        if(wRet == CDCA_RC_OK)
                printf("��ǰ������Ӧ" & STBID_List);        
        else if(wRet == CDCA_RC_CARD_INVALID)
                printf("��ⲻ����Ч���ܿ�");        
        else if(wRet == CDCA_RC_CARD_NOPAIR)
                printf("���ܿ�û�ж�Ӧ�κλ�����" & "�����û�����Ӧ");        
        else if(wRet == CDCA_RC_CARD_PAIROTHER)
                printf("���ܿ��Ѿ���Ӧ����������" & STBID_List);        
--*/
extern CDCA_U16 CDCASTB_IsPaired(CDCA_U8* pbyNum, CDCA_U8* pbySTBID_List);
extern CDCA_U16 CDCASTB_GetPlatformID(void);

//-------------------- IPPVӦ�� --------------------

	/*++
	˵�� ��
		���byArrSlotID����ָ��CDCA_MAXLEN_SLOT����ĵ�ַ
	���ӣ�
		CDCA_U8  byArrSlotID[CDCA_MAXNUM_SLOT];
        if(CDCASTB_GetSlotIDs(1,byArrSlotID) == CDCA_RC_OK)
        {
                for(CDCA_U8 i = 0;i<CDCA_MAXNUM_SLOT;i++)
                {
                        if(byArrSlotID[i] != 0)
                        {
                                printf("Ǯ��ID �� %4x\r\n",byArrSlotID[i]);
                        }
                        else
                                break;
                }
        }
	--*/
	extern CDCA_U16 CDCASTB_GetSlotIDs(CDCA_U16 wTVSID,  CDCA_U8* pbySlotID);
	extern CDCA_U16 CDCASTB_GetSlotInfo(CDCA_U16 wTVSID, CDCA_U8 bySlotID, SCDCATVSSlotInfo* pSlotInfo);
	/*++
	˵�� ��
		���һ����Ӫ�̵����е�IPPV��Ŀ
	���ӣ�
		 CDCA_U16  wNumber;
        SCDCAIppvInfo  ArrIppv[CDCA_MAXNUM_IPPVP];
        wNumber = CDCA_MAXNUM_IPPVP;
        if(CDCASTB_GetIPPVProgram(1,ArrIppv,&wNumber) == CDCA_RC_OK)
        {
                for(CDCA_U8 i = 0;i<wNumber;i++)
                {
                        printf("��ĿID �� %4x\r\n",ArrIppv[i].m_wProductID);
                }
        }
	--*/
	extern CDCA_U16 CDCASTB_GetIPPVProgram(CDCA_U16 wTVSID,  SCDCAIppvInfo* pIppv, CDCA_U16* pwNumber);
	/*++
	˵�� ��
		�û���IPPV��Ŀ�Ƿ������ѡ���ʱ�򣬵��øú���
	���ӣ�
	   ��IPPV��ʾ����ĶԻ�����ʾ�ڵ����ϵ�ʱ��
        CDCA_U16 wRet =0;
        if(�û�ѡ����)
        {
                �û�����PIN;
                CDCA_U8 byPin[CDCA_MAXLEN_PINCODE]={�û������PIN};
                wRet = CDCASTB_StopIPPVBuyDlg(CDCA_TRUE, wEcmPid,byPin,Price);
                switch(wRet)
                {
                        case CDCA_RC_OK;
                                printf("����ɹ�");
                                break;
                        case other:
                                printf("������Ӧ����");
                                break;
                        default:
                                break;
                }
        }        
        else
                CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,wEcmPid,NULL,NULL);
	 */
	extern CDCA_U16 CDCASTB_StopIPPVBuyDlg(CDCA_BOOL bBuyProgram, CDCA_U16 wEcmPid, CDCA_U8* pbyPinCode, SCDCAIPPVPrice* pPrice);


//-------------------- �ʼ����� --------------------

	/*++
	˵�� ��
		һ�·������Ի��������ȫ���ʼ�ͷ��Ϣ
	ע�⣺
		byFromIndex �� 5�����Ǵ��߼��ϵĵ�����ſ�ʼȡ�����Ǵ���������ռ�ĵ�5���ʼ�λ�ÿ�ʼȡ��
	���ӣ�
		 SCDCAEmailHead  ArrEmailHead[10];
        CDCA_U8                        byCount;
        CDCA_U8                        byFromIndex = 0;
        while(CDCA_TRUE)
        {
                byCount = 10;
                if(CDCASTB_GetEmailHeads(ArrEmailHead,&byCount,&byFromIndex) == CDCA_RC_OK)
                {
                        for(CDCA_U8 i = 0;i<byCount;i++)
                        {
                                SCDCAEmailContent EmailContent;
                                printf("�ʼ�ID �� 0x%8x\r\n",ArrEmailHead[i].m_dwActionID);
                                ArrEmailHead[i].m_szEmailHead[CDCA_MAXLEN_EMAIL_TITLE] = 0; //��ֹ���������
                                printf("�ʼ����� �� %s\r\n",ArrEmailHead[i].m_szEmailHead);        
                                if(CDCASTB_GetEmailContent(ArrEmailHead[i].m_dwActionID,&EmailContent)==CDCA_RC_OK)
                                {
                                        EmailContent.m_szEmail[CDCA_MAXLEN_EMAIL_CONTENT] = 0;//��ֹ���������
                                        printf("�ʼ����� �� %s\r\n",EmailContent.m_szEmail);        
                                }
                                //�����ɾ���ʼ�
                                CDCASTB_DelEmail(ArrEmailHead[i].m_dwActionID);
                        }
                }
                if(byCount < 10) //�����ʼ����Ѿ�ȡ��
                        break;
        }
	--*/
	extern CDCA_U16 CDCASTB_GetEmailHeads( SCDCAEmailHead* pEmailHead,  CDCA_U8* pbyCount,  CDCA_U8* pbyFromIndex);
	extern CDCA_U16 CDCASTB_GetEmailHead(CDCA_U32 dwEmailID,  SCDCAEmailHead* pEmailHead);
	extern CDCA_U16 CDCASTB_GetEmailContent(CDCA_U32 dwEmailID,  SCDCAEmailContent* pEmailContent);
		/*++
	˵�� ��
		ɾ��ĳһ���ʼ��ķ�����TFCASTB_GetEmailHeadsʹ�ý������Ѿ�����
		�������ɾ�������ʼ��ķ��� 
	˵����
		ʹ��CDCASTB_DelEmail��ɾ�������ʼ���
	���ӣ�
		CDCASTB_DelEmail(0);
	--*/
	extern void	CDCASTB_DelEmail(CDCA_U32 dwEmailID);
	/*++
	˵�� ��
		�������ʹ�õ����
	���ӣ�
				CDCA_U8    byEmailNum;
        CDCA_U8    byEmptyNum;
        if(CDCASTB_GetEmailSpaceInfo(&byEmailNum,&byEmptyNum) == CDCA_RC_OK)
        {
                printf("���������ʼ� �� %d��\n",byEmailNum);
                printf("���仹�������ʼ� �� %d��\n",byEmptyNum);
        }
	--*/
	extern CDCA_U16	CDCASTB_GetEmailSpaceInfo( CDCA_U8* pbyEmailNum,  CDCA_U8* pbyEmptyNum);


//-------------------- ��ĸ��Ӧ�� --------------------
/*
 ˵�� ��
	��ĸ����ȡ��Ӧ���ݣ�ֻ��ĸ����Ч
 ������
    pFeedData��ָ�򷵻����ݵĿռ䣬�ռ��С��С��256�ֽ�
	pbyLen��   ���شӿ��ж�ȡ�����ݵĳ��� 
���ӣ�
	
        CDCA_U8        data[256];
        CDCA_U8        len = 0;
        wRet= CDCASTB_ReadFeedDataFromParent(data,&len);
        if(wRet==CDCA_RC_OK)
                printf("��ȡĸ������");
        }

 */
extern CDCA_U16  CDCASTB_ReadFeedDataFromParent(CDCA_U16 wTVSID,  CDCA_U8* pbyFeedData,  CDCA_U8* pbyLen);
/*
 ˵�� ��
	�Ѵ�ĸ����ȡ������д���ӿ���ֻ���ӿ���Ч
 ������
    pFeedData��ָ��д���ӿ������ݵĿռ䣬ָ��������Ǵӵ���CDCASTB_ReadFeedDataFromParent��ĸ���з��ص�
        byLen��    д���ӿ������ݳ��� 
���ӣ�
        wRet= CDCASTB_WriteFeedDataToChild(data,len);
        if(wRet==CDCA_RC_OK)
                printf("д��Ӧ���ݳɹ�");
        }
 */
extern CDCA_U16  CDCASTB_WriteFeedDataToChild( CDCA_U8* pbyFeedData, CDCA_U8 byLen);
/*++
˵�� ��
	��ȡ��Ӫ�̵���ĸ��״̬
������
	  		wTVSID����Ҫ��ѯ����Ӫ��ID
        pbIsChild���������turue��1����������Ӫ�̵�ϵͳ�ڣ��˿����ӿ�
        pbyDelayTime�����ζ�Ӧ��ʱ��������λ��Сʱ��ֻ�����ӿ���ʱ���ֵ����Ч
        pLastFeedTime���ϴζ�Ӧ��ʱ�䡣ֻ�����ӿ���ʱ���ֵ����Ч
���ӣ�
        CDCA_TIME  pLastFeedTime;
        CDCA_U8    byDelayTime;
        CDCA_BOOL    bIsChild;
    		CDCA_U16   wOperatorID = 12345;
    		if(CDCASTB_GetOperatorChildStatus(wOperatorID,&bIsChild,&byDelayTime,&pLastFeedTime) == CDCA_RC_OK)
        {
                printf("��ȡ��Ӫ����ĸ����Ϣ!);
        }
--*/
extern CDCA_U16 CDCASTB_GetOperatorChildStatus(CDCA_U16 wTVSID,  CDCA_U8* pbyIsChild,  CDCA_U8* pbyDelayTime,  CDCA_TIME* pLastFeedTime);


/*-------------------- ��ʾ������� --------------------*/
extern void CDCASTB_RefreshInterface(void);


/*------------------------���Ͻӿ���CA_LIB�ṩ��STB----------------------------------------*/

	
/*------------------------���½ӿ���STB�ṩ��CA_LIB----------------------------------------*/

//-------------------- �̹߳��� --------------------
extern CDCA_BOOL CDSTBCA_RegisterTask( const char* szName, CDCA_U8 byPriority,  void* pTaskFun,  void* pParam, CDCA_U16 wStackSize);
extern void      CDSTBCA_Sleep(CDCA_U16 wMilliSeconds);

//-------------------- �ź������� --------------------
/*++
˵�� ��
	���ź������г�ʼ��
���õ����ӣ�
	 			CDCA_Semaphore  Semaphore1;
        CDSTBCA_SemaphoreInit(&Semaphore1,CDCA_TRUE);
        CDCA_Semaphore  Semaphore2;
        CDSTBCA_SemaphoreInit(&Semaphore2,CDCA_FALSE);
        ��� Semaphore1���ź�
           ��Semaphore2û���ź�
--*/
extern void CDSTBCA_SemaphoreInit( CDCA_Semaphore* pSemaphore, CDCA_BOOL bInitVal);
extern void CDSTBCA_SemaphoreSignal( CDCA_Semaphore* pSemaphore );
extern void CDSTBCA_SemaphoreWait( CDCA_Semaphore* pSemaphore );

//-------------------- �ڴ���� --------------------
/*++

--*/
extern void* CDSTBCA_Malloc(CDCA_U8 byBufSize);
extern void  CDSTBCA_Free(void* pBuf);
extern void  CDSTBCA_Memset(void* pDestBuf, CDCA_U8 c, CDCA_U16 wSize);
extern void  CDSTBCA_Memcpy( void* pDestBuf, const void* pSrcBuf, CDCA_U16 wSize );

//-------------------- Flash���� --------------------
/*++
˵�� ��
    ��ȡfalsh�Ŀ�ʼ��ַ�ͳ���,���п�ʼ��ַ��CAģ�鲻���޸�
���õ����ӣ�
		CDCA_U32 g_STBMemBeginAdd = 0,g_STBMemLen = 0;
		CDSTBCA_GetBuffer(&g_STBMemBeginAdd,&g_STBMemLen);
		if(g_STBMemLen<64*1024){
				CDSTBCA_Printf(0,"flash is small\r\n");
		}
������ʵ�ֵ�����:
		CDCA_U32  dwCaFlashBeginAddress = 0x80000000;
		CDSTBCA_GetBuffer( CDCA_U32* pdwStartAddr,CDCA_U32* pdwSize){
		*pdwStartAddr = dwCaFlashBeginAddress;
		*pdwSize = 64*1024;
		.
		.
		.
	}
--*/
extern void CDSTBCA_GetBuffer(CDCA_U32* pdwStartAddr, CDCA_U32* pdwSize);
/*++
˵�� ��
	CAģ��ֻ��������ʱ���ȡһ��falsh
���õ����ӣ�
		CDCA_U8	 temp[1000];
		CDCA_U32 dwreadLen = 1000;
		CDSTBCA_ReadBuffer(g_STBMemBeginAdd,temp,&dwreadLen);
		if(dwReadLen != 1000){
			CDSTBCA_Printf(0,"CDSTBCA_ReadBuffer is fail");
	}
--*/
extern void CDSTBCA_ReadBuffer(CDCA_U32 dwStartAddr, CDCA_U8* pbyData, CDCA_U32* pdwLen);
/*++
˵�� ��
    CA���еĹ����л���дflash,ÿ�εĵ�ַ�ͳ��ȶ���һ����

--*/
extern void CDSTBCA_WriteBuffer(CDCA_U32 dwStartAddr, const CDCA_U8* pbyData, CDCA_U32 dwLen);

//-------------------- TS������ --------------------
/*++
    //ע�� �����EMMPID ��CAT�з�������
	 CDCA_U8 byFilter[8]={0,1,2,3,4,5,6,7};
   CDCA_U8 byMask[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
   if(CDSTBCA_GetPrivateData(2,byFilter,byMask,8, 0x211, 2)
	��{
 					//CA�ڲ����ı���Ĺ�����״̬
	��}
		else
	��{
 					// ����ʧ�ܽ���������
		}
-*/
extern void	CDSTBCA_ScrSetCW(CDCA_U16 wEcmPID, const CDCA_U8* pbyOddKey, const CDCA_U8* pbyEvenKey, CDCA_U8 byKeyLen, CDCA_BOOL bTapingEnabled);
extern CDCA_BOOL CDSTBCA_GetPrivateData(CDCA_U8 byReqID, const CDCA_U8* pbyFilter, const CDCA_U8* pbyMask, CDCA_U8 byLen, CDCA_U16 wPid, CDCA_U8 byWaitSeconds);


//-------------------- ���ܿ����� --------------------
extern CDCA_BOOL CDSTBCA_SCReset(CDCA_U8* pbyATR,  CDCA_U8* pbyLen);
/*++
   �������Ҿ�һ��ʵ�ֵ�ģ�� ������ô���ɻ����о���
 	
	  CDSTBCA_SCPBRun( const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen)
        {
            �����ֽ��Ϳ�
            if(���ɹ�)
                return CDCA_FALSE;
            ��ȡ�����ص����ݷŵ�pbyReply //ָ��ָ��Ŀռ���CA������� t0Э���з������ݳ��Ȳ������256���ֽ�
            if(���ɹ�)
                 return CDCA_FALSE;
             *pwReplyLen���ڷ��ص����ݳ���
              pbyReply[*pwReplyLen] = �����ֽ�1
              pbyReply[(*pwReplyLen)+1] = �����ֽ�2
              *pwReplyLen+=2;
              return CDCA_TRUE;
         }
-*/
extern CDCA_BOOL CDSTBCA_SCPBRun( const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen);

//-------------------- ��Ȩ��Ϣ���� --------------------
/*�����ܿ���Ȩ�����仯��ʱ�򣬵��ýӿ�֪ͨ������*/
extern void CDSTBCA_EntitleChanged(CDCA_U16 wTvsID);

extern void CDSTBCA_DetitleReceived(CDCA_U16 wTvsID, CDCA_U8 bstatus);

//-------------------- ��ȫ���� --------------------
/*��ȡ�����е�Ψһ���
 pwPlatfromID:����ͬ������������г��̵�ID��ͬһ�������г��̲�ͬоƬ���в�ͬ��ֵ
 pdwUniqueID:�����е�Ψһ��־,��ͬһ��PlatfromID����UniqueID�����г��̱���Ψһ
*/
extern void     CDSTBCA_GetSTBID( CDCA_U16* pwPlatformID,  CDCA_U32* pdwUniqueID);
extern CDCA_U16 CDSTBCA_SCFunction( CDCA_U8* pData);


//-------------------- IPPVӦ�� --------------------
extern void CDSTBCA_HideIPPVDlg(CDCA_U16 wEcmPid);
extern void CDSTBCA_StartIppvBuyDlg(CDCA_U8 byMessageType, CDCA_U16 wEcmPid,  SCDCAIppvBuyInfo *pIppvProgram);

//-------------------- �ʼ�/OSD��ʾ���� --------------------
extern void CDSTBCA_ShowOSDMessage(CDCA_U8 byStyle,  char* szMessage);
extern void CDSTBCA_HideOSDMessage(CDCA_U8 byStyle);
/*++
        ��byShow��CDCA_Email_New && dwEmailID!=0 ��ʱ��dwEmailID��һ�����ʼ���ID
         ���ʱ����Ե���        CDCASTB_GetEmailHead
                                                 CDCASTB_GetEmailContent ��ȡ����ʼ��ı��������
        ����������ж�����ʼ������ID�����е�ĳһ���ʼ���ID
         ������Ҳ���Բ��������ID����ΪCDCASTB_GetEmailHeads�᷵�����������е��ʼ��ı�����Ϣ���ݵ�
 -*/
extern void CDSTBCA_EmailNotifyIcon(CDCA_U8 byShow, CDCA_U32 dwEmailID);

//-------------------- ��ĸ��Ӧ�� --------------------
/*
 ˵�� ��
	��������н����ӿ�ι������
*/
extern void  CDSTBCA_RequestFeeding(CDCA_U16 wTVSID);


//-------------------- ǿ���л�Ƶ�� --------------------
extern void CDSTBCA_LockService( SCDCALockService* pLockService);
extern void CDSTBCA_UNLockService(void);

//-------------------- ��ʾ������� --------------------
extern void CDSTBCA_ShowProgressStrip(CDCA_U8 byProgress, CDCA_U8 byMark);
extern void CDSTBCA_ShowBuyMessage(CDCA_U16 wEcmPID, CDCA_U8 byMessageType);
extern void CDSTBCA_ShowFingerMessage(CDCA_U16 wEcmPID, CDCA_U32 dwCardID);

//-------------------- ���� --------------------
extern CDCA_U16 CDSTBCA_Strlen( char* pString);
extern void     CDSTBCA_Printf(CDCA_U8 byLevel,  const char* szMesssage);

/*---------------------------���Ͻӿ���STB�ṩ��CA_LIB-------------------------------------*/

#ifdef  __cplusplus
}
#endif
#endif
/*EOF*/

