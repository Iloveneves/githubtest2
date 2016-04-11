/*------------------------------------------------
file name:game_box.c
discription:main routine of Game_Box
renewed time: 2009-3-5

------------------------------------------------------*/

/*----------------------------------------------------
    include files
----------------------------------------------------*/
#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"
//#include "ui_app_game_box.h"

//#define  ID_ODC_Box  ID_ODC_Sokoban
#define STYLE_GameMan_W20_H20 STYLE_BoxMan_W20_H20
#define STYLE_GameBox_W20_H20 STYLE_Box_W20_H20

/***************************************  常量定义****************************************/

#define  Game_Box_NullRegion     	      0    //0 : 表示游戏区域以外的空地
#define	 Game_Box_Wall				      1    //1 //: 表示障碍
#define	 Game_Box_Box				      2    //2 //: 表示箱子
#define	 Game_Box_Termini			      3    //3 //: 表示目的地
#define	 Game_Box_GameRegion		      4    //4 //: 表示游戏区域内的空地
#define	 Game_Box_Man			          5    //5 : 表示搬运工所在的位置
#define	 Game_Box_MaxLevel			      15

#if GUI_COLOR == GUI_COLOR_CLUT
	#define Game_Box_NullboxRegionColor     
	#define Game_Box_WallColor              50//0x32        //障碍颜色
	#define Game_Box_BoxColor               100//0x64       //箱子颜色
	#define Game_Box_TerminiColor           212//0xD4       //目的地颜色            
	#define Game_Box_RegionColor            1  //0x01     	//游戏区域颜色
	#define Game_Box_ManColor               228//0xE4    	//搬运工颜色
	#define Game_Box_ManBKColor             1  //0x01      	//搬运工背景    
	#define Game_Box_BoxBKColor             164//0xA4       //游戏区域外部颜色     
	#define Game_Box_TextColor				0  //0x00	    //文本显示颜色	

#elif GUI_COLOR == GUI_COLOR_1555
	#define Game_Box_NullboxRegionColor     
	#define Game_Box_WallColor              RGB(88,104,200)//0x005868C8       //障碍颜色
	#define Game_Box_BoxColor               RGB(24,104,200)//0x1868C8         //箱子颜色
	#define Game_Box_TerminiColor           RGB(84,165,107)//0x54A56B         //目的地颜色            
	#define Game_Box_RegionColor            RGB(255,255,255)//(0x00FFFFFF)    //游戏区域颜色
	#define Game_Box_ManColor               RGB(229,175,75)//0x00E5AF4B   	  //搬运工颜色
	#define Game_Box_ManBKColor             RGB(255,255,255)//(0x00FFFFFF)    //搬运工背景    
	#define Game_Box_BoxBKColor             RGB(200,232,248)//(0x00C8E8F8)    //游戏区域外部颜色     
	#define Game_Box_TextColor				RGB(0,0,0)                        //文本显示颜色

#elif GUI_COLOR == GUI_COLOR_565
	#define Game_Box_NullboxRegionColor     
	#define Game_Box_WallColor              50          //障碍颜色
	#define Game_Box_BoxColor               100         //箱子颜色
	#define Game_Box_TerminiColor           212         //目的地颜色            
	#define Game_Box_RegionColor            1       	//游戏区域颜色
	#define Game_Box_ManColor               228    	    //搬运工颜色
	#define Game_Box_ManBKColor             1        	//搬运工背景    
	#define Game_Box_BoxBKColor             164         //游戏区域外部颜色     
	#define Game_Box_TextColor				0			//文本显示颜色	
		
#endif

/*yy*/
#define	Game_Box_Xpos			          70+200
#define	Game_Box_Ypos			          130
#define	Game_Box_Width			          20
#define	Game_Box_Height			          20
#define Game_Box_BKWidth                   440
#define Game_Box_BKHeight                  350	

/***************************************  变量定义****************************************/

BU08		 Game_Box_CurrXPos ; 	//搬运工当前坐标
BU08		 Game_Box_CurrYPos ;	

BU08         Game_Box_TempBx;    //before
BU08         Game_Box_TempBy;
BU08         Game_Box_TempAx;    //after
BU08         Game_Box_TempAy;

BU08		 Game_Box_xTemp;
BU08         Game_Box_yTemp;
BU08	     Game_Box_BoxXPosAfterMove;    //箱子移动后的坐标
BU08	     Game_Box_BoxYPosAfterMove;

BU08		 Game_Box_StartViewLevel[15][ 15 ][ 19 ];
BU08		 Game_Box_BoxMoveArray[ 15 ][ 19 ];//变化后的数据
BU08		 Game_Box_BoxArray[ 15 ][ 19 ];  //原始数据

BU08 		 Box_BoxbMove = false;  //箱子是否移动
BU08	     Game_Box_PressUndoKey = false;	
BU08		 Game_Box_Level = 1 ;
BU08         Game_Box_BTermini = false;
BU32		 Game_Box_Moves = 0 ;


BU32		 Game_Box_LastKey;	    //上一个按键，撤销时候判断

BU08 		 g_box_toNextLevel = 0;
BU08 		 Game_box_StartFlag = 0;
/********************************函数声明***********************************/
void  Game_Box_Key(BU32 key);
BU08 Game_Box_JudgeGameIfPass( void );
void Game_Box_DrawBackground(BU32 hdc);
void Game_Box_InitializeGame(BU32 hdc );
void Game_Box_DrawWall(BU08 iX,BU08 iY,BU32 hdc);
void Game_Box_DrawMan(BU08 iX,BU08 iY,BU32 hdc);
void Game_Box_DrawManBk(BU08 iX,BU08 iY,BU32 hdc);
void Game_Box_DrawBox(BU08 iX,BU08 iY,BU32 hdc);
void Game_Box_DrawTermini(BU08 iX,BU08 iY,BU32 hdc);
void Game_Box_DrawBackRegion(BU08 iX,BU08 iY,BU32 hdc);
void Game_Box_DisplayLevelAndMoves(BU32 hdc);
BU08 Game_Box_KeyPressDisposal( BU08 iBoxXPosAfterMove , BU08 iBoxYPosAfterMove , BU08  iBoxXPosBeforeMove , BU08 iBoxYPosBeforeMove , BU32 iMoveKey , BU08 bGoodsUndo) ;
void Game_Box_DisplayMessage(BU32 hdc);

