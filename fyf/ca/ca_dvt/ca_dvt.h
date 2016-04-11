/**********************************************************************************
 File Name:   ca_dvt.h

 Description: 

 History:             
**********************************************************************************/
#ifndef _ca_dvt_h_
#define _ca_dvt_h_

#define FYF_CAERR_BASE_STB  0

#define FYF_CAERR_STB_POINTER_INVALID			(FYF_CAERR_BASE_STB + 1)		//输入的指针无效
#define FYF_CAERR_STB_DATA_LEN_ERROR				(FYF_CAERR_BASE_STB + 2)		//输入的数据长度不合法
#define FYF_CAERR_STB_TVS_NOT_FOUND				(FYF_CAERR_BASE_STB + 3)		//没有找到符合要求的运营商
#define FYF_CAERR_STB_EXCEED_MAX_COUNT			(FYF_CAERR_BASE_STB + 4)		//超过了允许的最大的个数
#define FYF_CAERR_STB_KEY_NOT_FOUND				(FYF_CAERR_BASE_STB + 5)		//没有找到解密密钥
#define FYF_CAERR_STB_PIN_INVALID				(FYF_CAERR_BASE_STB + 6)		//输入的PIN码无效
#define FYF_CAERR_STB_PIN_LOCKED					(FYF_CAERR_BASE_STB + 7)		//PIN码被锁定
#define FYF_CAERR_STB_CMD_INVALID				(FYF_CAERR_BASE_STB + 8)		//MAC校验错误
#define FYF_CAERR_STB_VER_EXPIRED				(FYF_CAERR_BASE_STB + 9)		//输入的版本没有IC卡中的大，即数据已经过期
#define FYF_CAERR_STB_EXCEED_MAX_LEN				(FYF_CAERR_BASE_STB + 10)	//运营商的名称超过最大的长度
#define FYF_CAERR_STB_NO_SPACE					(FYF_CAERR_BASE_STB + 11)	//已经没有空间
//#define FYF_CAERR_STB_IS_PAIRED					(FYF_CAERR_BASE_STB + 12)	//智能卡与当前机顶盒对应
#define FYF_CAERR_STB_NOT_PAIRED					(FYF_CAERR_BASE_STB + 13)	//智能卡与当前机顶盒没有对应
//#define FYF_CAERR_STB_COUNT_ISZERO				(FYF_CAERR_BASE_STB + 14)	//输入的Count为0. 
#define FYF_CAERR_STB_WORKTIME_INVALID			(FYF_CAERR_BASE_STB + 15)	//工作时段设置不合法，可能是时间不在00:00:00~23:59:59之间，或者开始时间大于结束时间。
//#define	FYF_CAERR_STB_OVERFLOW					(FYF_CAERR_BASE_STB + 16)	
#define	FYF_CAERR_STB_CARD_INVALID				(FYF_CAERR_BASE_STB + 17)	//智能卡无效
#define	FYF_CAERR_STB_Rating_LOW					(FYF_CAERR_BASE_STB + 18)	//智能卡设置的收看级别比当前收看的节目低
#define FYF_CAERR_STB_UNKNOWN					(FYF_CAERR_BASE_STB + 19)	//未知错误，STB不做任何提示，可通过卡复位恢复。

//#define	FYF_CAERR_STB_EMAILBOX_EMPTY				(FYF_CAERR_BASE_STB + 20)	//E-mail信箱为空
#define	FYF_CAERR_STB_NO_EMAIL					(FYF_CAERR_BASE_STB + 21)	//没有此E-mail
#define	FYF_CAERR_STB_EMAILBOX_FULL				(FYF_CAERR_BASE_STB + 22)	//E-mail信箱满
//#define	FYF_CAERR_STB_NO_SECTION					(FYF_CAERR_BASE_STB + 23)	//没有收到完整Section

#define FYF_CAERR_STB_MONEY_LACK					(FYF_CAERR_BASE_STB + 24)	//余额不多，只在预订时会返回该错误，需要提示用户"预订成功，但余额不多，请及时充值"。
#define FYF_CAERR_STB_PROD_NOT_FOUND				(FYF_CAERR_BASE_STB + 25)	//产品未找到
#define FYF_CAERR_STB_OPER_INVALID				(FYF_CAERR_BASE_STB + 26)	//产品已存在，不能操作
#define FYF_CAERR_STB_NEED_PIN_PASS				(FYF_CAERR_BASE_STB + 27)	//需要PIN验证通过
#define FYF_CAERR_STB_IC_COMMUNICATE				(FYF_CAERR_BASE_STB + 28)	//与IC卡通讯错误
#define FYF_CAERR_STB_SLOT_NOT_FOUND				(FYF_CAERR_BASE_STB + 29)	//运营商钱包没有发现
#define FYF_CAERR_STB_PRICE_INVALID				(FYF_CAERR_BASE_STB + 30)	//价格无效
#define FYF_CAERR_STB_PRODUCT_EXPIRED			(FYF_CAERR_BASE_STB + 31)	//产品已经过期

