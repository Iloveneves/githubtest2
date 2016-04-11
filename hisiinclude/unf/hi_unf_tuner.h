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

/*QAM���Ʒ�ʽ*/
typedef enum hiUNF_QAM_TYPE_E
{
	HI_UNF_MOD_TYPE_DEFAULT,			  /*Ĭ�ϵ�QAM����*/
	HI_UNF_MOD_TYPE_QAM_16 = 0x100,     /*16QAM��Ӧ��ö��ֵ*/
	HI_UNF_MOD_TYPE_QAM_32,                 /*32QAM��Ӧ��ö��ֵ*/
	HI_UNF_MOD_TYPE_QAM_64,                 /*64QAM��Ӧ��ö��ֵ*/
	HI_UNF_MOD_TYPE_QAM_128,               /*128QAM��Ӧ��ö��ֵ*/
	HI_UNF_MOD_TYPE_QAM_256,               /*256QAM��Ӧ��ö��ֵ*/
	HI_UNF_MOD_TYPE_QAM_512,               /*512QAM��Ӧ��ö��ֵ*/

	HI_UNF_MOD_TYPE_BPSK = 0x200,          /*BPSK QAM��Ӧ��ö��ֵ��HI3110E��֧��*/
	HI_UNF_MOD_TYPE_QPSK = 0x300,          /*QPSK QAM��Ӧ��ö��ֵ��HI3110E��֧��*/

	HI_UNF_MOD_TYPE_BUTT                    	/*�Ƿ���QAM����ö��ֵ*/
}HI_UNF_MODULATION_TYPE_E;

/*TUNER��Ƶ״̬*/
typedef enum  hiUNF_TUNER_LOCK_STATUS_E
{
	HI_UNF_TUNER_SIGNAL_DROPPED = 0,        /*�ź�δ����*/
	HI_UNF_TUNER_SIGNAL_LOCKED,             	/*�ź�������*/
	HI_UNF_TUNER_SIGNAL_BUTT                	/*�Ƿ��߽�ֵ*/
}HI_UNF_TUNER_LOCK_STATUS_E;

/*TUNER���ģʽ */
typedef enum hiUNF_TUNER_OUTPUT_MODE_E
{
    HI_UNF_TUNER_OUTPUT_MODE_DEFAULT ,			/*Ĭ��ģʽ*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A,	/*����ģʽA*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B,	/*����ģʽB*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_C,	/*����ģʽC*/
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL,				/*����ģʽ*/
    HI_UNF_TUNER_OUTPUT_MODE_BUTT				/*�Ƿ�ֵ*/
}HI_UNF_TUNER_OUPUT_MODE_E;

/*TUNER�ź�����*/
typedef	enum	hiTUNER_SIG_TYPE_E
{
	HI_UNF_TUNER_SIG_TYPE_CAB = 0  ,		/*CABLE�ź�*/
	HI_UNF_TUNER_SIG_TYPE_TER  ,			/*�����ź�*/
	HI_UNF_TUNER_SIG_TYPE_SAT ,			/*�����ź�*/
	HI_UNF_TUNER_SIG_TYPE_BUTT			/*�Ƿ�ֵ*/
}HI_UNF_TUNER_SIG_TYPE_E;

/*TUNERʹ�õ�I2Cͨ��*/	
typedef enum hiUNF_TUNER_I2cChannel
{
	HI_UNF_I2C_CHANNEL_0 = 0, 				/*I2Cͨ��0*/
	HI_UNF_I2C_CHANNEL_1, 				/*I2Cͨ��1*/
	HI_UNF_I2C_CHANNEL_2,					/*I2Cͨ��2*/
	HI_UNF_I2C_CHANNEL_MAX				/*�Ƿ��߽�ֵ*/
}HI_UNF_TUNER_I2cChannel_E;

/*TUNER�豸����*/
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
	
    HI_UNF_TUNER_DEV_TYPE_BUTT,				/*�Ƿ��߽�ֵ*/
}HI_UNF_TUNER_DEV_TYPE_E ;

