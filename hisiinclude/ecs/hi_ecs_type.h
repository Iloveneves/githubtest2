/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_ecs_type.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/6/17
Last Modified :
Description   : .....
Function List :
History       :
 ******************************************************************************/
#ifndef __HI_ECS_TYPE_H__
#define __HI_ECS_TYPE_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*---- WDG ----*/
#define WDG_FIRST_RESET_MODE     0
#define WDG_INTERRUP_ENABLE_MODE 1
/*---- e2prom ----*/
typedef struct hiE2PROM_Data_S
{       
    HI_U32 u32Addr;        
    HI_U8* pu8Buf;         
    HI_U32 u32Length;     
} E2PROM_Data_S, *PTR_E2PROM_Data_S;

typedef struct hiE2PROM_PARA_S
{
    HI_U32 u32EpromSize;
    HI_U32 u32PageSize;
    HI_U32 u32AddrLen;
} E2PROM_PARA_S;

typedef enum hiE2PROM_ChipType_E
{
    AT24C01,
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32,
    AT24C64,
    AT24C128,
    AT24C256,
    M24LC01,
    M24LC02,
    M24LC04,
    M24LC08,
    M24LC16,
    M24LC32,
    M24LC64,
    M24LC128,
    M24LC256
} E2PROM_ChipType_E;

/*---- ir ----*/
typedef struct
{
    HI_U32 u32Irkeycode;
    HI_U32 u32Irkeystate;
}IRDA_Info_S;

typedef enum  hiIR_CODETYPE_E
{
    HI_IR_NecWithSimple= 0,
    HI_IR_Tc9012,
    HI_IR_NecWithFull,
    HI_IR_Sony12Bit,
    HI_IR_CODETYPE_MAX
} IR_CodeType_E;

typedef struct hiIR_PARA_S
{       
    HI_U32 u32ParaMin;        
    HI_U32 u32ParaMax;     
} IR_Para_S, *PTR_IR_Para_S;

typedef struct hiIR_Dev_Param_S
{
    HI_U32 u32leads_min;
    HI_U32 u32leads_max;

    HI_U32 u32leade_min;
    HI_U32 u32leade_max;

    HI_U32 u32cnt0_b_min;
    HI_U32 u32cnt0_b_max;

    HI_U32 u32cnt1_b_min;
    HI_U32 u32cnt1_b_max;

    HI_U32 u32sleade_min;
    HI_U32 u32sleade_max;

    HI_U32 u32code_len;
    HI_U32 u32codetype;
    HI_U32 u32frequence;
}IR_Dev_Param_S;

/*---- gpio ----*/
typedef struct hiGPIO_Data_S
{
    HI_U32 u32gpioindex;
    HI_U32 u32gpiobit;
    HI_U32 u32gpiovalue;
} GPIO_Data_S, *PTR_GPIO_Data_S;

/*---- i2c ----*/
typedef enum enum_I2cChannel
{
	I2C_CHANNEL_0 = 0, 
	I2C_CHANNEL_1, 
	I2C_CHANNEL_2,
	I2C_CHANNEL_MAX
}I2cChannel_E;

typedef struct hiI2C_Data_S
{       
    HI_U8* pu8SendBuf;
    HI_U32 u32SendLength;
    HI_U8* pu8ReceiveBuf;
    HI_U32 u32ReceiveLength;
} I2C_Data_S, * PTR_I2C_Data_S;  

typedef struct hiI2C_IO_Data_S
{       
    HI_U32 u32I2cNum;        
    HI_U8* pu8Buf;         
    HI_U32 u32Length;
    HI_U8 u8DevAddress;
} I2C_IO_Data_S, *PTR_I2C_IO_Data_S;

typedef struct hiI2C_RATE_S
{       
    HI_U32 u32I2cNum;        
    HI_U32 u32I2cRate;
} I2C_Rate_S;

/*---- keyled ----*/
typedef struct
{
    HI_U32 keyled_code;
    HI_U32 keyled_state;
}keyled_key_info_s;

typedef enum hiHI_KEYLED_LEVEL_E
{
    HI_KEYLED_LEVEL1 = 0x01,
    HI_KEYLED_LEVEL2,
    HI_KEYLED_LEVEL3,
    HI_KEYLED_LEVEL4,
    HI_KEYLED_LEVEL5
}HI_KEYLED_LEVEL_E;

