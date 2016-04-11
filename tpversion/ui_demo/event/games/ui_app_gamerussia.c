/*------------------------------------------------
file name:game_Russia.c
discription:main routine of Game_Russia
renewed time: 2009-3-5

------------------------------------------------------*/
/*-------------------------------------
  include files
 --------------------------------------*/
#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"
//#include "ui_app_game_russia.h"

/***************************************俄罗斯方块－主函数****************************************/

// 输入参数：	无
//
// 输出参数：	无
//
// 返 回 值：	无
//
// 备    注：	无
//
// 修改记录：   日      期		作      者		修定
/***********************************************************************************************************/

//#define  ID_ODC_Russia  ID_ODC_Teltris
//#define  ID_TIM_Russia  ID_TIM_Teltris

/***************************************常量定义*****************************************/

#define Game_Russia_UPGRADESCORE   3000
//#define Game_Russia_MaxSpeed 				        5 //10
#define Game_Russia_MaxLevel                        10
#define Game_Russia_BackWidth				        16
#define Game_Russia_BackHigh				        20
#define Game_Russia_LittleDiamondWidth  	        14

#define Game_Russia_xMaxNum        Game_Russia_BackWidth-4        //方块行最大个数        /*内框x=14*12,y=14*18*/
#define Game_Russia_yMaxNum        Game_Russia_BackHigh-2         //方块列最大个数

#define Game_Russia_Xpos                            270
#define	Game_Russia_Ypos                            130
#define	Game_Russia_RimWidth                        380
#define	Game_Russia_RimHeight                       310                          
#define Game_Russia_XDistance                       190

#if GUI_COLOR == GUI_COLOR_CLUT
	#define Game_Russia_DownBlockColor                  163//(0xa3)     //落下后方块颜色
	#define Game_Russia_RegionColor                     108//(0x6C)     //游戏区域背景颜色
	#define Game_Russia_RimColor                        164//(0xA4)     //游戏区域外诓颜色
	#define Game_Russia_RimLineColor                    45//(0x2D)      //游戏区域外诓边颜色
	#define Game_Russia_TextColor                       0//(0x00)       //文本颜色
	//七种方块的不同颜色和初始界面障碍颜色
	//{6,236,219,201,202,166,170};  //{0x06,0xEC,0xDB,0xC9,0xCA,0xA6,0xAA};  
	BU32   Game_Russia_BlockColorArray[7] = {1,1,1,1,1,1,1}; 

#elif GUI_COLOR == GUI_COLOR_1555
	#define Game_Russia_DownBlockColor                  RGB(184,216,232)//(0x00B8D8E8)         //落下后方块颜色
	#define Game_Russia_RegionColor                     RGB(142,184,232)//(0x008EB8E8)   //游戏区域背景颜色
	#define Game_Russia_RimColor                        RGB(200,232,248)//(0x00C8E8F8)   //游戏区域外诓颜色
	#define Game_Russia_RimLineColor                    RGB(106,118,200)//(0x006A76C8)    //游戏区域外诓边颜色
	#define Game_Russia_TextColor                       RGB(0,0,0)                       //文本颜色
	//七种方块的不同颜色和初始界面障碍颜色
	//BU08    Game_Russia_BlockColorArray[7] = {1,1,1,1,1,1,1};
	/*
	RGB(229,100,139) (0x00E5648B)
	RGB(37,24,13) (0x0025180D)
	RGB(72,209,22) (0x0048D116)
	RGB(200,216,216)(0x00C8D8D8)
	RGB(216,232,232)(0x00D8E8E8)
	RGB(83,184,232)(0x0053B8E8)
	RGB(67,197,248)(0x0043C5F8)
	*/
	BU32   Game_Russia_BlockColorArray[7] = {RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255)};

#elif GUI_COLOR == GUI_COLOR_565
	#define Game_Russia_DownBlockColor                  0xa3   //落下后方块颜色
	#define Game_Russia_RegionColor                     108   //游戏区域背景颜色
	#define Game_Russia_RimColor                        164    //游戏区域外诓颜色
	#define Game_Russia_RimLineColor                    45    //游戏区域外诓边颜色
	#define Game_Russia_TextColor                       0      //文本颜色
	//七种方块的不同颜色和初始界面障碍颜色
	//{6,236,219,201,202,166,170};  //{0x06,0xEC,0xDB,0xC9,0xCA,0xA6,0xAA};  
	BU32   Game_Russia_BlockColorArray[7] = {1,1,1,1,1,1,1}; 
		
#endif

/*
#define Game_Russia_BlockColor                      0xa3   //方块颜色
#define Game_Russia_NextBlockColor                  143   
#define Game_Russia_ClearBlockColor                 182  
*/


/************************************  数据结构声明*************************************/
typedef struct RussiaDiamondType
{
	BU08 	CurNum;
	BS08 	CurDir;
	BU32 	x;
	BU32	y;
}GAME_RUSSIA_DIAMOND_TYPE;

