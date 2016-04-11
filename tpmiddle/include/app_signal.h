#ifndef _APP_SIGNAL_H
#define _APP_SIGNAL_H



void APP_signal_IPI_LockTuner(FYF_QUEUE_MESSAGE msgRev);



/*
*获得信号锁定状态
*返回值：APP_SIGNAL_LOCK      锁定
		 APP_SIGNAL_UNLOCK    未锁定
*/
BU32 APP_Signal_API_GetSignalState(void);
/*
*获得信号电平
*返回值：信号电平大小  单位dbuV
		 范围 0 ~ 103 dbuV
*/
BU32 APP_Signal_API_GetSignalLever(void);

/*
*获得信噪比
*返回值：信噪比大小  单位db
		 范围 0 ~ 40 db
*/
BU08 APP_Signal_API_GetSignalSNR(void);
/*
*获得误码率
*返回值：误码率
         数据格式:0xAA BB CC DD 代表误码率为 0xAA.0XBB E-0XDD
         比如函数返回值为0x09 63 XX 08 代表误码率为 9.99 E-8
*/
BU32 APP_Signal_API_GetSignalBER(void);
/*
锁定到新频点
*/
void APP_Signal_API_TuneToNewParam(BU32 freqMHZ,BU32 sysKbps,APP_CHN_QAM_E qam);
#endif









