/**********************************************************************************
 File Name:   cdca.h

 Description: 

 History:             
**********************************************************************************/
#ifndef _cdca_h_
#define _cdca_h_

//---------- 2. 机顶盒相关限制
#define FYF_CDCA_MAXNUM_PROGRAMBYCW       4U    /* 一个控制字最多解的节目数 */
#define FYF_CDCA_MAXNUM_ECM               8U    /* 同时接收处理的ECMPID的最大数量 */

#define FYF_CDCA_MAXNUM_DETITLE            5U    /* 每个运营商下可保存的反授权码个数 */

//---------- 4. 功能调用返回值定义
#define FYF_CDCA_RC_OK                    0x00  /* 成功 */
#define FYF_CDCA_RC_UNKNOWN               0x01  /* 未知错误 */
#define FYF_CDCA_RC_POINTER_INVALID       0x02  /* 指针无效 */
#define FYF_CDCA_RC_CARD_INVALID          0x03  /* 智能卡无效 */
#define FYF_CDCA_RC_PIN_INVALID           0x04  /* PIN码无效 */
#define FYF_CDCA_RC_DATASPACE_SMALL       0x06  /* 所给的空间不足 */
#define FYF_CDCA_RC_CARD_PAIROTHER        0x07  /* 智能卡已经对应别的机顶盒 */
#define FYF_CDCA_RC_DATA_NOT_FIND         0x08  /* 没有找到所要的数据 */
#define FYF_CDCA_RC_PROG_STATUS_INVALID   0x09  /* 要购买的节目状态无效 */
#define FYF_CDCA_RC_CARD_NO_ROOM          0x0A  /* 智能卡没有空间存放购买的节目 */
#define FYF_CDCA_RC_WORKTIME_INVALID      0x0B  /* 设定的工作时段无效 */
#define FYF_CDCA_RC_IPPV_CANNTDEL         0x0C  /* IPPV节目不能被删除 */
#define FYF_CDCA_RC_CARD_NOPAIR           0x0D  /* 智能卡没有对应任何的机顶盒 */
#define FYF_CDCA_RC_WATCHRATING_INVALID   0x0E  /* 设定的观看级别无效 */
#define FYF_CDCA_RC_CARD_NOTSUPPORT       0x0F  /* 当前智能卡不支持此功能 */
#define FYF_CDCA_RC_DATA_ERROR            0x10  /* 数据错误，智能卡拒绝 */
#define FYF_CDCA_RC_FEEDTIME_NOT_ARRIVE   0x11  /* 喂养时间未到，子卡不能被喂养 */
#define FYF_CDCA_RC_CARD_TYPEERROR        0x12  /* 子母卡喂养失败，插入智能卡类型错误 */

//---------- 10. 反授权确认码应用相关定义
#define FYF_CDCA_Detitle_All_Readed       0x00  /* 所有反授权确认码已经被读，隐藏图标 */
#define FYF_CDCA_Detitle_Received         0x01  /* 收到新的反授权码，显示反授权码图标 */
#define FYF_CDCA_Detitle_Space_Small      0x02  /* 反授权码空间不足，改变图标状态提示用户 */
#define FYF_CDCA_Detitle_Ignore           0x03  /* 收到重复的反授权码，可忽略，不做处理 */
//---------- 10. 进度条提示信息
#define FYF_CDCA_SCALE_RECEIVEPATCH       1     /* 升级数据接收中 */
#define FYF_CDCA_SCALE_PATCHING           2     /* 智能卡升级中 */

//---------- 7. OSD应用相关定义
//------------ 7.1. OSD的长度限制
#define FYF_CDCA_MAXLEN_OSD               180U  /* OSD内容的最大长度 */

//------------ 7.2. OSD显示类型
#define FYF_CDCA_OSD_TOP                  0x01  /* OSD风格：显示在屏幕上方 */
#define FYF_CDCA_OSD_BOTTOM               0x02  /* OSD风格：显示在屏幕下方 */
#define FYF_CDCA_OSD_FULLSCREAN           0x03  /* OSD风格：整屏显示 */
#define FYF_CDCA_OSD_HALFSCREAN           0x04  /* OSD风格：半屏显示 */
//---------- 3. 运营商信息
typedef struct {
	char  m_szTVSPriInfo[32+1];  /* 运营商私有信息 */
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
	BU08	m_bNewEmail;       /* 新邮件标记：0－不是新邮件；1－新邮件 */
	BU08	m_szEmailHead[31]; /* 邮件标题，最长为30 */        
	BU32   m_tCreateTime;     /* EMAIL创建的时间 */
	BU16   m_wImportance;     /* 重要性： 0－普通，1－重要 */
}FYF_CA_EmailHead_s;

typedef struct _FYF_CA_EmailIdx_s{
	BU08	m_Count;       /*实际获得的邮件个数*/
	BU08	m_byFromIndex; /*byFromIndex并不是从逻辑上的第byFromIndex封信开始取，而是从邮箱物理空间的第byFromIndex封邮件位置开始取*/        
}FYF_CA_EmailIdx_s;

typedef struct _FYF_CA_EmailContent_s{
	BU08	m_szEmail[161];      /* Email的正文 */
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


#define FYF_CDCA_MAXNUM_IPPVP             300U  /* 智能卡保存最多IPPV节目的个数 */

typedef struct {
  BU32   m_dwProductID;       /* 节目的ID */
  BU08    m_byBookEdFlag;      /* 产品状态：BOOKING，VIEWED */ 
  BU08    m_bCanTape;          /* 是否可以录像：1－可以录像；0－不可以录像 */
  BU16   m_wPrice;            /* 节目价格 */
  BU16  m_wExpiredDate;      /* 节目过期时间 */
  BU08    m_bySlotID;          /* 钱包ID */
}FYF_CDCA_IppvInfo;

typedef struct {
	BU16 wTVSID;
	FYF_CDCA_IppvInfo * pIppv;
	BU16 wNumber;
}FYF_CDCA_IppvInfo_s;

typedef struct _FYF_CDCA_Entitle_s{
	BU32 		m_dwProductID;	/*普通授权的节目ID*/
	BU16		m_tBeginDate;		/*授权的起始时间*/
	BU16		m_tExpireDate;	/*授权的过期时间*/
	BU08   	m_bCanTape;     /* 用户是否购买录像：1－可以录像；0－不可以录像 */
	BU08   	m_byReserved[3];/* 保留 */
}FYF_CDCA_Entitle_s;

typedef struct _FYF_CDCA_Entitles_s{
	BU16		m_wProductCount;
	BU08		m_m_byReserved[2];  /* 保留 */
	FYF_CDCA_Entitle_s  m_Entitles[300]; /* 授权列表 */
}FYF_CDCA_Entitles_s;

typedef struct _FYF_CDCA_TVSSlotInfo_s{
	BU08	m_wSlotId;
	BU32	m_wCreditLimit; /* 信用度（点数）*/
	BU32	m_wBalance;     /* 已花的点数 */
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


