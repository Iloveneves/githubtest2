/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
File Name     	:hi_unf_tuner.h
Version       	:Initial Draft
Author        	:Hisilicon multimedia Software group 
Created       	:2008/10/23
Last Modified 	:
Description   	: 
Function List 	:
History       	:
 ******************************************************************************/
#ifndef __HI_UNF_API_TUNER_H
#define __HI_UNF_API_TUNER_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"

#include <linux/ioctl.h>

/*QAM调制方式*/
typedef enum hiUNF_QAM_TYPE_E
{
	HI_UNF_MOD_TYPE_DEFAULT,			  /*默认的QAM类型*/
	HI_UNF_MOD_TYPE_QAM_16 = 0x100,     /*16QAM对应的枚举值*/
	HI_UNF_MOD_TYPE_QAM_32,                 /*32QAM对应的枚举值*/
	HI_UNF_MOD_TYPE_QAM_64,                 /*64QAM对应的枚举值*/
	HI_UNF_MOD_TYPE_QAM_128,               /*128QAM对应的枚举值*/
	HI_UNF_MOD_TYPE_QAM_256,               /*256QAM对应的枚举值*/
	HI_UNF_MOD_TYPE_QAM_512,               /*512QAM对应的枚举值*/

	HI_UNF_MOD_TYPE_BPSK = 0x200,          /*BPSK QAM对应的枚举值，HI3110E不支持*/
	HI_UNF_MOD_TYPE_QPSK = 0x300,          /*QPSK QAM对应的枚举值，HI3110E不支持*/

	HI_UNF_MOD_TYPE_BUTT                    	/*非法的QAM类型枚举值*/
}HI_UNF_MODULATION_TYPE_E;

/*TUNER锁频状态*/
typedef enum  hiUNF_TUNER_LOCK_STATUS_E
{
	HI_UNF_TUNER_SIGNAL_DROPPED = 0,        /*信号未锁定*/
	HI_UNF_TUNER_SIGNAL_LOCKED,             	/*信号已锁定*/
	HI_UNF_TUNER_SIGNAL_BUTT                	/*非法边界值*/
}HI_UNF_TUNER_LOCK_STATUS_E;

/*TUNER输出模式 */
typedef enum hiUNF_TUNER_OUTPUT_MODE_E
{
    HI_UNF_TUNER_OUTPUT_MODE_DEFAULT ,			/*默认模式*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A,	/*并行模式A*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B,	/*并行模式B*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_C,	/*并行模式C*/
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL,				/*串行模式*/
    HI_UNF_TUNER_OUTPUT_MODE_BUTT				/*非法值*/
}HI_UNF_TUNER_OUPUT_MODE_E;

/*TUNER信号类型*/
typedef	enum	hiTUNER_SIG_TYPE_E
{
	HI_UNF_TUNER_SIG_TYPE_CAB = 0  ,		/*CABLE信号*/
	HI_UNF_TUNER_SIG_TYPE_TER  ,			/*地面信号*/
	HI_UNF_TUNER_SIG_TYPE_SAT ,			/*卫星信号*/
	HI_UNF_TUNER_SIG_TYPE_BUTT			/*非法值*/
}HI_UNF_TUNER_SIG_TYPE_E;

/*TUNER使用的I2C通道*/	
typedef enum hiUNF_TUNER_I2cChannel
{
	HI_UNF_I2C_CHANNEL_0 = 0, 				/*I2C通道0*/
	HI_UNF_I2C_CHANNEL_1, 				/*I2C通道1*/
	HI_UNF_I2C_CHANNEL_2,					/*I2C通道2*/
	HI_UNF_I2C_CHANNEL_MAX				/*非法边界值*/
}HI_UNF_TUNER_I2cChannel_E;

