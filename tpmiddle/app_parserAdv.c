/*****************************************************************************************************
1、2010-04-05    cwd create      备注:南平广告规范的实现，参考st实现的代码进行移植和修改
2、2010-05-10	 cwd modify		 添加进度条
3、2010-06-23	 cwd modify		 修改图片过大的出错处理以及同一张图片不进行重复显示
******************************************************************************************************/
#include "app_common.h"
#include "app_ipi_common.h"


ADV_PICDATA_STRUCT* PicDataTemp;
ADV_PICINFO_STRUCT* ADInfoStruct;

static int 			Adv_TotalPicNum=-1;
BU08* 		AdvPicDataPoint=NULL;	
BU08* 		Adv1555DataPoint=NULL;	
char cRolltext[255];

/*for XML parsing*/
static BU08		*xmlFile_BeginDataPoint;
static BU16		xmlFileLength = 0;
static BS08 AdvmentVersion = -1;

ADVXML_STRUCT*	Advxml_attribute=NULL;   
static BOOLEAN 		Adv_SkipFilter = false;

static void ADV_CacheData(BU08* Data, BU32 chnnelid, BU32 filterid);
/**********************************************************************************
**********************************************************************************/ 
BOOLEAN ADV_GetSkipFilter_OK(void)
{
	return Adv_SkipFilter;
}

void ADV_SetSkipFilter_OK(BU08  tag)
{
	Adv_SkipFilter=tag;
}


static BOOLEAN ADV_Filter_Getmask(BU08 *mask_hi,BU08 *mask_lo)
{
	int i,j;
	int index;
	BU16 maskvalue=0xffff;
	BU08   count1,count2;
	BU16 ext[ADV_MAX_PIC_NUM];

	if(Advxml_attribute == NULL)
		return true;
	
	memset(ext, 0xff, ADV_MAX_PIC_NUM * sizeof(USHORT));/*zsy:0806*/
	if(ADV_GetTotalPicNum()>ADV_MAX_PIC_NUM || ADV_GetTotalPicNum()<=0)
		return true;
	
	for(index = 0; index < ADV_GetTotalPicNum(); index++) 
		ext[index] = (Advxml_attribute+index)->PIC_ID;
	
	for(i=0;i<ADV_MAX_PIC_NUM;i++)
	{
	       count1=(ext[0]>>i)&1;
		for(j=1;j<index;j++)
		{
	              count2=(ext[j]>>i)&1;
		       if(count1!=count2)
		   	{
			   	maskvalue^=(1<<i);
				break;
	       	}
		}
	}
	*mask_hi=(BU08)(maskvalue>>8);
	*mask_lo=(BU08)(maskvalue&0xff);
	printf("*mask_hi = %d, *mask_lo = %d\n", *mask_hi, *mask_lo);
	
	return false;
}

/**********************************************************************************
**********************************************************************************/ 
void ADV_SetTotalPicNum(int num)
{
	Adv_TotalPicNum = num;
}

int  ADV_GetTotalPicNum(void)
{
	return Adv_TotalPicNum;
}


void ADV_FlashUpgradeProcess(INT Step)
{
	char acDisplayStr[6];
	
	if (Step < 0)
	{
		Step = 0;
	}
	else if (Step > 100)
	{
		Step = 100;
	}

	sprintf(acDisplayStr, "%d%%", Step);
	GUI_API_FillRect(GUI_API_GetDesktopWindow(),540, 438, 60, 25, RGB(142,184,232));
	GUI_API_TextOut(GUI_API_GetDesktopWindow(), 560, 440, acDisplayStr, sizeof(acDisplayStr), RGB(0, 0, 0), RGB(142,184,232), 0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);

}

static BU32 AdvMask[8];
static BU32 TotalSectionNum;

void ADV_DefaultSectionNumTag()
{
	int i;

	for(i=0;i<8;i++)
		AdvMask[i] = 0;

	TotalSectionNum = 0;
}

BS32 ADV_SearchPicCacheIndex(BU16 PicId)/*zsy:0722*/
{
	BS32 i = -1;
	BS32 CacheIndex = -1;
//	APP_PRINT_DEBUG("PicId = %d\n", PicId);
	for (i = 0; i < ADV_GetTotalPicNum(); i++)
	{
		if ((Advxml_attribute+i)->PIC_ID == PicId)
		{
			break;
		}
	}

	if (i < ADV_GetTotalPicNum())
	{
		CacheIndex = i;
	}

	return CacheIndex;
}


/**********************************************************************************
**********************************************************************************/ 
int ADV_GetPicTotalNum(ADV_NAMETYPE type)
{
	int PicNum=0;
	int i;
	
	if(ADV_GetTotalPicNum() <=0 || Advxml_attribute==NULL)
		return 0;
	
	for(i=0;i<ADV_GetTotalPicNum();i++)
	{
		if((Advxml_attribute+i)->nametype == type )
			PicNum++;
	}
	return PicNum;
}

/**********************************************************************************
**********************************************************************************/ 
int ADV_GetFirstPicNum(ADV_NAMETYPE type)
{
	int PicNum=-1;

	if(ADV_GetTotalPicNum() <=0 || Advxml_attribute==NULL)
		return -1;

	for(PicNum=0;PicNum<ADV_GetTotalPicNum();PicNum++)
	{
		if((Advxml_attribute+PicNum)->nametype == type )
			break;
	}
	/*if(PicNum == ADV_GetTotalPicNum())
	{
		return -1;
	}*/
	return PicNum;
}

/**********************************************************************************
**********************************************************************************/ 
int ADV_GetPicMemorySize(void)
{
	int TotalLength=0;
	int i;
	
	for(i=0;i<ADV_GetTotalPicNum();i++)
		TotalLength += (Advxml_attribute+i)->Pic_original_size;

	return TotalLength;
}

BU08 * ADV_GetMpgPicInfo(INT * Size)/*zsy:1209*/
{
	int i = 0;
	BU08 * MpgPicInfoAddr = NULL;

	if (Size != NULL)
	{
		*Size = 0;
		
		if (Advxml_attribute != NULL)
		{
			for (i = 0; i < ADV_GetTotalPicNum(); i++)
			{
				if ((Advxml_attribute+i)->format == _MPG)
				{
					MpgPicInfoAddr = (Advxml_attribute+i)->PicData_p;
					*Size = (Advxml_attribute+i)->Pic_original_size;
					break;
				}
			}
		}
	}

	return MpgPicInfoAddr;
}

#define Malloc_DataStruct

void ADV_FreeXMLStruct(void)
{
	APP_PRINT_DEBUG("Ready to free Advxml_attribute......\n");
	
	if(Advxml_attribute != NULL)
	{
		free(Advxml_attribute);
		Advxml_attribute = NULL;
		APP_PRINT_DEBUG("***free Advxml_attribute\n");
	}
}

BOOLEAN ADV_MallocXMLStruct(void)
{
	int i;
	int StructSize=0;
	int channeltemp = 0;

	StructSize = sizeof(ADVXML_STRUCT)*ADV_GetTotalPicNum();
	//APP_PRINT_DEBUG("StructSize = %d\n",StructSize);
	if((StructSize+4) > 1024 * 20) /*+4 is for the TotalNum*/
	{
		APP_PRINT_DEBUG("has no enough flash memory for adv PicInfo Struct!\n");
		return true;
	}
	ADV_FreeXMLStruct();
	if(Advxml_attribute == NULL)
	{
		Advxml_attribute = (ADVXML_STRUCT*) malloc(ADV_GetTotalPicNum()*sizeof(ADVXML_STRUCT));
		APP_PRINT_DEBUG("****malloc Advxml_attribute=0x%x, size=%d\n",Advxml_attribute,(ADV_GetTotalPicNum()*sizeof(ADVXML_STRUCT)));
	}
	if(Advxml_attribute == NULL)
	{
		APP_PRINT_DEBUG("no enough memory for Advxml_attribute\n");
		return true;
	}

	for (i=0; i<ADV_GetTotalPicNum(); i++)
	{
		memset((Advxml_attribute+i)->pic_name, 0, advxml_picname_max);
		memset((Advxml_attribute+i)->text, 0, advxml_text_max);
		(Advxml_attribute+i)->total_channel_number = 0;
		for(channeltemp = 0; channeltemp < ADV_MAX_CHANNEL_NUM; channeltemp++)
		{
			(Advxml_attribute+i)->channel_name[channeltemp]=ADVXML_INVALID;
		}
		(Advxml_attribute+i)->duration=ADVXML_INVALID;
		(Advxml_attribute+i)->file_number=ADVXML_INVALID;
		(Advxml_attribute+i)->position_x=ADVXML_INVALID;
		(Advxml_attribute+i)->position_y=ADVXML_INVALID;
		(Advxml_attribute+i)->width=ADVXML_INVALID;
		(Advxml_attribute+i)->height=ADVXML_INVALID;
		(Advxml_attribute+i)->format=ADVXML_INVALID;
		(Advxml_attribute+i)->nametype=ADVXML_INVALID;
		
		(Advxml_attribute+i)->PIC_ID = 0;
		(Advxml_attribute+i)->Pic_original_size=0;
		(Advxml_attribute+i)->Pic_segment_length=0;
		(Advxml_attribute+i)->Pic_segment_num=0;
		(Advxml_attribute+i)->Pic_compressed_flag=false;
		(Advxml_attribute+i)->Pic_compressed_size=0;
		(Advxml_attribute+i)->PicData_p = NULL;
		
	}

	return false;

}

static BOOLEAN ADV_MallocPicDataTempStruct(void)
{
	int i;

	APP_PRINT_DEBUG("Ready to malloc PicDataTemp......\n");

	if(PicDataTemp == NULL)
	{
		//PicDataTemp = (ADV_PICDATA_STRUCT*) malloc(ADV_GetTotalPicNum()*sizeof(ADV_PICDATA_STRUCT));
		PicDataTemp = (ADV_PICDATA_STRUCT*) FYF_API_malloc(ADV_GetTotalPicNum()*sizeof(ADV_PICDATA_STRUCT));
		APP_PRINT_DEBUG("****malloc PicDataTemp=0x%x, size=%d\n",PicDataTemp,(ADV_GetTotalPicNum()*sizeof(ADV_PICDATA_STRUCT)));
	}
	if(PicDataTemp == NULL)
	{
		APP_PRINT_DEBUG("no enough memory for PicDataTemp\n");
		return true;
	}

	for(i=0;i<ADV_GetTotalPicNum();i++)
	{
		(PicDataTemp+i)->PicID = i+1;
		(PicDataTemp+i)->TotalSectNum = 0;
		memset((PicDataTemp+i)->mask,0,8*sizeof(BU32));/*zsy:0806*/
		(PicDataTemp+i)->Pic_OK = false;
		//(PicDataTemp+i)->buf = (BU08*)malloc(ADV_SECTION_LENGTH*((Advxml_attribute+i)->Pic_segment_num));
		(PicDataTemp+i)->buf = (BU08*)FYF_API_malloc (ADV_SECTION_LENGTH*((Advxml_attribute+i)->Pic_segment_num));
		if((PicDataTemp+i)->buf == NULL)
		{
			APP_PRINT_DEBUG("no enough memory for PicDataTemp buf\n");
			return true;
		}
		/*APP_PRINT_DEBUG("*******malloc (PicDataTemp+%d)->buf, buf=0x%x,size = %d\n",
			i,(PicDataTemp+i)->buf,
			(ADV_SECTION_LENGTH*((Advxml_attribute+i)->Pic_segment_num)));*/

	}
	return false;
	
}

void ADV_FreePicDataTempStruct(void)
{
	int i;

	APP_PRINT_DEBUG("Ready to free PicDataTemp......\n");

	if(PicDataTemp != NULL)
	{
		for(i=0;i<ADV_GetTotalPicNum();i++)
		{
			if((PicDataTemp+i)->buf != NULL)
			{
				//free((PicDataTemp+i)->buf);
				FYF_API_free((PicDataTemp+i)->buf);
				(PicDataTemp+i)->buf = NULL;
			}	
		}
		//free(PicDataTemp);
		FYF_API_free(PicDataTemp);
		PicDataTemp = NULL;
		APP_PRINT_DEBUG("***free PicDataTemp\n");
	}
	
}


void ADV_FreePicDataPoint(void)
{
	APP_PRINT_DEBUG("Ready to free AdvPicDataPoint......\n");

	if(AdvPicDataPoint!=NULL)
	{
		FYF_API_free (AdvPicDataPoint);
		AdvPicDataPoint = NULL;
		APP_PRINT_DEBUG("***free AdvPicDataPoint\n");
	}
}

BOOLEAN ADV_MallocPicDataPoint(void)
{
	int TotalLength=0;

	TotalLength = ADV_GetPicMemorySize();
	APP_PRINT_DEBUG("original Total Pic Size = %d\n",TotalLength);
	if(TotalLength > 1024 * 380)
	{
		APP_PRINT_DEBUG("has no enough flash memory for AdvPicDataPoint! TotalLength = %d\n",TotalLength);
		return true;
	}
	ADV_FreePicDataPoint();
	if(AdvPicDataPoint==NULL)
	{
		if(TotalLength&1 != 0)
			TotalLength+=1;
		
		//AdvPicDataPoint = (BU08*)malloc(TotalLength);
		AdvPicDataPoint = (BU08*)FYF_API_malloc(TotalLength);

		APP_PRINT_DEBUG("****malloc AdvPicDataPoint=0x%x, size=%d\n",AdvPicDataPoint,TotalLength);
	}
	if(AdvPicDataPoint==NULL)
	{
		APP_PRINT_DEBUG("no enough memory for AdvPicDataPoint\n");
		return true;
	}

	return false;
	
}

#define Parse_XML

