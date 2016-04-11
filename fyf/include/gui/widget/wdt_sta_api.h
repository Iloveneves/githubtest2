/*===============================================================================

 File Name:wdt_sta_api.h

 Author:szhy

 Create Time:2007-8-22
 
 Description:static control

 Class Name:"STA"

 History:

===============================================================================*/
#ifndef __WDT_STA_API_H_
#define __WDT_STA_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_STA(lpClassName,x,y,nWidth,nHeight,ID,parentID,style,dataID,bSaveBK) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,style,dataID,bSaveBK,0,
#define DEF_STA_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/

#define	WDT_STA_ALIGN_LEFT 1	//�����
#define	WDT_STA_ALIGN_RIGHT 2	//�Ҷ���
#define	WDT_STA_ALIGN_CENTER 3	//����



#define	WDT_STA_SV_DATAID 1		//����̬�ؼ������ֵ����ֵ�ID
#define	WDT_STA_SV_STYLE 2		//������ʽ��ID
#define	WDT_STA_SV_STRING 3		//ֱ�Ӹ���̬�ؼ������ַ���
#define	WDT_STA_SV_ALIGN 4		//�����ı����뷽ʽ
#define	WDT_STA_SV_FGC 5		/* ���þ�̬�ؼ��ı���ǰ��ɫ */
#define	WDT_STA_SV_BGC	6		/* ���þ�̬�ؼ��ı��ı���ɫ */
#define WDT_STA_SV_SAVESTRINGLENGTH 7 //�ַ�������  
#define WDT_STA_SV_BGS 8		/* ���ñ�����ʽ */
#define WDT_STA_SV_YPOS 9			/* ����Y���� */
 
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_STA_API_Register(void);
PU08 WDT_STA_API_GetVersion(void);
#endif

