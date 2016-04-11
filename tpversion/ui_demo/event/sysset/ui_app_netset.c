#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"
#ifdef USE_FZVOD
#include "../../../../ow_vod/ow_api.h"
#endif
//互动设置
#define FYF_FLS_VOD_DATA_SIZE      (1024*128)
#define NetSet_X 			10
#define NetSet_W 			 10
static BS08 SwitchItem_flag=0;
static BU08 AutoIP_flag=0;
static BU08 AutoDNS_flag=0;
static BU08 NetSet_offset=0;
BU08 IPAddress[16]="000.000.000.000";
BU08 SubNetMark[16]="000.000.000.000";
BU08 DefaultGateway[16]="000.000.000.000";
BU08 FirstChoseDNS[16]="000.000.000.000";
BU08 ReserveDNS[16]="000.000.000.000";

BU08 AutoIPAddress[16] = "000.000.000.000";
BU08 AutoSubNetMark[16] = "000.000.000.000";
BU08 AutoDefaultGateway[16] = "000.000.000.000";
BU08 AutoFirstChoseDNS[16] = "000.000.000.000";
BU08 AutoReserveDNS[16] = "000.000.000.000";

extern BU08 g_ip_address[16];
extern BU08 g_sub_net_Mark[16];
extern BU08 g_gateway_ip[16];
extern BU08 g_first_chose_dns[16];
extern BU08 g_reserve_dns[16];

static BU08 FirsrPageAddr[16]="000.000.000.000";
static BU16 IPAddr_Num[4]={0,0,0,0};
static BU16 SubNet_Num[4]={0,0,0,0};
static BU16 Gateway_Num[4]={0,0,0,0};
static BU16 FirstChoseDNS_Num[4]={0,0,0,0};
static BU16 ReserveDNS_Num[4]={0,0,0,0};
static BU16 FirstPageAddr_Num[4]={0,0,0,0};
static BU08 temp[16]="000.000.000.000";


typedef enum hiEth_Forward_E
{
    ETH_FW_ALL2CPU_D = 0,
    ETH_FW_ALL2UP_D,
    ETH_FW_ENA2CPU_D,
    ETH_FW_ENA2UP_D,
    ETH_FW_ALL2CPU_U,
    ETH_FW_ALL2UP_U,
    ETH_FW_ENA2CPU_U,
    ETH_FW_ENA2UP_U,
    ETH_FW_REG_CFG,
    ETH_FW_MAX
}Eth_Forward_E;


NetSetInfo IpInfomation;



void UI_APP_SetEthAttr(BU08 eth_port)
{
	if (eth_port > 1) return;
	HI_ETH_Close(0);
	HI_ETH_Close(1);

	HI_ETH_Open(eth_port);
	HI_ETH_IPAddressSet(eth_port, "172.016.004.121");
	HI_ETH_SubNetmaskSet(eth_port, "255.255.255.000");
	HI_ETH_GatewaySet(eth_port, "172.016.004.253");
	
	
}


void UI_APP_SetDefaultIP()
{
	UI_APP_SetEthAttr(0);
	
/*	HI_ETH_SetMac(0, "1a:2b:3c:4d:55:6f");

	
	HI_ETH_IPAddressSet(1, "172.016.003.121");
	HI_ETH_SubNetmaskSet(1, "255.255.255.000");
	HI_ETH_GatewaySet(1, "172.016.003.254");
	HI_ETH_SetMac(1, "1a:22:3c:4d:5e:6f");
	*/

/*

	HI_ETH_FwCtrlSet(ETH_FW_ALL2CPU_U, 0);
	HI_ETH_FwCtrlSet(ETH_FW_ALL2UP_U, 0);
	HI_ETH_FwCtrlSet(ETH_FW_ENA2CPU_U, 0);
	HI_ETH_FwCtrlSet(ETH_FW_ENA2UP_U, 0);

	HI_ETH_FwCtrlSet(ETH_FW_ALL2UP_D, 0);
	HI_ETH_FwCtrlSet(ETH_FW_ENA2UP_D, 0);
	HI_ETH_FwCtrlSet(ETH_FW_ALL2CPU_D, 1);
	HI_ETH_FwCtrlSet(ETH_FW_ENA2CPU_D, 1);
	*/
}


#if 0

void UI_APP_SetDefaultIP()
{
	FYF_FLS_IOCTL_PARAM_s fyf_fls_ioctl_param,fyf_fls_read_param;
	memset(&IpInfomation, 0, sizeof(IpInfomation));

	memcpy(IpInfomation.ipaddr, "172.016.004.121", strlen("172.016.004.121"));
	memcpy(IpInfomation.netmask, "255.255.255.000", strlen("255.255.255.000"));
	memcpy(IpInfomation.gateway, "172.016.004.253", strlen("172.016.004.253"));

	memcpy(g_ip_address, IpInfomation.ipaddr,  16);
	memcpy(g_sub_net_Mark, IpInfomation.netmask,  16);
	memcpy(g_gateway_ip, IpInfomation.gateway,  16);

	IpInfomation.netflag = 0xa0;

	//SQL_API_SaveToFlash("flash.db");

	HI_ETH_IPAddressSet(0, g_ip_address);
	HI_ETH_SubNetmaskSet(0, g_sub_net_Mark);
	HI_ETH_GatewaySet(0, g_gateway_ip);

	APP_SYS_API_SetSysInfo(APP_SYS_AutoIP_e,1);
	APP_SYS_API_SaveSysInfo();


}
#endif