/****************************************************************************************/
/****************************************************************************************/
static ADVXML_ELEMENT_TYPE ADV_Parse_XMLElementType(BU08 *datapoint)/*zsy:0723*/
{
	//ELEMENT_TYPE_ROOT_START,
	//ELEMENT_TYPE_ROOT_END,
	if(datapoint[0]=='<'\
		&& (datapoint[1]=='r'||datapoint[1]=='R') && (datapoint[2]=='o'||datapoint[2]=='O')\
		&& (datapoint[3]=='o'||datapoint[3]=='O')&& (datapoint[4]=='t'||datapoint[4]=='T') && (datapoint[5] == '>'))

		return ELEMENT_TYPE_ROOT_START;

	else if(datapoint[0]=='<' && datapoint[1]=='/'\
		&& (datapoint[2]=='r'||datapoint[2]=='R') && (datapoint[3]=='o'||datapoint[3]=='O')\
		&& (datapoint[4]=='o'||datapoint[4]=='O')&& (datapoint[5]=='t'||datapoint[5]=='T'))

		return ELEMENT_TYPE_ROOT_END;
	
	//ELEMENT_TYPE_BOOTLOGO_START,
	//ELEMENT_TYPE_BOOTLOGO_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='b'||datapoint[1]=='B') && (datapoint[2]=='o'||datapoint[2]=='O')\
		&& (datapoint[3]=='o'||datapoint[3]=='O') && (datapoint[4]=='t'||datapoint[4]=='T') && (datapoint[5]=='l'||datapoint[5]=='L')\
		&& (datapoint[6]=='o'||datapoint[6]=='O') && (datapoint[7]=='g'||datapoint[7]=='G') && (datapoint[8]=='o'||datapoint[8]=='O')\
		&& (datapoint[9] == '>'))

		return ELEMENT_TYPE_BOOTLOGO_START;

	else if(datapoint[0]=='<' && datapoint[1]=='/' && (datapoint[2]=='b'||datapoint[2]=='B') && (datapoint[3]=='o'||datapoint[3]=='O')\
		&& (datapoint[4]=='o'||datapoint[4]=='O') && (datapoint[5]=='t'||datapoint[5]=='T') && (datapoint[6]=='l'||datapoint[6]=='L')\
		&& (datapoint[7]=='o'||datapoint[7]=='O') && (datapoint[8]=='g'||datapoint[8]=='G') && (datapoint[9]=='o'||datapoint[9]=='O'))

		return ELEMENT_TYPE_BOOTLOGO_END;

	//ELEMENT_TYPE_MAINMENU_START,
	//ELEMENT_TYPE_MAINMENU_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='m'||datapoint[1]=='M') && (datapoint[2]=='a'||datapoint[2]=='A') && (datapoint[3]=='i'||datapoint[3]=='I')\
		&& (datapoint[4]=='n'||datapoint[4]=='N') && (datapoint[5]=='m'||datapoint[5]=='M') && (datapoint[6]=='e'||datapoint[6]=='E')\
		&& (datapoint[7]=='n'||datapoint[7]=='N') && (datapoint[8]=='u'||datapoint[8]=='U') && (datapoint[9] == '>'))

		return ELEMENT_TYPE_MAINMENU_START;

	else if(datapoint[0]=='<' && datapoint[1]=='/' && (datapoint[2]=='m'||datapoint[2]=='M') && (datapoint[3]=='a'||datapoint[3]=='A') && (datapoint[4]=='i'||datapoint[4]=='I')\
		&& (datapoint[5]=='n'||datapoint[5]=='N') && (datapoint[6]=='m'||datapoint[6]=='M') && (datapoint[7]=='e'||datapoint[7]=='E')\
		&& (datapoint[8]=='n'||datapoint[8]=='N') && (datapoint[9]=='u'||datapoint[9]=='U'))
		
		return ELEMENT_TYPE_MAINMENU_END;

	//ELEMENT_TYPE_STRIPE_START,
	//ELEMENT_TYPE_STRIPE_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='s'||datapoint[1]=='S') && (datapoint[2]=='t'||datapoint[2]=='T') && (datapoint[3]=='r'||datapoint[3]=='R')\
		&& (datapoint[4]=='i'||datapoint[4]=='I') && (datapoint[5]=='p'||datapoint[5]=='P') && (datapoint[6]=='e'||datapoint[6]=='E') && (datapoint[7] == '>'))

		return ELEMENT_TYPE_STRIPE_START;

	else if(datapoint[0]=='<' && datapoint[1]=='/' && (datapoint[2]=='s'||datapoint[2]=='S') && (datapoint[3]=='t'||datapoint[3]=='T') && (datapoint[4]=='r'||datapoint[4]=='R')\
		&& (datapoint[5]=='i'||datapoint[5]=='I') && (datapoint[6]=='p'||datapoint[6]=='P') && (datapoint[7]=='e'||datapoint[7]=='E'))

		return ELEMENT_TYPE_STRIPE_END;
	
	//ELEMENT_TYPE_LOGO_START,
	//ELEMENT_TYPE_LOGO_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='l'||datapoint[1]=='L') && (datapoint[2]=='o'||datapoint[2]=='O')\
		&& (datapoint[3]=='g'||datapoint[3]=='G')&& (datapoint[4]=='o'||datapoint[4]=='O'))

		return ELEMENT_TYPE_LOGO_START;
	
	else if(datapoint[0]=='<' && datapoint[1]=='/' && (datapoint[2]=='l'||datapoint[2]=='L') && (datapoint[3]=='o'||datapoint[3]=='O')\
		&& (datapoint[4]=='g'||datapoint[4]=='G')&& (datapoint[5]=='o'||datapoint[5]=='O'))

		return ELEMENT_TYPE_LOGO_END;	

	//ELEMENT_TYPE_AD_START,
	//ELEMENT_TYPE_AD_END,
	else if(datapoint[0] == '<' && (datapoint[1] == 'a'||datapoint[1] == 'A') && (datapoint[2] == 'd'||datapoint[2] == 'D'))
		
		return ELEMENT_TYPE_AD_START;
	
	else if(datapoint[0] == '<' && datapoint[1] == '/' && (datapoint[2] == 'a'||datapoint[2] == 'A') && (datapoint[3] == 'd'||datapoint[3] == 'D'))

		return ELEMENT_TYPE_AD_END;
	
	//ELEMENT_TYPE_CURRENT_FOLLOW_START,
	//ELEMENT_TYPE_CURRENT_FOLLOW_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='c'||datapoint[1]=='C') && (datapoint[2]=='u'||datapoint[2]=='U')\
		&& (datapoint[3]=='r'||datapoint[3]=='R') && (datapoint[4]=='r'||datapoint[4]=='R') && (datapoint[5]=='e'||datapoint[5]=='E')\
		&& (datapoint[6]=='n'||datapoint[6]=='N') && (datapoint[7]=='t'||datapoint[7]=='T') && datapoint[8]=='_'\
		&& (datapoint[9]=='f'||datapoint[9]=='F') && (datapoint[10]=='o'||datapoint[10]=='O'))
		
		return ELEMENT_TYPE_CURRENT_FOLLOW_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/' && (datapoint[2]=='c'||datapoint[2]=='C') && (datapoint[3]=='u'||datapoint[3]=='U')\
		&& (datapoint[4]=='r'||datapoint[4]=='R') && (datapoint[5]=='r'||datapoint[5]=='R') && (datapoint[6]=='e'||datapoint[6]=='E')\
		&& (datapoint[7]=='n'||datapoint[7]=='N') && (datapoint[8]=='t'||datapoint[8]=='T') && datapoint[9]=='_'\
		&& (datapoint[10]=='f'||datapoint[10]=='F') && (datapoint[11]=='o'||datapoint[11]=='O'))
		
		return ELEMENT_TYPE_CURRENT_FOLLOW_END;

	//ELEMENT_TYPE_CHANNEL_START,
	//ELEMENT_TYPE_CHANNEL_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='c'||datapoint[1]=='C') && (datapoint[2]=='h'||datapoint[2]=='H')\
		&& (datapoint[3]=='a'||datapoint[3]=='A') && (datapoint[4]=='n'||datapoint[4]=='N') && (datapoint[5]=='n'||datapoint[5]=='N')\
		&& (datapoint[6]=='e'||datapoint[6]=='E') && (datapoint[7]=='l'||datapoint[7]=='L')&&datapoint[8]=='_')
		
		return ELEMENT_TYPE_CHANNEL_INFO;

	else if(datapoint[0]=='<' && (datapoint[1]=='c'||datapoint[1]=='C') && (datapoint[2]=='h'||datapoint[2]=='H')\
		&& (datapoint[3]=='a'||datapoint[3]=='A') && (datapoint[4]=='n'||datapoint[4]=='N') && (datapoint[5]=='n'||datapoint[5]=='N')\
		&& (datapoint[6]=='e'||datapoint[6]=='E') && (datapoint[7]=='l'||datapoint[7]=='L'))
		
		return ELEMENT_TYPE_CHANNEL_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/'  && (datapoint[2]=='c'||datapoint[2]=='C') && (datapoint[3]=='h'||datapoint[3]=='H')\
		&& (datapoint[4]=='a'||datapoint[4]=='A') && (datapoint[5]=='n'||datapoint[5]=='N') && (datapoint[6]=='n'||datapoint[6]=='N')\
		&& (datapoint[7]=='e'||datapoint[7]=='E') && (datapoint[8]=='l'||datapoint[8]=='L'))
		
		return ELEMENT_TYPE_CHANNEL_END;
	
	
	//ELEMENT_TYPE_FILE_NUMBER_START,
	//ELEMENT_TYPE_FILE_NUMBER_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='f'||datapoint[1]=='F') && (datapoint[2]=='i'||datapoint[2]=='I')\
		&& (datapoint[3]=='l'||datapoint[3]=='L') && (datapoint[4]=='e'||datapoint[4]=='E') && datapoint[5]=='_'\
		&& (datapoint[6]=='n'||datapoint[6]=='N')&& (datapoint[7]=='u'||datapoint[7]=='U') && (datapoint[8]=='m'||datapoint[8]=='M')\
		&& (datapoint[9]=='b'||datapoint[9]=='B') && (datapoint[10]=='e'||datapoint[10]=='E')&& (datapoint[11]=='r'||datapoint[11]=='R'))
		
		return ELEMENT_TYPE_FILE_NUMBER_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/' && (datapoint[2]=='f'||datapoint[2]=='F') && (datapoint[3]=='i'||datapoint[3]=='I')\
		&& (datapoint[4]=='l'||datapoint[4]=='L') && (datapoint[5]=='e'||datapoint[5]=='E') && datapoint[6]=='_' && (datapoint[7]=='n'||datapoint[7]=='N')\
		&& (datapoint[8]=='u'||datapoint[8]=='U') && (datapoint[9]=='m'||datapoint[9]=='M') && (datapoint[10]=='b'||datapoint[10]=='B')\
		&& (datapoint[11]=='e'||datapoint[11]=='E')&& (datapoint[12]=='r'||datapoint[12]=='R'))
		
		return ELEMENT_TYPE_FILE_NUMBER_END;
	
	//ELEMENT_TYPE_FORMAT_START,
	//ELEMENT_TYPE_FORMAT_END,
	else if(datapoint[0] == '<' && (datapoint[1] == 'f'||datapoint[1] == 'F') && (datapoint[2] == 'o'||datapoint[2] == 'O')\
		&& (datapoint[3] == 'r'||datapoint[3] == 'R') && (datapoint[4] == 'm'||datapoint[4] == 'M')\
		&& (datapoint[5] == 'a'||datapoint[5] == 'A') && (datapoint[6] == 't'||datapoint[6] == 'T'))
		
		return ELEMENT_TYPE_FORMAT_START;

	else if(datapoint[0] == '<' && datapoint[1] == '/' && (datapoint[2] == 'f'||datapoint[2] == 'F') && (datapoint[3] == 'o'||datapoint[3] == 'O')\
		&& (datapoint[4] == 'r'||datapoint[4] == 'R') && (datapoint[5] == 'm'||datapoint[5] == 'M')\
		&& (datapoint[6] == 'a'||datapoint[6] == 'A') && (datapoint[7] == 't'||datapoint[7] == 'T'))

		return ELEMENT_TYPE_FORMAT_END;
	
	//ELEMENT_TYPE_POSITION_X_START,
	//ELEMENT_TYPE_POSITION_X_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='p'||datapoint[1]=='P') && (datapoint[2]=='o'||datapoint[2]=='O')\
		&& (datapoint[3]=='s'||datapoint[3]=='S') && (datapoint[4]=='i'||datapoint[4]=='I')\
		&& (datapoint[5]=='t'||datapoint[5]=='t') && (datapoint[6]=='i'||datapoint[6]=='I')\
		&& (datapoint[7]=='o'||datapoint[7]=='O')&& (datapoint[8]=='n'||datapoint[8]=='N')\
		&& datapoint[9]=='_'&& (datapoint[10]=='x'||datapoint[10]=='X'))
		
		return ELEMENT_TYPE_POSITION_X_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/' && (datapoint[2]=='p'||datapoint[2]=='P') && (datapoint[3]=='o'||datapoint[3]=='O')\
		&& (datapoint[4]=='s'||datapoint[4]=='S') && (datapoint[5]=='i'||datapoint[5]=='I')\
		&& (datapoint[6]=='t'||datapoint[6]=='t') && (datapoint[7]=='i'||datapoint[7]=='I')\
		&& (datapoint[8]=='o'||datapoint[8]=='O')&& (datapoint[9]=='n'||datapoint[9]=='N')\
		&& datapoint[10]=='_'&& (datapoint[11]=='x'||datapoint[11]=='X'))

		return ELEMENT_TYPE_POSITION_X_END;
	
	//ELEMENT_TYPE_POSITION_Y_START,
	//ELEMENT_TYPE_POSITION_Y_END,

	else if(datapoint[0]=='<' && (datapoint[1]=='p'||datapoint[1]=='P') && (datapoint[2]=='o'||datapoint[2]=='O')\
		&& (datapoint[3]=='s'||datapoint[3]=='S') && (datapoint[4]=='i'||datapoint[4]=='I')\
		&& (datapoint[5]=='t'||datapoint[5]=='t') && (datapoint[6]=='i'||datapoint[6]=='I')\
		&& (datapoint[7]=='o'||datapoint[7]=='O')&& (datapoint[8]=='n'||datapoint[8]=='N')\
		&& datapoint[9]=='_'&& (datapoint[10]=='y'||datapoint[10]=='Y'))
		
		return ELEMENT_TYPE_POSITION_Y_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/' && (datapoint[2]=='p'||datapoint[2]=='P') && (datapoint[3]=='o'||datapoint[3]=='O')\
		&& (datapoint[4]=='s'||datapoint[4]=='S') && (datapoint[5]=='i'||datapoint[5]=='I')\
		&& (datapoint[6]=='t'||datapoint[6]=='t') && (datapoint[7]=='i'||datapoint[7]=='I')\
		&& (datapoint[8]=='o'||datapoint[8]=='O')&& (datapoint[9]=='n'||datapoint[9]=='N')\
		&& datapoint[10]=='_'&& (datapoint[11]=='y'||datapoint[11]=='Y'))

		return ELEMENT_TYPE_POSITION_Y_END;
	
	//ELEMENT_TYPE_WIDTH_START,
	//ELEMENT_TYPE_WIDTH_END,

	else if(datapoint[0]=='<' && (datapoint[1]=='w'||datapoint[1]=='W') && (datapoint[2]=='i'||datapoint[2]=='I') && (datapoint[3]=='d'||datapoint[3]=='D')\
		&& (datapoint[4] == 't'||datapoint[4] == 'T') && (datapoint[5] == 'h'||datapoint[5] == 'H'))
		
		return ELEMENT_TYPE_WIDTH_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/' && (datapoint[2]=='w'||datapoint[2]=='W') && (datapoint[3]=='i'||datapoint[3]=='I') && (datapoint[4]=='d'||datapoint[4]=='D')\
		&& (datapoint[5] == 't'||datapoint[5] == 'T') && (datapoint[6] == 'h'||datapoint[6] == 'H'))
		
		return ELEMENT_TYPE_WIDTH_END;
	
	//ELEMENT_TYPE_HEIGHT_START,
	//ELEMENT_TYPE_HEIGHT_END,
	
	else if(datapoint[0]=='<' && (datapoint[1]=='h'||datapoint[1]=='H') && (datapoint[2]=='e'||datapoint[2]=='E') && (datapoint[3]=='i'||datapoint[3]=='I')\
		&& (datapoint[4]=='g'||datapoint[4]=='G') && (datapoint[5]=='h'||datapoint[5]=='H')&& (datapoint[6]=='t'||datapoint[6]=='T'))
		
		return ELEMENT_TYPE_HEIGHT_START;

	else if(datapoint[0]=='<' && datapoint[1] == '/' && (datapoint[2]=='h'||datapoint[2]=='H') && (datapoint[3]=='e'||datapoint[3]=='E') && (datapoint[4]=='i'||datapoint[4]=='I')\
		&& (datapoint[5]=='g'||datapoint[5]=='G') && (datapoint[6]=='h'||datapoint[6]=='H')&& (datapoint[7]=='t'||datapoint[7]=='T'))
		
		return ELEMENT_TYPE_HEIGHT_END;

	//ELEMENT_TYPE_DURATION_START,
	//ELEMENT_TYPE_DURATION_END,
	else if(datapoint[0]=='<' && (datapoint[1]=='d'||datapoint[1]=='D') && (datapoint[2]=='u'||datapoint[2]=='U') && (datapoint[3]=='r'||datapoint[3]=='R')\
		&& (datapoint[4]=='a'||datapoint[4]=='A') && (datapoint[5]=='t'||datapoint[5]=='T') && (datapoint[6]=='i'||datapoint[6]=='I')\
		&& (datapoint[7]=='o'||datapoint[7]=='O') && (datapoint[8]=='n'||datapoint[8]=='N'))

		return ELEMENT_TYPE_DURATION_START;

	else if(datapoint[0]=='<' && datapoint[1]=='/' && (datapoint[2]=='d'||datapoint[2]=='D') && (datapoint[3]=='u'||datapoint[3]=='U') && (datapoint[4]=='r'||datapoint[4]=='R')\
		&& (datapoint[5]=='a'||datapoint[5]=='A') && (datapoint[6]=='t'||datapoint[6]=='T') && (datapoint[7]=='i'||datapoint[7]=='I')\
		&& (datapoint[8]=='o'||datapoint[8]=='O') && (datapoint[9]=='n'||datapoint[9]=='N'))
		
		return ELEMENT_TYPE_DURATION_END;

	//ELEMENT_TYPE_NAME
	else if((datapoint[0]=='n'||datapoint[0]=='N') && (datapoint[1]=='a'||datapoint[1]=='A')\
		&& (datapoint[2]=='m'||datapoint[2]=='M') && (datapoint[3]=='e'||datapoint[3]=='E'))

		return ELEMENT_TYPE_NAME;
	
	//ELEMENT_TYPE_ROOT_START,
	//ELEMENT_TYPE_ROOT_END,
	if(datapoint[0]=='<'\
		&& (datapoint[1]=='t'||datapoint[1]=='T') && (datapoint[2]=='e'||datapoint[2]=='E')\
		&& (datapoint[3]=='x'||datapoint[3]=='X')&& (datapoint[4]=='t'||datapoint[4]=='T'))

		return ELEMENT_TYPE_TEXT_START;

	else if(datapoint[0]=='<' && datapoint[1]=='/'\
		&& (datapoint[2]=='t'||datapoint[2]=='T') && (datapoint[3]=='e'||datapoint[3]=='E')\
		&& (datapoint[4]=='x'||datapoint[4]=='X')&& (datapoint[5]=='t'||datapoint[5]=='T'))

		return ELEMENT_TYPE_TEXT_END;
	
	else
		
		return XMLELEMENT_TYPE_INVALID;

}

