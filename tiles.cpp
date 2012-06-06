#include "tiles.h"
	
	

	void setModeZero()
	{	
		// Set display options.
		// DCNT_MODE0 sets mode 0, 
		// DCNT_OBJ enables objects.
		// DCNT_BG0 to BG3 enables backgrounds 0 to 3
		REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3;
	
		// Sets screen blocks & charblocks
		REG_BG0CNT = BG_PRIO(0) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
		REG_BG1CNT = BG_PRIO(2) | BG_CBB(2) | BG_SBB(29) | BG_8BPP | BG_REG_32x32;
		REG_BG2CNT = BG_PRIO(2) | BG_CBB(1) | BG_SBB(28) | BG_8BPP | BG_REG_32x32;
		REG_BG3CNT = BG_PRIO(2) | BG_CBB(0) | BG_SBB(27) | BG_8BPP | BG_REG_32x32;
	}
	
	void loadBackgroundPallette()
	{
		for (int i = 1; i < 256; ++i)
			{SetPaletteBG(i, backgroundPal[i]);}
	}
	
	
	
	DynamicSky::DynamicSky()
	{	
		dayNightTimer = 1;
		frameCounter = 0;
		paletteNumber = 0;
		startPosition = 4;
		isRising = true;	
		initialise();
	}
	
	void DynamicSky::initialise()
	{	//loads 4 tiles for the sky
		LoadTile8(0, 1, skyBoxTile1);
		LoadTile8(0, 2, skyBoxTile2);
		LoadTile8(0, 3, skyBoxTile3);
		LoadTile8(0, 4, skyBoxTile4);
		
		//fills the background in even section of 32 x 3 tiles
		for (int y = 9; y < 32; ++y)
		{
			for (int x = 0; x < 32; ++x)
			{
				SetTile(27, x, y, 1);
			}
		}
		
		for (int y = 6; y < 9; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					SetTile(27, x, y, 2);
				}
			}
		
		for (int y = 3; y < 6; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					SetTile(27, x, y, 3);
				}
			}
			
		for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					SetTile(27, x, y, 4);
				}
			}
	}

	void DynamicSky::update()
	{
		if (frameCounter % 20 == 0)	
		{	
			//loops for each sky tile 
			for (int i = 0; i < 4; i++)
			{	//updates the palette by adding the value in a timer 
				//subtracts one from this for each tile
				paletteNumber = startPosition + dayNightTimer - i; 
				SetPaletteBG(200 + i, sky_gradiant_100_coloursPal[paletteNumber] );
			}
			
			//increments for through each value in the sky pallette
			//then decrements to zero and repeats 
			if (isRising)
				{dayNightTimer++;}
			else
				{dayNightTimer--;}
				
			
			if (dayNightTimer == 83)
				isRising = false;
			if (dayNightTimer == 0)
				isRising = true;
		}
		frameCounter++;
	}
	
	Background::Background()
	{
		mountainsXCoord = 0;
		dunesXCoord = 0;
		loadMountains();
		loadDunes();
		loadGround();
	}
	
	void Background::update(bool moving, bool right)
	{	//if the player is moving parallax the background
		if (moving && !right)
			{mountainsXCoord -= 0.25;
			 dunesXCoord -= 0.5;		}
			 
		if (moving && right)
			{mountainsXCoord += 0.25;
			 dunesXCoord += 0.5;		}	 
			
		REG_BG2HOFS = int(mountainsXCoord);
		REG_BG1HOFS = int(dunesXCoord);
	
	}
	
	void Background::moveBackground()
	{	//parallax the background
		mountainsXCoord += 0.25;
		dunesXCoord += 0.5; 	
		REG_BG2HOFS = int(mountainsXCoord);
		REG_BG1HOFS = int(dunesXCoord);
	}
	
	void Background::loadBackground(int arraySize, int screenBlock, int charBlock, int yOffset, uint8_t paletteOffset, const unsigned char externalArray[])
	{	
		int numberOfPixels = 64;
		uint8_t tileArray[arraySize][numberOfPixels];
		uint8_t offset = 0;
		
		for (int i = 0; i < arraySize; ++i)
		{							
			for (int j = 0; j < numberOfPixels; ++j)
			{	
				int position = j + (i * numberOfPixels); 
				if(externalArray[position] == 0){
					offset = 0;
				}
				else {
					offset = paletteOffset;
				}
				tileArray[i][j]= externalArray[position] + offset;
			}
		}
						
		for (int i = 1; i < arraySize+1; ++i)
			{
				LoadTile8(charBlock, i, tileArray[i-1]);
			}
		
		for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					SetTile(screenBlock, x, y+yOffset, x+(y*32)+1);
				}
			}
	}
	
	void Background::loadMountains()
	{	
		int arraySize = 128;
		int screenBlock = 28;
		int charBlock = 1;
		int yOffset = 11;
		uint8_t paletteOffset = 0;
		
		loadBackground(arraySize, screenBlock, charBlock, yOffset, paletteOffset, backgroundmountainsu8Tiles);
	}
	
	void Background::loadDunes()
	{	
		//Set the dunes
		int arraySize = 128;
		int screenBlock = 29;
		int charBlock = 2;
		int yOffset = 13;
		uint8_t paletteOffset = 64;
		
		loadBackground(arraySize, screenBlock, charBlock, yOffset, paletteOffset, backgroundsandu8Tiles);
		
		
	}
	
	void Background::loadGround()
	{
		//Set the ground
		uint8_t groundArray[64];
		uint8_t paletteOffset = 64;
		int screenBlock = 29;
		int charBlock = 2;
		int yOffset = 17;
		
		for (int i = 0; i < 64; ++i)
		{groundArray[i]= groundTiles[i] + paletteOffset;}
		
		
		LoadTile8(charBlock, 130, groundArray);
			
		
		for (int y = yOffset; y < 20; ++y)
		{
			for (int x = 0; x < 32; ++x)
			{
				SetTile(screenBlock, x, y, 130);
			}
		}
	}
		