#ifndef _APP_PARSERADV_H
#define _APP_PARSERADV_H

#define ADV_FLASH_BLOCK             		(64*1024)
#define ADV_SECTION_LENGTH  		(1024*4)
#define ADV_MAX_PIC_NUM                   40
#define ADV_STRIPE_NEWLAND_ID			  65535
#define ADV_DATA_PID			  8170
#define ADV_MAX_CHANNEL_NUM                   20

#define      	advxml_picname_max			60	
#define      	advxml_text_max				100 /*允许最多50个汉字*/	
#define      	ADVXML_INVALID				-1	
#define 		adv_max_ASCII_pixels			5
#define      	ADV_X_OFFSET					15	
#define 		ADV_Y_OFFSET					0

#ifndef NULL
#define NULL  0
#endif

#ifndef INVALID_PID
#define   INVALID_PID			       0x2000
#endif

typedef	struct 
{
	BU16 		ServiceId;
	BU08 		AD_Headdata_table_id;
	BU08 		AD_Picdata_table_id;
	BU16 		AD_PID;
	BU16		Index_xml_table_extension_id_start;
	BU16	 	Index_xml_table_extension_id_end;
	BU32 		Index_xml_size;
	BU16	 	Pic_info_table_extension_id_start;
	BU16	 	Pic_info_table_extension_id_end;
	BU32 		Pic_info_size;

	BOOLEAN BAT_OK;
	BOOLEAN PAT_OK;
	BOOLEAN PMT_OK;
	BOOLEAN DownCtrlInfo_OK;
	BOOLEAN PicInfo_OK;
	BOOLEAN Index_OK;
	BOOLEAN PicData_OK;

	BU08* 		IndexDataTemp;
	BU08* 		PicInfoDataTemp;

}ADV_PICINFO_STRUCT;

/***********************************************/
typedef struct
{
	BU16   PicID;
	BU16   TotalSectNum;
	BU32    mask[8];
	BU08*   buf;
	BOOLEAN Pic_OK;
} ADV_PICDATA_STRUCT;

typedef enum advxml_element_type
{
	XMLELEMENT_TYPE_INVALID = 0,
		
	ELEMENT_TYPE_ROOT_START,
	ELEMENT_TYPE_ROOT_END,

	ELEMENT_TYPE_BOOTLOGO_START,
	ELEMENT_TYPE_BOOTLOGO_END,
	
	ELEMENT_TYPE_MAINMENU_START,
	ELEMENT_TYPE_MAINMENU_END,

	ELEMENT_TYPE_STRIPE_START,
	ELEMENT_TYPE_STRIPE_END,
	
	ELEMENT_TYPE_LOGO_START,
	ELEMENT_TYPE_LOGO_END,
	
	ELEMENT_TYPE_AD_START,
	ELEMENT_TYPE_AD_END,
	
	ELEMENT_TYPE_CURRENT_FOLLOW_START,
	ELEMENT_TYPE_CURRENT_FOLLOW_END,
	
	ELEMENT_TYPE_CHANNEL_START,
	ELEMENT_TYPE_CHANNEL_END,
	ELEMENT_TYPE_CHANNEL_INFO,
	
	ELEMENT_TYPE_FILE_NUMBER_START,
	ELEMENT_TYPE_FILE_NUMBER_END,
	
	ELEMENT_TYPE_FORMAT_START,
	ELEMENT_TYPE_FORMAT_END,
	
	ELEMENT_TYPE_POSITION_X_START,
	ELEMENT_TYPE_POSITION_X_END,
	
	ELEMENT_TYPE_POSITION_Y_START,
	ELEMENT_TYPE_POSITION_Y_END,

	ELEMENT_TYPE_WIDTH_START,
	ELEMENT_TYPE_WIDTH_END,
	
	ELEMENT_TYPE_HEIGHT_START,
	ELEMENT_TYPE_HEIGHT_END,

	ELEMENT_TYPE_DURATION_START,
	ELEMENT_TYPE_DURATION_END,
	
	ELEMENT_TYPE_TEXT_START,
	ELEMENT_TYPE_TEXT_END,
	
	ELEMENT_TYPE_NAME
	
}ADVXML_ELEMENT_TYPE ;

typedef enum format_t
{
	_GIF = 0,
	_JPG,
	_PNG,
	_BMP,
	_1555,
	_MPG
}FORMAT_T;

typedef enum adv_nametype
{
	Bootlogo = 0,
	MainMenu,
	Stripe
}ADV_NAMETYPE;

typedef struct advxml_struct
{
	BU08	pic_name[advxml_picname_max];
	BU08       text[advxml_text_max];
	int  	total_channel_number;
	int        channel_name[ADV_MAX_CHANNEL_NUM];
	int	       file_number;
	int	       position_x;
	int	       position_y;
	int	       width;
	int	       height;	
	int	       duration;
	FORMAT_T      format;
	ADV_NAMETYPE	nametype;
	BU32 		Pic_original_size;
	BU32 		Pic_compressed_size;
	BU16 		PIC_ID;
	BU16 		Pic_segment_length;
	BU16 		Pic_segment_num;
	BOOLEAN Pic_compressed_flag;
	BU08* 		PicData_p;
}ADVXML_STRUCT;

typedef enum
{
	TrueColor = 0,
	CLUT8
}ADV_DRAW_TYPE;

typedef	enum
{
	MAINMENU_REQ,
	STRIPE_REQ
} FUNCTION_STRUCR;

typedef	struct	adv_msg_struct 
{
  	FUNCTION_STRUCR		DisplayReq;
} ADV_MSG_STRUCT;

#endif



