/**************************************************************

author:szhy

create time:080513

description:�ڴ��ļ�ϵͳ

**************************************************************/
#ifndef __MFS_API_H_
#define __MFS_API_H_
/**************************************************************
�������Ͷ���
**************************************************************/
typedef signed int MFS_HANDLE;
/**************************************************************
��ȡ�ڴ��ļ�ϵͳ�İ汾��
**************************************************************/
char * MFS_API_GetVersion(void);
/**************************************************************
��ʼ���ڴ��ļ�ϵͳ
**************************************************************/
int MFS_API_Init(unsigned int memSize);
/**************************************************************
�˳��ڴ��ļ�ϵͳ
**************************************************************/
int MFS_API_Exit(void);
/**************************************************************
���ļ�,����ֵ
**************************************************************/
#define FILE_OPEN_FAIL	-1
/**************************************************************
���ļ�,�������FILE_OPEN_FAIL��ʾ���ļ�ʧ��,����ɹ�
**************************************************************/
MFS_HANDLE MFS_API_Open(const char *pFileName, const char *pMode);
/**************************************************************
�ر��ļ�
**************************************************************/
int MFS_API_Close(MFS_HANDLE fileHandle);
/**************************************************************
ɾ���ļ�
**************************************************************/
int MFS_API_Remove(const char *pFileName);
/**************************************************************
��λ�ļ�ָ���������
**************************************************************/
#define MFS_SEEK_CUR         1
#define MFS_SEEK_END         2
#define MFS_SEEK_SET         0
/**************************************************************
��λ�ļ�ָ�뷵��ֵ
**************************************************************/
#define FILE_SEEK_FAIL		 -1
/**************************************************************
��λ�ļ�ָ��,�������FILE_SEEK_FAIL��ʾ��λ�ļ�ָ��ʧ��,���򷵻��ļ�ָ���λ��
**************************************************************/
signed int MFS_API_Seek(MFS_HANDLE fileHandle, signed int Offset, signed int Whence);
/**************************************************************
���ļ�����ֵ
**************************************************************/
#define FILE_READ_FAIL		 -1
/**************************************************************
���ļ�,�������FILE_READ_FAIL��ʾ���ļ�ʧ��,���򷵻ض�ȡ�����ݳ���
**************************************************************/
signed int MFS_API_Read(MFS_HANDLE fileHandle, void * data, signed int dataLen);
/**************************************************************
д�ļ�����ֵ
**************************************************************/
#define FILE_WRITE_FAIL		 -1
/**************************************************************
д�ļ�,�������FILE_WRITE_FAIL��ʾд�ļ�ʧ��,���򷵻�д�����ݳ���
**************************************************************/
signed int MFS_API_Write(MFS_HANDLE fileHandle,const void * data, signed int dataLen);
/**************************************************************
��ȡ�ļ���С����ֵ
**************************************************************/
#define FILE_GETSIZE_FAIL	 -1
/**************************************************************
��ȡ�ļ���С,�������FILE_GETSIZE_FAIL��ʾ��ȡ�ļ���Сʧ��,���򷵻��ļ���С
**************************************************************/
signed int MFS_API_GetSize(MFS_HANDLE fileHandle);
/**************************************************************
�ڴ��ļ�ϵͳ����ֲ�ӿ�
**************************************************************/
typedef struct _tagMFS_Interface_s
{
	void * (*MFS_API_Malloc)(unsigned int size);
	void (*MFS_API_Free)(void * pointer);
	void (*MFS_API_Printf)(char * string);
} MFS_Interface_s;
typedef MFS_Interface_s * MFS_Interface_sp;
/**************************************************************
ע���ڴ��ļ�ϵͳ����ֲ�ӿ�
**************************************************************/
void MFS_API_RegisterInterface(MFS_Interface_sp pInterface);
#endif
