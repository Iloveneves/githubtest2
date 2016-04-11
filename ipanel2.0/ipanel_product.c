/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment of the Product Config Porting APIs needed by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/
#include "ipanel_product.h"
#include "trd_ipanel_init.h"

char dnstmp[20] = {0};

/**********************************************************************************/
/*Description: get current network ID or host ID, max length is 16 bytes          */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : current network ID or host ID                                      */
/**********************************************************************************/
const char *ipanel_porting_get_host(void)
{IPANEL_DEBUG("aaa\n");
	return "myHost-iPanel";
}


/**********************************************************************************/
/*Description: Set Current Network ID or Host ID, max length is 16 bytes          */
/*Input      : Current Network ID or Host ID string                               */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_host(const char *host)
{IPANEL_DEBUG("aaa\n");
	int ret = -1, len = 0;
	len = strlen(host);
	if (len > 0 && len <= 16) {
		/*set current network ID or host ID here*/		
		ret = 0;
	}
	return ret;
}


/**********************************************************************************/
/*Description: get current user device unique ID                                  */
/*Input      : No                                                                 */
/*Output     : no                                                                 */
/*Return     : user ID                                                            */
/**********************************************************************************/
const char *ipanel_porting_get_userid(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel-001-123";
}


/**********************************************************************************/
/*Description: get current network device description, max length is 32 bytes     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : current network device description string                          */
/**********************************************************************************/
const char *ipanel_porting_get_description(void)
{IPANEL_DEBUG("aaa\n");
	return "my_network_lan_iPanel";
}


/**********************************************************************************/
/*Description: get NIC physical MAC address, format must be "xx:xx:xx:xx:xx:xx"   */
/*             where x --[0-9 a-f A-F]                                            */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : NIC MAC address string                                             */
/**********************************************************************************/
const char *ipanel_porting_get_macaddress(void)
{IPANEL_DEBUG("aaa\n");
	return "00:00:00:00:00:00";
}


/**********************************************************************************/
/*Description: get current DHCP work mode, 0 not use DHCP, 1 use DHCP             */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 or 1                                                             */
/**********************************************************************************/
int ipanel_porting_get_DHCP(void)
{IPANEL_DEBUG("aaa\n");
	return 1;
}


/**********************************************************************************/
/*Description: set current DHCP work mode                                         */
/*Input      : DHCP work mode, 0 not use DHCP, 1 use DHCP                         */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_DHCP(int dhcp)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if ((dhcp == 0) || (dhcp == 1))
	{
		/*set physical dhcp here.*/
		ret = 0;
	}
		
	return ret;
}


/**********************************************************************************/
/*Description: get current network terminal ip, format such as "xxx.xxx.xxx.xxx"  */
/*             where x --[0-9]                                                    */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string                                                          */
/**********************************************************************************/
const char *ipanel_porting_get_IPaddress(void)
{IPANEL_DEBUG("aaa\n");
	return "192.168.10.125";
}


/**********************************************************************************/
/*Description: set current network terminal ip address                            */
/*Input      : ip string, format such as "xxx.xxx.xxx.xxx" , where x --[0-9]      */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_IPaddress(const char *ip)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	/*set physical ip address here*/
	ret = 0;		
	return ret;
}

const char *ipanel_porting_get_STBapp_version(void)
{
	static char version_inf[40];
	struct tm *local; 
	time_t t; 
	
	t = time(NULL); 
	local=localtime(&t);
	sprintf(version_inf,"newland.%d.%d.%d.%d.%d.%d",local->tm_year,
													local->tm_mon,
													local->tm_mday,
													local->tm_hour,
													local->tm_min,
													local->tm_sec);     //±£´æËùÓÐ×Ö·û

	printf("version_inf = %s\n",version_inf);

	return version_inf;

}

