/**********************************************************************************
 File Name:   ca_dvt.h

 Description: 

 History:             
**********************************************************************************/
#ifndef _ca_dvt_h_
#define _ca_dvt_h_

#define FYF_CAERR_BASE_STB  0

#define FYF_CAERR_STB_POINTER_INVALID			(FYF_CAERR_BASE_STB + 1)		//�����ָ����Ч
#define FYF_CAERR_STB_DATA_LEN_ERROR				(FYF_CAERR_BASE_STB + 2)		//��������ݳ��Ȳ��Ϸ�
#define FYF_CAERR_STB_TVS_NOT_FOUND				(FYF_CAERR_BASE_STB + 3)		//û���ҵ�����Ҫ�����Ӫ��
#define FYF_CAERR_STB_EXCEED_MAX_COUNT			(FYF_CAERR_BASE_STB + 4)		//��������������ĸ���
#define FYF_CAERR_STB_KEY_NOT_FOUND				(FYF_CAERR_BASE_STB + 5)		//û���ҵ�������Կ
#define FYF_CAERR_STB_PIN_INVALID				(FYF_CAERR_BASE_STB + 6)		//�����PIN����Ч
#define FYF_CAERR_STB_PIN_LOCKED					(FYF_CAERR_BASE_STB + 7)		//PIN�뱻����
#define FYF_CAERR_STB_CMD_INVALID				(FYF_CAERR_BASE_STB + 8)		//MACУ�����
#define FYF_CAERR_STB_VER_EXPIRED				(FYF_CAERR_BASE_STB + 9)		//����İ汾û��IC���еĴ󣬼������Ѿ�����
#define FYF_CAERR_STB_EXCEED_MAX_LEN				(FYF_CAERR_BASE_STB + 10)	//��Ӫ�̵����Ƴ������ĳ���
#define FYF_CAERR_STB_NO_SPACE					(FYF_CAERR_BASE_STB + 11)	//�Ѿ�û�пռ�
//#define FYF_CAERR_STB_IS_PAIRED					(FYF_CAERR_BASE_STB + 12)	//���ܿ��뵱ǰ�����ж�Ӧ
#define FYF_CAERR_STB_NOT_PAIRED					(FYF_CAERR_BASE_STB + 13)	//���ܿ��뵱ǰ������û�ж�Ӧ
//#define FYF_CAERR_STB_COUNT_ISZERO				(FYF_CAERR_BASE_STB + 14)	//�����CountΪ0. 
#define FYF_CAERR_STB_WORKTIME_INVALID			(FYF_CAERR_BASE_STB + 15)	//����ʱ�����ò��Ϸ���������ʱ�䲻��00:00:00~23:59:59֮�䣬���߿�ʼʱ����ڽ���ʱ�䡣
//#define	FYF_CAERR_STB_OVERFLOW					(FYF_CAERR_BASE_STB + 16)	
#define	FYF_CAERR_STB_CARD_INVALID				(FYF_CAERR_BASE_STB + 17)	//���ܿ���Ч
#define	FYF_CAERR_STB_Rating_LOW					(FYF_CAERR_BASE_STB + 18)	//���ܿ����õ��տ�����ȵ�ǰ�տ��Ľ�Ŀ��
#define FYF_CAERR_STB_UNKNOWN					(FYF_CAERR_BASE_STB + 19)	//δ֪����STB�����κ���ʾ����ͨ������λ�ָ���

//#define	FYF_CAERR_STB_EMAILBOX_EMPTY				(FYF_CAERR_BASE_STB + 20)	//E-mail����Ϊ��
#define	FYF_CAERR_STB_NO_EMAIL					(FYF_CAERR_BASE_STB + 21)	//û�д�E-mail
#define	FYF_CAERR_STB_EMAILBOX_FULL				(FYF_CAERR_BASE_STB + 22)	//E-mail������
//#define	FYF_CAERR_STB_NO_SECTION					(FYF_CAERR_BASE_STB + 23)	//û���յ�����Section

