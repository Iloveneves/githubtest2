/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_unf_ecs_type.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2008/6/5
Last Modified :
Description   : ECS API接口
Function List :
History       :
 ******************************************************************************/
#ifndef __HI_UNF_ECS_TYPE_H__
#define __HI_UNF_ECS_TYPE_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*SCICLK引脚输出配置*/
typedef enum  hiUNF_SCI_CLK_MODE_E
{
    HI_UNF_SCI_CLK_MODE_OD = 0,	/*OD输出*/
    HI_UNF_SCI_CLK_MODE_CMOS,		/*CMOS输出*/
    HI_UNF_SCI_CLK_MODE_BUTT      	         
}HI_UNF_SCI_CLK_MODE_E;

/*---- e2prom ----*/
typedef enum hiUNF_E2PROM_CHIP_E
{
    HI_UNF_E2PROM_CHIP_AT24C01,
    HI_UNF_E2PROM_CHIP_AT24C02,
    HI_UNF_E2PROM_CHIP_AT24C04,
    HI_UNF_E2PROM_CHIP_AT24C08,
    HI_UNF_E2PROM_CHIP_AT24C16,
    HI_UNF_E2PROM_CHIP_AT24C32,
    HI_UNF_E2PROM_CHIP_AT24C64,
    HI_UNF_E2PROM_CHIP_AT24C128,
    HI_UNF_E2PROM_CHIP_AT24C256,
    HI_UNF_E2PROM_CHIP_M24LC01,
    HI_UNF_E2PROM_CHIP_M24LC02,
    HI_UNF_E2PROM_CHIP_M24LC04,
    HI_UNF_E2PROM_CHIP_M24LC08,
    HI_UNF_E2PROM_CHIP_M24LC16,
    HI_UNF_E2PROM_CHIP_M24LC32,
    HI_UNF_E2PROM_CHIP_M24LC64,
    HI_UNF_E2PROM_CHIP_M24LC128,
    HI_UNF_E2PROM_CHIP_M24LC256,

    HI_UNF_E2PROM_CHIP_BUTT
}HI_UNF_E2PROM_CHIP_E;

/*---- ir ----*/
typedef enum hiUNF_IR_CODE_E
{
    HI_UNF_IR_CODE_NEC_SIMPLE = 0,
    HI_UNF_IR_CODE_TC9012,
    HI_UNF_IR_CODE_NEC_FULL,
    HI_UNF_IR_CODE_SONY_12BIT,

    HI_UNF_IR_CODE_BUTT
}HI_UNF_IR_CODE_E;

/*---- keyled ----*/
typedef enum hiUNF_KEYLED_LEVEL_E
{
    HI_UNF_KEYLED_LEVEL_1 = 0x01,
    HI_UNF_KEYLED_LEVEL_2,
    HI_UNF_KEYLED_LEVEL_3,
    HI_UNF_KEYLED_LEVEL_4,
    HI_UNF_KEYLED_LEVEL_5,

    HI_UNF_KEYLED_LEVEL_BUTT
}HI_UNF_KEYLED_LEVEL_E;

typedef enum hiUNF_KEYLED_LIGHT_E
{
    HI_UNF_KEYLED_LIGHT_1 = 0x01,
    HI_UNF_KEYLED_LIGHT_2,
    HI_UNF_KEYLED_LIGHT_3,
    HI_UNF_KEYLED_LIGHT_4,
    HI_UNF_KEYLED_LIGHT_ALL,
    HI_UNF_KEYLED_LIGHT_NONE,

    HI_UNF_KEYLED_LIGHT_BUTT
}HI_UNF_KEYLED_LIGHT_E;

typedef struct hiUNF_KEYLED_Time_S
{
    HI_U32 u32Hour;
    HI_U32 u32Minute;
}HI_UNF_KEYLED_TIME_S, *HI_UNF_KEYLED_TIME_S_PTR;

/*---- pmoc ----*/
typedef enum hiUNF_PMOC_MODE_E
{
    HI_UNF_PMOC_MODE_NORMAL = 0,
    HI_UNF_PMOC_MODE_SLOW,
    HI_UNF_PMOC_MODE_DOZE,
    HI_UNF_PMOC_MODE_SLEEP,

    HI_UNF_PMOC_MODE_BUTT
}HI_UNF_PMOC_MODE_E;


