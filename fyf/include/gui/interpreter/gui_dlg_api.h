/*===============================================================================

 File Name:gui_dlg_api.h

 Author:szhy

 Create Time:2007-5-31
 
 Description:dialog

 History:

===============================================================================*/
#ifndef __GUI_DLG_API_H_
#define __GUI_DLG_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_DLG(lpClassName,x,y,nWidth,nHeight,ID,focusID) \
	lpClassName,x,y,nWidth,nHeight,ID,0,0,0,0,0,0,focusID,0,0,0,       
#define DEF_BKD(lpClassName,x,y,parentID,style) \
	lpClassName,x,y,0,0,0,parentID,0,0,0,0,0,style,0,0,0,
#define DEF_DLG_EVE(ID,lpEnter,lpEnd,lpOnKey) \
	ID,lpEnter,lpEnd,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define GUI_DLG_SV_KEYSTATE 0
#define GUI_DLG_SV_NOPAINT	1
#define GUI_DLG_SV_MSG_HANDLE 2
typedef enum
{
	GUI_DLG_KEYSTATE_ACTION=0x01,
	GUI_DLG_KEYSTATE_PAINT =0x02	
}ffffffffffff;
typedef void (* LPNoPaint)(void);
typedef struct
{
	BU32 msg;
	BU32 (*handle)(BU32 message,BU32 wParam,BU32 lParam);
}GUI_DLG_msg_handle_s,*GUI_DLG_msg_handle_sp;
/*-------------------------------------------------------------------------------

	Function Declare
-------------------------------------------------------------------------------*/
PU08 GUI_DLG_API_GetVersion(void);
void GUI_DLG_API_RegisterNoPaint(BU32 bu32ID,LPNoPaint lpNoPaint);
#endif

