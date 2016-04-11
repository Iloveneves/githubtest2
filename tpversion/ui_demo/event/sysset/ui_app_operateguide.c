 #include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define OPERATIONGUIDE_CONTENT_NUM 40      //˵��������
#define OPERATIONGUIDE_CONTENT_LENGTH 60   //ÿ�е���ֽ��� 

#define OPERATIONGUIDE_CONTENT_NUMINPAGE 8 //num in page

static BU08 g_operationGuide_Content[OPERATIONGUIDE_CONTENT_NUM][OPERATIONGUIDE_CONTENT_LENGTH]=
{
     "01   �����е�Դ ͨ��ң���������͹ػ�",
	 "02     ϲ��     ����ϲ��Ƶ���б�",
	 "03     ����     �ر���������ʹ�������� ",
	 "04     ����     �����������Ŀ",
	 "05     �㲥     ����Ƶ�㲥�������",
	 "06     ʱ��     ����ʱ�ƹ���",
	 "07     ��Ѷ     �����ݹ㲥�������",
	 "08   ����/�㲥  ���ֵ��Ӻ���Ƶ�㲥֮���л�",
	 "09   	 ָ��     �򿪽�Ŀָ�Ϸ������",
	 "10     �ʼ�     ���ʼ��б�",
	 "11     ��Ϸ     ���뱾����Ϸ",
	 "12    ��ɫ��    ���ܼ�1",
	 "13    ��ɫ��    ���ܼ�2",
	 "14    ��ɫ��    ���ܼ�3",
	 "15    ��ɫ��    ���ܼ�4/ȫ���²鿴��Ŀ״̬",
	 "16    ��Ŀ��    �����Ŀ�б�",
	 "17     �˵�     �����˵�,���߹رյ�ǰ����",
	 "18     ����     �����ϼ��˵�/����ۿ���Ƶ�����л�",	
	 "19     �˳�     �˳���ȫ��״̬",
	 "20     �ϼ�     �����ƶ����/����Ƶ��",
	 "21     �¼�     �����ƶ����/��СƵ��",
	 "22     ���     �����ƶ����/��С����",
	 "23     �Ҽ�     �����ƶ����/��������",
	 "24     ȷ��     ȷ��ѡ��",
	 "25     ����+    ��������",
	 "26     ����-    ��С����",
	 "27   ��ҳ/��ҳ  �ϡ��·�ҳ",
	 "28    Ƶ����Ϣ  ��ʾ��Ŀ��Ϣ,�ٰ�һ����ʾ��Ŀ״̬��Ϣ",
	 "29     �л�     �����ݹ㲥�л������Ӳ���",
	 "30     ����     ������,������,������ѡ���л���",
	 "31     ����     ����ʹ�ð���ҳ��",
	 "32-40   0-9     ���ּ�",
	 "��ɫ����       ѧϰ��,����ʹ�÷�������:",
	 "               1.��סSET������3������ɫָʾ�Ƴ���;",
	 "               2.��һ��ѧϰ��Ҫ���õļ�(����+,����-,",
	 "               AV/TV��),��ɫָʾ�ƿ�ʼ��˸;",
	 "               3.�����ӻ�ң������׼������ң�����ķ���",
	 "               ��,����Ҫѧϰ�İ���,��������ң��ֹͣ��",
	 "               ˸���ɿ�,��ʱ��ʾ�ü�������ѧϰ;",
	 "               4.�ظ�2.3��������������,����SET��ѧϰ���."
};

static GUI_Style_id g_operationGuide_Style[]=
{
    STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL,
	STYLE_NULL
	
};

static BU08 g_operationGuide_contentInPage[OPERATIONGUIDE_CONTENT_NUMINPAGE][OPERATIONGUIDE_CONTENT_LENGTH];
static GUI_Style_id g_operationGuide_styleInPage[OPERATIONGUIDE_CONTENT_NUMINPAGE];

static BU08 g_operationGuide_curItem = 0; //��ǰѡ�е�ѡ��
static BU08 g_operationGuide_curPage = 0; //��ǰҳ
static BU08 g_operationGuide_PageNum;     //��ҳ��
static BU08 g_operationGuide_rest;        //��ǰҳ��ѡ�����Ŀ