GAME_RUSSIA_DIAMOND_TYPE Game_Russia_NextDiamond= {0,0,60,155};
GAME_RUSSIA_DIAMOND_TYPE Game_Russia_ThisDiamond= {0,0,4,0};
GAME_RUSSIA_DIAMOND_TYPE Game_Russia_LastDiamond= {0,0,4,0};


/***************************************  变量定义****************************************/
BU08 Game_Russia_DiamondMode[7][4][4][4];
BU08 Game_Russia_BackgroundArray[Game_Russia_BackHigh][Game_Russia_BackWidth];

BU32    Game_Russia_Score = 0;
BU32    Game_Russia_TempScore = 0;
BU32	Game_Russia_HorizonStart = 300;
BU32	Game_Russia_VerticalStart = 150;


//BU08	RussiaSpeed = 1;
BU08	Game_Russia_LineNum = 0;
BU08	Game_Russia_Level = 1;
BU08    Game_Russia_StartFlag = false;
BU32    Game_Russia_PauseFlag = false;
BU32    Game_Russia_BlockColor = 1;

//BU08    change_flag = 0;
BU08    Game_Russia_Gridding = false;   //1:显示网格
BU08 g_game_russia_str[11];

void UI_APP_Game_Russia_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Russia);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);

}

void UI_APP_Game_Russia_Default(void)
{
	Game_Russia_StartFlag = false;
}

void UI_APP_Game_Russia_End(void)
{
	Game_Russia_PauseFlag = true; 

}
/***************************************  清上一个方块****************************************/

void Game_Russia_ClearLastDiamond(BU32 hdc)
{
	
	BU32 x,y;
	for(y=0;y<4;y++)
		for(x=0;x<4;x++)
			if( Game_Russia_DiamondMode[Game_Russia_LastDiamond.CurNum][Game_Russia_LastDiamond.CurDir][y][x]==1)
			{
				GUI_API_FillRect(hdc,Game_Russia_HorizonStart+((Game_Russia_LastDiamond.x+x)*Game_Russia_LittleDiamondWidth),
														Game_Russia_VerticalStart+((Game_Russia_LastDiamond.y+y)*Game_Russia_LittleDiamondWidth), 
														Game_Russia_LittleDiamondWidth, 
														Game_Russia_LittleDiamondWidth,Game_Russia_RegionColor);
				
			}

}
/**********************************************************************************************
				                          画方块图案
**********************************************************************************************/
void Game_Russia_DrawDiamond(BU32 hdc) 
{
	BU32 x,y;

	Game_Russia_BlockColor = Game_Russia_BlockColorArray[Game_Russia_ThisDiamond.CurNum];

	
	for(y=0;y<4;y++)
		for(x=0;x<4;x++)
			if( Game_Russia_DiamondMode[Game_Russia_ThisDiamond.CurNum][Game_Russia_ThisDiamond.CurDir][y][x]==1)
			{
				GUI_API_FillRect(hdc,Game_Russia_HorizonStart+((Game_Russia_ThisDiamond.x+x)*Game_Russia_LittleDiamondWidth),
														Game_Russia_VerticalStart+((Game_Russia_ThisDiamond.y+y)*Game_Russia_LittleDiamondWidth), 
														Game_Russia_LittleDiamondWidth, 
														Game_Russia_LittleDiamondWidth,Game_Russia_BlockColor);
				GUI_API_FillRect(hdc,Game_Russia_HorizonStart+((Game_Russia_ThisDiamond.x+x)*Game_Russia_LittleDiamondWidth+6),
														Game_Russia_VerticalStart+((Game_Russia_ThisDiamond.y+y)*Game_Russia_LittleDiamondWidth+6), 
														2, 
														2,Game_Russia_TextColor);

				GUI_API_DrawRectangle(hdc,Game_Russia_HorizonStart+((Game_Russia_ThisDiamond.x+x)*Game_Russia_LittleDiamondWidth),
					                      Game_Russia_VerticalStart+((Game_Russia_ThisDiamond.y+y)*Game_Russia_LittleDiamondWidth),
					                      Game_Russia_LittleDiamondWidth-1,
					                      Game_Russia_LittleDiamondWidth-1,0);
				
			}
		/*	GUI_API_DrawStyle(hdc,iTetrisColStart+((stTetrisThisDiamond.x+x)*Game_Russia_LittleDiamondWidth)+XDISTANCE,
														iTetrisRowStart+((stTetrisThisDiamond.y+y)*Game_Russia_LittleDiamondWidth), 
														STYLE_GAMEBOX_W20_H20);*/

}

