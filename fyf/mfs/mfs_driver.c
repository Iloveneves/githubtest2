/*
author     :szhy
create date:2008-12-03
*/
#include "fyf_def.h"
#include "mfs/mfs_api.h"
#include "sys/fyf_system.h"
/******************************************************************************/
/*Description:                                                                */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     :                                                                */
/******************************************************************************/
void * MFS_Malloc(unsigned int size)
{
	return FYF_API_malloc(size);
}
/******************************************************************************/
/*Description:                                                                */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     :                                                                */
/******************************************************************************/
void MFS_Free(void * pointer)
{
	FYF_API_free(pointer);
}
/******************************************************************************/
/*Description:                                                                */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     :                                                                */
/******************************************************************************/
void MFS_Printf(char * string)
{
	printf(string);
}
/******************************************************************************/
/*Description:                                                                */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     :                                                                */
/******************************************************************************/
static MFS_Interface_s pInterface= 
{
	MFS_Malloc,
	MFS_Free,
	MFS_Printf
};
void FYF_API_mfs_test(void)
{
	MFS_HANDLE handle;
	int readNum;
	char buf[10]="hello";
	
	handle = MFS_API_Open("file.txt","w");
	MFS_API_Write(handle,buf,5);
	memset(buf,0,10);
	MFS_API_Seek(handle,0,MFS_SEEK_SET);
	readNum = MFS_API_Read(handle,buf,10);
	buf[readNum]=0;
	printf("mfs %s\n", buf);
	MFS_API_Close(handle);
}
/******************************************************************************/
/*Description: init mfs module                                                */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     :                                                                */
/******************************************************************************/
void FYF_API_mfs_init(void)
{
	/* register mfs module */
	MFS_API_RegisterInterface(&pInterface);

	/* init mfs size */
	MFS_API_Init(1024*100);
	//FYF_API_mfs_test();
}



