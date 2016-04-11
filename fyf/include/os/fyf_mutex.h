#ifndef __FYF_MUTEX_H__
#define __FYF_MUTEX_H__
typedef struct 
{ 
	BU32 (*FYF_CPI_mutex_create)(void); 
	BS32 (*FYF_CPI_mutex_delete)(BU32 handle); 
	BS32 (*FYF_CPI_mutex_lock)(BU32 handle,BU32 timeout); 
	BS32 (*FYF_CPI_mutex_unlock)(BU32 handle);
}FYF_MUTEX_PORTING_s;
typedef FYF_MUTEX_PORTING_s* FYF_MUTEX_PORTING_sp;
void FYF_API_mutex_register(FYF_MUTEX_PORTING_sp porting);
BU32 FYF_API_mutex_create(void); 
BS32 FYF_API_mutex_delete(BU32 handle); 
BS32 FYF_API_mutex_lock(BU32 handle,BU32 timeout); 
BS32 FYF_API_mutex_unlock(BU32 handle);
#endif