/**********************************************************************************************
				                          画下一个方块图案
**********************************************************************************************/
void Game_Russia_DrawNextDiamond(BU32 hdc) 
{
	BU32 x,y;
	
	Game_Russia_BlockColor = Game_Russia_BlockColorArray[Game_Russia_NextDiamond.CurNum];
	
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(Game_Russia_DiamondMode[Game_Russia_NextDiamond.CurNum][Game_Russia_NextDiamond.CurDir][y][x]==1)
			{
				GUI_API_FillRect(hdc,Game_Russia_NextDiamond.x+(x*Game_Russia_LittleDiamondWidth)+315+Game_Russia_XDistance,
													Game_Russia_NextDiamond.y+(y*Game_Russia_LittleDiamondWidth),
														Game_Russia_LittleDiamondWidth,
														Game_Russia_LittleDiamondWidth,Game_Russia_BlockColor);
				GUI_API_FillRect(hdc,Game_Russia_NextDiamond.x+(x*Game_Russia_LittleDiamondWidth)+315+Game_Russia_XDistance+6,
													Game_Russia_NextDiamond.y+(y*Game_Russia_LittleDiamondWidth+6),
														2,
														2,Game_Russia_TextColor);
				GUI_API_DrawRectangle(hdc,Game_Russia_NextDiamond.x+(x*Game_Russia_LittleDiamondWidth)+315+Game_Russia_XDistance,
					                      Game_Russia_NextDiamond.y+(y*Game_Russia_LittleDiamondWidth),
					                      Game_Russia_LittleDiamondWidth-1,
					                      Game_Russia_LittleDiamondWidth-1,0);
			}

			else 
			{
				GUI_API_FillRect(hdc,Game_Russia_NextDiamond.x+(x*Game_Russia_LittleDiamondWidth)+315+Game_Russia_XDistance,
															Game_Russia_NextDiamond.y+(y*Game_Russia_LittleDiamondWidth),
															Game_Russia_LittleDiamondWidth,
															Game_Russia_LittleDiamondWidth,Game_Russia_RegionColor);
			/*	GUI_API_DrawRectangle(hdc,Game_Russia_NextDiamond.x+(x*Game_Russia_LittleDiamondWidth)+315+Game_Russia_XDistance,
					                      Game_Russia_NextDiamond.y+(y*Game_Russia_LittleDiamondWidth),
					                      Game_Russia_LittleDiamondWidth-1,
					                      Game_Russia_LittleDiamondWidth-1,0);*/

			}
		}
	}
}
/**********************************************************************************************
				                              画背景图案
**********************************************************************************************/
void Game_Russia_DrawBackground(BU32 hdc) 
{
	BU32 x,y;

	for(y=1;y<=Game_Russia_yMaxNum;y++)
		for(x=2;x<Game_Russia_xMaxNum+2;x++)
		{
			if(Game_Russia_BackgroundArray[y][x]==0)
			{
				GUI_API_FillRect(hdc,Game_Russia_HorizonStart+((x-2)*Game_Russia_LittleDiamondWidth),
															Game_Russia_VerticalStart+((y-1)*Game_Russia_LittleDiamondWidth),
															Game_Russia_LittleDiamondWidth,
															Game_Russia_LittleDiamondWidth,Game_Russia_RegionColor);
				if(Game_Russia_Gridding == true)
			        GUI_API_DrawRectangle(hdc,Game_Russia_HorizonStart+((x-2)*Game_Russia_LittleDiamondWidth),
             									  Game_Russia_VerticalStart+((y-1)*Game_Russia_LittleDiamondWidth),
             									  Game_Russia_LittleDiamondWidth,
             									  Game_Russia_LittleDiamondWidth,0);
			}

			else if(Game_Russia_BackgroundArray[y][x] >=2)
			{

            	Game_Russia_BlockColor = Game_Russia_BlockColorArray[Game_Russia_BackgroundArray[y][x]-2];

			    GUI_API_FillRect(hdc,Game_Russia_HorizonStart+((x-2)*Game_Russia_LittleDiamondWidth),
														Game_Russia_VerticalStart+((y-1)*Game_Russia_LittleDiamondWidth),
														Game_Russia_LittleDiamondWidth,
													Game_Russia_LittleDiamondWidth,Game_Russia_BlockColor);
			    GUI_API_FillRect(hdc,Game_Russia_HorizonStart+((x-2)*Game_Russia_LittleDiamondWidth+6),
														Game_Russia_VerticalStart+((y-1)*Game_Russia_LittleDiamondWidth+6),
														2,
													2,Game_Russia_TextColor);
				GUI_API_DrawRectangle(hdc,Game_Russia_HorizonStart+((x-2)*Game_Russia_LittleDiamondWidth),
									  Game_Russia_VerticalStart+((y-1)*Game_Russia_LittleDiamondWidth),
									  Game_Russia_LittleDiamondWidth-1,
									  Game_Russia_LittleDiamondWidth-1,0);
			}
		}

	

}
/**********************************************************************************************
			                    	      判断方块是否遇到障碍
**********************************************************************************************/
BU08 Game_Russia_JudgeDiamondIfMeetObstacle(BU32 X,BU32 Y) 
{
	BU32 x,y;

	for(y=0;y<4;y++)
		for(x=0;x<4;x++)

	if( Game_Russia_DiamondMode[Game_Russia_ThisDiamond.CurNum][Game_Russia_ThisDiamond.CurDir][y][x]==1 && Game_Russia_BackgroundArray[Y+y+1][X+x+2]!=0 )

		return 1;
	return 0;
}
/**********************************************************************************************
				                          将方块放入背景数组
**********************************************************************************************/
void Game_Russia_DiamondIntoBackgroundArray(void)
{
	BU08 x,y;

	for(y=0;y<4;y++)
		for(x=0;x<4;x++)
			if(Game_Russia_DiamondMode[Game_Russia_ThisDiamond.CurNum][Game_Russia_ThisDiamond.CurDir][y][x]==1)
				Game_Russia_BackgroundArray[1+Game_Russia_ThisDiamond.y-1+y][2+Game_Russia_ThisDiamond.x+x]=Game_Russia_ThisDiamond.CurNum+2;

}
/**********************************************************************************************
			                       	        画游戏背景
**********************************************************************************************/
void Game_Russia_BackgroundRim(BU32 hdc)
{
	GUI_API_FillRect(hdc,Game_Russia_Xpos,Game_Russia_Ypos-8,Game_Russia_RimWidth,Game_Russia_RimHeight,Game_Russia_RimLineColor);
	GUI_API_FillRect(hdc,Game_Russia_Xpos+8,Game_Russia_Ypos,Game_Russia_RimWidth-16,Game_Russia_RimHeight-16,Game_Russia_RimColor);
	GUI_API_FillRect(hdc,Game_Russia_HorizonStart-10,Game_Russia_VerticalStart+10,
					Game_Russia_LittleDiamondWidth*(Game_Russia_xMaxNum)+20,Game_Russia_LittleDiamondWidth*(Game_Russia_yMaxNum),Game_Russia_RimLineColor);
                          
}
/**********************************************************************************************
			            	               显示游戏等级和分数
**********************************************************************************************/
void Game_Russia_DisplayGameLevel( BU32 hdc)
{
	BU08 LevelStr[3],ScoreStr[5];
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		GUI_API_TextOut(hdc,490,170,"Next:",4,Game_Russia_TextColor,0,1);
		GUI_API_TextOut(hdc,495,270,"Score:",5,Game_Russia_TextColor,0,1);
		GUI_API_TextOut(hdc,495,360,"Level:",5,Game_Russia_TextColor,0,1);
	}
	else
	{
		GUI_API_TextOut(hdc,490,170,"下一个:",6,Game_Russia_TextColor,0,1);
		GUI_API_TextOut(hdc,495,270,"得分:",4,Game_Russia_TextColor,0,1);
		GUI_API_TextOut(hdc,495,360,"级别:",4,Game_Russia_TextColor,0,1);
	}

	GUI_API_AddDecStr(Game_Russia_Level, 2, LevelStr);
	GUI_API_TextOut(hdc,570,362,LevelStr,2,Game_Russia_TextColor,Game_Russia_RegionColor,0);
	
	if(Game_Russia_Score > 9999)	Game_Russia_Score = 0;
	GUI_API_AddDecStr(Game_Russia_Score, 4, ScoreStr);
	GUI_API_TextOut(hdc,570,272,ScoreStr,4,Game_Russia_TextColor,Game_Russia_RegionColor,0);

}