void charConvert(BU08 *beforeConvert,BU08 *afterConvert)
{
  BU08 temp[4][4]={0};
  BU08 i=0,m=0,n=0;
  BU08 Num[4]={0,0,0,0};
  for(i=0;i<strlen(beforeConvert);i++)
  {
	 if(beforeConvert[i] == '.')
	 {
	    m++;
		n=0;
		continue;
	 }
     temp[m][n++] = beforeConvert[i];
  }
  for(i=0;i<4;i++)
  {
	  switch(strlen(temp[i]))
	  {
	    case 1:Num[i] = temp[i][0] - 48;
			   break;
		case 2:Num[i] = (temp[i][0] - 48) * 10 + (temp[i][1] - 48);
		       break;
		case 3:Num[i] = (temp[i][0] - 48) * 100 + (temp[i][1] - 48) * 10 + (temp[i][2] - 48);
		       break;
		default:
			   break;

	  }
  }
  sprintf(afterConvert,"%03d.%03d.%03d.%03d",Num[0],Num[1],Num[2],Num[3]);
}

void UI_APP_NetSet_Enter(void)
{
	BU32 g_sys_SwitchItem=0;
	BU32 g_sys_AutoIP=0;
	BU32 g_sys_AutoDNS=0;
	BU32 hwnd;
	GUI_Rect_s rect;
	APP_EPG_API_StopGetEpg();
	hwnd = GUI_API_GetDlgItem(ID_VFM_NetSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_NetSet,ID_STA_Event2_NetSet,ID_PBR_Epg_NetSet,260,1);

	APP_SYS_API_GetSysInfo(APP_SYS_SwitchItem_e,&g_sys_SwitchItem);
	APP_SYS_API_GetSysInfo(APP_SYS_AutoIP_e,&g_sys_AutoIP);
	APP_SYS_API_GetSysInfo(APP_SYS_AutoDNS_e,&g_sys_AutoDNS);
	
	if(g_sys_SwitchItem) SwitchItem_flag=0;
	else SwitchItem_flag = 1;
	if(g_sys_AutoIP) AutoIP_flag = 0;
	else AutoIP_flag = 1;
	if(g_sys_AutoDNS) AutoDNS_flag = 0;
	else AutoDNS_flag = 1;
	GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet,WDT_CBX_SV_INDEX,(BU32)g_sys_SwitchItem,0);
	GUI_ENC_API_SetValue(ID_CBX_AutoIP_NetSet,WDT_CBX_SV_INDEX,(BU32)g_sys_AutoIP,0);
	GUI_ENC_API_SetValue(ID_CBX_AutoDNS_NetSet,WDT_CBX_SV_INDEX,(BU32)g_sys_AutoDNS,0);
	
	GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);

	memcpy(IPAddress, g_ip_address, sizeof(g_ip_address));
	memcpy(SubNetMark, g_sub_net_Mark, sizeof(g_sub_net_Mark));
	memcpy(DefaultGateway, g_gateway_ip, sizeof(g_gateway_ip));
	memcpy(FirstChoseDNS, g_first_chose_dns, sizeof(g_first_chose_dns));
	memcpy(ReserveDNS, g_reserve_dns, sizeof(g_reserve_dns));

	if (AutoIP_flag)
	{
	    memset(temp, 0, 16);
		HI_ETH_IPAddressGet(0, temp);
		charConvert(temp, AutoIPAddress);
		HI_ETH_SubNetmaskGet(0, temp);
		charConvert(temp, AutoSubNetMark);
		HI_ETH_GatewayGet(0, temp);
		charConvert(temp, AutoDefaultGateway);
	}

	if (AutoDNS_flag)
	{
		memset(temp, 0 , 16);
		HI_ETH_DNSGet(0, temp);
		charConvert(temp, AutoFirstChoseDNS);
		charConvert(temp, AutoReserveDNS);
	}
	
	if (IPAddress[0] == 0)
	{
		sprintf(IPAddress, "000.000.000.000");
		sprintf(SubNetMark, "000.000.000.000");
		sprintf(DefaultGateway, "000.000.000.000");
		sprintf(FirstChoseDNS, "000.000.000.000");
		sprintf(ReserveDNS, "000.000.000.000");
	}


	
}
void UI_APP_NetSet_End(void)
{

}
void UI_APP_NetSet_Key(BU32 key)
{
	BU32 index;
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_CBX_SwitchItem_NetSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_SwitchItem_NetSet,WDT_CBX_SV_INDEX,&index,0);
			if(index)
			{	
			    SwitchItem_flag=0;//单向
				GUI_ENC_API_Enable(ID_CBX_AutoIP_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_IPAddress_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_Subnetwork_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_GatewayIP_NetSet, 0);
				GUI_ENC_API_Enable(ID_CBX_AutoDNS_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_FirstChoseDNS_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_ReserveDNS_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_FirstpageAddr_NetSet, 0);
		
				GUI_ENC_API_Update(ID_DLG_NetSet,1);
			}
			else
			{
			    SwitchItem_flag=1;//双向
				GUI_ENC_API_Enable(ID_CBX_AutoIP_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_IPAddress_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_Subnetwork_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_GatewayIP_NetSet, 1);
				GUI_ENC_API_Enable(ID_CBX_AutoDNS_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_FirstChoseDNS_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_ReserveDNS_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_FirstpageAddr_NetSet, 1);
		
				GUI_ENC_API_Update(ID_DLG_NetSet,1);
			}

		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_AutoIP_NetSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_AutoIP_NetSet,WDT_CBX_SV_INDEX,&index,0);
			if(index)
			{	
			    AutoIP_flag=0;//手动设置IP信息
				
				GUI_ENC_API_Enable(ID_ODC_IPAddress_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_Subnetwork_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_GatewayIP_NetSet, 1);
				GUI_ENC_API_Update(ID_DLG_NetSet,1);
			}
			else
			{
			    AutoIP_flag=1;//自动获取IP信息

				printf("temp=%s,IPAddress=%s\n\n",temp,IPAddress);
				GUI_ENC_API_Enable(ID_ODC_IPAddress_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_Subnetwork_NetSet, 0);
				GUI_ENC_API_Enable(ID_ODC_GatewayIP_NetSet, 0);
				GUI_ENC_API_Update(ID_DLG_NetSet,1);
			}

		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_AutoDNS_NetSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_AutoDNS_NetSet,WDT_CBX_SV_INDEX,&index,0);
			if(index)
			{	
			    AutoDNS_flag=0;//手动设置DNS信息

				GUI_ENC_API_Enable(ID_ODC_FirstChoseDNS_NetSet, 1);
				GUI_ENC_API_Enable(ID_ODC_ReserveDNS_NetSet, 1);
				GUI_ENC_API_Update(ID_DLG_NetSet,1);
			}
			else
			{
			    AutoDNS_flag=1;//自动获取DNS信息

			}

		}
		break;
	case GUI_VK_UP:
	case GUI_VK_P_UP:
		if(GUI_ENC_API_IsFocus(ID_CBX_SwitchItem_NetSet))
		{
			
		   if(SwitchItem_flag) 
		   {
		   		GUI_ENC_API_SetFocus(ID_ODC_FirstpageAddr_NetSet);
				GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
		   }

		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_AutoIP_NetSet))
		{
		  	GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_SwitchItem_NetSet);

		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_AutoDNS_NetSet))
		{

		    if(AutoIP_flag)
		   	{
		   		GUI_ENC_API_SetFocus(ID_CBX_AutoIP_NetSet);

		    }
			else
			GUI_ENC_API_SetFocus(ID_ODC_GatewayIP_NetSet);

		}		
		break;
	case GUI_VK_DOWN:	
	case GUI_VK_P_DOWN:
		if(GUI_ENC_API_IsFocus(ID_CBX_SwitchItem_NetSet))
		{
		   if(SwitchItem_flag) 
		   {
		   		GUI_ENC_API_SetFocus(ID_CBX_AutoIP_NetSet);
				GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);

		   	}
		   else if(SwitchItem_flag == -1)
		   {			
		        GUI_ENC_API_GetValue(ID_CBX_SwitchItem_NetSet,WDT_CBX_SV_INDEX,&index,0);
				if(index)
				{
				}
				else
				GUI_ENC_API_SetFocus(ID_CBX_AutoIP_NetSet);

		   }

		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_AutoIP_NetSet))
		{
		    if(AutoIP_flag)
		   	GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
			else
			GUI_ENC_API_SetFocus(ID_ODC_IPAddress_NetSet);

		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_AutoDNS_NetSet))
		{
		    if(AutoDNS_flag)
		   	GUI_ENC_API_SetFocus(ID_ODC_FirstpageAddr_NetSet);
			else
			GUI_ENC_API_SetFocus(ID_ODC_FirstChoseDNS_NetSet);

		}

		break;
	case GUI_VK_RECALL:
		NetSet_offset = 0;
		GUI_ENC_API_SwitchState(ID_DLG_Preserve_NetSet, 1);
		//GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_SELECT:
	case GUI_VK_EXIT:
		NetSet_offset = 0;
		GUI_ENC_API_SwitchState(ID_DLG_Preserve_NetSet, 1);
		break;
	default:
		break;
	}
}

