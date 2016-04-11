/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_ecs_api.h
Version       : Initial Draft
Author        : Hisilicon multimedia Software group z60017280
Created       : 2006/7/21
Last Modified :
Description   : Hi3110-SW-D002
Function List :
History       :
 ******************************************************************************/
#ifndef __HI_ECS_API_H
#define __HI_ECS_API_H

#include "hi_type.h"
#include "hi_ecs_type.h"
#include "hi_ecs_errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <linux/ioctl.h>

#undef DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define API_DEBUG(fmt,args...) printf ("current pos : file (%s) , function(%s) ,line (%d)\n " fmt, __FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define API_DEBUG(fmt,args...) 
#endif


/*---- tuner ----*/
#define TUNERPORT0 0
#define TUNERPORT1 1
#define TUNERPORT2 2

HI_S32 HI_TUNER_Open (HI_VOID);
HI_S32 HI_TUNER_Close(HI_VOID);
HI_S32 HI_TUNER_Connect(HI_U32 u32TunerPort, TUNER_ACCQAMParameters_S  *pSignal);
HI_S32 HI_TUNER_ConnectUnblock(HI_U32 u32TunerPort, TUNER_ACCQAMParameters_S  *pSignal);
HI_S32 HI_TUNER_GetStatus(HI_U32 u32TunerPort,TUNER_Status_E  *pTunerStatus);
HI_S32 HI_TUNER_GetSignalStrength (HI_U32 u32TunerPort,HI_U32 *pu32SignalStrength);
HI_S32 HI_TUNER_GetBER(HI_U32 u32TunerPort,HI_U32 *pu32BER);
HI_S32 HI_TUNER_GetSNR(HI_U32 u32TunerPort,HI_U32 *pu32SNR);
HI_S32 HI_TUNER_SetTsType(HI_U32 u32TunerPort,TUNER_TsInterface_E eTsType);
HI_S32 HI_TUNER_SelectTuner(HI_U32 u32TunerPort,TUNER_Type_E eTunerType);
HI_S32 HI_TUNER_SelectI2cChannel(HI_U32 u32TunerPort, I2cChannel_E eI2cChannel); /* c47559 add 2008-05-20 AI7D02455 */
HI_S32 HI_TUNER_SelectSymbolRate(HI_U32 u32TunerPort, HI_U32 u32SymbolRate);
HI_S32 HI_TUNER_SetQamInside(TUNER_QamInside_E eQamInside);

HI_S32 HI_TUNER_CheckValid_I2cAddr(HI_U32 u32TunerPort, HI_U8 i2cAddr); /*add by yangsd @2009-07-17 */

/*---- e2prom ----*/
HI_S32 HI_E2PROM_Open(HI_VOID);
HI_S32 HI_E2PROM_Close(HI_VOID);
HI_S32 HI_E2PROM_Read (HI_U32 u32Addr, HI_U8* pu8Buf, HI_U32 u32Length);
HI_S32 HI_E2PROM_Write (HI_U32 u32Addr, HI_U8* pu8Buf, HI_U32 u32Length);
HI_S32 HI_E2PROM_SetChipType(HI_U32 u32ChipType);
HI_S32 HI_E2PROM_SetAddr(HI_U32 u32E2promAddr);
HI_S32 HI_E2PROM_SetI2cNum(HI_U32 u32I2cNum);


/*---- i2c ----*/
HI_S32 HI_I2C_Open (HI_VOID);
HI_S32 HI_I2C_Close(HI_VOID);
HI_S32 HI_I2C_Read (HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U8* pu8Buf, HI_U32 u32Length);
HI_S32 HI_I2C_Write (HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U8* pu8Buf, HI_U32 u32Length);
HI_S32 HI_I2C_SetRate (HI_U32 u32I2cNum, HI_U32 u32I2cRate);