int ipanel_porting_get_utc_time(EisTime *tm)
{
	struct timespec tp;
	int rs;
	rs = clock_gettime(CLOCK_REALTIME, &tp);
	tm->tm_sec = tp.tv_sec;
	tm->tm_usec = (tp.tv_nsec)/1000;
	return 0;
}
/**********************************************************************************/
/*Description: get current network terminal subnet mask, format such as           */
/*             "xxx.xxx.xxx.xxx"   where x --[0-9]                                */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : subnet mask                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_mask(void)
{IPANEL_DEBUG("aaa\n");
	return "255.255.255.0";
}


/**********************************************************************************/
/*Description: set current network terminal subnet mask                           */
/*Input      : ip string, format such as "xxx.xxx.xxx.xxx"   where x --[0-9]      */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_mask(const char *mask)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	/*set physical subnet mask here.*/
	ret = 0;
	return ret;
}


/**********************************************************************************/
/*Description: get current network terminal gateway ip                            */
/*             format such as "xxx.xxx.xxx.xxx"   where x --[0-9]                 */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string                                                          */
/**********************************************************************************/
const char *ipanel_porting_get_gateway(void)
{IPANEL_DEBUG("aaa\n");
	return "192.168.10.1";
}


/**********************************************************************************/
/*Description: set current network terminal gateway ip address                    */
/*Input      : ip string, format such as "xxx.xxx.xxx.xxx"   where x --[0-9]      */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_gateway(const char *gateway)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	/*set physical gateway here.*/
	ret = 0;
	return ret;
}


/**********************************************************************************/
/*Description: get current network offered DHCP server ip,                        */
/*             format such as "xxx.xxx.xxx.xxx"   where x --[0-9]                 */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string                                                          */
/**********************************************************************************/
const char *ipanel_porting_get_dhcpserver(void)
{IPANEL_DEBUG("aaa\n");
	return "192.168.10.252";
}


/**********************************************************************************/
/*Description: get current network offered master DNS server ip,                  */
/*             format such as "xxx.xxx.xxx.xxx"   where x --[0-9]                 */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string                                                          */
/**********************************************************************************/
const char *ipanel_porting_get_DNSserver1(void)
{	
	if (OW_VOD_GetDhcpFlag())
	{
		FYF_API_ETH_DNSGet(dnstmp);
	}
	else
	{
		
	}
	

	if (0 == strlen(dnstmp))
	{
		strcpy(dnstmp, "0.0.0.0");
	}

	//strcpy(dnstmp, "218.108.243.11");

	IPANEL_DEBUG("dns = %s\n", dnstmp);

	//return "218.108.243.11";
	return (char *)dnstmp;
	//return "218.85.157.99";
	//return FYF_API_GetDnsServer();
}


/**********************************************************************************/
/*Description: set current network offered master DNS server ip address           */
/*Input      : ip string, format such as "xxx.xxx.xxx.xxx"   where x --[0-9]      */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_DNSserver1(const char *dns1)
{
	int ret = -1;
	/*set physical master DNS here.*/
	IPANEL_DEBUG("\n");
	ret = 0;
	return ret;
}


/**********************************************************************************/
/*Description: get current network offered slave DNS server ip,                   */
/*             format such as "xxx.xxx.xxx.xxx"   where x --[0-9]                 */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string                                                          */
/**********************************************************************************/
const char *ipanel_porting_get_DNSserver2(void)
{
	if (OW_VOD_GetDhcpFlag())
	{
		FYF_API_ETH_DNSGet(dnstmp);
	}
	else
	{
		
	}
	

	if (0 == strlen(dnstmp))
	{
		strcpy(dnstmp, "0.0.0.0");
	}
	//strcpy(dnstmp, "218.108.243.11");

	IPANEL_DEBUG("dns = %s\n", dnstmp);
	//return NULL;
	//return "218.108.243.11";

	return dnstmp;
}


/**********************************************************************************/
/*Description: set current network offered slave DNS server ip address            */
/*Input      : ip string, format such as "xxx.xxx.xxx.xxx"   where x --[0-9]      */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_DNSserver2(const char *dns2)
{
	int ret = -1;
	/*set physical slave DNS here.*/
	ret = 0;
	IPANEL_DEBUG("\n");
	return ret;
}