/*TUNER设备类型*/
typedef	enum	hiUNF_TUNER_DEV_TYPE_E
{
    HI_UNF_TUNER_DEV_TYPE_CU1216 = 0X100,		/*CU1216*/
    HI_UNF_TUNER_DEV_TYPE_ALPS_TDE_3 ,		/*ALPS_TDE_3*/ 
    HI_UNF_TUNER_DEV_TYPE_ALPS_TDE_5 , 		/*ALPS_TDE_5*/
    HI_UNF_TUNER_DEV_TYPE_TMX70600,			/*TMX70600*/
    HI_UNF_TUNER_DEV_TYPE_XG_3BL,				/*XG_3BL*/
    HI_UNF_TUNER_DEV_TYPE_CD1616,				/*CD1616*/
    HI_UNF_TUNER_DEV_TYPE_ALPS, 				/*ALPS*/
    HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE,			/*ALPS_TDAE*/
    HI_UNF_TUNER_DEV_TYPE_1026, 				/*1026*/
    HI_UNF_TUNER_DEV_TYPE_0297, 				/*0297*/
    HI_UNF_TUNER_DEV_TYPE_TDCC, 				/*TDCC*/
    HI_UNF_TUNER_DEV_TYPE_TDA18251,			/*TDA18251*/ 
    HI_UNF_TUNER_DEV_TYPE_CD1616_LF_GIH_4,			/*CD1616_LF_GIH_4*/ 
    HI_UNF_TUNER_DEV_TYPE_MXL203,                /* MXL203 */
    HI_UNF_TUNER_DEV_TYPE_DTT5_E20 = 0x200 , 	/*DTT5_E20*/
	
    HI_UNF_TUNER_DEV_TYPE_BUTT,				/*非法边界值*/
}HI_UNF_TUNER_DEV_TYPE_E ;

/*QAM设备类型*/
typedef	enum	hiUNF_DEMOD_DEV_TYPE_E
{
    HI_UNF_DEMOD_DEV_TYPE_NONE,				
    HI_UNF_DEMOD_DEV_TYPE_3130I= 0x100,		/*内部QAM*/
    HI_UNF_DEMOD_DEV_TYPE_3130E,				/*外部Hi3130芯片*/

    HI_UNF_DEMOD_DEV_TYPE_PHILIP = 0x200,		/*PHILIP*/
    HI_UNF_DEMOD_DEV_TYPE_ST,					/*ST*/
    HI_UNF_DEMOD_DEV_TYPE_GX,					/*GX*/
	
    HI_UNF_DEMOD_DEV_TYPE_BUTT,				/*非法边界值*/
}HI_UNF_DEMOD_DEV_TYPE_E ;

/*定义CABLE传输信号*/
typedef	struct	hiUNF_CAB_CONNECT_PARA_S
{
	HI_U32				u32Freq ;				/*频率，单位：kHz*/
	HI_U32				u32SymbolRate ;			/*符号率，单位bps */
	HI_UNF_MODULATION_TYPE_E	enModType ;		/*QAM调制方式*/
	HI_BOOL				bReverse ;				/*频谱倒置处理方式*/
}HI_UNF_CAB_CONNECT_PARA_S ;

/*HI3110E 未使用*/
typedef enum hiUNF_TUNER_FE_GUARD_INTV_E
{
	HI_UNF_TUNER_FE_GUARD_INTV_DEFALUT = 0 ,
	HI_UNF_TUNER_FE_GUARD_INTV_32		,
	HI_UNF_TUNER_FE_GUARD_INTV_16		,
	HI_UNF_TUNER_FE_GUARD_INTV_8		,
	HI_UNF_TUNER_FE_GUARD_INTV_4		,
	HI_UNF_TUNER_FE_GUARD_INTV_BUTT		,
}HI_UNF_TUNER_FE_GUARD_INTV_E;

/*HI3110E 未使用*/
typedef enum hiUNF_TUNER_FE_DEPUNC_RATE_E
{
	HI_UNF_TUNER_FE_DEPUNC_RATE_DEFAULT = 0 ,
	HI_UNF_TUNER_FE_DEPUNC_RATE_12 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_23 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_34 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_45 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_56 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_67 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_78 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_89 		,
	HI_UNF_TUNER_FE_DEPUNC_RATE_BUTT	,
}HI_UNF_TUNER_FE_DEPUNC_RATE_E ;

/*HI3110E 未使用*/
typedef enum hiUNF_TUNER_FE_FFT_E
{ 
	HI_UNF_TUNER_FE_FFT_DEFAULT = 0,
	HI_UNF_TUNER_FE_FFT_2K ,
	HI_UNF_TUNER_FE_FFT_8K ,
	HI_UNF_TUNER_FE_FFT_BUTT ,
}HI_UNF_TUNER_FE_FFT_E;

