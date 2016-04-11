/*===============================================================================

 File Name:fyf_libc.h

 Author:

 Create Time:2008-12-22
 
 Description:

 History:

===============================================================================*/
void FYF_API_AddHexStr(BU32 value, BU32 len, PU08 ps);
void FYF_API_AddDecStr(BU32 value, BU32 len, PU08 ps);
BU32 FYF_API_AddStrDec(PU08 ps);
BU32 FYF_API_GetDecLen(BU32 value);
void FYF_API_Memset(void *lpmem,BU08 value,BU32 size);
void FYF_API_Memcpy(void *lpDsr,void * lpSrc,BU32 size);
BU08 FYF_API_Toupper(BU08 value);
BU32 FYF_API_Strlen(const PU08 s);
PU08 FYF_API_Strcpy(PU08 dest, const PU08 src);
BU32 FYF_API_Strcmp(PU08 str1,PU08 str2);
void FYF_API_InitRandom(BU32 num);
BU32 FYF_API_Rand(void);

