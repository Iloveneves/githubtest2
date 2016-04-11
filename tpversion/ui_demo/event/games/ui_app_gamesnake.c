/*------------------------------------------------
file name:game_snake.c
discription:main routine of Game_Snake
renewed time: 2009-3-5

------------------------------------------------------*/

/*---------------------------- include files ----------------------*/
#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"
//#include "ui_app_game_snake.h"

/*---------------------------- 常量定义 ------------------------------*/

#define Game_Snake_UPGRADESCORE    200


#define  Game_Snake_MAXNUM      200  //蛇最大节数
#define  Game_Snake_MaxLevel    10

#define Game_Snake_BackXPos     270
#define Game_Snake_BackYPos     125
#define Game_Snake_BackWidth    400
#define Game_Snake_BackHigh     340

#define Game_Snake_RegionXPos     Game_Snake_BackXPos+20
#define Game_Snake_RegionYPos     Game_Snake_BackYPos+20
#define Game_Snake_RegionWidth    360
#define Game_Snake_RegionHigh     240

#define Game_Snake_XNum     36
#define Game_Snake_YNum     24
#define Game_Snake_Size           10

#if GUI_COLOR == GUI_COLOR_CLUT
	#define Game_Snake_BackColor    108//(0x6C)    //背景颜色
	#define Game_Snake_RegionColor    1//(0x01)    //游戏区域颜色
	#define Game_Snake_SnakeColor     6//(0x06)    //蛇颜色
	#define Game_Snake_SnakeHeadColor 6//(0x06)    //蛇头颜色
	#define Game_Snake_FoodColor      61//(0x3D)   //食物颜色
	#define Game_Snake_BombColor      0            //炸弹颜色
	#define Game_Snake_TextColor      0            //文本颜色

#elif GUI_COLOR == GUI_COLOR_1555
	#define Game_Snake_BackColor                   RGB(142,184,232)//(0x008EB8E8)    //背景颜色
	#define Game_Snake_RegionColor                 RGB(255,255,255)//(0x00FFFFFF)    //游戏区域颜色
	#define Game_Snake_SnakeColor                  RGB(229,100,139)//(0x00E5648B)    //蛇颜色
	#define Game_Snake_SnakeHeadColor              RGB(229,100,139)//(0x00E5648B)    //蛇头颜色
	#define Game_Snake_FoodColor                   RGB(14,63,247)//(0x000E3FF7)      //食物颜色
	#define Game_Snake_BombColor                   RGB(0,0,0)                        //炸弹颜色
	#define Game_Snake_TextColor                   RGB(0,0,0)                        //文本颜色

#elif GUI_COLOR == GUI_COLOR_565
	#define Game_Snake_BackColor    108//(0x6C)    //背景颜色
	#define Game_Snake_RegionColor    1//(0x01)    //游戏区域颜色
	#define Game_Snake_SnakeColor     6//(0x06)    //蛇颜色
	#define Game_Snake_SnakeHeadColor 6//(0x06)    //蛇头颜色
	#define Game_Snake_FoodColor      61//(0x3D)   //食物颜色
	#define Game_Snake_BombColor      0            //炸弹颜色
	#define Game_Snake_TextColor      0            //文本颜色
		
#endif


#define LEVEL_INIT  1
#define DRAW_LEVEL_SCORE  2
#define DRAW_INFOMATION  3
#define DRAW_CONTINUE  4
#define DRAW_NEW  5

/*---------------------------------------------------------------------------------*/

typedef struct Game_Snake_FoodType
{
    BS08 x;                      /*食物横坐标*/
	BS08 y;                      /*食物纵坐标*/
	BS08 yes;                    /*判断是否出现食物的变量*/
}GAME_SNAKE_FOODTYPE;

typedef struct Game_Snake_SnakeType
{
    BS08 x[Game_Snake_MAXNUM];     /*蛇横坐标*/
	BS08 y[Game_Snake_MAXNUM];     /*蛇纵坐标*/
	BU08 Node;                  /*蛇的节数*/
	BS08 Direction;             /*蛇移动方向 1:上;2:下;3:左;4:右*/
	BS08 SnakeDead;             /* 蛇的生命, 0活着, 1死亡*/
//	BU08 SnakeHead;             /*蛇的头 1:是;0:不是*/
	
}GAME_SNAKE_SNAKETYPE;

