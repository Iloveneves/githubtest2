#ifndef _GUI_COLOR_H__
#define _GUI_COLOR_H__

#if GUI_COLOR == GUI_COLOR_CLUT
	#define GUI_COLOR_WHITE     		1
	#define GUI_COLOR_BLACK				0
	#define GUI_COLOR_TRANSPARENT 		255
#elif GUI_COLOR == GUI_COLOR_1555
	#define GUI_COLOR_WHITE				RGB(255,255,255)
	#define GUI_COLOR_BLACK				RGB(0,0,0)
	#define GUI_COLOR_TRANSPARENT 		((0xff>>3)<<10)|(0xff>>3)
#elif GUI_COLOR == GUI_COLOR_565
	#define GUI_COLOR_WHITE 			RGB(255,255,255)
	#define GUI_COLOR_BLACK 			RGB(0,0,0)
	#define GUI_COLOR_TRANSPARENT		((0xff>>3)<<10)|(0xff>>3)
#endif

#endif