#define FYF_CAERR_STB_MONEY_LACK					(FYF_CAERR_BASE_STB + 24)	//���ֻ࣬��Ԥ��ʱ�᷵�ظô�����Ҫ��ʾ�û�"Ԥ���ɹ��������࣬�뼰ʱ��ֵ"��
#define FYF_CAERR_STB_PROD_NOT_FOUND				(FYF_CAERR_BASE_STB + 25)	//��Ʒδ�ҵ�
#define FYF_CAERR_STB_OPER_INVALID				(FYF_CAERR_BASE_STB + 26)	//��Ʒ�Ѵ��ڣ����ܲ���
#define FYF_CAERR_STB_NEED_PIN_PASS				(FYF_CAERR_BASE_STB + 27)	//��ҪPIN��֤ͨ��
#define FYF_CAERR_STB_IC_COMMUNICATE				(FYF_CAERR_BASE_STB + 28)	//��IC��ͨѶ����
#define FYF_CAERR_STB_SLOT_NOT_FOUND				(FYF_CAERR_BASE_STB + 29)	//��Ӫ��Ǯ��û�з���
#define FYF_CAERR_STB_PRICE_INVALID				(FYF_CAERR_BASE_STB + 30)	//�۸���Ч
#define FYF_CAERR_STB_PRODUCT_EXPIRED			(FYF_CAERR_BASE_STB + 31)	//��Ʒ�Ѿ�����

//xb:20050617 ++
#define FYF_CAERR_STB_EXPIRED					(FYF_CAERR_BASE_STB + 33)	//��Ʒ�Ѿ�����
#define FYF_CAERR_IPPREC_NOT_FOUND				(FYF_CAERR_BASE_STB + 34)	//IPP��¼������
#define FYF_CAERR_TIME_INVALID					(FYF_CAERR_BASE_STB + 35)	//��ǰʱ����Ч
#define FYF_CAERR_NOT_BOOKED						(FYF_CAERR_BASE_STB + 37)	//��ƷδԤ��
#define FYF_CAERR_CONFIRMED						(FYF_CAERR_BASE_STB + 38)	//��Ʒ�Ѿ�ȷ��
#define FYF_CAERR_UNCONFIRMED					(FYF_CAERR_BASE_STB + 39)	//��Ʒ�Ѿ�ȡ��ȷ��
#define FYF_CAERR_INPUT_DATA_INVALID				(FYF_CAERR_BASE_STB + 40)	//�����������Ч

//xb:20050617 --

//������ң��������ȡ������ʾ��Ϣ�б�
#define	FYF_CA_RATING_TOO_LOW					0			//�տ����𲻹�
#define FYF_CA_NOT_IN_WATCH_TIME					1			//�����տ�ʱ����
#define	FYF_CA_NOT_PAIRED						2			//û�л�����Ӧ
#define	FYF_CA_IS_PAIRED_OTHER					3			//IC�������������ж�Ӧ
#define	FYF_CA_PLEASE_INSERT_CARD				4			//��忨
#define	FYF_CA_NO_ENTITLE						5			//û�й���˽�Ŀ
#define	FYF_CA_PRODUCT_RESTRICT					6			//��Ӫ�����ƹۿ��ý�Ŀ
#define	FYF_CA_AREA_RESTRICT						7			//��Ӫ����������ۿ�
#define	FYF_CA_DECRYPT_FAILURE					0xFF		//STB�����κ���ʾ����ͨѶ���⣬ͨ������λ�ɽ��
//V2.1��������ʾ��Ϣ
#define FYF_CA_MOTHER_RESTRICT					8			//�˿�Ϊ�ӿ����Ѿ��������տ�������ĸ�����
#define FYF_CA_NO_MONEY							9			//���㣬���ܹۿ��˽�Ŀ���뼰ʱ��ֵ
#define FYF_CA_IPPV_NO_CONFIRM					10			//�˽�ĿΪIPPV��Ŀ���뵽IPPV��Ŀȷ��/ȡ������˵���ȷ�Ϲ���˽�Ŀ
#define FYF_CA_IPPV_NO_BOOK						11			//�˽�ĿΪIPPV��Ŀ����û��Ԥ����ȷ�Ϲ��򣬲��ܹۿ��˽�Ŀ
#define FYF_CA_IPPT_NO_CONFIRM					12			//�˽�ĿΪIPPT��Ŀ���뵽IPPT��Ŀȷ��/ȡ������˵���ȷ�Ϲ���˽�Ŀ
#define FYF_CA_IPPT_NO_BOOK						13			//�˽�ĿΪIPPT��Ŀ����û��Ԥ����ȷ�Ϲ��򣬲��ܹۿ��˽�Ŀ
#define FYF_CA_IPPV_NO_MONEY						14			//�˽�ĿΪIPPV��Ŀ�����㣬���ܹۿ����뼰ʱ��ֵ
#define FYF_CA_IPPT_NO_MONEY						15			//�˽�ĿΪIPPT��Ŀ�����㣬���ܹۿ����뼰ʱ��ֵ
//xb:20050617
#define FYF_CA_DATA_INVALID						16			//������Ч��STB�����κ���ʾ������Կ���⡣
#define FYF_CA_SLOT_NOT_FOUND					17			//Ǯ��������
#define FYF_CA_SC_NOT_SERVER						18			//IC������ֹ����
#define FYF_CA_TVS_NOT_FOUND						19			//��Ӫ�̲�����
#define FYF_CA_KEY_NOT_FOUND						20			//�˿�δ���������ϵ��Ӫ��
#define FYF_CA_IPPNEED_CALLBACK					21			//����ϵ��Ӫ�̻ش�IPP��Ŀ��Ϣ

