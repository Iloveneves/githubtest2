#ifndef __GUI_CONFIG_H_
#define __GUI_CONFIG_H_

/**********平台**************/
#define ST            		(0)
#define PC					(1)
#define cxnt				(2)
#define Ali				    (3)
#define Hisi				(4)
#define GUI_PLATFORM        Hisi

/*********图片格式***********/
#define GUI_COLOR_CLUT	    (0)
#define GUI_COLOR_1555		(1)
#define GUI_COLOR_565		(2)
#define GUI_COLOR GUI_COLOR_1555

/********RGB定义*************/
#define RGB(R,G,B)  ((R>>3)<<10|(G>>3)<<5|(B>>3)|0x8000)

#endif