/**********************************************************************************
**********************************************************************************/
static int  ADV_DataString2HexData(BU08 *filename, int count)
{
	int			j			= 0;
	int			i			= 0;
	int			sum			= 1;
	int			data_out	= 0;
	
	for(i=count-1;i>=0;i--)
	{
		if(filename[i]<48||filename[i]>57)
			continue;
		if(i<count-1)
			sum*=10;
		data_out+=(filename[i]-48)*sum;
	}
	if(data_out>1024)
		APP_PRINT_DEBUG("string2hexdata error !\n");
	  
	return data_out;
	
}
/**********************************************************************************
**********************************************************************************/
static BU16 ADV_Search_XmlDecData(BU08 *dataPoint,BU08 Specialtag,int *ret_data)
{
	int  count,j;
	BU08 bFirstTime=TRUE;
	BU08 bDataError= true;   

	BU08 *dataPointtemp;    
	BU08 *tempPoint;
	
	tempPoint = dataPoint;
	count = 0;
	tempPoint+=1;
	
	while(tempPoint[0] != Specialtag && ((tempPoint)<(xmlFile_BeginDataPoint+xmlFileLength)))
	{
		if(tempPoint[0]<=0x39 && tempPoint[0] >= 0x30)
		{
			for(j=0;j<adv_max_ASCII_pixels;j++)
			{
				if(tempPoint[0]<=0x39 && tempPoint[0] >= 0x30)
				{
					if(bFirstTime)
					{
						bFirstTime = FALSE;
						dataPointtemp=tempPoint;
					}
					tempPoint++;
					count += 1;
				}
				else
				{
					bDataError = false;
					break;
				}											
			}
		}
		else
			tempPoint++;

		if(!bDataError)
			break;
	}
	
	if(count>0)
		*ret_data=ADV_DataString2HexData(dataPointtemp,count);
	else
		*ret_data=-1;
	
	return(tempPoint - dataPoint);
}
/**********************************************************************************/
/**********************************************************************************/
static BU16 ADV_Search_XmlSpecial_Tag(BU08 *dataPoint,BU08 Specialtag)
{
	BU08 *tempPoint;

	tempPoint = dataPoint;
	
	while(tempPoint[0] != Specialtag && tempPoint<(xmlFile_BeginDataPoint+xmlFileLength-1))
	{
		tempPoint+=1;
	}
	
	return(tempPoint - dataPoint);

}

/**********************************************************************************
**********************************************************************************/
static BU16 PARSE_XMLOnecell_name(BU08 *startdata,int cellNum)
{
	int j;
	BU08	*dataPoint;
	BU08      bTag=false;
	
	dataPoint=startdata;
	dataPoint+=1;
	
	for(j=0;j<advxml_picname_max;j++)
	{
		if(dataPoint[0] != '"')
		{
			(Advxml_attribute+cellNum)->pic_name[j] = dataPoint[0];
			dataPoint++;
		}
		else
		{
			bTag = true;
			break;
		}											
	}
	if(!bTag)
		memset((Advxml_attribute+cellNum)->pic_name,0,advxml_picname_max);		
	
	return(dataPoint-startdata);
}

/**********************************************************************************
**********************************************************************************/
static BU16 ADV_Parse_XMLOnecell(BU08 *startdata,ADVXML_ELEMENT_TYPE Type,int cellNum)
{
	BU08	*dataPoint;
	BU08	bExit=false;
	int           channel_name=0,file_number=0,position_x=0,position_y=0,width=0,height=0,duration=0;
       BU16     temp;
	ADVXML_ELEMENT_TYPE	Elenent_Type,channel_or_ad;

	channel_or_ad=Type;
	dataPoint = startdata;
	bExit = false;
	dataPoint+=1;
	
	while(!bExit && dataPoint < (xmlFile_BeginDataPoint+xmlFileLength))	
	{
		Elenent_Type = ADV_Parse_XMLElementType(dataPoint);

		switch(Elenent_Type)
		{
			case ELEMENT_TYPE_LOGO_END:
			case ELEMENT_TYPE_AD_END:
				
				dataPoint+=1;
				if(Type!=ELEMENT_TYPE_CHANNEL_START)
					bExit=true;

				break;
				
			case ELEMENT_TYPE_CHANNEL_END:
				  
				dataPoint+=1;
				bExit=true;

				break;
				
			case ELEMENT_TYPE_TEXT_START:
				
			{
				BU16 string_lenth;
				int num;
							
				dataPoint+=6;
				
				string_lenth=ADV_Search_XmlSpecial_Tag(dataPoint,'<');  
				if(string_lenth>advxml_text_max)
					string_lenth = advxml_text_max;
				for(num=0;num<string_lenth;num++)
					(Advxml_attribute+cellNum)->text[num]=dataPoint[num];
				//APP_PRINT_DEBUG("string_lenth=%d, %s\n",string_lenth,Advxml_attribute[cellNum]->text);
				dataPoint+=string_lenth;
			}

				break;
				
			case ELEMENT_TYPE_TEXT_END:
				
				dataPoint+=7;

				break;
				
			case ELEMENT_TYPE_AD_START:
				  
				dataPoint+=1;
				channel_or_ad=ELEMENT_TYPE_AD_START;

				break;
				
			case ELEMENT_TYPE_NAME:
				
				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'"');
				
				if(channel_or_ad==ELEMENT_TYPE_CHANNEL_START)
				{
					dataPoint+=ADV_Search_XmlDecData(dataPoint,'"',&channel_name);
					if((Advxml_attribute+cellNum)->total_channel_number < ADV_MAX_CHANNEL_NUM)
					{
						(Advxml_attribute+cellNum)->channel_name[(Advxml_attribute+cellNum)->total_channel_number]=channel_name;
						(Advxml_attribute+cellNum)->total_channel_number++;
					}
					//printf ("(Advxml_attribute+cellNum)->channel_name %d %d\n", channel_name, (Advxml_attribute+cellNum)->total_channel_number);
				}
					
				else if(channel_or_ad==ELEMENT_TYPE_AD_START||channel_or_ad==ELEMENT_TYPE_LOGO_START)
					dataPoint+=PARSE_XMLOnecell_name(dataPoint,cellNum);
				
				break;
				
			case ELEMENT_TYPE_FILE_NUMBER_START:
								
				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+=ADV_Search_XmlDecData(dataPoint,'<',&file_number);
				(Advxml_attribute+cellNum)->file_number=file_number;
				
				break;
				
			case ELEMENT_TYPE_FILE_NUMBER_END:
				
				dataPoint+=13;
				
				break;
				
			case 	ELEMENT_TYPE_FORMAT_START:
				
				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				while(dataPoint[0]!='<'&& dataPoint < (xmlFile_BeginDataPoint+xmlFileLength))
				{
					if((dataPoint[0]=='g'||dataPoint[0]=='G') && (dataPoint[1]=='i'||dataPoint[1]=='I')\
					&& (dataPoint[2]=='f'||dataPoint[2]=='F'))
					{
						(Advxml_attribute+cellNum)->format=_GIF;
						dataPoint++;
					}
					
					else if((dataPoint[0]=='j'||dataPoint[0]=='J') && (dataPoint[1]=='p'||dataPoint[1]=='P'))
					{
						(Advxml_attribute+cellNum)->format=_JPG;
						dataPoint++;
					}
					
					else if((dataPoint[0]=='p'||dataPoint[0]=='P') && (dataPoint[1]=='n'||dataPoint[1]=='N')\
					&& (dataPoint[2]=='g'||dataPoint[2]=='G'))
					{
						(Advxml_attribute+cellNum)->format=_PNG;
						dataPoint++;
					}
					
					else if((dataPoint[0]=='b'||dataPoint[0]=='B') && (dataPoint[1]=='m'||dataPoint[1]=='M')\
					&& (dataPoint[2]=='p'||dataPoint[2]=='P'))
					{
						(Advxml_attribute+cellNum)->format=_BMP;
						dataPoint++;
					}

					else if((dataPoint[0]=='m'||dataPoint[0]=='M') && (dataPoint[1]=='p'||dataPoint[1]=='P')\
					&& (dataPoint[2]=='g'||dataPoint[2]=='G'))/*zsy:1209*/
					{
						(Advxml_attribute+cellNum)->format=_MPG;
						dataPoint++;
					}				
					else
						dataPoint++;
				}
				
				break;
				
			case ELEMENT_TYPE_FORMAT_END:
				
				dataPoint+=8;
				
				break;
				
			case ELEMENT_TYPE_POSITION_X_START:

				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+=ADV_Search_XmlDecData(dataPoint,'<',&position_x);
				(Advxml_attribute+cellNum)->position_x=position_x;
				
				break;
				
			case ELEMENT_TYPE_POSITION_X_END:
			case ELEMENT_TYPE_POSITION_Y_END:
				
				dataPoint+=12;
			
				break;
				
			case ELEMENT_TYPE_POSITION_Y_START:
				
				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+=ADV_Search_XmlDecData(dataPoint,'<',&position_y);
				(Advxml_attribute+cellNum)->position_y=position_y;
				
				break;
				
			case ELEMENT_TYPE_WIDTH_START:
				
				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+=ADV_Search_XmlDecData(dataPoint,'<',&width);
				(Advxml_attribute+cellNum)->width=width;
				
				break;

			case ELEMENT_TYPE_HEIGHT_START:
								
				dataPoint+= ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+= ADV_Search_XmlDecData(dataPoint,'<',&height);
				(Advxml_attribute+cellNum)->height=height;
				
				break;
				
			case ELEMENT_TYPE_DURATION_START:
								
				dataPoint+= ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+= ADV_Search_XmlDecData(dataPoint,'<',&duration);
				(Advxml_attribute+cellNum)->duration=duration;
				
				break;
				
			case ELEMENT_TYPE_WIDTH_END:
			case ELEMENT_TYPE_HEIGHT_END:
			case ELEMENT_TYPE_DURATION_END:
				
				dataPoint+=6;
				
				break;
																
			default:
				
				dataPoint+=1;

				break;
		}

	}
	
	return(dataPoint-startdata);

}