void UI_APP_Game_Box_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Box);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频

}
/*******************************游戏－推箱子********************************/

void Game_Box_OnKey(BU32 key)
{
	Game_box_StartFlag = 1;
	Game_Box_Key(key);
}

void Game_Box_OnPaint(BU32 hdc,BU32 lparam)
{
	if(1 == lparam)//初始化
	{
	//	Game_Box_Moves = 0 ;
	//	Game_Box_Level = 1 ;
		if((!Game_box_StartFlag)||g_box_toNextLevel)
		{
			Game_Box_Moves=0;
		}

	    if(Game_Box_Level > Game_Box_MaxLevel)
			Game_Box_Level = 1;
		Game_Box_DrawBackground(hdc);
		Game_Box_InitializeGame(hdc);
		Game_Box_DisplayLevelAndMoves(hdc);
	}
	
	else if(2 == lparam) 
	{
		Game_Box_DrawBox(Game_Box_BoxXPosAfterMove,Game_Box_BoxYPosAfterMove,hdc);
		Game_Box_DrawManBk(Game_Box_TempAx,Game_Box_TempAy,hdc);
		Game_Box_DrawMan(Game_Box_TempAx,Game_Box_TempAy,hdc);//y+2
		if(Game_Box_BTermini == true)
		{
			Game_Box_BTermini = false;
			Game_Box_DrawTermini(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
		else
		{
			Game_Box_DrawBackRegion(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
	}

	else if(3 == lparam)
	{
		Game_Box_DrawManBk(Game_Box_TempAx,Game_Box_TempAy,hdc);
		Game_Box_DrawMan(Game_Box_TempAx,Game_Box_TempAy,hdc);
		if(Game_Box_BTermini == true)
		{
			Game_Box_BTermini = false;
			Game_Box_DrawTermini(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
		else
		{
			Game_Box_DrawBackRegion(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
	}

	else if(4 == lparam)
	{
		Game_Box_DrawManBk(Game_Box_xTemp,Game_Box_yTemp,hdc);
		Game_Box_DrawMan(Game_Box_xTemp,Game_Box_yTemp,hdc);//y+2
				
		if(Game_Box_BTermini == true)
		{
			Game_Box_BTermini = false;
			Game_Box_DrawTermini(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
		else
		{
			Game_Box_DrawBackRegion(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
	}

	else if(5 == lparam)
	{
		Game_Box_DrawManBk(Game_Box_TempAx,Game_Box_TempAy,hdc);
		Game_Box_DrawMan(Game_Box_TempAx,Game_Box_TempAy,hdc);			
		Game_Box_DrawBackRegion(Game_Box_TempBx,Game_Box_TempBy,hdc);
	}

	else if(6 == lparam)
	{
				
		Game_Box_DrawBox(Game_Box_TempAx,Game_Box_TempAy,hdc);
		if(Game_Box_BTermini)
		{
			Game_Box_BTermini = false;
			Game_Box_DrawTermini(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
		else
		{
			Game_Box_DrawBackRegion(Game_Box_TempBx,Game_Box_TempBy,hdc);
		}
	}
	
	else if(7 == lparam)
	{
		Game_Box_DisplayLevelAndMoves(hdc);
	}

	else if(8 == lparam)                    //初始化
	{
		Game_Box_InitializeGame(hdc);
	}

	else if(9 == lparam)
	{
		Game_Box_DrawBackground(hdc);
	}
	else if(10 == lparam)
	{
	    Game_Box_DisplayMessage(hdc);
	}
}


/**********************************************************************************************
			               				初始化游戏
**********************************************************************************************/
void  Game_Box_InitializeGame(BU32 hdc )
{
	BU08	iX, iY, iXTemp, iYTemp, iInitX, iInitY;
	if((!Game_box_StartFlag)||g_box_toNextLevel)
	{
		for( iInitY = 0 ; iInitY < 15 ; iInitY++ )
		{
			for( iInitX = 0 ; iInitX < 19 ; iInitX++ )
			{
				Game_Box_BoxMoveArray[ iInitY ][ iInitX ] = 0 ;
				Game_Box_BoxArray[ iInitY ][ iInitX ] = 0 ;
			}
		}

		for( iYTemp = 0 ; iYTemp < 15 ; iYTemp++ )
		{
			for( iXTemp = 0 ; iXTemp < 19 ; iXTemp++ )
			{
				Game_Box_BoxMoveArray[ iYTemp ][ iXTemp ] = Game_Box_StartViewLevel[Game_Box_Level-1][ iYTemp ][ iXTemp ] ;
				Game_Box_BoxArray[ iYTemp ][ iXTemp ] = Game_Box_StartViewLevel[Game_Box_Level-1][ iYTemp ][ iXTemp ] ;
			}
		}
	}

	for( iY = 0 ; iY < 15 ; iY++ )
	{
		for( iX = 0 ; iX < 19 ; iX++ )
		{
			switch( Game_Box_BoxMoveArray[ iY ][ iX ] )
			{
				case Game_Box_NullRegion :

					break;

				case Game_Box_Wall:

					Game_Box_DrawWall(iX,iY,hdc);

					break;

				case Game_Box_Box:
					
					Game_Box_DrawBox(iX,iY,hdc);

					break;

				case Game_Box_Termini:

					Game_Box_DrawTermini(iX,iY,hdc);

					break;

				case Game_Box_GameRegion:

					Game_Box_DrawBackRegion(iX,iY,hdc);

					break;

				case Game_Box_Man:
					if(Game_box_StartFlag&&(!g_box_toNextLevel))
					{
						Game_Box_DrawBackRegion(iX,iY,hdc);
					}
					else
					{
						Game_Box_DrawManBk(iX,iY,hdc);
						Game_Box_DrawMan(iX,iY,hdc);
			
						Game_Box_CurrXPos = iX ;
						Game_Box_CurrYPos = iY ; 
					}

					break;
			
			}
		}
	}
	if(Game_box_StartFlag&&(!g_box_toNextLevel))
	{
		Game_Box_DrawManBk(Game_Box_CurrXPos,Game_Box_CurrYPos,hdc);
		Game_Box_DrawMan(Game_Box_CurrXPos,Game_Box_CurrYPos,hdc);
	}
}


/****************************游戏主程序***********************************/

void  Game_Box_Key(BU32 key)
{
	BU08     	bMove = false ;
	BU08		iBoxXPosBeforeMove;
	BU08		iBoxYPosBeforeMove;
	

		switch( key )
		{
			case GUI_VK_SELECT:
				if(g_box_toNextLevel)
				{
					GUI_ENC_API_Update(ID_ODC_Box, 1);
					g_box_toNextLevel = 0;
				}
				break;
			case GUI_VK_DOWN:
				
				Game_Box_LastKey = key ;
				iBoxYPosBeforeMove = Game_Box_CurrYPos ;
				Game_Box_CurrYPos = Game_Box_CurrYPos + 1 ;
				
				bMove = Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove, key , false);
				if( bMove )
				{
					Game_Box_PressUndoKey = true ;
					Game_Box_Moves+=1;
					GUI_ENC_API_Update(ID_ODC_Box, 7);
				}
				if( Game_Box_JudgeGameIfPass()  )
				{					
				    /*
					Game_Box_Level += 1 ;
					Game_Box_Moves = 0;
					GUI_ENC_API_Update(ID_ODC_Box, 9);
					GUI_ENC_API_Update(ID_ODC_Box, 8);
					GUI_ENC_API_Update(ID_ODC_Box, 7);
					
					Game_Box_Moves += 1;*/
					GUI_ENC_API_Update(ID_ODC_Box, 7);   //显示级别和移动
					Game_Box_Level += 1 ;              //下一关
					//GUI_ENC_API_SwitchState(ID_DLG_BoxLevel,1);
					GUI_ENC_API_Update(ID_ODC_Box, 10);
				}
				break;

		  	case GUI_VK_UP:

				Game_Box_LastKey = key ;
				iBoxYPosBeforeMove = Game_Box_CurrYPos ;
				Game_Box_CurrYPos = Game_Box_CurrYPos - 1 ;

				bMove = Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove , key , false );
				if( bMove == true )
				{
					Game_Box_PressUndoKey = true ;
					Game_Box_Moves+=1;
					GUI_ENC_API_Update(ID_ODC_Box, 7);
				}
				if( Game_Box_JudgeGameIfPass() == true )
				{					
				    /*
					Game_Box_Level += 1 ;
					Game_Box_Moves = 0;
					GUI_ENC_API_Update(ID_ODC_Box, 9);
					GUI_ENC_API_Update(ID_ODC_Box, 8);
					GUI_ENC_API_Update(ID_ODC_Box, 7);
					
					Game_Box_Moves += 1;*/
					GUI_ENC_API_Update(ID_ODC_Box, 7);   //显示级别和移动
					Game_Box_Level += 1 ;              //下一关
					//GUI_ENC_API_SwitchState(ID_DLG_BoxLevel,1);
					GUI_ENC_API_Update(ID_ODC_Box, 10);

				}
				break;


			case GUI_VK_LEFT:

				Game_Box_LastKey = key ;
				iBoxXPosBeforeMove = Game_Box_CurrXPos ;
				Game_Box_CurrXPos = Game_Box_CurrXPos - 1 ;

				bMove = Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false );
				if( bMove == true )
				{
					Game_Box_PressUndoKey = true ;
					Game_Box_Moves+=1;
					GUI_ENC_API_Update(ID_ODC_Box, 7);
				}
				if( Game_Box_JudgeGameIfPass() == true )
				{					
				    /*
					Game_Box_Level += 1 ;
					Game_Box_Moves = 0;
					GUI_ENC_API_Update(ID_ODC_Box, 9);
					GUI_ENC_API_Update(ID_ODC_Box, 8);
					GUI_ENC_API_Update(ID_ODC_Box, 7);
					
					Game_Box_Moves += 1;*/
					GUI_ENC_API_Update(ID_ODC_Box, 7);   //显示级别和移动
					Game_Box_Level += 1 ;              //下一关
					//GUI_ENC_API_SwitchState(ID_DLG_BoxLevel,1);
					GUI_ENC_API_Update(ID_ODC_Box, 10);
				}
				break;

			case GUI_VK_RIGHT:

				Game_Box_LastKey = key ;
				iBoxXPosBeforeMove = Game_Box_CurrXPos ;
				Game_Box_CurrXPos = Game_Box_CurrXPos + 1 ;

				bMove = Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false );
 				if( bMove == true )
				{
					Game_Box_PressUndoKey = true ;
					Game_Box_Moves+=1;
					GUI_ENC_API_Update(ID_ODC_Box, 7);
				}
				if( Game_Box_JudgeGameIfPass() == true )
				{	
				    /*
					Game_Box_Level += 1 ;
					Game_Box_Moves = 0;
					GUI_ENC_API_Update(ID_ODC_Box, 9);
					GUI_ENC_API_Update(ID_ODC_Box, 8);
					GUI_ENC_API_Update(ID_ODC_Box, 7);
					
					Game_Box_Moves += 1;*/
					GUI_ENC_API_Update(ID_ODC_Box, 7);   //显示级别和移动
					Game_Box_Level += 1 ;              //下一关
					GUI_ENC_API_Update(ID_ODC_Box, 10);
					//GUI_ENC_API_Update(ID_ODC_Box, 1);
				}
				break;
				

			case GUI_VK_GREEN:

				//Game_Box_LastKey = key ;
				Game_Box_PressUndoKey = false;
				Game_Box_Level -= 1 ;
				if( Game_Box_Level < 1 )
				{
					Game_Box_Level = Game_Box_MaxLevel ;
				}
				g_box_toNextLevel = 1;
				GUI_ENC_API_Update(ID_ODC_Box, 9);
				GUI_ENC_API_Update(ID_ODC_Box, 8);
				Game_Box_Moves = 0 ;
				GUI_ENC_API_Update(ID_ODC_Box, 7);
				break;

			case GUI_VK_RED:

				//Game_Box_LastKey = key ;
				Game_Box_PressUndoKey = false;
			
				Game_Box_Level += 1 ;
				if( Game_Box_Level > Game_Box_MaxLevel )
				{
					Game_Box_Level = 1 ;
				}
				g_box_toNextLevel = 1;
				GUI_ENC_API_Update(ID_ODC_Box, 9);
				GUI_ENC_API_Update(ID_ODC_Box, 8);
				Game_Box_Moves = 0 ;
				GUI_ENC_API_Update(ID_ODC_Box, 7);
				break;

			case GUI_VK_BLUE:
	
				if( Game_Box_PressUndoKey == true )
				{
					Game_Box_PressUndoKey = false ;
					switch( Game_Box_LastKey )
					{
						case GUI_VK_DOWN:
							
							key = GUI_VK_UP ;
							iBoxYPosBeforeMove = Game_Box_CurrYPos ;
							Game_Box_CurrYPos = Game_Box_CurrYPos - 1 ;
							
							
							//if( Game_Box_BoxMoveArray[ iBoxYPosBeforeMove+1 ][ Game_Box_CurrXPos ] == Game_Box_Box&&Box_BoxbMove )
							if(Box_BoxbMove)
							{
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove , key , false);
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , iBoxYPosBeforeMove , Game_Box_CurrXPos , (BU08)(iBoxYPosBeforeMove+1) , key , true );
							}
							else
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove , key , false);
							
							break;

		  				case GUI_VK_UP:
							
							key = GUI_VK_DOWN ;
							iBoxYPosBeforeMove = Game_Box_CurrYPos ;
							Game_Box_CurrYPos = Game_Box_CurrYPos + 1 ;
							
							//Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove , key , false);
							//if( Game_Box_BoxMoveArray[ iBoxYPosBeforeMove+1 ][ Game_Box_CurrXPos ] == Game_Box_Box )
							if(Box_BoxbMove)
							{
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove , key , false);
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , iBoxYPosBeforeMove , Game_Box_CurrXPos , (BU08)(iBoxYPosBeforeMove-1) , key , true);
							}
							else
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , Game_Box_CurrXPos , iBoxYPosBeforeMove , key , false);
							
							break;

						case GUI_VK_LEFT:

							key = GUI_VK_RIGHT ;

							iBoxXPosBeforeMove = Game_Box_CurrXPos ;
							Game_Box_CurrXPos = Game_Box_CurrXPos + 1 ;

							//Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false);
							//if( Game_Box_BoxMoveArray[ iBoxYPosBeforeMove+1 ][ Game_Box_CurrXPos ] == Game_Box_Box )
							if(Box_BoxbMove)
							{
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false);
								Game_Box_KeyPressDisposal( iBoxXPosBeforeMove , Game_Box_CurrYPos , (BU08)(iBoxXPosBeforeMove-1) , Game_Box_CurrYPos , key , true);
							}
							else
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false);
							
							break;

						case GUI_VK_RIGHT:

							key = GUI_VK_LEFT ;
							iBoxXPosBeforeMove = Game_Box_CurrXPos ;
							Game_Box_CurrXPos = Game_Box_CurrXPos - 1 ;

							//if( Game_Box_BoxMoveArray[ iBoxYPosBeforeMove+1 ][ Game_Box_CurrXPos ] == Game_Box_Box )
							if(Box_BoxbMove)
							{
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false);
								Game_Box_KeyPressDisposal( iBoxXPosBeforeMove , Game_Box_CurrYPos , (BU08)(iBoxXPosBeforeMove+1) , Game_Box_CurrYPos , key , true);
							}
							else
								Game_Box_KeyPressDisposal( Game_Box_CurrXPos , Game_Box_CurrYPos , iBoxXPosBeforeMove , Game_Box_CurrYPos , key , false);
							
							break;
						
						case GUI_VK_P_DOWN:
						case GUI_VK_P_UP:

							Game_Box_Moves = 1;

							break;

					}
						Game_Box_Moves-=1;
						GUI_ENC_API_Update(ID_ODC_Box, 7);
				}
				break;

			case GUI_VK_RECALL:
			case GUI_VK_MENU:
				
				//GUI_ENC_API_Update(ID_ODC_Box, 9);

				break;
				
			case GUI_VK_EXIT:
				Game_Box_Level = 1;
				Game_box_StartFlag = 0;
//				GUI_ENC_API_Update(ID_ODC_Box, 9);

				break;			
		}
	}


/************************按键处理*********************/

BU08  Game_Box_KeyPressDisposal( BU08 iBoxXPosAfterMove , BU08 iBoxYPosAfterMove , BU08  iBoxXPosBeforeMove , BU08 iBoxYPosBeforeMove , BU32 iMoveKey , BU08 bGoodsUndo) 
{
	Game_Box_TempAx = iBoxXPosAfterMove;
	Game_Box_TempAy = iBoxYPosAfterMove;
	Game_Box_TempBx = iBoxXPosBeforeMove;
	Game_Box_TempBy = iBoxYPosBeforeMove;
	
	if( bGoodsUndo == false )
	{
		switch( Game_Box_BoxMoveArray[ iBoxYPosAfterMove ][ iBoxXPosAfterMove ] )
		{
			case Game_Box_Wall: 
				Box_BoxbMove = false;
				Game_Box_CurrXPos = iBoxXPosBeforeMove ; 
				Game_Box_CurrYPos = iBoxYPosBeforeMove ;
				
				return false;

			case Game_Box_Box:
				
				switch( iMoveKey )
				{				
					case GUI_VK_LEFT:

						Game_Box_BoxXPosAfterMove = iBoxXPosAfterMove - 1 ;
						Game_Box_BoxYPosAfterMove = iBoxYPosAfterMove ; 

						break;

					case GUI_VK_RIGHT:

						Game_Box_BoxXPosAfterMove = iBoxXPosAfterMove + 1 ;
						Game_Box_BoxYPosAfterMove = iBoxYPosAfterMove ; 

						break;

					case GUI_VK_DOWN:

						Game_Box_BoxYPosAfterMove = iBoxYPosAfterMove + 1 ;
						Game_Box_BoxXPosAfterMove = iBoxXPosAfterMove ;

						break;

		  			case GUI_VK_UP:

						Game_Box_BoxYPosAfterMove = iBoxYPosAfterMove - 1 ;
						Game_Box_BoxXPosAfterMove = iBoxXPosAfterMove ;

 						break;
				
				}
				if( Game_Box_BoxMoveArray[ Game_Box_BoxYPosAfterMove ][ Game_Box_BoxXPosAfterMove ] ==  Game_Box_Wall  )
				{
					Game_Box_CurrXPos = iBoxXPosBeforeMove ; 
					Game_Box_CurrYPos = iBoxYPosBeforeMove ;					
					return false;			
				}
				else if( Game_Box_BoxMoveArray[ Game_Box_BoxYPosAfterMove ][ Game_Box_BoxXPosAfterMove ] ==  Game_Box_Box  )
				{
					Game_Box_CurrXPos = iBoxXPosBeforeMove ; 
					Game_Box_CurrYPos = iBoxYPosBeforeMove ;					
					return false;			
				}
				
				Game_Box_BoxMoveArray[ Game_Box_BoxYPosAfterMove ][ Game_Box_BoxXPosAfterMove ] = Game_Box_Box ;
				Box_BoxbMove = true;
				if( Game_Box_BoxArray[ iBoxYPosBeforeMove ][ iBoxXPosBeforeMove ] == Game_Box_Termini )
				{
					Game_Box_BTermini = true;
					//判断原来人在的地方是否为目的区
 				}
				else
				{
					Game_Box_BTermini = false;
 				}
				GUI_ENC_API_Update(ID_ODC_Box, 2);
				if( Game_Box_BoxArray[ iBoxYPosAfterMove ][ iBoxXPosAfterMove ] ==  Game_Box_Termini )
				{
					Game_Box_BoxMoveArray[ iBoxYPosAfterMove ][ iBoxXPosAfterMove ] =  Game_Box_Termini ;
					//判断人移动后所在的地方是否为目的区
				}
				else
				{
					Game_Box_BoxMoveArray[ iBoxYPosAfterMove ][ iBoxXPosAfterMove ] = Game_Box_GameRegion ;
				}
				break;


			case Game_Box_Termini:
				Box_BoxbMove = false;
				if( Game_Box_BoxMoveArray[ iBoxYPosBeforeMove ][ iBoxXPosBeforeMove ] == Game_Box_Termini )
				{
					Game_Box_BTermini = true;
 				}
				else
				{
					Game_Box_BTermini = false;
 				}
				GUI_ENC_API_Update(ID_ODC_Box, 3);
				break;

			case Game_Box_GameRegion:
				Box_BoxbMove = false;
				Game_Box_xTemp = iBoxXPosAfterMove;
				Game_Box_yTemp = iBoxYPosAfterMove;
				if( Game_Box_BoxMoveArray[ iBoxYPosBeforeMove ][ iBoxXPosBeforeMove ] == Game_Box_Termini )
				{
					Game_Box_BTermini = true;
 				}
				else
				{
					Game_Box_BTermini = false;
 				}
				GUI_ENC_API_Update(ID_ODC_Box, 4);
				break;

			case 5:
				GUI_ENC_API_Update(ID_ODC_Box, 5);
 				break;
 				
		}
		
			return true ;
			
	}
	else
	{			
		Game_Box_BoxMoveArray[ iBoxYPosAfterMove ][ iBoxXPosAfterMove ] = Game_Box_Box ;
		
		if( Game_Box_BoxArray [ iBoxYPosBeforeMove ][ iBoxXPosBeforeMove ] == Game_Box_Termini)
		{
			Game_Box_BTermini = true;
 			Game_Box_BoxMoveArray[ iBoxYPosBeforeMove ][ iBoxXPosBeforeMove ] = Game_Box_Termini ;
 		}
		else
		{
			Game_Box_BTermini = false;
 			Game_Box_BoxMoveArray[ iBoxYPosBeforeMove ][ iBoxXPosBeforeMove ] = Game_Box_GameRegion ;
		}
		GUI_ENC_API_Update(ID_ODC_Box, 6);
	}
	return 0 ;

}
/**********************************************************************************************
				判断游戏是否过关
**********************************************************************************************/
BU08  Game_Box_JudgeGameIfPass( void )
{
	
	int iXTemp , iYTemp ; 
                      
	for( iYTemp = 0 ; iYTemp < 15 ; iYTemp++ )
	{
		for( iXTemp = 0 ; iXTemp < 19 ; iXTemp++ )
		{
			if( Game_Box_BoxMoveArray[ iYTemp ][ iXTemp ] == Game_Box_Termini )
				return false;

		}
	}
	
	return true ;
}


/******************************画图函数*****************************/

void Game_Box_DrawWall(BU08 iX,BU08 iY,BU32 hdc)
{
	GUI_API_FillRect(hdc,Game_Box_Xpos+iX*(Game_Box_Width+1),
		Game_Box_Ypos+iY*(Game_Box_Height+1)+1,Game_Box_Width,Game_Box_Height,Game_Box_WallColor);
	GUI_API_DrawRectangle(hdc,Game_Box_Xpos+iX*(Game_Box_Width+1),Game_Box_Ypos+iY*(Game_Box_Height+1)+1,
		                       Game_Box_Width-1,Game_Box_Height-1,249);
}


void Game_Box_DrawBox(BU08 iX,BU08 iY,BU32 hdc)
{
/*	GUI_API_FillRect(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ), 
		Game_Box_Ypos+iY*( Game_Box_Height+1),Game_Box_Width,Game_Box_Height,Game_Box_BoxColor);
*/
	GUI_API_DrawStyle(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ),Game_Box_Ypos+iY*( Game_Box_Height+1)+1,STYLE_GameBox_W20_H20);
	

}


