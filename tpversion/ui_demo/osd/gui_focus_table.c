#include "include/resource.h"

GUI_FocusTable_s focusTable[]={
//声道设置
	/*{ID_CBX_Track_ChnInfo,	IDI_ALL,	GUI_VK_DOWN,	ID_CBX_Offset_ChnInfo},
	{ID_CBX_Offset_ChnInfo,	IDI_ALL,	GUI_VK_DOWN,	ID_ODC_ChName_ChnInfo},
	
	{ID_CBX_Offset_ChnInfo,	IDI_ALL,	GUI_VK_UP,	ID_CBX_Track_ChnInfo},
	{ID_CBX_Track_ChnInfo,	IDI_ALL,	GUI_VK_UP,	ID_ODC_ChName_ChnInfo},
	*/
/*
//音视频设置
	{ID_CBX_Transparence_AvSet,		IDI_ALL,	GUI_VK_DOWN,		ID_CBX_TrackSet_AVSet},
    {ID_CBX_Transparence_AvSet,		IDI_ALL,	GUI_VK_UP,		    ID_CBX_VideoOut_AVSet},
    {ID_CBX_TrackSet_AVSet,		IDI_ALL,	GUI_VK_DOWN,		ID_CBX_DisplayFormat_AVSet},
	{ID_CBX_TrackSet_AVSet,		IDI_ALL,	GUI_VK_UP,		    ID_CBX_Transparence_AvSet},
	{ID_CBX_DisplayFormat_AVSet,IDI_ALL,	GUI_VK_DOWN,		ID_CBX_TVMode_AVSet},
	{ID_CBX_DisplayFormat_AVSet,IDI_ALL,	GUI_VK_UP,		    ID_CBX_TrackSet_AVSet},
	{ID_CBX_TVMode_AVSet,		IDI_ALL,	GUI_VK_DOWN,		ID_CBX_VideoOut_AVSet},
    {ID_CBX_TVMode_AVSet,		IDI_ALL,	GUI_VK_UP,		    ID_CBX_DisplayFormat_AVSet},
	{ID_CBX_VideoOut_AVSet,		IDI_ALL,	GUI_VK_DOWN,		ID_CBX_Transparence_AvSet},
	{ID_CBX_VideoOut_AVSet,		IDI_ALL,	GUI_VK_UP,		    ID_CBX_TVMode_AVSet},

//语言设置
	{ID_CBX_LangChoice_LanguageSet,		IDI_ALL,	GUI_VK_DOWN,		ID_CBX_InfoTime_LanguageSet},
	{ID_CBX_LangChoice_LanguageSet,		IDI_ALL,	GUI_VK_UP,		    ID_CBX_InfoTime_LanguageSet},
	{ID_CBX_InfoTime_LanguageSet,		IDI_ALL,	GUI_VK_DOWN,		ID_CBX_LangChoice_LanguageSet},
	{ID_CBX_InfoTime_LanguageSet,		  IDI_ALL,	GUI_VK_UP,		    ID_CBX_LangChoice_LanguageSet},
*/

/********************************工厂菜单 ******************/
//频点设置
    {ID_TBX_MainFreq_FreqSet, IDI_ALL,	GUI_VK_DOWN,	ID_TBX_AdFreq_FreqSet},
    {ID_TBX_AdFreq_FreqSet,   IDI_ALL,	GUI_VK_DOWN,	ID_TBX_NvodFreq_FreqSet},
    {ID_TBX_NvodFreq_FreqSet, IDI_ALL,  GUI_VK_DOWN,    ID_TBX_BroadFreq_FreqSet},
    {ID_TBX_BroadFreq_FreqSet,IDI_ALL,  GUI_VK_DOWN,    ID_TBX_StockFreq_FreqSet},
    {ID_TBX_StockFreq_FreqSet,IDI_ALL,  GUI_VK_DOWN,    ID_TBX_MainFreq_FreqSet},

    {ID_TBX_MainFreq_FreqSet,	IDI_ALL,	GUI_VK_UP,	ID_TBX_StockFreq_FreqSet},
    {ID_TBX_AdFreq_FreqSet,     IDI_ALL,    GUI_VK_UP,  ID_TBX_MainFreq_FreqSet},
    {ID_TBX_NvodFreq_FreqSet,   IDI_ALL,    GUI_VK_UP,  ID_TBX_AdFreq_FreqSet},
    {ID_TBX_BroadFreq_FreqSet,  IDI_ALL,    GUI_VK_UP,  ID_TBX_NvodFreq_FreqSet},
    {ID_TBX_StockFreq_FreqSet,  IDI_ALL,    GUI_VK_UP,  ID_TBX_BroadFreq_FreqSet},

//序列号
    {ID_TBX_Manufacture_Sequence, IDI_ALL,	GUI_VK_DOWN,	ID_TBX_StbType_Sequence},
    {ID_TBX_StbType_Sequence,   IDI_ALL,	GUI_VK_DOWN,	ID_TBX_Productyear_Sequence},
    {ID_TBX_Productyear_Sequence, IDI_ALL,  GUI_VK_DOWN,    ID_TBX_Productweek_Sequence},
    {ID_TBX_Productweek_Sequence,IDI_ALL,  GUI_VK_DOWN,    ID_TBX_Sequence_Sequence},
    {ID_TBX_Sequence_Sequence,IDI_ALL,  GUI_VK_DOWN,    ID_TBX_Manufacture_Sequence},

    {ID_TBX_Manufacture_Sequence,	IDI_ALL,	GUI_VK_UP,	ID_TBX_Sequence_Sequence},
    {ID_TBX_StbType_Sequence,     IDI_ALL,    GUI_VK_UP,  ID_TBX_Manufacture_Sequence},
    {ID_TBX_Productyear_Sequence,   IDI_ALL,    GUI_VK_UP,  ID_TBX_StbType_Sequence},
    {ID_TBX_Productweek_Sequence,  IDI_ALL,    GUI_VK_UP,  ID_TBX_Productyear_Sequence},
    {ID_TBX_Sequence_Sequence,  IDI_ALL,    GUI_VK_UP,  ID_TBX_Productweek_Sequence},

//空中升级
    {ID_TBX_PID_OTA, IDI_ALL,	GUI_VK_DOWN,	ID_TBX_Freq_OTA},
    {ID_TBX_Freq_OTA,   IDI_ALL,	GUI_VK_DOWN,	ID_TBX_Symb_OTA},
    {ID_TBX_Symb_OTA, IDI_ALL,  GUI_VK_DOWN,    ID_CBX_QAMMode_OTA},
    {ID_CBX_QAMMode_OTA,IDI_ALL,  GUI_VK_DOWN,    ID_TBX_PID_OTA},

    {ID_TBX_PID_OTA,	IDI_ALL,	GUI_VK_UP,	ID_CBX_QAMMode_OTA},
    {ID_TBX_Freq_OTA,     IDI_ALL,    GUI_VK_UP,  ID_TBX_PID_OTA},
    {ID_TBX_Symb_OTA,   IDI_ALL,    GUI_VK_UP,  ID_TBX_Freq_OTA},
    {ID_CBX_QAMMode_OTA,  IDI_ALL,    GUI_VK_UP,  ID_TBX_Symb_OTA},
//串口写序列号
   {ID_CBX_DetectCard_ComSequence,   IDI_ALL,    GUI_VK_DOWN,    ID_CBX_DetectE2_ComSequence},
   {ID_CBX_DetectE2_ComSequence, IDI_ALL,  GUI_VK_DOWN,    ID_CBX_DetectCom_ComSequence},
   {ID_CBX_DetectCom_ComSequence,IDI_ALL,  GUI_VK_DOWN,	  ID_CBX_EthSet_ComSequence},
   {ID_CBX_EthSet_ComSequence,IDI_ALL,  GUI_VK_DOWN,	  ID_CBX_DetectCard_ComSequence},

   {ID_CBX_DetectCard_ComSequence,	 IDI_ALL,	 GUI_VK_UP,  ID_CBX_EthSet_ComSequence},
   {ID_CBX_EthSet_ComSequence,	 IDI_ALL,	 GUI_VK_UP,  ID_CBX_DetectCom_ComSequence},
   {ID_CBX_DetectE2_ComSequence,   IDI_ALL,    GUI_VK_UP,  ID_CBX_DetectCard_ComSequence},
   {ID_CBX_DetectCom_ComSequence,  IDI_ALL,	  GUI_VK_UP,  ID_CBX_DetectE2_ComSequence},
};

BU32 focusTableNum = sizeof(focusTable)/sizeof(GUI_FocusTable_s);

