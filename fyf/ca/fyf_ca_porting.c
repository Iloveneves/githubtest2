/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
#include "fyf_def.h"
#include "os/fyf_thread.h"
#include "os/fyf_queue.h"
#include "ca/fyf_ca.h"

/*-------------------------------------------------------------------------------

-------------------------------------------------------------------------------*/
typedef void (*FYF_CA_notify)(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2);
static FYF_CA_notify fyf_ca_notify;
void FYF_API_CA_notify(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2);
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CPI_CaInit_porting(void)
{
#if defined (MG_CA)
	FYF_CA_MG_API_Init();
#elif defined (DVT_CA)
	FYF_CA_DVT_API_Init();
#elif defined (CD_CA)
	FYF_CA_CD_API_Init ();
#elif defined (TF_CA)
	FYF_CA_TF_API_Init ();
#endif
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CPI_SetCurEmmInfo_porting(int emmpid,int caid)
{
#if defined (MG_CA)
	FYF_MG_SetCurEmmInfo(emmpid,caid);
#elif defined (DVT_CA)
	FYF_DVT_SetCurEmmInfo(emmpid,caid);	
#elif defined (CD_CA)
	FYF_CD_SetCurEmmInfo (emmpid, caid);
#elif defined (TF_CA)
	FYF_TF_SetCurEmmInfo (emmpid, caid);
#endif
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CPI_SetCurEcmInfo_porting(const FYF_Ecm_Info_s * pServiceInfo)
{
#if defined (MG_CA)
	FYF_MG_SetCurEcmInfo(pServiceInfo);
#elif defined (DVT_CA)
	FYF_DVT_SetCurEcmInfo(pServiceInfo);
#elif defined (CD_CA)
	FYF_CD_SetCurEcmInfo (pServiceInfo);
#elif defined (TF_CA)
	FYF_TF_SetCurEcmInfo (pServiceInfo);
#endif
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CPI_SetProgramChange_porting(void)
{
#if defined (MG_CA)
	MG_Clean_LastPNo_Info();
#endif	
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
BS32 FYF_CPI_ca_set_value_porting(FYF_CA_SV_e type,BU32 para1,BU32 para2)
{
#if defined (MG_CA)
	return FYF_API_mgca_set_value(type, para1, para2);		
#elif defined (DVT_CA)
	return FYF_API_dvtca_set_value(type, para1, para2);
#elif defined (CD_CA)
    return FYF_API_cdca_set_value(type,para1,para2);
#elif defined (TF_CA)
    return FYF_API_tfca_set_value(type,para1,para2);
#endif
	return FYF_ERR;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
BS32 FYF_CPI_ca_get_value_porting(FYF_CA_SV_e type,BU32 *para1,BU32 *para2)
{
#if defined (MG_CA)
	return FYF_API_mgca_get_value(type,para1,para2);
#elif defined (DVT_CA)
    return FYF_API_dvtca_get_value(type,para1,para2);	
#elif defined (CD_CA)
    return FYF_API_cdca_get_value(type,para1,para2);
#elif defined (TF_CA)
    return FYF_API_tfca_get_value(type,para1,para2);
#endif
	return FYF_ERR;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CPI_ca_register_notify_porting(void (*notify)(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2))
{
	fyf_ca_notify = notify;			
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_IPI_ca_notify(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2)
{
	if(fyf_ca_notify)
	{
		fyf_ca_notify(type,para1,para2);
	}
}
FYF_CA_PORTING_s FYF_CA_porting=
{
	FYF_CPI_CaInit_porting,
	FYF_CPI_SetCurEmmInfo_porting,
	FYF_CPI_SetCurEcmInfo_porting,
	FYF_CPI_SetProgramChange_porting,
	FYF_CPI_ca_set_value_porting,
	FYF_CPI_ca_get_value_porting,
	FYF_CPI_ca_register_notify_porting
};

void FYF_API_CA_Init()
{
	FYF_API_ca_register(&FYF_CA_porting);
	FYF_API_ca_register_notify(FYF_API_CA_notify);
}