//wy:20071022���û��Զ�����������
#define FYF_CA_BOUQUETID							254			//�Զ�����������-Bouquet_id

//kfd:2007-11-16---�ַ�����������
#define FYF_CA_LANG_CHN_SIM						1			//��������(Ĭ��)
#define FYF_CA_LANG_ENG							2			//Ӣ��

typedef struct _FYF_CA_Ipp_s{
	BU16	m_wTVSID;											//��Ӫ�̱��
	BU16	m_wProdID;											//��ƷID
	BU08	m_bySlotID;											//Ǯ��ID
	BU08	m_szProdName[21];			//��Ʒ����
	BU32	m_tStartTime;										//��ʼʱ�䣬time_t��ʽ��
	BU32	m_dwDuration;										//��������
	BU08	m_szServiceName[21];			//ҵ������
	BU16	m_wCurTppTapPrice;									//��ǰ�Ĳ��ش�����¼��۸�(��)���۸�����ֵΪ0
	BU16	m_wCurTppNoTapPrice;								//��ǰ�Ĳ��ش�������¼��۸�(��)���۸�����ֵΪ1
	BU16	m_wCurCppTapPrice;									//��ǰ��Ҫ�ش�����¼��۸�(��)���۸�����ֵΪ2
	BU16	m_wCurCppNoTapPrice;								//��ǰ��Ҫ�ش�������¼��۸�(��)���۸�����ֵΪ3
	BU16	m_wBookedPrice;										//�Ѿ�Ԥ���ļ۸�(��)
	BU08	m_byBookedPriceType;								//�Ѿ�Ԥ���ļ۸����ͣ�ȡֵ��Χ0~3
	BU08	m_byBookedInterval;									//Ԥ���շѼ������λ����
	BU08	m_byCurInterval;									//��ǰ�շѼ������λ����
	BU08	m_byIppStatus;										//Ipp��Ʒ״̬
	BU08	m_byUnit;											//�շѼ���ĵ�λ0 -����1-Сʱ2-��3-��4-��
	BU16	m_wIpptPeriod; 										//�û�����IPPT�Ĺۿ�������,for Philippines LongIPPT��
}FYF_CA_Ipp_s;

typedef struct _FYF_CA_ViewedIpp{
	BU16	m_wTVSID;											//��Ӫ�̱��
	char	m_szProdName[21];			//��Ʒ����
	BU32	m_tStartTime;										//��ʼʱ�䣬time_t��ʽ��
	BU32	m_dwDuration;										//��������
	BU16	m_wBookedPrice;										//Ԥ���۸�(��)
	BU08	m_byBookedPriceType;								//Ԥ���۸����ͣ�0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
	BU08	m_byBookedInterval;									//Ԥ���շѼ������λ���ӣ�IPPVʱ��ֵΪ0
	char    m_szOtherInfo[44];									//ippvʱΪ���˲�ƷΪippv��Ʒ����ipptʱΪ���ۿ���ʱ�䣺�����ӣ���Ǯ���������֡�
	BU08	m_byUnit;										//�շѵ�λ��0 -����1-Сʱ2-��3-��4-��
}FYF_CA_ViewedIpp_s;

typedef struct _FYF_CA_ManuInfo{
	BU32	m_dwCardID;											//���ڲ�ID
	BU08	m_byszSerialNO[64];				//�ⲿ����
	BU32	m_dwSTBCASVer;										//������CASģ��İ汾��
	char	m_szSCCASManuName[21];	//CAS��Ӧ�̵�����
	BU32	m_dwSCCASVer;										//���ܿ�CAS�İ汾��
	BU32	m_dwSCCOSVer;										//SC�İ汾�ţ�Ҳ����SC��COS�İ汾��
	BU32	m_tSCExpireDate;									//���ܿ�����Ч�ڣ���STB��ʼ��ʱ�жϣ�������ڣ����û�һ����ʾ�������Լ���ʹ�á�time_tʱ���ʽ��
}FYF_CA_ManuInfo;

