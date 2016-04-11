#ifndef __HI_FILE_CIPHER_H__
#define __HI_FILE_CIPHER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_ecs_api.h"


/************************************************以下为加解密接口***********************************************************************************************/
/*
HI_S32 HI_CIPHER_EncryptFile(HI_U32 u32Type,HI_U8* u8File,HI_U8* u8Key,HI_U8* u8OutFile);
接口功能:
     对输入文件进行加密
入口参数:
     u32Type:表示加密方式，u32Type =0 表示覆盖方式加密，即加密后的文件覆盖原来的文件，此种情况u8OutFile名字可为空
                                                           u32Type =1表示新生成加密文件，新生成的加密文件名字由u8OutFile参数输入
     u8File:表示待加密文件名字
     u8Key:密钥，由用户输入要求长度必须为16字节
     u8OutFile: 生成的加密后文件的名字
     以上参数都为输入参数
返回值:
    成功返回0，失败返回-1

*/
HI_S32 HI_CIPHER_EncryptFile(HI_U32 u32Type,HI_U8* u8File,HI_U8* u8Key,HI_U8* u8OutFile);

/*
HI_S32 HI_CIPHER_DecryptOpenFile(HI_U8* u8File,HI_U8* u8Key,HI_U32** handle);
接口功能:
     验证输入密钥是否正确，如果正确返回待解密文件的句柄，否则删除待解密文件
入口参数:
     u8File:  表示待解密文件名字，输入参数
     u8Key:  密钥，由用户输入要求长度必须为16字节，输入参数
     handle: 待解密文件的句柄，输出参数
返回值:
    成功返回0，失败返回-1
注意:
    解密密钥和加密密钥应该相同均为16字节，否则会返回错误，并删除待解密文件
*/
HI_S32 HI_CIPHER_DecryptOpenFile(HI_U8* u8File,HI_U8* u8Key,HI_U32** handle);

/*
HI_S32 HI_CIPHER_GetMediaData(HI_U32* handle,HI_U64 pos,HI_U32 size,HI_U8* u8Buffer);
接口功能:
     对输入的数据进行解密，并将解密后的值拷贝到用户指定的缓冲区中
入口参数:
     handle:经过密钥验证后， 待解密文件的句柄，输入参数
     pos:起始地址，表示用户想从文件的哪个具体位置开始解密，输入参数
     size: 要解密的数据的长度，输入参数
     u8Buffer: 解密后将解密数据拷贝到缓冲区的起始地址，该缓冲区由用户分配，输入参数
返回值:
    成功返回0，失败返回-1
注意:
   pos + size 的大小不能超过文件长度，否则会报错。
*/
HI_S32 HI_CIPHER_GetMediaData(HI_U32* handle,HI_U64 pos,HI_U32 size,HI_U8* u8Buffer);

/*
HI_S32 HI_CIPHER_DecryptCloseFile(HI_U32* handle);
接口功能:
      关闭已解密完的文件
入口参数:
     handle: 已解密完毕的文件的句柄
返回值:
    成功返回0，失败返回-1
注意:
    解密完后，一定要调用 此函数关闭文件
*/
HI_S32 HI_CIPHER_DecryptCloseFile(HI_U32* handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__SOYEA_PUSHVOD_H*/