/**********************************************************************************/
/*Description: get the time of obtained ip in DHCP mode, must as "HH:MM:SS"       */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : time string                                                        */
/**********************************************************************************/

/*!
 * \author wqs,lipan
 *
 */
const char *ipanel_porting_get_obtained(void)
{IPANEL_DEBUG("aaa\n");
	return "00:01:23";
}


/**********************************************************************************/
/*Description: get the time of ip expires in DHCP mode, must as "HH:MM:SS"        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : time string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_expires(void)
{IPANEL_DEBUG("aaa\n");
	return "10:00:00";
}


/**********************************************************************************/
/*Description: get current network NTP server ip or domain                        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string or domain string                                         */
/**********************************************************************************/
const char *ipanel_porting_get_NTPserver(void)
{IPANEL_DEBUG("aaa\n");
	return "163.164.24.32";
}


/**********************************************************************************/
/*Description: set current network NTP server ip or domain                        */
/*Input      : ip string or domain string no more than 32 bytes                   */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_set_NTPserver(const char *ntp)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
//	if (is_ip_string(ntp))
//	{
//		ret = set_ntp_server_ip(ntp);
//	}
//	else if (is_domain_string(ntp))
//	
//	{
//		ret = set_ntp_server_domain(ntp);
//	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get the difference of local time and base time   (0 ~ 23)          */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : difference of local time and base time                             */
/**********************************************************************************/
int ipanel_porting_get_timezone(void)
{
    IPANEL_DEBUG("\n");	
	
	return 8;
}


/**********************************************************************************/
/*Description: set the difference of local time and base time                     */
/*Input      : difference of local time and base time (0 ~ 23)                    */
/*Output     : no                                                                 */
/*Return     : 0 -- success, -1 ---fail                                           */
/**********************************************************************************/
int ipanel_porting_set_timezone(int timezone)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if ((timezone >0) && (timezone < 24))
	{
		//ret = set_timezone(timezone);
		ret = 0;
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get dialup mode, 0 not user dialup, 1 PPP, 2 PPPoE, other no effect*/
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : dialup mode                                                        */
/**********************************************************************************/
int ipanel_porting_get_dialup(void)
{IPANEL_DEBUG("aaa\n");
	return 2;
}


/**********************************************************************************/
/*Description: set dialup mode                                                    */
/*Input      : 0 not user dialup, 1 PPP, 2 PPPoE, other no effect                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_dialup(int type)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if ((type>=1) && (type<=2))
	{
	//	ret = set_dialup(type);
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get current dialup mode user name, string no more the 16 bytes     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : user name string                                                   */
/**********************************************************************************/
const char *ipanel_porting_get_dialup_user(void)
{IPANEL_DEBUG("aaa\n");
	return "user1";
}


/**********************************************************************************/
/*Description: set current dialup mode user name                                  */
/*Input      : string no more the 16 bytes                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_dialup_user(const char *usr)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if (strlen(usr) <= 16)
	{
		//ret = set_dialup_user(usr);
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get current dialup mode user password, string no more the 16 bytes */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : password string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_dialup_password(void)
{IPANEL_DEBUG("aaa\n");
	return "123456";
}


/**********************************************************************************/
/*Description: set current dialup mode user password                              */
/*Input      : string no more the 16 bytes                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_dialup_password(const char *pwd)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if (strlen(pwd) <= 16)
	{
		//ret = set_dialup_password(pwd);
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get current offered proxy server ip or domain,                     */
/*             string as "192.168.10.228"                                         */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip or domain string                                                */
/**********************************************************************************/
const char *ipanel_porting_get_proxy_server(void)
{IPANEL_DEBUG("aaa\n");
	return "192.168.10.228";
}


/**********************************************************************************/
/*Description: set current offered proxy server ip or domain                      */
/*Input      : string as "192.168.10.228" or "nero.hkbn.net.hk"                   */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_proxy_server(const char *proxy)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
//	if (is_ip_string(proxy))
//	{
//		ret = set_ntp_server_ip(proxy);
//	}
//	else if (is_domain_string(proxy))
//	
//	{
//		ret = set_ntp_server_domain(proxy);
//	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get current offered proxy server port                              */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : port number                                                        */
/**********************************************************************************/
int ipanel_porting_get_proxy_port(void)
{IPANEL_DEBUG("aaa\n");
	return 1152;
}


/**********************************************************************************/
/*Description: set current offered proxy server port                              */
/*Input      : port number                                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_proxy_port(int port)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	// ret = set_proxy_port(port);
	
	return ret;
}


/**********************************************************************************/
/*Description: get current offered proxy serve user name,                         */ 
/*             string no more the 16 bytes                                        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : name string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_proxy_user(void)
{IPANEL_DEBUG("aaa\n");
	return "user";
}


/**********************************************************************************/
/*Description: set current offered proxy serve user name                          */
/*Input      : string no more the 16 bytes                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_proxy_user(const char *usr)
{IPANEL_DEBUG("aaa\n");
	int ret = -1, len;

	len = strlen(usr);
	if (len>0 && len<=16)
	{
		/*set real proxy user here.*/
		ret = 0;
	}
	
	return ret;
}



