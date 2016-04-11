

/**********************************文件头部注释*******************************************/

//////////////////////////////////////////////////////////////////////////////////////////
//
// 文件名：     envod_main.c
//
// 创建者：     曾振宇
//
// 创建时间：   2010.04.01
//
// 文件描述：   
//
// 修改记录： 日       期      作      者       修定
//                    ---------         ---------         -----
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "../ipanel2.0/ipanel_porting_event.h"
#include "../ipanel2.0/ipanel_socket.h"
#include "../ipanel2.0/ipanel_base.h"
#include "../ipanel2.0/trd_ipanel_init.h"
#include "../ipanel2.0/ipanel_typedef.h"
#include "../ipanel2.0/ipanel_os.h"
#include "evtc.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"
#include "../hisiinclude/hi_stb_api.h"
#include "../hiadp/include/msg_event.h"
#include "../hiadp/include/software.h"
#include "../hiadp/include/hardware.h"
#include "../hisiinclude/higo/hi_go.h"
#include "../tpmiddle/include/app_sys.h"

#include "../tpversion/ui_demo/osd/include/resource.h"
#include "../tpmiddle/include/app_common.h"
#include "../ow_vod/ow_debug.h" 
#include "enreach_keymap.h"


//#include "../tpmiddle/include/app_com.h"
//#include "../hiadp/include/hi_osdgal.h"

/*zhengsl-------------------------------------------------*/
sem_t      my_sem;
static int  sem_key;
/*zhengsl-------------------------------------------------*/

char *const en_fuzhouTSGAddr = "fuzhou-tsg.wasu.cn";
char *const en_fuzhouTSGPort = "2777";
PROG_TYPE vodortvod ;	
extern unsigned char IP_Address[9][4];
char *np_homepage1 = "10.10.0.11"; 
//char *np_homepage1 = "172.16.4.39"; 
/*EVTC_TSLIST Timeshiftlist[MAX_TSVOD_NUM]= 
   {
 { 610000,
  1701},
 { 602000,
  1601},
  {634000,
  2004},
 { 618000,
  1801},
 { 610000,
  1702},
 { 602000,
  1602},
  {610000,
  1705},
  {618000,
  1808},
  {610000,
  1703},
  {602000,
  1604},
  {634000,
  2002},
  {626000,
  1901},
  {610000,
  1704},
  {602000,
  1606},
  {634000,
  2001},
  {626000,
  1906}};*/
//  EVTC_TSLIST Timeshiftlist[MAX_TSVOD_NUM];
EVTC_TSLIST *Timeshiftlist;
int tvod_num = 0;/*时移节目个数*/
int b_get_tslist = false;
extern int cur_ts_index ;
static int bEN_VOD_Exit = true;

void envod_set_exit( int status)
{
	bEN_VOD_Exit = status;
}

int  envod_is_exit( void)
{
	return bEN_VOD_Exit;
}

void envod_printf (const char *fmt, ...)
{
	char report_buffer[4 * 1024];
	va_list list;

#if 1
	va_start(list, fmt);
	vsprintf(report_buffer, fmt, list);
	va_end(list);
	ENREACH_DEBUG(("evtc:     "));
	ENREACH_DEBUG((report_buffer));
	printf("evtc:     ");
	printf(report_buffer);
#endif
}   

void  test_list()
{
	#if 1

	char *host = "10.10.0.11"; 
	int i;
	char tvod_index;

	envod_printf(" test_lisd\n");
	if (b_get_tslist) return;
	///*zhengsenlin*/evtc_default_bmp();
	evtc_init_sem_prtct();
	tvod_num = evtc_getTSList(host, &Timeshiftlist);
	for(tvod_index=0;tvod_index<tvod_num;tvod_index++)
	{
		printf("_______________ flag_serid = %d:::::flag_freq = %d  __________________\n",Timeshiftlist[tvod_index].serviceid,Timeshiftlist[tvod_index].freq);
	}
	if(tvod_num > 0)
	{
		b_get_tslist = true;
		envod_printf(" test_list evtc_getTSList  !!tvod_num = %d\n",tvod_num);
	}
		envod_printf(" test_lisd end\n");
		#endif
}