/**********************************************************************************
**********************************************************************************/
static BU16 ADV_GetXmlFileNumber(BU08 *startdata)
{
	BU08	*dataPoint;
	INT     file_number = 0;
	ADVXML_ELEMENT_TYPE	Elenent_Type;
	BU08 bExit = FALSE;

	dataPoint = startdata;
	dataPoint+=1;
	
	while ((bExit == FALSE) && (dataPoint < (xmlFile_BeginDataPoint+xmlFileLength)))
	{
		Elenent_Type = ADV_Parse_XMLElementType(dataPoint);

		switch(Elenent_Type)
		{
			case ELEMENT_TYPE_FILE_NUMBER_START:
				dataPoint+=ADV_Search_XmlSpecial_Tag(dataPoint,'>');
				dataPoint+=ADV_Search_XmlDecData(dataPoint,'<',&file_number);
				break;

			case ELEMENT_TYPE_FILE_NUMBER_END:
				bExit = TRUE;
				break;

			default:
				dataPoint+=1;
				break;
		}
	}
	return file_number;
}

/**********************************************************************************
**********************************************************************************/
static BU16 ADV_Parse_XMLOneRow(BU08 *startdata,ADVXML_ELEMENT_TYPE Type)
{
	BU08	*dataPoint;
	BU08	bExit=false;
	ADVXML_ELEMENT_TYPE	Elenent_Type;
	BU16 XmlFileNumber;
	BS32 PicNo;

	dataPoint = startdata;
	bExit = false;
	dataPoint +=1;
	
	while(!bExit && dataPoint < (xmlFile_BeginDataPoint+xmlFileLength))	
	{
		Elenent_Type = ADV_Parse_XMLElementType(dataPoint);
		XmlFileNumber = ADV_GetXmlFileNumber(dataPoint);
		PicNo = ADV_SearchPicCacheIndex(XmlFileNumber);

		switch(Elenent_Type)
		{
			case ELEMENT_TYPE_BOOTLOGO_END:
			case ELEMENT_TYPE_MAINMENU_END:
			case ELEMENT_TYPE_STRIPE_END:
				
				bExit = true;
				
				break;
				
			case ELEMENT_TYPE_LOGO_START:
			case ELEMENT_TYPE_AD_START:
			case ELEMENT_TYPE_CHANNEL_START:

				if(Type==ELEMENT_TYPE_BOOTLOGO_START)
					(Advxml_attribute+PicNo)->nametype=Bootlogo;
				else if(Type==ELEMENT_TYPE_MAINMENU_START)
					(Advxml_attribute+PicNo)->nametype=MainMenu;
				else if(Type==ELEMENT_TYPE_STRIPE_START)
					(Advxml_attribute+PicNo)->nametype=Stripe;

				dataPoint+=ADV_Parse_XMLOnecell(dataPoint,Elenent_Type,PicNo);

				break;
				
			case ELEMENT_TYPE_CURRENT_FOLLOW_START:
			case ELEMENT_TYPE_CURRENT_FOLLOW_END:
				
				dataPoint+=14;
				
				break;
								
			default:
				
				dataPoint+=1;

				break;
		}
	}
	
	return(dataPoint-startdata);

}

/**********************************************************************************
**********************************************************************************/
static BU16 ADV_Parse_XMLRoot(BU08 *root_startdata)
{
	BU08	*dataPoint;
	BU08	bExit=false;
	ADVXML_ELEMENT_TYPE	Elenent_Type;

	dataPoint = root_startdata;
	bExit = false;

	
	while(!bExit && dataPoint < (xmlFile_BeginDataPoint+xmlFileLength))	
	{
		Elenent_Type = ADV_Parse_XMLElementType(dataPoint);

		switch(Elenent_Type)
		{
			case ELEMENT_TYPE_BOOTLOGO_START:
			case ELEMENT_TYPE_MAINMENU_START:
			case ELEMENT_TYPE_STRIPE_START:
				
				dataPoint+=ADV_Parse_XMLOneRow(dataPoint,Elenent_Type);
				
				break;

			default:
				
				dataPoint+=1;

				break;
		}
	}

	return(dataPoint-root_startdata);
}

/**********************************************************************************
**********************************************************************************/
static BU08  ADV_Parse_Advxml(BU08 *pFile, BU16 Filelength)
{
	BU08					*dataPoint;
	BU16					temp;
	BU08					bExit=false;
	ADVXML_ELEMENT_TYPE		Elenent_Type;
	
	xmlFile_BeginDataPoint		= pFile;
	xmlFileLength				= Filelength;
	
	bExit = false;
	dataPoint = xmlFile_BeginDataPoint;
	
	while(!bExit && dataPoint < (xmlFile_BeginDataPoint+xmlFileLength))	
	{
		Elenent_Type = ADV_Parse_XMLElementType(dataPoint);
		
		switch(Elenent_Type)
		{
			case ELEMENT_TYPE_ROOT_END:

				bExit = true;
				
				break;

			case ELEMENT_TYPE_ROOT_START:

				temp  = ADV_Parse_XMLRoot(dataPoint);
				
				break;


			default:
				
				temp = 1;

				break;
				
		}

		dataPoint+=temp;

	}
}

#define Parse_table

static BU32 AdvChannelID = 0;
static BU32 AdvFilterID = 0;
static BU08 isStopParserAdv = 0;
static BU32 queueParserAdvEnd = 0;

void APP_ADV_IPI_StopDemuxADV(void)
{
	isStopParserAdv = 1;
	APP_Parser_IPI_StopDemux(&AdvChannelID, &AdvFilterID);
}

/**********************************************************************************
**********************************************************************************/ 
static void ADV_ParsePrivateDataByte1(BU08 *pucSectionData)
{
	unsigned int		frequence=0;
	BU08				modulation;
	unsigned int		symbol_rate;
	BU08				AD_Headdata_table_id;
	BU08				AD_Picdata_table_id;
	BU08				byte1=0,byte2=0,byte3=0,byte4=0;

	byte1 = pucSectionData[1]/16;
	byte2 = pucSectionData[1]%16;
	byte3 = pucSectionData[2]/16;
	byte4 = pucSectionData[2]%16;
	frequence = byte1*1000+byte2*100+byte3*10+byte4; /*暂时忽略小数点后*/

	modulation = pucSectionData[5];
	
	byte1 = pucSectionData[6]/16;
	byte2 = pucSectionData[6]%16;
	byte3 = pucSectionData[7]/16;
	symbol_rate = byte1*100+byte2*10+byte3; /*暂时忽略小数点后*/

	AD_Headdata_table_id = pucSectionData[10];
	AD_Picdata_table_id = pucSectionData[11];
	ADInfoStruct->AD_Headdata_table_id = AD_Headdata_table_id;
	ADInfoStruct->AD_Picdata_table_id = AD_Picdata_table_id;
	//APP_PRINT_DEBUG("AD_Headdata_table_id = %x \n",AD_Headdata_table_id);
	//APP_PRINT_DEBUG("AD_Picdata_table_id = %x \n",AD_Picdata_table_id);
}

/**********************************************************************************
						分析BAT  表的描述子
**********************************************************************************/ 
static int	ADV_ParseBATDescriptor(BU08 *pucSectionData)
{
	int	iDescriptorLength;
	BU16	transport_stream_id;
	BU16	original_network_id;
	BU16	service_id;
	BU08	linkage_type;
	int i = 0;
	BU32 WeatherSize;
	
	iDescriptorLength = pucSectionData [ 1 ];

	switch ( pucSectionData [ 0 ] )
	{
		case 0xF0:  /* private_data_specifier_descriptor*/

			pucSectionData += 2;
			if(pucSectionData[0]!='a' ||pucSectionData[1]!='d' ||pucSectionData[2]!='v' 
				||pucSectionData[3]!='e' ||pucSectionData[4]!='r' ||pucSectionData[5]!='t' 
				||pucSectionData[6]!='i' ||pucSectionData[7]!='s' ||pucSectionData[8]!='e' 
				||pucSectionData[9]!='m' ||pucSectionData[10]!='e' ||pucSectionData[11]!='n' 
				||pucSectionData[12]!='t' )
			{
				/*do something~~*/
				APP_PRINT_DEBUG("private_data_specifier_descriptor not equal to advertisement ! \n");
			}
			
			break;

		case 0x59:  /* private_data_specifier_descriptor*/
			
			pucSectionData += 2;
			memcpy(cRolltext, pucSectionData, iDescriptorLength);
			APP_PRINT_DEBUG("Rolltext = %s \n",cRolltext);
			APP_PRINT_DEBUG("iDescriptorLength = %d \n",iDescriptorLength);

			break;

		case 0x57:  /* private_data_specifier_descriptor*/
			
			pucSectionData += 2;
			WeatherSize = (pucSectionData[0]<<24)|(pucSectionData[1]<<16)|(pucSectionData[2]<<8)|pucSectionData[3];
			APP_PRINT_DEBUG("WeatherSize = %d \n",WeatherSize);
			APP_PRINT_DEBUG("iDescriptorLength = %d \n",iDescriptorLength);

			break;
			
		case 0x4A:	/* linkage_descriptor*/
			
			pucSectionData += 2;
			
			transport_stream_id	= (pucSectionData[0]<<8)|pucSectionData[1];
			original_network_id	= (pucSectionData[2]<<8)|pucSectionData[3];
			service_id		 	= (pucSectionData[4]<<8)|pucSectionData[5];
			APP_PRINT_DEBUG("service_id = %x \n",service_id);
			ADInfoStruct->ServiceId = service_id;
			
			linkage_type			= pucSectionData[6];
			if(linkage_type==0xA1)
				ADV_ParsePrivateDataByte1(pucSectionData+7);
			
			break;
			
		default:
			break;
	}

	return ( iDescriptorLength + 2 );
}