/*HI3110E 未使用*/
typedef enum hiUNF_TUNER_FE_HIERARCHY_E
{
	HI_UNF_TUNER_FE_HIERARCHY_DEFAULT = 0 ,
	HI_UNF_TUNER_FE_HIERARCHY_NO,
	HI_UNF_TUNER_FE_HIERARCHY_ALHPA1,
	HI_UNF_TUNER_FE_HIERARCHY_ALHPA2,
	HI_UNF_TUNER_FE_HIERARCHY_ALHPA4,
	HI_UNF_TUNER_FE_HIERARCHY_BUTT,
}HI_UNF_TUNER_FE_HIERARCHY_E ;

/*HI3110E 未使用*/
typedef	struct	hiUNF_TER_CONNECT_PARA_S
{
	HI_U32			u32Freq 	;
	HI_U32			u32BandWidth 	;
	HI_UNF_MODULATION_TYPE_E enModType	;	
}HI_UNF_TER_CONNECT_PARA_S ;

/*HI3110E 未使用*/
typedef	struct	hiUNF_TUNER_TER_ATTR_S
{
	HI_UNF_TUNER_FE_DEPUNC_RATE_E	enHpCode ;
	HI_UNF_TUNER_FE_DEPUNC_RATE_E	enLpCode ;
	HI_UNF_TUNER_FE_GUARD_INTV_E	enGuardIntv ;
	HI_UNF_TUNER_FE_FFT_E		enFftType ;
	HI_UNF_TUNER_FE_HIERARCHY_E	enHierType ;

}HI_UNF_TUNER_TER_ATTR_S;

/*TUNER属性*/
typedef	struct 	hiTUNER_ATTR_S
{
	HI_UNF_TUNER_SIG_TYPE_E		enSigType ;			/*信号类型*/
	HI_UNF_TUNER_DEV_TYPE_E		enTunerDevType ;	/*TUNER类型*/
	HI_UNF_DEMOD_DEV_TYPE_E	enDemodDevType ;	/*QAM类型*/
	HI_UNF_TUNER_OUPUT_MODE_E	enOutputMode ;		/*TS流输出模式*/
       HI_UNF_TUNER_I2cChannel_E	enI2cChannel;		/*TUNER使用的I2C通道*/

	union{
		HI_UNF_TUNER_TER_ATTR_S	stTer ;			/*地面信号属性，Hi3110E不支持*/
	}unTunerAttr ;
}HI_UNF_TUNER_ATTR_S ;

/*TUNER锁频参数*/
typedef	struct	hiUNF_TUNER_CONNECT_PARA_S
{
	HI_UNF_TUNER_SIG_TYPE_E	enSigType ;			/*信号类型*/	

	union
	{
		HI_UNF_CAB_CONNECT_PARA_S	stCab ;		/*CABLE传输信号*/
		HI_UNF_TER_CONNECT_PARA_S	stTer ;		/*地面传输信号*/
	}unConnectPara;
}HI_UNF_TUNER_CONNECT_PARA_S ;

/*TUNER锁频状态和锁频参数*/
typedef	struct	hiUNF_TUNER_STATUS_S
{
	HI_UNF_TUNER_LOCK_STATUS_E	enLockStatus ;			/*锁频状态*/
	HI_UNF_TUNER_CONNECT_PARA_S	stConnectPara;		/*实际锁频参数*/
}HI_UNF_TUNER_STATUS_S ;

/*****************************************************************************
* Function		:HI_UNF_TUNER_Init
* Description		:初始化TUNER设备
* Input			:无
* Output			:无
* Return			:成功 / 错误码
* Others			:支持重复初始化TUNER设备，此时直接返回成功
*****************************************************************************/     
HI_S32 HI_UNF_TUNER_Init(void);

