/*======================================================================

 File Name:pse_205_api.h

 Description:PSE TP205 API header file

 History:

======================================================================*/
#ifndef _PSE_205_API_H_
#define _PSE_205_API_H_

/*------------------------------------------------------------------
	Enum
----------------------------------------------------------------------*/
typedef enum {
	PSE_205_ENU_PIO_DIR_IPT,
	PSE_205_ENU_PIO_DIR_OPT
}PSE_205_PioDir_e;

typedef enum {
	PSE_205_ENU_PIO_LEVEL_L=0,
	PSE_205_ENU_PIO_LEVEL_H=1
}PSE_205_PioLevel_e;

typedef enum {
	PSE_205_ENU_RESULT_FAIL      =0,
	PSE_205_ENU_RESULT_OK        =1,
	PSE_205_ENU_RESULT_NOT_VERIFY=3
}PSE_205_Result_e;
/*----------------------------------------------------------------------
	Struct
----------------------------------------------------------------------*/
typedef struct {
	void(*SetRset)(PSE_205_PioLevel_e);
	void(*SetData)(PSE_205_PioLevel_e);
	void(*SetClck)(PSE_205_PioLevel_e);
	PSE_205_PioLevel_e (*GetData)(void);
	void(*DatDirt)(PSE_205_PioDir_e);
	unsigned long(*SemInit)(unsigned long);
	void(*SemSigl)(unsigned long);
	void(*SemWait)(unsigned long);
	void(*msDelay)(unsigned long);
}PSE_205_IniPms_s;

typedef struct {
	unsigned long CpuFreqMHz;
	unsigned long msInterval;
}PSE_205_CfgPms_s;
/*----------------------------------------------------------------------
	Function Prototypes
----------------------------------------------------------------------*/
PSE_205_Result_e PSE_205_API_Reset(void);
PSE_205_Result_e PSE_205_API_SetMainArea(unsigned char StartAddress,unsigned char* Data,unsigned long Length);
PSE_205_Result_e PSE_205_API_GetMainArea(unsigned char StartAddress,unsigned char* Data,unsigned long Length);
void PSE_205_API_WritCfgPms(PSE_205_CfgPms_s* CfgPms);
PSE_205_Result_e PSE_205_API_InitCpiPms(PSE_205_IniPms_s* IniPms);
void PSE_205_API_GetVersion(unsigned long* VersionInfo);

#endif


