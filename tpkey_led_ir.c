#include <stdio.h>
#include <string.h>
#include <pthread.h>


#include "hi_stb_api.h"
#include "fyf_com.h"
#include "tpmiddle/include/app_common.h"

#define IR_USERCODE 0x7f80
#define LED_DELAY_TIME 1000000

//#define SANMING_VERSION

extern BU08 bfirst;
#if 0
void * LED_DisplayTask(void *args)
{
    HI_U32 u32Loop = 0;
    HI_S32 s32Ret;
    HI_U8 DigDisCode[] = {0x03, 0x9F, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};

    HI_U8 UCharDisCode[] = {
        0xFF, 0xFF, 0x63, 0xFF, 0x61,
        0x71, 0xFF, 0x91, 0xFF, 0xFF,
        0xFF, 0xE3, 0xFF, 0xFF, 0x03,
        0x31, 0xFF, 0xFF, 0x49, 0xFF,
        0x83, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

    //大写字母只能够显示'C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U'，从左至右分别对应此数组中的非0xFF值
    HI_U8 LCharDisCode[] = {
        0x00, 0xC1, 0xE5, 0x85, 0x21,
        0x00, 0x09, 0xD1, 0xDF, 0xFF,
        0x00, 0x9F, 0xFF, 0xD5, 0xC5,
        0x31, 0x19, 0xFF, 0x49, 0xFF,
        0xC7, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00
    };

    while (1)
    {
        for (u32Loop = 0; u32Loop < sizeof(DigDisCode); u32Loop++)
        {
            /*在LED上显示字母或者数字*/
            s32Ret = HI_UNF_LED_Display(DigDisCode[u32Loop] << 24 | DigDisCode[u32Loop] << 16
                                        | DigDisCode[u32Loop] << 8 | DigDisCode[u32Loop]);
            if (HI_SUCCESS != s32Ret)
            {
                printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
                break;
            }

            usleep(LED_DELAY_TIME);
        }

        for (u32Loop = 0; u32Loop < sizeof(UCharDisCode); u32Loop++)
        {
            if (UCharDisCode[u32Loop] != 0xFF)
            {
                /*在LED上显示字母或者数字*/
                s32Ret = HI_UNF_LED_Display(UCharDisCode[u32Loop] << 24 | UCharDisCode[u32Loop] << 16
                                            | UCharDisCode[u32Loop] << 8 | UCharDisCode[u32Loop]);
                if (HI_SUCCESS != s32Ret)
                {
                    printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
                    break;
                }

                usleep(LED_DELAY_TIME);
            }
        }

        for (u32Loop = 0; u32Loop < sizeof(LCharDisCode); u32Loop++)
        {
            if (LCharDisCode[u32Loop] != 0xFF)
            {
                /*在LED上显示字母或者数字*/
                s32Ret = HI_UNF_LED_Display(LCharDisCode[u32Loop] << 24 | LCharDisCode[u32Loop] << 16
                                            | LCharDisCode[u32Loop] << 8 | LCharDisCode[u32Loop]);
                if (HI_SUCCESS != s32Ret)
                {
                    printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
                    break;
                }

                usleep(LED_DELAY_TIME);
            }
        }
    }
	
    return 0;
}
#endif

void * KEY_ReceiveTask(void *args)
{
    HI_S32 s32Ret;
    HI_U32 u32PressStatus, u32KeyId;

    while (1)
    {
        /*获取KEY的按键值和按键状态*/
        s32Ret = HI_UNF_KEY_GetValue(&u32PressStatus, &u32KeyId);
        if (HI_SUCCESS == s32Ret)
        {
        	printf("KEY  KeyId : 0x%x    PressStatus :%d \n", u32KeyId, u32PressStatus);
		    switch(u32KeyId)
			{
			case 0x9:
				u32KeyId = GUI_VK_DOWN;
				break;
			case 0xa:
			case 0xc:
				u32KeyId = GUI_VK_UP;
				break;
			case 0x7:
			case 0x12:
				u32KeyId = GUI_VK_SELECT;
				break;
			case 0x4:
			case 0xf:
				u32KeyId = GUI_VK_MENU;
				break;
			case 0x3:
				u32KeyId = GUI_VK_RIGHT;
				break;
			case 0x6:
				u32KeyId = GUI_VK_LEFT;
				break;
			default:
				usleep(50000);
				continue;
				break;
			}
			if(bfirst)
			{
			   GUI_API_PostKeyMessage(u32KeyId, 0);
			}
			else
			{
#ifdef ENABLE_BROADCAST_IPANEL
			   TRD_IPANEL_Input(u32KeyId);
#endif
			}
  
        }
        else
        {
            usleep(50000);
        }
    }

    return 0;

}

int bConnectKey(u32KeyId)
{
	if (u32KeyId >= GUI_VK_0 && u32KeyId <= GUI_VK_9)
		return 1;
	if (u32KeyId >= GUI_VK_LEFT && u32KeyId <= GUI_VK_DOWN)
		return 1;
	if (u32KeyId == GUI_VK_P_UP || u32KeyId == GUI_VK_P_DOWN)
		return 1;
	return 0;
}

void * IR_ReceiveTask(void *args)
{
    HI_S32 s32Ret;
    HI_U32 u32PressStatus, u32KeyId;
	HI_U32 u32Count = 0;
	HI_U32 LastPressStatus = 1;

    while (1)
    {
        /*获取遥控器的按键值和按键状态*/
        s32Ret = HI_UNF_IR_GetValue(&u32PressStatus, &u32KeyId);
        if (HI_SUCCESS == s32Ret)
        {
            printf("IR   KeyId : 0x%x           PressStatus :%d\n", u32KeyId, u32PressStatus);

		
			//u32KeyId = (u32KeyId >> 16) & 0xffff;
			 switch(u32KeyId)
			{
			case 0x91:
				u32KeyId = GUI_VK_0;
				break;
			case 0x94:
				u32KeyId = GUI_VK_1;
				break;
			case 0x95:
				u32KeyId = GUI_VK_2;
				break;
			case 0x80:
				u32KeyId = GUI_VK_3;
				break;
			case 0x88:
				u32KeyId = GUI_VK_4;
				break;
			case 0x89:
				u32KeyId = GUI_VK_5;
				break;
			case 0x8a:
				u32KeyId = GUI_VK_6;
				break;
			case 0x8c:
				u32KeyId = GUI_VK_7;
				break;
			case 0x8d:
				u32KeyId = GUI_VK_8;
				break;
			case 0x8e:
				u32KeyId = GUI_VK_9;
				break;
			case 0xce:
				u32KeyId = GUI_VK_MENU;
				break;
			case 0x99:
				u32KeyId = GUI_VK_EXIT;
				break;
			case 0x9a:
				u32KeyId = GUI_VK_UP;
				break;
			case 0x9e:
				u32KeyId = GUI_VK_DOWN;
				break;
			case 0x9c:
				u32KeyId = GUI_VK_LEFT;
				break;
			case 0x98:
				u32KeyId = GUI_VK_RIGHT;
				break;
			case 0x9d:
				u32KeyId = GUI_VK_SELECT;
				break;
			case 0xd1:
				u32KeyId = GUI_VK_RED;
				break;
			case 0xcc:
				u32KeyId = GUI_VK_GREEN;
				break;
			case 0xcd:
				u32KeyId = GUI_VK_YELLOW;
				break;
			case 0xd7:
				u32KeyId = GUI_VK_BLUE;
				break;
			case 0x86:
				u32KeyId = GUI_VK_FAV;
				break;
			case 0x97:
				u32KeyId = GUI_VK_RadioORTV;
				break;
			case 0xcb:
				u32KeyId = GUI_VK_P_UP;
				break;
			case 0xc7:
				u32KeyId = GUI_VK_P_DOWN;
				break;
			case 0xc0:
				u32KeyId = GUI_VK_MUTE;
				break;
			case 0xd3:
				u32KeyId = GUI_VK_TRACK;
				break;
			case 0xcf:
				u32KeyId = GUI_VK_INFO;
				break;
		#ifndef SANMING_VERSION
			case 0x8f:
				u32KeyId = GUI_VK_STATE;
		#endif
				break;
			case 0x81:
				u32KeyId = GUI_VK_F1;//广播
				break;		
			case 0xca:
				u32KeyId = GUI_VK_F2;//时移
				break;
			case 0xdf:
				u32KeyId = GUI_VK_F3;//点播
				break;
			case 0x93:
				u32KeyId = GUI_VK_F4;//节目指南
				break;
			case 0x85:
				u32KeyId = GUI_VK_F5;//帮组
				break;
			case 0x96:
				u32KeyId = GUI_VK_F6; //游戏
				break;
			case 0x84:
				u32KeyId = GUI_VK_F8;//切换
				break;
			case 0xd8:
				u32KeyId = GUI_VK_PROGTABLE;
				break;
			case 0xd2:
				u32KeyId = GUI_VK_MAIL;
				break;
			case 0xc2:
				u32KeyId = GUI_VK_RECALL;
				break;
			case 0x92:
				u32KeyId = GUI_VK_POWER;
				break;
			case 0xc4:
				u32KeyId = GUI_VK_V_UP;
				break;
			case 0xc8:
				u32KeyId = GUI_VK_V_UP;
				break;
			case 0xc5:
				u32KeyId = GUI_VK_V_DOWN;
				break;
			case 0xc9:
				u32KeyId = GUI_VK_V_DOWN;
				break;
			default:
				u32KeyId = 0xff;
				break;
			 }
             
			 if(u32KeyId == GUI_VK_POWER && IsMainTaskEnd() )
			 {
			//	 FYF_API_OS_Set_SysTime(g_Year, g_Month, g_Date, g_Hour, g_Minute, g_Sec);
				 APP_SYS_API_SaveTime();
			 	 exit(0xfe);
			 }


			
			if (LastPressStatus == 0 && u32PressStatus == 0)
			{
				if (u32Count > 2)
				{
					if (u32Count %3 == 0 || bConnectKey(u32KeyId))
					{
						if(bfirst)
						{
							GUI_API_PostKeyMessage(u32KeyId, !u32PressStatus);
						}
						else
						{
					#ifdef	ENABLE_BROADCAST_IPANEL
							TRD_IPANEL_Input(u32KeyId);
					#endif
					#ifdef ENABLE_BROADCAST_ENREACH
							HI_TRD_ENREACH_Input(u32KeyId);
					#endif
						}

					#ifndef ENABLE_BROADCAST_ENREACH		
						if((u32KeyId == GUI_VK_V_DOWN )||(u32KeyId == GUI_VK_V_UP) || (u32KeyId == GUI_VK_MUTE))
						{
							GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_KEY_MESSAGE, u32KeyId, 0);
						}
						#endif
					}
				}
				u32Count ++;
			}
			else if (LastPressStatus == 0 && u32PressStatus == 1 )
			{
				if (u32Count > 3)
				{
					if(bfirst)
					{
						GUI_API_PostKeyMessage(u32KeyId, !u32PressStatus);
					}	
					else
					{
				#ifdef	ENABLE_BROADCAST_IPANEL
						TRD_IPANEL_Input(u32KeyId);
				#endif

					#ifdef ENABLE_BROADCAST_ENREACH
							HI_TRD_ENREACH_Input(u32KeyId);
					#endif
					}
				}
				u32Count = 0;
					
	        }
			else if (LastPressStatus == 1 && u32PressStatus == 0)//按下状态，发送弹起状态
			{
				if (u32Count == 0)
				{
					if (bfirst)
					{
						GUI_API_PostKeyMessage(u32KeyId, u32PressStatus);
					}
					else
					{
					#ifdef	ENABLE_BROADCAST_IPANEL
						TRD_IPANEL_Input(u32KeyId);
					#endif

					#ifdef ENABLE_BROADCAST_ENREACH
							HI_TRD_ENREACH_Input(u32KeyId);
							
					#endif
					}

					#ifndef ENABLE_BROADCAST_ENREACH		
					if((u32KeyId == GUI_VK_V_DOWN )||(u32KeyId == GUI_VK_V_UP) || (u32KeyId == GUI_VK_MUTE))
				    {
				    
					    GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_KEY_MESSAGE, u32KeyId, 0);

					}
					#endif
				}
			//	GUI_API_PostKeyMessage(u32KeyId, !u32PressStatus);
			}
			LastPressStatus = u32PressStatus;
        }
    }

    return 0;
}


