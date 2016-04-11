/**********************************************************************************
 File Name:   cdca.h

 Description: 

 History:             
**********************************************************************************/
#ifndef _cdca_h_
#define _cdca_h_

//---------- 2. �������������
#define FYF_CDCA_MAXNUM_PROGRAMBYCW       4U    /* һ������������Ľ�Ŀ�� */
#define FYF_CDCA_MAXNUM_ECM               8U    /* ͬʱ���մ����ECMPID��������� */

#define FYF_CDCA_MAXNUM_DETITLE            5U    /* ÿ����Ӫ���¿ɱ���ķ���Ȩ����� */

//---------- 4. ���ܵ��÷���ֵ����
#define FYF_CDCA_RC_OK                    0x00  /* �ɹ� */
#define FYF_CDCA_RC_UNKNOWN               0x01  /* δ֪���� */
#define FYF_CDCA_RC_POINTER_INVALID       0x02  /* ָ����Ч */
#define FYF_CDCA_RC_CARD_INVALID          0x03  /* ���ܿ���Ч */
#define FYF_CDCA_RC_PIN_INVALID           0x04  /* PIN����Ч */
#define FYF_CDCA_RC_DATASPACE_SMALL       0x06  /* �����Ŀռ䲻�� */
#define FYF_CDCA_RC_CARD_PAIROTHER        0x07  /* ���ܿ��Ѿ���Ӧ��Ļ����� */
#define FYF_CDCA_RC_DATA_NOT_FIND         0x08  /* û���ҵ���Ҫ������ */
#define FYF_CDCA_RC_PROG_STATUS_INVALID   0x09  /* Ҫ����Ľ�Ŀ״̬��Ч */
#define FYF_CDCA_RC_CARD_NO_ROOM          0x0A  /* ���ܿ�û�пռ��Ź���Ľ�Ŀ */
#define FYF_CDCA_RC_WORKTIME_INVALID      0x0B  /* �趨�Ĺ���ʱ����Ч */
#define FYF_CDCA_RC_IPPV_CANNTDEL         0x0C  /* IPPV��Ŀ���ܱ�ɾ�� */
#define FYF_CDCA_RC_CARD_NOPAIR           0x0D  /* ���ܿ�û�ж�Ӧ�κεĻ����� */
#define FYF_CDCA_RC_WATCHRATING_INVALID   0x0E  /* �趨�Ĺۿ�������Ч */
#define FYF_CDCA_RC_CARD_NOTSUPPORT       0x0F  /* ��ǰ���ܿ���֧�ִ˹��� */
#define FYF_CDCA_RC_DATA_ERROR            0x10  /* ���ݴ������ܿ��ܾ� */
#define FYF_CDCA_RC_FEEDTIME_NOT_ARRIVE   0x11  /* ι��ʱ��δ�����ӿ����ܱ�ι�� */
#define FYF_CDCA_RC_CARD_TYPEERROR        0x12  /* ��ĸ��ι��ʧ�ܣ��������ܿ����ʹ��� */

//---------- 10. ����Ȩȷ����Ӧ����ض���
#define FYF_CDCA_Detitle_All_Readed       0x00  /* ���з���Ȩȷ�����Ѿ�����������ͼ�� */
#define FYF_CDCA_Detitle_Received         0x01  /* �յ��µķ���Ȩ�룬��ʾ����Ȩ��ͼ�� */
#define FYF_CDCA_Detitle_Space_Small      0x02  /* ����Ȩ��ռ䲻�㣬�ı�ͼ��״̬��ʾ�û� */
#define FYF_CDCA_Detitle_Ignore           0x03  /* �յ��ظ��ķ���Ȩ�룬�ɺ��ԣ��������� */
//---------- 10. ��������ʾ��Ϣ
#define FYF_CDCA_SCALE_RECEIVEPATCH       1     /* �������ݽ����� */
#define FYF_CDCA_SCALE_PATCHING           2     /* ���ܿ������� */

//---------- 7. OSDӦ����ض���
//------------ 7.1. OSD�ĳ�������
#define FYF_CDCA_MAXLEN_OSD               180U  /* OSD���ݵ���󳤶� */

//------------ 7.2. OSD��ʾ����
#define FYF_CDCA_OSD_TOP                  0x01  /* OSD�����ʾ����Ļ�Ϸ� */
#define FYF_CDCA_OSD_BOTTOM               0x02  /* OSD�����ʾ����Ļ�·� */
#define FYF_CDCA_OSD_FULLSCREAN           0x03  /* OSD���������ʾ */
#define FYF_CDCA_OSD_HALFSCREAN           0x04  /* OSD��񣺰�����ʾ */
//---------- 3. ��Ӫ����Ϣ
typedef struct {
	char  m_szTVSPriInfo[32+1];  /* ��Ӫ��˽����Ϣ */
}FYF_CDCAOperatorInfo_s;
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef enum
{
	FYF_CA_CARD_INVALID = 1,
	FYF_CA_CARD_NOPAIR,
	FYF_CA_CARD_PAIROTHER
}FYF_CA_CardPair_e;
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
#define FYF_CA_Debug printf

