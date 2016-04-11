#ifndef KEY_H
#define KEY_H


#define KEY_BKSP    0x08    /*Backspace��*/
#define KEY_TAB     0x09    /*Tab��*/
#define KEY_ENTER   0x0d    /*Enter��*/
#define KEY_ESC     0x1b    /*Escape��*/
#define KEY_SPACE   0x20    /*Space��*/
#define KEY_INS     0x01    /*Insert��*/
#define KEY_DEL     0x02    /*Delete��*/
#define KEY_HOME    0x03    /*Home��*/
#define KEY_END     0x04    /*End��*/
#define KEY_PGUP    0x05    /*Page Up��*/
#define KEY_PGDN    0x06    /*Page Down��*/
#define KEY_F1      0x0e    /*F1��*/
#define KEY_F2      0x0f    /*F2������������һҳ*/
#define KEY_F3      0x10    /*F3����������һҳ*/
#define KEY_F4      0x11    /*F4����ˢ�µ�ǰҳ*/
#define KEY_F5      0x12    /*F5��*/
#define KEY_F6      0x13    /*F6��*/
#define KEY_F7      0x14    /*F7��*/
#define KEY_F8      0x15    /*F8��*/
#define KEY_F9      0x16    /*F9��*/
#define KEY_F10     0x17    /*F10��*/
#define KEY_F11     0x18    /*F11��*/
#define KEY_F12     0x19    /*F12��*/
#define KEY_UP      0x1c    /*���ϼ�*/
#define KEY_LEFT    0x1d    /*�����*/
#define KEY_RIGHT   0x1e    /*���Ҽ�*/
#define KEY_DOWN    0x1f    /*���¼�*/
#define KEY_SHIFT   0x07    /*Shift��*/
#define KEY_CTRL    0x0b    /*Contrl��*/
#define KEY_ALT     0x0c    /*Alt��*/
#define KEY_CAPS    0x1a    /*CapsLock��*/
#define KEY_NUM     0x7f    /*Num��*/
#define KEY_PRSCR   0x2a    /*������Ļ��*/
#define KEY_SCROLL  0x00    /*������*/
#define KEY_RSHIFT  0x80    /*��Shift��*/
#define KEY_RCTRL   0x81    /*��Contrl��*/
#define KEY_RALT    0x82    /*��Contrl��*/
#define KEY_LWIN    0x83    /*��Windows��*/
#define KEY_RWIN    0x84    /*��Windows��*/
#define KEY_MENU    0x85    /*Menu��*/
#define KEY_EXIT    0x48
#define KEY_VOL_UP    0x200
#define KEY_VOL_DOWN  0x201


#define KEY_0       0x30    /*���ּ�0*/
#define KEY_1       0x31    /*���ּ�1*/
#define KEY_2       0x32    /*���ּ�2*/
#define KEY_3       0x33    /*���ּ�3*/
#define KEY_4       0x34    /*���ּ�4*/
#define KEY_5       0x35    /*���ּ�5*/
#define KEY_6       0x36    /*���ּ�6*/
#define KEY_7       0x37    /*���ּ�7*/
#define KEY_8       0x38    /*���ּ�8*/
#define KEY_9       0x39    /*���ּ�9*/
                 
#define KEYSTATUS_ALT       0x0c        /*Alt ��*/
#define KEYSTATUS_CTRL      0x30        /*Ctrl��*/
#define KEYSTATUS_SHIFT     0xc0        /*Shift��*/

/* define the function key alias */
#define KEY_GOTO            KEY_F1      /*�����ַ*/
#define KEY_REFRESH         KEY_F4      /*ˢ��*/

#define KEY_PAUSE     'a'         /*VOD��ͣ��*/
#define KEY_PLAY      'b'         /*VOD��ͣ�󲥷ż�*/
#define KEY_STOP      'c'         /*VODֹͣ��*/
#define KEY_BACK      'd'         /*����*/
#define KEY_FORWARD   'e'         /*���*/

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

