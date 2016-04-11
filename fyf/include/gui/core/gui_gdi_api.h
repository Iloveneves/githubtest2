#ifndef __GUI_GDI_API_H_
#define __GUI_GDI_API_H_

/********define**********/

//DrawState 
//定义了画图状态:整副图,高有变化,宽有变化,全部都有变
#define DS_STATIC	0
#define DS_HEIGHT 	1
#define DS_WIDTH	2
#define DS_ALL		3  

//DrawMode 
//定义了画图模式:画的是图片,线,矩形,文本

#define DM_PICTURE			0
#define DM_LINE				1
#define DM_RECT				2
#define DM_TEXT				3
#define DM_STYLE			4


#define TEXT_SVL_SIZE			0
#define TEXT_SVL_RECT			1
#define TEXT_SVL_LINEDISTANCE 	2
#define TEXT_SVL_NORECT			3
#define TEXT_SVL_PASSLEN		4

/**********struct***************/
//定义了画图样式的子结构体
typedef struct	_tagGUI_DrawStyleSub_s				
{
	BU32	DS_State;          
	BU32	DM_Mode;

	union 
	{
		BU32	BmpID;   		//根据DrawMode来确定联合体的值
		BU32	LineColor;
		BU32	RectColor;
		BU32	TextID;
		BU32 StyleID;
	}ResoureID;

	BU32	PosX;
	BU32	PosY;
	BU32	Width;
	BU32	Height;
	union 
	{
		BU32	WCycleTimes;   		//根据DrawMode来确定联合体的值
		BU32	LinePosX;
		BU32	TextColor;
		BU32	Reserve;
	}Para1;	

	union 
	{
		BU32	HCycleTimes;   		//根据DrawMode来确定联合体的值
		BU32	LinePosY;
		BU32	TextBKColor;
		BU32	Reserve;
	}Para2;	

}GUI_DrawStyleSub_s,* GUI_DrawStyleSub_sp;

typedef struct _tagGUI_DrawStyleTabel_s
{
	BU32 size;
	BU32 offset;
}GUI_DrawStyleTabel_s,* GUI_DrawStyleTabel_sp;

typedef struct _tagGUI_FontLibHeader_s
{
    BU32 mainVersionNo;
    BU32 secondVersionNo;
    BU32 height;
    BU32 gb2312Width;
    BU32 signalStart;
    BU32 chineseStart;
    BU32 asciiStart;
} GUI_FontLibHeader_s,* GUI_FontLibHeader_sp;

typedef void(*TextOutHOOK)(BU32 hWnd,BU32 x,BU32 y,PU08 pStr,BU32 StrSize,BU32 color,BU32 bkcolor,BU32 trans);
typedef void (*SetTextLineLength)(BU32 len);//3.04
typedef void (*SetLineDistance)(BU32 value);
typedef BU32 (*GetTextLenght)(PU08 pStr,BU32 StrSize);


void GUI_API_SetOSDDevice(PSD osddevice);
PSD GUI_API_GetCurOSDDevice(void);
void GUI_API_SetPalette(void);
void GUI_API_SetTransValue(BU32 color);

void GUI_API_SetBmpLibAddr(void *addr);
void GUI_API_SetFontLibAddr(void *addr);
void GUI_API_SetGB2312LibAddr(void *addr);
void GUI_API_SetAsciiLibAddr(void *addr);
void GUI_API_SetGBSignalLibAddr(void *addr);
void GUI_API_SetStyleDataAddr(void *addr);
void GUI_API_SetStyleTableAddr(void *addr);
void GUI_API_SetDictionaryDataAddr(void *addr);
PU08 GUI_API_GetDictionaryData(BU32 DicID);

void GUI_API_TextOut(BU32 hWnd,BU32 x,BU32 y,PU08 pStr,BU32 StrSize,BU32 color,BU32 bkcolor,BU32 trans);
BU32 GUI_API_GetTextLenght(PU08 pStr,BU32 StrSize);
void GUI_API_SetTextLineLength(BU32 len);//3.04
void GUI_API_TextSetValue(BU32 type,BU32 para1,BU32 para2) ;

void GUI_API_SetLineDistance(BU32 value);
void GUI_API_SetForceGroundColor(BU32 color);
void GUI_API_SetBackGroundColor(BU32 color);
BU32 GUI_API_GetForceGroundColor(void);
BU32 GUI_API_GetBackGroundColor(void);

void GUI_API_SetPixel(BU32 hWnd,BU32 x,BU32 y,BU32 crColor);
BU32 GUI_API_GetPixel(BU32 hWnd,BU32 x,BU32 y);
void GUI_API_DrawHLine(BU32 hWnd,BU32 x,BU32 y,BU32 width, BU32 color);
void GUI_API_DrawVLine(BU32 hWnd,BU32 x,BU32 y,BU32 height, BU32 color);
void GUI_API_DrawLine(BU32 hWnd,BU32 x0,BU32 y0,BU32 x1,BU32 y1, BU32 color);
void GUI_API_DrawRectangle(BU32 hWnd,BU32 x,BU32 y,BU32 width,BU32 height,BU32 color);
void GUI_API_DrawCircle(BU32 hWnd,BU32 x0, BU32 y0, BU32 r,BU32 color);
void GUI_API_DrawEllipse(BU32 hWnd,BU32 x0, BU32 y0, BU32 rx, BU32 ry,BU32 color);
void GUI_API_FillRect(BU32 hWnd,BU32 x,BU32 y,BU32 width,BU32 height,BU32 color);
void GUI_API_DrawIcon(BU32 hWnd,BU32 x,BU32 y,BU32 IconID);
void GUI_API_DrawBitmap(BU32 hWnd,BU32 x,BU32 y,BU32 width,BU32 height, void * buf);
void GUI_API_GetBitmap(BU32 hWnd,BU32 x,BU32 y,BU32 width,BU32 height, void * buf);

void GUI_API_DrawStyle(BU32 hWnd,BU32 x,BU32 y,BU32 resourceID);

void GUI_API_SetRectFreeSize(BU32 size);
BU32 GUI_API_SaveRect(BU32 hWnd);
void GUI_API_ReleaseRect(BU32 hWnd);
void GUI_API_DeleteRect(BU32 hWnd);
BU32 GUI_API_SaveScreenRect(BU32 Hdc,BU32 xpos,BU32 ypos);
BU32 GUI_API_ReleaseScreenRect(BU32 Hdc,BU32 xpos,BU32 ypos);
BU32 GUI_API_SaveScreenRectEx(BU32 Hdc,BU32 xpos,BU32 ypos,BU32 width,BU32 height);
BU32 GUI_API_ReleaseScreenRectEx(BU32 Hdc,BU32 xpos,BU32 ypos,BU32 width,BU32 height);
BU32 GUI_API_CreateDC(BU32 Width,BU32 Height);
void GUI_API_DeleteDC(BU32 hDC);
void GUI_API_GetDCValue(BU32 hDC,PU32 Witdh,PU32 Height);

void GUI_API_OSDUpdate(BU32 x, BU32 y, BU32 width, BU32 height);

void GUI_API_TextOutHook(TextOutHOOK fun);
void GUI_API_SetTextLineLengthHook(SetTextLineLength fun);
void GUI_API_SetLineDistanceHook(SetLineDistance fun);
void GUI_API_GetTextLenghtHook(GetTextLenght fun);



#endif