/**********************************************************************************
				Parse  the BAT table
**********************************************************************************/ 
 void	ADV_ParseBAT (BU08* pSectionData)
 {
	BU16 				bouquet_id=0;
	BU08    			ucLastSectionNumber 	= 0;
	BU08			ucSectionNumber 		= 0;
	BU08			ucVersionNumber 		= 0;
	BU08			ucCurNextIndicator 		= 0;
	BU32			ucOldVersionNumber		= 0;
	BU16 				bouquet_descriptor_length = 0;
	int				iNoOfBytesParsedSoFar=0;
	int				iNoOfDataBytes=0;
	BU16				transport_stream_loop_length=0;
	BU16				transport_stream_id=0;
	BU16				original_network_id=0;
	BU16				transport_descriptors_length=0;
	BU32	flash_destroy_flag = 0;
	BU08 DownCtrlCoef[3];
	BU08 DownCtrlMask[3] = {0xFF, 0xFF, 0xFF};
	BU08 DownCtrlExcl[3] = {0x00, 0x00, 0x00};
	struct ParserFuncNode_s parserFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;

	//APP_PRINT_DEBUG("----Get BAT----\n");
	
	if(pSectionData == NULL)
		return;
	if(pSectionData[0] != 0x4a/*BAT_TABLE_ID*/)
		return;
	
	bouquet_id = (pSectionData[3]<<8)|pSectionData[4];
	if(bouquet_id != 0xff00)
	{
		APP_PRINT_DEBUG("bouquet_id != 0xff00 in ADV_ParseBAT! \n");
		return;
	}
		
	ucVersionNumber 		= (pSectionData[5] & 0x3E) >>1;
	ucCurNextIndicator 	= pSectionData[5] & 0x01;
	ucSectionNumber 		= pSectionData[6];
	ucLastSectionNumber 	= pSectionData[7];
	APP_SYS_API_GetSysInfo(APP_SYS_AdvVersion_e, &ucOldVersionNumber);
	//ucOldVersionNumber = (BU08)TPSYSTEM_GetLogoVerNumValue();
	AdvmentVersion = ucVersionNumber;/*zsy:0520*/
	
	bouquet_descriptor_length = ((pSectionData[8] & 0xF)<<8)|pSectionData[9];

	pSectionData	= pSectionData + 10; /*point to the first descriptor loop*/
	
	if ( bouquet_descriptor_length )
	{
		iNoOfBytesParsedSoFar = 0;
		while ( iNoOfBytesParsedSoFar < bouquet_descriptor_length )
		{
			iNoOfDataBytes = ADV_ParseBATDescriptor(pSectionData);
			pSectionData += iNoOfDataBytes;
			iNoOfBytesParsedSoFar += iNoOfDataBytes;
			APP_PRINT_DEBUG("iNoOfBytesParsedSoFar = %d ,bouquet_descriptor_length = %d \n",iNoOfBytesParsedSoFar,bouquet_descriptor_length);
			
		}
	}

	transport_stream_loop_length = ((pSectionData[0] & 0xF)<<8)|pSectionData[1];
	
	pSectionData += 2;/*point to second descriptor loop*/

	while ( transport_stream_loop_length > 0 )
	{		
		transport_stream_id = (pSectionData[0]<<8)|pSectionData[1];
		original_network_id = (pSectionData[2]<<8)|pSectionData[3];

		transport_descriptors_length = ((pSectionData[4]& 0x0F) << 8) | pSectionData[5];
		pSectionData += 6;

		if (transport_descriptors_length)
		{
			iNoOfBytesParsedSoFar = 0;
			
			while (iNoOfBytesParsedSoFar < transport_descriptors_length)
			{
				iNoOfDataBytes = ADV_ParseBATDescriptor(pSectionData);
				pSectionData += iNoOfDataBytes;
				iNoOfBytesParsedSoFar += iNoOfDataBytes;
			}
		}
		transport_stream_loop_length = transport_stream_loop_length - transport_descriptors_length - 6;
	}

	APP_SYS_API_GetSysInfo(APP_SYS_AdvSaveFlag_e, &flash_destroy_flag);
#if 1
	if ((ucOldVersionNumber == ucVersionNumber && flash_destroy_flag == 0) /*&& (TPSYSTEM_GetAdvFlashCorruptedFlag() == FALSE)*/)
	{	
		APP_PRINT_DEBUG("New bat version[%d] = old bat version[%d],no need to update adv data\n", 
			ucVersionNumber, ucOldVersionNumber);
		ADV_SetSkipFilter_OK(true);
		msgRecvTemp.qlstWordOfMsg = 2;
		FYF_API_queue_send(queueParserAdvEnd, &msgRecvTemp);
		return;
	}
	else 
#endif
	{
		ADV_FlashUpgradeProcess(0);
		//ADV_FlashUpgradeProcess(0);
		APP_PRINT_DEBUG("New bat version[%d] != old bat version[%d] or flash corrupted,need to update adv data\n", 
			ucVersionNumber, ucOldVersionNumber);
	}

	ADInfoStruct->BAT_OK = true;

	/*req for DownCtrlInfo*/
	APP_ADV_IPI_StopDemuxADV();
	DownCtrlCoef[0] = ADInfoStruct->AD_Headdata_table_id;
	DownCtrlCoef[1] = (ADInfoStruct->Pic_info_table_extension_id_start >> 8) & 0xff;
	DownCtrlCoef[2] = (ADInfoStruct->Pic_info_table_extension_id_start) & 0xff;
	
	APP_Parser_IPI_StartDemux(3, ADV_DATA_PID, DownCtrlCoef, DownCtrlMask, DownCtrlExcl, &AdvChannelID, &AdvFilterID);
	parserFuncNode.channelId = AdvChannelID;
	parserFuncNode.filterId = AdvFilterID;
	parserFuncNode.ParserFunc = ADV_CacheData;
	APP_IPI_AddPasFuncNode(parserFuncNode);
	msgRecvTemp.qlstWordOfMsg = 1;
	FYF_API_queue_send(queueParserAdvEnd, &msgRecvTemp);

	APP_PRINT_DEBUG("----ADV_ParseBAT OK!  and req DownCtrlInfo----\n");

}

/**********************************************************************************

/**********************************************************************************
**********************************************************************************/ 
void	ADV_ParseDownCtrlInfo (BU08* data_p)
{
	BU08 		Table_id;
	BU16		Table_extension_id;
	BU08 		Section_number;
	BU08 		Last_section_number;
	BU16		Index_xml_table_extension_id_start;
	BU16	 	Index_xml_table_extension_id_end;
	BU32 		Index_xml_size;
	BU16	 	Pic_info_table_extension_id_start;
	BU16	 	Pic_info_table_extension_id_end;
	BU32 		Pic_info_size;
	int 		SizeTemp=0;
	BU08 PicInfoCoef[3];
	BU08 PicInfoMask[3] = {0xFF, 0xFF, 0xFF};
	BU08 PicInfoExcl[3] = {0x00, 0x00, 0x00};
	struct ParserFuncNode_s parserFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	
	//APP_PRINT_DEBUG("----Get DownCtrlInfo----\n");
	
	Table_extension_id 	= (data_p[3]<<8)|data_p[4];
	Section_number 		= data_p[6];
	Last_section_number 	= data_p[7];
	Index_xml_table_extension_id_start 	= (data_p[8]<<8)|data_p[9];
	Index_xml_table_extension_id_end 	= (data_p[10]<<8)|data_p[11];
	Index_xml_size	= (data_p[12]<<24)|(data_p[13]<<16)|(data_p[14]<<8)|data_p[15];
	Pic_info_table_extension_id_start 	= (data_p[16]<<8)|data_p[17];
	Pic_info_table_extension_id_end 	= (data_p[18]<<8)|data_p[19];
	Pic_info_size	= (data_p[20]<<24)|(data_p[21]<<16)|(data_p[22]<<8)|data_p[23];
	
	ADInfoStruct->Index_xml_table_extension_id_start = Index_xml_table_extension_id_start;
	ADInfoStruct->Index_xml_table_extension_id_end = Index_xml_table_extension_id_end;
	ADInfoStruct->Index_xml_size = Index_xml_size;
	ADInfoStruct->Pic_info_table_extension_id_start = Pic_info_table_extension_id_start;
	ADInfoStruct->Pic_info_table_extension_id_end = Pic_info_table_extension_id_end;
	ADInfoStruct->Pic_info_size = Pic_info_size;

	if(Index_xml_table_extension_id_start!=Index_xml_table_extension_id_end 
		|| Pic_info_table_extension_id_start!=Pic_info_table_extension_id_end)
	{	
		APP_PRINT_DEBUG("Index_xml_table_extension_id has more than one!!\n");
		APP_PRINT_DEBUG("Pic_info_table_extension_id_start has more than one!!\n");
		ADV_SetSkipFilter_OK(true);
		return;
	}

	/**********malloc men for PicInfoDataTemp and IndexDataTemp******************/
	if(ADInfoStruct->PicInfoDataTemp == NULL)
	{
		SizeTemp = ((Pic_info_size/ADV_SECTION_LENGTH)+1)*ADV_SECTION_LENGTH;
		ADInfoStruct->PicInfoDataTemp = (BU08*) malloc(SizeTemp);	
		APP_PRINT_DEBUG(" malloc ADInfoStruct->PicInfoDataTemp size=%d\n",SizeTemp);
	}
	if(ADInfoStruct->IndexDataTemp == NULL)
	{
		SizeTemp = ((Index_xml_size/ADV_SECTION_LENGTH)+1)*ADV_SECTION_LENGTH;
		ADInfoStruct->IndexDataTemp = (BU08*) malloc(SizeTemp);
		APP_PRINT_DEBUG(" malloc ADInfoStruct->IndexDataTemp size=%d\n",SizeTemp);
	}
	if(ADInfoStruct->PicInfoDataTemp == NULL || ADInfoStruct->IndexDataTemp == NULL)
	{	
		ADV_SetSkipFilter_OK(true);
		return;
	}
	/************************************************************************/
	
	ADInfoStruct->DownCtrlInfo_OK = true;

	/*req for PicInfo*/
	APP_ADV_IPI_StopDemuxADV();
	ADV_DefaultSectionNumTag();
	isStopParserAdv = 0;
	PicInfoCoef[0] = ADInfoStruct->AD_Headdata_table_id;
	PicInfoCoef[1] = (ADInfoStruct->Pic_info_table_extension_id_start >> 8) & 0xff;
	PicInfoCoef[2] = (ADInfoStruct->Pic_info_table_extension_id_start) & 0xff;
	
	APP_Parser_IPI_StartDemux(3, ADV_DATA_PID, PicInfoCoef, PicInfoMask, PicInfoExcl, &AdvChannelID, &AdvFilterID);
	parserFuncNode.channelId = AdvChannelID;
	parserFuncNode.filterId = AdvFilterID;
	parserFuncNode.ParserFunc = ADV_CacheData;
	APP_IPI_AddPasFuncNode(parserFuncNode);
	



	APP_PRINT_DEBUG("----ADV_ParseDownCtrlInfo OK!  and req PicInfo----\n");

	APP_PRINT_DEBUG("Index_xml_table_extension_id_start = %d\n",Index_xml_table_extension_id_start);
	APP_PRINT_DEBUG("Index_xml_table_extension_id_end = %d\n",Index_xml_table_extension_id_end);
	APP_PRINT_DEBUG("Index_xml_size = %d\n",Index_xml_size);
	APP_PRINT_DEBUG("Pic_info_table_extension_id_start = %d\n",Pic_info_table_extension_id_start);
	APP_PRINT_DEBUG("Pic_info_table_extension_id_end = %d\n",Pic_info_table_extension_id_end);
	APP_PRINT_DEBUG("Pic_info_size = %d\n",Pic_info_size);
/*
	ADV_SetTotalPicNum(2);
	ADV_MallocXMLStruct();
	(Advxml_attribute+0)->PIC_ID = 1;
	(Advxml_attribute+1)->PIC_ID = 3;
	*/
	
}


/**********************************************************************************
**********************************************************************************/ 
static BOOLEAN	ADV_ExtractPicInfoStruct(BU08 *pData, int DataLength)
{
	BU16 	Pic_num=0;
	int 	i;

	Pic_num = (pData[0]<<8)|pData[1];
	//APP_PRINT_DEBUG("Pic_num=%d\n",Pic_num);
	ADV_SetTotalPicNum(Pic_num);

	if(ADV_MallocXMLStruct())
		return true;
	
	pData+=2; /*point to first loop struct*/	
	for(i=0;i<ADV_GetTotalPicNum();i++)
	{
		(Advxml_attribute+i)->PIC_ID = (pData[0]<<8)|pData[1];
		(Advxml_attribute+i)->Pic_original_size = (pData[2]<<24)|(pData[3]<<16)|(pData[4]<<8)|pData[5];
		(Advxml_attribute+i)->Pic_segment_length = (pData[10]<<8)|pData[11];
		(Advxml_attribute+i)->Pic_segment_num = (pData[12]<<8)|pData[13];
		(Advxml_attribute+i)->Pic_compressed_flag = (pData[13]&0x80)>>7;
		(Advxml_attribute+i)->Pic_compressed_size = (pData[15]<<24)|(pData[16]<<16)|(pData[17]<<8)|pData[18];
		APP_PRINT_DEBUG("i = %d, picid = %d, original_size = %d, segment_length = %d,\
			segment_num = %d, compressed_flag = %d, compressed_size = %d\n", i, (Advxml_attribute+i)->PIC_ID,\
			(Advxml_attribute+i)->Pic_original_size, (Advxml_attribute+i)->Pic_segment_length, (Advxml_attribute+i)->Pic_segment_num,\
					(Advxml_attribute+i)->Pic_compressed_flag, 	(Advxml_attribute+i)->Pic_compressed_size)
		pData+=19;
	}

	return false;
}



static BU08 ADV_MallocPicInfoStruct()
{
	if(ADInfoStruct == NULL)
	{
		ADInfoStruct = (ADV_PICINFO_STRUCT*)FYF_API_malloc(sizeof(ADV_PICINFO_STRUCT));
		APP_PRINT_DEBUG("malloc ADInfoStruct=0x%x, size=%d\n",ADInfoStruct,(sizeof(ADV_PICINFO_STRUCT)));
	}
	if(ADInfoStruct == NULL)
	{
		APP_PRINT_ERROR("no enough memory for ADInfoStruct\n");
		return true;
	}

	ADInfoStruct->ServiceId = 0xffff;
	ADInfoStruct->AD_Headdata_table_id = 0xff;
	ADInfoStruct->AD_Picdata_table_id = 0xff;
	ADInfoStruct->AD_PID = INVALID_PID;
	ADInfoStruct->Index_xml_table_extension_id_start = 0;
	ADInfoStruct->Index_xml_table_extension_id_end= 0;
	ADInfoStruct->Index_xml_size = 0;
	ADInfoStruct->Pic_info_table_extension_id_end = 0;
	ADInfoStruct->Pic_info_table_extension_id_start = 0;
	ADInfoStruct->Pic_info_size = 0;

	ADInfoStruct->BAT_OK = false;
	ADInfoStruct->PAT_OK = false;
	ADInfoStruct->PMT_OK = false;
	ADInfoStruct->DownCtrlInfo_OK = false;
	ADInfoStruct->PicInfo_OK = false;
	ADInfoStruct->Index_OK = false;
	ADInfoStruct->PicData_OK = false;
	
	ADInfoStruct->IndexDataTemp = NULL;
	ADInfoStruct->PicInfoDataTemp = NULL;

	return false;
	
}

void ADV_FreePicInfoStruct()
{
	APP_PRINT_DEBUG("Ready to free ADInfoStruct......\n");
	
	if(ADInfoStruct != NULL)
	{
		if(ADInfoStruct->PicInfoDataTemp != NULL)
		{
			free(ADInfoStruct->PicInfoDataTemp);
			ADInfoStruct->PicInfoDataTemp = NULL;
		}
		if(ADInfoStruct->IndexDataTemp != NULL)
		{
			free(ADInfoStruct->IndexDataTemp);
			ADInfoStruct->IndexDataTemp = NULL;
		}
			
		free(ADInfoStruct);
		ADInfoStruct = NULL;
		
		APP_PRINT_DEBUG("free ADInfoStruct\n");
	}
}