/*QAM�豸����*/
typedef	enum	hiUNF_DEMOD_DEV_TYPE_E
{
    HI_UNF_DEMOD_DEV_TYPE_NONE,				
    HI_UNF_DEMOD_DEV_TYPE_3130I= 0x100,		/*�ڲ�QAM*/
    HI_UNF_DEMOD_DEV_TYPE_3130E,				/*�ⲿHi3130оƬ*/

    HI_UNF_DEMOD_DEV_TYPE_PHILIP = 0x200,		/*PHILIP*/
    HI_UNF_DEMOD_DEV_TYPE_ST,					/*ST*/
    HI_UNF_DEMOD_DEV_TYPE_GX,					/*GX*/
	
    HI_UNF_DEMOD_DEV_TYPE_BUTT,				/*�Ƿ��߽�ֵ*/
}HI_UNF_DEMOD_DEV_TYPE_E ;

/*����CABLE�����ź�*/
typedef	struct	hiUNF_CAB_CONNECT_PARA_S
{
	HI_U32				u32Freq ;				/*Ƶ�ʣ���λ��kHz*/
	HI_U32				u32SymbolRate ;			/*�����ʣ���λbps */
	HI_UNF_MODULATION_TYPE_E	enModType ;		/*QAM���Ʒ�ʽ*/
	HI_BOOL				bReverse ;				/*Ƶ�׵��ô���ʽ*/
}HI_UNF_CAB_CONNECT_PARA_S ;

/*HI3110E δʹ��*/
typedef enum hiUNF_TUNER_FE_GUARD_INTV_E
{
	HI_UNF_TUNER_FE_GUARD_INTV_DEFALUT = 0 ,
	HI_UNF_TUNER_FE_GUARD_INTV_32		,
	HI_UNF_TUNER_FE_GUARD_INTV_16		,
	HI_UNF_TUNER_FE_GUARD_INTV_8		,
	HI_UNF_TUNER_FE_GUARD_INTV_4		,
	HI_UNF_TUNER_FE_GUARD_INTV_BUTT		,
}HI_UNF_TUNER_FE_GUARD_INTV_E;

/*HI3110E δʹ��*/
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

/*HI3110E δʹ��*/
typedef enum hiUNF_TUNER_FE_FFT_E
{ 
	HI_UNF_TUNER_FE_FFT_DEFAULT = 0,
	HI_UNF_TUNER_FE_FFT_2K ,
	HI_UNF_TUNER_FE_FFT_8K ,
	HI_UNF_TUNER_FE_FFT_BUTT ,
}HI_UNF_TUNER_FE_FFT_E;

/*HI3110E δʹ��*/
typedef enum hiUNF_TUNER_FE_HIERARCHY_E
{
	HI_UNF_TUNER_FE_HIERARCHY_DEFAULT = 0 ,
	HI_UNF_TUNER_FE_HIERARCHY_NO,
	HI_UNF_TUNER_FE_HIERARCHY_ALHPA1,
	HI_UNF_TUNER_FE_HIERARCHY_ALHPA2,
	HI_UNF_TUNER_FE_HIERARCHY_ALHPA4,
	HI_UNF_TUNER_FE_HIERARCHY_BUTT,
}HI_UNF_TUNER_FE_HIERARCHY_E ;

/*HI3110E δʹ��*/
typedef	struct	hiUNF_TER_CONNECT_PARA_S
{
	HI_U32			u32Freq 	;
	HI_U32			u32BandWidth 	;
	HI_UNF_MODULATION_TYPE_E enModType	;	
}HI_UNF_TER_CONNECT_PARA_S ;

/*HI3110E δʹ��*/
typedef	struct	hiUNF_TUNER_TER_ATTR_S
{
	HI_UNF_TUNER_FE_DEPUNC_RATE_E	enHpCode ;
	HI_UNF_TUNER_FE_DEPUNC_RATE_E	enLpCode ;
	HI_UNF_TUNER_FE_GUARD_INTV_E	enGuardIntv ;
	HI_UNF_TUNER_FE_FFT_E		enFftType ;
	HI_UNF_TUNER_FE_HIERARCHY_E	enHierType ;

}HI_UNF_TUNER_TER_ATTR_S;