/**********************************************************************************************
				                           初始化游戏
**********************************************************************************************/
void Game_Russia_InitializeGame(void) 
{
	BU08 i,j;

	Game_Russia_Level = 1;
	Game_Russia_Score = 0;
	Game_Russia_TempScore = 0;
	Game_Russia_PauseFlag = false;
	Game_Russia_Gridding = false;
	
	for(i=0;i<Game_Russia_BackHigh;i++) 
		for(j=0;j<Game_Russia_BackWidth;j++) 
			Game_Russia_BackgroundArray[i][j]=1;

	for(i=1;i<Game_Russia_BackHigh-1;i++) 
		for(j=2;j<Game_Russia_BackWidth-2;j++) 
			Game_Russia_BackgroundArray[i][j]=0;

	Game_Russia_NextDiamond.CurNum=(BU08)GUI_API_Random(7);
	Game_Russia_NextDiamond.CurDir=(BU08)GUI_API_Random(4);

	Game_Russia_ThisDiamond.CurNum=Game_Russia_NextDiamond.CurNum;
	Game_Russia_ThisDiamond.CurDir=Game_Russia_NextDiamond.CurDir;

	Game_Russia_ThisDiamond.x=4;
	Game_Russia_ThisDiamond.y=0;
	
	//GUI_ENC_API_Update(ID_ODC_Russia, 3);//画下一个--方块图案
}