typedef struct Game_Snake_BombType
{
    BS08 x[Game_Snake_MAXNUM];    /*炸弹横坐标*/
	BS08 y[Game_Snake_MAXNUM];    /*炸弹纵坐标*/
	BU08 Num;                  /*炸弹个数*/
	BU08 No;                   /*是否添加炸弹标志*/
	
}GAME_SNAKE_BOMBTYPE;


/*---------------------------- 变量定义 ------------------------------*/
//BU32 Game_Snake_Speed = 200;    //ms

static BU32 g_game_match;
BU32 Game_Snake_Score = 0;
//BU32 Game_Snake_TempScore = 0;
BU32 Game_Snake_HeroScore = 0;

//BU32 Game_Snake_Heros[11] = {0};


BU08 Game_Snake_Level = 1;
BU08 Game_Snake_StartFlag = false;
BU08 Game_Snake_PauseFlag = false;
BU08 g_game_snake_str[11];
GAME_SNAKE_FOODTYPE  Game_Snake_sFood;
GAME_SNAKE_SNAKETYPE Game_Snake_sSnake;
GAME_SNAKE_BOMBTYPE  Game_Snake_sBomb;


/*判断矩阵0:墙; 1:背景; 2:蛇; 3:食物;*/

//BU08 Game_Snake_SnakeArray[Game_Snake_XNum][Game_Snake_YNum];  


/*-------------------------------------- 函数声明 ------------------------------------*/

void Game_Snake_InitialGame(void);
void Game_Snake_DrawStartView(BU32 hdc);
void Game_Snake_DisplayScoreAndLevel(BU32 hdc);
void Game_Snake_DrawSnake(BU32 hdc,BU32 drawAll);
void Game_Snake_DrawFood(BU32 hdc);
void Game_Snake_DrawBackground(BU32 hdc);
void Game_Snake_DisplayGameOver(BU32 hdc);
void Game_Snake_SnakeMove(void);
void Game_Snake_DrawBomb(BU32 hdc,BU32 drawAll);
void Game_Snake_DisplayHeros(BU32 hdc);
void Game_Snake_DisplayCongratulation(BU32 hdc);

void UI_APP_Game_Snake_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Snake);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_STOP,0,0);
}
void UI_APP_Game_Snake_End(void)
{
	Game_Snake_PauseFlag = true;

}



