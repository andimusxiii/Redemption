


#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <stdlib.h>
#include "gba.h"
#include "font.h"
#include "art.h"
#include <string.h>

enum GameState {START, LEVEL, BOSS, RESULTS, END};

void loadAlphabet();
void drawText(int x, int y, const char string[]);
void clearText();

class Text
	{	
		private:
		
		public:
			Text();
			bool startGame;
			void startScreen();
			void checkForStart();
			void gameOverScreen();
			void winScreen();
	};

#endif