void Game_Box_DrawTermini(BU08 iX,BU08 iY,BU32 hdc)
{
	GUI_API_FillRect(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ), 
		Game_Box_Ypos+iY*( Game_Box_Height+1)+1,Game_Box_Width,Game_Box_Height,Game_Box_TerminiColor);
						
}


void Game_Box_DrawBackRegion(BU08 iX,BU08 iY,BU32 hdc)
{

	GUI_API_FillRect(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ), 
		Game_Box_Ypos+iY*( Game_Box_Height+1),Game_Box_Width+1,Game_Box_Height+1,Game_Box_RegionColor);


						
}


void Game_Box_DrawMan(BU08 iX,BU08 iY,BU32 hdc)
{
/*
	GUI_API_FillRect(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ), 
		Game_Box_Ypos+iY*( Game_Box_Height+1),Game_Box_Width,Game_Box_Height,Game_Box_ManColor);	
	*/
	GUI_API_DrawStyle(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ),Game_Box_Ypos+iY*( Game_Box_Height+1)+1,STYLE_GameMan_W20_H20);

}


void Game_Box_DrawManBk(BU08 iX,BU08 iY,BU32 hdc)
{
	GUI_API_FillRect(hdc,Game_Box_Xpos+iX*( Game_Box_Width + 1 ), 
		Game_Box_Ypos+iY*( Game_Box_Height+1)+1,Game_Box_Width,Game_Box_Height,Game_Box_ManBKColor);								
}

