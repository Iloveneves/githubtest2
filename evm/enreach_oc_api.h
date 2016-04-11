/*------------------------------------------------------*/
/*     File     : dsmcc_oc_api.h                                                                */
/*     Date     : 2005/9/1                                                                 */
/*     Purpose :                                                                                */
/*------------------------------------------------------*/
 
#ifndef _DSMCC_OC_API_H_
#define _DSMCC_OC_API_H_
#include "enreach_oc_type.h"

typedef UINT32 DSMCC_OC_HANDLE;

typedef enum
{
    DSMCC_OC_OK 			= 0x01,	 		
    DSMCC_OC_ERROR 		=   0x02,
    DSMCC_OC_NO_MORE_CAROUSEL 			= 0x03,
    DSMCC_OC_DEMUX_ALLOC_CHAN_FAILED 	= 0x04,
    DSMCC_OC_DEMUX_ALLOC_FILTER_FAILED 	=0x05,
    DSMCC_OC_DEMUX_SET_FILTER_FAILED  	=0x06,	
    DSMCC_OC_DEMUX_ENABLE_FAILED 		= 0x07,
    DSMCC_OC_OS_CREATE_SEMAPHORE_FAILED = 0x08,
    DSMCC_OC_INVALID_HANDLE 	= 0x09,
    DSMCC_OC_TIMEOUT 			= 0x0a,
    DSMCC_OC_PARSE_FAILED 		= 0x0b ,
    DSMCC_OC_NOT_FOUND 		= 0x0c ,
    DSMCC_OC_MODULE_BUF_NOT_ENOUGH 	= 0x0d,
    DSMCC_OC_MODULE_REV_NOT_FINISH 		= 0x0e,
    DSMCC_OC_MEM_ALLOC_FAILED 	= 0x0f,
    DSMCC_OC_FILESYS_NOT_CREAT 	= 0x10,
    DSMCC_FL_NOT_FOUND 			= 0x11,
    DSMCC_SERVICEID_NOT_FOUND 	= 0x12 ,
    DSMCC_PMTPID_NOT_FOUND 		= 0x13,
    DSMCC_PARSE_PMT_FAILED 		= 0x14,
    DSMCC_PARSE_AIT_FAILED 		= 0x15,
    DSMCC_NOT_HTML_APP 			= 0x16,
    DSMCC_INVALID_PID 				= 0x17,
    DSMCC_OC_DEMUX_SET_CALLBACK_FAILED  = 0x18,
    DSMCC_OC_DEMUX_SET_PID_FAILED  		= 0x19,
    DSMCC_OC_INVALID_MODID 				= 0x1a,
    DSMCC_OC_UPDATE_TO
}DSMCC_OC_STATUES;

DSMCC_OC_STATUES DSMCC_OC_LowLevelInit(void*p, INT32 iSize);
DSMCC_OC_STATUES DSMCC_OC_Open(DSMCC_OC_HANDLE *CarouselHandle, UINT32 ulDSITimeOutInMs);
DSMCC_OC_STATUES DSMCC_OC_Close(DSMCC_OC_HANDLE CarouselHandle);
char DSMCC_FindWebEntry(unsigned char ** pWebEntry);
UINT8 DsmccOcSetCurServiceID(OC_SERVICE_INFO);
char* EnReach_OC_GetVersion(void); 
#endif