typedef enum hiHI_KEYLED_LIGHT_E
{
    HI_KEYLED_LIGHT1 = 0x01,
    HI_KEYLED_LIGHT2,
    HI_KEYLED_LIGHT3,
    HI_KEYLED_LIGHT4,
    HI_KEYLED_LIGHTALL,
    HI_KEYLED_LIGHTNONE
}HI_KEYLED_LIGHT_E;

typedef struct hikeyled_time_s
{
    HI_U32  hour;
    HI_U32  minute;
}keyled_time_s,*ptr_keyled_time_s;

typedef enum hiHI_KEYLED_LEDTYPE_E
{
    HI_KEYLED_CATHODE = 0x0,
    HI_KEYLED_ANODE,
    HI_KEYLED_LEDTYPE_MAX
} HI_KEYLED_LEDTYPE_E;

typedef enum hiHI_KEYLED_CSTYPE_E
{
    HI_KEYLED_CS_LOW = 0x0,
    HI_KEYLED_CS_HIGH,
    HI_KEYLED_CSTYPE_MAX
} HI_KEYLED_CSTYPE_E;

/*---- pmoc ----*/
typedef enum hiPMoC_MODE_E
{
    HI_PMoC_NORMAL_MODE = 0,
    HI_PMoC_SLOW_MODE,
    HI_PMoC_DOZE_MODE,
    HI_PMoC_SLEEP_MODE,
    HI_PMoC_MODE_E_MAX
} PMoC_MODE_E;

typedef enum hiPMoC_WAKEUP_E
{
    HI_PmoC_IR_WAKEUP = 1,
    HI_PmoC_KEY_WAKEUP,
    HI_PmoC_RTC_WAKEUP
} PMoC_WAKEUP_E;

/* led display time when arm is stand by, add by David Huang 2008-05-02 start */
typedef struct hiPMoC_TIME_S
{
    HI_U32  	hour;
    HI_U32  	minute;
    HI_U32  	second;
} PMoC_TIME_S,*PTR_PMoC_TIME_S;

typedef struct hiPMoC_STANDBY_MODE_S
{
    HI_U32		u32Mode;				/* 0 : no display; 1 : display channel; 2 : display time. */
    HI_U32		u32Channel_Disp_Code;	/* FIXME : channel display code, other than channel value */
	PMoC_TIME_S PMoC_Time_info;			/* system current time info */
} PMoC_STANDBY_MODE_S, *PTR_PMoC_STANDBY_MODE_S;
/* led display time when arm is stand by, add by David Huang 2008-05-02 end */

/* start modified by huangshiqiu 2008-01-16 AI7D02124 */
#define HI_PMoC_KEY_7_PRESS          (1 << 7)
#define HI_PMoC_KEY_6_PRESS          (1 << 6)
#define HI_PMoC_KEY_5_PRESS          (1 << 5)
#define HI_PMoC_KEY_4_PRESS          (1 << 4)
#define HI_PMoC_KEY_3_PRESS          (1 << 3)
#define HI_PMoC_KEY_2_PRESS          (1 << 2)
#define HI_PMoC_KEY_1_PRESS          (1 << 1)
#define HI_PMoC_KEY_0_PRESS          (1 << 0)
/* end modified by huangshiqiu 2008-01-16 AI7D02124 */

/*---- rtc ----*/
struct rtc_time_t
{
    HI_U32  u32year;
    HI_U32  u32month;
    HI_U32  u32date;
    HI_U32  u32hour;
    HI_U32  u32minute;
    HI_U32  u32second;
    HI_U32  u32weekday;
};

typedef struct rtc_time_t RTC_DATETIME_S ,*PTR_RTC_DATETIME_S;

/*---- sc ----*/
typedef enum hiGPIO_PORT_FUNC_E
{
    HI_GPIO_PIN_UART          = 0, 
    HI_GPIO_PIN_I2C0          = 1,
    HI_GPIO_PIN_IR            = 2,
    HI_GPIO_PIN_SCI           = 3,
    HI_GPIO_PIN_SMCDDR_DMA    = 4,
    HI_GPIO_PIN_LED           = 5,
    HI_GPIO_PIN_AUX_CLK       = 6,
    HI_GPIO_PIN_SMC3          = 7,
    HI_GPIO_PIN_I2C1          = 8,
    HI_GPIO_PIN_SMC2_DMA      = 9,
    HI_GPIO_PIN_UART_DMA      = 10,
    HI_GPIO_PIN_PWM_SMC       = 11,
    HI_GPIO_PIN_TS_CERR       = 12,
    HI_GPIO_PIN_I2S_SSI_DMA   = 13,
} GPIO_PORT_FUNC_E;