/*--------------------------------画图函数----------------------------------------------*/
void Game_Snake_OnPaint(BU32 hdc,BU32 lparam)
{
	
	
    switch(lparam)
    {
		case LEVEL_INIT:
            Game_Snake_InitialGame();
			Game_Snake_DrawStartView(hdc);
			Game_Snake_DisplayScoreAndLevel(hdc);
		//	Game_Snake_DisplayHeros(hdc);
			Game_Snake_DrawSnake(hdc,1);  //整条蛇
			Game_Snake_DrawFood(hdc);
			break;
		case DRAW_LEVEL_SCORE:
			Game_Snake_DisplayScoreAndLevel(hdc);
			break;
	    case DRAW_CONTINUE:
			Game_Snake_DrawBackground(hdc);
			Game_Snake_DrawBomb(hdc,1);//
			Game_Snake_DrawSnake(hdc,1);
			break;
		case DRAW_INFOMATION:
			Game_Snake_DisplayGameOver(hdc);
			break;
		case DRAW_NEW:
			Game_Snake_DrawFood(hdc);
			Game_Snake_DrawSnake(hdc,0);//仅画有变化的 一头一尾
			Game_Snake_DrawBomb(hdc,0);//只画增加的那个
			break;
		default:
			break;
				
    }	

}
/*-------------------------按键处理函数------------------------------------*/
void Game_Snake_OnKey(BU32 KeyValue) 
{
	BU32 lang;

	if(GUI_VK_0 == KeyValue)
	{
		g_game_match = 1;
	}
	else if(g_game_match == 1 && GUI_VK_5 == KeyValue)
	{
		g_game_match = 2;
	}
	else if(g_game_match == 2 && GUI_VK_9 == KeyValue)
	{
		g_game_match = 3;
	}
	else if(g_game_match == 3 && GUI_VK_1 == KeyValue)
	{
		Game_Snake_Score = Game_Snake_Score+1000;
	}
	else
	{
		g_game_match = 0;
	}

    if(!Game_Snake_StartFlag)
    {
		if(KeyValue == GUI_VK_SELECT)
		{
			GUI_ENC_API_Update(ID_ODC_Snake,LEVEL_INIT);
			Game_Snake_StartFlag = true;		
			Game_Snake_PauseFlag = false;
			
			if(Game_Snake_Level>=5)
			{
			    GUI_ENC_API_SetValue(ID_TIM_Snake, WDT_TIM_SV_PERIOD,(BU32)150-Game_Snake_Level*10, 0);
			}
			else
			{
			    GUI_ENC_API_SetValue(ID_TIM_Snake, WDT_TIM_SV_PERIOD,(BU32)200-(Game_Snake_Level-1)*25, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_PLAY,0,0);
		}
        else if(KeyValue == GUI_VK_LEFT)
        {
			Game_Snake_Level--;
			if(Game_Snake_Level ==0)Game_Snake_Level = Game_Snake_MaxLevel;
    		GUI_ENC_API_Update(ID_ODC_Snake,DRAW_LEVEL_SCORE);

		}
        else if(KeyValue == GUI_VK_RIGHT)
        {
			Game_Snake_Level++;
			if(Game_Snake_Level > Game_Snake_MaxLevel)Game_Snake_Level = 1;
            GUI_ENC_API_Update(ID_ODC_Snake,DRAW_LEVEL_SCORE);

		}		

    }	
	
	else
	{
	    switch(KeyValue)
		{
		case GUI_VK_SELECT:
			if(Game_Snake_PauseFlag == true)
			{
			    Game_Snake_PauseFlag = false;
				GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_PLAY,0,0);					
				GUI_ENC_API_Update(ID_ODC_Snake,DRAW_CONTINUE); //恢复
			}
			break;
		case GUI_VK_RED:                           //暂停
			if(Game_Snake_PauseFlag == true)
			{
			    Game_Snake_PauseFlag = false;
				GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_PLAY,0,0);		
				GUI_ENC_API_Update(ID_ODC_Snake,DRAW_CONTINUE); //恢复
				break;
			}
			else
			{
			    Game_Snake_PauseFlag = true;
				GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_STOP,0,0);
				APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
				if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
				{
					GUI_API_Strcpy(g_game_snake_str, "PAUSE");
				}
				else
				{
					GUI_API_Strcpy(g_game_snake_str, "暂停");
				}
				GUI_ENC_API_Update(ID_ODC_Snake,DRAW_INFOMATION); //游戏暂停
				return;
			}
		case GUI_VK_UP:
			if(Game_Snake_sSnake.Direction!=2)
			{
				Game_Snake_sSnake.Direction = 1;        //上
			}
			break;
		case GUI_VK_DOWN:
			if(Game_Snake_sSnake.Direction!=1)
			{
				Game_Snake_sSnake.Direction = 2;        //下
			}
			break;
		case GUI_VK_LEFT:
			if(Game_Snake_sSnake.Direction!=4)
			{
				Game_Snake_sSnake.Direction = 3;        //左
			}
			break;
		case GUI_VK_RIGHT:
			if(Game_Snake_sSnake.Direction!=3)
			{
				Game_Snake_sSnake.Direction = 4;        //右
			}
			break;
		case GUI_VK_EXIT:
			Game_Snake_StartFlag = 0;
			Game_Snake_Level = 1;
			GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_STOP,0,0);
			break;			
		default:
			break;
		}
			
	}
	
}
/*-----------------------------------定时函数----------------------------------------*/
void Game_Snake_Timing(void)
{
	BU32 lang;
    
	if((Game_Snake_sFood.x == Game_Snake_sSnake.x[0])&&(Game_Snake_sFood.y == Game_Snake_sSnake.y[0]))
	{
	    Game_Snake_sFood.yes = 1;
		Game_Snake_sSnake.Node++;
		if(Game_Snake_sSnake.Node >= Game_Snake_MAXNUM)Game_Snake_sSnake.Node = Game_Snake_MAXNUM;

		Game_Snake_Score += 10;
#if 0
		if((Game_Snake_Score - Game_Snake_TempScore) >= Game_Snake_UPGRADESCORE)
			{
			    Game_Snake_Level++;
				if(Game_Snake_Level > Game_Snake_MaxLevel)Game_Snake_Level = Game_Snake_MaxLevel;

                if(Game_Snake_Level>=5)
    			{
    			    GUI_ENC_API_SetValue(ID_TIM_Snake, WDT_TIM_SV_PERIOD,(BU32)150-Game_Snake_Level*10, 0);
    			}
    			else
    			{
    			    GUI_ENC_API_SetValue(ID_TIM_Snake, WDT_TIM_SV_PERIOD,(BU32)200-(Game_Snake_Level-1)*25, 0);
    			}     
				
				Game_Snake_TempScore += Game_Snake_UPGRADESCORE;
				
			}
#endif		
		GUI_ENC_API_Update(ID_ODC_Snake,DRAW_LEVEL_SCORE);             //更新分数
	    
		Game_Snake_sBomb.No--;
		
	}
	
	Game_Snake_SnakeMove();             //蛇移动
	
	if(Game_Snake_sSnake.SnakeDead == 0) //没死
    {
	    GUI_ENC_API_Update(ID_ODC_Snake,DRAW_NEW); //更新画面
	}	
	else
	{
        GUI_ENC_API_SetValue(ID_TIM_Snake,WDT_TIM_SV_STOP,0,0); 
		APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
		{
			GUI_API_Strcpy(g_game_snake_str, "GAME OVER");
		}
		else
		{
			GUI_API_Strcpy(g_game_snake_str, "游戏结束");
		}
		GUI_ENC_API_Update(ID_ODC_Snake,DRAW_INFOMATION); //游戏结束
		Game_Snake_StartFlag = 0;


		#if 0
		if(Game_Snake_Score > Game_Snake_HeroScore)//进入英雄榜
		{
		    Game_Snake_HeroScore= Game_Snake_Score;
		    GUI_ENC_API_Update(ID_ODC_Snake,9);//显示恭喜,更新英雄榜
		}
		#endif
    }	




	
}

