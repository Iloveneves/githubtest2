#ifndef __GUI_REGCLASS_API_H_
#define __GUI_REGCLASS_API_H_

//regclass
typedef BU32 		(*WNDPROC)(BU32 hWnd,BU32 iMsg,BU32 wParam,BU32 lParam);
typedef void 		(*INITPROC)(BU32 hWnd,void * pAttrib);
typedef void		(*EVENTPROC)(BU32 hWnd,void *pAttrib);
typedef void		(*STATEPROC)(BU32 hWnd,void *pAttrib);
typedef void		(*FOCUSPROC)(BU32 hWnd,void *pAttrib);
typedef void		(*SETVALUEPROC)(BU32 hWnd,BU32 type,BU32 value,BU32 Param);
typedef void		(*GETVALUEPROC)(BU32 hWnd,BU32 type,BU32 * value,BU32 * Param);

typedef struct _tagGUI_WndClass_s
{
	PU08						lpszClassName;	
	WNDPROC 					lpfnWndProc;
	INITPROC					lpfnInitProc;
	EVENTPROC					lpfnEventProc;
	STATEPROC					lpfnStateProc;
	FOCUSPROC					lpfnFocusProc;
	SETVALUEPROC				lpfnSetvalueProc;
	GETVALUEPROC				lpfnGetvalueProc;
	struct _tagGUI_WndClass_s *	pNext;
}GUI_WndClass_s,* GUI_WndClass_sp;

BU32 GUI_API_RegisterClass(GUI_WndClass_sp pWndClass);
GUI_WndClass_sp GUI_API_GetRegClass(PU08 pClassName);


#endif
