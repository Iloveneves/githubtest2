#ifndef KEY_H
#define KEY_H


#define KEY_BKSP    0x08    /*Backspace¼ü*/
#define KEY_TAB     0x09    /*Tab¼ü*/
#define KEY_ENTER   0x0d    /*Enter¼ü*/
#define KEY_ESC     0x1b    /*Escape¼ü*/
#define KEY_SPACE   0x20    /*Space¼ü*/
#define KEY_INS     0x01    /*Insert¼ü*/
#define KEY_DEL     0x02    /*Delete¼ü*/
#define KEY_HOME    0x03    /*Home¼ü*/
#define KEY_END     0x04    /*End¼ü*/
#define KEY_PGUP    0x05    /*Page Up¼ü*/
#define KEY_PGDN    0x06    /*Page Down¼ü*/
#define KEY_F1      0x0e    /*F1¼ü*/
#define KEY_F2      0x0f    /*F2¼ü£¬»ØÍËÖÁÉÏÒ»Ò³*/
#define KEY_F3      0x10    /*F3¼ü£¬½øÈëÏÂÒ»Ò³*/
#define KEY_F4      0x11    /*F4¼ü£¬Ë¢ÐÂµ±Ç°Ò³*/
#define KEY_F5      0x12    /*F5¼ü*/
#define KEY_F6      0x13    /*F6¼ü*/
#define KEY_F7      0x14    /*F7¼ü*/
#define KEY_F8      0x15    /*F8¼ü*/
#define KEY_F9      0x16    /*F9¼ü*/
#define KEY_F10     0x17    /*F10¼ü*/
#define KEY_F11     0x18    /*F11¼ü*/
#define KEY_F12     0x19    /*F12¼ü*/
#define KEY_UP      0x1c    /*ÏòÉÏ¼ü*/
#define KEY_LEFT    0x1d    /*Ïò×ó¼ü*/
#define KEY_RIGHT   0x1e    /*ÏòÓÒ¼ü*/
#define KEY_DOWN    0x1f    /*ÏòÏÂ¼ü*/
#define KEY_SHIFT   0x07    /*Shift¼ü*/
#define KEY_CTRL    0x0b    /*Contrl¼ü*/
#define KEY_ALT     0x0c    /*Alt¼ü*/
#define KEY_CAPS    0x1a    /*CapsLock¼ü*/
#define KEY_NUM     0x7f    /*Num¼ü*/
#define KEY_PRSCR   0x2a    /*¿½±´ÆÁÄ»¼ü*/
#define KEY_SCROLL  0x00    /*¹ö¶¯¼ü*/
#define KEY_RSHIFT  0x80    /*ÓÒShift¼ü*/
#define KEY_RCTRL   0x81    /*ÓÒContrl¼ü*/
#define KEY_RALT    0x82    /*ÓÒContrl¼ü*/
#define KEY_LWIN    0x83    /*×óWindows¼ü*/
#define KEY_RWIN    0x84    /*ÓÒWindows¼ü*/
#define KEY_MENU    0x85    /*Menu¼ü*/
#define KEY_EXIT    0x48
#define KEY_VOL_UP    0x200
#define KEY_VOL_DOWN  0x201


#define KEY_0       0x30    /*Êý×Ö¼ü0*/
#define KEY_1       0x31    /*Êý×Ö¼ü1*/
#define KEY_2       0x32    /*Êý×Ö¼ü2*/
#define KEY_3       0x33    /*Êý×Ö¼ü3*/
#define KEY_4       0x34    /*Êý×Ö¼ü4*/
#define KEY_5       0x35    /*Êý×Ö¼ü5*/
#define KEY_6       0x36    /*Êý×Ö¼ü6*/
#define KEY_7       0x37    /*Êý×Ö¼ü7*/
#define KEY_8       0x38    /*Êý×Ö¼ü8*/
#define KEY_9       0x39    /*Êý×Ö¼ü9*/
                 
#define KEYSTATUS_ALT       0x0c        /*Alt ¼ü*/
#define KEYSTATUS_CTRL      0x30        /*Ctrl¼ü*/
#define KEYSTATUS_SHIFT     0xc0        /*Shift¼ü*/

/* define the function key alias */
#define KEY_GOTO            KEY_F1      /*ä¯ÀÀÍøÖ·*/
#define KEY_REFRESH         KEY_F4      /*Ë¢ÐÂ*/

#define KEY_PAUSE     'a'         /*VODÔÝÍ£¼ü*/
#define KEY_PLAY      'b'         /*VODÔÝÍ£ºó²¥·Å¼ü*/
#define KEY_STOP      'c'         /*VODÍ£Ö¹¼ü*/
#define KEY_BACK      'd'         /*¿ìÍË*/
#define KEY_FORWARD   'e'         /*¿ì½ø*/

#define REMOTE_POWER        0x639cff00
#define REMOTE_MUTE         0x22ddff00
#define REMOTE_INPUT_MOD    0x609fff00
#define REMOTE_TRACK        0x2ed1ff00
#define REMOTE_0            0x7887ff00
#define REMOTE_1            0x6d92ff00
#define REMOTE_2            0x6c93ff00
#define REMOTE_3            0x33ccff00
#define REMOTE_4            0x718eff00
#define REMOTE_5            0x708fff00
#define REMOTE_6            0x37c8ff00
#define REMOTE_7            0x758aff00
#define REMOTE_8            0x748bff00
#define REMOTE_9            0x3bc4ff00
#define REMOTE_ENTER        0x31ceff00
#define REMOTE_BACK         0x6f90ff00
#define REMOTE_VOD          0x659aff00
#define REMOTE_NVOD         0x3fc0ff00
#define REMOTE_PLAYPAUSE    0x3cc3ff00
#define REMOTE_FORWARD      0x29d6ff00
#define REMOTE_REW          0x25daff00
#define REMOTE_UP           0x35caff00
#define REMOTE_LEFT         0x6699ff00
#define REMOTE_RIGHT        0x3ec1ff00
#define REMOTE_DOWN         0x2dd2ff00
#define REMOTE_STOP         0x2fd0ff00
#define REMOTE_DELAYTV      0x7d82ff00
#define REMOTE_EPG          0x629dff00
#define REMOTE_VOL_PLUS     0x7f80ff00
#define REMOTE_VOL_MINUS    0x7e81ff00

#define REMOTE_F1           0x7b84ff00
#define REMOTE_F2           0x7689ff00
#define REMOTE_F3           0x26d9ff00
#define REMOTE_F4           0x6996ff00
#define REMOTE_GOTO         REMOTE_F1

#define REMOTE_PAGEUP	    0x30cfff00
#define REMOTE_PAGEDOWN	    0x6798ff00

#endif /*KEY_H*/

