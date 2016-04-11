#include <os/tds2/alitypes.h>
#include <osal/osal.h>
#include <hld/hld_dev.h>
#include <hld/dmx/dmx.h>
#include <irca/pubh/s3_vddf.h>
#include <irca/pubh/s3_vdsc.h>


/**************************************SMC*****************************************************/
typedef enum
{
	CA_SMC_CARD_IN_E                    = 0x00000001,
	CA_SMC_CARD_OUT_E                   = 0x00000002,
	CA_DVT_SMC_CARD_CHNG_E                  = 0x80000000
}CA_SMC_CARD_EVENT_E;
enum  
{
   T0   = 0,
   T14 = 14,
   INVALIDT15 = 15
};

typedef struct
{
   vd_sc_atr_st  Length;
   BYTE 	 pATR[33];
}ATR_DATA_STRUCT;
/****************************************OS***************************************************/



/* Private Constants ------------------------------------------------------ */
#define     IRCA_MAX_TASK_NUM       6
#define 	IRCA_TASK_QUANTUM		10
#define 	IRCA_TASK_PRIORITY		OSAL_PRI_NORMAL
#define 	IRCA_TASK_STACKSIZE		0x1000
/* private variables ------------------------------------------------------ */
typedef struct
{
	ID			pstTaskID;
	OSAL_ID 	pstMessageQueue;
	int 		MessageLength;
}IRCA_TASK_STRUCT;

typedef struct
{
	void *    MsgData;
	ia_word16 MsgLen;
}IRCA_MESSAGE_STRUCT;


/*****************************************DEMUX**************************************************/
#define NR_OF_CA_CHANNELS    (5)
#define	NR_OF_CA_FILTER		 (12)
#define CA_QUEUE_SIZE        (100)
#define CAS_MAX_SEC_SIZE     (1024)

typedef	enum
{
	CHANNEL_FREE= 0,
	CHANNEL_READY,
	CHANNEL_FILTERSET,
	CHANNEL_INUSE
}CHANNEL_STATUS;

typedef enum
{
	DISABLE_FILTER = 0,
	ENABLE_FILTER,
	READY_FILTER
} filter_command;

typedef struct
{
	struct get_section_param get_sec_param;
	struct restrict mask_value;
	UINT8 mv_enable[NR_OF_CA_FILTER];
	UINT8 flt_id;
	UINT8 sec_buf[CAS_MAX_SEC_SIZE];

	void 					(*ReceiveCallback)(vd_ref_id,ia_word32);
	ia_word32 				wSoftCellVal;
	ia_word16 				wMaxSectionSize;
	vd_df_channel_type_e 	eChannelType;

	CHANNEL_STATUS		    channel_status;
	ia_word16				PidValue;

	vd_df_filter_type_e 	eFilterType;
	ia_word16 				wFilterLength;
	void 					(*EventCallback)(vd_ref_id,ia_word32,vd_df_event_e,ia_word16,void*);

 }Channel_t;

typedef struct
{
   BYTE     	pSection[1024];
   UINT32     	SectionLength; 
   BYTE 		SlotIndex;
   BYTE			FilterIndex;
}QUEUE_STRUCT;

/*************************************DESC******************************************************/
#define MAX_DESC_NUM  			16
#define INVALID_DESC_INDEX  	MAX_DESC_NUM

typedef enum
{
	DESCFREE=0,
	DESCOCCUPIED=1 
}Irca_DESC_Status;

typedef struct 
{
	Irca_DESC_Status DescStatus ;
}Irca_Desc_Struct;

typedef struct
{
	WORD EsPid;
	BYTE EsType;
	BYTE DescIndex;
}Irca_EsDecs_Struct;

/*************************************STORAGE******************************************************/

/* Private Constants ------------------------------------------------------ */

/*        << NVRAM MEMORY MAP >>

   0  +=============================+ GENERAL_PT_INDEX = 0x0000
      |                             |
      |                             |
      |      General Partition      |
      |                             |
      |      GENERAL_PT_SIZE        |
 7116 +-----------------------------+ CA_PT_INDEX=(GENERAL_PT_INDEX + GENERAL_PT_SIZE)
      |                             |
      |        CA Partition         |
      |         (512 bytes)         |
 7628 +-----------------------------+ LOADER_PT_INDEX=(CA_PT_INDEX + CA_PT_SIZE)
      |                             |
      |      Loader Partition       |
      |    LOADER_PT_SIZE (52B)     |
 7680 +-----------------------------+ IRDETO_PT_INDEX=(LOADER_PT_INDEX + LOADER_PT_SIZE)
      |                             |
      |      Irdeto Partition       |
      |         (512 bytes)         |
 8192 +=============================+ 
以上是原在ST方案上的E2配置，不用爱迪德loader的话省去Loader Partition和Irdeto Partition*/

#define CA_PT_SIZE         (512)
#define CA_PT_INDEX        (0)/*起始地址*/


#define STG_REFID_INDEX (0)
#define MAX_STG_REFID   (5)

typedef struct
{
   vd_ref_id RefId;
   ia_word32 SoftCellVal; 
   void 	 (*pvErr_Notify)(vd_ref_id,ia_word32,ia_result);
   BOOL   		DeleTag;
}STG_STRUCT;

/*************************************Timer******************************************************/

#define MAX_TIMER_NUM    (10)

/* private variables ------------------------------------------------------ */
typedef	enum
{
	TIMER_CLOSE=0,
	TIMER_OPEN,
	TIMER_STOP,
	TIMER_START
}TIMER_STATUS;

typedef struct
{
	void 			(*TimeoutCallback)(vd_ref_id,ia_word32);
	UINT32 			StartTime;
	UINT32 			UsedTime;
	vd_ref_id 		RefId;
	ia_word32 		SoftCellVal;
	ia_bool 		Repetitive;
	ia_word32 		Timeout;
	TIMER_STATUS 	Status;
}TIMER_STRUCT;

/*************************************SI******************************************************/


#define      MAX_PMTDATA_LEN    400
#define      MAX_CATDATA_LEN    200