void OperationGuide_IPI_SetParam(void)
{
    g_operationGuide_curItem = 0;
	g_operationGuide_curPage = 0;
	g_operationGuide_PageNum = (OPERATIONGUIDE_CONTENT_NUM - 1)/OPERATIONGUIDE_CONTENT_NUMINPAGE +1;
	g_operationGuide_rest = OPERATIONGUIDE_CONTENT_NUM  %  OPERATIONGUIDE_CONTENT_NUMINPAGE;

	if(g_operationGuide_rest == 0)
	{
	    g_operationGuide_rest = OPERATIONGUIDE_CONTENT_NUMINPAGE;
	}
	
    GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_MAX, (BU32)OPERATIONGUIDE_CONTENT_NUM - 1,0);
	GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_MIN, (BU32)0,0);
	GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_VALUE, (BU32)g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE + g_operationGuide_curItem,0);

}
void OperationGuide_IPI_ShowPage(void)
{ 
    BU08 curPageNum;
	BU08 i;
    if(g_operationGuide_curPage  >=  g_operationGuide_PageNum)
    {
        g_operationGuide_curPage = 0;
		g_operationGuide_curItem = 0;
    }

	if(g_operationGuide_curPage + 1 == g_operationGuide_PageNum)
	{
	    curPageNum = g_operationGuide_rest;
	}
	else
	{
	    curPageNum = OPERATIONGUIDE_CONTENT_NUMINPAGE;
	}
	/*��ջ���*/
	for(i = 0; i < OPERATIONGUIDE_CONTENT_NUMINPAGE; i++)
	{
         GUI_API_Memset(g_operationGuide_contentInPage[i], 0, GUI_API_Strlen(g_operationGuide_contentInPage[i]));
		 g_operationGuide_styleInPage[i] = STYLE_NULL;
	}
	for(i = 0; i< curPageNum; i++)
	{
	    GUI_API_Memcpy(g_operationGuide_contentInPage[i], g_operationGuide_Content[i + g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE],
			           GUI_API_Strlen(g_operationGuide_Content[i + g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE]));
        g_operationGuide_styleInPage[i] = g_operationGuide_Style[i + g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE];
	}
	GUI_ENC_API_SetValue(ID_SBR_OperationGuide, WDT_SBR_SV_VALUE, (BU32)g_operationGuide_curPage*OPERATIONGUIDE_CONTENT_NUMINPAGE + g_operationGuide_curItem,0);
    GUI_ENC_API_Update(ID_ODC_OperationGuide, g_operationGuide_curItem);
}
void UI_APP_OperationGuide_Enter(void)
{
    OperationGuide_IPI_SetParam();
	OperationGuide_IPI_ShowPage();
	APP_Com_API_SetFullScreen();
}

void UI_APP_OperationGuide_Key(BU32 key)
{
    BU08 curPageNum;
	
	
    switch(key)
    {
    case GUI_VK_UP:
		if(g_operationGuide_curItem == 0)
		{
		     if(g_operationGuide_curPage == 0)
		     {
		          g_operationGuide_curPage = g_operationGuide_PageNum -1;
				  g_operationGuide_curItem = g_operationGuide_rest - 1;
		     }
			 else
			 {    
			      g_operationGuide_curPage--;
			      g_operationGuide_curItem = OPERATIONGUIDE_CONTENT_NUMINPAGE - 1;
			 }
		}
		else
		{
		    g_operationGuide_curItem--;
		}
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_DOWN:
		curPageNum = (g_operationGuide_curPage + 1 == g_operationGuide_PageNum)?
			          g_operationGuide_rest : OPERATIONGUIDE_CONTENT_NUMINPAGE;
		if(g_operationGuide_curItem == curPageNum - 1)
		{
		     g_operationGuide_curPage++;
			 g_operationGuide_curItem = 0;
		}
        else
        {
             g_operationGuide_curItem++;
        }
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_P_UP:
		if(g_operationGuide_curPage == 0)
		{
		     g_operationGuide_curPage = g_operationGuide_PageNum - 1;
		}
		else
		{
		     g_operationGuide_curPage--;
		}
		curPageNum = (g_operationGuide_curPage + 1 == g_operationGuide_PageNum)?
			          g_operationGuide_rest : OPERATIONGUIDE_CONTENT_NUMINPAGE;
		g_operationGuide_curItem = curPageNum - 1;
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_P_DOWN:
		g_operationGuide_curPage++;
		g_operationGuide_curItem = 0;
		OperationGuide_IPI_ShowPage();
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	    GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_SELECT, RS_SystemSetting, 0);
		 GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_INDEX, 8, 0);
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
    }
}

void UI_APP_OperationGuide_OnPaint(BU32 hdc,BU32 lParam)
{
    BU32 i;
	BU08 curPageNum;
    lParam = g_operationGuide_curItem;

	GUI_API_DrawStyle(hdc, 0, 0, STYLE_OperationGuideBk_W620_H304);
	GUI_API_DrawStyle(hdc, 0, 38*lParam,STYLE_OperationGuideBar_W620_H36);

    if(g_operationGuide_curPage + 1 == g_operationGuide_PageNum)
	{
	    curPageNum = g_operationGuide_rest;
	}
	else
	{
	    curPageNum = OPERATIONGUIDE_CONTENT_NUMINPAGE;
	}
	
	for(i = 0; i < curPageNum; i++)
	{
	    if(g_operationGuide_styleInPage[i] != STYLE_NULL)
	    {
	        GUI_API_DrawStyle(hdc, 56, 38*i+3, g_operationGuide_styleInPage[i]);
	    }
		GUI_API_TextOut(hdc,2,38*i + 10,g_operationGuide_contentInPage[i],
			            GUI_API_Strlen(g_operationGuide_contentInPage[i]), COLOR_BLACK, COLOR_BLACK, 1);
	}
}