/**********************************************************************************************
			                          	判断方块是否成行
**********************************************************************************************/
BU08 Game_Russia_JudgeDiamondIfBecomeLine(void) 
{
	BU32 y=Game_Russia_yMaxNum,x,y2,s;

	while(y>=1)
	{
		s=0;

		for(x=2;x<=Game_Russia_BackWidth-3;x++) 
		{
			if(Game_Russia_BackgroundArray[y][x]>0) s+=1;
		}
			
		if(s==Game_Russia_BackWidth-4)
		{
			Game_Russia_LineNum++;
			for(y2=y;y2>=1;y2--)
				for(x=2;x<=Game_Russia_BackWidth-3;x++)
					{
						if(y2==1)
						{
							Game_Russia_BackgroundArray[y2][x]=0; 
						}							
						else 
						{
							Game_Russia_BackgroundArray[y2][x]=Game_Russia_BackgroundArray[y2-1][x];
						}							
					}
			return 1;
		}
		else 
			--y;
	}
	

	if(Game_Russia_LineNum) /*计算分数*/
	{
		switch(Game_Russia_LineNum)
		{
			case 1:Game_Russia_Score+=100;
			    break;
			case 2:Game_Russia_Score+=300;
				break;
			case 3:Game_Russia_Score+=600;
				break;
			case 4:Game_Russia_Score+=1000;
			default:
				break;
		}
        
		if( (Game_Russia_Score - Game_Russia_TempScore) >= Game_Russia_UPGRADESCORE)
		{		
			//RussiaSpeed++;
			Game_Russia_Level++;
			if(Game_Russia_Level > Game_Russia_MaxLevel)Game_Russia_Level = Game_Russia_MaxLevel; 		
			
			Game_Russia_TempScore += Game_Russia_UPGRADESCORE;
			
			if(Game_Russia_Level <= 4)				
		        GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PERIOD,(600-(BU32)Game_Russia_Level*100),0);
		    else 
			    GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PERIOD,(200-(BU32)(Game_Russia_Level-4)*20),0);
		}	
		
	}

	return 0;

}
/******************************************************************
                                  游戏是否结束 //lx
******************************************************************/
BU08 Game_Russia_GameOver(void)
{

	BU08 x = Game_Russia_xMaxNum+1, y=2;
	
	while(x>=2)
	{
	   	if(Game_Russia_BackgroundArray[y][x]>0) return 1;	
		x--;		
	}

	return 0;

}
/*************************************************************************
                       提示游戏结束 //lx
**************************************************************************/
void Game_Russia_DisplayOver(BU32 hdc) 
{
	BU32 len = GUI_API_Strlen(g_game_russia_str);
	GUI_API_FillRect(hdc,Game_Russia_HorizonStart+20,Game_Russia_VerticalStart+80,130,60,Game_Russia_RimColor);
	GUI_API_TextOut(hdc,Game_Russia_HorizonStart+20+65-len*11/2,Game_Russia_VerticalStart+100,g_game_russia_str,len,0,1,1);

}
/*************************************************************************
                       提示是否继续游戏//lx
**************************************************************************/

void Game_Russia_DisplayContinue(BU32 hdc)
{
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	GUI_API_FillRect(hdc,Game_Russia_HorizonStart+20,Game_Russia_VerticalStart+80,130,60,1);
	
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		GUI_API_TextOut(hdc,Game_Russia_HorizonStart+40,Game_Russia_VerticalStart+100,"continue?",9,1,Game_Russia_RimColor,0);
	}
	else
	{
		GUI_API_TextOut(hdc,Game_Russia_HorizonStart+40,Game_Russia_VerticalStart+100,"继续游戏?",9,1,Game_Russia_RimColor,0);
	}

}
/***********************************************************************
                         不同 等级初始化
***********************************************************************/
void Game_Russia_LevelView(void)
{/*
    BU08 x,y;
   	for(y=Game_Russia_BackHigh-2;y>Game_Russia_BackHigh-2-(Game_Russia_Level/2);y--)
   		for(x=2+y%2;x<Game_Russia_BackWidth-2;x+=2)
   			Game_Russia_BackgroundArray[y][x] = 9;
*/			
}

			    
/**********************************************************************************************
			               				全局函数
**********************************************************************************************/
		
	
	void Game_Russia_OnPaint(BU32 hdc,BU32 lparam)
	{
		if(1 == lparam)//初始化
		{
			Game_Russia_BackgroundRim(hdc);//画游戏背景外框
			
			if(false==Game_Russia_StartFlag)
			{				
				Game_Russia_InitializeGame();//初始化游戏
				Game_Russia_DrawBackground(hdc);//画背景图案
			}
			else
			{
				Game_Russia_DrawBackground(hdc);//画背景图案
				Game_Russia_DisplayContinue(hdc);/* 提示是否继续游?*/
                Game_Russia_PauseFlag =true;     /*   预约时间到提示后返回*/
				                                   
			}
			Game_Russia_DisplayGameLevel(hdc);/*显示游戏分数与等级*/

			Game_Russia_DrawNextDiamond(hdc); //下一个方块
		}
	
		else if(2 == lparam)
		{
			Game_Russia_DisplayGameLevel(hdc);//	显示游戏等级和分数
		}
	
		else if(3 == lparam)
		{
			Game_Russia_DrawNextDiamond(hdc); //画下一个--方块图案
		}
		else if(4 == lparam)
		{
			Game_Russia_DrawBackground(hdc);//画背景--图案
		}
	
		else if(5 == lparam)
		{
			Game_Russia_DrawDiamond(hdc); //画方块图案
		}
		else if(6 == lparam)
		{
		    Game_Russia_DisplayOver(hdc); //提示游戏结束
		}
		else if(7 == lparam)
		{
			Game_Russia_DisplayContinue(hdc);// 提示是否继续游戏
		}
		else if(8== lparam)
		{
			Game_Russia_ClearLastDiamond(hdc);
		}
		
	
	}