typedef struct _FYF_CA_EmailHead_s{
	BU32   m_dwActionID;      /* Email ID */
	BU08	m_bNewEmail;       /* ���ʼ���ǣ�0���������ʼ���1�����ʼ� */
	BU08	m_szEmailHead[31]; /* �ʼ����⣬�Ϊ30 */        
	BU32   m_tCreateTime;     /* EMAIL������ʱ�� */
	BU16   m_wImportance;     /* ��Ҫ�ԣ� 0����ͨ��1����Ҫ */
}FYF_CA_EmailHead_s;

typedef struct _FYF_CA_EmailIdx_s{
	BU08	m_Count;       /*ʵ�ʻ�õ��ʼ�����*/
	BU08	m_byFromIndex; /*byFromIndex�����Ǵ��߼��ϵĵ�byFromIndex���ſ�ʼȡ�����Ǵ���������ռ�ĵ�byFromIndex���ʼ�λ�ÿ�ʼȡ*/        
}FYF_CA_EmailIdx_s;

typedef struct _FYF_CA_EmailContent_s{
	BU08	m_szEmail[161];      /* Email������ */
}FYF_CA_EmailContent_s;

/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef struct _FYF_CA_WorkTime_s
{
	BU08 byStartHour;
	BU08 byStartMinute;
	BU08 byStartSecond;
	BU08 byEndHour;
	BU08 byEndMinute;	
	BU08 byEndSecond;
}FYF_CA_WorkTime_s;
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/

typedef struct _FYF_CA_MotherCard_s
{
	BU08 m_isMotherCard;
	BU08 m_FeedCycle;
	BU32 m_LastFeedTime;
}FYF_CA_MotherCard_s;

/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/


#define FYF_CDCA_MAXNUM_IPPVP             300U  /* ���ܿ��������IPPV��Ŀ�ĸ��� */

typedef struct {
  BU32   m_dwProductID;       /* ��Ŀ��ID */
  BU08    m_byBookEdFlag;      /* ��Ʒ״̬��BOOKING��VIEWED */ 
  BU08    m_bCanTape;          /* �Ƿ����¼��1������¼��0��������¼�� */
  BU16   m_wPrice;            /* ��Ŀ�۸� */
  BU16  m_wExpiredDate;      /* ��Ŀ����ʱ�� */
  BU08    m_bySlotID;          /* Ǯ��ID */
}FYF_CDCA_IppvInfo;

typedef struct {
	BU16 wTVSID;
	FYF_CDCA_IppvInfo * pIppv;
	BU16 wNumber;
}FYF_CDCA_IppvInfo_s;

typedef struct _FYF_CDCA_Entitle_s{
	BU32 		m_dwProductID;	/*��ͨ��Ȩ�Ľ�ĿID*/
	BU16		m_tBeginDate;		/*��Ȩ����ʼʱ��*/
	BU16		m_tExpireDate;	/*��Ȩ�Ĺ���ʱ��*/
	BU08   	m_bCanTape;     /* �û��Ƿ���¼��1������¼��0��������¼�� */
	BU08   	m_byReserved[3];/* ���� */
}FYF_CDCA_Entitle_s;

typedef struct _FYF_CDCA_Entitles_s{
	BU16		m_wProductCount;
	BU08		m_m_byReserved[2];  /* ���� */
	FYF_CDCA_Entitle_s  m_Entitles[300]; /* ��Ȩ�б� */
}FYF_CDCA_Entitles_s;

typedef struct _FYF_CDCA_TVSSlotInfo_s{
	BU08	m_wSlotId;
	BU32	m_wCreditLimit; /* ���öȣ�������*/
	BU32	m_wBalance;     /* �ѻ��ĵ��� */
}FYF_CDCA_TVSSlotInfo_s;

typedef struct _FYF_CDCA_TVSSlotId_s{
	BU32	m_SlotCount;
	FYF_CDCA_TVSSlotInfo_s m_SlotInfo[20];
}FYF_CDCA_TVSSlotId_s;

typedef struct _FYF_CDCA_PairInfo_s{
	BU08 m_PairedInfo[256];
	BU08 m_PairedInfo_Len;
}FYF_CDCA_PairInfo_s;

typedef struct _FYF_CDCA_TVSACList_s{
	BU32	m_acCount;
	BU32	m_ACList[18];
}FYF_CDCA_TVSACList_s;
#endif	


