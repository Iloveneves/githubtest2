/*===============================================================================

 File Name:wdt_mnu_api.h

 Author:szhy

 Create Time:2007-4-1
 
 Description:menu control

 Class Name:"MNU"

 History:

===============================================================================*/
#ifndef __WDT_MNU_API_H_
#define __WDT_MNU_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_MNU(lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle,unSelectStyle,styleWidth,styleHeight,menuItemX,menuItemY,titleX,titleY) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle,selectAndUnFocusStyle, \
	unSelectStyle,styleWidth,styleHeight,menuItemX,menuItemY,titleX,titleY,
#define DEF_MNU_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_MNU_SV_TYPE
{
	WDT_MNU_SV_CURDATAID,
	WDT_MNU_SV_DATA,
	WDT_MNU_SV_SELECT,
	WDT_MNU_SV_INDEX,
	WDT_MNU_SV_SELECT_TEXT_FGC,		/* �˵��ؼ���ǰ����ı�ǰ��ɫ */
	WDT_MNU_SV_UNSELECT_TEXT_FGC,	/* �˵��ؼ��ǵ�ǰ����ı�ǰ��ɫ */
	WDT_MNU_SV_TITLE_POSITION,		/* ���ò˵���������� */
	WDT_MNU_SV_TITLE_TEXT_FGC,		/* ���ò˵�������ı�ǰ��ɫ */
	WDT_MNU_SV_ITEM_INTERVAL,		/* ���ò˵���ļ�� */
	WDT_MNU_SV_TITLE_BGS,			/* ���ò˵�����ı�����ʽ */
	WDT_MNU_SV_TITLE_BGS_POS,		/* ���ò˵�����ı�����ʽ������ */
	WDT_MNU_SV_BKSTYLE,				/* ���ò˵�������ʽ */
	WDT_MNU_SV_BKSTYLE_POS,
	WDT_MNU_SV_ALIGNMENT,			/* ���ò˵����뷽ʽ */
	WDT_MNU_SV_OFFSET,				/* ���ò˵��ı�����ڲ˵�������ƫ���� */
	WDT_MNU_SV_TITLE_RANGE,			/* ���ò˵������ı���ʾ��Χ */
	WDT_MNU_SV_EXIT_OR_RECALL,		/* �˳����򷵻ؼ����ص���һ���˵� */
	WDT_MNU_SV_NUSELECT_STYLE_ARR  /* ���÷�ѡ����ʽ���� */
};
enum WDT_MUN_ALIGNMENT
{
	WDT_MNU_ALIGNMENT_LEFT,			/* ����� */
	WDT_MNU_ALIGNMENT_CENTER,		/* ���� */
	WDT_MNU_ALIGNMENT_RIGHT			/* �Ҷ��� */
};
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_MNU_API_Register(void);
PU08 WDT_MNU_API_GetVersion(void);
#endif