void UI_APP_IPAddress_OnPaint(BU32 hdc,BU32 lParam)
{	


	if(GUI_ENC_API_IsFocus(ID_ODC_IPAddress_NetSet))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(NetSet_offset>=0&&NetSet_offset<3)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+NetSet_offset*NetSet_W, 30, NetSet_W, COLOR_BLACK);			
		}
		else if(NetSet_offset>=3&&NetSet_offset<6)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+1)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=6&&NetSet_offset<9)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+2)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=9&&NetSet_offset<12)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+3)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		if (AutoIP_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoIPAddress, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, IPAddress, 16, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, NetSet_X, 30, NetSet_W, COLOR_BLACK);			
		if (AutoIP_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoIPAddress, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, IPAddress, 16, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_Subnetwork_OnPaint(BU32 hdc,BU32 lParam)
{
	if(GUI_ENC_API_IsFocus(ID_ODC_Subnetwork_NetSet))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(NetSet_offset>=0&&NetSet_offset<3)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+NetSet_offset*NetSet_W, 30, NetSet_W, COLOR_BLACK);			
		}
		else if(NetSet_offset>=3&&NetSet_offset<6)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+1)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=6&&NetSet_offset<9)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+2)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=9&&NetSet_offset<12)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+3)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		if (AutoIP_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoSubNetMark, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, SubNetMark, 16, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, NetSet_X, 30, NetSet_W, COLOR_BLACK);			
		if (AutoIP_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoSubNetMark, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, SubNetMark, 16, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_GatewayIP_OnPaint(BU32 hdc,BU32 lParam)
{
	if(GUI_ENC_API_IsFocus(ID_ODC_GatewayIP_NetSet))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(NetSet_offset>=0&&NetSet_offset<3)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+NetSet_offset*NetSet_W, 30, NetSet_W, COLOR_BLACK);			
		}
		else if(NetSet_offset>=3&&NetSet_offset<6)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+1)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=6&&NetSet_offset<9)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+2)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=9&&NetSet_offset<12)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+3)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		if (AutoIP_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoDefaultGateway, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, DefaultGateway, 16, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, NetSet_X, 30, NetSet_W, COLOR_BLACK);			
		if (AutoIP_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoDefaultGateway, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, DefaultGateway, 16, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_FirstChoseDNS_OnPaint(BU32 hdc,BU32 lParam)
{
	if(GUI_ENC_API_IsFocus(ID_ODC_FirstChoseDNS_NetSet))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(NetSet_offset>=0&&NetSet_offset<3)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+NetSet_offset*NetSet_W, 30, NetSet_W, COLOR_BLACK);			
		}
		else if(NetSet_offset>=3&&NetSet_offset<6)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+1)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=6&&NetSet_offset<9)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+2)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=9&&NetSet_offset<12)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+3)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		if (AutoDNS_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoFirstChoseDNS, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, FirstChoseDNS, 16, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, NetSet_X, 30, NetSet_W, COLOR_BLACK);			
		if (AutoDNS_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoFirstChoseDNS, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, FirstChoseDNS, 16, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_ReserveDNS_OnPaint(BU32 hdc,BU32 lParam)
{
	if(GUI_ENC_API_IsFocus(ID_ODC_ReserveDNS_NetSet))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(NetSet_offset>=0&&NetSet_offset<3)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+NetSet_offset*NetSet_W, 30, NetSet_W, COLOR_BLACK);			
		}
		else if(NetSet_offset>=3&&NetSet_offset<6)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+1)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=6&&NetSet_offset<9)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+2)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=9&&NetSet_offset<12)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+3)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		if (AutoDNS_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoReserveDNS, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, ReserveDNS, 16, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, NetSet_X, 30, NetSet_W, COLOR_BLACK);	
		if (AutoDNS_flag)
			GUI_API_TextOut(hdc, NetSet_X, 9, AutoReserveDNS, 16, COLOR_WHITE, COLOR_BLACK, 1);
		else
			GUI_API_TextOut(hdc, NetSet_X, 9, ReserveDNS, 16, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_FirstpageAddr_OnPaint(BU32 hdc,BU32 lParam)
{
	if(GUI_ENC_API_IsFocus(ID_ODC_FirstpageAddr_NetSet))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(NetSet_offset>=0&&NetSet_offset<3)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+NetSet_offset*NetSet_W, 30, NetSet_W, COLOR_BLACK);			
		}
		else if(NetSet_offset>=3&&NetSet_offset<6)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+1)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=6&&NetSet_offset<9)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+2)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		else if(NetSet_offset>=9&&NetSet_offset<12)
		{
			GUI_API_DrawHLine(hdc, NetSet_X+(NetSet_offset+3)*NetSet_W, 30, NetSet_W, COLOR_BLACK);
		}
		GUI_API_TextOut(hdc, NetSet_X, 9, FirsrPageAddr, 16, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, NetSet_X, 30, NetSet_W, COLOR_BLACK);			
		GUI_API_TextOut(hdc, NetSet_X, 9, FirsrPageAddr, 16, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_ODCNetSet_Key(BU32 key)
{
	BU32 index = 0;
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:		
		if(GUI_ENC_API_IsFocus(ID_ODC_IPAddress_NetSet))
		{
			switch(NetSet_offset%3)
			{
			case 0:
				IPAddr_Num[NetSet_offset/3]=(IPAddr_Num[NetSet_offset/3]%100 + key*100);
				break;
			case 1:
				IPAddr_Num[NetSet_offset/3]=(IPAddr_Num[NetSet_offset/3]%10 + key*10 + IPAddr_Num[NetSet_offset/3]/100*100);
				break;
			case 2:
				IPAddr_Num[NetSet_offset/3]=(IPAddr_Num[NetSet_offset/3]/10*10 + key);
				break;
			default:
				break;
			}
			if(IPAddr_Num[NetSet_offset/3]>255)	IPAddr_Num[NetSet_offset/3] = 255;
			if(IPAddr_Num[NetSet_offset/3]<0)	IPAddr_Num[NetSet_offset/3] = 0;
			sprintf(IPAddress,"%03d.%03d.%03d.%03d",IPAddr_Num[0],IPAddr_Num[1],IPAddr_Num[2],IPAddr_Num[3]);
			
			NetSet_offset++;
			if(NetSet_offset > 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_Subnetwork_NetSet);
			}
				GUI_ENC_API_Update(ID_ODC_IPAddress_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_Subnetwork_NetSet))
		{
			switch(NetSet_offset%3)
			{
			case 0:
				SubNet_Num[NetSet_offset/3]=(SubNet_Num[NetSet_offset/3]%100 + key*100);
				break;
			case 1:
				SubNet_Num[NetSet_offset/3]=(SubNet_Num[NetSet_offset/3]%10 + key*10 + SubNet_Num[NetSet_offset/3]/100*100);
				break;
			case 2:
				SubNet_Num[NetSet_offset/3]=(SubNet_Num[NetSet_offset/3]/10*10 + key);
				break;
			default:
				break;
			}
			if(SubNet_Num[NetSet_offset/3]>255)	SubNet_Num[NetSet_offset/3] = 255;
			if(SubNet_Num[NetSet_offset/3]<0)	SubNet_Num[NetSet_offset/3] = 0;
			sprintf(SubNetMark,"%03d.%03d.%03d.%03d",SubNet_Num[0],SubNet_Num[1],SubNet_Num[2],SubNet_Num[3]);
			NetSet_offset++;
			if(NetSet_offset > 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_GatewayIP_NetSet);
			}
				GUI_ENC_API_Update(ID_ODC_Subnetwork_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_GatewayIP_NetSet))
		{
			switch(NetSet_offset%3)
			{
			case 0:
				Gateway_Num[NetSet_offset/3]=(Gateway_Num[NetSet_offset/3]%100 + key*100);
				break;
			case 1:
				Gateway_Num[NetSet_offset/3]=(Gateway_Num[NetSet_offset/3]%10 + key*10 + Gateway_Num[NetSet_offset/3]/100*100);
				break;
			case 2:
				Gateway_Num[NetSet_offset/3]=(Gateway_Num[NetSet_offset/3]/10*10 + key);
				break;
			default:
				break;
			}
			if(Gateway_Num[NetSet_offset/3]>255)	Gateway_Num[NetSet_offset/3] = 255;
			if(Gateway_Num[NetSet_offset/3]<0)	Gateway_Num[NetSet_offset/3] = 0;
			sprintf(DefaultGateway,"%03d.%03d.%03d.%03d",Gateway_Num[0],Gateway_Num[1],Gateway_Num[2],Gateway_Num[3]);
			NetSet_offset++;
			if(NetSet_offset > 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
			}
				GUI_ENC_API_Update(ID_ODC_GatewayIP_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstChoseDNS_NetSet))
		{
			switch(NetSet_offset%3)
			{
			case 0:
				FirstChoseDNS_Num[NetSet_offset/3]=(FirstChoseDNS_Num[NetSet_offset/3]%100 + key*100);
				break;
			case 1:
				FirstChoseDNS_Num[NetSet_offset/3]=(FirstChoseDNS_Num[NetSet_offset/3]%10 + key*10 + FirstChoseDNS_Num[NetSet_offset/3]/100*100);
				break;
			case 2:
				FirstChoseDNS_Num[NetSet_offset/3]=(FirstChoseDNS_Num[NetSet_offset/3]/10*10 + key);
				break;
			default:
				break;
			}
			if(FirstChoseDNS_Num[NetSet_offset/3]>255)	FirstChoseDNS_Num[NetSet_offset/3] = 255;
			if(FirstChoseDNS_Num[NetSet_offset/3]<0)	FirstChoseDNS_Num[NetSet_offset/3] = 0;
			sprintf(FirstChoseDNS,"%03d.%03d.%03d.%03d",FirstChoseDNS_Num[0],FirstChoseDNS_Num[1],FirstChoseDNS_Num[2],FirstChoseDNS_Num[3]);
			NetSet_offset++;
			if(NetSet_offset > 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_ReserveDNS_NetSet);
			}
				GUI_ENC_API_Update(ID_ODC_FirstChoseDNS_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_ReserveDNS_NetSet))
		{
			switch(NetSet_offset%3)
			{
			case 0:
				ReserveDNS_Num[NetSet_offset/3]=(ReserveDNS_Num[NetSet_offset/3]%100 + key*100);
				break;
			case 1:
				ReserveDNS_Num[NetSet_offset/3]=(ReserveDNS_Num[NetSet_offset/3]%10 + key*10 + ReserveDNS_Num[NetSet_offset/3]/100*100);
				break;
			case 2:
				ReserveDNS_Num[NetSet_offset/3]=(ReserveDNS_Num[NetSet_offset/3]/10*10 + key);
				break;
			default:
				break;
			}
			if(ReserveDNS_Num[NetSet_offset/3]>255)	ReserveDNS_Num[NetSet_offset/3] = 255;
			if(ReserveDNS_Num[NetSet_offset/3]<0)	ReserveDNS_Num[NetSet_offset/3] = 0;
			sprintf(ReserveDNS,"%03d.%03d.%03d.%03d",ReserveDNS_Num[0],ReserveDNS_Num[1],ReserveDNS_Num[2],ReserveDNS_Num[3]);
			NetSet_offset++;
			if(NetSet_offset > 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_FirstpageAddr_NetSet);
			}
				GUI_ENC_API_Update(ID_ODC_ReserveDNS_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstpageAddr_NetSet))
		{
			switch(NetSet_offset%3)
			{
			case 0:
				FirstPageAddr_Num[NetSet_offset/3]=(FirstPageAddr_Num[NetSet_offset/3]%100 + key*100);
				break;
			case 1:
				FirstPageAddr_Num[NetSet_offset/3]=(FirstPageAddr_Num[NetSet_offset/3]%10 + key*10 + FirstPageAddr_Num[NetSet_offset/3]/100*100);
				break;
			case 2:
				FirstPageAddr_Num[NetSet_offset/3]=(FirstPageAddr_Num[NetSet_offset/3]/10*10 + key);
				break;
			default:
				break;
			}
			if(FirstPageAddr_Num[NetSet_offset/3]>255)	FirstPageAddr_Num[NetSet_offset/3] = 255;
			if(FirstPageAddr_Num[NetSet_offset/3]<0)	FirstPageAddr_Num[NetSet_offset/3] = 0;
			sprintf(FirsrPageAddr,"%03d.%03d.%03d.%03d",FirstPageAddr_Num[0],FirstPageAddr_Num[1],FirstPageAddr_Num[2],FirstPageAddr_Num[3]);
			NetSet_offset++;
			if(NetSet_offset > 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_CBX_SwitchItem_NetSet);
			}
				GUI_ENC_API_Update(ID_ODC_FirstpageAddr_NetSet, 1);
		}
		break;
	case GUI_VK_UP:
	case GUI_VK_P_UP:
		if(GUI_ENC_API_IsFocus(ID_ODC_IPAddress_NetSet))
		{
		   GUI_ENC_API_SetFocus(ID_CBX_AutoIP_NetSet);
		   GUI_ENC_API_Update(ID_ODC_IPAddress_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_Subnetwork_NetSet))
		{
		  
			GUI_ENC_API_SetFocus(ID_ODC_IPAddress_NetSet);
			GUI_ENC_API_Update(ID_ODC_Subnetwork_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_GatewayIP_NetSet))
		{
			GUI_ENC_API_SetFocus(ID_ODC_Subnetwork_NetSet);
			GUI_ENC_API_Update(ID_ODC_GatewayIP_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstChoseDNS_NetSet))
		{
		   GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
		   GUI_ENC_API_Update(ID_ODC_FirstChoseDNS_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_ReserveDNS_NetSet))
		{
		  
			GUI_ENC_API_SetFocus(ID_ODC_FirstChoseDNS_NetSet);
			GUI_ENC_API_Update(ID_ODC_ReserveDNS_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstpageAddr_NetSet))
		{
		    if(AutoDNS_flag)
			GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
			else
			GUI_ENC_API_SetFocus(ID_ODC_ReserveDNS_NetSet);
			GUI_ENC_API_Update(ID_ODC_FirstpageAddr_NetSet, 1);

		}	
		break;
	case GUI_VK_DOWN:
	case GUI_VK_P_DOWN:
		if(GUI_ENC_API_IsFocus(ID_ODC_IPAddress_NetSet))
		{
		   GUI_ENC_API_SetFocus(ID_ODC_Subnetwork_NetSet);
		   GUI_ENC_API_Update(ID_ODC_IPAddress_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_Subnetwork_NetSet))
		{
		  
			GUI_ENC_API_SetFocus(ID_ODC_GatewayIP_NetSet);
			GUI_ENC_API_Update(ID_ODC_Subnetwork_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_GatewayIP_NetSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
			GUI_ENC_API_Update(ID_ODC_GatewayIP_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstChoseDNS_NetSet))
		{
		   GUI_ENC_API_SetFocus(ID_ODC_ReserveDNS_NetSet);
		   GUI_ENC_API_Update(ID_ODC_FirstChoseDNS_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_ReserveDNS_NetSet))
		{
		  
			GUI_ENC_API_SetFocus(ID_ODC_FirstpageAddr_NetSet);
			GUI_ENC_API_Update(ID_ODC_ReserveDNS_NetSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstpageAddr_NetSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_SwitchItem_NetSet);
			GUI_ENC_API_SetValue(ID_CBX_SwitchItem_NetSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);

			GUI_ENC_API_Update(ID_ODC_FirstpageAddr_NetSet, 1);

		}
		break;
	case GUI_VK_LEFT:
        if(GUI_ENC_API_IsFocus(ID_ODC_FirstpageAddr_NetSet))
		{
			if(NetSet_offset == 0)
			{
				NetSet_offset = 11;
				GUI_ENC_API_SetFocus(ID_ODC_ReserveDNS_NetSet);
			}
			else
			{
				NetSet_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_FirstpageAddr_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_ReserveDNS_NetSet))
		{
			if(NetSet_offset == 0)
			{
				NetSet_offset = 11;
				GUI_ENC_API_SetFocus(ID_ODC_FirstChoseDNS_NetSet);
			}
			else
			{
				NetSet_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_ReserveDNS_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstChoseDNS_NetSet))
		{
			if(NetSet_offset == 0)
			{
				NetSet_offset = 11;
				GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
			}
			else
			{
				NetSet_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_FirstChoseDNS_NetSet, 1);
		}
        else if(GUI_ENC_API_IsFocus(ID_ODC_GatewayIP_NetSet))
		{
			if(NetSet_offset == 0)
			{
				NetSet_offset = 11;
				GUI_ENC_API_SetFocus(ID_ODC_Subnetwork_NetSet);
			}
			else
			{
				NetSet_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_GatewayIP_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_Subnetwork_NetSet))
		{
			if(NetSet_offset == 0)
			{
				NetSet_offset = 11;
				GUI_ENC_API_SetFocus(ID_ODC_IPAddress_NetSet);
			}
			else
			{
				NetSet_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_Subnetwork_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_IPAddress_NetSet))
		{
			if(NetSet_offset == 0)
			{
				NetSet_offset = 11;
				GUI_ENC_API_SetFocus(ID_CBX_AutoIP_NetSet);
			}
			else
			{
				NetSet_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_IPAddress_NetSet, 1);
		}
		break;
	case GUI_VK_RIGHT:
		 if(GUI_ENC_API_IsFocus(ID_ODC_IPAddress_NetSet))
		{
			if(NetSet_offset == 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_Subnetwork_NetSet);
			}
			else
			{
				NetSet_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_IPAddress_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_Subnetwork_NetSet))
		{
			if(NetSet_offset == 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_GatewayIP_NetSet);
			}
			else
			{
				NetSet_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_Subnetwork_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_GatewayIP_NetSet))
		{
			if(NetSet_offset == 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_CBX_AutoDNS_NetSet);
			}
			else
			{
				NetSet_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_GatewayIP_NetSet, 1);
		}
		 else if(GUI_ENC_API_IsFocus(ID_ODC_FirstChoseDNS_NetSet))
		{
			if(NetSet_offset == 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_ReserveDNS_NetSet);
			}
			else
			{
				NetSet_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_FirstChoseDNS_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_ReserveDNS_NetSet))
		{
			if(NetSet_offset == 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_FirstpageAddr_NetSet);
			}
			else
			{
				NetSet_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_ReserveDNS_NetSet, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_FirstpageAddr_NetSet))
		{
			if(NetSet_offset == 11)
			{
				NetSet_offset = 0;
				GUI_ENC_API_SetFocus(ID_CBX_SwitchItem_NetSet);
			}
			else
			{
				NetSet_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_FirstpageAddr_NetSet, 1);
		}
		break;
	case GUI_VK_SELECT:
	case GUI_VK_EXIT:
		NetSet_offset = 0;
		GUI_ENC_API_SwitchState(ID_DLG_Preserve_NetSet, 1);
		break;
	case GUI_VK_RECALL:
		NetSet_offset = 0;
		GUI_ENC_API_SwitchState(ID_DLG_Preserve_NetSet, 1);
		//GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}
}