/*---- rtc ----*/
typedef struct hiUNF_RTC_TIME_S
{
    HI_U32 u32year;
    HI_U32 u32month;
    HI_U32 u32date;
    HI_U32 u32hour;
    HI_U32 u32minute;
    HI_U32 u32second;
    HI_U32 u32weekday;
}HI_UNF_RTC_TIME_S, *HI_UNF_RTC_TIME_S_PTR;

/*-----wdg----*/
typedef enum hiUNF_WDG_RESPOND_MODE_E
{
    HI_UNF_WDG_RESPOND_MODE_FIRSTRESET = 0,
    HI_UNF_WDG_RESPOND_MODE_SECONDRESET,

    HI_UNF_WDG_RESPOND_MODE_BUTT
}HI_UNF_WDG_RESPOND_MODE_E;







typedef enum hiUNF_I2C_RATE_E
{
    HI_UNF_I2C_RATE_100k = 0,
    HI_UNF_I2C_RATE_400k,

    HI_UNF_I2C_RATE_BUTT
}HI_UNF_I2C_RATE_E;

/*-----------gpio-------------*/
typedef struct hiUNF_GPIO_Data_S
{
    HI_U32 u32GpioIndex; /* GPIO组号 */
    HI_U32 u32GpioBit;   /* 引脚号   */
    HI_U32 u32GpioValue; /* GPIO值   */
}HI_UNF_GPIO_DATA_S, * HI_UNF_GPIO_DATA_S_PTR;

/*----------- stanby wake up -------------*/
typedef struct hiUNF_PMOC_WKUP_S
{
    HI_U32 u32IrPowerKey;		 	/* 红外遥控 power 键值 */
    HI_U32 u32KeypadPowerKey;   	/* 按键 power   键值 */
    HI_U32 u32WakeUpTime ; 			/* 唤醒定时时间 ,单位为 秒  */
}HI_UNF_PMOC_WKUP_S, *HI_UNF_PMOC_WKUP_S_PTR;

/* led display time when arm is stand by, add by David Huang 2008-05-02 start */
typedef struct hiUNF_PMOC_TIME_S
{
    HI_U32  	u32Hour;
    HI_U32  	u32Minute;
    HI_U32  	u32Second;
}HI_UNF_PMOC_TIME_S,*HI_UNF_PMOC_TIME_S_PTR;

typedef struct hiUNF_PMOC_STANDBY_MODE_S
{
    HI_U32		u32Mode;				/* 0 : no display; 1 : display channel; 2 : display time. */
    HI_U32		u32DispCode;	    /* display code, other than value */
	HI_UNF_PMOC_TIME_S stTimeInfo;		/* system current time info */
}HI_UNF_PMOC_STANDBY_MODE_S, *HI_UNF_PMOC_STANDBY_MODE_S_PTR;
/* led display time when arm is stand by, add by David Huang 2008-05-02 end */


/** 智能卡状态 */
typedef enum hiUNF_SCI_STATUS_E
{
    HI_UNF_SCI_STATUS_UNINIT     = 0,           /*  SCI未初始化 */
    HI_UNF_SCI_STATUS_FIRSTINIT,                /*  SCI初始化过程中*/
    HI_UNF_SCI_STATUS_NOCARD,                   /*  无卡 */
    HI_UNF_SCI_STATUS_INACTIVECARD,             /*  卡未完成激活（卡无效） */
    HI_UNF_SCI_STATUS_WAITATR,                  /*  等待ATR*/
    HI_UNF_SCI_STATUS_READATR,                  /*  正在接收ATR*/
    HI_UNF_SCI_STATUS_READY,                    /*  卡可以使用（卡激活） */
    HI_UNF_SCI_STATUS_RX,                       /*  卡忙（接收数据中） */
    HI_UNF_SCI_STATUS_TX                        /*  卡忙（发送数据中） */
} HI_UNF_SCI_STATUS_E;

