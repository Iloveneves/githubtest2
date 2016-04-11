#include <time.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#include "hi_stb_api.h"
#include "fyf_com.h"
#include "hi_unf_tuner.h"
//#include <unistd.h>
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <string.h>



#define POWERUP_IR		0x6d927f80
#define POWERUP_KEY		3

HI_U32 g_standy_flag = 0xff;

#define REG_SYS_BASE            0x101e0000

#define REG_USB_RESET           0x80
#define SET_ETH_RESET           (1<<15)
#define SET_USB_PHY_RESET       (1<<16)
#define SET_USB_CTRL_RESET      (1<<17)

#define REG_USB_CLK_EN          0x24
#define REG_USB_CLK_DIS         0x28
#define SET_USB_CLK             (1<<20)
#define SET_ETH_CLK             (1<<12)

void Hi_Pmoc_USB_ETH_CTL(int low_pwr)
{
	int reg;
	void *sysctl_map;

	int fd=open("/dev/mem",O_RDWR|O_SYNC,00777);
	sysctl_map = mmap(NULL,0x4000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,REG_SYS_BASE);
	if (sysctl_map == MAP_FAILED)
	{
		printf(" mmap failed.\n");
		return ;
	}

	/*Set GPIO_05 Reuse*/
	reg = *(volatile unsigned int*)(0x1c + sysctl_map);
	reg |= (0x70000);
	*(volatile unsigned int*)(0x1c + sysctl_map) = reg;
	msync(sysctl_map, 0x4000, MS_ASYNC);

	if (low_pwr)
	{
		/************* Disable USB start ****************/
		/*Rmmod USB Driver*/
		printf("77777777777\n");
		system("rmmod  ohci_hcd");
		system("rmmod  ehci_hcd");


		/*reset USB PHY & controller*/
		reg = *(volatile unsigned int*)(REG_USB_RESET + sysctl_map);
		reg |= (SET_USB_PHY_RESET | SET_USB_CTRL_RESET);
		*(volatile unsigned int*)(REG_USB_RESET + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);

		/*Disable USB CLK*/
		reg = *(volatile unsigned int*)(REG_USB_CLK_DIS + sysctl_map);
		reg |= (SET_USB_CLK);
		*(volatile unsigned int*)(REG_USB_CLK_DIS + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);
		/************ Disable USB end *******************/

		/************ Disable ETH Power start ****************/
		/*Disable ETH CLK*/
		reg = *(volatile unsigned int*)(0x40 + sysctl_map);
		reg |= (1<<26);
		*(volatile unsigned int*)(0x40 + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);

		/************ Disable ETH Power end   ****************/

	}
	else
	{
		/************** Enable USB start ****************/
		/*Enable USB CLK*/
		reg = *(volatile unsigned int*)(REG_USB_CLK_EN + sysctl_map);
		reg |= (SET_USB_CLK);
		*(volatile unsigned int*)(REG_USB_CLK_EN + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);
		//udelay(100);
		usleep(100);

		/*Enable USB PHY*/
		reg = *(volatile unsigned int*)(REG_USB_RESET + sysctl_map);
		reg &= ~(SET_USB_PHY_RESET);
		*(volatile unsigned int*)(REG_USB_RESET + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);
		//udelay(100);
		usleep(100);

		/*Enable USB controller*/
		reg = *(volatile unsigned int*)(REG_USB_RESET + sysctl_map);
		reg &= ~(SET_USB_CTRL_RESET);
		*(volatile unsigned int*)(REG_USB_RESET + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);
		//udelay(100);
		usleep(100);


		/*Insmod USB driver*/
		system("insmod /kmod/ehci-hcd.ko");
		system("insmod /kmod/ohci-hcd.ko");
		/************** Enable USB end *******************/

		/************ Enable ETH Power start ****************/
		/*Disable ETH CLK*/
		reg = *(volatile unsigned int*)(0x40 + sysctl_map);
		reg &= ~(1<<26);
		*(volatile unsigned int*)(0x40 + sysctl_map) = reg;
		msync(sysctl_map, 0x4000, MS_ASYNC);

		/************ Enable ETH Power end   ****************/
	}

	if ( munmap( sysctl_map, 0x4000) < 0 )
	{
		printf("munmap failed");
		exit(1);
	}

	close(fd);

	return ;
}

