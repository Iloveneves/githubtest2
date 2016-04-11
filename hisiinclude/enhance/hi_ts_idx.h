/******************************************************************************
Copyright (C), 2004-2010, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_ts_idx.h
Version        : Initial 
Author         : Hisilicon multimedia software group
Created       : 2009-02-23
Last Modified :
Description   : Hisilicon ASF I frame Index file
Function List :
History        :
* Version   Date              Author                  DefectNum    Description
* 
******************************************************************************/
#ifndef __HI_TS_IDX_H__
#define __HI_TS_IDX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct hiTSIDX_SEND_INFO
{
    HI_U32 u32SendInfo0;
    HI_U32 u32SendInfo1;
    HI_U32 u32SendInfo2;
    HI_U32 u32SendInfo3;
}TSIDX_SEND_INFO;

typedef enum
{
    TSIDX_TIME,
    TSIDX_OFFSET,
    TSIDX_FRAMENO,    
}TSIDX_POS_TYPE;

typedef struct
{
    HI_U64 u64Offset;
    HI_U32 u32Time;
    HI_U32 u32FrameNo;    
}TSIDX_POS_INFO;


typedef struct
{
    HI_U32 u32HddBufRemain;
    HI_U32 u32AudPesRemain;
    HI_U32 u32VidPesRemain;    
}TSIDX_BUF_INFO;


typedef enum
{
    TSIDX_FLUSH_HDD = 1,
    TSIDX_FLUSH_AUD_PES = 2,
    TSIDX_FLUSH_VID_PES = 4,        
}TSIDX_FLUSH_MODE;


//����:���������ļ���
//����1:ԭʼWMV�ļ���
//����2:�����ļ���
//����3:������������ƵPID
HI_S32 HI_UNF_TSIDX_CreateIdxFile(HI_U8 *SourceFile, HI_U8 *TargetFile, HI_U32 VidPid);

//����:�������ļ���ע�ⲻҪֱ����fopen�ӿڡ�
//����:�����ļ���
FILE *HI_UNF_TSIDX_OpenIdxFile(HI_U8 *FileName);

//����:�ر������ļ���ע�ⲻҪֱ����fclose�ӿڡ�
//����:ͨ��HI_UNF_ASF_OpenIdxFile���ص������ļ����
HI_S32 HI_UNF_TSIDX_CloseIdxFile(FILE *pIdx);


//����:��ȡ������Ϣ�������������ŵĳ���ʱ�䣬��֡����
//����1:�����ļ����
//����2:�ļ�������ʱ��
//����3:֡����
HI_S32 HI_UNF_TSIDX_GetIdxInfo(FILE *pIdx, HI_U32 *DuartionInMS, HI_U32 *FrmNum);


//����:��ȡ�ļ�λ����Ϣ��ʵ��offset��time��frameno֮���ת��
//����1:�����ļ����
//����2:λ����Ϣ��������offset��time��frameno��ͨ��Type��ʶ
//����3:pos����
//����3:��ȡ��λ����Ϣ������offset��time��frameno
HI_S32 HI_UNF_TSIDX_GetPosInfo(FILE *pIdx, HI_U64 Pos, TSIDX_POS_TYPE Type, TSIDX_POS_INFO *PosInfo);


//����:ͨ��֡�Ż�ȡλ����Ϣ
//����1:�����ļ����
//����2:�Ƿ�Ѱ���ڽ�I֡���ؼ�����4������ҪѰ���ڽ�I֡
//����3:֡��
//����4:λ��
//����5:�������ݳߴ�
//����6:������Ϣ,��Ҫ����HI_UNF_TSIDX_PrepareToSend
HI_S32 HI_UNF_TSIDX_GetOffsetByFrameNo(FILE *pIdx, HI_BOOL IsKeyFrame, HI_U32 FrameNo, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);


//����:����video����ʾ�ٶ�
//����1:��ʾ�ٶ�
HI_S32 HI_UNF_TSIDX_VidSetSpeed(HI_S32 Speed);



//����:send֮ǰ��Ԥ����
//����1:Hdd buffer��ַ
//����2:send�����ݳ���
//����3:������Ϣ���ɽӿ�HI_UNF_TSIDX_GetOffsetByFrameNo�ṩ
HI_S32 HI_UNF_TSIDX_PrepareToSend(HI_U8* pTsAddr, HI_U32 ReadLen, TSIDX_SEND_INFO *SendInfo);



//����:��ȡbuffer����
//����1:buff�����ṹ��ָ��
HI_S32 HI_UNF_TSIDX_GetBufInfo(TSIDX_BUF_INFO *pBufInfo);