int TPEN_VOD_MainMenu(char *home_page,PROG_TYPE prog_type)
{
	int ret = 0,iBroadcastKey = -1;
	HI_U32 u32PressStatus, u32KeyId;
	int ts_prog_index = 0;/*当前在播第几套时移节目*/
	EVTC_INIT  INITPARA;
	EVTC_VOD  VODPLAYPARA;
	EVTC_TS  RELAYPARA;
	int status = 0;
	int speed_num = 1;/*播放倍数,注意正常播放设几*/
	char host[100] = {0}; 
	char ipStr[20] = { 0 };
	char portStr[10] = { 0 };
	char *curtime = NULL;

	//test_av();
	///*zhengsenlin*/evtc_default_bmp();
	/*if(evtc_drawBPZhun() == envod_fail)
	{
		return;
	}zhengsenlin 0423*/
	
	BroadCast_AppInit();
	///*zhengsenlin*/evtc_draw_connect();
	envod_set_exit(false);
	envod_printf(" TPEN_VOD_MainMenu !! \n");
	vodortvod = prog_type;
	/*if(home_page == NULL)
	{
		sprintf(host,"%d.%d.%d.%d:80",IP_Address[8][0],IP_Address[8][1],IP_Address[8][2],IP_Address[8][3]);
	}
	else
	{
		evtc_GetIPStrAndPortStr(home_page, ipStr,portStr);
		
		sprintf(host,"%s:%s",ipStr,portStr);
	}*/

	sprintf(host,"%s",np_homepage1);


	if((ret = evtc_init(host, &INITPARA)) == envod_fail)
	{
		envod_set_exit(true);
		envod_printf(" evtc_init err!! \n");
	}
	/*else if((ret = evtc_register(host)) == envod_fail)
	{
		envod_set_exit(true);
		envod_printf(" evtc_register err!! \n");
	}
    else if((INITPARA.mainfreq == 0) || (ret = evtc_set_tuner(INITPARA.mainfreq)) == envod_fail)
    {
		envod_printf(" evtc_set_tuner unlock INITPARA.mainfreq = %d err!! \n",INITPARA.mainfreq);
		if((INITPARA.sndfreq == 0) || (ret = evtc_set_tuner(INITPARA.sndfreq)) == envod_fail)
		{
			envod_set_exit(true);
			envod_printf(" evtc_set_tuner unlock INITPARA.sndfreq = %d err!! \n",INITPARA.sndfreq);
		}
		else if((ret = evtc_getIPQAMGroup(host, INITPARA.sndtsid)) == envod_fail)
		{
			envod_set_exit(true);
			envod_printf(" evtc_getIPQAMGroup lock sndtsid err!! \n");
		}
    }*/
	/*else if((ret = evtc_getIPQAMGroup(host, INITPARA.maintsid)) == envod_fail)
	{
		envod_set_exit(true);
		envod_printf(" evtc_getIPQAMGroup lock maintsid err!! \n");
	}*/

#if 0
	if(envod_is_exit())
	{
		envod_printf(" TPEN_VOD_MainMenu init err and exit vod!! \n");
	}
	else if((prog_type ==TS_VOD) && (tvod_num = evtc_getTSList(host, Timeshiftlist)) == envod_fail)
	{
		envod_printf(" evtc_getTSList err!! \n");
	}
	else
	{
		if(tvod_num > MAX_TSVOD_NUM)
		{
			envod_printf(" evtc_getTSList get %d nums > MAX_TSVOD_NUM !! \n",tvod_num);
		}
	}
#endif	
	regist_callback_task_destroy(evtc_task_destroy);
	
#if 1
	if(prog_type == EN_VOD)   //change from VOD to EN_VOD by zhengsenlin
	{
		if((ret = evtc_vodplay(host, /*int moveid*/310, &VODPLAYPARA)) == envod_fail)
		{
			envod_printf(" evtc_vodplay err!! \n");
		}
/*		else if((evtc_clear_connect())&&(ret = evtc_open_av(VODPLAYPARA.freq,VODPLAYPARA.symbolrate,VODPLAYPARA.qam,VODPLAYPARA.vpid, VODPLAYPARA.apid, VODPLAYPARA.pcrpid)) == envod_fail)
		{
			envod_printf(" evtc_open_av err!! \n");
		}zhengsenlin*/
	}
	else
#endif
	{
		if(tvod_num > 0)
		{
			if((ret = evtc_relay(host,Timeshiftlist[cur_ts_index].freq/* 634000*/, Timeshiftlist[cur_ts_index].serviceid/*2004*/, &RELAYPARA)) == envod_fail)
			{
				envod_printf(" evtc_relay err!! \n");
				//envod_set_exit(true);
			}
			else if(evtc_getvidaid(&RELAYPARA,Timeshiftlist[cur_ts_index].serviceid) == 2)
			{
				
			}
			else if((ret = evtc_open_av(RELAYPARA.freq/*802000*/, RELAYPARA.symbolrate,RELAYPARA.qam,RELAYPARA.vpid, RELAYPARA.apid, RELAYPARA.pcrpid)) == envod_fail)
			{
				envod_printf(" evtc_open_av err!! \n");
				envod_set_exit(true);
			}
			printf(" evtc_relay begin = %s ,end = %s,cur = %s\n",RELAYPARA.begintime,RELAYPARA.endtime,RELAYPARA.curtime);


		}
		else
		{
			envod_printf(" there is no ts programe !! \n");
			envod_set_exit(true);
		}
	}

	while(!envod_is_exit())
	{
		iBroadcastKey = HI_UNF_IR_GetValue(&u32PressStatus, &u32KeyId);
		if(iBroadcastKey == -1)
		{
			continue;
		}

		switch (u32KeyId)		//快进的时候不能暂停，暂停的时候不能快进
		{
		case GUI_VK_EXIT:
/*			if(evtc_draw_exit() == envod_success)
			{
				evtc_end(host, status);
				envod_set_exit(true);
			}zhengsenlin*/
			break;

		case GUI_VK_SELECT:			//暂停或播放

			if(speed_num != 1)
			{
				ret = evtc_replay(host, status);
				if(ret == envod_fail)
				{
					envod_printf(" evtc_replay err!! \n");
				}
				else
				{
/*					evtc_clear_pp();zhengsenlin*/
					speed_num = 1;
				}
			}
			else
			{
				 #if 0
				curtime = evtc_pause(host, status);
				if(curtime == envod_fail)
				{
					envod_printf(" evtc_pause err!! \n");
				}
				else
				{
					evtc_draw_pp(curtime);
					speed_num = 0;
				}
				#else
				ret = evtc_pause(host, status);
				if(ret == envod_fail)
				{
					envod_printf(" evtc_pause err!! \n");
				}
				else
				{
/*					evtc_draw_pp(curtime);zhengsenlin*/
					speed_num = 0;
				}
				#endif
			}
			break;
		
		case GUI_VK_RIGHT:
			#if 0
			if(speed_num >= 0 ) /*保证在快进状态下*/
			{
				//speed_num = evtc_ff(host, status);
				evtc_ff(host, status);
				if(/*speed_num == envod_fail*/0)
				{
					envod_printf(" evtc_ff err!! \n");
				}
				else
				{
					if(speed_num == 1 || speed_num == 0)
					{
						if(speed_num == 0)
						{
							evtc_clear_pp();
						}
						speed_num = 2;
					}
					else
					{
						speed_num++;
					}
				        speed_num %= 4;
					evtc_draw_speed(speed_num ,0);
				}
			}
			#else
				speed_num = evtc_ff(host, status) * 2;
				if(speed_num == envod_fail)
				{
					envod_printf(" evtc_ff err!! \n");
				}
				else
				{
					/*evtc_draw_speed(speed_num ,0);zhengsenlin*/
				}
			#endif

			
			break;
		case GUI_VK_LEFT:
			#if 0
			if(speed_num <= 1) /*保证在快退状态下*/
			{
				//speed_num = evtc_fb(host, status);
				 evtc_fb(host, status);
				if(/*speed_num == envod_fail*/0)
				{
					envod_printf(" evtc_fb err!! \n");
				}
				else
				{
					if(speed_num == 1 || speed_num == 0)
					{
						if(speed_num == 0)
						{
							evtc_clear_pp();
						}
						speed_num = -2;
					}
					else
					{
						speed_num--;
					}
				        speed_num %= 4;
					evtc_draw_speed(speed_num ,0);
				}
			}
			#else
				speed_num = evtc_fb(host, status) * 2;
				if(speed_num == envod_fail)
				{
					envod_printf(" evtc_fb err!! \n");
				}
				else
				{
					speed_num = -speed_num;
/*					evtc_draw_speed(speed_num ,0);zhengsenlin*/
				}
			#endif
				
			break;
		case GUI_VK_UP:
			if(prog_type == TS_VOD)
			{
				ret = evtc_pgup(host,&RELAYPARA);
				printf(" evtc_pgup freq = %d begin = %s ,end = %s,cur = %s\n",RELAYPARA.freq,RELAYPARA.begintime,RELAYPARA.endtime,RELAYPARA.curtime);
				if(ret == envod_fail)
				{
					envod_printf(" evtc_pgup err!! \n");
				}
				else
				{
					ret = evtc_open_av(RELAYPARA.freq, RELAYPARA.symbolrate,RELAYPARA.qam,RELAYPARA.vpid, RELAYPARA.apid, RELAYPARA.pcrpid);
					if(ret == envod_fail)
					{
						envod_printf(" evtc_open_av err!! \n");
					}
				}
			}

			break;
		case GUI_VK_DOWN:
			if(prog_type == TS_VOD)
			{
				ret = evtc_pgdn(host,&RELAYPARA);
				printf(" evtc_pgdn freq = %d begin = %s ,end = %s,cur = %s\n",RELAYPARA.freq,RELAYPARA.begintime,RELAYPARA.endtime,RELAYPARA.curtime);
				if(ret == envod_fail)
				{
					envod_printf(" evtc_pgdn err!! \n");
				}
				else
				{
					ret = evtc_open_av(RELAYPARA.freq,RELAYPARA.symbolrate,RELAYPARA.qam,RELAYPARA.vpid, RELAYPARA.apid, RELAYPARA.pcrpid);
					if(ret == envod_fail)
					{
						envod_printf(" evtc_open_av err!! \n");
					}
				}
			}

			break;

		case GUI_VK_P_UP:
		case GUI_VK_P_DOWN:
			/*evtc_draw_bar(host, &RELAYPARA);zhengsenlin*/
			break;
			
		case GUI_VK_F7:		//f7
/*			TPDRAW_ClearScreenIron(3);
			TPSYSTEM_AudioVolumeKeyFunction(DECREASE_VOLUME);
			MessageDisPlayEixtMenu();zhengsenlin*/
			break;
		case GUI_VK_F8:			//f8
/*			TPDRAW_ClearScreenIron(3);
			TPSYSTEM_AudioVolumeKeyFunction(INCREASE_VOLUME);
			MessageDisPlayEixtMenu();zhengsenlin*/
			break;
		default:
			break;

		}

	}
	ret = evtc_close();
	if(ret == envod_fail)
	{
		envod_printf(" evtc_close err!! \n");
	}
	/*BroadCast_ExitInit(); zhengsenlin*/
	envod_set_exit(true);
	envod_printf(" TPEN_VOD_MainMenu  exit!! \n");
}


