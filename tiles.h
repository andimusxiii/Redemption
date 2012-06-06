


#ifndef TILES_H
#define TILES_H

#include <stdint.h>
#include <stdlib.h>
#include "gba.h"
#include "font.h"
#include "art.h"
#include <string.h>

void setModeZero();
void loadBackgroundPallette();


class DynamicSky
{	
	int dayNightTimer ;
	int paletteNumber ;
	int startPosition ;
	bool isRising ;	
	int frameCounter ;
	void initialise();
	
	public:
		DynamicSky();
		void update();
	
};

class Background
{	
	float mountainsXCoord;
	float dunesXCoord;
	void loadBackground(int arraySize, int screenBlock, int charBlock, int yOffset, uint8_t paletteOffset, const unsigned char externalArray[]);
	void loadMountains();
	void loadDunes();
	void loadGround();
	

	public:
		Background();
		void moveBackground();
		void update(bool moving, bool right);
};		

void DrawText(int x, int y, const char string[]);
#endif
