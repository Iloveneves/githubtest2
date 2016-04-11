
/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hi_demux_api.h
* Description:  Declare interfaces provided to users who develop application.
*
* History:
* Version      Date         Author     DefectNum    Description
* main\1    2006-11-01      l55251        NULL      Create this file.
***********************************************************************************/

#ifndef __HI_DEMUX_API_H__
#define __HI_DEMUX_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_demux_errcode.h"
#include "hi_demux_type.h"

/******************************* API declaration *****************************/
HI_S32 HI_DMX_Open(HI_VOID);
HI_S32 HI_DMX_Close(HI_VOID);
HI_S32 HI_DMX_Reset(HI_VOID);
HI_S32 HI_DMX_RegError (HI_U32  u32ErrorEvent);
HI_S32 HI_DMX_WaitError(HI_U32 u32TimeOut, DMX_WaitError_S *pError);/*AI7D01879*/
HI_S32  HI_DMX_ReadData(HI_U32 u32ChannelId, HI_U32* pFilterId,HI_U8 *pBuf, HI_U32 u32Len, HI_U32 *pCopyLen);
HI_S32  HI_DMX_GetDataFlag(DMX_ChanSelect_E eChanOption, HI_U32 u32ChannelId, HI_U32 *pFlag, HI_U32 u32TimeOut);

/* port */
HI_S32 HI_DMX_OpenTSPort(HI_VOID);
HI_S32 HI_DMX_CloseTSPort(HI_VOID);
HI_S32 HI_DMX_SetTSPortType(DMX_PortType_E eType);
HI_S32 HI_DMX_GetTSPortType(DMX_PortType_E *pType);
HI_S32 HI_DMX_GetTSPortStatus(DMX_PortStatus_E *pStatus);


/* channel */
HI_S32 HI_DMX_NewChannel(DMX_ChanType_E eChannelType, DMX_BufType_E eBufType, HI_U32 *pChannelId);
HI_S32 HI_DMX_DeleteChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_GetAudioChannelId(HI_U32 *pChannelId);
HI_S32 HI_DMX_GetVideoChannelId(HI_U32 *pChannelId);
HI_S32 HI_DMX_GetAudioScrambledFlag(DMX_ScrambleFlag_E *pScrambleFlag);
HI_S32 HI_DMX_GetVideoScrambledFlag(DMX_ScrambleFlag_E *pScrambleFlag);
HI_S32 HI_DMX_OpenChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_CloseChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_GetChannelStatus (HI_U32 u32ChannelId, HI_BOOL *pChanStatus);
HI_S32 HI_DMX_SetChannelPID(HI_U32 u32ChannelId, HI_U32 u32Pid);
HI_S32 HI_DMX_GetChannelPID(HI_U32 u32ChannelId, HI_U32 *pPid);
HI_S32 HI_DMX_GetChannelId(HI_U32 u32Pid, HI_U32 *pChannelId);
HI_S32 HI_DMX_GetFreeChannelCnt(HI_U32 *pFreeCount);
HI_S32 HI_DMX_EnableChannelCRC(HI_U32 u32ChannelId);
HI_S32 HI_DMX_DisableChannelCRC(HI_U32 u32ChannelId);
HI_S32 HI_DMX_EnableTsPost(HI_U32 u32ChannelId);
HI_S32 HI_DMX_DisableTsPost(HI_U32 u32ChannelId);
HI_S32 HI_DMX_SetTsPostThresh(HI_U32 u32ChannelId, HI_U32 u32Threshold);
HI_S32 HI_DMX_GetTsPostThresh(HI_U32 u32ChannelId, HI_U32 *pThreshold);
HI_S32 HI_DMX_GetChannelFilterId(HI_U32 u32ChannelId, HI_U32 *pFilterFlag);

/* data filter */
HI_S32 HI_DMX_NewFilter(HI_U32 *pFilterId);
HI_S32 HI_DMX_DeleteFilter(HI_U32 u32FilterId);
HI_S32 HI_DMX_DeleteAllFilter(HI_U32 u32ChannelId);

HI_S32 HI_DMX_SetFilter(HI_U32 u32FilterId, HI_U32 u32FilterDepth,HI_U8 *pMatch, HI_U8 *pMask, HI_U8 *pNegate);
HI_S32 HI_DMX_GetFilter(HI_U32 u32FilterId, HI_U32 *pFilterDepth, HI_U8 *pMatch, HI_U8 *pMask, HI_U8 *pNegate);
HI_S32 HI_DMX_AttachFilter(HI_U32 u32FilterId, HI_U32 u32ChannelId);
HI_S32 HI_DMX_DetachFilter(HI_U32 u32FilterId, HI_U32 u32ChannelId);
HI_S32 HI_DMX_GetFilterChannelId(HI_U32 u32FilterId, HI_U32 *pChannelId);
HI_S32 HI_DMX_GetFreeFilterCnt(HI_U32 *pFreeCount);

