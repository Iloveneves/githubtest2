/*===============================================================================

 File Name:ui_color_def.h

 Author:wpengf

 Create Time:2009-8-3
 
 Description:color define

 History:

===============================================================================*/
#if GUI_COLOR == GUI_COLOR_CLUT
	#define COLOR_TRANS	 	255/*透明色*/ 
	#define COLOR_BK_S    	164	//(0xA4)
	#define COLOR_BK_L		146 //(0x92)
	#define COLOR_DUCKBLUE	48  //(0x30)
	#define COLOR_INFOBK    108	//(0x6C)
	#define COLOR_TIMEBK    	
	#define COLOR_PBR_BAR	220 //(0xDC)
	#define COLOR_PBR_BK	1   //(0x01)
	#define COLOR_WHITE    	1	//(0x01)
	#define COLOR_BLACK    	0	//(0x00)
	#define LOCK_COLOR   	220 //(0xDC)
	#define UNLOCK_COLOR  	7 

#elif GUI_COLOR == GUI_COLOR_1555
	//#define COLOR_TRANS	 	0x00000000/*透明色*/
	#define COLOR_TRANS	 	((0xff>>3)<<10)|(0xff>>3)//0x00000000/*透明色*/ 
	#define COLOR_BK_S    	RGB(200,232,248)//(0x00C8E8F8)
	#define COLOR_BK_L		RGB(152,200,232)//(0x0098C8E8)
	#define COLOR_DUCKBLUE	RGB(73,88,182)
	#define COLOR_INFOBK    RGB(142,184,232)//(0x008EB8E8)
	#define COLOR_TIMEBK    //RGB(140,204,252)
	#define COLOR_PBR_BAR	RGB(145,207,93)//(0x0091CF5D)
	#define COLOR_PBR_BK	RGB(255,255,255)//(0x00FFFFFF)
	#define COLOR_WHITE    	RGB(255,255,255)//(0x00FFFFFF)
	#define COLOR_BLACK 	RGB(0,0,0)
	#define LOCK_COLOR   	RGB(145,207,93)//(0x0091CF5D) 
	#define UNLOCK_COLOR  	RGB(224,46,103)//(0x00E02E67)
	#define COLOR_BARBK     RGB(150, 226, 250)
	#define COLOR_GRAY      RGB(189, 189, 189)
	
#elif GUI_COLOR == GUI_COLOR_565
	#define COLOR_TRANS	 	255/*透明色*/ 
	#define COLOR_BK_S    	3	
	#define COLOR_BK_L		69
	#define COLOR_DUCKBLUE	5
	#define COLOR_INFOBK    161	
	#define COLOR_TIMEBK    123	
	#define COLOR_WHITE  RGB(255,255,255)
	#define COLOR_PBR_BAR  0X0868E8
	#define COLOR_PBR_BK  RGB(255,255,255)
	#define COLOR_BLACK RGB(0,0,0)

#endif