/**********************************************************************************
**********************************************************************************/ 
void	ADV_ParsePicInfo(BU08* data_p)
{
	BU08* 	data;   
	int 		loop=0; 
	BU16		SecLength=0;
	int		i,j;
	int 		hasRangeLength=0;
	int SizeTemp = ((ADInfoStruct->Pic_info_size/ADV_SECTION_LENGTH)+1)*ADV_SECTION_LENGTH;

	BU08 XmlInfoCoef[3];
	BU08 XmlInfoMask[3] = {0xFF, 0xFF, 0xFF};
	BU08 XmlInfoExcl[3] = {0x00, 0x00, 0x00};

	struct ParserFuncNode_s parserFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	

	//APP_PRINT_DEBUG("----Get PicInfo----\n");
	
	/*暂时处理同一个tableExtenId 的数据*/
	
	data = ADInfoStruct->PicInfoDataTemp; /*内部数据已经按照section_number 存放好*/
	loop = (ADInfoStruct->Pic_info_size)/ADV_SECTION_LENGTH+1;
	
	for(i=0;i<loop;i++)
	{
		SecLength = ((data[i*ADV_SECTION_LENGTH+9]&0xf)<<8)|data[i*ADV_SECTION_LENGTH+10];
		
		if((hasRangeLength+SecLength)<SizeTemp)
			memcpy(data+hasRangeLength,data+i*ADV_SECTION_LENGTH+11,SecLength);
			
		hasRangeLength += SecLength;
	}
	
	if(ADV_ExtractPicInfoStruct(data,hasRangeLength))
	{
		ADV_SetSkipFilter_OK(true);
		return;
	}

	if(ADInfoStruct->PicInfoDataTemp != NULL)
	{
		free(ADInfoStruct->PicInfoDataTemp);
		ADInfoStruct->PicInfoDataTemp = NULL;
		APP_PRINT_DEBUG("----free ADInfoStruct->PicInfoDataTemp\n");
	}

	/*req for index.xml*/
	
	ADInfoStruct->PicInfo_OK = true;
	ADV_DefaultSectionNumTag();

	isStopParserAdv = 0;
	XmlInfoCoef[0] = ADInfoStruct->AD_Headdata_table_id;
	XmlInfoCoef[1] = (ADInfoStruct->Index_xml_table_extension_id_start >> 8) & 0xff;
	XmlInfoCoef[2] = (ADInfoStruct->Index_xml_table_extension_id_start) & 0xff;
	
	APP_Parser_IPI_StartDemux(3, ADV_DATA_PID, XmlInfoCoef, XmlInfoMask, XmlInfoExcl, &AdvChannelID, &AdvFilterID);
	parserFuncNode.channelId = AdvChannelID;
	parserFuncNode.filterId = AdvFilterID;
	parserFuncNode.ParserFunc = ADV_CacheData;
	APP_IPI_AddPasFuncNode(parserFuncNode);
	



	APP_PRINT_DEBUG("----ADV_ParsePicInfo OK!  and req index----\n");

}

//#define GetIndexXmlFile 1
#ifdef GetIndexXmlFile
BU08    aaaa[300*1024];
long int       dataFile;                
int            numBytes ;
#endif

/**********************************************************************************
**********************************************************************************/ 
void	ADV_ParseIndex(BU08* data_p)
{
	BU08* 	data;   
	int 		loop=0; 
	BU16		SecLength=0;
	int		i;
	int 		hasRangeLength=0;
	int SizeTemp = ((ADInfoStruct->Index_xml_size/ADV_SECTION_LENGTH)+1)*ADV_SECTION_LENGTH;
	
	FYF_QUEUE_MESSAGE msgRecvTemp;
	BU08 PicDataCoef[3];
	BU08 PicDataMask[3] = {0xFF, 0xFF, 0xFF};
	BU08 PicDataExcl[3] = {0x00, 0x00, 0x00};
	struct ParserFuncNode_s parserFuncNode;

	//APP_PRINT_DEBUG("----Get Index----\n");
	
	/*暂时处理同一个tableExtenId 的数据*/

	data = ADInfoStruct->IndexDataTemp; /*内部数据已经按照section_number 存放好*/
	loop = (ADInfoStruct->Index_xml_size)/ADV_SECTION_LENGTH+1;
	APP_PRINT_DEBUG("===========loop = %d========================\n", loop);
	for(i=0;i<loop;i++)
	{
		SecLength = ((data[i*ADV_SECTION_LENGTH+9]&0xf)<<8)|data[i*ADV_SECTION_LENGTH+10];
		APP_PRINT_DEBUG("===========SecLength = %d========================\n", SecLength);
		if((hasRangeLength+SecLength)<SizeTemp)
			memcpy(data+hasRangeLength,data+i*ADV_SECTION_LENGTH+11,SecLength);
			
		hasRangeLength += SecLength;
	}

#ifdef GetIndexXmlFile	
	memcpy(aaaa,data,hasRangeLength);
	dataFile=debugopen("index.xml", "wb+");
	numBytes =(int)debugwrite((long int)dataFile,aaaa,hasRangeLength);     
       debugclose(dataFile);
#endif

	APP_PRINT_DEBUG("===========hasRangeLength = %d========================\n", hasRangeLength);
	ADV_Parse_Advxml(data,hasRangeLength);

	APP_PRINT_DEBUG("===========Advxml_attribute info========================\n");
	for(i=0;i<ADV_GetTotalPicNum();i++)
	{
		APP_PRINT_DEBUG("PIC_ID=%d,Channel_name = %d,SegmLength=%d,SegmNum=%d,size=%d,x=%d,y=%d,width=%d,height=%d,duration=%d,format=%d,nametype=%d\n",
			(Advxml_attribute+i)->PIC_ID,
			(Advxml_attribute+i)->channel_name[0],
			(Advxml_attribute+i)->Pic_segment_length,
			(Advxml_attribute+i)->Pic_segment_num,
			(Advxml_attribute+i)->Pic_original_size,
			(Advxml_attribute+i)->position_x,
			(Advxml_attribute+i)->position_y,
			(Advxml_attribute+i)->width,
			(Advxml_attribute+i)->height,
			(Advxml_attribute+i)->duration,
			(Advxml_attribute+i)->format,
			(Advxml_attribute+i)->nametype);

		/*if ((Advxml_attribute+i)->Pic_original_size == 2430)
		{
			(Advxml_attribute+i)->nametype = 2;
			(Advxml_attribute+i)->position_x = 60;
			(Advxml_attribute+i)->position_y = 452;
			(Advxml_attribute+i)->width = 94;
			(Advxml_attribute+i)->height = 58;
		}
		else if ((Advxml_attribute+i)->Pic_original_size == 66859)
		{
			(Advxml_attribute+i)->nametype = 1;
			(Advxml_attribute+i)->position_x = 35;
			(Advxml_attribute+i)->position_y = 105;
			(Advxml_attribute+i)->width = 375;
			(Advxml_attribute+i)->height = 364;		
		}*/
	}
	APP_PRINT_DEBUG("=======================================================\n");
	
	if(ADInfoStruct->IndexDataTemp != NULL)
	{
		free(ADInfoStruct->IndexDataTemp);
		ADInfoStruct->IndexDataTemp = NULL;
		APP_PRINT_DEBUG("----free ADInfoStruct->IndexDataTemp\n");
	}

	ADInfoStruct->Index_OK = true;
	ADV_DefaultSectionNumTag();
	
	if(ADV_MallocPicDataTempStruct())
	{	
		ADV_SetSkipFilter_OK(true);
		return;
	}

	PicDataCoef[0] = ADInfoStruct->AD_Picdata_table_id;
	PicDataCoef[1] = (((Advxml_attribute+0)->PIC_ID) >> 8) & 0xff;
	PicDataCoef[2] = (((Advxml_attribute+0)->PIC_ID)) & 0xff;
	ADV_Filter_Getmask(&(PicDataMask[1]), &(PicDataMask[2]));
	
	/*req for pic data*/
	APP_Parser_IPI_StartDemux(3, ADV_DATA_PID, PicDataCoef, PicDataMask, PicDataExcl, &AdvChannelID, &AdvFilterID);
	parserFuncNode.channelId = AdvChannelID;
	parserFuncNode.filterId = AdvFilterID;
	parserFuncNode.ParserFunc = ADV_CacheData;
	APP_IPI_AddPasFuncNode(parserFuncNode);


	msgRecvTemp.qlstWordOfMsg = 1;
	FYF_API_queue_send(queueParserAdvEnd, &msgRecvTemp);

	APP_PRINT_DEBUG("----ADV_ParseIndex OK!  and req Pic data----\n");

}

/**********************************************************************************
**********************************************************************************/ 
BOOLEAN	ADV_ParsePicData(void)
{
	BU08* 		data_p;
	int 		i;
	int 		length=0;
	int 		hasRangeLength = 0;
	int 		CurrPicTemp;
	int			temp=0;

	if(ADV_MallocPicDataPoint())
	{
		ADV_FreePicInfoStruct();
		ADV_FreePicDataTempStruct();
		return true;
	}

	APP_PRINT_DEBUG("================Parse Picture Data========================\n");
	for(CurrPicTemp=0;CurrPicTemp<ADV_GetTotalPicNum();CurrPicTemp++)
	{
		data_p = (PicDataTemp+CurrPicTemp)->buf; /*内部数据已经按照section_number 存放好*/
		hasRangeLength = 0;
		
		for(i=0;i<(Advxml_attribute+CurrPicTemp)->Pic_segment_num;i++)
		{
			length =  ((data_p[9+i*ADV_SECTION_LENGTH]&0xf)<<8)|data_p[10+i*ADV_SECTION_LENGTH] ;
			/*APP_PRINT_DEBUG(	"--parse length = %d,secNum=%d, tabId=0x%x extTd=%d\n",
						length,data_p[6+i*ADV_SECTION_LENGTH],
						data_p[0+i*ADV_SECTION_LENGTH],
						((data_p[3+i*ADV_SECTION_LENGTH]&0xf)<<8)|data_p[4+i*ADV_SECTION_LENGTH]);
			*/
			memcpy(data_p+hasRangeLength,data_p+i*ADV_SECTION_LENGTH+8+3,length);
				
			hasRangeLength += length;
			
		}
		APP_PRINT_DEBUG("parse pic %d hasRangeLength = %d\n",CurrPicTemp,hasRangeLength);
		
		if(hasRangeLength != (Advxml_attribute+CurrPicTemp)->Pic_original_size)
			hasRangeLength = (Advxml_attribute+CurrPicTemp)->Pic_original_size;

		/*#ifdef GetIndexXmlFile
		memcpy(aaaa,data_p,hasRangeLength);
		dataFile=debugopen("pic.gif", "wb+");
		numBytes =(int)debugwrite((long int)dataFile,aaaa,hasRangeLength);     
	    debugclose(dataFile);
		#endif*/

	}
	APP_PRINT_DEBUG("============================================================\n");
	
	for(CurrPicTemp=0;CurrPicTemp<ADV_GetTotalPicNum();CurrPicTemp++)
	{
		memcpy(	AdvPicDataPoint+temp,
				(PicDataTemp+CurrPicTemp)->buf,
				(Advxml_attribute+CurrPicTemp)->Pic_original_size);
		
		(Advxml_attribute+CurrPicTemp)->PicData_p = AdvPicDataPoint+temp;

#ifdef GetIndexXmlFile
		memcpy(aaaa,(Advxml_attribute+CurrPicTemp)->PicData_p,(Advxml_attribute+CurrPicTemp)->Pic_original_size);
		switch (CurrPicTemp)
		{
			case 0:
				dataFile=debugopen("pic0.gif", "wb+");
				break;
				
			case 1:
				dataFile=debugopen("pic1.gif", "wb+");
				break;
				
			case 2:
				dataFile=debugopen("pic2.gif", "wb+");
				break;
			
			case 3:
				dataFile=debugopen("pic3.gif", "wb+");
				break;
		}

		numBytes =(int)debugwrite((long int)dataFile,aaaa,(Advxml_attribute+CurrPicTemp)->Pic_original_size);     
	    debugclose(dataFile);
#endif

		/*APP_PRINT_DEBUG("(Advxml_attribute+%d)->PicData_p=0x%x Pic_original_size=%d\n",
					CurrPicTemp,
					(Advxml_attribute+CurrPicTemp)->PicData_p,
					(Advxml_attribute+CurrPicTemp)->Pic_original_size);*/
		
		temp += (Advxml_attribute+CurrPicTemp)->Pic_original_size;
	}
	
	ADV_FreePicInfoStruct();
	ADV_FreePicDataTempStruct();
	
	return false;

}

BS08 ADV_GetAdvmentVersion(void)
{
	return AdvmentVersion;
}









#define Cache_data




/*******************************************************************************
********************************************************************************/
static void ADV_CachePicInfoData(BU08* Data_p,int SectLength,BU16 TableExtenId)
{
	BU08 SectionNum;		
	BU08 LastSecNum;
	int i;
	int length = ((ADInfoStruct->Pic_info_size)/ADV_SECTION_LENGTH+1)*ADV_SECTION_LENGTH;

	/*暂时认为PicInfo 的tableExtenId只有一个*/
	
	if(ADInfoStruct->PicInfoDataTemp==NULL)
	{
		APP_PRINT_DEBUG("no PicInfo memory!!!!");
		return;
	}

	SectionNum = Data_p[6];
	LastSecNum = Data_p[7];
	
	if(Data_p[8] != 0x02 )
		return;

	if ( (AdvMask[SectionNum>>5]&(1<<(SectionNum&0x1f))) != (1<<(SectionNum&0x1f)) ) 
	{
		AdvMask[SectionNum>>5] |= (1<<(SectionNum&0x1f));
		TotalSectionNum++;

		if(SectionNum*ADV_SECTION_LENGTH+SectLength<length)
			memcpy(ADInfoStruct->PicInfoDataTemp+SectionNum*ADV_SECTION_LENGTH,Data_p,SectLength);
	}

	if(TotalSectionNum == (LastSecNum+1))
	{
		ADInfoStruct->PicInfo_OK =  true;
		ADV_DefaultSectionNumTag();
		APP_ADV_IPI_StopDemuxADV();
		ADV_ParsePicInfo(ADInfoStruct->PicInfoDataTemp);
	}

}