void UI_APP_PreserveNetset_Enter(void)
{

}
void UI_APP_PreserveNetset_End(void)
{

}
void UI_APP_PreserveNetset_Key(BU32 key)
{
	
	if(key == GUI_VK_SELECT)
	{
       GUI_ENC_API_SwitchState(ID_DLG_Prompt_NetSet, 1);
	}
	else if(key ==GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
	}
}

//保存IP设置信息提示
void UI_APP_PromptNetSet_Enter(void)
{

}


void UI_APP_PromptNetSet_End(void)
{
	BU32 SwitchIndex;
	BU32 AutoipIndex;
	BU32 index;
	
	FYF_FLS_IOCTL_PARAM_s fyf_fls_ioctl_param,fyf_fls_read_param;
	int i = 0;

	GUI_ENC_API_GetValue(ID_CBX_SwitchItem_NetSet,WDT_CBX_SV_INDEX,&SwitchIndex,0);
	APP_SYS_API_SetSysInfo(APP_SYS_SwitchItem_e,SwitchIndex);
	
	GUI_ENC_API_GetValue(ID_CBX_AutoIP_NetSet,WDT_CBX_SV_INDEX,&AutoipIndex,0);
	APP_SYS_API_SetSysInfo(APP_SYS_AutoIP_e,AutoipIndex);
	
	if (AutoipIndex == 0)
	{
		system("killall -9 udhcpc");
		system("udhcpc -V HZCNCSTBV1 -n 3");
		system("killall -9 udhcpc");
		
		memset(temp, 0, 16);
		HI_ETH_IPAddressGet(0, temp);
		charConvert(temp, AutoIPAddress);
		HI_ETH_SubNetmaskGet(0, temp);
		charConvert(temp, AutoSubNetMark);
		HI_ETH_GatewayGet(0, temp);
		charConvert(temp, AutoDefaultGateway);
		
		memset(temp, 0 , 16);
		HI_ETH_DNSGet(0, temp);
		charConvert(temp, AutoFirstChoseDNS);
		charConvert(temp, AutoReserveDNS);

		/*memset(&IpInfomation, 0, sizeof(IpInfomation));
		memcpy(IpInfomation.ipaddr, IPAddress, 16);
		memcpy(IpInfomation.netmask, SubNetMark, 16);
		memcpy(IpInfomation.gateway, DefaultGateway, 16);
		memcpy(IpInfomation.firstdns, FirstChoseDNS, 16);
		memcpy(IpInfomation.secdns, ReserveDNS, 16);
		
		fyf_fls_ioctl_param.buffer = &IpInfomation;
		printf("buffer = %s\n", fyf_fls_ioctl_param.buffer);
		fyf_fls_ioctl_param.bufferSize = sizeof(IpInfomation);
		FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_VODINFO, (BU32)&fyf_fls_ioctl_param);
*/
	}
	else if (AutoipIndex == 1)
	{
		
		memset(&IpInfomation, 0, sizeof(IpInfomation));

		memcpy(IpInfomation.ipaddr, IPAddress, 16);
		memcpy(IpInfomation.netmask, SubNetMark, 16);
		memcpy(IpInfomation.gateway, DefaultGateway, 16);
		memcpy(IpInfomation.firstdns, FirstChoseDNS, 16);
		memcpy(IpInfomation.secdns, ReserveDNS, 16);

		memcpy(g_ip_address, IpInfomation.ipaddr,  16);
		memcpy(g_sub_net_Mark, IpInfomation.netmask,  16);
		memcpy(g_gateway_ip, IpInfomation.gateway,  16);
		memcpy(g_first_chose_dns, IpInfomation.firstdns,  16);
		memcpy(g_reserve_dns, IpInfomation.secdns, 16);
		IpInfomation.netflag = 0xa0;
		SQL_API_SaveToFlash("flash.db");
		//fyf_fls_ioctl_param.buffer = &IpInfomation;
		//printf("buffer = %s\n", fyf_fls_ioctl_param.buffer);
		//fyf_fls_ioctl_param.bufferSize = sizeof(IpInfomation);
		//FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_VODINFO, (BU32)&fyf_fls_ioctl_param);
		HI_ETH_Close(1);
		HI_ETH_Open(0);
		HI_ETH_IPAddressSet(0, IPAddress);
		HI_ETH_SubNetmaskSet(0, SubNetMark);
		HI_ETH_GatewaySet(0, DefaultGateway);
		HI_ETH_DNSSet(0, 1, FirstChoseDNS);

		
	
		
	}
	GUI_ENC_API_GetValue(ID_CBX_AutoDNS_NetSet,WDT_CBX_SV_INDEX,&index,0);
	APP_SYS_API_SetSysInfo(APP_SYS_AutoDNS_e,index);
	APP_SYS_API_SaveSysInfo();
	printf("IPAddress3333=%s\n\n",IPAddress);
	
}
void UI_APP_PromptNetSet_Timer(void)
{
	GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
}