/***********************************************************************
                                          按键处理函数
**************************************************************************/
static BU32 g_game_match;
void Game_Russia_OnKey(BU32 key)
{
	BU32 lang;

	if(GUI_VK_0 == key)
	{
		g_game_match = 1;
	}
	else if(g_game_match == 1 && GUI_VK_5 == key)
	{
		g_game_match = 2;
	}
	else if(g_game_match == 2 && GUI_VK_9 == key)
	{
		g_game_match = 3;
	}
	else if(g_game_match == 3 && GUI_VK_1 == key)
	{
		Game_Russia_Score = Game_Russia_Score+1000;
	}
	else
	{
		g_game_match = 0;
	}
	
	if(key == GUI_VK_SELECT)			 //确认游戏开始
	{
		Game_Russia_StartFlag = true;
		Game_Russia_PauseFlag = false;
		if(Game_Russia_GameOver())   /*lx*/
		{
             Game_Russia_InitializeGame(); 
		}
		//Game_Russia_LevelView();//等级
		//GUI_ENC_API_Update(ID_ODC_Russia,2);
		
		if(Game_Russia_Level <=4)					 
			 GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PERIOD,(600-(BU32)Game_Russia_Level*100),0);
		else 
			 GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PERIOD,(200-(BU32)(Game_Russia_Level-4)*20),0);
		GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PLAY,0,0);
		GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景图案
		GUI_ENC_API_Update(ID_ODC_Russia, 5);   //画方块图案
		
	}
    else if(Game_Russia_GameOver()&& key == GUI_VK_RED)  //重玩 /*lx*/
	{	
		GUI_ENC_API_Update(ID_ODC_Russia, 1);
	}

	else if(!Game_Russia_StartFlag)				   //选择等级
	{
		if(key == GUI_VK_RIGHT)
		{
			Game_Russia_Level++;
//				RussiaSpeed++;
			if(Game_Russia_Level >Game_Russia_MaxLevel)
			{
				Game_Russia_Level =1;
//					RussiaSpeed = 1;
			}
			
			
			
		}
		else if(key == GUI_VK_LEFT)
	    {
			Game_Russia_Level--;
//				RussiaSpeed--;
			if(Game_Russia_Level == 0)
			{
				Game_Russia_Level = 10;
//					RussiaSpeed = 10;
			}				
		}
/*
		else if(key == GUI_VK_BLUE)
		{
			if(Game_Russia_Gridding == false)
				Game_Russia_Gridding = true;
			else Game_Russia_Gridding = false;
			GUI_ENC_API_Update(ID_ODC_Russia, 4);    //显示网格
		}

	   if(Game_Russia_Level <=4)					
	        GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PERIOD,(600-(BU32)Game_Russia_Level*100),0);
	   else 
		    GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PERIOD,(200-(BU32)(Game_Russia_Level-4)*20),0);
*/
		
	}
	else if(key == GUI_VK_BLUE)
	{
		if(Game_Russia_Gridding == false)
			Game_Russia_Gridding = true;
		else Game_Russia_Gridding = false;
		GUI_ENC_API_Update(ID_ODC_Russia, 4);    //显示网格
	}


	GUI_ENC_API_Update(ID_ODC_Russia, 2);		//显示游戏等级和分数

	if(Game_Russia_StartFlag && (!Game_Russia_PauseFlag))
	{

		if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y))
		{
			Game_Russia_StartFlag = false;
			GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
			return;
		}
		
		//3.5 GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景图案
		//3.5 GUI_ENC_API_Update(ID_ODC_Russia, 5);   //画方块图案
		GUI_API_Memcpy(&Game_Russia_LastDiamond, &Game_Russia_ThisDiamond, sizeof(GAME_RUSSIA_DIAMOND_TYPE));

		switch( key )
		{

			case GUI_VK_LEFT: 
				/*方块左移*/
				if(!Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x-1,Game_Russia_ThisDiamond.y))
				{
					Game_Russia_ThisDiamond.x--;
				}											
				break;

			case GUI_VK_RIGHT: 
				/*方块右移*/
				if(!Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x+1,Game_Russia_ThisDiamond.y))
				{
					Game_Russia_ThisDiamond.x++;
				}
				break;

			case GUI_VK_UP: 
				/*向右旋转方块*/
			
				if(Game_Russia_ThisDiamond.CurNum>0)
				{
					if(Game_Russia_ThisDiamond.CurDir==1||Game_Russia_ThisDiamond.CurDir==3)
					{
						if(++Game_Russia_ThisDiamond.CurDir>3)Game_Russia_ThisDiamond.CurDir=0;

						if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y))
						{
							if(!Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x-1,Game_Russia_ThisDiamond.y)) --Game_Russia_ThisDiamond.x;
							else if(--Game_Russia_ThisDiamond.CurDir<0) Game_Russia_ThisDiamond.CurDir=3;
						}	
					}
					else
					{
						if(++Game_Russia_ThisDiamond.CurDir>3) Game_Russia_ThisDiamond.CurDir=0;

						if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y))
						if(--Game_Russia_ThisDiamond.CurDir<0) Game_Russia_ThisDiamond.CurDir=3;
					}
				}
				else
				{
					if(++Game_Russia_ThisDiamond.CurDir>3) Game_Russia_ThisDiamond.CurDir=0;

					if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y))
					if(--Game_Russia_ThisDiamond.CurDir<0) Game_Russia_ThisDiamond.CurDir=3;
				}
				break;

			case GUI_VK_DOWN: 
				/*方块下移
				iTetrisScrCnt=GAME_TETRIS_MAX_SPEED; */
				{
					while(!(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y)))
					{		
						Game_Russia_ThisDiamond.y++;						
					}
					Game_Russia_ThisDiamond.y--;
				}
				break;