//����:��buffer
//����1:���ģʽ
HI_S32 HI_UNF_TSIDX_FlushBuf(TSIDX_FLUSH_MODE FlushMode);


//���������ƽӿ�
HI_S32 HI_UNF_TSIDX_AudPause();
HI_S32 HI_UNF_TSIDX_AudResume();


HI_S32 HI_UNF_TSIDX_VidPause();
HI_S32 HI_UNF_TSIDX_VidResume();




#if 0
//����:ָ��ʱ�䣬��ȡ�ؼ�֡(I֡)
//����1:�����ļ����
//����2:ָ���Ĳ���ʱ�̡���λ���롣
//����3:ʵ�ʶ�Ӧ�ؼ�֡�Ĳ���ʱ�̡���λ���롣
//����4:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����5:�ؼ�֡��һ�����ݵĴ�С
//����6:����TS������ؼ���Ϣ
HI_S32 HI_UNF_TSIDX_GetKeyFrmByTime(FILE *pIdx, HI_U32 SeekTimeInMS, HI_U32 *PlayTimeInMS, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);

//����:��ȡ��һ���ؼ�֡�����ڿ���
//����1:�����ļ����
//����2:ʵ�ʶ�Ӧ�ؼ�֡�Ĳ���ʱ�̡���λ���롣
//����3:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����4:�ؼ�֡��TS�ļ��еĴ�С
//����5:����TS������ؼ���Ϣ
HI_S32 HI_UNF_TSIDX_GetPrevKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);

//����:��ȡ��һ���ؼ�֡�����ڿ��
//����1:�����ļ����
//����2:ʵ�ʶ�Ӧ�ؼ�֡�Ĳ���ʱ�̡���λ���롣
//����3:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����4:�ؼ�֡��TS�ļ��еĴ�С
//����5:����TS������ؼ���Ϣ
HI_S32 HI_UNF_TSIDX_GetNextKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);

//����:��ȡ�ؼ�֡�ĺ������ݿ顣
//����1:�����ļ����
//����2:�ؼ�֡��ԭʼ�ļ��е�ƫ��
//����3:�ؼ�֡��TS�ļ��еĴ�С
HI_S32 HI_UNF_TSIDX_GetKeyFrmNextBlk(FILE *pIdx, HI_U64 *Offset, HI_U32 *Size);


/*
�������ܣ������ļ�ƫ�Ƶ�ַoffset,��ѯ��Ӧ��ʱ�䡣
pIdx�� ��������������ļ������
Offset���ļ����������ʼλ�õ�ƫ������
TimeInMs: ��λ���룬���ز���
*/
HI_S32 HI_UNF_TSIDX_GetCurrentTime(FILE *pIdx, HI_U64 Offset, HI_U32 *PlayTimeInMS);

/*
�������ܣ�ͨ�������ļ���������ת��ʱ��㣬���ض�Ӧ�ļ���ƫ�Ƶ�ַ��
�������ĳʱ�̲���Ӧ�ó���, ����ʹ�ýӿ�: HI_UNF_TSIDX_GetKeyFrmByTime
indexfile_fd�� ��������������ļ����
TimeInMs���������������ʱ�䣬��λ�Ǻ���
Offset �����ز�������Ӧ�ļ��е�ƫ�Ƶ�ַ�����ڲ��Ŷ�λ֮�ã�fseek��
*/
HI_S32 HI_UNF_TSIDX_GetFileOffset(FILE *pIdx, HI_U32 PlayTimeInMS, HI_U64 *Offset);


/*
�������ܣ���ȡӰƬ���ŵĳ���ʱ�䡣
pIdx�� ��������������ļ����
duration�����ز���������ʱ�䣬��λ����
*/
//HI_S32 HI_UNF_TSIDX_GetFileDuration(FILE *pIdx, HI_U32 *duration);


/*
�������ܣ����ÿ�����˱��١�
pIdx�� ��������������ļ����
Speed��������������ŵı���: -32 -16 -8 -4 -2-0 2 -4 8 16 32
*/
HI_S32 HI_UNF_TSIDX_SetFileFrameSpeed(FILE *pIdx,HI_S32 Speed, HI_U32 PlayTimeInMS);


/*
�������ܣ���ѯ������˱��١�
indexfile_fd�� ��������������ļ����
playtimems��������������ŵı���: -32 -16 -8 -4 -2-0 2 -4 8 16 32
*/
HI_S32 HI_UNF_TSIDX_GetFileFrameSpeed(FILE *pIdx,HI_S32 *Speed);

#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TS_IDX_H__ */