/* HI_GPIO_PIN_SMCDDR_DMA    SSMCADD23/DMA_ACK */
#define SSMCADDR3_FUNC 0
#define DMA_ACK0_FUNC  1
#define GPIO2_1_FUNC   2

/* HI_GPIO_PIN_SMC2_DMA      SSMCC2/DMA_REQ0 */
#define SSMCCS2_FUNC   0 
#define DMA_REQ0_FUNC  1 
#define GPIO2_0_FUNC   2 

/* HI_GPIO_PIN_UART_DMA      UART/DMA/RGBX */
#define UART_CTS1_RTS1_FUNC  0
#define DMA_REQ2_ACK2_FUNC   4
#define GPIO0_56_FUNC        7

/* HI_GPIO_PIN_PWM_SMC       PWM/SSMCRDY/AUX_CLK0 */
#define PWM_FUNC             0
#define SSMCRDY_FUNC         1
#define AUX_CLK_FUNC         2
#define GPIO0_1_FUNC         3

/* HI_GPIO_PIN_I2S_SSI_DMA   I2S/SSI/DMA/UART */
#define I2S_FUNC             0
#define SSI_FUNC             8
#define CTS0_RTS0_FUNC       16
#define DMA_REQ1_ACK1_FUNC   28
#define GPIO2_2345_FUNC      31 

typedef struct HI_PINCONFIG_S
{
    GPIO_PORT_FUNC_E ePortFunc;
    HI_U32 PinSet;
}PINCONFIG_S;

/*---- tuner ----*/
#define TUNER_NUM 2
typedef enum  hiTUNER_Status_E
{
    HI_TUNER_SIGNAL_DROPPED = 0, 
    HI_TUNER_SIGNAL_CONNECTED   
}TUNER_Status_E;

typedef struct  hiTUNER_Data_S 
{
    HI_U32         port;
    HI_U32         u32Data; 
}TUNER_Data_S, *PTR_TUNER_Data_S;

typedef struct  hiTUNER_DataBUF_S 
{
    HI_U32         port;
    HI_U32         u32DataBuf[3]; 
}TUNER_DataBUF_S, *PTR_TUNER_DataBUF_S;

typedef enum hiTUNER_QAMType_E
{
    HI_TUNER_QAM_TYPE_16 = 0,
    HI_TUNER_QAM_TYPE_32,
    HI_TUNER_QAM_TYPE_64,
    HI_TUNER_QAM_TYPE_128,
    HI_TUNER_QAM_TYPE_256,
    HI_TUNER_QAM_TYPE_MAX
}TUNER_QAMType_E;

typedef enum hiTUNER_QamInside_E
{
    TUNER_QAMINSIDE_IN = 0,
    TUNER_QAMINSIDE_OUT,
    TUNER_QAMINSIDE_INVALID,
}TUNER_QamInside_E;

typedef struct hiTUNER_ACCQAMParameters_S
{
    HI_U32       u32frequency;        /* µ±Ç°ÆµµÀÆµÂÊÖµ */
    HI_U32       u32symbolRate;        /* µ±Ç°ÆµµÀ·ûºÅÂÊÖµ */
    TUNER_QAMType_E    eqamType;
    HI_U32       u32bSI;             /*·´×ª*/ 
}TUNER_ACCQAMParameters_S;


typedef struct  hiTUNER_Signal_S 
{
    HI_U32 port;
    TUNER_ACCQAMParameters_S  signal; 
}TUNER_Signal_S, *PTR_TUNER_Signal_S;

/* --------Dmac---------- */
enum dw_dmac_transfer_flow {
        Dmac_mem2mem_dma    = 0x0, /* mem to mem - DMAC   flow ctlr */
        Dmac_mem2prf_dma    = 0x1, /* mem to prf - DMAC   flow ctlr */
        Dmac_prf2mem_dma    = 0x2, /* prf to mem - DMAC   flow ctlr */
        Dmac_prf2prf_dma    = 0x3, /* prf to prf - DMAC   flow ctlr */
        Dmac_prf2mem_prf    = 0x4, /* prf to mem - periph flow ctlr */
        Dmac_prf2prf_srcprf = 0x5, /* prf to prf - source flow ctlr */
        Dmac_mem2prf_prf    = 0x6, /* mem to prf - periph flow ctlr */
        Dmac_prf2prf_dstprf = 0x7  /* prf to prf - dest   flow ctlr */
};