/* SCI 协议 */
typedef	enum  hiUNF_SCI_PROTOCOL_E
{
	HI_UNF_SCI_PROTOCOL_T0 , 		/* 7816 T0 协议 */
	HI_UNF_SCI_PROTOCOL_T1 , 		/* 7816 T1 协议 */
	HI_UNF_SCI_PROTOCOL_T14 , 		/* 7816 T14 协议 */
	HI_UNF_SCI_PROTOCOL_BUTT 		
}HI_UNF_SCI_PROTOCOL_E ;


/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/


/*****************************************************************************
* Function		:HI_UNF_SCI_Open
* Description		:打开SCI设备,必须在其他 SCI 函数使用前调用
* Input			u8SCIPort		:   SCI端口号，取值范围为0和1
*				enProtocolType	:	协议类型
*				u32Frequency:要设置的SCI卡频率。对于T0，T1卡，支持频率1MHz~5MHz；对于T14卡，只支持6MHz。单位为khz
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_Open(HI_U8 u8SCIPort, HI_UNF_SCI_PROTOCOL_E enProtocolType , HI_U32 u32Frequency);

HI_S32 HI_UNF_SCI_Close(HI_VOID);
/*****************************************************************************
* Function		:HI_UNF_SCI_ResetCard
* Description		:热复位SCI卡
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
				 bWarmResetValid: 复位方式，取值范围为0和1。
				                 0:warm reset
				                 1:cold reset（推荐用这种方式）
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_ResetCard(HI_U8 u8SCIPort ,HI_BOOL bWarmResetValid);

/*****************************************************************************
* Function		:HI_UNF_SCI_DeactiveCard
* Description		:去激活SCI卡
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_DeactiveCard(HI_U8 u8SCIPort);
/*****************************************************************************
* Function		:HI_UNF_SCI_GetATR
* Description		:获取SCI卡ATR数据
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
				u32AtrBufSize : ATR 数据读取 buffer 长度
* Output		:pu8AtrRcvCount	:	实际获取的ATR数据个数
*				:pu8AtrBuf		:	获取的ATR数据的存储地址
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_GetATR(HI_U8 u8SCIPort, HI_U8 * pu8AtrBuf , HI_U32 u32AtrBufSize , HI_U8 * pu8AtrRcvCount );

/*****************************************************************************
* Function		:HI_UNF_SCI_GetCardStatus
* Description		:获取SCI卡状态
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
* Output			:penSCIStatus	:	SCI卡状态
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_GetCardStatus(HI_U8 u8SCIPort, HI_UNF_SCI_STATUS_E  *penSCIStatus);

/*****************************************************************************
* Function		:HI_UNF_SCI_Send
* Description		:向SCI卡发送数据
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
*				pSCISendBuf:发送数据的存储地址
*				u32SendLength:发送数据的个数,单位为 BYTE
				pu32ActualLength : 实际发送数据个数,单位为 BYTE
		      	u32TimeOut : 等待超时值, 单位是毫秒, 0 - 不阻塞, 0xFFFFFFFF-永久阻塞
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_Send(HI_U8 u8SCIPort, HI_U8 * pSCISendBuf, HI_U32 u32SendLength , HI_U32 * pu32ActualLength ,HI_U32  u32TimeOut);


/*****************************************************************************
* Function		:HI_UNF_SCI_Receive
* Description		:从SCI卡接收数据
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
*				:pSCIReceiveBuf:接收数据的存储地址
*				:u32ReceiveLength:期望接收数据的个数,,单位为 BYTE
*				:pu32ActualLength:实际接收数据个数,单位为 BYTE
		      	u32TimeOut : 等待超时值, 单位是毫秒, 0 - 不阻塞, 0xFFFFFFFF-永久阻塞

* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_Receive (HI_U8 u8SCIPort, HI_U8 * pSCIReceiveBuf, HI_U32 u32ReceiveLength,
						 HI_U32 * pu32ActualLength ,  HI_U32  u32TimeOut );
/*****************************************************************************
* Function		:HI_UNF_SCI_ConfigVccEn
* Description		:配置VCCEN信号线的有效电平
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
*				:bHighVoltValid:信号线的有效电平。HI_FALSE：低电平有效，HI_TRUE：高电平有效
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_ConfigVccEn(HI_U8 u8SCIPort, HI_U32 u32Level);

/*****************************************************************************
* Function		:HI_UNF_SCI_ConfigDetect
* Description		:配置DETECT信号线的有效电平
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
*				:bHighVoltValid:0：信号线的有效电平。HI_FALSE：低电平有效，HI_TRUE：高电平有效
* Output			:无
* Return			:成功 / 错误码
*****************************************************************************/
HI_S32 HI_UNF_SCI_ConfigDetect(HI_U8 u8SCIPort, HI_U32 u32Level);