/*---- keyled ----*/
HI_S32 HI_KEYLED_Open(HI_VOID);
HI_S32 HI_KEYLED_Close(HI_VOID);
HI_S32 HI_KEYLED_GetValue(HI_U32 *u32PressStatus, HI_U32 *u32KeyId);
HI_S32 HI_KEYLED_IsKeyUp(HI_U32 IsKeyUpArg);
HI_S32 HI_KEYLED_IsRepKey(HI_U32 IsRepKeyArg);
HI_S32 HI_KEYLED_RepKeyTimeoutVal(HI_U32 TimeoutValArg);
HI_S32 HI_KEYLED_Enable(HI_U32 u32Manager);
HI_S32 HI_KEYLED_Disable(HI_VOID);
HI_S32 HI_KEYLED_SetFlashPin(HI_KEYLED_LIGHT_E epin);
HI_S32 HI_KEYLED_SetFlashFreq(HI_KEYLED_LEVEL_E elevel);
HI_S32 HI_KEYLED_Display(HI_U32 data);
HI_S32 HI_KEYLED_DisplayTime(keyled_time_s keyled_time_info);
HI_S32 HI_KEYLED_SetBlockTime(HI_U32 blocktime);
HI_S32 HI_KEYLED_ClearBuf(HI_VOID);
HI_S32 HI_KEYLED_Set_LedType(HI_KEYLED_LEDTYPE_E eLedType);
HI_S32 HI_KEYLED_Set_CsType(HI_KEYLED_CSTYPE_E eCsType);

/*---- sc ----*/
HI_S32 HI_SC_Open(HI_VOID);
HI_S32 HI_SC_Close(HI_VOID);
HI_S32 HI_SC_SetPinMode(GPIO_PORT_FUNC_E eGroupName, HI_U32 u32Mode);
/*
HI_S32 HI_SC_SoftReset(HI_VOID);
HI_S32 HI_SC_ModuleReset(HI_U32 u32Module);
*/
/*---- pm ----*/
HI_S32 HI_PMoC_Open(HI_VOID);
HI_S32 HI_PMoC_Close(HI_VOID);
HI_S32 HI_PMoC_SwitchSystemMode(PMoC_MODE_E enSystemMode, PMoC_WAKEUP_E *enGetWakeupStatus);
HI_S32 HI_PMoC_ReadSystemMode(PMoC_MODE_E *enSystemMode);
HI_S32 HI_PMoC_SetPowerKeyVal(HI_U32 u32KeyVal);
HI_S32 HI_PMoC_SetKeyledPowerKeyVal(HI_U32 u32KeyVal);
HI_S32 HI_PMoC_StandbyWithDispTime(PTR_PMoC_STANDBY_MODE_S pstSet_Standby_Info);

/*---- ir ----*/
HI_S32 HI_IR_Open(HI_VOID);
HI_S32 HI_IR_Enable ( HI_U32 u32Manner);
HI_S32 HI_IR_Disable(HI_VOID);
HI_S32 HI_IR_GetValue(HI_U32 *u32PressStatus, HI_U32 *u32KeyId);
HI_S32 HI_IR_IsKeyUp(HI_U32 u32IsKeyUpArg);
HI_S32 HI_IR_IsRepKey(HI_U32 u32IsRepKeyArg);
HI_S32 HI_IR_RepKeyTimeoutVal(HI_U32 u32TimeoutValArg);
HI_S32 HI_IR_CodeType(IR_CodeType_E eCodeType);
HI_S32 HI_IR_UserCode(HI_U32 u32UserCode);
HI_S32 HI_IR_SetBlockTime(HI_U32 blocktime);
HI_S32 HI_IR_Close(HI_VOID);

/*---- rtc ----*/
HI_S32 HI_RTC_Open(HI_VOID);
HI_S32 HI_RTC_Close(HI_VOID);
HI_S32 HI_RTC_SetDateTime(PTR_RTC_DATETIME_S pRtcTime);
HI_S32 HI_RTC_GetDateTime(PTR_RTC_DATETIME_S pRtcTime);
HI_S32 HI_RTC_AlarmEnable(HI_VOID);
HI_S32 HI_RTC_AlarmDisable(HI_VOID);
HI_S32 HI_RTC_SetAlarm(PTR_RTC_DATETIME_S pRtcTime);
HI_S32 HI_RTC_GetAlarm(PTR_RTC_DATETIME_S pRtcTime);