//xb:20050617 ++
#define FYF_CAERR_STB_EXPIRED					(FYF_CAERR_BASE_STB + 33)	//产品已经过期
#define FYF_CAERR_IPPREC_NOT_FOUND				(FYF_CAERR_BASE_STB + 34)	//IPP记录不存在
#define FYF_CAERR_TIME_INVALID					(FYF_CAERR_BASE_STB + 35)	//当前时间无效
#define FYF_CAERR_NOT_BOOKED						(FYF_CAERR_BASE_STB + 37)	//产品未预定
#define FYF_CAERR_CONFIRMED						(FYF_CAERR_BASE_STB + 38)	//产品已经确认
#define FYF_CAERR_UNCONFIRMED					(FYF_CAERR_BASE_STB + 39)	//产品已经取消确认
#define FYF_CAERR_INPUT_DATA_INVALID				(FYF_CAERR_BASE_STB + 40)	//输入的数据无效

//xb:20050617 --

//机顶盒遥控器不能取消的显示信息列表
#define	FYF_CA_RATING_TOO_LOW					0			//收看级别不够
#define FYF_CA_NOT_IN_WATCH_TIME					1			//不在收看时段内
#define	FYF_CA_NOT_PAIRED						2			//没有机卡对应
#define	FYF_CA_IS_PAIRED_OTHER					3			//IC卡与其它机顶盒对应
#define	FYF_CA_PLEASE_INSERT_CARD				4			//请插卡
#define	FYF_CA_NO_ENTITLE						5			//没有购买此节目
#define	FYF_CA_PRODUCT_RESTRICT					6			//运营商限制观看该节目
#define	FYF_CA_AREA_RESTRICT						7			//运营商限制区域观看
#define	FYF_CA_DECRYPT_FAILURE					0xFF		//STB不做任何提示。卡通讯问题，通过卡复位可解决
//V2.1新增的提示信息
#define FYF_CA_MOTHER_RESTRICT					8			//此卡为子卡，已经被限制收看，请与母卡配对
#define FYF_CA_NO_MONEY							9			//余额不足，不能观看此节目，请及时充值
#define FYF_CA_IPPV_NO_CONFIRM					10			//此节目为IPPV节目，请到IPPV节目确认/取消购买菜单下确认购买此节目
#define FYF_CA_IPPV_NO_BOOK						11			//此节目为IPPV节目，您没有预订和确认购买，不能观看此节目
#define FYF_CA_IPPT_NO_CONFIRM					12			//此节目为IPPT节目，请到IPPT节目确认/取消购买菜单下确认购买此节目
#define FYF_CA_IPPT_NO_BOOK						13			//此节目为IPPT节目，您没有预订和确认购买，不能观看此节目
#define FYF_CA_IPPV_NO_MONEY						14			//此节目为IPPV节目，余额不足，不能观看，请及时充值
#define FYF_CA_IPPT_NO_MONEY						15			//此节目为IPPT节目，余额不足，不能观看，请及时充值
//xb:20050617
#define FYF_CA_DATA_INVALID						16			//数据无效，STB不做任何提示。卡密钥问题。
#define FYF_CA_SLOT_NOT_FOUND					17			//钱包不存在
#define FYF_CA_SC_NOT_SERVER						18			//IC卡被禁止服务
#define FYF_CA_TVS_NOT_FOUND						19			//运营商不存在
#define FYF_CA_KEY_NOT_FOUND						20			//此卡未被激活，请联系运营商
#define FYF_CA_IPPNEED_CALLBACK					21			//请联系运营商回传IPP节目信息

//wy:20071022－用户自定义数据类型
#define FYF_CA_BOUQUETID							254			//自定义数据类型-Bouquet_id

//kfd:2007-11-16---字符串语言类型
#define FYF_CA_LANG_CHN_SIM						1			//简体中文(默认)
#define FYF_CA_LANG_ENG							2			//英文

typedef struct _FYF_CA_Ipp_s{
	BU16	m_wTVSID;											//运营商编号
	BU16	m_wProdID;											//产品ID
	BU08	m_bySlotID;											//钱包ID
	BU08	m_szProdName[21];			//产品名称
	BU32	m_tStartTime;										//开始时间，time_t格式。
	BU32	m_dwDuration;										//持续秒数
	BU08	m_szServiceName[21];			//业务名称
	BU16	m_wCurTppTapPrice;									//当前的不回传、能录像价格(分)，价格类型值为0
	BU16	m_wCurTppNoTapPrice;								//当前的不回传、不可录像价格(分)，价格类型值为1
	BU16	m_wCurCppTapPrice;									//当前的要回传、能录像价格(分)，价格类型值为2
	BU16	m_wCurCppNoTapPrice;								//当前的要回传、不可录像价格(分)，价格类型值为3
	BU16	m_wBookedPrice;										//已经预订的价格(分)
	BU08	m_byBookedPriceType;								//已经预订的价格类型，取值范围0~3
	BU08	m_byBookedInterval;									//预订收费间隔，单位分钟
	BU08	m_byCurInterval;									//当前收费间隔，单位分钟
	BU08	m_byIppStatus;										//Ipp产品状态
	BU08	m_byUnit;											//收费间隔的单位0 -分钟1-小时2-天3-月4-年
	BU16	m_wIpptPeriod; 										//用户订购IPPT的观看周期数,for Philippines LongIPPT。
}FYF_CA_Ipp_s;