/*****************************************************************************
* Function		:HI_UNF_TUNER_Destroy
* Description		:去初始化TUNER设备
* Input			:无
* Output			:无
* Return			:成功 / 错误码
* Others			:支持重复去初始化TUNER设备，此时直接返回成功
*****************************************************************************/
HI_S32 HI_UNF_TUNER_Destroy(void);

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetDeftAttr
* Description		:获取TUNER的默认属性
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:pstTunerAttr: 返回当前TUNER的默认属性
* Return			:成功 / 错误码
* Others			:此接口在TUNER设备初始化后才能使用
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetDeftAttr(HI_U32	u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/*****************************************************************************
* Function		:HI_UNF_TUNER_SetAttr
* Description		:设置TUNER当前属性
* Input			:u32tunerId: TUNER端口号，取值为0或1
*                		:pstTunerAttr: TUNER的属性
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/  
HI_S32 HI_UNF_TUNER_SetAttr(HI_U32	u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetAttr
* Description		:获取TUNER的属性
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:pstTunerAttr: 返回当前TUNER的属性
* Return			:成功 / 错误码
* Others			:此接口在TUNER设备初始化后才能使用
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetAttr(HI_U32	u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/*****************************************************************************
* Function		:HI_UNF_TUNER_Open
* Description		:打开TUNER设备
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:无
* Return			:成功 / 错误码
* Others			:支持重复打开TUNER设备。此时，直接返回成功
*****************************************************************************/
HI_S32 HI_UNF_TUNER_Open (HI_U32	u32tunerId);

/*****************************************************************************
* Function		:HI_UNF_TUNER_Close
* Description		:关闭TUNER设备
* Input			:u32tunerId:TUNER端口号，取值为0或1
* Output			:无
* Return			:成功 / 错误码
* Others			:支持重复关闭TUNER设备。此时，直接返回成功
*****************************************************************************/  
HI_S32 HI_UNF_TUNER_Close(HI_U32	u32tunerId);

/*****************************************************************************
* Function		:HI_UNF_TUNER_Connect
* Description		:TUNER锁频
* Input			:u32tunerId:TUNER端口号，取值为0或1
*                	 	:pstConnectPara:TUNER的频点信息
* Output			:无
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_TUNER_Connect(HI_U32  u32tunerId , HI_UNF_TUNER_CONNECT_PARA_S  *pstConnectPara,HI_U32 u32TimeOut);

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetStatus
* Description		:获取TUNER锁频状态和锁频参数
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:pstTunerStatus: 返回当前TUNER的锁频状态和锁频参数
				 此参数只在函数返回成功的情况下才有意义
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetStatus(HI_U32	u32tunerId , HI_UNF_TUNER_STATUS_S  *pstTunerStatus);

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetBER
* Description		:获取当前TUNER 的误码率，用科学计数法表示
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:pu32BER: 指向当前TUNER 误码率的指针。该指针指向一个包含三个元素的数组，三个元素含义如下:
				 pu32BER[0]:误码率底数的整数部分
				 pu32BER[1]:误码率底数的小数部分乘以1000
				 pu32BER[2]:误码率指数部分取绝对值
				 例如:误码率为2.156E-7，那么三个元素的取值分别为
				 2、156和7
				 这三个元素只在函数返回成功的情况下才有意义，
				 否则应用层可根据实际情况自行设置一个较大值(比如0.5，即三个元素分别为5、0、1)。
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetBER(HI_U32   u32tunerId , HI_U32 *pu32BER); 

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetSNR
* Description		:获取当前TUNER 的信噪比
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:pu32SNR:指向当前TUNER信噪比的指针
				:*pu32SNR取值范围为22 ~ 51
				 此参数只在函数返回成功的情况下才有意义，
				 否则，应用层可将此值设置为最小值。
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetSNR(HI_U32	u32tunerId , HI_U32 *pu32SNR );				/* range : 0-255  */

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetSignalStrength
* Description		:获取当前TUNER的信号强度
* Input			:u32tunerId: TUNER端口号，取值为0或1
* Output			:pu32SignalStrength:指向当前TUNER信号强度的指针
				:*pu32SignalStrength的取值范围为20~60
			  	 此参数只在函数返回成功的情况下才有意义，
			  	 否则，应用层可将此值设置为最小值。
* Return			:成功 / 错误码
* Others			:无
*****************************************************************************/  
HI_S32 HI_UNF_TUNER_GetSignalStrength(HI_U32   u32tunerId , HI_U32 *pu32SignalStrength );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_UNF_API_TUNER_H*/