/*---- wdg ----*/
//#define WATCHDOG_TIMEOUT 39768
#define WATCHDOG_TIMEOUT_MIN  2
#define WATCHDOG_TIMEOUT_MAX  42000


HI_S32 HI_WDG_SetTimeout(HI_U32 u32Value);
HI_S32 HI_WDG_Open(HI_VOID);
HI_S32 HI_WDG_Close(HI_VOID);
HI_S32 HI_WDG_Enable(HI_VOID);
HI_S32 HI_WDG_Disable(HI_VOID);
HI_S32 HI_WDG_ClearWatchDog (HI_VOID);
HI_S32 HI_WDG_RespondMode(HI_U32 u32RespondMode);
HI_S32 HI_WDG_Reset(HI_VOID);
/*---- gpio ----*/
HI_S32 HI_GPIO_Open(HI_VOID);
HI_S32 HI_GPIO_Close(HI_VOID);
HI_S32 HI_GPIO_Read_Byte(PTR_GPIO_Data_S);
HI_S32 HI_GPIO_Write_Byte(PTR_GPIO_Data_S);  
HI_S32 HI_GPIO_Read_Bit(PTR_GPIO_Data_S);
HI_S32 HI_GPIO_Write_Bit(PTR_GPIO_Data_S);
HI_S32 HI_GPIO_DirSet_Bit(PTR_GPIO_Data_S);
HI_S32 HI_GPIO_DirGet_Bit(PTR_GPIO_Data_S);
HI_S32 HI_GPIO_DirSet_Byte(PTR_GPIO_Data_S);
HI_S32 HI_GPIO_DirGet_Byte(PTR_GPIO_Data_S);

/*---- dmac ----*/
HI_S32 HI_DMAC_Open(HI_VOID);
HI_S32 HI_DMAC_Close(HI_VOID);
HI_S32 HI_DMAC_GetFreeChannel(HI_U32 *pu32ChannelNum);
HI_S32 HI_DMAC_GetStatus(HI_U32 u32ChannelNum);
HI_S32 HI_DMAC_MemToMemtrans(Dmac_Config_Para_S *prtConfigPara);
HI_S32 HI_DMAC_MemPeripheralTrans(Dmac_Config_Para_S *prtConfigPara);
HI_S32 HI_DMAC_ListParaReset(HI_U32 u32ChannelNum);
HI_S32 HI_DMAC_AddListItem(Dmac_Config_Para_S *prtConfigPara);
HI_S32 HI_DMAC_StartListTransfer(Dmac_Config_Para_S *prtConfigPara);

/* ---- cipher ---- */
HI_S32 HI_CIPHER_Open(HI_VOID);
HI_S32 HI_CIPHER_Close(HI_VOID);
HI_S32 HI_CIPHER_GetHandle(HI_U32* pu32HandleID);
HI_S32 HI_CIPHER_ReleaseHandle(HI_U32 u32HandleID);
HI_S32 HI_CIPHER_ConfigAES (CIPHER_CTRL_S* daes_ctrl , CIPHER_AES_KEYIVIN_S* aes_key_ivin, HI_U32 u32Handle);
HI_S32 HI_CIPHER_ConfigDES (CIPHER_CTRL_S* daes_ctrl , CIPHER_DES_KEYIVIN_S* des_key_ivin,  HI_U32 u32Handle);
HI_S32 HI_CIPHER_Encrypt(HI_U32 u32Src, HI_U32 u32Dest, HI_U32 u32ByteLength, HI_U32 u32Handle);
HI_S32 HI_CIPHER_Decrypt(HI_U32 u32Src, HI_U32 u32Dest, HI_U32 u32ByteLength, HI_U32 u32Handle);