/*****************************************************************************
* Function		:HI_UNF_SCI_ConfigClkMode
* Description		:设置时钟线的模式
* Input			:u8SCIPort: SCI端口号，取值范围为0和1
*				:enClkMode:时钟线的模式
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_ConfigClkMode(HI_U8 u8SCIPort, HI_UNF_SCI_CLK_MODE_E enClkMode);

/*****************************************************************************
* Function		:HI_UNF_SCI_SwitchCard
* Description		:切换卡槽
* Input			u8SCIPort		:   SCI端口号，取值范围为0和1
*				enProtocolType	:	协议类型
*				u32Frequency:要设置的SCI卡频率。对于T0，T1卡，支持频率1MHz~5MHz；对于T14卡，只支持6MHz。单位为khz
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_SCI_SwitchCard(HI_U8 u8SCIPort, HI_UNF_SCI_PROTOCOL_E enProtocolType , HI_U32 u32Frequency);


/*---- e2prom ----*/
HI_S32 HI_UNF_E2PROM_Open(HI_VOID);
HI_S32 HI_UNF_E2PROM_Close(HI_VOID);
HI_S32 HI_UNF_E2PROM_Read (HI_U32 u32Addr, HI_U8 * pu8Buf, HI_U32 u32Length);
HI_S32 HI_UNF_E2PROM_Write (HI_U32 u32Addr, HI_U8 * pu8Buf, HI_U32 u32Length);
HI_S32 HI_UNF_E2PROM_SetChipType(HI_UNF_E2PROM_CHIP_E enChip);
HI_S32 HI_UNF_E2PROM_SetAddr(HI_U32 u32E2promAddr);
HI_S32 HI_UNF_E2PROM_SetI2cNum(HI_U32 u32I2cNum);


/*---- i2c ----*/
HI_S32 HI_UNF_I2C_Open (HI_VOID);
HI_S32 HI_UNF_I2C_Close(HI_VOID);
HI_S32 HI_UNF_I2C_Read(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr, 
                       HI_U32 u32RegAddrCount, HI_U8 *pu8Buf, HI_U32 u32Length);
HI_S32 HI_UNF_I2C_Write(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr, 
                       HI_U32  u32RegAddrCount, HI_U8 * pu8Buf, HI_U32 u32Length);
HI_S32 HI_UNF_I2C_SetRate(HI_U32 u32I2cNum, HI_UNF_I2C_RATE_E enI2cRate);


/*---- keyled ----*/
HI_S32 HI_UNF_KEY_Open(HI_VOID);
HI_S32 HI_UNF_KEY_Close(HI_VOID);
HI_S32 HI_UNF_LED_Open(HI_VOID);
HI_S32 HI_UNF_LED_Close(HI_VOID);
HI_S32 HI_UNF_KEY_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId);
HI_S32 HI_UNF_LED_Display(HI_U32 u32Value);
HI_S32 HI_UNF_LED_DisplayTime(HI_UNF_KEYLED_TIME_S stKeyLedTime);
HI_S32 HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_E enPin);

HI_S32 HI_UNF_KEY_Reset(HI_VOID);
HI_S32 HI_UNF_KEY_SetBlockTime(HI_U32 u32Blocktime);
HI_S32 HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_E enLevel);
HI_S32 HI_UNF_LED_Disable(HI_VOID);
HI_S32 HI_UNF_LED_Enable(HI_U32 u32Manner);
HI_S32 HI_UNF_KEY_Disable(HI_VOID);
HI_S32 HI_UNF_KEY_Enable(HI_U32 u32Manner);
HI_S32 HI_UNF_KEY_RepKeyTimeoutVal(HI_U32 u32TimeoutValArg);
HI_S32 HI_UNF_KEY_IsRepKey(HI_U32 u32IsRepKeyArg);
HI_S32 HI_UNF_KEY_IsKeyUp(HI_U32 u32IsKeyUpArg);


