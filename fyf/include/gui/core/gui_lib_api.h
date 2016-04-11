
#ifndef __GUI_LIB_API_H_
#define __GUI_LIB_API_H_


#define GUIMax(x, y)           ((x > y)?x:y)
#define GUIMin(x, y)           ((x < y)?x:y)
#define swap(x,y)			{(x)^=(y);(y)^=(x);(x)^=(y);}

#define GUI_API_Max(x, y)       ((x > y)?x:y)
#define GUI_API_Min(x, y)       ((x < y)?x:y)	
#define GUI_API_Swap(x,y)			{(x)^=(y);(y)^=(x);(x)^=(y);}
#define GUI_API_Abs(v)			((v>0) ? v : -v)
#define GUI_API_Random(num)			(GUI_API_Rand()%(num))

void GUI_API_SetFixedMemony(void * addr,BU32 size);
void * GUI_API_MallocFixed(BU32 size);
BU32 GUI_API_GetUseFixedMemonySize(void);

void * GUI_API_Malloc(BU32 size);
void GUI_API_Free(void * lpmem);

void GUI_API_Memset(void *lpmem,BU08 value,BU32 size);
void GUI_API_Memcpy(void *lpDsr,void * lpSrc,BU32 size);
BU08 GUI_API_Toupper(BU08 value);
BU32 GUI_API_Strlen(const PU08 s);
void GUI_API_TimerDelay(BU32 ms);
PU08 GUI_API_Strcpy(PU08 dest, const PU08 src);
BU32 GUI_API_Strcmp(PU08 str1,PU08 str2);
void GUI_API_Printf(PU08 fmt);
void GUI_API_AddHexStr(BU32 value, BU32 len, PU08 ps);
void GUI_API_AddDecStr(BU32 value, BU32 len, PU08 ps);
BU32 GUI_API_GetDecLen(BU32 value);
BU32 GUI_API_AddStrDec(PU08 ps);
BU32 GUI_API_Rand(void);
void GUI_API_InitRandom(BU32 num);

#endif