/*******************************************************************************
********************************************************************************/
static void ADV_CacheIndexData(BU08* Data_p,int SectLength,BU16 TableExtenId)
{
	BU08 SectionNum;		
	BU08 LastSecNum;
	int i;
	int length = ((ADInfoStruct->Index_xml_size)/ADV_SECTION_LENGTH+1)*ADV_SECTION_LENGTH;

	/*暂时认为index.xml 的tableExtenId只有一个*/

	if(ADInfoStruct->IndexDataTemp==NULL)
	{
		APP_PRINT_DEBUG("no index memory!!!!");
		return;
	}

	SectionNum = Data_p[6];
	LastSecNum = Data_p[7];

	if(Data_p[8] != 0x01)
		return;

	if ( (AdvMask[SectionNum>>5]&(1<<(SectionNum&0x1f))) != (1<<(SectionNum&0x1f)) ) 
	{
		AdvMask[SectionNum>>5] |= (1<<(SectionNum&0x1f));
		TotalSectionNum++;
		if(SectionNum*ADV_SECTION_LENGTH+SectLength<length)
			memcpy(ADInfoStruct->IndexDataTemp+SectionNum*ADV_SECTION_LENGTH,Data_p,SectLength);
	}

	if(TotalSectionNum == (LastSecNum+1))
	{
		ADInfoStruct->Index_OK = true;
		ADV_DefaultSectionNumTag();
		APP_ADV_IPI_StopDemuxADV();
		ADV_ParseIndex(ADInfoStruct->IndexDataTemp);
	}

}

/*******************************************************************************
********************************************************************************/
static void ADV_CachePictureData(BU08* Data_p,int SectLength,BU16 TableExtenId)/*zsy:0722*/
{
	BU08 SectionNum;		
	BU08 LastSecNum;
	int i;
	static BU16 CachePicNum=0;
	BS32 PicCacheIndex;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	PicCacheIndex = ADV_SearchPicCacheIndex(TableExtenId);
	if (PicCacheIndex == -1)
	{
		return;
	}

	if((PicDataTemp+PicCacheIndex)->Pic_OK == true)
		return;
	
	SectionNum = Data_p[6];
	LastSecNum = Data_p[7];
	

	if(PicCacheIndex >= ADV_GetTotalPicNum() || (PicDataTemp + PicCacheIndex)->buf ==NULL)
	{
		APP_PRINT_DEBUG("no PictureData memory!!!!\n");
		return;
	}
       
	if(SectionNum>=(Advxml_attribute+PicCacheIndex)->Pic_segment_num 
		|| SectLength > ADV_SECTION_LENGTH)
		return;

	if ( (((PicDataTemp+PicCacheIndex)->mask[SectionNum>>5])&(1<<(SectionNum&0x1f))) != (1<<(SectionNum&0x1f)) ) 
	{
		((PicDataTemp+PicCacheIndex)->mask[SectionNum>>5]) |= (1<<(SectionNum&0x1f));
		((PicDataTemp+PicCacheIndex)->TotalSectNum)++;
		//APP_PRINT_DEBUG("TableExtenId=%d,SectionNum=%d,SectLength=%d\n",TableExtenId,SectionNum,SectLength);
		memcpy((PicDataTemp+PicCacheIndex)->buf+SectionNum*ADV_SECTION_LENGTH,Data_p,SectLength);
	}

	if(((PicDataTemp+PicCacheIndex)->TotalSectNum) == (LastSecNum+1))
	{
		(PicDataTemp+PicCacheIndex)->Pic_OK = true;
		CachePicNum++;
		APP_PRINT_DEBUG("pic %d OK!\n",PicCacheIndex + 1);
		//for(i=0;i<(Advxml_attribute+TableExtenId-1)->Pic_segment_num;i++)
		//APP_PRINT_DEBUG("--length = %d,secNum=%d, tabId=0x%x extTd=%d\n",
		//(((PicDataTemp+TableExtenId-1)->buf[9+i*ADV_SECTION_LENGTH]&0xf)<<8)|(PicDataTemp+TableExtenId-1)->buf[10+i*ADV_SECTION_LENGTH] ,(PicDataTemp+TableExtenId-1)->buf[6+i*ADV_SECTION_LENGTH],(PicDataTemp+TableExtenId-1)->buf[0+i*ADV_SECTION_LENGTH],(((PicDataTemp+TableExtenId-1)->buf[3+i*ADV_SECTION_LENGTH]&0xf)<<8)|(PicDataTemp+TableExtenId-1)->buf[4+i*ADV_SECTION_LENGTH]);

	}
	if(ADV_GetTotalPicNum() == CachePicNum)
	{	
		msgRecvTemp.qlstWordOfMsg = 1;
		FYF_API_queue_send(queueParserAdvEnd, &msgRecvTemp);
		APP_ADV_IPI_StopDemuxADV();
		ADInfoStruct->PicData_OK= true;
		APP_PRINT_DEBUG("ADV_CachePictureData OK!\n");
	}

}

/*******************************************************************************
********************************************************************************/
static void ADV_CacheData(BU08* Data, BU32 chnnelid, BU32 filterid)
{
	int i;
	BU16 TableExtenId;
	BU32 SectLength;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	APP_PRINT_DEBUG("ADV_CacheData");
	if(ADV_GetSkipFilter_OK())
		return;

	TableExtenId = (Data[3]<<8)|Data[4];
	SectLength = ((0x0F & Data[1]) << 8) + Data[2];

	//APP_PRINT_DEBUG("ADV_CacheData Data[0] = 0x%x, TableExtenId = 0x%x, SectLength = %d \n", Data[0], TableExtenId, SectLength);

	/*****************for adv picture  table****************************/
	if(Data[0]==ADInfoStruct->AD_Picdata_table_id && ADInfoStruct->PicData_OK==false)
	{
		ADV_CachePictureData(Data,SectLength,TableExtenId);
	}
	/*****************for Down_Ctrl_Info  table************************/
	else if(Data[0]==ADInfoStruct->AD_Headdata_table_id && TableExtenId==0 
		&& ADInfoStruct->DownCtrlInfo_OK==false)
	{
		ADV_ParseDownCtrlInfo(Data);
		return;
	}
	/*****************for Pic_info  table*******************************/
	else if(Data[0]==ADInfoStruct->AD_Headdata_table_id && ADInfoStruct->PicInfo_OK == false &&
		TableExtenId>=ADInfoStruct->Pic_info_table_extension_id_start &&
		TableExtenId<=ADInfoStruct->Pic_info_table_extension_id_end)
	{
		ADV_CachePicInfoData(Data,SectLength,TableExtenId);
		return;
	}
	/*****************for index_xml  table*****************************/
	else if(Data[0]==ADInfoStruct->AD_Headdata_table_id && ADInfoStruct->Index_OK == false &&
		TableExtenId>=ADInfoStruct->Index_xml_table_extension_id_start&&
		TableExtenId<=ADInfoStruct->Index_xml_table_extension_id_end)
	{
		ADV_CacheIndexData(Data,SectLength,TableExtenId);
		return;
	}

	/*****************for SI  table***************************/
	else if(ADInfoStruct->BAT_OK==false && Data[0]==0x4a/*BAT_TABLE_ID*/)
	{
		ADV_ParseBAT(Data);

		return;
	}


}



#if 0
static void ADV_SetFlashCorruptedFlag(BOOLEAN CorruptedFlag)
{
	TPSYSTEM_SetAdvFlashCorruptedFlag(CorruptedFlag);
	TPSYSTEM_UpdateNvmBoxBase(E2_SAVE_ADV_FLASH_CORRUPT);
	MILLI_DELAY(100);
}
#endif





BU08 APP_ADV_IPI_StartDemuxADVVersion(void)
{
	BU08 batCoef[3] = {0x4A, 0xFF, 0x00};
	BU08 batMask[3] = {0xFF, 0xFF, 0xFF};
	BU08 batExcl[3] = {0x00, 0x00, 0x00};

	ADV_MallocPicInfoStruct();


	struct ParserFuncNode_s parserFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	if(queueParserAdvEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserAdvEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserAdvEnd = FYF_API_queue_create("qPAdvE", 1, FYF_THREAD_WAIT_FIFO);		
	}

	APP_ADV_IPI_StopDemuxADV();	
	isStopParserAdv = 0;
	APP_Parser_IPI_StartDemux(3, ADV_DATA_PID, batCoef, batMask, batExcl, &AdvChannelID, &AdvFilterID);
	parserFuncNode.channelId = AdvChannelID;
	parserFuncNode.filterId = AdvFilterID;
	parserFuncNode.ParserFunc = ADV_CacheData;
	APP_IPI_AddPasFuncNode(parserFuncNode);

	msgRecvTemp.qlstWordOfMsg = 0;
	FYF_API_queue_recv(queueParserAdvEnd, &msgRecvTemp, 4000);
	if (msgRecvTemp.qlstWordOfMsg == 1)
	{
		APP_PRINT_DEBUG("recv bat version num ok!\n");
	}
	else if (msgRecvTemp.qlstWordOfMsg == 2)
	{
		APP_PRINT_DEBUG("recv bat version num ok but eqal!\n");
		APP_ADV_IPI_StopDemuxADV();	
		ADV_FreePicInfoStruct();
		ADV_FreePicDataTempStruct();
		return 2;
	}
	else
	{
		APP_PRINT_DEBUG("recv bat version num error!\n");
		APP_ADV_IPI_StopDemuxADV();	
		ADV_FreePicInfoStruct();
		ADV_FreePicDataTempStruct();
		return APP_FALSE;
	}

	msgRecvTemp.qlstWordOfMsg = 0;
	FYF_API_queue_recv(queueParserAdvEnd, &msgRecvTemp, 10000);
	if (msgRecvTemp.qlstWordOfMsg == 1)
	{
		APP_PRINT_DEBUG("recv pic info  ok!\n");
	}
	else
	{
		APP_PRINT_DEBUG("recv pic info  error!\n");
		APP_ADV_IPI_StopDemuxADV();	
		ADV_FreePicInfoStruct();
		ADV_FreePicDataTempStruct();
		return APP_FALSE;
	}

	msgRecvTemp.qlstWordOfMsg = 0;
	FYF_API_queue_recv(queueParserAdvEnd, &msgRecvTemp, 15000);
	if (msgRecvTemp.qlstWordOfMsg == 1)
	{
		APP_PRINT_DEBUG("recv pic data  ok!\n");
	}
	else
	{
		APP_PRINT_DEBUG("recv pic data error!\n");
		APP_ADV_IPI_StopDemuxADV();	
		ADV_FreePicInfoStruct();
		ADV_FreePicDataTempStruct();
		return APP_FALSE;
	}

	return APP_TRUE;
	
}



/****************************************************
将新的LOGO数据保存到FLASH中
*****************************************************/
void APP_ADV_IPI_SaveAdvData()
{
	BU08*  Data_p=NULL;
	ADVXML_STRUCT*  DataTemp;
	BU32* dataflag;
	int i, channeltemp;
	int StructSize=0;
	BU08 TotalNum[4];
	int TotalLength=0;
	int loop=0;
	FYF_FLS_IOCTL_PARAM_s fyf_fls_ioctl_param;
	if(AdvPicDataPoint == NULL || Advxml_attribute == NULL)
		return;

	StructSize = sizeof(ADVXML_STRUCT)*ADV_GetTotalPicNum();
	if((StructSize+4) > 1024 * 20) /*+4 is for the TotalNum*/
	{
		APP_PRINT_DEBUG("has no enough flash memory for adv PicInfo Struct!\n");
		return;
	}
	TotalLength = ADV_GetPicMemorySize();
	if(TotalLength&1 != 0)
		TotalLength+=1;
	if(TotalLength > 380 * 1024)
	{
		APP_PRINT_DEBUG("Adv total memory too large!!\n");
		return;
	}

	Data_p = (BU08*) malloc(StructSize + 20);
	if(Data_p == NULL)
	{
		APP_PRINT_DEBUG("has no enough memory for adv PicInfo Struct!\n");
		return;
	}
	dataflag = (BU32*)Data_p;
	*dataflag = 0x12345678;
	dataflag ++;
	*dataflag = ADV_GetTotalPicNum();
	printf("*dataflag = %d\n", *dataflag);
	
	DataTemp = (ADVXML_STRUCT*)(Data_p + 8);
	
	for (i = 0; i < ADV_GetTotalPicNum(); i++)
	{
		memcpy(DataTemp->pic_name,(Advxml_attribute+i)->pic_name,advxml_picname_max);
		memcpy(DataTemp->text,(Advxml_attribute+i)->text,advxml_text_max);
		DataTemp->total_channel_number = (Advxml_attribute+i)->total_channel_number;
		for(channeltemp = 0; channeltemp < ADV_MAX_CHANNEL_NUM; channeltemp++)
		{
			DataTemp->channel_name[channeltemp] 			= (Advxml_attribute+i)->channel_name[channeltemp];
		}
		//DataTemp->channel_name 			= (Advxml_attribute+i)->channel_name;
		DataTemp->file_number 			= (Advxml_attribute+i)->file_number;
		DataTemp->position_x 			= (Advxml_attribute+i)->position_x;
		DataTemp->position_y 			= (Advxml_attribute+i)->position_y;
		DataTemp->width 				= (Advxml_attribute+i)->width;
		DataTemp->height 				= (Advxml_attribute+i)->height;
		DataTemp->duration 				= (Advxml_attribute+i)->duration;
		DataTemp->format 				= (Advxml_attribute+i)->format;
		DataTemp->nametype 				= (Advxml_attribute+i)->nametype;
		DataTemp->Pic_original_size 	= (Advxml_attribute+i)->Pic_original_size;
		DataTemp->Pic_compressed_size 	= (Advxml_attribute+i)->Pic_compressed_size;
		DataTemp->PIC_ID 				= (Advxml_attribute+i)->PIC_ID;
		DataTemp->Pic_segment_length 	= (Advxml_attribute+i)->Pic_segment_length;
		DataTemp->Pic_segment_num 		= (Advxml_attribute+i)->Pic_segment_num;
		DataTemp->Pic_compressed_flag 	= (Advxml_attribute+i)->Pic_compressed_flag;
		
		DataTemp++;
	}
	ADV_FlashUpgradeProcess(50);
	fyf_fls_ioctl_param.buffer = Data_p;
	fyf_fls_ioctl_param.bufferSize = StructSize + 20;

	FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_ADVLOGO_INFO, (BU32)&fyf_fls_ioctl_param);
	ADV_FlashUpgradeProcess(70);
		
	if(Data_p != NULL)
		free(Data_p);

	fyf_fls_ioctl_param.buffer = AdvPicDataPoint;
	fyf_fls_ioctl_param.bufferSize = TotalLength;

	FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_ADVLOGO_DATA, (BU32)&fyf_fls_ioctl_param);
	ADV_FlashUpgradeProcess(100);
	APP_PRINT_DEBUG("----ADV_WritePicToFlash success!----\n");
}

