/*===============================================================================

 File Name:wdt_edt_api.h

 Author:szhy

 Create Time:2007-8-27
 
 Description:edit control

 Class Name:"EDT"

 History:

===============================================================================*/
#ifndef __WDT_EDT_API_H_
#define __WDT_EDT_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_EDT(lpClassName,x,y,nWidth,nHeight,ID,parentID,maxLength) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,maxLength,0,0,0,
#define DEF_EDT_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_EDT_SCROLL_STATE	/* ����״̬ */
{
	WDT_EDT_SCROLL_STATE_PLAY,	/* ��ʼ���� */
	WDT_EDT_SCROLL_STATE_STOP	/* ֹͣ���� */
};
enum WDT_EDT_SV_TYPE
{
	WDT_EDT_SV_VALUE,			/* �����ı����� */
	WDT_EDT_SV_TEXT_COLOR,		/* �����ı���ɫ */
	WDT_EDT_SV_V_SCROLL_POS,    /* ���ô�ֱ����λ�� */
	WDT_EDT_SV_SCROLL_TYPE,		/* ���ù������� */
	WDT_EDT_SV_H_SCROLL_POS,	/* ����ˮƽ����λ�� */
	WDT_EDT_SV_SCROLL_STATE,	/* ���ù���״̬ */
	WDT_EDT_SV_SCROLL_CYCLE,	/* ���ù������� */
	WDT_EDT_SV_SCROLL_BGS,		/* ���ù����ı���ɫ */
	WDT_EDT_SV_PAGE_INDEX,		/* ���ù����ؼ��ĵ�ǰҳ */
	WDT_EDT_SV_PAGE_NUM,		/* ��ù����ؼ���ҳ�� */
	WDT_EDT_SV_CREATE_TIMER,	/* ������ʱ�� */
	WDT_EDT_SV_TEXT_BKCOLOR		/* ���ù����ı��ı���ɫ */
};
enum WDT_EDT_SCROLL_TYPE
{
	WDT_EDT_SCROLL_TYPE_LEFT_RIGHT,		/* ���ù�������Ϊ���� */
	WDT_EDT_SCROLL_TYPE_UP_DOWN			/* ���ù�������Ϊ���� */
};
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_EDT_API_Register(void);
PU08 WDT_EDT_API_GetVersion(void);
#endif