typedef struct _FYF_CA_AreaInfo{
	BU32		m_dwCardArea;			//����������
	BU32		m_tSetCardAreaTime;		//�趨����ʱ�䡣
	BU08		m_byStartFlag;			//������־��1:������0:û�С�
	BU32		m_tSetFlagTime;			//���ñ�־ʱ�䡣
	BU32		m_tSetStreamTime;		//��������������ʱ�䡣
	BU32		m_dwIntervalTime;		//���е�ǰʱ���ȥ������ʱ������ֵ��
	BU32		m_dwStreamMinArea;		//��������С������
	BU32		m_dwStreamMaxArea;		//���������������
}FYF_CA_AreaInfo;

typedef struct _FYF_DATE_s
{
    BU32   m_mjd;//���ڱ���
	BU16   m_year;
	BU08   m_month;
	BU08   m_day;
	BU08   m_weekday;
	BU08   m_hour;
	BU08   m_min;
	BU08   m_sec;	
}FYF_Date_s;

typedef struct _FYF_CA_WorkTime_s
{
	BU08 byStartHour;
	BU08 byStartMinute;
	BU08 byEndHour;
	BU08 byEndMinute;	
}FYF_CA_WorkTime_s;

typedef struct _FYF_CA_EmailHead_s
{
	BU32  m_dwActionID;     /* Email ID */
	BU08 m_szEmailHead[21]; /*����*/
	BU08 m_szSender[21];	/*������*/
	BU08 m_bNewEmail;		/*�Ƿ�Ϊ���ʼ�,	1:�� 0:�Ѷ�*/
	BU32 m_tCreateTime;	/*����ʱ��*/
}FYF_CA_EmailHead_s;

typedef struct _FYF_CA_EmailContent_s{
	BU08	m_szEmail[1025];      /* Email������ */
	BU32	m_wEmailLength;		//�ʼ����ݳ���
}FYF_CA_EmailContent_s;

typedef struct _FYF_CA_Money_s
{
	char	m_szTVSName[21];		
	BU32 m_paid;			//�ѻ�Ǯ��
	BU32 m_balance;			//���
}FYF_CA_Money_s;

typedef struct _FYF_CA_TvsInfo{
	BU16	m_wTVSID; 											//��Ӫ�̵��ⲿ���
	char	m_szTVSName[21];									//��Ӫ������
}FYF_CA_TvsInfo;

typedef struct _FYF_CA_ServiceEntitle{
	BU16 	m_dwProductID;										//��ƷID
	BU32	m_tExpireDate;										//��Ȩʱ�䣬time_t��ʽ��
	BU32	m_tBeginDate;										//��ʼʱ�䣬time_t��ʽ��
	BU32	m_tEndTime;											//����ʱ�䣬time_t��ʽ��
	char	m_szProductName[21];			//��Ʒ����
	BU08	m_bCanTape;										//¼���ʶ��
}FYF_CA_ServiceEntitle_s;											//��ͨ��Ʒ��Ȩ

typedef struct _FYF_CA_Entitle_s
{
	BU08 pEntitleCount;				//��Ʒ����
	FYF_CA_ServiceEntitle_s m_Entitles[250]; /* ��Ȩ�б� */
}FYF_CA_Entitle_s;

typedef enum
{
	FYF_CA_CARD_STATE_IN,		/* ������ */
	FYF_CA_CARD_STATE_OUT,		/* ���γ� */
	FYF_CA_CARD_STATE_NORMAL	/* ������ */
}FYF_CA_CARD_STATE_e;

typedef enum
{
	FYF_STB_DATA_LEN_ERROR = 1,
	FYF_STB_PIN_INVALID,
	FYF_STB_PIN_LOCKED
}FYF_CA_PIN_e;

typedef struct _FYF_DVTCA_PairInfo_s{
	BU08 m_PairedInfo[256];
	BU08 m_PairedInfo_Len;
}FYF_DVTCA_PairInfo_s;

typedef struct{
	BU16 ecmPid;
	FYF_CA_Ipp_s ippvProgram;
}FYF_CA_RealTimeIpp_s;

typedef struct _caInfo_time_s
{
    BU08 m_days[7];
	BU08 m_hrs[3];
	BU08 m_mins[3];
	BU08 m_secs[3];
}caInfo_time_s;

typedef struct _caInfo_currency_s
{
    BU08 m_yuan[8];
	BU08 m_fen[2];
	BU08 m_jiao[2];
}caInfo_currency_s;

#endif	

