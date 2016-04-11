#ifndef __GUI_WINDOW_API_H_
#define __GUI_WINDOW_API_H_

/*
 * Window field offsets for GetWindowLong()
 */

#define GWL_USERDATA        (1)


/*
 * ShowWindow() Commands
 */
#define SW_HIDE             (0)
#define SW_SHOWNOPAINT      (1)
#define SW_SHOW           	(2)

/*
 * Window field offsets for GetWindow()
 */

#define GW_CHILD   		(1)
#define GW_HWNDNEXT   	(2)
#define GW_HWNDPREV   	(3)


// Window type
#define WS_TYPEMASK			0x000f0000L

#define WS_DST			0x00010000L
#define WS_MAIN				0x00020000L
#define WS_CONTROL          0x00040000L

//window status
#define WS_STATUSMASK		0x000000ffL
#define WS_VISIBLE          0x00000001L
#define WS_FOCUS	        0x00000002L
#define WS_DISABLE			0x00000004L
#define WS_ACTIVE	        0x00000008L
#define WS_TRANS			0x00000010L


void GUI_API_SetRect(GUI_Rect_sp lprc, BU32 x, BU32 y,BU32 width,BU32 height );
void GUI_API_OffsetRect(GUI_Rect_sp lprc,BU32 dx, BU32 dy);
void GUI_API_CopyRect(GUI_Rect_sp lprcDst, const GUI_Rect_sp lprcSrc);
void GUI_API_ScreenToWindow(const BU32 hWnd,PU32 px,PU32 py);
void GUI_API_ScreenToWindowRect(const BU32 hWnd,GUI_Rect_sp lprc);
void GUI_API_WindowToScreen(const BU32 hWnd,PU32 px,PU32 py);
void GUI_API_WindowToScreenRect(const BU32 hWnd,GUI_Rect_sp lprc);
void GUI_API_SetFocus(const BU32 hWnd);
BU32 GUI_API_GetFocus(void);
BU32 GUI_API_GetParent(BU32 hWnd);
BU32 GUI_API_GetDesktopWindow(void);
BU32 GUI_API_IsVisible(BU32 hWnd);//2.40
BU32 GUI_API_IsFocus(BU32 hWnd);//2.41
BU32 GUI_API_GetZOrder(BU32 hWnd);//2.42
void GUI_API_SetVisible(BU32 hWnd,BU32 state);//3.01

BU32 GUI_API_CreateWindow(PU08 lpClassName,PU08 lpWindowName,BU32 dwStyle, 
  BU32 x,BU32 y,BU32 nWidth,BU32 nHeight,
  BU32 hWndParent,BU32 WndId,BU32 hInstance,void * lpParam);
BU32 GUI_API_GetWindowLong(BU32 hWnd, BU32 nIndex);
BU32 GUI_API_SetWindowLong(BU32 hWnd, BU32 nIndex, BU32 lNewLong);
BU32 GUI_API_ShowWindow(BU32 hWnd,BU32 nCmdShow);
void GUI_API_DestroyWindow(BU32 hWnd);
BU32 GUI_API_GetClassName(BU32 hWnd,PU08 lpClassName,BU32 nMaxCount);
void GUI_API_SetWindowNum(BU32 num);
BU32 GUI_API_InvalidateRect(const BU32 hWnd,const GUI_Rect_sp lpRect,BU32 bErase);
BU32 GUI_API_BeginPaint(const BU32 hWnd);
void GUI_API_EndPaint(const BU32 hWnd);
void GUI_API_GetClientRect(BU32 hWnd,GUI_Rect_sp lpRect);
void GUI_API_GetWindowRect(BU32 hWnd,GUI_Rect_sp lpRect);
void GUI_API_SetClientPos(BU32 hWnd,BU32 x,BU32 y);
BU32 GUI_API_GetGuiVersion(void);
BU32 GUI_API_GetDlgItem(BU32 nIDDlgItem);
INITPROC GUI_API_GetInitProc(BU32 hWnd);
EVENTPROC GUI_API_GetEventProc(BU32 hWnd);
STATEPROC GUI_API_GetStateProc(BU32 hWnd);
FOCUSPROC GUI_API_GetFocusProc(BU32 hWnd);
SETVALUEPROC GUI_API_GetSValueProc(BU32 hWnd);
GETVALUEPROC GUI_API_GetGValueProc(BU32 hWnd);
BU32 GUI_API_GetCtrlID(BU32 hWnd);
BU32 GUI_API_GetWindow(BU32 hWnd,BU32 uCmd);
BU32 GUI_API_GetDC(const BU32 hWnd);
void GUI_API_ReleaseDC(const BU32 hWnd);
PU08 GUI_API_GetVersionString(void);

#endif
