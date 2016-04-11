/*===============================================================================

 File Name:gui_plg.h

 Author:szhy

 Create Time:2008-2-13
 
 Description:plugin

 History:

===============================================================================*/
/*-------------------------------------------------------------------------------
	Struct Define
-------------------------------------------------------------------------------*/
typedef void (* GUI_PLG_API_Fun)(BU32 bu32Value,BU32 bu32Param);
typedef struct _tagGUI_Plg_s
{
	BU32 plg;
	GUI_PLG_API_Fun fun;
} GUI_Plg_s;
typedef GUI_Plg_s * GUI_Plg_sp;
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void GUI_PLG_API_Loop(BU32 plg,BU32 bu32Value,BU32 bu32Param);
void GUI_PLG_API_Register(GUI_Plg_sp pPlg,BU32 num);
PU08 GUI_PLG_API_GetVersion(void);