/*TUNER����*/
typedef	struct 	hiTUNER_ATTR_S
{
	HI_UNF_TUNER_SIG_TYPE_E		enSigType ;			/*�ź�����*/
	HI_UNF_TUNER_DEV_TYPE_E		enTunerDevType ;	/*TUNER����*/
	HI_UNF_DEMOD_DEV_TYPE_E	enDemodDevType ;	/*QAM����*/
	HI_UNF_TUNER_OUPUT_MODE_E	enOutputMode ;		/*TS�����ģʽ*/
       HI_UNF_TUNER_I2cChannel_E	enI2cChannel;		/*TUNERʹ�õ�I2Cͨ��*/

	union{
		HI_UNF_TUNER_TER_ATTR_S	stTer ;			/*�����ź����ԣ�Hi3110E��֧��*/
	}unTunerAttr ;
}HI_UNF_TUNER_ATTR_S ;

/*TUNER��Ƶ����*/
typedef	struct	hiUNF_TUNER_CONNECT_PARA_S
{
	HI_UNF_TUNER_SIG_TYPE_E	enSigType ;			/*�ź�����*/	

	union
	{
		HI_UNF_CAB_CONNECT_PARA_S	stCab ;		/*CABLE�����ź�*/
		HI_UNF_TER_CONNECT_PARA_S	stTer ;		/*���洫���ź�*/
	}unConnectPara;
}HI_UNF_TUNER_CONNECT_PARA_S ;

/*TUNER��Ƶ״̬����Ƶ����*/
typedef	struct	hiUNF_TUNER_STATUS_S
{
	HI_UNF_TUNER_LOCK_STATUS_E	enLockStatus ;			/*��Ƶ״̬*/
	HI_UNF_TUNER_CONNECT_PARA_S	stConnectPara;		/*ʵ����Ƶ����*/
}HI_UNF_TUNER_STATUS_S ;

/*****************************************************************************
* Function		:HI_UNF_TUNER_Init
* Description		:��ʼ��TUNER�豸
* Input			:��
* Output			:��
* Return			:�ɹ� / ������
* Others			:֧���ظ���ʼ��TUNER�豸����ʱֱ�ӷ��سɹ�
*****************************************************************************/     
HI_S32 HI_UNF_TUNER_Init(void);