/*---- pm ----*/
HI_S32 HI_UNF_PMOC_Open(HI_VOID);
HI_S32 HI_UNF_PMOC_Close(HI_VOID);
HI_S32 HI_UNF_PMOC_SwitchSystemMode(HI_UNF_PMOC_MODE_E enSystemMode, HI_U32 * pu32WakeUpStatus);
HI_S32 HI_UNF_PMOC_ReadSystemMode(HI_UNF_PMOC_MODE_E * penSystemMode);
HI_S32 HI_UNF_PMOC_SetWakeUpAttr(HI_UNF_PMOC_WKUP_S_PTR pstAttr);
HI_S32 HI_UNF_PMOC_SetStandbyDispMode(HI_UNF_PMOC_STANDBY_MODE_S_PTR pstStandbyMode);

/*---- ir ----*/
HI_S32 HI_UNF_IR_Open(HI_VOID);
HI_S32 HI_UNF_IR_Close(HI_VOID);
HI_S32 HI_UNF_IR_Enable ( HI_U32 u32Manner);
HI_S32 HI_UNF_IR_Disable(HI_VOID);
HI_S32 HI_UNF_IR_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId);
HI_S32 HI_UNF_IR_IsKeyUp(HI_U32 u32IsKeyUpArg);
HI_S32 HI_UNF_IR_IsRepKey(HI_U32 u32IsRepKeyArg);
HI_S32 HI_UNF_IR_RepKeyTimeoutVal(HI_U32 u32TimeoutValArg);
HI_S32 HI_UNF_IR_CodeType(HI_UNF_IR_CODE_E enIRCode);
HI_S32 HI_UNF_IR_SelectUserCode(HI_U32 u32UserCode);
HI_S32 HI_UNF_IR_SetBlockTime(HI_U32 u32BlockTime);
HI_S32 HI_UNF_IR_Reset(HI_VOID);

/*---- rtc ----*/
HI_S32 HI_UNF_RTC_Open(HI_VOID);
HI_S32 HI_UNF_RTC_Close(HI_VOID);
HI_S32 HI_UNF_RTC_SetDateTime(HI_UNF_RTC_TIME_S_PTR pstRtcTime);
HI_S32 HI_UNF_RTC_GetDateTime(HI_UNF_RTC_TIME_S_PTR pstRtcTime);
HI_S32 HI_UNF_RTC_AlarmEnable(HI_VOID);
HI_S32 HI_UNF_RTC_AlarmDisable(HI_VOID);
HI_S32 HI_UNF_RTC_SetAlarm(HI_UNF_RTC_TIME_S_PTR pstRtcTime);
HI_S32 HI_UNF_RTC_GetAlarm(HI_UNF_RTC_TIME_S_PTR pstRtcTime);


/*---- wdg ----*/
HI_S32 HI_UNF_WDG_Open(HI_VOID);
HI_S32 HI_UNF_WDG_Close(HI_VOID);
HI_S32 HI_UNF_WDG_Enable(HI_VOID);
HI_S32 HI_UNF_WDG_Disable(HI_VOID);
HI_S32 HI_UNF_WDG_SetTimeout(HI_U32 u32Value);
HI_S32 HI_UNF_WDG_ClearWatchDog (HI_VOID);
HI_S32 HI_UNF_WDG_RespondMode(HI_UNF_WDG_RESPOND_MODE_E enRespondMode);
HI_S32 HI_UNF_WDG_Reset(HI_VOID);

/*---- gpio ----*/
HI_S32 HI_UNF_GPIO_Open(HI_VOID);
HI_S32 HI_UNF_GPIO_Close(HI_VOID);
HI_S32 HI_UNF_GPIO_Read_Bit(HI_U32  u32BitNo , HI_U32		*pu32Val );
HI_S32 HI_UNF_GPIO_Write_Bit(HI_U32  u32BitNo , HI_U32		pu32Val );
HI_S32 HI_UNF_GPIO_DirSet_Bit(HI_U32  u32BitNo , HI_BOOL	bInput);
HI_S32 HI_UNF_GPIO_DirGet_Bit(HI_U32  u32BitNo , HI_BOOL	*pbInput);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ECS_TYPE_H__ */