static BOOLEAN  ADV_StructValidCheck(void)
{
	int i;
	
	if(ADV_GetTotalPicNum()<=0)
		return FALSE;

	for (i = 0; i < ADV_GetTotalPicNum(); i++)
	{
		if (Advxml_attribute[i].format != _MPG && Advxml_attribute[i].nametype != -1)
		{
			if((Advxml_attribute[i].position_x) <0 || (Advxml_attribute[i].position_x) > 720)
				return FALSE;

			if((Advxml_attribute[i].position_y) <0 || (Advxml_attribute[i].position_y) > 576)
				return FALSE;

			if((Advxml_attribute[i].width) <= 0 || (Advxml_attribute[i].width) > 720)
				return FALSE;

			if((Advxml_attribute[i].height) <= 0 || (Advxml_attribute[i].height) > 576)
				return FALSE;
		}
	}
	return TRUE;
}

/****************************************************
获取FLASH中ADV的长度和版本号
*****************************************************/
BU08 APP_ADV_IPI_GetAdvData()
{
	BU08 data[8];
	BU32* dataflag;
	BU08*  Data_p=NULL;
	ADVXML_STRUCT* DataTemp;
	int StructSize=0;
	int i, channeltemp;
	int TotalLength=0;
	int temp = 0;
	BU32 flash_destroy_flag;
	APP_SYS_API_GetSysInfo(APP_SYS_AdvSaveFlag_e, &flash_destroy_flag);
	if (flash_destroy_flag == 1)
		return APP_FALSE;
	FYF_FLS_IOCTL_PARAM_s fyf_fls_ioctl_param;
	fyf_fls_ioctl_param.buffer = data;
	fyf_fls_ioctl_param.bufferSize = 8;
	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_ADVLOGO_INFO, (BU32)&fyf_fls_ioctl_param);

	dataflag = (BU32 *)data;
	APP_PRINT_DEBUG("dataflag = 0x%x", *dataflag);
	if (*dataflag != 0x12345678)
	{
		return APP_FALSE;
	}

	dataflag++;
	APP_PRINT_DEBUG("dataflag = 0x%x", *dataflag);
	ADV_SetTotalPicNum(*dataflag);
	if (ADV_MallocXMLStruct())
	{
		return APP_FALSE;
	}
	
	StructSize = sizeof(ADVXML_STRUCT)*ADV_GetTotalPicNum();

	Data_p = (BU08*) malloc(StructSize + 20);
	if (Data_p == NULL)
	{
		APP_PRINT_DEBUG("has no enough memory for adv PicInfo Struct!\n");
		return APP_FALSE;
	}

	fyf_fls_ioctl_param.buffer = Data_p;
	fyf_fls_ioctl_param.bufferSize = StructSize + 20;

	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_ADVLOGO_INFO, (BU32)&fyf_fls_ioctl_param);

	DataTemp = (ADVXML_STRUCT*)(Data_p + 8);
	for (i = 0; i < ADV_GetTotalPicNum(); i++)
	{
		memcpy((Advxml_attribute+i)->pic_name,DataTemp->pic_name,advxml_picname_max);
		memcpy((Advxml_attribute+i)->text,DataTemp->text,advxml_text_max);
		 //ADV_Debug("%s\n",(Advxml_attribute+i)->text);

		(Advxml_attribute+i)->total_channel_number = DataTemp->total_channel_number;
		for(channeltemp = 0; channeltemp < ADV_MAX_CHANNEL_NUM; channeltemp++)
		{
			(Advxml_attribute+i)->channel_name[channeltemp] = DataTemp->channel_name[channeltemp];
		}
		(Advxml_attribute+i)->file_number = DataTemp->file_number;
		(Advxml_attribute+i)->position_x = DataTemp->position_x;
		(Advxml_attribute+i)->position_y = DataTemp->position_y;
		(Advxml_attribute+i)->width = DataTemp->width ;
		(Advxml_attribute+i)->height = DataTemp->height ;
		(Advxml_attribute+i)->duration = DataTemp->duration;
		(Advxml_attribute+i)->format = DataTemp->format;
		(Advxml_attribute+i)->nametype = DataTemp->nametype;
		(Advxml_attribute+i)->Pic_original_size = DataTemp->Pic_original_size;
		(Advxml_attribute+i)->Pic_compressed_size = DataTemp->Pic_compressed_size;
		(Advxml_attribute+i)->PIC_ID = DataTemp->PIC_ID;
		(Advxml_attribute+i)->Pic_segment_length = DataTemp->Pic_segment_length;
		(Advxml_attribute+i)->Pic_segment_num = DataTemp->Pic_segment_num;
		(Advxml_attribute+i)->Pic_compressed_flag = DataTemp->Pic_compressed_flag;
		
		DataTemp++;
	}

	if (Data_p != NULL)
		free(Data_p);

	if (ADV_StructValidCheck()==FALSE )
	{
			return APP_FALSE;
	}
	
	if (ADV_MallocPicDataPoint())
	{
		return APP_FALSE;
	}

	TotalLength = ADV_GetPicMemorySize();
	if (TotalLength&1 != 0)
		TotalLength += 1;

	fyf_fls_ioctl_param.buffer = AdvPicDataPoint;
	fyf_fls_ioctl_param.bufferSize = TotalLength;
	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_ADVLOGO_DATA, (BU32)&fyf_fls_ioctl_param);
	for (i=0;i<ADV_GetTotalPicNum();i++)
	{
		(Advxml_attribute+i)->PicData_p = AdvPicDataPoint+temp;
		temp += (Advxml_attribute+i)->Pic_original_size;
	}

	return APP_TRUE;
}


void APP_ADV_API_DisPlayStartLogoGif()
{
	BU32 hDecoder, hDecSurface;
	FYF_Rect_s RealRect;
	BS32 ret;
	BS32 i;
	BU32 hLayerSurface;
	BU32 ucOldVersionNumber;
	OSDGAL_GetSurface(FYF_API_OSD_GetHandle(), &hLayerSurface);
	for (i = 0; i < ADV_GetTotalPicNum(); i++)
	{
		if (Advxml_attribute[i].channel_name[0] == -1 && Advxml_attribute[i].nametype == 0)
		{
			ret = FYF_API_JPEG_CreateDecoderFromMem(&hDecoder, Advxml_attribute[i].PicData_p, Advxml_attribute[i].Pic_original_size);
			ret |= FYF_API_JPEG_DecImageData(hDecoder, 720, 576, FYF_OSD_PF_1555, &hDecSurface);
			ret |= FYF_API_JPEG_DestroyDecoder(hDecoder);
			if (ret != FYF_OK)
			{
				APP_PRINT_ERROR("picture decoer error\n");
			}
			printf("Advxml_attribute[i].width = %d %d \n", Advxml_attribute[i].width, Advxml_attribute[i].height);
			RealRect.x = 0;
			RealRect.y = 0;
			RealRect.w = Advxml_attribute[i].width;
			RealRect.h = Advxml_attribute[i].height;
			if (FYF_OK == FYF_API_OSD_Blit(hDecSurface, NULL, hLayerSurface, &RealRect))
			{
				FYF_API_OSD_Refresh(0, 0, 720, 576);
			}
			FYF_API_OSD_DestroySurface(hDecSurface);
			break;
		}
	}
		
}

void APP_ADV_API_TestDisPlay(BU32 prog_lcn, BU08 bDisplay)
{
	BU32 hDecoder, hDecSurface;
	FYF_Rect_s RealRect;
	BS32 ret;
	BS32 i, j;
	BU32 hLayerSurface;
	BU32 ucOldVersionNumber;
	BU08 adv_flag = 0;
	BU32 total_channel_proglcn;
	static BS08 cur_pic_num = -1;
	APP_PRINT_DEBUG("prog_lcn = %d， bDisplay = %d, cur_pic_num = %d\n", prog_lcn, bDisplay, cur_pic_num);
	OSDGAL_GetSurface(FYF_API_OSD_GetHandle(), &hLayerSurface);
	for (i = 0; i < ADV_GetTotalPicNum(); i++)
	{

		if (Advxml_attribute[i].nametype != 2) continue;
		total_channel_proglcn = Advxml_attribute[i].total_channel_number;
		printf("total_channel_proglcn = %d\n", total_channel_proglcn);
		for (j = 0; j < total_channel_proglcn; j++)
		{
			if (Advxml_attribute[i].channel_name[j] == prog_lcn)
			{
				adv_flag = 1;
				break;
			}
		}
		if (adv_flag)
		{
			//与上次显示的广告图片一致，则不再重新显示
			if (cur_pic_num != -1 && cur_pic_num == i && bDisplay == 0)
				break;
			cur_pic_num = i;
			ret = FYF_API_JPEG_CreateDecoderFromMem(&hDecoder, Advxml_attribute[i].PicData_p, Advxml_attribute[i].Pic_original_size);
			ret |= FYF_API_JPEG_DecImageData(hDecoder, 720, 576, FYF_OSD_PF_1555, &hDecSurface);
			ret |= FYF_API_JPEG_DestroyDecoder(hDecoder);
			if (ret != FYF_OK)
			{
				APP_PRINT_ERROR("picture decoer error\n");
			}
			printf("Advxml_attribute[i].width = %d %d \n", Advxml_attribute[i].width, Advxml_attribute[i].height);
			RealRect.x = 527;
			RealRect.y = 402;
			RealRect.w = Advxml_attribute[i].width;
			RealRect.h = Advxml_attribute[i].height;
			if (FYF_OK == FYF_API_OSD_Blit(hDecSurface, NULL, hLayerSurface, &RealRect))
			{
				FYF_API_OSD_Refresh(0, 0, 720, 576);
			}
			FYF_API_OSD_DestroySurface(hDecSurface);
			break;
		}
	}

	if (!adv_flag)
	{
		for (i = 0; i < ADV_GetTotalPicNum(); i++)
		{
			if (Advxml_attribute[i].nametype != 2) continue;
			if (Advxml_attribute[i].channel_name[0] == 65535 )
			{
				//与上次显示的广告图片一致，则不再重新显示
				if (cur_pic_num != -1 && cur_pic_num == i && bDisplay == 0)
					break;
				cur_pic_num = i;
				ret = FYF_API_JPEG_CreateDecoderFromMem(&hDecoder, Advxml_attribute[i].PicData_p, Advxml_attribute[i].Pic_original_size);
				ret |= FYF_API_JPEG_DecImageData(hDecoder, 720, 576, FYF_OSD_PF_1555, &hDecSurface);
				ret |= FYF_API_JPEG_DestroyDecoder(hDecoder);
				if (ret != FYF_OK)
				{
					APP_PRINT_ERROR("picture decoer error\n");
				}
				printf("Advxml_attribute[i].width = %d %d \n", Advxml_attribute[i].width, Advxml_attribute[i].height);
				RealRect.x = 527;
				RealRect.y = 402;
				RealRect.w = Advxml_attribute[i].width;
				RealRect.h = Advxml_attribute[i].height;
				if (FYF_OK == FYF_API_OSD_Blit(hDecSurface, NULL, hLayerSurface, &RealRect))
				{
					FYF_API_OSD_Refresh(0, 0, 720, 576);
				}
				FYF_API_OSD_DestroySurface(hDecSurface);
				break;
			}
		}
	}
		
}
void APP_ADV_API_Main()
{
	BU32 hDecoder, hDecSurface;
	FYF_Rect_s RealRect;
	BU08 ret;
	BS32 i;
	BU32 hLayerSurface;
	BU32 ucOldVersionNumber;
	OSDGAL_GetSurface(FYF_API_OSD_GetHandle(), &hLayerSurface);
	ret = APP_ADV_IPI_StartDemuxADVVersion();
	if (APP_TRUE == ret)
	{
		ADV_FlashUpgradeProcess(18);
		if (ADV_ParsePicData())//如果图片大小超出则不保存还是用以前的图片
		{
			ADV_SetTotalPicNum(0);
			APP_ADV_IPI_GetAdvData();
			return;
		}
		
		APP_SYS_API_SetSysInfo(APP_SYS_AdvSaveFlag_e, 1);
		APP_SYS_API_SaveSysInfo();
		APP_ADV_IPI_SaveAdvData();
		APP_SYS_API_SetSysInfo(APP_SYS_AdvSaveFlag_e, 0);
		APP_SYS_API_SaveSysInfo();

		APP_SYS_API_GetSysInfo(APP_SYS_AdvVersion_e, &ucOldVersionNumber);
		if ((ADV_GetAdvmentVersion() != ucOldVersionNumber)
			&& (ADV_GetAdvmentVersion() != -1))
		{
		
			APP_PRINT_DEBUG("Save advment version to EEP\n");
			APP_SYS_API_SetSysInfo(APP_SYS_AdvVersion_e, ADV_GetAdvmentVersion());
			APP_SYS_API_SaveSysInfo();

		}
		
	}
	else if (ret == APP_FALSE)
	{
		ADV_SetTotalPicNum(0);
		APP_ADV_IPI_GetAdvData();
	}
}


