#ifndef _APP_PARSERTOT_H_
#define _APP_PARSERTOT_H_

typedef struct _TOT_Content_s
{
	BU32 week;
	BU32 hour;
	BU32 min;
    BU32 sec;
    BU32 year;
    BU32 month;
    BU32 date;
	BU32 timeOffset;
}TOT_Content_s;
typedef TOT_Content_s * TOT_Content_sp;


#endif








