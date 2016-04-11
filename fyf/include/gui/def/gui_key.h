#ifndef __GUI_KEY_H_
#define __GUI_KEY_H_

typedef enum 
{
	GUI_VK_0 = 0,		/* 数字0 */
	GUI_VK_1,			/* 数字1 */
	GUI_VK_2,			/* 数字2 */
	GUI_VK_3,			/* 数字3 */
	GUI_VK_4,			/* 数字4 */
	GUI_VK_5,			/* 数字5 */
	GUI_VK_6,			/* 数字6 */
	GUI_VK_7,			/* 数字7 */
	GUI_VK_8,			/* 数字8 */
	GUI_VK_9,			/* 数字9 */

	GUI_VK_LEFT,		/* 向左 */
	GUI_VK_RIGHT,		/* 向右 */
	GUI_VK_UP,			/* 向上 */
	GUI_VK_DOWN,		/* 向下 */
	GUI_VK_SELECT,		/* 确认 */

	GUI_VK_MENU,		/* 菜单 */
	GUI_VK_EXIT,		/* 退出 */
	
	GUI_VK_POWER,		/* 电源 */
	GUI_VK_TRACK,		/* 声道 */
	GUI_VK_STATE, 		/* 状态 */
	GUI_VK_INFO,		/* 信息 */
	GUI_VK_PROGTABLE,	/* 节目表 */
	
	GUI_VK_MUTE,		/* 静音 */
	GUI_VK_RECALL,		/* 返回 */
	GUI_VK_F1,			/* F1 */
	GUI_VK_F2,			/* F2 */
	GUI_VK_F3,			/* F3 */

	GUI_VK_C_UP,		/* 节目+ */
	GUI_VK_C_DOWN,		/* 节目- */
	GUI_VK_V_UP,		/* 音量+ */
	GUI_VK_V_DOWN,		/* 音量- */
	GUI_VK_P_UP,		/* 翻页+ */
	GUI_VK_P_DOWN,		/* 翻页- */
    GUI_VK_OFFSET_UP,   /* 音量补偿+*/
    GUI_VK_OFFSET_DOWN, /* 音量补偿-*/
	
	GUI_VK_F4,			/* F4 */
	GUI_VK_F5,			/* F5 */
	GUI_VK_F6,			/* F6 */
	GUI_VK_F7,			/* F7 */
	GUI_VK_F8,			/* F8 */
	GUI_VK_F9,			/* F9 */
	GUI_VK_F10,			/* F10 */
	GUI_VK_F11,			/* F11 */
	GUI_VK_F12,			/* F12 */
	
	GUI_VK_RED,			/* 红色键 */
	GUI_VK_GREEN,		/* 绿色键 */
	GUI_VK_YELLOW,		/* 黄色键 */
	GUI_VK_BLUE,		/* 蓝色键 */
	GUI_VK_MAIL,		/*邮件*/
	GUI_VK_RadioORTV,	/*Radio/TV切换*/
	GUI_VK_FAV,			/*喜爱*/
	GUI_VK_RES_0,		/* 保留0 */
	GUI_VK_RES_1,		/* 保留1 */
	GUI_VK_RES_2,		/* 保留2 */
	GUI_VK_RES_3,		/* 保留3 */
	GUI_VK_RES_4,		/* 保留4 */
	GUI_VK_RES_5,		/* 保留5 */
	GUI_VK_RES_6,		/* 保留6 */
	GUI_VK_RES_7,		/* 保留7 */
	GUI_VK_RES_8,		/* 保留8 */
	GUI_VK_RES_9		/* 保留9 */
}GUI_VKeyValue_e;

#endif