/* ---- eth ---- */
HI_S32 HI_ETH_Open(Eth_Port_E ePort);
HI_S32 HI_ETH_Close(Eth_Port_E ePort);
HI_S32 HI_ETH_IPAddressSet (Eth_Port_E ePort, HI_S8* ipAdd);
HI_S32 HI_ETH_IPAddressGet (Eth_Port_E ePort, HI_S8 *ipAdd);
HI_S32 HI_ETH_SubNetmaskSet (Eth_Port_E ePort, HI_S8* subNetmask);
HI_S32 HI_ETH_SubNetmaskGet (Eth_Port_E ePort, HI_U8* subNetmask);
HI_S32 HI_ETH_GatewaySet (Eth_Port_E ePort, HI_S8* gateway);
HI_S32 HI_ETH_GatewayGet (Eth_Port_E ePort, HI_U8* gateway);
HI_S32 HI_ETH_DNSSet (Eth_Port_E ePort, HI_BOOL enable, HI_S8 *dns);
HI_S32 HI_ETH_DNSGet (Eth_Port_E ePort, HI_U8 *dns);
HI_S32 HI_ETH_SetMac(Eth_Port_E ePort, HI_S8 *mac);
HI_S32 HI_ETH_GetMac(Eth_Port_E ePort, HI_U8 *mac);
HI_S32 HI_ETH_GetLinkStatus(Eth_Port_E ePort, PTR_Eth_LinkStatus_E ptrLinkStatus);
HI_S32 HI_ETH_GetLinkStatus_Special(Eth_Port_E ePort, PTR_Eth_LinkStatus_E ptrLinkStatus);
HI_S32 HI_ETH_LinkCfg(Eth_Port_E ePort, Eth_LinkCfg_E eLinkCfg);
HI_S32 HI_ETH_WorkModeSet(Eth_WorkMode_E eWorkMode);
HI_S32 HI_ETH_FwCtrlSet(Eth_Forward_E eItem, HI_U32 u32Fw);
HI_S32 HI_ETH_MactCtrlSet(Eth_Mactctrl_E eItem, HI_U32 u32Macctrl);
HI_S32 HI_ETH_MacTableEnable(Eth_Port_E ePort);
HI_S32 HI_ETH_MacTableDisable(Eth_Port_E ePort);
HI_S32 HI_ETH_MacTableSet(Eth_MacTblIdx_E eMacTblIndex, HI_U32 u32MacHi16, HI_U32 u32MacLo32);
HI_VOID HI_ETH_VlanEnable(HI_VOID);
HI_VOID HI_ETH_VlanDisable(HI_VOID);
HI_S32 HI_ETH_VlanTableSet(Eth_VlanTblIdx_E eVlanTblIndex, HI_U32 u32Vlan);
HI_VOID HI_ETH_CpuVlanCtrlSet(HI_U32 u32CpuVlanCtrl);
HI_VOID HI_ETH_NoVlanCtrlSet(HI_U32 u32NoVlanCtrl);
HI_VOID HI_ETH_SpecVlanCtrlSet(HI_U32 u32SpecVlanCtrl);
HI_VOID HI_ETH_UnkownVlanSet(HI_U32 u32UnkownVlanCtrl);
HI_VOID HI_ETH_PortVlanIdSet(HI_U32 u32PortVlanId);
HI_S32 HI_ETH_FcTimeCtrl(Eth_Port_E ePort, HI_U32 u32FcTimeCtrl);
HI_S32 HI_ETH_FcRxLimit(Eth_Port_E ePort, HI_U32 u32FcRxLimit);
HI_S32 HI_ETH_FcDropCtrl(Eth_Port_E ePort, HI_U32 u32FcDropCtrl);
HI_S32 HI_ETH_FcLevel(Eth_Port_E ePort, HI_U32 u32FcLevel);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_ECS_API_H*/
