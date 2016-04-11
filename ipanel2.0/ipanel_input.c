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
	    case GUI_VK_POWER:		/*��Դ*/
            return EIS_IRKEY_POWER;
			
        case GUI_VK_0:			/*����0*/
            return EIS_IRKEY_NUM0;
			
        case GUI_VK_1:			/*����1*/
            return EIS_IRKEY_NUM1;
			
        case GUI_VK_2:			/*����2*/
            return EIS_IRKEY_NUM2;
			
        case GUI_VK_3:			/*����3*/
            return EIS_IRKEY_NUM3;
			
        case GUI_VK_4:			/*����4*/
            return EIS_IRKEY_NUM4;
			
        case GUI_VK_5:			/*����5*/
            return EIS_IRKEY_NUM5;
			
        case GUI_VK_6:			/*����6*/
            return EIS_IRKEY_NUM6;
			
        case GUI_VK_7:			/*����7*/
            return EIS_IRKEY_NUM7;
			
        case GUI_VK_8:			/*����8*/
            return EIS_IRKEY_NUM8;
			
        case GUI_VK_9:			/*����9*/
            return EIS_IRKEY_NUM9;
			
        case GUI_VK_LEFT:		/*��*/
            return EIS_IRKEY_LEFT;
			
        case GUI_VK_RIGHT:		/*��*/
            return EIS_IRKEY_RIGHT;
			
        case GUI_VK_DOWN:		/*��*/
            return EIS_IRKEY_DOWN;
			
        case GUI_VK_UP:			/*��*/
            return EIS_IRKEY_UP;
			
        case GUI_VK_SELECT:		/*ȷ��*/
            return EIS_IRKEY_SELECT;
			
        case GUI_VK_EXIT:		/*�˳�*/
            return EIS_IRKEY_EXIT;
			
        case GUI_VK_MENU:		/*�˵�*/
			if(Ipanel_GetEntrance_Flag())
			{
				value = EIS_IRKEY_HOMEPAGE;
			}
			else
			{
				value = EIS_IRKEY_MENU;
			}
			return value;
			
        case GUI_VK_RED:		/*��ɫ����ӦF1*/
            return EIS_IRKEY_IME;
			
        case GUI_VK_GREEN:		/*��ɫ����ӦF2*/
             return EIS_IRKEY_GREEN;
			 
        case GUI_VK_YELLOW:		/*��ɫ����ӦF3*/
            return EIS_IRKEY_YELLOW;
			
        case GUI_VK_BLUE:		/*��ɫ����ӦF4*/
            return EIS_IRKEY_BLUE;
			
		case GUI_VK_V_UP:		/*������*/
		case GUI_VK_OFFSET_UP:
			return EIS_IRKEY_VOLUME_UP;
			
		case GUI_VK_V_DOWN:		/*������*/
		case GUI_VK_OFFSET_DOWN:
			return EIS_IRKEY_VOLUME_DOWN;
			
		case GUI_VK_MUTE:		/*����*/
			return EIS_IRKEY_VOLUME_MUTE;
			
		case GUI_VK_RECALL:		/*����*/
			return EIS_IRKEY_BACK;
			
		case GUI_VK_P_UP:		/*�Ϸ�ҳ*/
			return EIS_IRKEY_PAGE_BACKWARD;

		case GUI_VK_P_DOWN:		/*�·�ҳ*/
			return EIS_IRKEY_PAGE_FORWARD;
			
			
        default:
            return FYF_ERR;
    }
	
    return FYF_ERR;
}


#ifdef __cplusplus
}
#endif
