/******************************************************************************
Copyright (C), 2004-2010, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_asf_idx.h
Version        : Initial 
Author         : Hisilicon multimedia software group
Created       : 2009-02-23
Last Modified :
Description   : Hisilicon ASF I frame Index file
Function List :
History        :
* Version   Date              Author                  DefectNum    Description
* main\1    2009-02-23   HongGuo Qiu         NULL             Create this file.
******************************************************************************/
#ifndef __HI_ASF_IDX_H__
#define __HI_ASF_IDX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

//����:���������ļ���
//����1:ԭʼWMV�ļ���
//����2:�����ļ���
HI_S32 HI_UNF_ASF_CreateIdxFile(HI_U8 *SourceFile, HI_U8 *TargetFile);

//����:�������ļ���ע�ⲻҪֱ����fopen�ӿڡ�
//����:�����ļ���
FILE *HI_UNF_ASF_OpenIdxFile(HI_U8 *FileName);

//����:�ر������ļ���ע�ⲻҪֱ����fclose�ӿڡ�
//����:ͨ��HI_UNF_ASF_OpenIdxFile���ص������ļ����
HI_S32 HI_UNF_ASF_CloseIdxFile(FILE *pIdx);

//����:��ȡ������Ϣ�������������ŵĳ���ʱ�䣬�͹ؼ�֡����
//����1:�����ļ����
//����2:�ļ�������ʱ��
//����3:�ؼ�֡����
HI_S32 HI_UNF_ASF_GetIdxInfo(FILE *pIdx, HI_U32 *DuartionInMS, HI_U32 *KeyFrmNum);

//����:ָ��ʱ�䣬��ȡ�ؼ�֡
//����1:�����ļ����
//����2:ָ���Ĳ���ʱ�̡���λ���롣
//����3:ʵ�ʶ�Ӧ�ؼ�֡�Ĳ���ʱ�̡���λ���롣
//����4:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����5:�ؼ�֡��һ�����ݵĴ�С
//����6:�ؼ�֡�Ƿ��ȡ���������IfOKΪ1�������ؼ�֡���������IfOKΪ0���������к������ݿ顣
//ע��:�ؼ�֡��WMVԭʼ�ļ����Ǳ��ָ�����ɶεġ�����ֻ���ص�һ�����ݣ��������ݿ�ͨ��HI_UNF_ASF_GetKeyFrmNextBlk��ȡ��
HI_S32 HI_UNF_ASF_GetKeyFrmByTime(FILE *pIdx, HI_U32 SeekTimeInMS, HI_U32 *PlayTimeInMS, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

//����:��ȡ��һ���ؼ�֡�����ڿ���
//����1:�����ļ����
//����2:ʵ�ʶ�Ӧ�ؼ�֡�Ĳ���ʱ�̡���λ���롣
//����3:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����4:�ؼ�֡��һ�����ݵĴ�С
//����5:�ؼ�֡�Ƿ��ȡ������
HI_S32 HI_UNF_ASF_GetPrevKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

//����:��ȡ��һ���ؼ�֡�����ڿ��
//����1:�����ļ����
//����2:ʵ�ʶ�Ӧ�ؼ�֡�Ĳ���ʱ�̡���λ���롣
//����3:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����4:�ؼ�֡��һ�����ݵĴ�С
//����5:�ؼ�֡�Ƿ��ȡ������
HI_S32 HI_UNF_ASF_GetNextKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

//����:��ȡ�ؼ�֡�ĺ������ݿ顣
//����1:�����ļ����
//����2:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����3:�ؼ�֡�������ݵĴ�С
HI_S32 HI_UNF_ASF_GetKeyFrmNextBlk(FILE *pIdx, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_ASF_IDX_H__ */