/*------------------------游戏初始化-------------------------------------------*/
void Game_Snake_InitialGame(void)
{
 //   BU08 x,y;

    Game_Snake_PauseFlag = false;
	Game_Snake_Score = 0;
// 	Game_Snake_TempScore = 0;
	Game_Snake_sSnake.x[0] = 2;
    Game_Snake_sSnake.y[0] = 18;
	Game_Snake_sSnake.x[1] = 1;
	Game_Snake_sSnake.y[1] = 18;
	Game_Snake_sSnake.Node = 2;
	Game_Snake_sSnake.Direction = 4;
	Game_Snake_sSnake.SnakeDead = 0;
	Game_Snake_sFood.yes = 0;
	Game_Snake_sFood.x = 12;
	Game_Snake_sFood.y = 18;
	Game_Snake_sBomb.Num = 0;
	Game_Snake_sBomb.No = 2;
	/*

	for(x=0;x<Game_Snake_XNum;x++)
		for(y=0;y<Game_Snake_YNum;y++)
			Game_Snake_SnakeArray[x][y] = 0;
    
    for(x=1;x<Game_Snake_XNum-1;x++)
		for(y=1;y<Game_Snake_YNum-1;y++)
			Game_Snake_SnakeArray[x][y] = 1;
	*/
		
			
}

/*-------------------------画开始画面------------------------------------------*/
void Game_Snake_DrawStartView(BU32 hdc)
{

    GUI_API_FillRect(hdc, Game_Snake_BackXPos, Game_Snake_BackYPos, 
		                  Game_Snake_BackWidth, Game_Snake_BackHigh, Game_Snake_BackColor);
    GUI_API_FillRect(hdc, Game_Snake_RegionXPos, Game_Snake_RegionYPos, 
                          Game_Snake_RegionWidth+1, Game_Snake_RegionHigh+1, Game_Snake_RegionColor);
	
	//GUI_API_FillRect(hdc, 60, 260, 200, 240, COLOR_BLUE);
		
}