/*****************************************************************************
* Function		:HI_UNF_TUNER_Destroy
* Description		:ȥ��ʼ��TUNER�豸
* Input			:��
* Output			:��
* Return			:�ɹ� / ������
* Others			:֧���ظ�ȥ��ʼ��TUNER�豸����ʱֱ�ӷ��سɹ�
*****************************************************************************/
HI_S32 HI_UNF_TUNER_Destroy(void);

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetDeftAttr
* Description		:��ȡTUNER��Ĭ������
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:pstTunerAttr: ���ص�ǰTUNER��Ĭ������
* Return			:�ɹ� / ������
* Others			:�˽ӿ���TUNER�豸��ʼ�������ʹ��
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetDeftAttr(HI_U32	u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/*****************************************************************************
* Function		:HI_UNF_TUNER_SetAttr
* Description		:����TUNER��ǰ����
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
*                		:pstTunerAttr: TUNER������
* Output			:��
* Return			:�ɹ� / ������
* Others			:��
*****************************************************************************/  
HI_S32 HI_UNF_TUNER_SetAttr(HI_U32	u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetAttr
* Description		:��ȡTUNER������
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:pstTunerAttr: ���ص�ǰTUNER������
* Return			:�ɹ� / ������
* Others			:�˽ӿ���TUNER�豸��ʼ�������ʹ��
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetAttr(HI_U32	u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/*****************************************************************************
* Function		:HI_UNF_TUNER_Open
* Description		:��TUNER�豸
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:��
* Return			:�ɹ� / ������
* Others			:֧���ظ���TUNER�豸����ʱ��ֱ�ӷ��سɹ�
*****************************************************************************/
HI_S32 HI_UNF_TUNER_Open (HI_U32	u32tunerId);

/*****************************************************************************
* Function		:HI_UNF_TUNER_Close
* Description		:�ر�TUNER�豸
* Input			:u32tunerId:TUNER�˿ںţ�ȡֵΪ0��1
* Output			:��
* Return			:�ɹ� / ������
* Others			:֧���ظ��ر�TUNER�豸����ʱ��ֱ�ӷ��سɹ�
*****************************************************************************/  
HI_S32 HI_UNF_TUNER_Close(HI_U32	u32tunerId);

/*****************************************************************************
* Function		:HI_UNF_TUNER_Connect
* Description		:TUNER��Ƶ
* Input			:u32tunerId:TUNER�˿ںţ�ȡֵΪ0��1
*                	 	:pstConnectPara:TUNER��Ƶ����Ϣ
* Output			:��
* Return			:�ɹ� / ������
* Others			:��
*****************************************************************************/
HI_S32 HI_UNF_TUNER_Connect(HI_U32  u32tunerId , HI_UNF_TUNER_CONNECT_PARA_S  *pstConnectPara,HI_U32 u32TimeOut);

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetStatus
* Description		:��ȡTUNER��Ƶ״̬����Ƶ����
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:pstTunerStatus: ���ص�ǰTUNER����Ƶ״̬����Ƶ����
				 �˲���ֻ�ں������سɹ�������²�������
* Return			:�ɹ� / ������
* Others			:��
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetStatus(HI_U32	u32tunerId , HI_UNF_TUNER_STATUS_S  *pstTunerStatus);

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetBER
* Description		:��ȡ��ǰTUNER �������ʣ��ÿ�ѧ��������ʾ
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:pu32BER: ָ��ǰTUNER �����ʵ�ָ�롣��ָ��ָ��һ����������Ԫ�ص����飬����Ԫ�غ�������:
				 pu32BER[0]:�����ʵ�������������
				 pu32BER[1]:�����ʵ�����С�����ֳ���1000
				 pu32BER[2]:������ָ������ȡ����ֵ
				 ����:������Ϊ2.156E-7����ô����Ԫ�ص�ȡֵ�ֱ�Ϊ
				 2��156��7
				 ������Ԫ��ֻ�ں������سɹ�������²������壬
				 ����Ӧ�ò�ɸ���ʵ�������������һ���ϴ�ֵ(����0.5��������Ԫ�طֱ�Ϊ5��0��1)��
* Return			:�ɹ� / ������
* Others			:��
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetBER(HI_U32   u32tunerId , HI_U32 *pu32BER); 

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetSNR
* Description		:��ȡ��ǰTUNER �������
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:pu32SNR:ָ��ǰTUNER����ȵ�ָ��
				:*pu32SNRȡֵ��ΧΪ22 ~ 51
				 �˲���ֻ�ں������سɹ�������²������壬
				 ����Ӧ�ò�ɽ���ֵ����Ϊ��Сֵ��
* Return			:�ɹ� / ������
* Others			:��
*****************************************************************************/
HI_S32 HI_UNF_TUNER_GetSNR(HI_U32	u32tunerId , HI_U32 *pu32SNR );				/* range : 0-255  */

/*****************************************************************************
* Function		:HI_UNF_TUNER_GetSignalStrength
* Description		:��ȡ��ǰTUNER���ź�ǿ��
* Input			:u32tunerId: TUNER�˿ںţ�ȡֵΪ0��1
* Output			:pu32SignalStrength:ָ��ǰTUNER�ź�ǿ�ȵ�ָ��
				:*pu32SignalStrength��ȡֵ��ΧΪ20~60
			  	 �˲���ֻ�ں������سɹ�������²������壬
			  	 ����Ӧ�ò�ɽ���ֵ����Ϊ��Сֵ��
* Return			:�ɹ� / ������
* Others			:��
*****************************************************************************/  
HI_S32 HI_UNF_TUNER_GetSignalStrength(HI_U32   u32tunerId , HI_U32 *pu32SignalStrength );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_UNF_API_TUNER_H*/