/*
			case GUI_VK_RED:

				GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);				
				GUI_ENC_API_Update(ID_ODC_Russia, 1);//重新开始
				break;


			case GUI_VK_BLUE:

				change_flag++;
				if(change_flag == 3)
				{
					Game_Russia_Score +=500;
					change_flag = 0;
				}
				break;
*/

			case GUI_VK_EXIT:					
				//GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
				Game_Russia_StartFlag = false;
				break;
			default:
				break;						   


		} 
		//3.5 GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景图案

		//stTetrisThisDiamond.y++;	
		if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y))
		{		   
			Game_Russia_DiamondIntoBackgroundArray();
			Game_Russia_LineNum=0;
			while(Game_Russia_JudgeDiamondIfBecomeLine());
			Game_Russia_ThisDiamond.y=0;
			Game_Russia_ThisDiamond.x=4;

			Game_Russia_ThisDiamond.CurNum=Game_Russia_NextDiamond.CurNum;
			Game_Russia_ThisDiamond.CurDir=Game_Russia_NextDiamond.CurDir;

			Game_Russia_NextDiamond.CurNum=(BU08)GUI_API_Random(7);
			Game_Russia_NextDiamond.CurDir=(BU08)GUI_API_Random(4);

			GUI_ENC_API_Update(ID_ODC_Russia, 3);	  //画下一个方块图案

			if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y)) 
			{
				GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景--图案
				GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
				Game_Russia_StartFlag = false;
			}
		}
		else
		{
			GUI_ENC_API_Update(ID_ODC_Russia, 8);	//清方块上一个位置
		}
		GUI_ENC_API_Update(ID_ODC_Russia, 5);   //画方块图案
		GUI_ENC_API_Update(ID_ODC_Russia, 2);	//显示游戏等级和分数

		
	}
	

	if(Game_Russia_StartFlag&&(key == GUI_VK_RED))  //暂停
	{
		if(Game_Russia_PauseFlag == false)
		{
			Game_Russia_PauseFlag = true; 
			GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
			APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
			if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
			{
				GUI_API_Strcpy(g_game_russia_str, "PAUSE");
			}
			else
			{
				GUI_API_Strcpy(g_game_russia_str, "暂停");
			}
			GUI_ENC_API_Update(ID_ODC_Russia,6);  //提示游戏暂停
			return;
		}
		else 
		{
			Game_Russia_PauseFlag = false;
			GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_PLAY,0,0);
			GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景图案
			GUI_ENC_API_Update(ID_ODC_Russia, 5);   //画方块图案
		}
	}


	
}