/*-------------------------------------------------------------------*/
void Game_Snake_DisplayScoreAndLevel(BU32 hdc)
{
    BU08 Score_str[5],Level_str[2];
	
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);

    if(Game_Snake_Score > 9990)
	{
		Game_Snake_Score=0;
    }
	
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		GUI_API_TextOut(hdc, Game_Snake_BackXPos+90, Game_Snake_BackYPos+280, "level:", 6, Game_Snake_TextColor, 0, 1);
	}
	else
	{
		GUI_API_TextOut(hdc, Game_Snake_BackXPos+90, Game_Snake_BackYPos+280, "等级:", 5, Game_Snake_TextColor, 0, 1);
	}
	GUI_API_AddDecStr(Game_Snake_Level, 2, Level_str);
    GUI_API_TextOut(hdc, Game_Snake_BackXPos+150, Game_Snake_BackYPos+282, Level_str, 2, Game_Snake_TextColor, Game_Snake_BackColor, 0);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		GUI_API_TextOut(hdc, Game_Snake_BackXPos+200, Game_Snake_BackYPos+280, "score:", 6, Game_Snake_TextColor, 0, 1);
	}
	else
	{
		GUI_API_TextOut(hdc, Game_Snake_BackXPos+200, Game_Snake_BackYPos+280, "得分:", 5, Game_Snake_TextColor, 0, 1);
	}    
	GUI_API_AddDecStr(Game_Snake_Score, 4, Score_str);
    GUI_API_TextOut(hdc, Game_Snake_BackXPos+260, Game_Snake_BackYPos+280, Score_str, 4, Game_Snake_TextColor, Game_Snake_BackColor, 0);

	

	
}

/*-------------------------------------------------------------------*/
void Game_Snake_DrawSnake(BU32 hdc , BU32 drawAll )
{
	int i;

	GUI_API_FillRect(hdc, Game_Snake_sSnake.x[0]*10+1+Game_Snake_RegionXPos, Game_Snake_sSnake.y[0]*10+1+Game_Snake_RegionYPos, 
		Game_Snake_Size-2, Game_Snake_Size-2,Game_Snake_SnakeHeadColor);
	if(drawAll)
	{
		for(i=1;i<Game_Snake_sSnake.Node-1;i++)
		{
		/*
			GUI_API_DrawCircle(hdc,Game_Snake_sSnake.x[i]*10+5+Game_Snake_RegionXPos, Game_Snake_sSnake.y[i]*10+5+Game_Snake_RegionYPos, 
				                   Game_Snake_Size/2-1,Game_Snake_SnakeColor);
			GUI_API_DrawCircle(hdc,Game_Snake_sSnake.x[i]*10+5+Game_Snake_RegionXPos, Game_Snake_sSnake.y[i]*10+5+Game_Snake_RegionYPos, 
				                   Game_Snake_Size/2-2,Game_Snake_SnakeColor);*/
			GUI_API_FillRect(hdc, Game_Snake_sSnake.x[i]*10+1+Game_Snake_RegionXPos, Game_Snake_sSnake.y[i]*10+1+Game_Snake_RegionYPos, 
				Game_Snake_Size-2, Game_Snake_Size-2,Game_Snake_SnakeColor);
				                   
		}
	}
	else
	{
		i = Game_Snake_sSnake.Node-1;	                   
		GUI_API_FillRect(hdc, Game_Snake_sSnake.x[i]*10+1+Game_Snake_RegionXPos, Game_Snake_sSnake.y[i]*10+1+Game_Snake_RegionYPos, 
		Game_Snake_Size-2, Game_Snake_Size-2,Game_Snake_RegionColor);

	}
	
}

/*-------------------------------------------------------------------*/
void Game_Snake_DrawFood(BU32 hdc)
{
    BU08 i;
	BU08 FoodAgain = 0;
    while(Game_Snake_sFood.yes)
    {    
        do
        {
            Game_Snake_sFood.x = (BS08)GUI_API_Random(36);
	        Game_Snake_sFood.y = (BS08)GUI_API_Random(24);
			
			for(i=0;i<Game_Snake_sSnake.Node;i++)//食物出现在蛇身上,则重新产生随机坐标.
			{
				if((Game_Snake_sFood.x == Game_Snake_sSnake.x[i])&&(Game_Snake_sFood.y == Game_Snake_sSnake.y[i]))
				{
                    FoodAgain = 1;
				    break;
				}
				else 
					FoodAgain = 0;
			}	
			
			if(!FoodAgain)
			{
				for(i=1;i<Game_Snake_sBomb.Num;i++)//食物出现在炸弹上,则重新产生随机坐标.
			    {
			        if((Game_Snake_sFood.x == Game_Snake_sBomb.x[i])&&(Game_Snake_sFood.y == Game_Snake_sBomb.y[i]))
			        {
					    FoodAgain = 1;
						break;
			        }
					else 
						FoodAgain = 0;
			    }
			}
			
		}while(FoodAgain);      

        Game_Snake_sFood.yes = 0;
    }
	/*
	GUI_API_DrawCircle(hdc,Game_Snake_sFood.x*10+5+Game_Snake_RegionXPos, Game_Snake_sFood.y*10+5+Game_Snake_RegionYPos,
		                   Game_Snake_Size/2-2,Game_Snake_FoodColor);
	GUI_API_DrawCircle(hdc,Game_Snake_sFood.x*10+5+Game_Snake_RegionXPos, Game_Snake_sFood.y*10+5+Game_Snake_RegionYPos,
		                   Game_Snake_Size/2-3,Game_Snake_FoodColor);*/
		                   
	GUI_API_FillRect(hdc,Game_Snake_sFood.x*10+1+Game_Snake_RegionXPos, Game_Snake_sFood.y*10+1+Game_Snake_RegionYPos,
		                  Game_Snake_Size-2, Game_Snake_Size-2,Game_Snake_FoodColor);

}

