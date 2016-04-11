#ifndef _APP_CHNDATABASE_H_
#define _APP_CHNDATABASE_H_

#define CHN_ZERO_PROG_SERVICE_ID_INVALID 0x1A2B3C4D
#define CHN_LONGYAN_ZERO_CHN_ID 1207
#define CHN_ZERO_PROG_SERVICE_ID 2003//CHN_ZERO_PROG_SERVICE_ID_INVALID //CHN_LONGYAN_ZERO_CHN_ID

/*一个节目最多可以存在于多少个分类中*/
#define CHN_MAX_SORT_COUNT 32


struct sortInfo_s
{
	BU08 sortName[APP_CHN_NAME_LEN];
	struct dbaData_s dataSort;
};

/*
创建节目列表
*/
BU32 APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType);

/*
设置当前播放节目的类型
*/
BU32 APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType);
/*
获得当前节目组类型
*/
APP_CHN_GROUP_TYPE_E APP_ChnDatabase_API_GetCurGroupType();

/*
获得当前播放类型
*/
APP_CHN_AV_TYPE_E APP_ChnDatabase_API_GetCurChnType(void);

/*
调换2个节目
*/
BU32 APP_ChnDatabase_API_SwapChn(BU16 ProNo1, BU16 ProNo2);

/*
根据标志删除节目
*/
BU32 APP_ChnDatabase_API_DelChnByTag(void);
/*
*获得当前节目类型的节目数
*/
BU16 APP_ChnDatabase_API_GetCurTypeChnNum(void);
/*
获得某种节目类型的节目数
*/
BU16 APP_ChnDatabase_API_GetTypeChnNum(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType);

/*
得到频道的基本信息
*/
BU32 APP_ChnDatabase_API_GetChnInfo(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 ChnNo, ChnDataBase_ChnInfo_S* ChnInfo);
/*
通过serviceId获取节目信息
*/
BU32 APP_ChnDatabase_API_GetChnInfoByService(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 ServiceId, ChnDataBase_ChnInfo_S* ChnInfo);
/*
设置节目基本信息
*/
BU32 APP_ChnDatabase_API_SetChnInfo(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 ChnNo,const ChnDataBase_ChnInfo_S* ChnInfo);

/*
*/
BU32 APP_ChnDatabase_API_SetChnNameByServiceid(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 serviceId, BU08 *ChnName);
/*
获得当前频道NO
*/
BU16 APP_ChnDatabase_API_GetCurChnNO(void);
/*
设置当前节目号
*/
BU32 APP_ChnDatabase_API_SetCurChnNO(BU32 curChnNO);

BU32 APP_ChnDatabase_API_ClearAllTypeChnNO(void);
/*
将节目库的改动保存到Flash中，
*/
BU32 APP_ChnDatabase_API_UpDataToFlash(BU32 bat);
/*
从Flash重新读取节目库
*/
BU32 APP_ChnDatabase_API_LoadFromFlash(void);


/*
得到节目分类数量
*/
BU32 APP_ChnDatabase_API_GetSortCount(void);
/*
得到分类名称
*/
void APP_ChnDatabase_API_GetSortName(BU32 sortIndex, BU08 *strName);
/*
得到分类中节目数量
*/
BU32 APP_ChnDatabase_API_GetSortChnCount(BU32 sortIndex);
/*
得到分类节目信息(编号不是列表中的编号)
*/
BU08 APP_ChnDatabase_API_GetSortChnInfo(BU32 sortIndex, BU32 chnIndex, ChnDataBase_ChnInfo_S* chnInfo);
/*
得到0频道标志
*/
BU16 APP_ChnDatabase_API_GetChn0Flag(void);
/*
设置0频道标志
*/
void APP_ChnDatabase_API_SetChn0Flag(BU16 ch0Flag);
/*
判断是否有0频道
0: 0频道存在
1: 0频道不存在
*/
void APP_ChnDatabase_API_JudgeChn0(void);
/*
设置最后一次观看的节目号
*/
void APP_ChnDatabase_API_SetLastChnInfo(BU16 chn);
/*
得到最后一次观看的节目信息
*/
void APP_ChnDatabase_API_GetLastChnInfo(APP_CHN_AV_TYPE_E *avType, BU32 *chIndex);

#endif






