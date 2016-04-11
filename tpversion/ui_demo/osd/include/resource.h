/*===============================================================================

 File Name:resource.h

 Author:szhy

 Create Time:2007-8-10
 
 Description:resource

 History:

===============================================================================*/
#ifndef __RESOURCE_INTERNATIONAL_H_
#define __RESOURCE_INTERNATIONAL_H_
/*-------------------------------------------------------------------------------
	Include Files
-------------------------------------------------------------------------------*/
#include "../../../../fyf/include/gui/gui_common.h"
#include "wndId.h"
#include "gui_style_id.h"
#include "S_Chinese_strs_id.h"
#include "bmpfilename.h"
#include "config.h"
#include "ui_color_def.h"
#include "../../../ui_com_api.h"
/*-------------------------------------------------------------------------------
	extern
-------------------------------------------------------------------------------*/
extern unsigned int DialogBox[][9];
extern unsigned int StyleTble[][2];
extern char *DicData[];
extern char *DicDataEnglish[];
extern GUI_OSDTable_s OSDTable[];
extern GUI_StateTable_s stateTable[];
extern GUI_FocusTable_s focusTable[];
extern GUI_EventTable_s eventTable[];

extern BU32 OSDTableNum;
extern BU32 stateTableNum;
extern BU32 focusTableNum;
extern BU32 eventTableNum;

extern BU32 acascii[];
extern BU32 acgb2312[];
extern BU32 acbmplib[];
extern unsigned char acfontlib[];

void osdInit(void);


#endif