typedef struct _FYF_CA_ViewedIpp{
	BU16	m_wTVSID;											//运营商编号
	char	m_szProdName[21];			//产品名称
	BU32	m_tStartTime;										//开始时间，time_t格式。
	BU32	m_dwDuration;										//持续秒数
	BU16	m_wBookedPrice;										//预订价格(分)
	BU08	m_byBookedPriceType;								//预订价格类型：0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
	BU08	m_byBookedInterval;									//预订收费间隔，单位分钟，IPPV时此值为0
	char    m_szOtherInfo[44];									//ippv时为“此产品为ippv产品”，ippt时为“观看总时间：？分钟，扣钱总数：？分”
	BU08	m_byUnit;										//收费单位，0 -分钟1-小时2-天3-月4-年
}FYF_CA_ViewedIpp_s;

typedef struct _FYF_CA_ManuInfo{
	BU32	m_dwCardID;											//卡内部ID
	BU08	m_byszSerialNO[64];				//外部卡号
	BU32	m_dwSTBCASVer;										//机顶盒CAS模块的版本号
	char	m_szSCCASManuName[21];	//CAS供应商的名称
	BU32	m_dwSCCASVer;										//智能卡CAS的版本号
	BU32	m_dwSCCOSVer;										//SC的版本号，也就是SC中COS的版本号
	BU32	m_tSCExpireDate;									//智能卡的有效期，由STB初始化时判断，如果过期，给用户一个提示，但可以继续使用。time_t时间格式。
}FYF_CA_ManuInfo;

typedef struct _FYF_CA_AreaInfo{
	BU32		m_dwCardArea;			//卡的区域码
	BU32		m_tSetCardAreaTime;		//设定区域时间。
	BU08		m_byStartFlag;			//启动标志。1:启动；0:没有。
	BU32		m_tSetFlagTime;			//设置标志时间。
	BU32		m_tSetStreamTime;		//设置特征码流的时间。
	BU32		m_dwIntervalTime;		//卡中当前时间减去特征码时间的最大值。
	BU32		m_dwStreamMinArea;		//码流的最小区域码
	BU32		m_dwStreamMaxArea;		//码流的最大区域码
}FYF_CA_AreaInfo;

typedef struct _FYF_DATE_s
{
    BU32   m_mjd;//日期编码
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
	BU08 m_szEmailHead[21]; /*标题*/
	BU08 m_szSender[21];	/*发送者*/
	BU08 m_bNewEmail;		/*是否为新邮件,	1:是 0:已读*/
	BU32 m_tCreateTime;	/*发送时间*/
}FYF_CA_EmailHead_s;

typedef struct _FYF_CA_EmailContent_s{
	BU08	m_szEmail[1025];      /* Email的正文 */
	BU32	m_wEmailLength;		//邮件内容长度
}FYF_CA_EmailContent_s;

typedef struct _FYF_CA_Money_s
{
	char	m_szTVSName[21];		
	BU32 m_paid;			//已花钱数
	BU32 m_balance;			//余额
}FYF_CA_Money_s;

typedef struct _FYF_CA_TvsInfo{
	BU16	m_wTVSID; 											//运营商的外部编号
	char	m_szTVSName[21];									//运营商名称
}FYF_CA_TvsInfo;

typedef struct _FYF_CA_ServiceEntitle{
	BU16 	m_dwProductID;										//产品ID
	BU32	m_tExpireDate;										//授权时间，time_t格式。
	BU32	m_tBeginDate;										//开始时间，time_t格式。
	BU32	m_tEndTime;											//结束时间，time_t格式。
	char	m_szProductName[21];			//产品名称
	BU08	m_bCanTape;										//录像标识。
}FYF_CA_ServiceEntitle_s;											//普通产品授权

typedef struct _FYF_CA_Entitle_s
{
	BU08 pEntitleCount;				//产品总数
	FYF_CA_ServiceEntitle_s m_Entitles[250]; /* 授权列表 */
}FYF_CA_Entitle_s;

typedef enum
{
	FYF_CA_CARD_STATE_IN,		/* 卡插入 */
	FYF_CA_CARD_STATE_OUT,		/* 卡拔出 */
	FYF_CA_CARD_STATE_NORMAL	/* 卡正常 */
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

