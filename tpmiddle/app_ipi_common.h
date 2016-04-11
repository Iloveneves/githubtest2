#ifndef _APP_IPI_COMMON_H
#define _APP_IPI_COMMON_H

#define FYF_THREAD_PRIORITY_CLOCK 			(12)
#define FYF_THREAD_STACK_CLOCK 				(20*1024)

#define FYF_THREAD_PRIORITY_SIGNALDETECT 	(13)
#define FYF_THREAD_STACK_SIGNALDETECT 		(20*1024)

#define FYF_THREAD_PRIORITY_RESTARTEPG 		(15)
#define FYF_THREAD_STACK_RESTARTEPG 		(10*1024)

#define FYF_THREAD_PRIORITY_SEARCH 			(14)
#define FYF_THREAD_STACK_SEARCH	 			(20*1024)

#define FYF_THREAD_PRIORITY_PLAYCHN 		(15)
#define FYF_THREAD_STACK_PLAYCHN  			(20*1024)

/*开机动态广告更新*/
//#define SYS_CONFIG_UPDATE_STARTLOGO

BU32 APP_IPI_GetQueueCommon(void);
BU32 APP_IPI_GetQueuePlayChn(void);
BU32 APP_IPI_GetQueueLockState(void);
BU32 APP_IPI_GetSectionCount(SectionNode_s *pFirstSectionNode);
BU32 APP_IPI_GetSemaphoreEit(void);
BU32 APP_IPI_GetSemaphoreLockTuner(void);
BU32 APP_IPI_GetSemaphoreParserFuncList(void);
BU32 APP_IPI_GetTime(void);
BU32 APP_IPI_GetTimeWithSec(void);
void APP_IPI_AddPasFuncNode(struct ParserFuncNode_s parserFuncNode);
BU08 APP_IPI_AddSection(SectionNode_s *pFirstSectionNode, BU08 section);
void APP_IPI_ClearSection(SectionNode_s **ppFirstSectionNode);
void APP_IPI_InitSectionNode(SectionNode_s **ppFirstSectionNode);
void APP_IPI_PauseParserTableThread(void);
void APP_IPI_ReStartParserTableThread(void);
BU32 APP_Parser_IPI_StartDemux(BU32 wide,BU16 pID, BU08 coef[], BU08 mask[], BU08 excl[], BU32 *channelID, BU32 *filterID);
BU32 APP_Parser_IPI_StopDemux(BU32 *channelID, BU32 *filterID);
BU32 APP_Cat_IPI_GetQueueParserCatEnd(void);
void APP_Cat_IPI_StartDemuxCat(APP_ParserFunc parserFunc);
void APP_Cat_IPI_ParCatOnSearchProg(BU08 *section, BU32 channelId, BU32 filterId);
void APP_Cat_IPI_ParCatOnChangeFreq(BU08 *section, BU32 channelId, BU32 filterId);
void APP_Cat_IPI_StopDemuxCat(void);
void APP_EIT_IPI_PauseDemuxEit(void);
void APP_EIT_IPI_ReStartDemuxEit(void);
void APP_EIT_IPI_StartDemuxCurChnAllEit(void);
void APP_EIT_IPI_StartDemuxPFActualEit(void);
void APP_EIT_IPI_StartParserEit(BU08 *section, BU32 channelId, BU32 filterId);
void APP_EIT_IPI_StopDemuxEit(void);
BU32 APP_EPG_IPI_Semaphore_Wait(BU32 time);
void APP_EPG_IPI_Semaphore_Release(void);
void APP_EPG_IPI_ReserveCallBack(BU32 callback_type, Epg_Info_S *EpgInfo);
void APP_EPG_IPI_InitReserve(void);
BU32 APP_NIT_IPI_GetQueueParserNitEnd(void);
void APP_NIT_IPI_StartDemuxNit(APP_ParserFunc parserFunc);
void APP_NIT_IPI_StartParserNit(BU08 *section, BU32 channelId, BU32 filterId);
void APP_NIT_IPI_StartParserNitGetVerAndOTA(BU08 *section, BU32 channelId, BU32 filterId);
void APP_NIT_IPI_StartParserNitGetVer(BU08 *section, BU32 channelId, BU32 filterId);
void APP_NIT_IPI_StopDemuxNit(void);
void APP_NIT_IPI_StartParseOTAInfo(BU08 *section, BU32 channelId, BU32 filterId);
BU32 APP_NIT_IPI_GetQueueParserOtaEnd(void);

void APP_NIT_API_StopDemuxOTAInfo();

void APP_NIT_API_StartDemuxOTAInfo(APP_ParserFunc parserFunc);
void APP_PAT_IPI_StartParserPat(BU08 *pSection, BU32 channelId, BU32 filterId);
void APP_PAT_IPI_StartDemuxPatOnTime(void);
void APP_PAT_IPI_StopDemuxPat(void);
void APP_PMT_IPI_StartDemuxPmtPcrEcm(BS16 pmtPID);
void APP_IPI_ResetTimeGetPmt(void);
void APP_IPI_SetTimeGetPmt(BU32 time);
void APP_PMT_IPI_StartParserPmt(BU08 *section, BU32 channelId, BU32 filterId);
void APP_PMT_IPI_StopDemuxPmt(BU16 index);
void APP_SDT_IPI_StartParserSdt(BU08 *section, BU32 channelId, BU32 filterId);
void APP_SDT_IPI_StopDemuxSdt(void);
void APP_TDT_IPI_StartParserTdt(BU08 *section);
void APP_TDT_IPI_StartParserTdtTot(BU08 *section, BU32 channelId, BU32 filterId);
void APP_TDT_IPI_StartParserTot(BU08 *section);
void APP_TDT_IPI_StopDemuxTot(void);
void APP_TDT_IPI_StartDemuxTdt(void);
void APP_TDT_IPI_StopDemuxTdt(void);
void APP_BAT_IPI_StartDemuxBat(void);
void APP_BAT_IPI_StartParserBat(BU08 *section, BU32 channelId, BU32 filterId);
void APP_IPI_SetTime(BU32 Year,BU32 Month, BU32 Date, BU32 Week, BU32 hour, BU32 Min, BU32 Sec);
BU32 APP_IPI_GetDay(void);
void StartClockThread(void);
void APP_IPI_SaveTunerInfo(BS32 Frequency, BS32 symbol_rate, BS32 Modulation);
BU32 APP_IPI_GetTunerFreq(void);
BU32 APP_IPI_GetTunerSym(void);
BU32 APP_IPI_GetTunerMod(void);





#endif




