/*****************************************************************************

  File name: ipanel_input.c

Description: IR Driver setup and starting

COPYRIGHT (C) 2004 STMicroelectronics

 ******************************************************************************/

/* Includes ---------------------------------------------------------------- */
#include "trd_ipanel_init.h"
#include "ipanel_porting_event.h"
#include "ipanel_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_FZVOD
BU08 bPlayTVOD = FYF_FALSE;
#endif

char *urlllll = "rtsp://218.108.226.132:2554/fzsan05/tv/4/d09070200020010001_z.ts?token=40A795806E749E51F7317BC1A650D5284945433F8FF49843845C0EBCE1830B4CF703F89DD301275A79D5E7AC28BF70C0346C9FB2E257EACA925BB7DDCE93C817777DBFEAF7C7C712D719BFE083DD3828E38F76&beginTime=0";
BS32 TRD_IPANEL_MapKey(BU32 u32Key)
{
    BU32 value = 0;
	printf("TRD_IPANEL_MapKey , key = %d\n", u32Key);
#ifdef USE_FZVOD
	if(u32Key == GUI_VK_SELECT)
	{
		bPlayTVOD = FYF_TRUE;
	}
	else
	{
		bPlayTVOD = FYF_FALSE;
	}
#endif

	
	switch(u32Key)
    {
	    case GUI_VK_POWER:		/*电源*/
            return EIS_IRKEY_POWER;
			
        case GUI_VK_0:			/*数字0*/
            return EIS_IRKEY_NUM0;
			
        case GUI_VK_1:			/*数字1*/
            return EIS_IRKEY_NUM1;
			
        case GUI_VK_2:			/*数字2*/
            return EIS_IRKEY_NUM2;
			
        case GUI_VK_3:			/*数字3*/
            return EIS_IRKEY_NUM3;
			
        case GUI_VK_4:			/*数字4*/
            return EIS_IRKEY_NUM4;
			
        case GUI_VK_5:			/*数字5*/
            return EIS_IRKEY_NUM5;
			
        case GUI_VK_6:			/*数字6*/
            return EIS_IRKEY_NUM6;
			
        case GUI_VK_7:			/*数字7*/
            return EIS_IRKEY_NUM7;
			
        case GUI_VK_8:			/*数字8*/
            return EIS_IRKEY_NUM8;
			
        case GUI_VK_9:			/*数字9*/
            return EIS_IRKEY_NUM9;
			
        case GUI_VK_LEFT:		/*左*/
            return EIS_IRKEY_LEFT;
			
        case GUI_VK_RIGHT:		/*右*/
            return EIS_IRKEY_RIGHT;
			
        case GUI_VK_DOWN:		/*下*/
            return EIS_IRKEY_DOWN;
			
        case GUI_VK_UP:			/*上*/
            return EIS_IRKEY_UP;
			
        case GUI_VK_SELECT:		/*确定*/
            return EIS_IRKEY_SELECT;
			
        case GUI_VK_EXIT:		/*退出*/
            return EIS_IRKEY_EXIT;
			
        case GUI_VK_MENU:		/*菜单*/
			if(Ipanel_GetEntrance_Flag())
			{
				value = EIS_IRKEY_HOMEPAGE;
			}
			else
			{
				value = EIS_IRKEY_MENU;
			}
			return value;
			
        case GUI_VK_RED:		/*红色键对应F1*/
            return EIS_IRKEY_IME;
			
        case GUI_VK_GREEN:		/*绿色键对应F2*/
             return EIS_IRKEY_GREEN;
			 
        case GUI_VK_YELLOW:		/*黄色键对应F3*/
            return EIS_IRKEY_YELLOW;
			
        case GUI_VK_BLUE:		/*蓝色键对应F4*/
            return EIS_IRKEY_BLUE;
			
		case GUI_VK_V_UP:		/*音量加*/
		case GUI_VK_OFFSET_UP:
			return EIS_IRKEY_VOLUME_UP;
			
		case GUI_VK_V_DOWN:		/*音量减*/
		case GUI_VK_OFFSET_DOWN:
			return EIS_IRKEY_VOLUME_DOWN;
			
		case GUI_VK_MUTE:		/*静音*/
			return EIS_IRKEY_VOLUME_MUTE;
			
		case GUI_VK_RECALL:		/*返回*/
			return EIS_IRKEY_BACK;
			
		case GUI_VK_P_UP:		/*上翻页*/
			return EIS_IRKEY_PAGE_BACKWARD;

		case GUI_VK_P_DOWN:		/*下翻页*/
			return EIS_IRKEY_PAGE_FORWARD;
			
			
        default:
            return FYF_ERR;
    }
	
    return FYF_ERR;
}


#ifdef __cplusplus
}
#endif
