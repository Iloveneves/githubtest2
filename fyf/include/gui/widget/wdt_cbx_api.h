/*===============================================================================

 File Name:wdt_cbx_api.h

 Author:szhy

 Create Time:2007-5-10
 
 Description:combobox control

 Class Name:"CBX"

 History:

===============================================================================*/
#ifndef __WDT_CBX_API_H_
#define __WDT_CBX_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_CBX(lpClassName,x,y,nWidth,nHeight,ID,parentID,focusStyle,unFocusStyle,styleWidth,styleHeight,exdata_x) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,focusStyle,unFocusStyle,0,styleWidth,styleHeight,exdata_x,0,0,0,
#define DEF_CBX_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/

#define	WDT_CBX_ALIGN_LEFT 1
#define	WDT_CBX_ALIGN_RIGHT 2
#define	WDT_CBX_ALIGN_CENTER 3

#define	WDT_CBX_SV_INDEX 1	//���û�������ֵ
#define	WDT_CBX_SV_DATA 2	//��ֵ
#define	WDT_CBX_SV_EXDATA 3 //��չ��������ʱ����
#define	WDT_CBX_SV_ALIGN 4	//�������ֵ���ʾλ��
#define	WDT_CBX_SV_LABEL 5	//���ÿؼ�����ʾ���ı�
#define	WDT_CBX_SV_LABEL_X 6//���ÿؼ����ı���λ��
#define	WDT_CBX_SV_FGC 7			/* ������Ͽ��ı���ǰ��ɫ */
#define	WDT_CBX_SV_BGC 8			/* ������Ͽ��ı��ı���ɫ */
#define WDT_CBX_SV_STRING 9	//������ʾ�����֣���Ҫȡ����Ϊ0
#define WDT_CBX_SV_TEXTRECT 10 //����������ʾ���� :����1:���ֵ�Xλ��;����2:���ֿ��
#define WDT_CBX_SV_SAVESTRINGLENGTH 11 //�ַ�������
#define WDT_CBX_SV_CYCLE			12
#define WDT_CBX_SV_LABEL_FGC		13//����CBX�ؼ�label��ǰ��ɫ
#define WDT_CBX_SV_TEXT_FGC			14//����CBX�ؼ��ı���ǰ��ɫ

typedef struct tagWDT_CBX_Attrib_s
{
	BU32 textColor_Focus;
	BU32 textColor;
	BU32 labelColor_Focus;
	BU32 labelColor;
}WDT_CBX_Attrib_s;
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_CBX_API_Register(void);
PU08 WDT_CBX_API_GetVersion(void);
void WDT_CBX_API_SetAttrib(WDT_CBX_Attrib_s *pAttrib);
#endif
 