/**************画游戏背景***************/

void Game_Box_DrawBackground(BU32 hdc)
{
	GUI_API_FillRect(hdc,Game_Box_Xpos,Game_Box_Ypos,Game_Box_BKWidth,Game_Box_BKHeight,Game_Box_BoxBKColor);
}


/***********游戏成绩显示框****************/

void Game_Box_DisplayLevelAndMoves(BU32 hdc)
{
	BU08 str_m[4],str_l[3];
	BU32 lang;

	if(Game_Box_Moves > 999)
	{
		Game_Box_Moves = 0;
	}	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	//GUI_API_FillRect(hdc,Game_Box_Xpos-20,440,250,50,Game_Box_BoxBKColor);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		GUI_API_TextOut(hdc,123,300,"LEVEL",5,Game_Box_TextColor,1,1);
		GUI_API_TextOut(hdc,123,356,"MOVES",5,Game_Box_TextColor,1,1);
	}
	else
	{
		GUI_API_TextOut(hdc,123,300,"级别",4,Game_Box_TextColor,1,1);
		GUI_API_TextOut(hdc,123,356,"移动",4,Game_Box_TextColor,1,1);
	}
	GUI_API_AddDecStr(Game_Box_Moves, 3, str_m);
	GUI_API_TextOut(hdc,123+5,384,str_m,sizeof(str_m),Game_Box_TextColor,Game_Box_BoxBKColor,0);

	GUI_API_AddDecStr(Game_Box_Level, 2, str_l);
	GUI_API_TextOut(hdc,123+10,328,str_l,sizeof(str_l),Game_Box_TextColor,Game_Box_BoxBKColor,0);

}