/* keyset */
HI_S32 HI_DMX_NewDescrambler(HI_U32 *pKeyId);
HI_S32 HI_DMX_DeleteDescrambler(HI_U32 u32KeyId);
HI_S32 HI_DMX_SetDescramblerEvenKey(HI_U32 u32KeyId, HI_U8 *pEvenKey);
HI_S32 HI_DMX_SetDescramblerOddKey(HI_U32 u32KeyId, HI_U8 *pOddKey);
HI_S32 HI_DMX_GetDescramblerKeyId(HI_U32 u32ChannelId, HI_U32 *pKeyId);
HI_S32 HI_DMX_AttachDescrambler(HI_U32 u32KeyId, HI_U32 u32ChannelId);
HI_S32 HI_DMX_DetachDescrambler(HI_U32 u32KeyId, HI_U32 u32ChannelId);
HI_S32 HI_DMX_GetFreeDescramblerKeyCnt(HI_U32 *pFreeCount);

/* pcr */
HI_S32 HI_DMX_SetPCRPID(HI_U32 u32Pid);
HI_S32 HI_DMX_GetPCRPID(HI_U32 *pPid);
HI_S32 HI_DMX_GetCurrentPCR(DMX_PCR_S *pPCR);
HI_S32 HI_DMX_GetCurrentLMC(DMX_PCR_S * pLMC);
HI_S32 HI_DMX_ReleaseMsg(DMX_UserMsgDesc_S *psUserMsgDesc);
HI_S32 HI_DMX_AcquireMsg(DMX_UserMsgDesc_S *psUserMsgDesc);
HI_S32	HI_DMX_GetMsgBufSize(HI_U32 *u32MsgBufSize) ;//AI7D05022 09/03/24
/*X5 新增*/

