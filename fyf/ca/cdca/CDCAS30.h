#ifndef CDCAS_CALIB_APIEX_H
#define CDCAS_CALIB_APIEX_H

#ifdef  __cplusplus
extern "C" {
#endif


/*-------------------------------------基本数据类型---------------------------------------*/
typedef unsigned  long  CDCA_U32;
typedef unsigned  short CDCA_U16;
typedef unsigned  char  CDCA_U8;

typedef CDCA_U8 CDCA_BOOL;

#define CDCA_TRUE    ((CDCA_BOOL)1)
#define CDCA_FALSE   ((CDCA_BOOL)0)

/*----------------------------------------宏定义-----------------------------------------*/

//---------- 1. 智能卡相关限制
#define CDCA_MAXLEN_SN                16U   /* 智能卡序列号的长度 */
#define CDCA_MAXLEN_PINCODE           6U    /* PIN码的长度 */
#define CDCA_MAXLEN_TVSPRIINFO        32U   /* 运营商私有信息的长度 */
#define CDCA_MAXNUM_OPERATOR          4U    /* 最多的运营商个数 */
#define CDCA_MAXNUM_ACLIST            18U   /* 智能卡内保存的每个运营商的用户特征个数 */
#define CDCA_MAXNUM_SLOT              20U   /* 卡存储的最大钱包数 */

#define CDCA_MAXNUM_IPPVP             300U  /* 智能卡保存最多IPPV节目的个数 */
#define CDCA_MAXNUM_PRICE             2U    /* 最多的IPPV价格个数 */

#define CDCA_MAXNUM_ENTITLE           300U  /* 智能卡保存最多授权产品的个数 */

//---------- 2. 机顶盒相关限制
#define CDCA_MAXNUM_PROGRAMBYCW       4U    /* 一个控制字最多解的节目数 */
#define CDCA_MAXNUM_ECM               8U    /* 同时接收处理的ECMPID的最大数量 */

#define CDCA_MAXNUM_DETITLE            5U    /* 每个运营商下可保存的反授权码个数 */

//---------- 3. CAS提示信息
#define CDCA_MESSAGE_CANCEL_TYPE      0x00  /* 取消当前的显示 */
#define CDCA_MESSAGE_BADCARD_TYPE     0x01  /* 无法识别卡 */
#define CDCA_MESSAGE_EXPICARD_TYPE    0x02  /* 智能卡过期，请更换新卡 */
#define CDCA_MESSAGE_INSERTCARD_TYPE  0x03  /* 加扰节目，请插入智能卡 */
#define CDCA_MESSAGE_NOOPER_TYPE      0x04  /* 卡中不存在节目运营商 */
#define CDCA_MESSAGE_BLACKOUT_TYPE    0x05  /* 条件禁播 */
#define CDCA_MESSAGE_OUTWORKTIME_TYPE 0x06  /* 当前时段被设定为不能观看 */
#define CDCA_MESSAGE_WATCHLEVEL_TYPE  0x07  /* 节目级别高于设定的观看级别 */
#define CDCA_MESSAGE_PAIRING_TYPE     0x08  /* 智能卡与本机顶盒不对应 */
#define CDCA_MESSAGE_NOENTITLE_TYPE   0x09  /* 没有授权 */
#define CDCA_MESSAGE_DECRYPTFAIL_TYPE 0x0A  /* 节目解密失败 */
#define CDCA_MESSAGE_NOMONEY_TYPE     0x0B  /* 卡内金额不足 */
#define CDCA_MESSAGE_ERRREGION_TYPE   0x0C  /* 区域不正确 */
#define CDCA_MESSAGE_NEEDFEED_TYPE    0x0D  /* 子卡需要和母卡对应，请插入母卡 */
#define CDCA_MESSAGE_ERRCARD_TYPE     0x0E  /* 智能卡校验失败，请联系运营商 */
#define CDCA_MESSAGE_UPDATE_TYPE      0x0F  /* 智能卡升级中，请不要拔卡或者关机 */
#define CDCA_MESSAGE_LOWCARDVER_TYPE  0x10  /* 请升级智能卡 */
#define CDCA_MESSAGE_VIEWLOCK_TYPE    0x11  /* 请勿频繁切换频道 */
#define CDCA_MESSAGE_MAXRESTART_TYPE  0x12  /* 智能卡暂时休眠，请5分钟后重新开机 */
#define CDCA_MESSAGE_FREEZE_TYPE      0x13  /* 智能卡已冻结，请联系运营商 */
#define CDCA_MESSAGE_CALLBACK_TYPE    0x14  /* 智能卡已暂停，请回传收视记录给运营商 */
#define CDCA_MESSAGE_STBLOCKED_TYPE   0x20  /* 请重启机顶盒 */

//---------- 4. 功能调用返回值定义
#define CDCA_RC_OK                    0x00  /* 成功 */
#define CDCA_RC_UNKNOWN               0x01  /* 未知错误 */
#define CDCA_RC_POINTER_INVALID       0x02  /* 指针无效 */
#define CDCA_RC_CARD_INVALID          0x03  /* 智能卡无效 */
#define CDCA_RC_PIN_INVALID           0x04  /* PIN码无效 */
#define CDCA_RC_DATASPACE_SMALL       0x06  /* 所给的空间不足 */
#define CDCA_RC_CARD_PAIROTHER        0x07  /* 智能卡已经对应别的机顶盒 */
#define CDCA_RC_DATA_NOT_FIND         0x08  /* 没有找到所要的数据 */
#define CDCA_RC_PROG_STATUS_INVALID   0x09  /* 要购买的节目状态无效 */
#define CDCA_RC_CARD_NO_ROOM          0x0A  /* 智能卡没有空间存放购买的节目 */
#define CDCA_RC_WORKTIME_INVALID      0x0B  /* 设定的工作时段无效 */
#define CDCA_RC_IPPV_CANNTDEL         0x0C  /* IPPV节目不能被删除 */
#define CDCA_RC_CARD_NOPAIR           0x0D  /* 智能卡没有对应任何的机顶盒 */
#define CDCA_RC_WATCHRATING_INVALID   0x0E  /* 设定的观看级别无效 */
#define CDCA_RC_CARD_NOTSUPPORT       0x0F  /* 当前智能卡不支持此功能 */
#define CDCA_RC_DATA_ERROR            0x10  /* 数据错误，智能卡拒绝 */
#define CDCA_RC_FEEDTIME_NOT_ARRIVE   0x11  /* 喂养时间未到，子卡不能被喂养 */
#define CDCA_RC_CARD_TYPEERROR        0x12  /* 子母卡喂养失败，插入智能卡类型错误 */

//---------- 5. ECM_PID设置的操作类型
#define CDCA_LIST_OK                  0x00
#define CDCA_LIST_FIRST               0x01
#define CDCA_LIST_ADD                 0x02

//---------- 6. 邮件应用相关定义
//------------ 6.1. 邮件大小及数量限制
#define CDCA_MAXNUM_EMAIL             100U   /* 机顶盒保存的最大邮件个数 */
#define CDCA_MAXLEN_EMAIL_TITLE       30U    /* 邮件标题的长度 */
#define CDCA_MAXLEN_EMAIL_CONTENT     160U   /* 邮件内容的长度 */

//------------ 6.2. 邮件图标显示方式
#define CDCA_Email_IconHide           0x00  /* 隐藏邮件通知图标 */
#define CDCA_Email_New                0x01  /* 新邮件通知，显示新邮件图标 */
#define CDCA_Email_SpaceExhaust       0x02  /* 磁盘空间以满，图标闪烁。 */

//---------- 7. OSD应用相关定义
//------------ 7.1. OSD的长度限制
#define CDCA_MAXLEN_OSD               180U  /* OSD内容的最大长度 */

//------------ 7.2. OSD显示类型
#define CDCA_OSD_TOP                  0x01  /* OSD风格：显示在屏幕上方 */
#define CDCA_OSD_BOTTOM               0x02  /* OSD风格：显示在屏幕下方 */
#define CDCA_OSD_FULLSCREAN           0x03  /* OSD风格：整屏显示 */
#define CDCA_OSD_HALFSCREAN           0x04  /* OSD风格：半屏显示 */


//---------- 8. IPPV/IPPT应用相关定义
//------------ 8.1. IPPV/IPPT不同购买阶段提示
#define CDCA_IPPV_FREEVIEWED_SEGMENT  0x00  /* IPPV免费预览阶段，是否购买 */
#define CDCA_IPPV_PAYVIEWED_SEGMENT   0x01  /* IPPV收费阶段，是否购买 */
#define CDCA_IPPT_PAYVIEWED_SEGMENT   0x02  /* IPPT收费段，是否购买 */

//------------ 8.2. IPPV价格类型
#define CDCA_IPPVPRICETYPE_TPPVVIEW       0x0  /* 不回传，不录像类型 */
#define CDCA_IPPVPRICETYPE_TPPVVIEWTAPING 0x1  /* 不回传，可录像类型 */

//------------ 8.3. IPPV节目的状态
#define CDCA_IPPVSTATUS_BOOKING       0x01  /* 预定 */
#define CDCA_IPPVSTATUS_VIEWED        0x03  /* 已看 */


//---------- 9. 频道锁定应用相关定义
#define CDCA_MAXNUM_COMPONENT         5U    /* 节目组件最大个数 */
#define CDCA_MAXLEN_LOCKMESS          40U

//---------- 10. 反授权确认码应用相关定义
#define CDCA_Detitle_All_Readed       0x00  /* 所有反授权确认码已经被读，隐藏图标 */
#define CDCA_Detitle_Received         0x01  /* 收到新的反授权码，显示反授权码图标 */
#define CDCA_Detitle_Space_Small      0x02  /* 反授权码空间不足，改变图标状态提示用户 */
#define CDCA_Detitle_Ignore           0x03  /* 收到重复的反授权码，可忽略，不做处理 */
	
//---------- 10. 进度条提示信息
#define CDCA_SCALE_RECEIVEPATCH       1     /* 升级数据接收中 */
#define CDCA_SCALE_PATCHING           2     /* 智能卡升级中 */

/*-------------------------------------end of 宏定义--------------------------------------*/



/*----------------------------------------数据结构----------------------------------------*/

//---------- 1. 系统时间
typedef CDCA_U32  CDCA_TIME;
typedef CDCA_U16  CDCA_DATE;

//---------- 2. 信号量定义（不同的操作系统可能不一样）
typedef CDCA_U32  CDCA_Semaphore;

//---------- 3. 运营商信息
typedef struct {
	char  m_szTVSPriInfo[CDCA_MAXLEN_TVSPRIINFO+1];  /* 运营商私有信息 */
}SCDCAOperatorInfo;

//---------- 4. 节目信息
typedef struct {
	CDCA_U16	m_wEcmPid;														/*节目相应控制信息的PID*/				
	CDCA_U8		m_byServiceNum;												/*当前PID下的节目个数*/
	CDCA_U8   m_byReserved;                         /* 保留 */
	CDCA_U16 	m_wServiceID[CDCA_MAXNUM_PROGRAMBYCW];	/*当前PID下的节目ID列表*/	
}SCDCASServiceInfo;

//---------- 5. 普通授权信息
//------------ 5.1. 授权信息
typedef struct {
	CDCA_U32 		m_dwProductID;		/*普通授权的节目ID*/
	CDCA_DATE		m_tBeginDate;		/*授权的起始时间*/
	CDCA_DATE		m_tExpireDate;		/*授权的过期时间*/
	CDCA_U8   	m_bCanTape;     /* 用户是否购买录像：1－可以录像；0－不可以录像 */
	CDCA_U8   	m_byReserved[3];/* 保留 */
}SCDCAEntitle;

//------------ 5.2. 授权信息集合
typedef struct {
	CDCA_U16      m_wProductCount;
  CDCA_U8       m_m_byReserved[2];               /* 保留 */
  SCDCAEntitle  m_Entitles[CDCA_MAXNUM_ENTITLE]; /* 授权列表 */
}SCDCAEntitles;

//---------- 6. IPPV信息
//------------ 6.1. 钱包信息
typedef struct {
  CDCA_U32  m_wCreditLimit; /* 信用度（点数）*/
  CDCA_U32  m_wBalance;     /* 已花的点数 */
}SCDCATVSSlotInfo;

//------------ 6.2. IPPV节目的价格
typedef struct {
  CDCA_U16  m_wPrice;       /* 节目价格（点数）*/
  CDCA_U8   m_byPriceCode;  /* 节目价格类型 */
  CDCA_U8   m_byReserved;   /* 保留 */
}SCDCAIPPVPrice; 

//------------ 6.3. IPPV节目购买提示信息
typedef struct {
  CDCA_U32        m_dwProductID;              /* 节目的ID */
  CDCA_U16        m_wTvsID;                   /* 运营商ID */
  CDCA_U8         m_bySlotID;                 /* 钱包ID */
  CDCA_U8         m_byPriceNum;               /* 节目价格个数 */
  SCDCAIPPVPrice  m_Price[CDCA_MAXNUM_PRICE]; /* 节目价格 */
	CDCA_DATE       m_wExpiredDate;             /* 节目过期时间 */
}SCDCAIppvBuyInfo;

//------------ 6.4. IPPV节目信息
typedef struct {
	CDCA_U32   m_dwProductID;       /* 节目的ID */
  CDCA_U8    m_byBookEdFlag;      /* 产品状态：BOOKING，VIEWED */ 
  CDCA_U8    m_bCanTape;          /* 是否可以录像：1－可以录像；0－不可以录像 */
  CDCA_U16   m_wPrice;            /* 节目价格 */
  CDCA_DATE  m_wExpiredDate;      /* 节目过期时间 */
  CDCA_U8    m_bySlotID;          /* 钱包ID */
}SCDCAIppvInfo;


//---------- 7. 邮件信息
//------------ 7.1. 邮件头
typedef struct {
  CDCA_U32   m_dwActionID;                             /* Email ID */
  CDCA_U8    m_bNewEmail;                              /* 新邮件标记：0－不是新邮件；1－新邮件 */
  char       m_szEmailHead[CDCA_MAXLEN_EMAIL_TITLE+1]; /* 邮件标题，最长为30 */        
	CDCA_U32   m_tCreateTime;                            /* EMAIL创建的时间 */
  CDCA_U16   m_wImportance;                            /* 重要性： 0－普通，1－重要 */
}SCDCAEmailHead;

//------------ 7.2. 邮件内容
typedef struct {
  char    m_szEmail[CDCA_MAXLEN_EMAIL_CONTENT+1];      /* Email的正文 */
}SCDCAEmailContent;


//---------- 8. 频道锁定信息
//------------ 8.1. 节目组件信息
typedef struct {    /* 组件用于通知机顶盒节目类型及PID等信息，一个节目可能包含多个组件 */
  CDCA_U16   m_wCompPID;     /* 组件PID */
  CDCA_U16   m_wECMPID;      /* 组件对应的ECM包的PID，如果组件是不加扰的，则应取0。 */
  CDCA_U8    m_CompType;     /* 组件类型 */
  CDCA_U8    m_byReserved[3];/* 保留 */
}SCDCAComponent;

//------------ 8.2. 频道参数信息
typedef struct {        
  CDCA_U32       m_dwFrequency;                          /* 频率，BCD码 */
  CDCA_U32       m_symbol_rate;                          /* 符号率，BCD码 */
  CDCA_U16       m_wPcrPid;                              /* PCR PID */
  CDCA_U8        m_fec_outer;                            /* 前项纠错外码 */
  CDCA_U8        m_fec_inner;                            /* 前项纠错内码 */
  CDCA_U8        m_Modulation;                           /* 调制方式 */
  CDCA_U8        m_ComponentNum;                         /* 节目组件个数 */
  SCDCAComponent m_CompArr[CDCA_MAXNUM_COMPONENT];       /* 节目组件列表 */
  char           m_szBeforeInfo[CDCA_MAXLEN_LOCKMESS+1]; /* 保留 */
  char           m_szQuitInfo[CDCA_MAXLEN_LOCKMESS+1];   /* 保留 */
  char           m_szEndInfo[CDCA_MAXLEN_LOCKMESS+1];    /* 保留 */
}SCDCALockService;
	
/*-----------------------------------------------------------------------------------
a. 本系统中，参数m_dwFrequency和m_symbol_rate使用BCD码，编码前取MHz为单位。
   编码时，前4个4-bit BCD码表示小数点前的值，后4个4-bit BCD码表示小数点后的值。
   例如：
		若频率为642000KHz，即642.0000MHz，则对应的m_dwFrequency的值应为0x06420000；
		若符号率为6875KHz，即6.8750MHz，则对应的m_symbol_rate的值应为0x00068750。

b. 本系统中，m_Modulation的取值如下：
	0		Reserved
	1		QAM16
	2		QAM32
	3		QAM64
	4		QAM128
	5		QAM256
	6～255	Reserved
------------------------------------------------------------------------------------*/ 


/*------------------------------------end of 数据结构-------------------------------------*/



/*---------------------------以下接口是CA_LIB提供给STB-------------------------------------*/

//-------------------- CA_LIB调度管理 --------------------
/*++
 * 在CA中将启动几个任务，byThreadPrior就是这几个任务的优先级
-*/
extern CDCA_BOOL  CDCASTB_Init(CDCA_U8 byThreadPrior);

/*++
 * 这个函数将被用来判断是否是CASYSTEMID能够被当前CA识别	
-*/
extern CDCA_BOOL  CDCASTB_IsCDCa(CDCA_U16 wCaSystemID);

//-------------------- Flash管理 --------------------
/*+
 *  这个函数用来重新格式化分配给CA的数据flash 
 *  如果flash的内容由于不正常关机或者其它原因导致错误可调用这个函数重新格式化flash
 *  目前只支持FormatType取值为0的应用，非0时CALIB不对flash做任何操作
-*/
extern void  CDCASTB_FormatBuffer(CDCA_U8 byFormatType);
extern void  CDCASTB_RequestMaskBuffer(void);
extern void  CDCASTB_RequestUpdateBuffer(void);

//-------------------- TS流管理 --------------------
/*++
	if(用户切换频道||或者当前频道中ECMPID发生增加或者减少)
	{
		CDCASTB_SetEcmPid(CDCA_LIST_FIRST,NULL);
		if(如果当前频道中有ECMPID,既节目是加扰的) //有几个ECMPID送几次，ECMPID相同送一次就可以
			CDCASTB_SetEcmPid(CDCA_LIST_ADD,pServiceInfo);
		CDCASTB_SetEcmPid(CDCA_LIST_OK,NULL);
	}
--*/
extern void  CDCASTB_SetEcmPid(CDCA_U8 byType,  SCDCASServiceInfo* pServiceInfo);
extern void  CDCASTB_SetEmmPid(CDCA_U16 wEmmPid);
extern void  CDCASTB_PrivateDataGot(CDCA_U8 byReqID, CDCA_BOOL bOK, CDCA_U16 wPid, const CDCA_U8* pbyReceiveData, CDCA_U16 wLen);

//-------------------- 智能卡管理 --------------------
extern CDCA_BOOL  CDCASTB_SCInsert(void);
extern void  			CDCASTB_SCRemove(void);
/*
 说明 ：
	读取智能卡序列号，智能卡序列号是16个字节组成得字符串
例子：
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

级别说明：
   byRating的范围是[4,18],是年龄 
*/
extern CDCA_U16  CDCASTB_SetRating(const CDCA_U8* pbyPin, CDCA_U8 byRating);
extern CDCA_U16  CDCASTB_GetRating(CDCA_U8* pbyRating);

extern CDCA_U16  CDCASTB_SetWorkTime(const CDCA_U8* pbyPin, CDCA_U8 byStartHour, CDCA_U8 byStartMin, CDCA_U8 byStartSec, CDCA_U8 byEndHour, CDCA_U8 byEndMin, CDCA_U8 byEndSec);
extern CDCA_U16  CDCASTB_GetWorkTime(CDCA_U8* pbyStartHour, CDCA_U8* pbyStartMin, CDCA_U8* pbyStartSec, CDCA_U8* pbyEndHour, CDCA_U8* pbyEndMin, CDCA_U8* pbyEndSec);
//-------------------- 基本信息查询 --------------------
extern CDCA_U32 CDCASTB_GetVer(void);
/*++
说明 ：
	这个pwTVSID至少指向CDCA_MAXNUM_OPERATOR个CDCA_U16数组的地址，如果没有运营商pwTVSID[i]值是0
例子：
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
				printf("运营商ID： %d 运营商名称：%s \r\n",wArrTvsID[i],OperatorInfo.m_szTVSPriInfo);
			}
		}
	}
--*/
extern CDCA_U16 CDCASTB_GetOperatorIds(CDCA_U16* pwTVSID);
extern CDCA_U16 CDCASTB_GetOperatorInfo(CDCA_U16 wTVSID,  SCDCAOperatorInfo* pOperatorInfo);
/*++
说明 ：
	这个pACArray至少指向CDCA_MAXNUM_ACLIST个CDCA_U32数组的地址
例子：
		CDCA_U32  ACArray[CDCA_MAXNUM_ACLIST];
    CDCA_U16  wOperatorID = 12345;
    if(CDCASTB_GetACList(wOperatorID,ACArray) == CDCA_RC_OK)
	{
		printf("Get operator AC OK!);
	}
--*/
extern CDCA_U16 CDCASTB_GetACList(CDCA_U16 wTVSID, CDCA_U32* pACArray);
/*++
说明 ：
	获得某一个运营商的授权信息
例子：
	 SCDCAEntitles Entitle;
        CDCA_U16        wRet = 0;
        CDCA_U16        wIndex;
        wRet= CDCASTB_GetServiceEntitles(1,&Entitle);
        if(wRet == CDCA_RC_OK)
        {
                printf("共有授权 %d个",Entitle.m_wProductCount);
                for(wIndex =0 ; wIndex < Entitle.m_wProductCount ; wIndex++)
                {
                        printf("节目ID %d\n:",Entitle.m_Entitles[wIndex].m_dwProductID);
                        printf("节目是否允许录像 %d\n:",Entitle.m_Entitles[wIndex].m_bCanTape);
                        printf("授权结束时间\n:",Entitle.m_Entitles[wIndex].m_tExpireDate);
                }
        }
        else
                printf("相应的错误");
*/
extern CDCA_U16 CDCASTB_GetServiceEntitles(CDCA_U16 wTVSID, SCDCAEntitles* pServiceEntitles);

//-------------------- 授权信息管理 --------------------
/*++
说明 ：
	获得某一个运营商的授权ID信息
例子：
	          CDCA_U32   EntitleIds[CDCA_MAXNUM_ENTITLE];
         CDCA_U16 wRet = 0,wOperatorID=1;
         wRet = CDCASTB_GetEntitleIDs(wOperatorID,EntitleIds);
         if(wRet == CDCA_RC_OK)
         {
          CDCA_U16 wIndex=0;
          for(wIndex =0 ; wIndex < CDCA_MAXNUM_ENTITLE ; wIndex++)
          {
           if(EntitleIds[wIndex]==0) break;
           printf("节目ID %d\n:",EntitleIds[wIndex]);
          }
         }
         else
          printf("相应的错误");
*/
extern CDCA_U16 CDCASTB_GetEntitleIDs(CDCA_U16 wTVSID, CDCA_U32* pdwEntitleIds);

extern CDCA_U16  CDCASTB_GetDetitleChkNums(CDCA_U16 wTvsID,  CDCA_U32 *pdwDetitleChkNums);
extern CDCA_BOOL CDCASTB_GetDetitleReaded(CDCA_U16 wTvsID);
extern CDCA_BOOL CDCASTB_DelDetitleChkNum(CDCA_U16 wTvsID, CDCA_U32 dwDetitleChkNum);

//-------------------- 机卡对应 --------------------

/*++
说明 ：
	检查当前机顶盒和智能卡对应的情况并返回智能卡对应的机顶盒的STBID列表
		pbyNum：当前智能卡对应的机顶盒的数量，最大为5
		pbySTBID_List：指向一个数据缓存区用于返回STBID列表，每个STBID为6字节
例子：
 				CDCA_U16  wRet = 0;
        wRet = CDCASTB_IsPaired(pbyNum, pbySTBID_List);
        if(wRet == CDCA_RC_OK)
                printf("当前机卡对应" & STBID_List);        
        else if(wRet == CDCA_RC_CARD_INVALID)
                printf("检测不到有效智能卡");        
        else if(wRet == CDCA_RC_CARD_NOPAIR)
                printf("智能卡没有对应任何机顶盒" & "请设置机卡对应");        
        else if(wRet == CDCA_RC_CARD_PAIROTHER)
                printf("智能卡已经对应其它机顶盒" & STBID_List);        
--*/
extern CDCA_U16 CDCASTB_IsPaired(CDCA_U8* pbyNum, CDCA_U8* pbySTBID_List);
extern CDCA_U16 CDCASTB_GetPlatformID(void);

//-------------------- IPPV应用 --------------------

	/*++
	说明 ：
		这个byArrSlotID必须指向CDCA_MAXLEN_SLOT数组的地址
	例子：
		CDCA_U8  byArrSlotID[CDCA_MAXNUM_SLOT];
        if(CDCASTB_GetSlotIDs(1,byArrSlotID) == CDCA_RC_OK)
        {
                for(CDCA_U8 i = 0;i<CDCA_MAXNUM_SLOT;i++)
                {
                        if(byArrSlotID[i] != 0)
                        {
                                printf("钱包ID ＝ %4x\r\n",byArrSlotID[i]);
                        }
                        else
                                break;
                }
        }
	--*/
	extern CDCA_U16 CDCASTB_GetSlotIDs(CDCA_U16 wTVSID,  CDCA_U8* pbySlotID);
	extern CDCA_U16 CDCASTB_GetSlotInfo(CDCA_U16 wTVSID, CDCA_U8 bySlotID, SCDCATVSSlotInfo* pSlotInfo);
	/*++
	说明 ：
		获得一个运营商的所有的IPPV节目
	例子：
		 CDCA_U16  wNumber;
        SCDCAIppvInfo  ArrIppv[CDCA_MAXNUM_IPPVP];
        wNumber = CDCA_MAXNUM_IPPVP;
        if(CDCASTB_GetIPPVProgram(1,ArrIppv,&wNumber) == CDCA_RC_OK)
        {
                for(CDCA_U8 i = 0;i<wNumber;i++)
                {
                        printf("节目ID ＝ %4x\r\n",ArrIppv[i].m_wProductID);
                }
        }
	--*/
	extern CDCA_U16 CDCASTB_GetIPPVProgram(CDCA_U16 wTVSID,  SCDCAIppvInfo* pIppv, CDCA_U16* pwNumber);
	/*++
	说明 ：
		用户对IPPV节目是否购买进行选择的时候，调用该函数
	例子：
	   在IPPV提示购买的对话框显示在电视上的时候
        CDCA_U16 wRet =0;
        if(用户选择购买)
        {
                用户输入PIN;
                CDCA_U8 byPin[CDCA_MAXLEN_PINCODE]={用户输入的PIN};
                wRet = CDCASTB_StopIPPVBuyDlg(CDCA_TRUE, wEcmPid,byPin,Price);
                switch(wRet)
                {
                        case CDCA_RC_OK;
                                printf("购买成功");
                                break;
                        case other:
                                printf("其它相应错误");
                                break;
                        default:
                                break;
                }
        }        
        else
                CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,wEcmPid,NULL,NULL);
	 */
	extern CDCA_U16 CDCASTB_StopIPPVBuyDlg(CDCA_BOOL bBuyProgram, CDCA_U16 wEcmPid, CDCA_U8* pbyPinCode, SCDCAIPPVPrice* pPrice);


//-------------------- 邮件管理 --------------------

	/*++
	说明 ：
		一下方法可以获得邮箱中全部邮件头信息
	注意：
		byFromIndex ＝ 5并不是从逻辑上的第五封信开始取，而是从邮箱物理空间的第5封邮件位置开始取，
	例子：
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
                                printf("邮件ID ＝ 0x%8x\r\n",ArrEmailHead[i].m_dwActionID);
                                ArrEmailHead[i].m_szEmailHead[CDCA_MAXLEN_EMAIL_TITLE] = 0; //防止溢出＋＋＋
                                printf("邮件标题 ＝ %s\r\n",ArrEmailHead[i].m_szEmailHead);        
                                if(CDCASTB_GetEmailContent(ArrEmailHead[i].m_dwActionID,&EmailContent)==CDCA_RC_OK)
                                {
                                        EmailContent.m_szEmail[CDCA_MAXLEN_EMAIL_CONTENT] = 0;//防止溢出＋＋＋
                                        printf("邮件内容 ＝ %s\r\n",EmailContent.m_szEmail);        
                                }
                                //如果想删掉邮件
                                CDCASTB_DelEmail(ArrEmailHead[i].m_dwActionID);
                        }
                }
                if(byCount < 10) //所有邮件都已经取出
                        break;
        }
	--*/
	extern CDCA_U16 CDCASTB_GetEmailHeads( SCDCAEmailHead* pEmailHead,  CDCA_U8* pbyCount,  CDCA_U8* pbyFromIndex);
	extern CDCA_U16 CDCASTB_GetEmailHead(CDCA_U32 dwEmailID,  SCDCAEmailHead* pEmailHead);
	extern CDCA_U16 CDCASTB_GetEmailContent(CDCA_U32 dwEmailID,  SCDCAEmailContent* pEmailContent);
		/*++
	说明 ：
		删除某一封邮件的方法在TFCASTB_GetEmailHeads使用介绍中已经介绍
		下面介绍删除所有邮件的方法 
	说明：
		使用CDCASTB_DelEmail来删除所有邮件，
	例子：
		CDCASTB_DelEmail(0);
	--*/
	extern void	CDCASTB_DelEmail(CDCA_U32 dwEmailID);
	/*++
	说明 ：
		获得信箱使用的情况
	例子：
				CDCA_U8    byEmailNum;
        CDCA_U8    byEmptyNum;
        if(CDCASTB_GetEmailSpaceInfo(&byEmailNum,&byEmptyNum) == CDCA_RC_OK)
        {
                printf("信箱中有邮件 ＝ %d个\n",byEmailNum);
                printf("信箱还可容纳邮件 ＝ %d个\n",byEmptyNum);
        }
	--*/
	extern CDCA_U16	CDCASTB_GetEmailSpaceInfo( CDCA_U8* pbyEmailNum,  CDCA_U8* pbyEmptyNum);


//-------------------- 子母卡应用 --------------------
/*
 说明 ：
	从母卡读取对应数据，只对母卡有效
 参数：
    pFeedData：指向返回数据的空间，空间大小不小于256字节
	pbyLen：   返回从卡中读取到数据的长度 
例子：
	
        CDCA_U8        data[256];
        CDCA_U8        len = 0;
        wRet= CDCASTB_ReadFeedDataFromParent(data,&len);
        if(wRet==CDCA_RC_OK)
                printf("获取母卡数据");
        }

 */
extern CDCA_U16  CDCASTB_ReadFeedDataFromParent(CDCA_U16 wTVSID,  CDCA_U8* pbyFeedData,  CDCA_U8* pbyLen);
/*
 说明 ：
	把从母卡读取的数据写入子卡，只对子卡有效
 参数：
    pFeedData：指向写入子卡的数据的空间，指向的数据是从调用CDCASTB_ReadFeedDataFromParent从母卡中返回的
        byLen：    写入子卡的数据长度 
例子：
        wRet= CDCASTB_WriteFeedDataToChild(data,len);
        if(wRet==CDCA_RC_OK)
                printf("写对应数据成功");
        }
 */
extern CDCA_U16  CDCASTB_WriteFeedDataToChild( CDCA_U8* pbyFeedData, CDCA_U8 byLen);
/*++
说明 ：
	获取运营商的子母卡状态
参数：
	  		wTVSID：想要查询的运营商ID
        pbIsChild：如果返回turue（1）表明在运营商的系统内，此卡是子卡
        pbyDelayTime：两次对应的时间间隔，单位是小时。只有是子卡的时候此值才有效
        pLastFeedTime：上次对应的时间。只有是子卡的时候此值才有效
例子：
        CDCA_TIME  pLastFeedTime;
        CDCA_U8    byDelayTime;
        CDCA_BOOL    bIsChild;
    		CDCA_U16   wOperatorID = 12345;
    		if(CDCASTB_GetOperatorChildStatus(wOperatorID,&bIsChild,&byDelayTime,&pLastFeedTime) == CDCA_RC_OK)
        {
                printf("获取运营商子母卡信息!);
        }
--*/
extern CDCA_U16 CDCASTB_GetOperatorChildStatus(CDCA_U16 wTVSID,  CDCA_U8* pbyIsChild,  CDCA_U8* pbyDelayTime,  CDCA_TIME* pLastFeedTime);


/*-------------------- 显示界面管理 --------------------*/
extern void CDCASTB_RefreshInterface(void);


/*------------------------以上接口是CA_LIB提供给STB----------------------------------------*/

	
/*------------------------以下接口是STB提供给CA_LIB----------------------------------------*/

//-------------------- 线程管理 --------------------
extern CDCA_BOOL CDSTBCA_RegisterTask( const char* szName, CDCA_U8 byPriority,  void* pTaskFun,  void* pParam, CDCA_U16 wStackSize);
extern void      CDSTBCA_Sleep(CDCA_U16 wMilliSeconds);

//-------------------- 信号量管理 --------------------
/*++
说明 ：
	对信号量进行初始化
调用的例子：
	 			CDCA_Semaphore  Semaphore1;
        CDSTBCA_SemaphoreInit(&Semaphore1,CDCA_TRUE);
        CDCA_Semaphore  Semaphore2;
        CDSTBCA_SemaphoreInit(&Semaphore2,CDCA_FALSE);
        结果 Semaphore1有信号
           　Semaphore2没有信号
--*/
extern void CDSTBCA_SemaphoreInit( CDCA_Semaphore* pSemaphore, CDCA_BOOL bInitVal);
extern void CDSTBCA_SemaphoreSignal( CDCA_Semaphore* pSemaphore );
extern void CDSTBCA_SemaphoreWait( CDCA_Semaphore* pSemaphore );

//-------------------- 内存管理 --------------------
/*++

--*/
extern void* CDSTBCA_Malloc(CDCA_U8 byBufSize);
extern void  CDSTBCA_Free(void* pBuf);
extern void  CDSTBCA_Memset(void* pDestBuf, CDCA_U8 c, CDCA_U16 wSize);
extern void  CDSTBCA_Memcpy( void* pDestBuf, const void* pSrcBuf, CDCA_U16 wSize );

//-------------------- Flash管理 --------------------
/*++
说明 ：
    获取falsh的开始地址和长度,其中开始地址在CA模块不会修改
调用的例子：
		CDCA_U32 g_STBMemBeginAdd = 0,g_STBMemLen = 0;
		CDSTBCA_GetBuffer(&g_STBMemBeginAdd,&g_STBMemLen);
		if(g_STBMemLen<64*1024){
				CDSTBCA_Printf(0,"flash is small\r\n");
		}
机顶盒实现的例子:
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
说明 ：
	CA模块只在启动的时候读取一次falsh
调用的例子：
		CDCA_U8	 temp[1000];
		CDCA_U32 dwreadLen = 1000;
		CDSTBCA_ReadBuffer(g_STBMemBeginAdd,temp,&dwreadLen);
		if(dwReadLen != 1000){
			CDSTBCA_Printf(0,"CDSTBCA_ReadBuffer is fail");
	}
--*/
extern void CDSTBCA_ReadBuffer(CDCA_U32 dwStartAddr, CDCA_U8* pbyData, CDCA_U32* pdwLen);
/*++
说明 ：
    CA运行的过程中会多次写flash,每次的地址和长度都是一样的

--*/
extern void CDSTBCA_WriteBuffer(CDCA_U32 dwStartAddr, const CDCA_U8* pbyData, CDCA_U32 dwLen);

//-------------------- TS流管理 --------------------
/*++
    //注意 具体的EMMPID 由CAT中分析出来
	 CDCA_U8 byFilter[8]={0,1,2,3,4,5,6,7};
   CDCA_U8 byMask[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
   if(CDSTBCA_GetPrivateData(2,byFilter,byMask,8, 0x211, 2)
	　{
 					//CA内部更改保存的过滤器状态
	　}
		else
	　{
 					// 设置失败将重新设置
		}
-*/
extern void	CDSTBCA_ScrSetCW(CDCA_U16 wEcmPID, const CDCA_U8* pbyOddKey, const CDCA_U8* pbyEvenKey, CDCA_U8 byKeyLen, CDCA_BOOL bTapingEnabled);
extern CDCA_BOOL CDSTBCA_GetPrivateData(CDCA_U8 byReqID, const CDCA_U8* pbyFilter, const CDCA_U8* pbyMask, CDCA_U8 byLen, CDCA_U16 wPid, CDCA_U8 byWaitSeconds);


//-------------------- 智能卡管理 --------------------
extern CDCA_BOOL CDSTBCA_SCReset(CDCA_U8* pbyATR,  CDCA_U8* pbyLen);
/*++
   在这里我举一个实现的模型 具体怎么做由机顶盒决定
 	
	  CDSTBCA_SCPBRun( const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen)
        {
            命令字节送卡
            if(不成功)
                return CDCA_FALSE;
            获取卡返回的数据放到pbyReply //指针指向的空间由CA分配管理 t0协议中返回数据长度不会大于256个字节
            if(不成功)
                 return CDCA_FALSE;
             *pwReplyLen等于返回的数据长度
              pbyReply[*pwReplyLen] = 过程字节1
              pbyReply[(*pwReplyLen)+1] = 过程字节2
              *pwReplyLen+=2;
              return CDCA_TRUE;
         }
-*/
extern CDCA_BOOL CDSTBCA_SCPBRun( const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen);

//-------------------- 授权信息管理 --------------------
/*当智能卡授权发生变化的时候，调用接口通知机顶盒*/
extern void CDSTBCA_EntitleChanged(CDCA_U16 wTvsID);

extern void CDSTBCA_DetitleReceived(CDCA_U16 wTvsID, CDCA_U8 bstatus);

//-------------------- 安全控制 --------------------
/*获取机顶盒的唯一编号
 pwPlatfromID:永新同方分配给机顶盒厂商的ID，同一个机顶盒厂商不同芯片具有不同数值
 pdwUniqueID:机顶盒的唯一标志,在同一个PlatfromID下面UniqueID机顶盒厂商必须唯一
*/
extern void     CDSTBCA_GetSTBID( CDCA_U16* pwPlatformID,  CDCA_U32* pdwUniqueID);
extern CDCA_U16 CDSTBCA_SCFunction( CDCA_U8* pData);


//-------------------- IPPV应用 --------------------
extern void CDSTBCA_HideIPPVDlg(CDCA_U16 wEcmPid);
extern void CDSTBCA_StartIppvBuyDlg(CDCA_U8 byMessageType, CDCA_U16 wEcmPid,  SCDCAIppvBuyInfo *pIppvProgram);

//-------------------- 邮件/OSD显示管理 --------------------
extern void CDSTBCA_ShowOSDMessage(CDCA_U8 byStyle,  char* szMessage);
extern void CDSTBCA_HideOSDMessage(CDCA_U8 byStyle);
/*++
        当byShow＝CDCA_Email_New && dwEmailID!=0 的时候dwEmailID是一封新邮件的ID
         这个时候可以调用        CDCASTB_GetEmailHead
                                                 CDCASTB_GetEmailContent 获取这封邮件的标题和内容
        如果邮箱内有多封新邮件，这个ID是其中的某一封邮件的ID
         机顶盒也可以不关心这个ID，因为CDCASTB_GetEmailHeads会返回邮箱内所有的邮件的标题信息内容的
 -*/
extern void CDSTBCA_EmailNotifyIcon(CDCA_U8 byShow, CDCA_U32 dwEmailID);

//-------------------- 子母卡应用 --------------------
/*
 说明 ：
	请求机顶盒进入子卡喂养界面
*/
extern void  CDSTBCA_RequestFeeding(CDCA_U16 wTVSID);


//-------------------- 强制切换频道 --------------------
extern void CDSTBCA_LockService( SCDCALockService* pLockService);
extern void CDSTBCA_UNLockService(void);

//-------------------- 显示界面管理 --------------------
extern void CDSTBCA_ShowProgressStrip(CDCA_U8 byProgress, CDCA_U8 byMark);
extern void CDSTBCA_ShowBuyMessage(CDCA_U16 wEcmPID, CDCA_U8 byMessageType);
extern void CDSTBCA_ShowFingerMessage(CDCA_U16 wEcmPID, CDCA_U32 dwCardID);

//-------------------- 其它 --------------------
extern CDCA_U16 CDSTBCA_Strlen( char* pString);
extern void     CDSTBCA_Printf(CDCA_U8 byLevel,  const char* szMesssage);

/*---------------------------以上接口是STB提供给CA_LIB-------------------------------------*/

#ifdef  __cplusplus
}
#endif
#endif
/*EOF*/

