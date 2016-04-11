#ifndef _APP_INIT_H_
#define _APP_INIT_H_



/*
是否需要更新节目
*/
BU08 APP_API_GetIsHasNewChn(void);
/*
设置标志节目已经更新
*/
void APP_API_SetHadUpdateChn(void);
/*
中间层初始化
*/
void APP_API_Init(void);
/*
获取OTA升级类型
*/
BU08 APP_OTA_API_GetOTAType(OTA_Info_S *pOtaInfo);
#endif


