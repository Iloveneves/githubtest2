/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment of the Url Network and Cursor Porting APIs needed by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#include "ipanel_UrlNetCursor.h"
#include "trd_ipanel_init.h"

#include "../tpmiddle/include/app_sys.h"

//char *ipanel_startpagebuf1="http://fuzhoump2.wasu.cn:8080/tvportal/index.jsp?stbid=21040010102052544c1e643a";
//char *ipanel_startpagebuf2="http://fuzhou-timeshift.wasu.cn:7080/timeShift/fuzhou/index.jsp?stbid=21040010102052544c1e643a&type=mpeg2";
char ipanel_startpagebuf1[] = "http://10.10.0.11/eums/client/template17/shaoguan/index.ecgi";//"http://10.10.0.11/eums/check.ecgi?submit_mac=00606e56bf83";
//char ipanel_startpagebuf1[200]=//huabeiyt.wasu.cn/tvportal_huabeiyt/index_huabeiyt.jsp?stbid=011204003011600024c104190b";
//char *ipanel_startpagebuf2="http://fuzhou-timeshift.wasu.cn:7080/timeShift/fuzhou/index.jsp?stbid=011204003011600024c104190b&type=mpeg2";
//char *ipanel_startpagebuf2 = "http://125.210.216.148:7080/timeShift/huabeiyt/index.jsp?stbid=011204003011600024C104190B&type=mpeg4";
//char ipanel_startpagebuf2[200] = "http://public-timeshift.wasu.cn:7080/timeShift/huabeiyt/index.jsp?stbid=011204003011600024C104190B&type=mpeg4";
char ipanel_startpagebuf2[] = "http://10.10.0.11/eums/client/template17/shiyi/dshk.ecgi";//?submit_mac=00606e56bf83";
MAC_BOX_INFO_STRUCT pstMacBoxInfo;
char *strbuf2="&type=mpeg4-Test";
//MAC_BOX_INFO_STRUCT MacBoxInfo;
//extern char stbid[27];
char stbid[27] = {0};					/*stbid*/ /*hqx:do it later*/

void  ipanel_update_startpagebuf(void)
{
	int  i;
    char *dataPointer = NULL;

	memset(&pstMacBoxInfo, 0, sizeof(MAC_BOX_INFO_STRUCT));

	APP_SYS_API_GetStbID(&pstMacBoxInfo);
	//pstMacBoxInfo = (MAC_BOX_INFO_STRUCT *)&MacBoxInfo;

	/******************处理 ipanel_startpagebuf1***************************/
#if 1
	dataPointer = strstr(ipanel_startpagebuf1,"submit_mac=");
	if (dataPointer != NULL)
	{
		for(i =0 ;i<12 ;i++)
		{
			*(dataPointer+11+i)=pstMacBoxInfo.ucMacAddress[i];
		}
	}

	printf("OK=%s\n",ipanel_startpagebuf1);

#else
	dataPointer = strstr(ipanel_startpagebuf1,"stbid=");

	printf("data = %s\n",dataPointer);
	//return;
	for(i =0 ;i<4 ;i++)
	{
		*(dataPointer+6+i)=pstMacBoxInfo.AreaIndicatorCode[i];
	}

	for(i =0 ;i<2 ;i++)
	{
		*(dataPointer+10+i)=pstMacBoxInfo.ManufactureCode[i];
	}

	for(i =0 ;i<3 ;i++)
	{
		*(dataPointer+12+i)=pstMacBoxInfo.StbFactoryIndicator[i];
	}

	for(i =0 ;i<2 ;i++)
	{
		*(dataPointer+15+i)=pstMacBoxInfo.StbType[i];
	}

	for(i =0 ;i<2 ;i++)
	{
		*(dataPointer+17+i)=pstMacBoxInfo.HwVersion[i];
	}

	*(dataPointer+19)=pstMacBoxInfo.StbIndicatorType[0];

	for(i =0 ;i<12 ;i++)
	{
		*(dataPointer+20+i)=pstMacBoxInfo.ucMacAddress[i];
	}

	if (pstMacBoxInfo.ucMacAddress[10]=='0' && pstMacBoxInfo.ucMacAddress[11]=='0')
	{ 
	   *(dataPointer+20+10)=0;
	}
	else
	{ 
	   *(dataPointer+20+12)=0;
	}
	
	printf("OK=%s\n",ipanel_startpagebuf1);

	/******************处理 ipanel_startpagebuf2***************************/
	dataPointer = strstr(ipanel_startpagebuf2,"stbid=");

	for(i =0 ;i<4 ;i++)
	{
		*(dataPointer+6+i)=pstMacBoxInfo.AreaIndicatorCode[i];
	}

	for(i =0 ;i<2 ;i++)
	{
		*(dataPointer+10+i)=pstMacBoxInfo.ManufactureCode[i];
	}

	for(i =0 ;i<3 ;i++)
	{
		*(dataPointer+12+i)=pstMacBoxInfo.StbFactoryIndicator[i];
	}

	for(i =0 ;i<2 ;i++)
	{
		*(dataPointer+15+i)=pstMacBoxInfo.StbType[i];
	}

	for(i =0 ;i<2 ;i++)
	{
		*(dataPointer+17+i)=pstMacBoxInfo.HwVersion[i];
	}

	*(dataPointer+19)=pstMacBoxInfo.StbIndicatorType[0];

	for(i =0 ;i<12 ;i++)
	{
		*(dataPointer+20+i)=pstMacBoxInfo.ucMacAddress[i];
	}

	if (pstMacBoxInfo.ucMacAddress[10]=='0' && pstMacBoxInfo.ucMacAddress[11]=='0')
	{  
	   for(i =0 ;i<11 ;i++)
		   *(dataPointer+20+10+i)=strbuf2[i];
	   *(dataPointer+20+10+11)=0;
	}
	else
	{
	   for(i =0 ;i<11 ;i++)
		   *(dataPointer+20+12+i)=strbuf2[i];
	   *(dataPointer+20+12+11)=0;
	}
#endif

	dataPointer = strstr(ipanel_startpagebuf2,"submit_mac=");
	if (dataPointer != NULL)
	{
		for(i =0 ;i<12 ;i++)
		{
			*(dataPointer+11+i)=pstMacBoxInfo.ucMacAddress[i];
		}
	}
	printf("OK=%s\n",ipanel_startpagebuf2);

}