/****d* dmac.data/dw_dmac_transfer_type
 * DESCRIPTION
 *  This data type is used for selecting the transfer type for a
 *  specified DMA channel when using the specified driver API
 *  functions. See the DW_ahb_dmac databook for a detailed
 *  description on these transfer types.
 * SEE ALSO
 *  dw_dmac_setTransferType(), dw_dmac_getTransferType()
 * SOURCE
 */
enum dw_dmac_transfer_type {
    Dmac_transfer_row1  = 0x1, /* single block or last multi-block */
                               /*  no write back                   */
    Dmac_transfer_row2  = 0x2, /* multi-block auto-reload DAR      */
                               /*  contiguous SAR no write back    */
    Dmac_transfer_row3  = 0x3, /* multi-block auto reload SAR      */
                               /*  contiguous DAR no write back    */
    Dmac_transfer_row4  = 0x4, /* multi-block auto-reload SAR DAR  */
                               /*  no write back                   */
    Dmac_transfer_row5  = 0x5, /* single block or last multi-block */
                               /*  with write back                 */
    Dmac_transfer_row6  = 0x6, /* multi-block linked list DAR      */
                               /*  contiguous SAR with write back  */
    Dmac_transfer_row7  = 0x7, /* multi-block linked list DAR auto */
                               /*  reload SAR  with write back     */
    Dmac_transfer_row8  = 0x8, /* multi-block linked list SAR      */
                               /*  contiguous DAR with write back  */
    Dmac_transfer_row9  = 0x9, /* multi-block linked list SAR auto */
                               /*  reload DAR with write back      */
    Dmac_transfer_row10 = 0xa  /* multi-block linked list SAR DAR  */
                               /*  with write back                 */
};
/*****/

/****d* dmac.data/dw_dmac_burst_trans_length
 * DESCRIPTION
 *  This data type is used for selecting the burst transfer length
 *  on the source and/or destination of a DMA channel when using the
 *  specified driver API functions. These transfer length values do
 *  not relate to the AMBA HBURST parameter.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit field(s): (x = channel number)
 *    - CTLx.SRC_MSIZE, CTLx.DEST_MSIZE
 * SEE ALSO
 *  dw_dmac_setBurstTransLength(), dw_dmac_getBurstTransLength()
 * SOURCE
 */
enum dw_dmac_burst_trans_length {
    Dmac_msize_1   = 0x0,
    Dmac_msize_4   = 0x1,
    Dmac_msize_8   = 0x2,
    Dmac_msize_16  = 0x3,
    Dmac_msize_32  = 0x4,
    Dmac_msize_64  = 0x5,
    Dmac_msize_128 = 0x6,
    Dmac_msize_256 = 0x7
};
/*****/

/****d* dmac.data/dw_dmac_address_increment
 * DESCRIPTION
 *  This data type is used for selecting the address increment
 *  type for the source and/or destination on a DMA channel when using
 *  the specified driver API functions.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit-field(s): (x = channel number)
 *    - CTLx.SINC, CTLx.DINC
 * SEE ALSO
 *  dw_dmac_setAddressInc(), dw_dmac_getAddressInc()
 * SOURCE
 */
enum dw_dmac_address_increment {
    Dmac_addr_increment = 0x0,
    Dmac_addr_decrement = 0x1,
    Dmac_addr_nochange  = 0x2
};
/*****/

/****d* dmac.data/dw_dmac_transfer_width
 * DESCRIPTION
 *  This data type is used for selecting the transfer width for the
 *  source and/or destination on a DMA channel when using the specified
 *  driver API functions. This data type maps directly to the AMBA AHB
 *  HSIZE parameter.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit field(s): (x = channel number)
 *    - CTLx.SRC_TR_WIDTH, CTLx.DST_TR_WIDTH
 * SEE ALSO
 *  dw_dmac_setTransWidth(), dw_dmac_getTransWidth()
 * SOURCE
 */
enum dw_dmac_transfer_width {
    Dmac_trans_width_8   = 0x0,
    Dmac_trans_width_16  = 0x1,
    Dmac_trans_width_32  = 0x2,
    Dmac_trans_width_64  = 0x3,
    Dmac_trans_width_128 = 0x4,
    Dmac_trans_width_256 = 0x5
};
/*****/
/****d* dmac.data/dw_dmac_master_number
 * DESCRIPTION
 *  This data type is used to select the master interface number
 *  on the DMA Controller when using the specified driver API
 *  functions.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit-field(s): (x = channel number)
 *    - CTLx.SMS, CTLx.DMS, LLPx.LMS
 * SEE ALSO
 *  dw_dmac_setMstSelect(), dw_dmac_getMstSelect(),
 *  dw_dmac_setListMstSelect(), dw_dmac_getListMstSelect()
 * SOURCE
 */