/**********************提示过关*********************************/
void Game_Box_DisplayMessage(BU32 hdc)
{
    BU08 str_cn[]="恭喜你过关! 进入下一关?";
	BU08 str_en[]="Congratulations! Next level?";
	
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		
   		GUI_API_TextOut(hdc,Game_Box_Xpos+100,Game_Box_Ypos+150,str_en,sizeof(str_en),0,Game_Box_TerminiColor,0);
	}
	else
	{
   		GUI_API_TextOut(hdc,Game_Box_Xpos+100,Game_Box_Ypos+150,str_cn,sizeof(str_cn),0,Game_Box_TerminiColor,0);
	}
	g_box_toNextLevel = 1;
}


BU08	Game_Box_StartViewLevel[15][ 15 ][ 19 ] = 
{
	/*0*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  	
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  	 
	0 , 0 , 0 , 0 , 1 , 2 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 1 , 1 , 1 , 4 , 4 , 2 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 1 , 4 , 4 , 2 , 4 , 2 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	1 , 1 , 1 , 4 , 1 , 4 , 1 , 1 , 4 , 1 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 ,  
	1 , 4 , 4 , 4 , 1 , 4 , 1 , 1 , 4 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 3 , 3 , 1 ,  
	1 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 3 , 3 , 1 ,  
	1 , 1 , 1 , 1 , 1 , 4 , 1 , 1 , 1 , 4 , 1 , 5 , 1 , 1 , 4 , 4 , 3 , 3 , 1 ,  
	0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,  
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,   	 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	/*1*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  //1	
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,   
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 0 , 0 ,   
	0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 2 , 1 , 1 , 1 , 1 , 4 , 4 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 4 , 5 , 4 , 4 , 1 , 1 , 4 , 4 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 4 , 1 , 4 , 4 , 2 , 4 , 1 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 1 , 1 , 2 , 4 , 2 , 4 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 1 , 4 , 2 , 4 , 4 , 2 , 4 , 2 , 4 , 2 , 4 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 ,  	 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	/*2*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,    //2               
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 5 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 2 , 1 , 2 , 4 , 1 , 1 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 2 , 4 , 4 , 2 , 1 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 2 , 4 , 2 , 4 , 1 , 0 , 0 , 0 ,  
	0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 2 , 4 , 1 , 4 , 1 , 1 , 1 , 0 ,  
	0 , 1 , 3 , 3 , 3 , 3 , 4 , 4 , 1 , 1 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 1 , 0 ,  
	0 , 1 , 1 , 3 , 3 , 3 , 4 , 4 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 4 , 1 , 0 ,  
	0 , 1 , 3 , 3 , 3 , 3 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 ,  
	0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,

	/*3*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,//3                                        
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 1 , 4 , 4 , 1 , 1 , 1 , 1 , 0 ,  
	0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 1 , 4 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 1 , 0 ,  
	0 , 1 , 1 , 1 , 1 , 4 , 1 , 2 , 2 , 4 , 1 , 1 , 4 , 1 , 1 , 4 , 4 , 1 , 0 ,  
	1 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 4 , 1 , 4 , 4 , 1 , 1 , 4 , 1 , 1 , 1 ,  
	1 , 4 , 4 , 1 , 1 , 1 , 4 , 2 , 1 , 2 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 1 ,  
	1 , 3 , 3 , 3 , 4 , 4 , 4 , 4 , 1 , 4 , 1 , 1 , 4 , 4 , 1 , 4 , 4 , 4 , 1 ,  
	1 , 3 , 3 , 3 , 1 , 4 , 4 , 4 , 4 , 5 , 4 , 1 , 4 , 1 , 1 , 1 , 4 , 1 , 1 ,  
	1 , 3 , 3 , 3 , 1 , 4 , 4 , 1 , 1 , 1 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 1 ,  
	1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 1 , 4 , 4 , 4 , 1 , 4 , 4 , 4 , 1 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*4*/
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 0 , 
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 1 , 2 , 1 , 1 , 4 , 4 , 1 , 0 , 
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 2 , 4 , 1 , 0 ,
	0 ,1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 0 , 
	0 ,1 , 3 , 3 , 3 , 3 , 4 , 4 , 1 , 1 , 4 , 2 , 4 , 4 , 2 , 1 , 1 , 1 , 0 ,
	0 ,1 , 3 , 3 , 3 , 3 , 4 , 4 , 4 , 4 , 2 , 4 , 2 , 2 , 4 , 1 , 1 , 0 , 0 ,
	0 ,1 , 3 , 3 , 3 , 3 , 4 , 4 , 1 , 1 , 2 , 4 , 4 , 2 , 4 , 5 , 1 , 0 , 0 ,
	0 ,1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 2 , 4 , 4 , 1 , 1 , 0 , 0 ,
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 2 , 4 , 2 , 4 , 4 , 1 , 0 , 0 ,
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 4 , 1 , 1 , 4 , 1 , 0 , 0 ,
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 1 , 0 , 0 ,
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 ,
	0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*5*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 0 , 1 , 1 , 5 , 1 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 4 , 4 , 4 , 2 , 2 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 3 , 3 , 4 , 4 , 1 , 4 , 1 , 4 , 2 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 3 , 3 , 1 , 1 , 1 , 4 , 1 , 4 , 2 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 4 , 2 , 4 , 1 , 2 , 4 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 2 , 1 , 4 , 2 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 1 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*6*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 4 , 1 , 4 , 5 , 1 , 1 , 4 , 2 , 2 , 4 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 2 , 4 , 4 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 4 , 1 , 1 , 1 , 1 , 1 , 2 , 1 , 1 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 2 , 4 , 4 , 1 , 1 , 1 , 4 , 3 , 3 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 2 , 4 , 2 , 4 , 2 , 4 , 3 , 3 , 3 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 3 , 3 , 3 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 2 , 2 , 4 , 1 , 0 , 1 , 3 , 3 , 3 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 1 , 1 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*7*/
	0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 1 , 4 , 1 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 2 , 5 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 ,  
	0 , 1 , 4 , 2 , 4 , 1 , 1 , 2 , 4 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 0 ,  
	0 , 1 , 4 , 1 , 1 , 1 , 1 , 4 , 2 , 4 , 4 , 4 , 4 , 2 , 4 , 1 , 0 , 0 , 0 ,  
	0 , 1 , 4 , 1 , 1 , 4 , 1 , 1 , 4 , 1 , 4 , 4 , 1 , 2 , 4 , 1 , 1 , 1 , 1 ,  
	1 , 1 , 4 , 4 , 1 , 1 , 1 , 1 , 4 , 1 , 1 , 2 , 4 , 4 , 4 , 4 , 4 , 4 , 1 ,  
	1 , 4 , 4 , 2 , 1 , 4 , 4 , 2 , 4 , 4 , 1 , 4 , 1 , 1 , 4 , 1 , 1 , 4 , 1 ,  
	1 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 4 , 1 , 3 , 3 , 3 , 1 , 4 , 1 ,  
	1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 1 , 1 , 1 , 4 , 4 , 3 , 3 , 3 , 4 , 4 , 1 ,  
	0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 1 , 4 , 1 , 3 , 3 , 3 , 1 , 4 , 1 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 1 , 1 , 1 , 4 , 1 , 4 , 1 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 1 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
	
	/*8*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 
	0 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 
	0 , 1 , 4 , 1 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 
	0 , 1 , 4 , 1 , 4 , 4 , 2 , 4 , 2 , 4 , 2 , 4 , 2 , 1 , 4 , 4 , 1 , 0 , 0 , 
	0 , 1 , 4 , 1 , 4 , 4 , 4 , 2 , 5 , 2 , 4 , 4 , 4 , 1 , 1 , 4 , 1 , 1 , 0 , 
	0 , 1 , 4 , 1 , 4 , 4 , 2 , 4 , 2 , 4 , 2 , 1 , 1 , 1 , 3 , 3 , 3 , 1 , 0 , 
	0 , 1 , 4 , 1 , 4 , 4 , 4 , 2 , 4 , 2 , 4 , 4 , 1 , 1 , 3 , 3 , 3 , 1 , 0 , 
	0 , 1 , 4 , 1 , 1 , 1 , 2 , 2 , 2 , 4 , 2 , 4 , 1 , 1 , 3 , 3 , 3 , 1 , 0 , 
	0 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 4 , 1 , 1 , 4 , 1 , 1 , 3 , 3 , 3 , 1 , 0 , 
	0 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 1 , 4 , 1 , 1 , 3 , 3 , 3 , 1 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*9*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 1 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 1 , 1 , 4 , 4 , 1 , 4 , 4 , 1 , 3 , 3 , 3 , 1 , 1 , 1 , 0 , 0 , 0 , 
	0 , 0 , 1 , 4 , 4 , 2 , 1 , 4 , 4 , 1 , 3 , 3 , 3 , 4 , 4 , 1 , 0 , 0 , 0 , 
	0 , 0 , 1 , 4 , 4 , 2 , 1 , 4 , 4 , 1 , 3 , 3 , 3 , 4 , 3 , 1 , 0 , 0 , 0 , 
	0 , 0 , 1 , 4 , 2 , 4 , 1 , 2 , 2 , 4 , 3 , 3 , 3 , 4 , 4 , 1 , 0 , 0 , 0 , 
	0 , 0 , 1 , 4 , 4 , 4 , 1 , 4 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 
	0 , 0 , 1 , 1 , 2 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 2 , 4 , 2 , 4 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 1 , 4 , 4 , 2 , 2 , 4 , 1 , 4 , 4 , 4 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 1 , 1 , 2 , 2 , 5 , 1 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*10*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 ,  
	0 , 1 , 3 , 3 , 3 , 4 , 4 , 4 , 1 , 4 , 4 , 4 , 4 , 1 , 4 , 4 , 4 , 1 , 1 ,  
	1 , 1 , 3 , 3 , 3 , 3 , 3 , 4 , 4 , 2 , 1 , 1 , 4 , 1 , 4 , 1 , 2 , 4 , 1 ,  
	1 , 3 , 3 , 3 , 3 , 3 , 3 , 1 , 4 , 4 , 2 , 4 , 4 , 1 , 4 , 4 , 4 , 4 , 1 ,  
	1 , 3 , 3 , 3 , 3 , 3 , 3 , 1 , 4 , 4 , 1 , 4 , 4 , 1 , 4 , 1 , 4 , 4 , 1 ,  
	1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 2 , 4 , 4 , 2 , 4 , 2 , 4 , 4 , 1 ,  
	0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 2 , 1 , 1 , 2 , 4 , 1 , 1 , 2 , 1 , 1 ,  
	0 , 1 , 1 , 4 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 1 , 4 , 2 , 4 , 4 , 4 , 4 , 1 ,  
	0 , 1 , 4 , 4 , 1 , 1 , 4 , 1 , 1 , 1 , 4 , 1 , 4 , 4 , 1 , 1 , 2 , 4 , 1 ,  
	0 , 1 , 4 , 2 , 4 , 2 , 2 , 4 , 4 , 4 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 1 ,  
	0 , 1 , 4 , 2 , 4 , 4 , 4 , 4 , 2 , 1 , 1 , 2 , 4 , 1 , 1 , 1 , 1 , 1 , 1 ,  
	0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 5 , 4 , 1 , 1 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  ,
	
	/*11*/
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 1 , 1 , 0 , 0 , 0 , 0 , 
	0 , 1 , 1 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 4 , 4 , 2 , 4 , 1 , 1 , 1 , 1 , 1 , 
	0 , 1 , 4 , 4 , 4 , 1 , 1 , 4 , 1 , 1 , 4 , 4 , 4 , 1 , 1 , 3 , 3 , 3 , 1 , 
	0 , 1 , 4 , 1 , 2 , 2 , 4 , 2 , 4 , 2 , 2 , 1 , 2 , 1 , 1 , 3 , 3 , 3 , 1 , 
	0 , 1 , 4 , 1 , 4 , 4 , 4 , 4 , 5 , 4 , 4 , 1 , 4 , 4 , 4 , 3 , 3 , 3 , 1 , 
	0 , 1 , 4 , 4 , 2 , 1 , 4 , 1 , 1 , 1 , 2 , 2 , 4 , 4 , 4 , 3 , 3 , 3 , 1 , 
	0 , 1 , 4 , 2 , 4 , 4 , 2 , 2 , 4 , 4 , 2 , 4 , 1 , 1 , 3 , 3 , 3 , 3 , 1 , 
	0 , 1 , 1 , 1 , 2 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
	0 , 0 , 0 , 1 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  ,
	
	/*12*/

	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 4 , 2 , 4 , 2 , 2 , 4 , 2 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 4 , 1 , 4 , 4 , 2 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 1 , 4 , 4 , 4 , 2 , 2 , 4 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 1 , 1 , 1 , 4 , 4 , 4 , 2 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 1 , 3 , 4 , 2 , 4 , 1 , 1 , 4 , 1 , 1 , 1 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 3 , 3 , 4 , 4 , 2 , 1 , 1 , 4 , 2 , 4 , 4 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 3 , 3 , 2 , 4 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 3 , 1 , 1 , 1 , 2 , 1 , 1 , 1 , 4 , 1 , 5 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 3 , 3 , 1 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*13*/

	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  
	0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 4 , 4 , 4 , 4 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 
	0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 1 , 1 , 4 , 2 , 4 , 2 , 4 , 4 , 4 , 5 , 1 , 
	0 , 0 , 0 , 1 , 4 , 1 , 1 , 4 , 1 , 1 , 2 , 1 , 2 , 4 , 2 , 4 , 2 , 1 , 1 , 
	0 , 1 , 1 , 1 , 4 , 3 , 3 , 3 , 3 , 3 , 3 , 1 , 4 , 4 , 2 , 2 , 4 , 1 , 1 , 
	0 , 1 , 4 , 4 , 4 , 3 , 3 , 3 , 3 , 3 , 3 , 1 , 4 , 4 , 1 , 4 , 4 , 4 , 1 , 
	0 , 1 , 4 , 1 , 4 , 3 , 3 , 3 , 3 , 3 , 3 , 1 , 2 , 4 , 4 , 2 , 4 , 4 , 1 , 
	0 , 1 , 4 , 1 , 2 , 3 , 3 , 3 , 3 , 3 , 3 , 4 , 2 , 2 , 1 , 4 , 2 , 4 , 1 , 
	0 , 1 , 4 , 4 , 4 , 1 , 1 , 1 , 4 , 1 , 1 , 1 , 2 , 4 , 4 , 2 , 4 , 1 , 1 , 
	0 , 1 , 1 , 1 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 1 , 0 , 
	0 , 0 , 0 , 1 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 4 , 2 , 4 , 1 , 0 , 
	0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	
	/*14*/

	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 4 , 5 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 4 , 4 , 2 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 2 , 1 , 1 , 4 , 2 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 1 , 1 , 2 , 1 , 3 , 3 , 3 , 1 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 4 , 2 , 3 , 3 , 3 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 4 , 1 , 3 , 4 , 3 , 1 , 4 , 1 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 4 , 1 , 4 , 1 , 2 , 4 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 2 , 4 , 4 , 2 , 4 , 4 , 4 , 4 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 4 , 4 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 
};