HI_S32	TP_Led_Init()
{
	HI_S32	s32Ret;
	pthread_t keyTaskid;

	/*打开LED模块*/
    s32Ret = HI_UNF_LED_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    /*使能LED设备*/
    s32Ret = HI_UNF_LED_Enable(0);/*0: 阻塞方式 1:非阻塞方式*/
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
		goto ERR;
    }

    /*使能刷新方式*/
    s32Ret = HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_2);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
		goto ERR;
    }

    /*设置要显示的LED序号*/
    s32Ret = HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }
#if 0
	    /*启动按键接收线程*/
    s32Ret = pthread_create(&keyTaskid, NULL, KEY_ReceiveTask, NULL);
    if (0 != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        perror("pthread_create");
		goto ERR;
    }
#endif	
	return HI_SUCCESS;
		
ERR:
	HI_UNF_LED_Close();
	return s32Ret;

}


HI_S32 TP_KEY_Init()
{
	
    HI_S32	s32Ret;
	pthread_t keyTaskid;
	
    /*打开前面板的KEY设备，初始化按键接口*/
    s32Ret = HI_UNF_KEY_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置是否上报按键弹起有效*/
    s32Ret = HI_UNF_KEY_IsKeyUp(0);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置是否使能按键产生重复按键功能*/
    s32Ret = HI_UNF_KEY_IsRepKey(1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

#if 1
	/*启动按键接收线程*/
    s32Ret = pthread_create(&keyTaskid, NULL, KEY_ReceiveTask, NULL);
    if (0 != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        perror("pthread_create");
		goto ERR;
    }

#endif
	return HI_SUCCESS;

ERR:
	HI_UNF_KEY_Close();
	return s32Ret;
	
}

HI_S32 TP_IR_Init()
{
    HI_S32	s32Ret;
	pthread_t irTaskid;
	
	  /*打开红外设备*/
    s32Ret = HI_UNF_IR_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置IR工作模式为阻塞*/
    s32Ret = HI_UNF_IR_Enable(0);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置IR阻塞超时时间为500ms*/
    s32Ret = HI_UNF_IR_SetBlockTime(500);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置IR用户码*/
    s32Ret = HI_UNF_IR_SelectUserCode(IR_USERCODE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*使能重复键和弹起键上报*/
    s32Ret = HI_UNF_IR_IsKeyUp(1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置是否上报重复按键*/
    s32Ret = HI_UNF_IR_IsRepKey(1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }
#if 1
	/*设置上报重复按键的时间间隔*/
	
	s32Ret = HI_UNF_IR_RepKeyTimeoutVal(100);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }
#endif	
	  /*启动红外接收线程*/
    s32Ret = pthread_create(&irTaskid, NULL, IR_ReceiveTask, NULL);
    if (0 != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        perror("pthread_create");
        goto ERR;
    }
	
	return HI_SUCCESS;
	
ERR:
	HI_UNF_IR_Close();
	return s32Ret;
}