enum dw_dmac_master_number {
    Dmac_master1 = 0x0,
    Dmac_master2 = 0x1,
    Dmac_master3 = 0x2,
    Dmac_master4 = 0x3
};


typedef struct hiDmac_Config_Para_S
{
        HI_U32 u32ChannelNum;
        HI_U32 pu32Para;
        HI_U32 *pu32SrcAddr;
        HI_U32 *pu32DstAddr;
        enum dw_dmac_transfer_flow      enumFlowCtrl;

        HI_U32                          transLen;
        enum dw_dmac_transfer_type      transType;
        HI_U32                          blockSize;
        HI_U32                          numBlocks;
        enum dw_dmac_burst_trans_length src_msize;
        enum dw_dmac_burst_trans_length dst_msize;
        enum dw_dmac_address_increment  sinc;
        enum dw_dmac_address_increment  dinc;
        enum dw_dmac_transfer_width     src_tr_width;
        enum dw_dmac_transfer_width     dst_tr_width;
        enum dw_dmac_master_number      ctl_sms;
        enum dw_dmac_master_number      ctl_dms;
}Dmac_Config_Para_S;

typedef enum enumTS_INTERFACE
{
	DVB_INTERFACE = 0, 
	SERIAL_INTERFACE, 
	PARALLEL_INTERFACE,
	TS_INTERFACE_MAX
}TUNER_TsInterface_E;

typedef enum enumTUNER_Type
{
    HI3130_INSIDE_MIN = 0x0,
    HI3130_INSIDE_TMX70600,
    HI3130_INSIDE_XG_3BL,
    HI3130_INSIDE_CD1616,
    HI3130_INSIDE_ALPS,
    HI3130_INSIDE_ALPS_TDAE,
    HI3130_INSIDE_TDCC,
    HI3130_INSIDE_TDA18251,
    HI3130_INSIDE_CD1616_LF_GIH_4,
    HI3130_INSIDE_MXL203,    
    HI3130_INSIDE_MAX,

    HI3130_OUTSIDE_MIN = 0x100,	
    HI3130_OUTSIDE_TMX70600,
    HI3130_OUTSIDE_XG_3BL,
    HI3130_OUTSIDE_CD1616,
    HI3130_OUTSIDE_ALPS, 
    HI3130_OUTSIDE_ALPS_TDAE,
    HI3130_OUTSIDE_TDCC,
    HI3130_OUTSIDE_TDA18251,
    HI3130_OUTSIDE_CD1616_LF_GIH_4,
    HI3130_OUTSIDE_MAX,
    
    PHILIP_1026 = 0x200, 
    ST_0297, 
    GX_ALPS,
    TUNER_TYPE_MAX  
}TUNER_Type_E;

typedef struct enumTUNER_RegRW
{
    HI_U32 port;
    HI_U8 rw_flag;
    HI_U8 reg_addr;    
    HI_U8 write_val;
    HI_U8 read_val;
}TUNER_RegRW_S, *ptr_tuner_reg_rw;

typedef struct enumTUNER_SetTuner
{
    HI_U32 freq;
    HI_S32 times;    
}TUNER_SetTuner_S, *ptr_SetTuner_S;

#define CIPHER_ENCRYPT 0
#define CIPHER_DECRYPT 1

typedef enum hiCIPHER_WORK_MODE_E
{
	ECB_MODE = 0,
	CBC_MODE,
	CFB_MODE,
	OFB_MODE,
	MAX_MODE
}CIPHER_WORK_MODE_E;

typedef enum hiCIPHER_ALG_E
{
	ALG_AES = 0,
	ALG_DES,
	ALG_3DES	
}CIPHER_ALG_E;

typedef enum hiCIPHER_KEY_LENGTH_E
{
	AES_KEY_128BIT = 0,
	DES_KEY_64BIT,  /* key valid on des  */
	DES_KEY_128BIT, /* 2key valid on 3des  */
}CIPHER_KEY_LENGTH_E;

typedef enum hiCIPHER_BIT_WIDTH_E
{
	WIDTH_8BIT = 0,
	WIDTH_64BIT,
	WIDTH_128BIT,
}CIPHER_BIT_WIDTH_E;

