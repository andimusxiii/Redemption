#include "text.h"
	
	void loadAlphabet()
	{ 	//into charblock 0
		for (int i = 10; i < 139; ++i)
		{ 
			LoadTile8(0, i, font_bold[i-10]);
		}
	}
	
	void drawText(int x, int y, const char string[])
	{	//displays text at the given x & y position
		int endOfString;
		
		endOfString = strlen(string);
		
		for (int i = 0; i < endOfString; ++i)
			{SetTile(30, x, y, string[i]+10);
			 ++x;	
			}
	}
	
	void clearText()
	{	//wipes all text from screen
		
		for (int y = 0; y < 32; ++y)
		{
			for (int x = 0; x < 32; ++x)
			{
				SetTile(30, x, y, 0);
			}
		} 
	}
	
	Text::Text()
	{
		startGame = false;
		
	}
	
	void Text::startScreen()
	{	
		char textline1[] = "Redemption";
		char textline2[] = "Press Start";
		drawText(10, 8, textline1);
		drawText(10, 10, textline2);
	}

	void Text::checkForStart()
	{
		if ((REG_P1 & KEY_START) == 0)
			{
				startGame = true;
			}
	}
	
	void Text::gameOverScreen()
	{
		char textline1[] = "Game Over";
		char textline2[] = "Press Start To Play Again";
		char textline3[] = "Hint: Use the robot to shield" ; 
		char textline4[] = " the girl from fire";
		drawText(10, 5, textline1);
		drawText(2, 7, textline2);
		drawText(0, 14, textline3);
		drawText(5, 16, textline4);
	}
	
	void Text::winScreen()
	{
		char textline1[] = "Congratulations";
		char textline2[] = "You Saved The Human Race";
		char textline3[] = "Press Start To Play Again";
		drawText(7, 5, textline1);
		drawText(2, 9, textline2);
		drawText(2, 11, textline3);
		
	}
	
	
	