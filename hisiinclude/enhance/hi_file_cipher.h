#ifndef __HI_FILE_CIPHER_H__
#define __HI_FILE_CIPHER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_ecs_api.h"


/************************************************����Ϊ�ӽ��ܽӿ�***********************************************************************************************/
/*
HI_S32 HI_CIPHER_EncryptFile(HI_U32 u32Type,HI_U8* u8File,HI_U8* u8Key,HI_U8* u8OutFile);
�ӿڹ���:
     �������ļ����м���
��ڲ���:
     u32Type:��ʾ���ܷ�ʽ��u32Type =0 ��ʾ���Ƿ�ʽ���ܣ������ܺ���ļ�����ԭ�����ļ����������u8OutFile���ֿ�Ϊ��
                                                           u32Type =1��ʾ�����ɼ����ļ��������ɵļ����ļ�������u8OutFile��������
     u8File:��ʾ�������ļ�����
     u8Key:��Կ�����û�����Ҫ�󳤶ȱ���Ϊ16�ֽ�
     u8OutFile: ���ɵļ��ܺ��ļ�������
     ���ϲ�����Ϊ�������
����ֵ:
    �ɹ�����0��ʧ�ܷ���-1

*/
HI_S32 HI_CIPHER_EncryptFile(HI_U32 u32Type,HI_U8* u8File,HI_U8* u8Key,HI_U8* u8OutFile);

/*
HI_S32 HI_CIPHER_DecryptOpenFile(HI_U8* u8File,HI_U8* u8Key,HI_U32** handle);
�ӿڹ���:
     ��֤������Կ�Ƿ���ȷ�������ȷ���ش������ļ��ľ��������ɾ���������ļ�
��ڲ���:
     u8File:  ��ʾ�������ļ����֣��������
     u8Key:  ��Կ�����û�����Ҫ�󳤶ȱ���Ϊ16�ֽڣ��������
     handle: �������ļ��ľ�����������
����ֵ:
    �ɹ�����0��ʧ�ܷ���-1
ע��:
    ������Կ�ͼ�����ԿӦ����ͬ��Ϊ16�ֽڣ�����᷵�ش��󣬲�ɾ���������ļ�
*/
HI_S32 HI_CIPHER_DecryptOpenFile(HI_U8* u8File,HI_U8* u8Key,HI_U32** handle);

/*
HI_S32 HI_CIPHER_GetMediaData(HI_U32* handle,HI_U64 pos,HI_U32 size,HI_U8* u8Buffer);
�ӿڹ���:
     ����������ݽ��н��ܣ��������ܺ��ֵ�������û�ָ���Ļ�������
��ڲ���:
     handle:������Կ��֤�� �������ļ��ľ�����������
     pos:��ʼ��ַ����ʾ�û�����ļ����ĸ�����λ�ÿ�ʼ���ܣ��������
     size: Ҫ���ܵ����ݵĳ��ȣ��������
     u8Buffer: ���ܺ󽫽������ݿ���������������ʼ��ַ���û��������û����䣬�������
����ֵ:
    �ɹ�����0��ʧ�ܷ���-1
ע��:
   pos + size �Ĵ�С���ܳ����ļ����ȣ�����ᱨ��
*/
HI_S32 HI_CIPHER_GetMediaData(HI_U32* handle,HI_U64 pos,HI_U32 size,HI_U8* u8Buffer);

/*
HI_S32 HI_CIPHER_DecryptCloseFile(HI_U32* handle);
�ӿڹ���:
      �ر��ѽ�������ļ�
��ڲ���:
     handle: �ѽ�����ϵ��ļ��ľ��
����ֵ:
    �ɹ�����0��ʧ�ܷ���-1
ע��:
    �������һ��Ҫ���� �˺����ر��ļ�
*/
HI_S32 HI_CIPHER_DecryptCloseFile(HI_U32* handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__SOYEA_PUSHVOD_H*/