/**********************************************************************************/
/*Description: get current offered proxy serve user password,                     */
/*             string no more the 16 bytes                                        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : password string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_proxy_password(void)
{IPANEL_DEBUG("aaa\n");
	return "123456";
}


/**********************************************************************************/
/*Description: set current offered proxy serve user password                      */
/*Input      : string no more the 16 bytes                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_proxy_password(const char *pwd)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if (strlen(pwd) <= 16)
	{
		/*set real proxy user password here.*/
		ret = 0;
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get current system loader name, string no more the 40 bytes        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : loader name string                                                 */
/**********************************************************************************/
const char *ipanel_porting_get_loader_name(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel_loader_name";
}


/**********************************************************************************/
/*Description: get current system loader version, string no more the 40 bytes     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : loader version string                                              */
/**********************************************************************************/
const char *ipanel_porting_get_loader_version(void)
{IPANEL_DEBUG("aaa\n");
	return "2.0.0.2";
}


/**********************************************************************************/
/*Description: get current system loader provider, string no more the 40 bytes    */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : provider string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_loader_provider(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel_loader_provider";
}


/**********************************************************************************/
/*Description: get current system loader size, string no more the 40 bytes        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_loader_size(void)
{IPANEL_DEBUG("aaa\n");
	return "204800";
}


/**********************************************************************************/
/*Description: get current system driver name, string no more the 40 bytes        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : driver name string                                                 */
/**********************************************************************************/
const char *ipanel_porting_get_driver_name(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel_driver_name";
}


/**********************************************************************************/
/*Description: get current system driver version, string no more the 40 bytes     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : driver versin string                                               */
/**********************************************************************************/
const char *ipanel_porting_get_driver_version(void)
{IPANEL_DEBUG("aaa\n");
	return "3.2.1";
}


/**********************************************************************************/
/*Description: get current system driver provider, string no more the 40 bytes    */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : driver provider string                                             */
/**********************************************************************************/
const char *ipanel_porting_get_driver_provider(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel_driver_provider";
}


/**********************************************************************************/
/*Description: get current system driver size, string no more the 40 bytes        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : driver size string                                                 */
/**********************************************************************************/
const char *ipanel_porting_get_driver_size(void)
{IPANEL_DEBUG("aaa\n");
	return "204800";
}

#if 0
/**********************************************************************************/
/*Description: get current OS name, string no more the 40 bytes                   */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : OS name string                                                     */
/**********************************************************************************/
const char *ipanel_porting_get_os_name(void)
{
	return "iPanel_OS_name";
}


