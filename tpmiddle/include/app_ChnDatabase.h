#ifndef _APP_CHNDATABASE_H_
#define _APP_CHNDATABASE_H_

#define CHN_ZERO_PROG_SERVICE_ID_INVALID 0x1A2B3C4D
#define CHN_LONGYAN_ZERO_CHN_ID 1207
#define CHN_ZERO_PROG_SERVICE_ID 2003//CHN_ZERO_PROG_SERVICE_ID_INVALID //CHN_LONGYAN_ZERO_CHN_ID

/*һ����Ŀ�����Դ����ڶ��ٸ�������*/
#define CHN_MAX_SORT_COUNT 32


struct sortInfo_s
{
	BU08 sortName[APP_CHN_NAME_LEN];
	struct dbaData_s dataSort;
};

/*
������Ŀ�б�
*/
BU32 APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType);

/*
���õ�ǰ���Ž�Ŀ������
*/
BU32 APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType);
/*
��õ�ǰ��Ŀ������
*/
APP_CHN_GROUP_TYPE_E APP_ChnDatabase_API_GetCurGroupType();

/*
��õ�ǰ��������
*/
APP_CHN_AV_TYPE_E APP_ChnDatabase_API_GetCurChnType(void);

/*
����2����Ŀ
*/
BU32 APP_ChnDatabase_API_SwapChn(BU16 ProNo1, BU16 ProNo2);

/*
���ݱ�־ɾ����Ŀ
*/
BU32 APP_ChnDatabase_API_DelChnByTag(void);
/*
*��õ�ǰ��Ŀ���͵Ľ�Ŀ��
*/
BU16 APP_ChnDatabase_API_GetCurTypeChnNum(void);
/*
���ĳ�ֽ�Ŀ���͵Ľ�Ŀ��
*/
BU16 APP_ChnDatabase_API_GetTypeChnNum(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType);

/*
�õ�Ƶ���Ļ�����Ϣ
*/
BU32 APP_ChnDatabase_API_GetChnInfo(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 ChnNo, ChnDataBase_ChnInfo_S* ChnInfo);
/*
ͨ��serviceId��ȡ��Ŀ��Ϣ
*/
BU32 APP_ChnDatabase_API_GetChnInfoByService(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 ServiceId, ChnDataBase_ChnInfo_S* ChnInfo);
/*
���ý�Ŀ������Ϣ
*/
BU32 APP_ChnDatabase_API_SetChnInfo(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 ChnNo,const ChnDataBase_ChnInfo_S* ChnInfo);

/*
*/
BU32 APP_ChnDatabase_API_SetChnNameByServiceid(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 serviceId, BU08 *ChnName);
/*
��õ�ǰƵ��NO
*/
BU16 APP_ChnDatabase_API_GetCurChnNO(void);
/*
���õ�ǰ��Ŀ��
*/
BU32 APP_ChnDatabase_API_SetCurChnNO(BU32 curChnNO);

BU32 APP_ChnDatabase_API_ClearAllTypeChnNO(void);
/*
����Ŀ��ĸĶ����浽Flash�У�
*/
BU32 APP_ChnDatabase_API_UpDataToFlash(BU32 bat);
/*
��Flash���¶�ȡ��Ŀ��
*/
BU32 APP_ChnDatabase_API_LoadFromFlash(void);


/*
�õ���Ŀ��������
*/
BU32 APP_ChnDatabase_API_GetSortCount(void);
/*
�õ���������
*/
void APP_ChnDatabase_API_GetSortName(BU32 sortIndex, BU08 *strName);
/*
�õ������н�Ŀ����
*/
BU32 APP_ChnDatabase_API_GetSortChnCount(BU32 sortIndex);
/*
�õ������Ŀ��Ϣ(��Ų����б��еı��)
*/
BU08 APP_ChnDatabase_API_GetSortChnInfo(BU32 sortIndex, BU32 chnIndex, ChnDataBase_ChnInfo_S* chnInfo);
/*
�õ�0Ƶ����־
*/
BU16 APP_ChnDatabase_API_GetChn0Flag(void);
/*
����0Ƶ����־
*/
void APP_ChnDatabase_API_SetChn0Flag(BU16 ch0Flag);
/*
�ж��Ƿ���0Ƶ��
0: 0Ƶ������
1: 0Ƶ��������
*/
void APP_ChnDatabase_API_JudgeChn0(void);
/*
�������һ�ιۿ��Ľ�Ŀ��
*/
void APP_ChnDatabase_API_SetLastChnInfo(BU16 chn);
/*
�õ����һ�ιۿ��Ľ�Ŀ��Ϣ
*/
void APP_ChnDatabase_API_GetLastChnInfo(APP_CHN_AV_TYPE_E *avType, BU32 *chIndex);

#endif