#if 0
int TPEN_VOD_MainMenu(char *home_page, PROG_TYPE prog_type)
{
	int ret = 0,iBroadcastKey = -1;
	int tvod_num = 0;/*时移节目个数*/
	int ts_prog_index = 0;/*当前在播第几套时移节目*/
	EVTC_INIT  INITPARA;
	EVTC_TSLIST Timeshiftlist[MAX_TSVOD_NUM];
	EVTC_VOD  VODPLAYPARA;
	EVTC_TS  RELAYPARA;
	int status = 0;
	int speed_num = 1;/*播放倍数,注意正常播放设几*/
	char host[100] = {0}; 
	char ipStr[20] = { 0 };
	char portStr[10] = { 0 };

	ENREACH_DEBUG("________________Starting Create HI_OS_SemaphoreCreate________________");

	ret = HI_OS_SemaphoreCreate(1, &my_sem);
	if(ret != 0)
	{	
		printf(" _________________create semaphore error_________________TPEN_VOD_MainMenu________");
	}
	
	ENREACH_DEBUG("________________HI_OS_SemaphoreCreate Successfuly________________");

	while(true)
	{

	 	HI_U32 u32PressStatus, u32KeyId;
		iBroadcastKey = HI_UNF_IR_GetValue(&u32PressStatus, &u32KeyId);
		if(iBroadcastKey == -1)
		{
			continue;
		}

/*
		sem_key = u32KeyId;
		HI_OS_SemaphoreSignal(my_sem);
		ENREACH_DEBUG("________________Already send HI_OS_SemaphoreSignal ________________");
		HI_OS_SemaphoreWait(my_sem,20000);
		ENREACH_DEBUG("________________Already waited HI_OS_SemaphoreWait________________");
		u32KeyId = sem_key;
		ENREACH_DEBUG("________________Starting print switch key________________");
*/		
		switch (u32KeyId)		//快进的时候不能暂停，暂停的时候不能快进
		{
	//	case 0x8e:
		case GUI_VK_F10:
		ENREACH_DEBUG("________________ Touch the GUI_VK_F10 ________________");
		u32KeyId = NULL;
			break;
			
		default:
			break;   
		}
	}
	if((ret = evtc_init(host, &INITPARA)) == envod_fail)
	{
		envod_set_exit(true);
		evtc_dbg(" evtc_init err!! \n");
	}
}

#endif