/*------------------------画游戏区域背景-------------------*/
void Game_Snake_DrawBackground(BU32 hdc)
{
    GUI_API_FillRect(hdc, Game_Snake_RegionXPos, Game_Snake_RegionYPos, 
		                Game_Snake_RegionWidth+1, Game_Snake_RegionHigh+1, Game_Snake_RegionColor);
	
}

/*--------------------------提示游戏结束 ---------------------------------*/
void Game_Snake_DisplayGameOver(BU32 hdc)
{
	BU32 len = GUI_API_Strlen(g_game_snake_str);
	GUI_API_TextOut(hdc, Game_Snake_RegionXPos+120+50-len*11/2, Game_Snake_RegionYPos+100, g_game_snake_str, len, Game_Snake_TextColor, Game_Snake_BackColor, 0);
}

/*--------------------------------- 蛇移动 ------------------------------------*/
void Game_Snake_SnakeMove(void)
{
	    BU08 i;
		BS08 SnakeHeadTempX, SnakeHeadTempY;  //保存蛇头的坐标
		
		SnakeHeadTempX = Game_Snake_sSnake.x[0];
		SnakeHeadTempY = Game_Snake_sSnake.y[0];
	
		switch(Game_Snake_sSnake.Direction)  //蛇头移动的方向
		{
			
		case 1:
			Game_Snake_sSnake.y[0]--;	   //向上
			if(Game_Snake_sSnake.y[0] < 0)     //如果撞到墙
			{
				Game_Snake_sSnake.y[0] = 0;     //不置0, 头会伸出墙外
				Game_Snake_sSnake.SnakeDead = 1;
				return;
			}
			break;
			 
		case 2:
			Game_Snake_sSnake.y[0]++;	   //向下
			if(Game_Snake_sSnake.y[0] >= 24)//如果撞到墙
			{
				Game_Snake_sSnake.y[0] = 23;
				Game_Snake_sSnake.SnakeDead = 1;
				return;
			}
			break;
		case 3:
			Game_Snake_sSnake.x[0]--;	   //向左
			if(Game_Snake_sSnake.x[0] < 0)//如果撞到墙
			{
				Game_Snake_sSnake.x[0] = 0;
				Game_Snake_sSnake.SnakeDead = 1;
				return;
			}
			break;
		case 4:
			Game_Snake_sSnake.x[0]++;	   //向右
			if(Game_Snake_sSnake.x[0] >= 36)//如果撞到墙
			{
				Game_Snake_sSnake.x[0] = 35;
				Game_Snake_sSnake.SnakeDead = 1;
				return;
			}
			break;
		default:
			break;
		}
			
		for(i=Game_Snake_sSnake.Node-1;i>1;i--)      //后一节用前一节的坐标
		{
			Game_Snake_sSnake.x[i] = Game_Snake_sSnake.x[i-1];
			Game_Snake_sSnake.y[i] = Game_Snake_sSnake.y[i-1];
		}
		Game_Snake_sSnake.x[1] = SnakeHeadTempX;
		Game_Snake_sSnake.y[1] = SnakeHeadTempY;

        for(i=Game_Snake_sSnake.Node-1;i>2;i--)      //判断蛇是否撞到自己
        {
        	if((Game_Snake_sSnake.x[i] == Game_Snake_sSnake.x[0])&&(Game_Snake_sSnake.y[i] == Game_Snake_sSnake.y[0]))
        	{
        		Game_Snake_sSnake.SnakeDead = 1;
        		return;
        	}
        	
        }
		
		for(i=1;i<=Game_Snake_sBomb.Num;i++)         //判断是否撞到炸弹
		{
			if((Game_Snake_sBomb.x[i] == Game_Snake_sSnake.x[0])&&(Game_Snake_sBomb.y[i] == Game_Snake_sSnake.y[0]))
			{
			    Game_Snake_sSnake.SnakeDead = 1;
				return;
			}
		}


}