/**********************************************************************************/
/*Description: iWM Get Start Page                                                 */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : start page string                                                  */
/**********************************************************************************/
const char *ipanel_porting_get_startpage(void)
{
    IPANEL_DEBUG("\n");

	//return "http://www.baidu.com";
    //return "http://192.168.1.24/index.htm";
	//return "http://www.sohu.com";

	if(Ipanel_GetEnterVodOrTvod_Flag() == 2)//进点播
	{
		printf("============istartpage is buf = %s!!!\n",ipanel_startpagebuf1);
		ipanel_startpagebuf1;
	}
	else if(Ipanel_GetEnterVodOrTvod_Flag() == 1)//进时移
	{
		printf("============istartpage is buf = %s!!!\n",ipanel_startpagebuf2);
		return ipanel_startpagebuf2;
	}
	else
	{
		return NULL;
	}
		//return "http://192.168.1.24/index.htm";
		//return "http://www.google.cn";
		//printf("%s\n",ipanel_startpagebuf1);
		//return ipanel_startpagebuf1;


}


int ipanel_porting_set_startpage(const char *url)
{
    IPANEL_DEBUG("\n");
	if(Ipanel_GetEntrance_Flag())
	{
		return NULL;
		//printf("============istartpage is buf = %s!!!\n",ipanel_startpagebuf1);
		//return ipanel_startpagebuf1;
	}
	else
	{	
		return NULL;
	}
}

const char *ipanel_porting_get_helppage(void)
{
	return "test";
}

/**********************************************************************************/
/*Description: iWM Get Home Page                                                  */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : home page string                                                   */
/**********************************************************************************/
const char *ipanel_porting_get_homepage(void)
{
    IPANEL_DEBUG("\n");
	if ( Ipanel_GetEntrance_Flag() == FYF_TRUE)
	{
		//return "http://192.168.1.24/index.htm";
		//return "http://www.google.cn";
		//printf("%s\n",ipanel_startpagebuf1);
		return ipanel_startpagebuf1;
	}
	else
	{
	    return NULL;
	}
}


/**********************************************************************************/
/*Description: iWM set Home Page                                                  */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : home page string                                                   */
/**********************************************************************************/
int ipanel_porting_set_homepage(const char *url)
{
    IPANEL_DEBUG("\n");
#if 0
    return "http://192.168.10.207";
#else
    return NULL;
#endif
}