HI_S32 HISDK_Init()
{
	HI_S32	s32Ret;

	/* 初始化输出设备 */
	s32Ret  = HI_UNF_VO_Init();
	if (HI_SUCCESS != s32Ret)
	{
		printf("ErrorCode:%08x LINE %d\n", s32Ret, __LINE__);
		return s32Ret;
	}
	s32Ret = HI_UNF_GPIO_Write_Bit(14,1);
	if (HI_SUCCESS != s32Ret)
	{
		printf("(%d):HI_UNF_GPIO_Open Error,ret=0x%x\n", __LINE__, s32Ret);
	}
	s32Ret |= HI_UNF_SND_Init();
	if (HI_SUCCESS != s32Ret)
	{
		printf("ErrorCode:%08x LINE %d\n", s32Ret, __LINE__);
		return s32Ret;
	}
	s32Ret |= HI_UNF_DISP_Init();
	if (HI_SUCCESS != s32Ret)
	{
		printf("ErrorCode:%08x LINE %d\n", s32Ret, __LINE__);
		return s32Ret;
	}

	s32Ret |= HI_UNF_AVPLAY_Init();
	if (HI_SUCCESS != s32Ret)
	{
		printf("ErrorCode:%08x LINE %d\n", s32Ret, __LINE__);
		return s32Ret;
	}

	HI_UNF_TUNER_ATTR_S stTunerAttr;

	if (HI_SUCCESS != HI_UNF_TUNER_Init())
	{
		printf("\n +++ HI_UNF_TUNER_Init failed +++\n");
	}


	if (HI_SUCCESS != HI_UNF_TUNER_Open(0))
	{
		printf("\n +++ HI_UNF_TUNER_Open failed +++\n");
	}

	HI_UNF_TUNER_GetDeftAttr(0 , &stTunerAttr);
#ifdef _NEWLAND_DEMO_
	stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TMX70600;
	if (HI_TUNER_CheckValid_I2cAddr(0,0xc6) != HI_SUCCESS)
	{
		HI_TUNER_CheckValid_I2cAddr(0,0xc0);
		FYF_API_tuner_set_dev_addr(0);
		printf("detect tuner devaddr is 0xc0 \n");
	}
	else
	{
		FYF_API_tuner_set_dev_addr(1);
		printf("detect tuner devaddr is 0xc6 \n");
	}

#endif
	HI_UNF_TUNER_SetAttr(0 , &stTunerAttr);

	s32Ret = HI_UNF_PMOC_Open();
	if (HI_SUCCESS != s32Ret)
	{
		printf("(%d):HI_UNF_PMOC_Open Error,ret=0x%x\n", __LINE__, s32Ret);
	}


	/*
		s32Ret = OSDGAL_Init();
		if (s32Ret != HI_SUCCESS)
		{
			printf("OSDGAL_Init fail\n");

		}
	*/

	return s32Ret;
}

int main(int argc, char *argv[])
{
	HI_S32	ret,s32Ret;
	HI_UNF_PMOC_STANDBY_MODE_S stStandbyMode;
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	BU08 time[2];

#ifdef _HAISI_DEMO_
	printf("************************************HAISI DEMO***************************\n");
#else
	printf("************************************NEWLAND DEMO***************************\n");

#endif

#ifdef FLASH_8M
	printf("************************************8M FLASH***************************\n");
#else
	printf("************************************16M FLASH***************************\n");
#endif

#ifdef BOX_ENCRYPT_E2PROM
	printf("************************************ENCRYPT EEPROM***************************\n");
#else
	printf("************************************NORMAL EEPROM***************************\n");
#endif

	if ( argc == 2 )
	{
		g_standy_flag = atoi(argv[1]);
	}


	s32Ret = HI_UNF_GPIO_Open();
	if (HI_SUCCESS != s32Ret)
	{
		printf("(%d):HI_UNF_GPIO_Open Error,ret=0x%x\n", __LINE__, s32Ret);
	}

	s32Ret = HI_UNF_GPIO_DirSet_Bit(14,HI_FALSE);

	if (HI_SUCCESS != s32Ret)
	{
		printf("(%d):HI_UNF_GPIO_DirSet_Bit Error,ret=0x%x\n", __LINE__, s32Ret);
	}
	HISDK_Init();
	TP_Led_Init();
	TP_KEY_Init();
	TP_IR_Init();
	FYF_API_dev_register();

	//进入低功耗状态
	if ( 0xfe == g_standy_flag)
	{
		HI_UNF_PMOC_WKUP_S sAttr;
		HI_UNF_PMOC_MODE_E enUnfSystemMode;
		HI_U32 u32WakeupStatus;
		APP_Com_API_ShowLED("    ");
		HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_1);
		fyf_e2p_ioctl_param.buffer = (PU08)time;
		fyf_e2p_ioctl_param.bufferSize = sizeof(time);

		//FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, 513, (BU32)&fyf_e2p_ioctl_param);
		FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, 32, (BU32)&fyf_e2p_ioctl_param);
		printf("power down ...........\n");
		sAttr.u32IrPowerKey = POWERUP_IR;
		sAttr.u32KeypadPowerKey = POWERUP_KEY;
		//设置唤醒方式
		ret = HI_UNF_PMOC_SetWakeUpAttr(&sAttr);
		if ( HI_SUCCESS != ret)
		{
			printf("HI_UNF_PMOC_SetWakeUpAttr %08x\n",ret);
		}
		//进入睡眠模式
		printf("sleeping......\n");
		printf("time = %d %d\n", time[0], time[1]);

