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
	FYF_CA_SV_MAIL_HEAD,	/* 读取邮件头 */
	FYF_CA_SV_MAIL_BODY,	/* 读取邮件正文,最多包含512个有效字节,不包含NULL */
	FYF_CA_SV_MSG,			/* 读取CA消息,最多包含128个有效字节,不包含NULL */
	FYF_CA_SV_PPV,          /* 订购或退订当前PPV,订购使用FYF_TRUE,退订使用FYF_FALSE */
	FYF_CA_SV_OPERATOR_INFO,/* 获得所有运营商信息 */
	FYF_CA_SV_MONEY_INFO,	/* 获得电子钱包信息 */
	FYF_CA_SV_CORRESPOND_INFO, /* 子母卡配对信息 */	
	FYF_CA_SV_EMAIL_COUNT,	/* 获取邮件总数 */
	FYF_CA_SV_EMAIL_READED, /* 邮件已读 */
	FYF_CA_SV_EMAIL_DEL,	/* 删除邮件 */
	FYF_CA_SV_WORK_TIME,	/* 工作时段 */
	FYF_CA_SV_VERIFY_PIN,	/* 检查PIN */
	FYF_CA_SV_GET_BOOK_IPP,	/* 获取IPP节目信息 */
	FYF_CA_SV_VIEWED_PPV,	/* 已观看IPPV*/
	FYF_CA_SV_IPP_OVER,     /* 实时订购结束*/
	FYF_CA_SV_RATING,		/* 修改成人级 */
	FYF_CA_SV_SEVICE_ENTITLE, /*授权信息*/
	FYF_CA_SV_CARD_INFO, 		/*卡基本信息*/
	FYF_CA_SV_PIN_LOCK, 		/*pin是否被锁*/
	FYF_CA_SV_AREA_INFO, 		/*区域信息*/
	FYF_CA_SV_MOTHER_INFO, 		/*母卡卡号*/
	FYF_CA_SV_CHANGE_PIN,	/*修改PIN*/
	FYF_CA_SV_CARD_STATE,	/* 获得卡状态 */
	FYF_CA_SV_MSG_OVER,		/* 信息显示完成通知 */
	FYF_CA_SV_BOOK_UNBOOK,	/* 订购或退订*/
	FYF_CA_SV_CAS_ID_VERIRY,	/* 检查CASID */
	FYF_CA_SV_SWITCH_CHANNEL_OVER, /* 单频点区域锁定结束 */
	FYF_CA_SV_CARD_IN,		/* 判断卡有没有插入 */
	FYF_CA_SV_Card_Pair,	/*获得机卡配对信息*/
	FYF_CA_SV_DETITLE_READED,	/* 获取反授权确认码读取状态 */
	FYF_CA_SV_DETITLE_CHK_NUMS,	/* 查询反授权确认码信息 */
	FYF_CA_SV_DEL_DETITLE_CHK,	/* 删除反授权确认码 */
	FYF_CA_SV_ACLIST_INFO, /*获得AC List信息*/
	FYF_CA_SV_STBID,			/* 获得STBID */
	FYF_CA_SV_TVS_NAME,			/* 获得运营商名称  */
	FYF_CA_SV_DebugMsgSign			/* 控制回显打印  */
}FYF_CA_SV_e;
/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef enum
{
	FYF_CA_NOTIFY_CARD_OUT,		/* 拔卡通知 */
	FYF_CA_NOTIFY_CARD_IN,		/* 插卡通知 */
	FYF_CA_NOTIFY_MAIL,			/* 邮件通知 */
	FYF_CA_NOTIFY_MSG,			/* 消息通知 */
	FYF_CA_NOTIFY_PPV,			/* PPV通知  */

	FYF_CA_NOTIFY_APP_LOCKED,   /* 服务被锁定 */
	FYF_CA_NOTIFY_CARD_LOCKED,  /* 智能卡已锁定 */
	FYF_CA_NOTIFY_NOT_AUTHORED,	/* 本节目没有授权 */
	FYF_CA_NOTIFY_CARD_NOTFOUND,/* 找不到智能卡 */
	FYF_CA_NOTIFY_NO_MONEY,		/* 智能卡余额不足 */
	FYF_CA_NOTIFY_EXTER_AUTHEN,	/* 智能卡认证错误 */
	FYF_CA_NOTIFY_PIN_AUTHEN,	/* 配对错误 */
	FYF_CA_NOTIFY_NOREG_CARD,	/* 智能卡未注册 */
	FYF_CA_NOTIFY_ILLEGAL_CARD,	/* 智能卡出错 */
	FYF_CA_NOTIFY_SCARD_NOT_ACT,/* 激活子卡 */
	FYF_CA_NOTIFY_SCARD_DISABLE,/* 子卡激活时间到 */
	FYF_CA_NOTIFY_ZONE_CODE,	/* 区域码错误 */

	FYF_CA_NOTIFY_APP_STOP,		/* 智能卡暂停服务 */
	FYF_CA_NOTIFY_APP_RESUME,	/* 智能卡恢复服务 */
	FYF_CA_NOTIFY_PROG_UNAUTH,  /* 取消节目订购 */
	FYF_CA_NOTIFY_PROG_AUTH,	/* 增加节目订购 */
	FYF_CA_NOTIFY_EN_PARTNER,	/* 启用机卡配对模式 */
	FYF_CA_NOTIFY_DIS_PARTNER,	/* 取消机卡配对模式 */
	FYF_CA_NOTIFY_UPDATE_GNO,	/* 智能卡状态已修改 */
	FYF_CA_NOTIFY_UPDATE_GK,	/* 智能卡升级,请插拔卡 */
	FYF_CA_NOTIFY_CHARGE,		/* 帐户远程充值 */
	FYF_CA_NOTIFY_FIGEIDENT,	/* 指纹识别 */
	FYF_CA_NOFITY_SONCARD,		/* 设置子母卡 */
	FYF_CA_NOFITY_URGENT_SERVICE,/*紧急服务通知 */
	FYF_CA_NOTIFY_MODIFY_ZONECODE,/*智能卡区域码已修改 */
	FYF_CA_NOTIFY_LOADER,		/* 收到升级数据 */
	FYF_CA_EMAIL_NEW,			/* 新邮件通知 */
	FYF_CA_EMAIL_NEW_NO_ROOM,	/* 邮箱已满通知 */
	FYF_CA_EMAIL_NONE,			/* 没有邮件通知 */
	FYF_CA_NOTIFY_SCROLL_MSG,	/* 滚动消息通知 */
	FYF_CA_NOTIFY_CLEAR_MSG,	/* 滚动消息通知 */
	
	FYF_CA_NOTIFY_SHOW_PROMPT,  /* 显示提示信息*/
	FYF_CA_NOTIFY_HIDE_PROMPT,   /* 隐藏提示信息*/
	FYF_CA_NOTIFY_SWITCH_CHANNEL, /* 单频点区域锁定 */
	FYF_CA_NOTIFY_AREA_LOCK_OK,		/* 区域锁定成功 */
	FYF_CA_NOTIFY_DETITLE_RECEIVED,	/* 反授权确认码通知 */
	FYF_CA_NOTIFY_PROGRESS_STRIP,	/* 进度显示 */
	FYF_CA_NOFITY_FEEDING_HIDE		/* 隐藏喂养提示 */
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
	FYF_CA_ECM_VIDEO_START,		/* 开始设置视频的ECM */
	FYF_CA_ECM_VIDEO_END,		/* 结束设置视频的ECM */
	FYF_CA_ECM_AUDIO_START,		/* 开始设置音频的ECM */
	FYF_CA_ECM_AUDIO_END		/* 结束设置音频的ECM */
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