/**********************************************************************************/
/*Description: Connect network device                                             */
/*Input      : device name string                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- connect success, -1 -- fail                                   */
/**********************************************************************************/
int ipanel_porting_network_connect(const char *devname)
{
    IPANEL_DEBUG("aaa\n");
    int ret = -1;

    if (!strcmp(devname, "lan") || !strcmp(devname, "dialup") || !strcmp(devname, "pppoe")) 
    {
        /*connect device*/
        /*if success or had connected*/
        ret = 0;
    }

    return ret;
}


/**********************************************************************************/
/*Description: Disconnect network device                                          */
/*Input      : device name string                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- disconnect success, -1 -- fail                                */
/**********************************************************************************/
int ipanel_porting_network_disconnect(const char *devname)
{
    IPANEL_DEBUG("aaa\n");
    int ret = -1;

    if (!strcmp(devname, "lan") || !strcmp(devname, "dialup") || !strcmp(devname, "pppoe")) 
    {
        /*disconnect device*/
        /*if success or had connected*/
        ret = 0;
    }

    return ret;
}


/**********************************************************************************/
/*Description: Get network parameter                                              */
/*Input      : device name, property name, buffer&length to save parameter        */
/*Output     : parameter                                                          */
/*Return     : parameter length, or 0                                             */
/**********************************************************************************/
int ipanel_porting_network_get_parameter(const char *devname, const char *name, char *buf, int len)
{
    IPANEL_DEBUG("aaa\n");
    int nbytes = 0;

    if (!strcmp(devname, "lan")) 
    {
        if (!strcmp(name, "status")) 
        {
            if(len>=7) 
            {
            memcpy(buf, "connect", 7);
            nbytes = 7;
            }
        /*or 
        if(len>=10) {
        memcpy(buf,"disconnect", 10);
        nbytes = 10;
        }
        */
        }
        else if (!strcmp(name, "ipaddr")) 
        {
            if(len>=14) 
            {
                memcpy(buf, "192.168.10.121", 14);
                nbytes = 14;
            }
        }
        else if (!strcmp(name, "netmask")) 
        {
            if(len>=15) 
            {
                memcpy(buf, "255.255.255.128", 15);
                nbytes = 15;
            }
        }
        else if (!strcmp(name, "gateway")) 
        {
            if(len>=12)
            {
                memcpy(buf, "192.168.10.1", 12);
                nbytes = 12;
            }
        }
        else if (!strcmp(name, "nameserver")) 
        {
            if(len>=14) 
            {
                memcpy(buf, "192.168.10.241", 14);
                nbytes = 14;
            }
        }
        else if (!strcmp(name, "nameserver2")) 
        {
            if(len>=14) 
            {
                memcpy(buf, "192.168.10.242", 14);
                nbytes = 14;
            }
        }
        else if (!strcmp(name, "dhcp")) 
        {
            if(len>=1) 
            {
            memcpy(buf, "1", 1);
            nbytes = 1;
            }
        /*or 
        if(len>=1) {
        memcpy(buf,"0", 1);
        nbytes = 1;
        }
        */
        }
        else if (!strcmp(name, "proxy")) 
        {
            if(len>=24) 
            {
                memcpy(buf, "http://192.168.10.1:8080", 24);
                nbytes = 24;
            }
        }
        else if (!strcmp(name, "proxyflag")) 
        {
            if(len>=1) 
            {
                memcpy(buf, "1", 1);
                nbytes = 1;
            }
            /*or 
            if(len>=1) {
            memcpy(buf,"0", 1);
            nbytes = 1;
            }
            */
        }
        else if (!strcmp(name, "username")) 
        {
            if(len>=6) 
            {
                memcpy(buf, "ipanel", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "password")) 
        {
            if(len>=6) 
            {
                memcpy(buf, "123456", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "unproxyurls")) 
        {
            if(len>=29)
            {
                memcpy(buf, "http://192.168.10.1/proxy.htm", 29);
                nbytes = 29;
            }
        }
    }
    else if (!strcmp(devname, "dialup")) 
    {
        if (!strcmp(name, "status")) 
        {
            if(len>=7) 
            {
                memcpy(buf, "connect", 7);
                nbytes = 7;
            }
            /*or 
            if(len>=10) {
            memcpy(buf,"disconnect", 10);
            nbytes = 10;
            }
            or 
            idle, dialing, no dialtone, 
            dialing, no modem connection, 
            line busy, PPP failed, disconnecting,
            PPP Handshaking, connection dropped, delay error
            */
        }
        else if (!strcmp(name, "username")) 
        {
            if(len>=6) 
            {
                memcpy(buf, "ipanel", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "password"))
        {
            if(len>=6) 
            {
                memcpy(buf, "123456", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "phonenumber")) 
        {
            if(len>=11) 
            {
                memcpy(buf, "075582374586", 11);
                nbytes = 11;
            }
        }
        else if (!strcmp(name, "nameserver")) 
        {
            if(len>=14) 
            {
                memcpy(buf, "192.168.10.241", 14);
                nbytes = 14;
            }
        }
        else if (!strcmp(name, "nameserver2")) 
        {
            if(len>=14) 
            {
                memcpy(buf, "192.168.10.242", 14);
                nbytes = 14;
            }
        }
        else if (!strcmp(name, "idletime")) 
        {
            if(len>=3)
            {
                memcpy(buf, "300", 3);
                nbytes = 3;
            }
        }
        else if (!strcmp(name, "dialcount")) 
        {
            if(len>=2)
            {
                memcpy(buf, "10", 2);
                nbytes = 2;
            }
        }
        else if (!strcmp(name, "speed")) 
        {
            if(len>=6)
            {
                memcpy(buf, "128000", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "atcommand")) 
        {
            if(len>=3) 
            {
                memcpy(buf, "atd", 3);
                nbytes = 3;
            }
        }
        else if (!strcmp(name, "proxy"))
        {
            if(len>=24) 
            {
                memcpy(buf, "http://192.168.10.1:8080", 24);
                nbytes = 24;
            }
        }
        else if (!strcmp(name, "proxyflag")) 
        {
            if(len>=1) 
            {
                memcpy(buf, "1", 1);
                nbytes = 1;
            }
            /*or 
            if(len>=1) {
            memcpy(buf,"0", 1);
            nbytes = 1;
            }
            */
        }
    }
    else if (!strcmp(devname, "pppoe")) 
    {
        if (!strcmp(name, "status")) 
        {
            if(len>=7) 
            {
                memcpy(buf, "connect", 7);
                nbytes = 7;
            }
            /*or 
            if(len>=10) {
            memcpy(buf,"disconnect", 10);
            nbytes = 10;
            }
            */
        }
        else if (!strcmp(name, "username")) 
        {
            if(len>=6) 
            {
                memcpy(buf, "ipanel", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "password")) 
        {
            if(len>=6) 
            {
                memcpy(buf, "123456", 6);
                nbytes = 6;
            }
        }
        else if (!strcmp(name, "timeout")) 
        {
            if(len>=3)
            {
                memcpy(buf, "900", 3);
                nbytes = 3;
            }
        }
    }

    return nbytes;
}


/**********************************************************************************/
/*Description: Set network parameter                                              */
/*Input      : device name, property name, parameter                              */
/*Output     : No                                                                 */
/*Return     : 0  -- success, -1 -- fail.                                         */
/**********************************************************************************/
int ipanel_porting_network_set_parameter(const char *devname, const char *name, const char *val)
{
    IPANEL_DEBUG("aaa\n");
    int ret = -1;

    if (!strcmp(devname, "lan")) 
    {
        if (!strcmp(name, "ipaddr") && strlen(val)<=40) 
        {
    /*set physical ip address*/
        ret = 0;
        }
        else if (!strcmp(name, "netmask") && strlen(val)<=40) 
        {
        /*set physical ip netmask*/
            ret = 0;
        }
        else if (!strcmp(name, "gateway") && strlen(val)<=40) 
        {
            /*set physical ip gateway*/
            ret = 0;
        }
        else if (!strcmp(name, "nameserver") && strlen(val)<=40) {
        /*set physical ip nameserver*/
        ret = 0;
        }
        else if (!strcmp(name, "nameserver2") && strlen(val)<=40) 
        {
            /*set physical ip nameserver2*/
            ret = 0;
        }
        else if (!strcmp(name, "dhcp") && strlen(val)<=8) 
        {
            /*set physical DHCP is effect or not*/
            ret = 0;
        }
        else if (!strcmp(name, "proxy") && strlen(val)<=40) 
        {
            /*set physical ip proxy*/
            ret = 0;
        }
        else if (!strcmp(name, "proxyflag") && strlen(val)<=8) 
        {
            /*set physical PROXY is effect or not*/
            ret = 0;
        }
        else if (!strcmp(name, "username") && strlen(val)<=40) 
        {
            /*set physical ip user name*/
            ret = 0;
        }
        else if (!strcmp(name, "password") && strlen(val)<=40) 
        {
            /*set physical ip user password*/
            ret = 0;
        }
        else if (!strcmp(name, "unproxyurls") && strlen(val)<=250) 
        {
            /*set physical ip user proxy url page*/
            ret = 0;
        }
    }
    else if (!strcmp(devname, "dialup")) 
    {
        if (!strcmp(name, "username") && strlen(val)<=40)
        {
            /*set physical ip user name*/
            ret = 0;
        }
        else if (!strcmp(name, "password") && strlen(val)<=40) 
        {
            /*set physical ip user password*/
            ret = 0;
        }
        else if (!strcmp(name, "phonenumber") && strlen(val)<=40) 
        {
            /*set physical ip user phonenumber*/
            ret = 0;
        }
        else if (!strcmp(name, "nameserver") && strlen(val)<=40) 
        {
            /*set physical ip nameserver*/
            ret = 0;
        }
        else if (!strcmp(name, "nameserver2") && strlen(val)<=40) 
        {
            /*set physical ip nameserver2*/
            ret = 0;
        }
        else if (!strcmp(name, "idletime") && strlen(val)<=40)
        {
            /*set physical idle time, uint is second*/
            ret = 0;
        }
        else if (!strcmp(name, "dialcount") && strlen(val)<=40) 
        {
            /*set physical dial count*/
            ret = 0;
        }
        else if (!strcmp(name, "proxy") && strlen(val)<=40) 
        {
            /*set physical ip proxy*/
            ret = 0;
        }
        else if (!strcmp(name, "proxyflag") && strlen(val)<=8)
        {
            /*set physical PROXY is effect or not*/
            ret = 0;
        }
    }
    else if (!strcmp(devname, "pppoe"))
    {
        if (!strcmp(name, "username") && strlen(val)<=40) 
        {
            /*set physical ip user name*/
            ret = 0;
        }
        else if (!strcmp(name, "password") && strlen(val)<=40) 
        {
            /*set physical ip user password*/
            ret = 0;
        }
        else if (!strcmp(name, "timeout") && strlen(val)<=40) 
        {
            /*set physical timeout*/
            ret = 0;
        }
    }
    return ret;
}



/**********************************************************************************/
/*Description: Get cursor position, Note the origin position is in the left and   */
/*             top of the screen .                                                */
/*Input      : the address to save x and y.                                       */
/*Output     : x and y value                                                      */
/*Return     : No                                                                 */
/**********************************************************************************/
void ipanel_porting_cursor_get_position(int *x, int *y)
{
    IPANEL_DEBUG("\n");

    /*the default position of cursor is in center of the TV screen.*/
    *x = SCREENWIDTH/2;
    *y = SCREENHEIGTH/2;
}



/**********************************************************************************/
/*Description: Set cursor position, Note the origin position is in the left and   */
/*             top of the screen .                                                */
/*Input      : x and y.                                                           */
/*Output     : No                                                                 */
/*Return     : No                                                                 */
/**********************************************************************************/
void ipanel_porting_cursor_set_position(int x, int y)
{
    IPANEL_DEBUG("\n");

    if (x>=0 && x<=SCREENWIDTH && y>=0 && y<=SCREENHEIGTH) {
        /*set physical cursor position here*/
    }
}

/**********************************************************************************/
/*Description: iWM Get Cursor(1~4)shape: 1)arrow head 2)hand 3)hourglass 4)I type */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : Cursor(1~4)shape                                                   */
/**********************************************************************************/
int ipanel_porting_cursor_get_shape(void)
{
    IPANEL_DEBUG("aaa\n");return 1; /*arrow head is the default value*/
    /*you can use following code
    return 2; //hand
    or
    return 3; //hourglass
    or
    return 4; //I type
    */
}


/**********************************************************************************/
/*Description: Set Cursor(1~4)shape: 1)arrow head 2)hand 3)hourglass 4)I type     */
/*Input      : Cursor(1~4)shape                                                   */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_cursor_set_shape(int shape)
{
    //IPANEL_DEBUG("aaa\n");
    int ret = -1;

    if (shape>0 && shape<=4) 
    {
        /*Set physical cursor here.*/
        ret = 0;
    }

    return ret;
}