#if 0
		stStandbyMode.u32Mode = 2;
		stStandbyMode.u32DispCode = 9009;
		stStandbyMode.stTimeInfo.u32Hour = time[0]%24;//tv.tv_sec /3600;
		stStandbyMode.stTimeInfo.u32Minute = time[1]%60;//(tv.tv_sec %3600)/60;
		stStandbyMode.stTimeInfo.u32Second = 30;//(tv.tv_sec %3600)%60;

		s32Ret = HI_UNF_PMOC_SetStandbyDispMode(&stStandbyMode);
		if (HI_SUCCESS != s32Ret)
		{
			printf("(%d): HI_UNF_PMOC_SetStandbyDispMode Error,ret=0x%x\n", __LINE__, s32Ret);
			return HI_FAILURE;
		}
#endif
#if 1
		system("echo 2000 > /proc/sys/vm/min_free_kbytes");

		system("ifconfig eth0 down");
		system("ifconfig eth1 down");

		system("echo 0x0 0x1 0x0 0x190 > /proc/x5net/client_iface/phy_write");
		system("echo 0x1 0x3 0x0 0x190 > /proc/x5net/client_iface/phy_write");
		system("echo 0x0 0x1 0x0 0x800 > /proc/x5net/client_iface/phy_write");
		system("echo 0x1 0x3 0x0 0x800 > /proc/x5net/client_iface/phy_write");

		system("rmmod hi_eth.ko");
		Hi_Pmoc_USB_ETH_CTL(1);
#endif

		ret = HI_UNF_PMOC_SwitchSystemMode(HI_UNF_PMOC_MODE_SLEEP, &u32WakeupStatus);
		if (HI_SUCCESS != ret)
		{
			printf("HI_UNF_PMOC_SwitchSystemMode,ret=0x%x\n", ret);
			return HI_FAILURE;
		}
		HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_NONE);
#if 1
		Hi_Pmoc_USB_ETH_CTL(0);


		system("insmod /kmod/hi_eth.ko");
		system("ifconfig eth0 up");
		system("ifconfig eth1 up");
		system("echo 0x0 0x1 0x0 0x1000 >/proc/x5net/client_iface/phy_write");
		system("echo 0x1 0x1 0x0 0x1000 >/proc/x5net/client_iface/phy_write");
#endif

		printf("power up ........... by %d\n",u32WakeupStatus);
	}
#if 1                //added by mabel for opening sound
	s32Ret |= HI_UNF_GPIO_Write_Bit(14,0);
	if (HI_SUCCESS != s32Ret)
	{
		printf("(%d):HI_UNF_GPIO_Write_Bit Error,ret=0x%x\n", __LINE__, s32Ret);
	}
	usleep(50000);
#endif
	HI_ETH_Open(0);
	APP_Com_API_ShowLED("0000");
	FYF_API_OSD_Init();
	FYF_API_os_register();
	FYF_API_sys_register();
	FYF_API_demux_init();
#ifdef ENABLE_PVR
	FYF_API_PVR_Init();
#endif
	FYF_API_av_register();
#ifndef NO_CA
	FYF_API_SMC_Init();
	FYF_API_CA_Init();
	FYF_API_DESCR_init();
#endif
	FYF_API_mfs_init();
	FYF_API_sql_init();
	FYF_API_tuner_init();
	//Ipanel_DLRegionId();

	FYF_API_createGUIThread();

	while (1)
	{
		//	printf("main task is sleeping\n");
		FYF_API_thread_sleep(2148573);
	}
	return 0;
}
//chenmeibao