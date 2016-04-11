/*===============================================================================

 File Name:gui_typedef.h

 Author:szhy

 Create Time:2007-11-20
 
 Description:

 History:

===============================================================================*/
#ifndef __GUI_TYPEDEF_H_
#define __GUI_TYPEDEF_H_

#ifndef TYPE_BU32
#define TYPE_BU32
typedef unsigned long		BU32;
typedef BU32 *				PU32;
#endif

#ifndef TYPE_BU16
#define TYPE_BU16
typedef unsigned short		BU16;
typedef BU16 *				PU16;
#endif

#ifndef TYPE_BU08
#define TYPE_BU08
#if GUI_PLATFORM == PC
typedef unsigned char		BU08;
#elif GUI_PLATFORM == ST
typedef char 	       BU08;
#elif GUI_PLATFORM == cxnt 
typedef char 				BU08;
#elif GUI_PLATFORM == Ali
typedef unsigned char 		BU08;
#elif GUI_PLATFORM == Hisi
typedef unsigned char 		BU08;
#endif
typedef BU08 *				PU08;
#endif

#ifndef TYPE_BS32
#define TYPE_BS32
typedef signed long			BS32;
typedef BS32 *				PS32;
#endif

#ifndef TYPE_BS16
#define TYPE_BS16
typedef signed short		BS16;
typedef BS16 *				PS16;
#endif

#ifndef TYPE_BS08
#define TYPE_BS08
typedef signed char			BS08;
typedef BS08 *				PS08;
#endif

#ifndef TYPE_BU64
#define TYPE_BU64
typedef unsigned long long     BU64;
typedef BU64 *            PU64;
#endif


#ifndef TYPE_BS64
#define TYPE_BS64
typedef unsigned long long     BS64;
typedef BS64 *    PS64;
#endif

typedef struct _tagGUI_Point_s
{
	BU32 xpos;
	BU32 ypos;
}GUI_Point_s,* GUI_Point_sp;

typedef struct _tagGUI_Rect_s
{
	BU32 xpos;
	BU32 ypos;
	BU32 width;
	BU32 height;
}GUI_Rect_s,*GUI_Rect_sp;




/* RGB struct */
typedef struct _tagGUI_RGB_s
{
	BU08 b;
	BU08 g;
	BU08 r;
}GUI_RGB_s;



#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        1
#endif

#ifndef false
#define false       0
#endif
#ifndef true
#define true        1
#endif

#ifndef NULL
#define NULL                ((void *)0)
#endif
#ifndef null
#define null                ((void *)0)
#endif

#define GUI_SUCCESS    	1
#define GUI_ERROR		0

#endif