/**********************************************************************************/
/*Description: get current OS version, string no more the 40 bytes                */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : OS version string                                                  */
/**********************************************************************************/
const char *ipanel_porting_get_os_version(void)
{
	return "ipanel-v2.0.0.0";
}


/**********************************************************************************/
/*Description: get current OS provider, string no more the 40 bytes               */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : provider string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_os_provider(void)
{
	return "iPanel_OS_provider";
}


/**********************************************************************************/
/*Description: get current OS size, string no more the 40 bytes                   */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_os_size(void)
{
	return "102400";
}


/**********************************************************************************/
/*Description: get current middleware name, string no more the 40 bytes           */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : name string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_MWclient_name(void)
{
	return "iPanel_MW_name";
}


/**********************************************************************************/
/*Description: get current middleware version, string no more the 40 bytes        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : version string                                                     */
/**********************************************************************************/
const char *ipanel_porting_get_MWclient_version(void)
{
	return "0.0.00";
}


/**********************************************************************************/
/*Description: get current middleware provider, string no more the 40 bytes       */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : provider string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_MWclient_provider(void)
{
	return "iPanel_MW_provider";
}


/**********************************************************************************/
/*Description: get current middleware size, string no more the 40 bytes           */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_MWclient_size(void)
{
	return "1048576";
}


/**********************************************************************************/
/*Description: get current application layer name, string no more the 40 bytes    */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : name string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_application_name(void)
{
	return "iPanel_app_layer_name";
}

/**********************************************************************************/
/*Description: get current application layer version, string no more the 40 bytes */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : version string                                                     */
/**********************************************************************************/
const char *ipanel_porting_get_application_version(void)
{
	return "1.1.1";
}


/**********************************************************************************/
/*Description: get current application layer provider, string no more the 40 bytes*/
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : provider name string                                               */
/**********************************************************************************/
const char *ipanel_porting_get_application_provider(void)
{
	return "iPanel_app_layer_provider";
}


/**********************************************************************************/
/*Description: get current application layer size, string no more the 40 bytes    */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_application_size(void)
{
	return "4009600";
}
#endif

/**********************************************************************************/
/*Description: get current CA name, string no more the 40 bytes                   */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : name string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_ca_name(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel_CA_name";
}


/**********************************************************************************/
/*Description: get current CA version, string no more the 40 bytes                */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : version string                                                     */
/**********************************************************************************/
const char *ipanel_porting_get_ca_version(void)
{IPANEL_DEBUG("aaa\n");
	return "0.0.12";
}


/**********************************************************************************/
/*Description: get current CA provider, string no more the 40 bytes               */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : provider string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_ca_provider(void)
{IPANEL_DEBUG("aaa\n");
	return "iPanel_rpovider";
}


/**********************************************************************************/
/*Description: get current CA size, string no more the 40 bytes                   */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_ca_size(void)
{IPANEL_DEBUG("aaa\n");
	return "11534244";
}

#if 0
/**********************************************************************************/
/*Description: get product brand, string no more the 40 bytes                     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : brand string                                                       */
/**********************************************************************************/
const char *ipanel_porting_get_product_brand(void)
{
	return "iPanel_brand";
}


/**********************************************************************************/
/*Description: get product name, string no more the 40 bytes                      */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : name string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_product_name(void)
{
	return "iPanel_product";
}


/**********************************************************************************/
/*Description: get product cpu type, string no more the 40 bytes                  */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : type name string                                                   */
/**********************************************************************************/
const char *ipanel_porting_get_product_SOC(void)
{
	return "STi55166";
}


/**********************************************************************************/
/*Description: get product PCB ID, string no more the 40 bytes                    */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : PCB ID string                                                      */
/**********************************************************************************/
const char *ipanel_porting_get_product_PCB(void)
{
	return "dbau1200";
}
#endif

/**********************************************************************************/
/*Description: get product serial number, string no more the 32 bytes             */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : serial number string                                               */
/**********************************************************************************/
const char *ipanel_porting_get_product_serial(void)
{IPANEL_DEBUG("aaa\n");
	return "12345678901234567890";
}