#ifdef HI_DMX_CHAN_BUF_DIRECTLY
HI_S32 HI_DMX_TestGetDemuxInfo(HI_DMX_TEST_INFO_S *pstDmx);
HI_S32 HI_DMX_TestGetChannelInfo(HI_DMX_TEST_CHAN_INFO_S *pstChan);
HI_S32 HI_DMX_TestGetFilterInfo(HI_DMX_TEST_FILTER_INFO_S * pstFilter);
HI_S32 HI_DMX_TestGetKeyInfo(HI_DMX_TEST_KEYSET_INFO_S * pstKey);
HI_S32 HI_DMX_TestGetRecordInfo(HI_DMX_TEST_RECORD_INFO_S * pstRecord);
HI_S32 HI_DMX_TestGetAllInfo(HI_DMX_TEST_ALL_INFO_S * pstAll);
HI_S32 HI_DMX_TestGetAllProcInfo(HI_DMX_TEST_ALL_PROC_INFO_S * pstProc);
HI_CHAR * HI_DMX_TestGetNameTsSourceType(DMX_TS_SOURCE_TYPE_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameChanType(DMX_ChanType_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameChanFuncType(DMX_CHAN_FUNC_TYTE_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameChanStatus(DMX_CHAN_STATUS_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameFlushStatus(DMX_FlushStatus_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameFlushType(DMX_FLUSH_TYPE_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameFilterState(DMX_FilterState_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameKeySetState(DMX_KeySetState_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameRecordType(DMX_RECORD_TYPE_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameRecordStat(DMX_RECORD_STATUS_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameDbgEvtAct(HI_DMX_DBG_EVT_ACT_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameDbgEvtDatType(HI_DMX_DBG_EVT_CHAN_DAT_TYPE_E enVol, HI_CHAR * pszName);
HI_CHAR * HI_DMX_TestGetNameDbgEvtDescDat(HI_DMX_DBG_EVT_DESC_DAT_TYPE_E enVol, HI_CHAR * pszName);
HI_U32 HI_DMX_GetChanId(DMX_CHAN_HANDLE hChanHandle, HI_U32 *pu32DmxId);
HI_U32 HI_DMX_GetFilterId(DMX_FILTER_HANDLE hFilterHandle, HI_U32 *pu32DmxId);
HI_U32 HI_DMX_GetKeyId(DMX_KEY_HANDLE hKeyHandle, HI_U32 *pu32DmxId);
DMX_CHAN_HANDLE HI_DMX_GetChanHandle(HI_U32 u32DmxId, HI_U32 u32ChanId);
DMX_FILTER_HANDLE HI_DMX_GetFilterHandle(HI_U32 u32DmxId, HI_U32 u32FilterId);
DMX_KEY_HANDLE HI_DMX_GetKeyHandle(HI_U32 u32DmxId, HI_U32 u32KeyId);
HI_S32 HI_DMX_GetAttrFlag(HI_DMX_ATTR_FLAG_TYPE_E enType, 
    HI_U32 u32DmxId, HI_U32 u32ObjectId, HI_U32 *pu32AttrVol, HI_U32 *pu32AttrExpand);
HI_S32 HI_DMX_SetAttrFlag(HI_DMX_ATTR_FLAG_TYPE_E enType, 
    HI_U32 u32DmxId, HI_U32 u32ObjectId, HI_U32 u32AttrVol, HI_U32 u32AttrExpand);
HI_U32 HI_DMX_GetDebugBufferSize(HI_VOID);
HI_S32 HI_DMX_SetDebugBufferSize(HI_U32 u32Size);
#define HI_DMX_DBG_EVT_CRT_DEL      1
#define HI_DMX_DBG_EVT_EOP_DAT      2
#define HI_DMX_DBG_EVT_DAT_BUF      4
#define HI_DMX_DBG_EVT_MSG_BUF      8
#define HI_DMX_DBG_EVT_KEY_DAT      16
#define HI_DMX_DBG_EVT_ALL          0xffffffff
HI_U32 HI_DMX_GetGlobalDebugEventFlag(HI_VOID);
HI_S32 HI_DMX_SetGlobalDebugEventFlag(HI_U32 u32Flag);
HI_U32 HI_DMX_GetChanDebugEventFlag(HI_U32 u32DmxId, HI_U32 u32ChanId);
HI_S32 HI_DMX_SetChanDebugEventFlag(HI_U32 u32DmxId, HI_U32 u32ChanId, HI_U32 u32Flag);
HI_U32 HI_DMX_GetKeyDebugEventFlag(HI_U32 u32DmxId, HI_U32 u32ChanId);
HI_S32 HI_DMX_SetKeyDebugEventFlag(HI_U32 u32DmxId, HI_U32 u32ChanId, HI_U32 u32Flag);
HI_BOOL HI_DMX_GetDebugPrintTrigger(HI_DMX_DBG_EVT_TRIGGER_E enTriggerNo);
HI_S32 HI_DMX_SetDebugPrintTrigger(HI_DMX_DBG_EVT_TRIGGER_E enTriggerNo, HI_BOOL bPrint);
HI_S32 HI_DMX_SetAllDebugPrintTrigger(HI_BOOL bPrint);
HI_BOOL HI_DMX_GetDebugPrintApply(HI_VOID);
HI_S32 HI_DMX_SetDebugPrintApply(HI_BOOL bApply);
HI_S32 HI_DMX_GetFirstDebugEvent(HI_DMX_DBG_EVT_REC_S *pRecord);
HI_S32 HI_DMX_PrintfDebugEvent(HI_DMX_DBG_EVT_REC_S *pRecord, HI_U8 *pBuf, HI_U32 u32BufSize);
HI_S32 HI_DMX_CheckPrintfDebugEvent(HI_VOID);
#endif

/*设置指定DEMUX的输入接口类型*/
HI_S32 HI_MutiDMX_SetTSSource(HI_U32 DemuxId, DMX_TS_SOURCE_TYPE_E eSource);
/*获取指定DEMUX的输入接口类型*/
HI_S32 HI_MutiDMX_GetTSSource(HI_U32 DemuxId, DMX_TS_SOURCE_TYPE_E *pSource);

/*api for certern Dev id*/
HI_S32 HI_MutiDMX_OpenSubDev(HI_U32 u32DmxId);
HI_S32 HI_MutiDMX_ResetSubDev(HI_U32 u32DmxId);
HI_S32 HI_MutiDMX_CloseSubDev(HI_U32 u32DmxId);


HI_S32 HI_MutiDMX_RegError (HI_U32 DemuxId,HI_U32  u32ErrorEvent);
HI_S32 HI_MutiDMX_WaitError(HI_U32 DemuxId,HI_U32 u32TimeOut, DMX_WaitError_S *pError);
HI_S32  HI_MutiDMX_ReadData(HI_U32 u32DmxId,HI_U32 u32ChanId,HI_U32* pFilterId, HI_U8 *pBuf,
        HI_U32 u32Len, HI_U32* pCopyLen,DMX_DataRead_E eReadType);
HI_S32  HI_MutiDMX_GetDataFlag( HI_U32 DemuxId,DMX_ChanSelect_E eChanOption,HI_U32 u32ChannelId, HI_U32 *pFlag, HI_U32 u32TimeOut);

/*AI7D03609 m00129935 08/11/21 begin*/
/*适配统一接口:获取三路子设备的数据状态*/
HI_S32 HI_MutiDMX_GetAllDmxDataFlag(HI_U32 *pu32ChannelSelect,HI_U32 * pu32Flag, HI_U32 u32ChTotalNum, HI_U32 u32TimeOut);
/*AI7D03609 m00129935 08/11/21 end*/

/* port */
HI_S32 HI_MutiDMX_OpenTSPort(HI_U32 DemuxId);
HI_S32 HI_MutiDMX_CloseTSPort(HI_U32 DemuxId);
HI_S32 HI_MutiDMX_SetTSPortType(HI_U32 DemuxId,DMX_PortType_E eType);
HI_S32 HI_MutiDMX_GetTSPortType(HI_U32 DemuxId,DMX_PortType_E *pType);
HI_S32 HI_MutiDMX_GetTSPortStatus(HI_U32 DemuxId,DMX_PortStatus_E *pStatus);

/*AI7D04251 适配统一接口 m00129935 09/01/04 begin*/
HI_S32 HI_MutiDMX_TSPortAttrGet(HI_U32 u32PortId, DMX_Port_Attr_S * pstAttr);
HI_S32 HI_MutiDMX_TSPortAttrSet(HI_U32 u32PortId, DMX_Port_Attr_S * pstAttr);
HI_S32 HI_MutiDMX_TSPortOpen(HI_U32 u32PortId);
HI_S32 HI_MutiDMX_TSPortClose(HI_U32 u32PortId);
HI_S32 HI_MutiDMX_TSPortStatusGet(HI_U32 u32PortId,DMX_PortStatus_E *pStatus);
/*AI7D04521 适配统一接口 m00129935 09/01/04 end*/

/*begin: add by mxf on 2009.04.29 */
HI_S32 HI_MUtiDMX_GetTsVersatileState(DMX_SOURCE_SEL_E enPortSource, DMX_PORT_VERSATILE_STATE_S *pstVersatileState);
/*end: add by mxf on 2009.04.29 */

/* channel */
HI_S32 HI_MutiDMX_NewChannel(HI_U32 DemuxId, DMX_ChanType_E eChannelType,  DMX_CHAN_FUNC_TYTE_E CreatType, HI_U32 u32BufSize,HI_U32 *pChannelId);

HI_S32 HI_MutiDMX_AddChannelFunc(HI_U32 DemuxId, HI_U32 ChannelId, DMX_CHAN_FUNC_TYTE_E CreatType, DMX_BUFFER_SIZE_E eBufSize);
HI_S32 HI_MutiDMX_RemoveChannelFunc(HI_U32 DemuxId, HI_U32 ChannelId, DMX_CHAN_FUNC_TYTE_E CreatType);

HI_S32 HI_MutiDMX_DeleteChannel(HI_U32 DemuxId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_GetAudioChannelId(HI_U32 DemuxId, HI_U32 *pChannelId);
HI_S32 HI_MutiDMX_GetVideoChannelId(HI_U32 DemuxId, HI_U32 *pChannelId);
HI_S32 HI_MutiDMX_GetAudioScrambledFlag(HI_U32 DemuxId, DMX_ScrambleFlag_E *pScrambleFlag);
HI_S32 HI_MutiDMX_GetVideoScrambledFlag(HI_U32 DemuxId, DMX_ScrambleFlag_E *pScrambleFlag);
HI_S32 HI_MutiDMX_OpenChannel(HI_U32 DemuxId, HI_U32 u32ChannelId,DMX_CHAN_FUNC_TYTE_E CreatType);
HI_S32 HI_MutiDMX_CloseChannel(HI_U32 DemuxId, HI_U32 u32ChannelId,DMX_CHAN_FUNC_TYTE_E CreatType);
HI_S32 HI_MutiDMX_GetChannelStatus (HI_U32 u32DmxId,HI_U32 u32ChannelId, DMX_CHAN_STATUS_E *pChanStatus);
HI_S32 HI_MutiDMX_SetChannelPID(HI_U32 DemuxId, HI_U32 u32ChannelId, HI_U32 u32Pid);
HI_S32 HI_MutiDMX_GetChannelPID(HI_U32 DemuxId, HI_U32 u32ChannelId, HI_U32 *pPid);
HI_S32 HI_MutiDMX_GetChannelId(HI_U32 DemuxId, HI_U32 u32Pid, HI_U32 *pChannelId);
HI_S32 HI_MutiDMX_GetFreeChannelCnt(HI_U32 DemuxId, HI_U32 *pFreeCount);
HI_S32 HI_MutiDMX_EnableChannelCRC(HI_U32 DemuxId, HI_U32 u32ChannelId,DMX_CHAN_FUNC_TYTE_E eFuncType);
HI_S32 HI_MutiDMX_DisableChannelCRC(HI_U32 DemuxId, HI_U32 u32ChannelId,DMX_CHAN_FUNC_TYTE_E eFuncType);
HI_S32 HI_MutiDMX_EnableTsPost(HI_U32 DemuxId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_DisableTsPost(HI_U32 DemuxId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_SetTsPostThresh(HI_U32 DemuxId, HI_U32 u32ChannelId, HI_U32 u32Threshold);
HI_S32 HI_MutiDMX_GetTsPostThresh(HI_U32 DemuxId, HI_U32 u32ChannelId, HI_U32 *pThreshold);
HI_S32 HI_MutiDMX_GetChannelFilterId(HI_U32 DemuxId, HI_U32 u32ChannelId, HI_U32 *pHighFilterFlag, HI_U32* pLowFilterFlag);

/*AI7D03609 m00129935 08/11/21 begin*/
/*适配统一接口:获取通道属性*/
HI_S32 HI_MutiDMX_GetChannelAttr( HI_U32 DemuxId, HI_U32 u32ChannelId, DMX_Chan_Attr_S * pstChAttr);
/*AI7D03609 m00129935 08/11/21 end*/

/* data filter */
HI_S32 HI_MutiDMX_NewFilter(HI_U32 DemuxId, HI_U32 *pFilterId);
HI_S32 HI_MutiDMX_DeleteFilter(HI_U32 DemuxId, HI_U32 u32FilterId);
HI_S32 HI_MutiDMX_DeleteAllFilter(HI_U32 DemuxId, HI_U32 u32ChannelId);

HI_S32 HI_MutiDMX_SetFilter(HI_U32 DemuxId, HI_U32 u32FilterId, HI_U32 u32FilterDepth,HI_U8 *pMatch, HI_U8 *pMask, HI_U8 *pNegate,HI_BOOL bRev);
HI_S32 HI_MutiDMX_GetFilter(HI_U32 DemuxId, HI_U32 u32FilterId, HI_U32 *pFilterDepth, HI_U8 *pMatch, HI_U8 *pMask, HI_U8 *pNegate,HI_BOOL *pbRev);
HI_S32 HI_MutiDMX_AttachFilter(HI_U32 DemuxId, HI_U32 u32FilterId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_DetachFilter(HI_U32 DemuxId, HI_U32 u32FilterId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_GetFilterChannelId(HI_U32 DemuxId, HI_U32 u32FilterId, HI_U32 *pChannelId);
HI_S32 HI_MutiDMX_GetFreeFilterCnt(HI_U32 DemuxId, HI_U32 *pFreeCount);

/* keyset */
HI_S32 HI_MutiDMX_NewDescrambler(HI_U32 DemuxId, HI_U32 *pKeyId);
HI_S32 HI_MutiDMX_DeleteDescrambler(HI_U32 DemuxId, HI_U32 u32KeyId);
HI_S32 HI_MutiDMX_SetDescramblerEvenKey(HI_U32 DemuxId, HI_U32 u32KeyId, HI_U8 *pEvenKey);
HI_S32 HI_MutiDMX_SetDescramblerOddKey(HI_U32 DemuxId, HI_U32 u32KeyId, HI_U8 *pOddKey);
HI_S32 HI_MutiDMX_GetDescramblerKeyId(HI_U32 DemuxId, HI_U32 u32ChannelId, HI_U32 *pKeyId);
HI_S32 HI_MutiDMX_AttachDescrambler(HI_U32 DemuxId, HI_U32 u32KeyId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_DetachDescrambler(HI_U32 DemuxId, HI_U32 u32KeyId, HI_U32 u32ChannelId);
HI_S32 HI_MutiDMX_GetFreeDescramblerKeyCnt(HI_U32 DemuxId, HI_U32 *pFreeCount);


/*api about recoder supporting PVR functions*/
HI_S32 HI_MutiDMX_SetRecodeType(HI_U32 u32DmxId, DMX_RECORD_TYPE_E eRecType);
HI_S32 HI_MutiDMX_GetRecodeType(HI_U32 u32DmxId, DMX_RECORD_TYPE_E *pRecType);

HI_S32 HI_MutiDMX_StartRecoder(HI_U32 u32DmxId,PVR_REC_INDEX_TYPE_E eIndexType,HI_U32 u32SCDBufSize,
                              HI_U32 u32TsBufSize,const PVR_CIPHER_S *pCipherCfg);
HI_S32 HI_MutiDMX_StopRecoder(HI_U32 u32DmxId);
HI_S32 HI_MutiDMX_SmoothSwichRecoder(HI_U32 u32DmxId,HI_U32 timeout);
HI_S32 HI_MutiDMX_GetRecoderStatus(HI_U32 u32DmxId,HI_U32 * pStatus);
HI_S32 HI_MutiDMX_SetRefChanType(DMX_REF_CHAN_TYPE_E eRefChanType);
HI_S32 HI_MutiDMX_GetHddBufCnt(HI_U32 u32DemuxID, HI_U32 *pCnt);
#ifdef HI_DMX_CHAN_BUF_DIRECTLY
HI_S32 HI_MutiDMX_ResetHddBuf(HI_VOID);
#else
HI_S32 HI_MutiDMX_ResetHddBuf();
#endif
HI_S32 HI_MutiDMX_StartCipher(HI_U32 u32DemuxID);



/*ring buffer manager*/
/****************************************************************
Summary:
This function resets a ring buffer to the init status.
****************************************************************/

/****************************************************************
Summary:
This function requests to add data into the ring buffer manager
If there is no free space available this function will
block (if required) until free space becomes available.
****************************************************************/
HI_S32  HI_MutiDMX_AddDataRequest(
            HI_U32 u32DemuxID,    /* the buffer id */
            DMX_DATA_S *p_bfr, /* returns pointer of starting buffer address of free space and size of the free space (in bytes) */
            HI_BOOL block,      /* false (do not block), true (block until free space is available) */
            HI_U32 timeout      /* max num of msec to block */
            );

/****************************************************************
Summary:
This function increments the ring buffer write pointer
It is used in conjuntion with the HI_MutiDMX_AddDataRequest()
to add data into the ring buffer.
****************************************************************/
HI_S32 HI_MutiDMX_UpdateWritePointer(
            HI_U32 u32DemuxID,                  /* the demux id */
            HI_U32 count                        /* number of bytes to increment the write pointer */
            );


/****************************************************************
Summary:
This function requests to remove valid data from the ring buffer manager
If there is no valid data available this function will
block (if required) until valid data becomes available.
****************************************************************/
HI_S32 HI_MutiDMX_RemoveDataRequest(
            HI_U32 u32DemuxID,            /* the buffer id */
            DMX_DATA_S *p_bfr,         /* returns pointer of starting buffer address of valid data and size of the valid data (in bytes) */
            HI_BOOL block,              /* false (do not block), true (block until free space is available) */
            HI_BOOL bDoCipher,
            HI_U32 timeout              /* max num of msec to block */
            );

/****************************************************************
Summary:
This function increments the ring buffer read pointer.
It is used in conjuntion with the HI_MutiDMX_RemoveDataRequest()
to remove valid data from the ring buffer.
****************************************************************/
HI_S32 HI_MutiDMX_UpdateReadPointer(
            HI_U32 u32DemuxID,                    /* the buffer id */
            HI_U32 count                        /* number of bytes to increment the read pointer */
            );

HI_S32 HI_MutiDMX_RemoveSCDRequest(
            HI_U32 u32DemuxID,            /* the buffer id */
            DMX_DATA_S *p_bfr,         /* returns pointer of starting buffer address of valid data and size of the valid data (in bytes) */
            HI_BOOL block,              /* false (do not block), true (block until free space is available) */
            HI_U32 timeout              /* max num of msec to block */
            );
HI_S32 HI_MutiDMX_UpdateSCDReadPointer(
            HI_U32 u32DemuxID,                    /* the buffer id */
            HI_U32 count                        /* number of bytes to increment the read pointer */
            );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEMUX_API_H__ */


