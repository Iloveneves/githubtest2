#ifndef _APP_SIGNAL_H
#define _APP_SIGNAL_H



void APP_signal_IPI_LockTuner(FYF_QUEUE_MESSAGE msgRev);



/*
*����ź�����״̬
*����ֵ��APP_SIGNAL_LOCK      ����
		 APP_SIGNAL_UNLOCK    δ����
*/
BU32 APP_Signal_API_GetSignalState(void);
/*
*����źŵ�ƽ
*����ֵ���źŵ�ƽ��С  ��λdbuV
		 ��Χ 0 ~ 103 dbuV
*/
BU32 APP_Signal_API_GetSignalLever(void);

/*
*��������
*����ֵ������ȴ�С  ��λdb
		 ��Χ 0 ~ 40 db
*/
BU08 APP_Signal_API_GetSignalSNR(void);
/*
*���������
*����ֵ��������
         ���ݸ�ʽ:0xAA BB CC DD ����������Ϊ 0xAA.0XBB E-0XDD
         ���纯������ֵΪ0x09 63 XX 08 ����������Ϊ 9.99 E-8
*/
BU32 APP_Signal_API_GetSignalBER(void);
/*
��������Ƶ��
*/
void APP_Signal_API_TuneToNewParam(BU32 freqMHZ,BU32 sysKbps,APP_CHN_QAM_E qam);
#endif