typedef struct hiCIPHER_CTRL_S
{
	CIPHER_ALG_E daes_alg;
	CIPHER_WORK_MODE_E daes_work_mode;
	CIPHER_BIT_WIDTH_E daes_bit_width;
	CIPHER_KEY_LENGTH_E daes_key_length;
}CIPHER_CTRL_S;

typedef struct hiCIPHER_DES_KEYIVIN_S
{
	HI_U32 cipher_key1[2];
	HI_U32 cipher_key2[2];
	HI_U32 ivin[2];
}CIPHER_DES_KEYIVIN_S;

typedef struct hiCIPHER_AES_KEYIVIN_S
{
	HI_U32 cipher_key[4];
	HI_U32 ivin[4];
}CIPHER_AES_KEYIVIN_S;

typedef union hiCIPHER_KEYIVIN_U
{
	CIPHER_DES_KEYIVIN_S des;
	CIPHER_AES_KEYIVIN_S aes;
}CIPHER_KEYIVIN_U;

typedef struct hiCIPHER_INFO_S
{
	HI_U32 handle_id;
	HI_U32 u32src;
	HI_U32 u32dest;
	HI_U32 u32length;
	HI_U32 u32flag;
}CIPHER_INFO_S;

typedef struct hiCIPHER_CONFIG_S
{
	HI_U32 handle_id;
	CIPHER_CTRL_S ctrl;
	CIPHER_KEYIVIN_U key_ivin;
}CIPHER_CONFIG_S;

typedef HI_S32 (*funcCipherCallback)(HI_VOID*);

/* eth */
typedef enum hiEth_LinkStatus_E
{
    ETH_LINK_STATUS_OFF = 0,
    ETH_LINK_STATUS_ON,
    ETH_LINK_STATUS_MAX
}Eth_LinkStatus_E, *PTR_Eth_LinkStatus_E;

typedef enum hiEth_Port_E
{
    ETH_PORT_UP = 0,
    ETH_PORT_DOWN,
    ETH_PORT_MAX
}Eth_Port_E;

typedef enum hiEth_WorkMode_E
{
    ETH_WORKMODE_UMDS = 0,
    ETH_WORKMODE_DMUS,
    ETH_WORKMODE_UMDM,    
    ETH_WORKMODE_MAX
}Eth_WorkMode_E;

typedef enum hiEth_LinkCfg_E
{
    ETH_LinkCfg_10M_HALF = 0,
    ETH_LinkCfg_10M_FULL,
    ETH_LinkCfg_100M_HALF,    
    ETH_LinkCfg_100M_FULL,
    ETH_LinkCfg_AUTONEG,
    ETH_LinkCfg_MAX
}Eth_LinkCfg_E;


typedef enum hiEth_MacTblIdx_E
{
    ETH_MACTBL_INDEX_0 = 0,
    ETH_MACTBL_INDEX_1,
    ETH_MACTBL_INDEX_2,
    ETH_MACTBL_INDEX_3,
    ETH_MACTBL_INDEX_4,
    ETH_MACTBL_INDEX_5,
    ETH_MACTBL_INDEX_6,
    ETH_MACTBL_INDEX_7,    
    ETH_MACTBL_INDEX_MAX
}Eth_MacTblIdx_E;

typedef enum hiEth_VlanTblIdx_E
{
    ETH_VLANTBL_INDEX_0 = 0,
    ETH_VLANTBL_INDEX_1,
    ETH_VLANTBL_INDEX_2,
    ETH_VLANTBL_INDEX_3,
    ETH_VLANTBL_INDEX_4,
    ETH_VLANTBL_INDEX_5,
    ETH_VLANTBL_INDEX_6,
    ETH_VLANTBL_INDEX_7,    
    ETH_VLANTBL_INDEX_MAX
}Eth_VlanTblIdx_E;

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

typedef enum hiEth_Mactctrl_E
{
    ETH_MACT_BROAD2CPU_D = 0,
    ETH_MACT_BROAD2UP_D,
    ETH_MACT_MULTI2CPU_D,
    ETH_MACT_MULTI2UP_D,    
    ETH_MACT_UNI2CPU_D,
    ETH_MACT_UNI2UP_D,    
    ETH_MACT_BROAD2CPU_U,
    ETH_MACT_BROAD2DOWN_U,    
    ETH_MACT_MULTI2CPU_U,
    ETH_MACT_MULTI2DOWN_U,
    ETH_MACT_UNI2CPU_U,
    ETH_MACT_UNI2DOWN_U,    
    ETH_MACT_REG_CFG,
    ETH_MACT_MAX
}Eth_Mactctrl_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEBUG_H__ */