/**********************************************************************************/
/*Description: get product flash size, int bytes                                  */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size in bytes                                                      */
/**********************************************************************************/
int ipanel_porting_get_product_flash(void)
{IPANEL_DEBUG("aaa\n");
	return 0x200000;
}


/**********************************************************************************/
/*Description: get product RAM size, in bytes                                     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : size in bytes */
/**********************************************************************************/
int ipanel_porting_get_product_RAM(void)
{IPANEL_DEBUG("aaa\n");
	return 0x2000000;
}


/**********************************************************************************/
/*Description: get product upgrade mode, 1: DvbSi£¬2: Unicast£¬3: remote£¬4: local*/
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : upgrade mode                                                       */
/**********************************************************************************/
int ipanel_porting_get_upgrade_mode(void)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	// ret = get_upgrade_mode();
	
	return ret;
}


/**********************************************************************************/
/*Description: set product upgrade mode                                           */
/*Input      : 1: DvbSi£¬2: Unicast£¬3: remote£¬4: local                          */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_upgrade_mode(int mode)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if ((mode>0) && (mode<=4))
	{
		//ret = set_upgrade_mode(mode);
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get product upgrade server ip or domain, must as "192.168.22.222"  */
/*             or "kdf.dk.hk.cn", string normaly no more the 32 bytes             */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : ip string or domain string                                         */
/**********************************************************************************/
const char *ipanel_porting_get_upgrade_server(void)
{IPANEL_DEBUG("aaa\n");
	return "192.168.22.222";
}


/**********************************************************************************/
/*Description: set product upgrade server ip or domain                            */
/*Input      : ip string of domain string no more than 32 bytes                   */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_upgrade_server(const char *server)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
//	if (is_ip_string(ntp))
//	{
//		ret = set_ntp_server_ip(ntp);
//	}
//	else if (is_domain_string(ntp))
//	
//	{
//		ret = set_ntp_server_domain(ntp);
//	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get product upgrade port                                           */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : port number                                                        */
/**********************************************************************************/
int ipanel_porting_get_upgrade_port(void)
{IPANEL_DEBUG("aaa\n");
	return 1024;
}


/**********************************************************************************/
/*Description: set product upgrade port                                           */
/*Input      : port number                                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_upgrade_port(int port)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
//	if (is_effect_port(port))
//	{
//		ret = set_upgrade_port(port);
//	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get product upgrade user name, string no more the 16 bytes         */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : name string                                                        */
/**********************************************************************************/
const char *ipanel_porting_get_upgrade_user(void)
{IPANEL_DEBUG("aaa\n");
	return "user";
}


/**********************************************************************************/
/*Description: set product upgrade user name                                      */
/*Input      : string no more the 16 bytes                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 --- fail                                         */
/**********************************************************************************/
int ipanel_porting_set_upgrade_user(const char *usr)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if (strlen(usr) <= 16)
	{
    //   ret = set_upgrade_password(usr);	
	}
	
	return ret;
}


/**********************************************************************************/
/*Description: get product upgrade user password, string no more the 16 bytes     */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : password string                                                    */
/**********************************************************************************/
const char *ipanel_porting_get_upgrade_password(void)
{IPANEL_DEBUG("aaa\n");
	return "123456";
}


/**********************************************************************************/
/*Description: set product upgrade user password                                  */
/*Input      : string no more the 16 bytes                                        */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_set_upgrade_password(const char *pwd)
{IPANEL_DEBUG("aaa\n");
	int ret = -1;
	
	if (strlen(pwd) <= 16)
	{
    //   ret = set_upgrade_password;	
	}
	
	return ret;
}

/**********************************************************************************/
/*Description: config ip information                                              */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_ipconfig_all(void)
{IPANEL_DEBUG("aaa\n");
	/*config ip information here.*/
	return 0;
}

