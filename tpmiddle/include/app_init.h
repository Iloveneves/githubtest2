#ifndef _APP_INIT_H_
#define _APP_INIT_H_



/*
�Ƿ���Ҫ���½�Ŀ
*/
BU08 APP_API_GetIsHasNewChn(void);
/*
���ñ�־��Ŀ�Ѿ�����
*/
void APP_API_SetHadUpdateChn(void);
/*
�м���ʼ��
*/
void APP_API_Init(void);
/*
��ȡOTA��������
*/
BU08 APP_OTA_API_GetOTAType(OTA_Info_S *pOtaInfo);
#endif