void Game_Russia_Timing(void)
{
	BU32 lang;
	

	GUI_API_Memcpy(&Game_Russia_LastDiamond, &Game_Russia_ThisDiamond, sizeof(GAME_RUSSIA_DIAMOND_TYPE));
	Game_Russia_ThisDiamond.y++;
	if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y))
		{		   
			Game_Russia_DiamondIntoBackgroundArray();
			Game_Russia_LineNum=0;
			while(Game_Russia_JudgeDiamondIfBecomeLine());
			if(Game_Russia_GameOver())
			{
				APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
				if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
				{
					GUI_API_Strcpy(g_game_russia_str, "GAME OVER");
				}
				else
				{
					GUI_API_Strcpy(g_game_russia_str, "游戏结束");
				}
			    GUI_ENC_API_Update(ID_ODC_Russia,6);  //提示游戏结束
			    GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
				Game_Russia_StartFlag = false;
				return;
			}
			Game_Russia_ThisDiamond.y=0;
			Game_Russia_ThisDiamond.x=4;

			Game_Russia_ThisDiamond.CurNum=Game_Russia_NextDiamond.CurNum;
			Game_Russia_ThisDiamond.CurDir=Game_Russia_NextDiamond.CurDir;

			GUI_API_Memcpy(&Game_Russia_LastDiamond, &Game_Russia_ThisDiamond, sizeof(GAME_RUSSIA_DIAMOND_TYPE));

			Game_Russia_NextDiamond.CurNum=(BU08)GUI_API_Random(7);
			Game_Russia_NextDiamond.CurDir=(BU08)GUI_API_Random(4);
            
			GUI_ENC_API_Update(ID_ODC_Russia, 3);	   //画下一个方块图案

			if(Game_Russia_JudgeDiamondIfMeetObstacle(Game_Russia_ThisDiamond.x,Game_Russia_ThisDiamond.y)) 
			{
				GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景--图案
				GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
				Game_Russia_StartFlag = false;
			}
			
		 	GUI_ENC_API_Update(ID_ODC_Russia, 4);	//画背景图案
		}
		else
		{
			GUI_ENC_API_Update(ID_ODC_Russia, 8);	//清方块上一个位置

		}
		GUI_ENC_API_Update(ID_ODC_Russia, 5);   //画方块图案
		GUI_ENC_API_Update(ID_ODC_Russia, 2);	//显示游戏等级和分数

	
}
/*
void Russia_End(void)
{	
	//Game_Russia_PauseFlag = true; 
	GUI_ENC_API_SetValue(ID_TIM_Russia,WDT_TIM_SV_STOP,0,0);
	
}
*/
BU08	Game_Russia_BackgroundArray[Game_Russia_BackHigh][Game_Russia_BackWidth]; /*游戏背景数组*/
BU08 	Game_Russia_DiamondMode[7][4][4][4]= /*存储方块的样式*/
{
	1,1,1,1,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	

	1,1,1,1,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,

	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,

/************/

	1,1,1,0,
	0,1,0,0,
	0,0,0,0,
	0,0,0,0,
	
	0,1,0,0,
	1,1,0,0,
	0,1,0,0,
	0,0,0,0,
	
	0,1,0,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0,
	
	1,0,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0,
	
/************/
	1,1,1,0,
	1,0,0,0,
	0,0,0,0,
	0,0,0,0,

	1,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0,
	
	0,0,1,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0,
	
	1,0,0,0,
	1,0,0,0,
	1,1,0,0,
	0,0,0,0,
	
/************/
	1,1,1,0,
	0,0,1,0,
	0,0,0,0,
	0,0,0,0,
	
	0,1,0,0,
	0,1,0,0,
	1,1,0,0,
	0,0,0,0,
	
	1,0,0,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0,
	
	1,1,0,0,
	1,0,0,0,
	1,0,0,0,
	0,0,0,0,
	
/************/
	1,1,0,0,
	0,1,1,0,
	0,0,0,0,
	0,0,0,0,
	
	0,1,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0,

	1,1,0,0,
	0,1,1,0,
	0,0,0,0,
	0,0,0,0,
	
	0,1,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0,
	
/************/
	0,1,1,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
	
	1,0,0,0,
	1,1,0,0,
	0,1,0,0,
	0,0,0,0,
	
	0,1,1,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
	
	1,0,0,0,
	1,1,0,0,
	0,1,0,0,
	0,0,0,0,
	
/***************/
	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
	
	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
	
	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,

	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
/*-----------------------------------------------------------------------------*/
	/****************/
/*	1,0,0,0,
	1,0,0,0,
	0,0,0,0,
	0,0,0,0,
	
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	
	1,0,0,0,
	1,0,0,0,
	0,0,0,0,
	0,0,0,0,

	1,1,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,

	/***************/
/*	1,0,1,0,
	1,0,1,0,
	1,1,1,0,
	0,0,0,0,
	
	1,1,1,0,
	1,0,0,0,
	1,1,1,0,
	0,0,0,0,
	
	1,1,1,0,
	1,0,1,0,
	1,0,1,0,
	0,0,0,0,

	1,1,1,0,
	0,0,1,0,
	1,1,1,0,
	0,0,0,0,
  */
};