/**********************************************************************************/
/*Description: flush dns ip information                                           */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_ipconfig_flushdns(void)
{IPANEL_DEBUG("aaa\n");
	/*flush dns ip information here.*/
	return 0;
}

/**********************************************************************************/
/*Description: Register dns ip information                                        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_ipconfig_registerdns(void)
{IPANEL_DEBUG("aaa\n");
	/*Register dns ip information here.*/
	return 0;
}

/**********************************************************************************/
/*Description: Release ip information                                             */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_ipconfig_release(void)
{IPANEL_DEBUG("aaa\n");
	/*Release ip information here.*/
	return 0;
}

/**********************************************************************************/
/*Description: Renew config ip information                                        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_ipconfig_renew(void)
{IPANEL_DEBUG("aaa\n");
	/*Reconfig ip information here.*/
	return 0;
}

/**********************************************************************************/
/*Description: Save ip config information                                         */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_ipconfig_save(void)
{IPANEL_DEBUG("aaa\n");
	/*Save ip config information here.*/
	return 0;
}

/**********************************************************************************/
/*Description: Reboot STB after s seconds                                         */
/*Input      : time (uints is seconds)                                            */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_reboot(int s)
{IPANEL_DEBUG("aaa\n");
	/*sleep(s);
	rebootsystem();*/
	return 0;
}

/**********************************************************************************/
/*Description: Update STB after s seconds                                         */
/*Input      : time (uints is seconds)                                            */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_system_upgrade(int s)
{IPANEL_DEBUG("aaa\n");
	/*sleep(s);
	updatesystem();*/
	return 0;
}



/**********************************************************************************/
/*Description: Get remote control                                                 */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : remote control string                                              */
/**********************************************************************************/
const char *ipanel_porting_get_remote_control(void)
{IPANEL_DEBUG("aaa\n");
	return "Sony-2";
}


/**********************************************************************************/
/*Description: Get Remote Keyboard                                                */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : remote Keyboard string                                             */
/**********************************************************************************/
const char *ipanel_porting_get_remote_keyboard(void)
{IPANEL_DEBUG("aaa\n");
	return "US-1";
}



/**********************************************************************************/
/*Description: Get administrator password                                         */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : administrator password                                             */
/**********************************************************************************/
const char *ipanel_porting_get_admin_password(void)
{IPANEL_DEBUG("aaa\n");
	return "123456";
}


/**********************************************************************************/
/*Description: Set Administrator password                                         */
/*Input      : Administrator password                                             */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_set_admin_password(const char *pwd)
{IPANEL_DEBUG("aaa\n");
	/*set administrator password here*/
	return 0;
}



/**********************************************************************************/
/*Description: Get administrator user name                                        */
/*Input      : no                                                                 */
/*Output     : no                                                                 */
/*Return     : administrator user name                                            */
/**********************************************************************************/
const char *ipanel_porting_get_admin_user(void)
{IPANEL_DEBUG("aaa\n");
	return "ipanel";
}



/**********************************************************************************/
/*Description: Set Administrator Usr Name                                         */
/*Input      : Administrator Usr Name                                             */
/*Output     : no                                                                 */
/*Return     : 0 --- success, -1 fail                                             */
/**********************************************************************************/
int ipanel_porting_set_admin_user(const char *usr)
{IPANEL_DEBUG("aaa\n");
	/*set administrator User Name here*/
	return 0;
}


int ipanel_porting_hotkey_add(int number, const char *url)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

int ipanel_porting_hotkey_clean(void)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

int ipanel_porting_hotkey_del(int number)
{IPANEL_DEBUG("aaa\n");
	return 0;
}
int ipanel_porting_hotkey_edit(int number)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

int ipanel_porting_hotkey_go(int number)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

int ipanel_porting_hotkey_list(void)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

int ipanel_porting_hotkey_recall(void)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

int ipanel_porting_hotkey_save(void)
{IPANEL_DEBUG("aaa\n");
	return 0;
}

