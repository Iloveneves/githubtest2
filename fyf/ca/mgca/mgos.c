#include "mgdef.h"
#include "../../inc/fyf_def.h"
#include "../../os/fyf_semaphore.h"
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static BU32 MG_semaphore;
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Create_Semaphore()
{
	MG_semaphore = FYF_API_semaphore_create("MGS",1,FYF_THREAD_WAIT_FIFO);
	if(FYF_NULL == MG_semaphore)
	{
		return MG_FAILING;
	}
	else
	{
		return MG_SUCCESS;
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Obtain_Semaphore()
{
	BU32 ret;
	ret = FYF_API_semaphore_wait(MG_semaphore,FYF_NO_WAIT);
	if(FYF_ERR == ret)
	{
		return MG_FAILING;
	}
	else
	{
		return MG_SUCCESS;
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Release_Semaphore()
{
	BU32 ret;
	ret = FYF_API_semaphore_release(MG_semaphore);
	if(FYF_ERR == ret)
	{
		return MG_FAILING;
	}
	else
	{
		return MG_SUCCESS;
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Delete_Semaphore()
{
	BU32 ret;
	ret = FYF_API_semaphore_destroy(MG_semaphore);
	if(FYF_ERR == ret)
	{
		return MG_FAILING;
	}
	else
	{
		return MG_SUCCESS;
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Enable_Scheduler(MG_VOID)
{
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Disable_Scheduler(MG_VOID)
{
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U32 MG_STBPort_Get_OSTime(MG_VOID)
{
	return FYF_API_time_ms();

}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U32 MG_STBPort_Add_OSTime(MG_U32 time1,MG_U32 time2)
{
	return (time1+time2);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U32 MG_STBPort_Minus_OSTime(MG_U32 time1,MG_U32 time2)
{
	MG_U32 ctime3;

	if(time2 <= time1)
		ctime3 = time1 - time2;
	else
		ctime3 =  time2 - time1;

	return (ctime3);	
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U32 MG_STBPort_Get_OSTime_Ticks(MG_VOID)
{
	return 1000;
}