/*------------------------------画障碍物--------------------------------*/

void Game_Snake_DrawBomb(BU32 hdc, BU32 bdrawAll)
{
    BU08 i=0;
	BU08 BombAgain = 0;
	

	if(bdrawAll)
	{
		for(i=1;i<=Game_Snake_sBomb.Num;i++)
		{
			GUI_API_FillRect(hdc, Game_Snake_sBomb.x[i]*10+1+Game_Snake_RegionXPos, Game_Snake_sBomb.y[i]*10+1+Game_Snake_RegionYPos,
			                  Game_Snake_Size-2, Game_Snake_Size-2,Game_Snake_BombColor);
		}

	}
	else
	{
	    while(!Game_Snake_sBomb.No)
	    {    
			Game_Snake_sBomb.Num++;
			if(Game_Snake_sBomb.Num >= Game_Snake_MAXNUM)Game_Snake_sBomb.Num=Game_Snake_MAXNUM;
				
	        do
	        {			
	            Game_Snake_sBomb.x[Game_Snake_sBomb.Num] = (BS08)GUI_API_Random(36);
		        Game_Snake_sBomb.y[Game_Snake_sBomb.Num] = (BS08)GUI_API_Random(24);
				for(i=0;i<Game_Snake_sSnake.Node;i++)
				{
					if((Game_Snake_sBomb.x[Game_Snake_sBomb.Num] == Game_Snake_sSnake.x[i])&&(Game_Snake_sBomb.y[Game_Snake_sBomb.Num] == Game_Snake_sSnake.y[i]))
	                {
	                    BombAgain = 1;
						break;
					}
					else BombAgain = 0;
				}	
				
			}while(BombAgain);      //炸弹出现在蛇或食物上,重新产生随机坐标.

	        Game_Snake_sBomb.No = 2;
	    }
		i = Game_Snake_sBomb.Num;
		if(i)
		GUI_API_FillRect(hdc, Game_Snake_sBomb.x[i]*10+1+Game_Snake_RegionXPos, Game_Snake_sBomb.y[i]*10+1+Game_Snake_RegionYPos,
		                  Game_Snake_Size-2, Game_Snake_Size-2,Game_Snake_BombColor);
	}
}

/*---------------------------------英雄榜--------------------------------------*/
void Game_Snake_DisplayHeros(BU32 hdc)
{
    /*
      BU08 i, Hero_str[5],Level_str[2];
	
	GUI_API_FillRect(hdc, 184, 256,50, 130, 69);

	GUI_API_TextOut(hdc, 60, 260, "英\n雄\n榜", 10, 244, 0, 1);

	for(i=1;i<=Game_Snake_MaxLevel;i++)
	{
		GUI_API_TextOut(hdc, 100, 240+i*24, "Level", 5, 19, 0, 1);
		GUI_API_AddDecStr(i, 2, Level_str);
		GUI_API_TextOut(hdc, 160, 240+i*24, Level_str, 2, 166, 0, 1);

		GUI_API_AddDecStr(Game_Snake_Heros[i], 4, Hero_str);
		GUI_API_TextOut(hdc, 200, 240+i*24, Hero_str, 4, 236, 69, 0);	
	}
     */

}

/*-----------------------------------------------------------------------------*/
void Game_Snake_DisplayCongratulation(BU32 hdc)
{
    BU08 Hero_str[5];
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
	    GUI_API_TextOut(hdc, 414, 270, "create a new record!", 20, 163, 0, 1);	
	    GUI_API_TextOut(hdc, 424, 300, "new record:", 11, 236, 0, 1);
	}
	else
	{
	    GUI_API_TextOut(hdc, 414, 270, "恭喜你破纪录!", 13, 163, 0, 1);	
	    GUI_API_TextOut(hdc, 424, 300, "新纪录:", 7, 236, 0, 1);
	}
	GUI_API_AddDecStr(Game_Snake_HeroScore, 4, Hero_str);
	GUI_API_TextOut(hdc, 508, 304, Hero_str, 4, 236, 69, 1);
}

