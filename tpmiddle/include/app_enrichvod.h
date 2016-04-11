#ifndef _H_APP_ENRICHVOD_H_
#define _H_APP_ENRICHVOD_H_
typedef struct _EnrichVod_DateTime_S
{
	BU16 year;
	BU08 month;
	BU08 date;
	BU16 hour;
	BU08 minute;
	BU08 second;

}EnrichVod_DateTime_S;
typedef enum
{
	APP_EnrichVod_Error_PlayStart = 0,
	APP_EnrichVod_Error_PlayEnd,
	APP_EnrichVod_Error_PlayReach,
	APP_EnrichVod_Error_PlayNextError,
	APP_EnrichVod_Error_NetUnConnect_Error,
	APP_EnrichVod_Error_NetConnect_Error,
	APP_EnrichVod_Error_PlayError,
	APP_EnrichVod_Error_None,
	
}APP_EnrichVod_Error_Type;

typedef enum
{
	APP_EnrichVod_ATTR_PrevProg_TYPE,
	APP_EnrichVod_ATTR_CurProg_Type,
	APP_EnrichVod_ATTR_NextProg_Type,
	
}APP_EnrichVod_ATTR_TYPE_E;
#endif