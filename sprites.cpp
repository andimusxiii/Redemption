#include "sprites.h"
	//Andrew Allan
	//Sprite Class and related functions
	
	 const int SpriteDefines::groundlevel = 152;
	 const int SpriteDefines::robotOffset = 0;
	 const int SpriteDefines::robotLegOffset = 40;
	 const int SpriteDefines::gunOffset = 192;
	 const int SpriteDefines::girlOffset = 24;
	 const int SpriteDefines::girlLegOffset= 56;
	 const int SpriteDefines::enemyOffset= 96;
	 const int SpriteDefines::enemyLegOffset= 80;
	 const int SpriteDefines::bossOffset= 128;
	 
	
	void loadSpritePallette()
	{
		//Set up sprite palette from file
		for (int i = 1; i < 220; ++i){
			SetPaletteObj(i, robot1u16Pal[i]);
		}
	}
	
	Sprite::Sprite()
	{
		frameCounter = 0;
	}
	
	void Sprite::keepOnScreen()
		{	//stop sprite leaving the screen 
			if (xCoord > (SCREEN_WIDTH - width)){
				xCoord = SCREEN_WIDTH - width;
			}
			if (xCoord < 0){
				xCoord = 0;
			}
		}
	
	void Sprite::loadSprite(const unsigned char externalArray[])
	{	//load an array of palette number into charblock 4
	
		int numberOfTiles = 64;	
		uint8_t tileArray[arraySize][numberOfTiles];
			
		//Loads a sprite from an external array into an array
		//and adds a offset so the correct palette is used 
		for (int i = 0; i < arraySize; ++i){
		
			for (int j = 0; j < numberOfTiles; ++j){	
				int position = j + (i * numberOfTiles);
				uint8_t offset= 0;
				
				if (externalArray[position] > 0){
					offset = paletteOffset;
				} else {
					offset = 0;
				}
				 
				tileArray[i][j]= (externalArray[position] + offset);
			}
		}
					
		// Load the tiles for the objects into charblock 4.
		for (int i = spriteLocation; i < (spriteLocation + arraySize); ++i){
			LoadTile8(4, i, tileArray[i - spriteLocation]);
		}	
	}		
			
	void Sprite::setSprite(int number)
	{
		// Sets up the sprite tiles as an object
		SetObject(number,
				  ATTR0_SHAPE(spriteShape) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(yCoord),
				  ATTR1_SIZE(spriteSize) | ATTR1_X(xCoord),
				  ATTR2_ID8(spriteLocation)| ATTR2_PRIO(1));
					  
		if (!visible){
		ObjBuffer[number].attr0 ^= ATTR0_HIDE;
		}			
	}
	
	
		
	Projectile::Projectile()
	{ 
		yCoord = 180;
		height = 4;
		width = 8;
		spriteSize = 0;
		spriteShape = 0;
		spriteLocation = 250;
		paletteOffset = 0;
		damage = 1;
		visible = false;
		facingRight = false;
		isFired = false;
		LoadTile8(4, 250, bulletTile);	
	}
	
		
		
	void Projectile::update()
	{	
		if (isFired){   
						
			if (facingRight){
				xCoord += 3;
			} else {
				xCoord -= 3;
			}
		}
			
		checkOnScreen();
		SetObjectX(spriteNumber,  xCoord);
		SetObjectY(spriteNumber,  yCoord);
	}
		
	void Projectile::reset()
	{
		ObjBuffer[spriteNumber].attr0 = ATTR0_HIDE;
		isFired = false;
		xCoord = 0;
		yCoord = 180;
	}
		
	void Projectile::fire()
	{   
		setSprite(spriteNumber);
		ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
		isFired = true;
		intialXCoord = xCoord;
	}
		
	void Projectile:: checkOnScreen()
	{	
		//removes the projectile when it leaves the screen
		if ((xCoord > (SCREEN_WIDTH - width)) || xCoord < 0){
			reset();
		}
	}
		
	CharacterSprite::CharacterSprite()
	{
		walkCyclePointer = 0;
		isJumping = false;
		isMoving = false; 
	}
		
	void CharacterSprite::setCoords()
	{//draws object on screen
		SetObjectX(spriteNumber,  xCoord);
		SetObjectX(LegSprite,  xCoord);
		SetObjectY(spriteNumber,  yCoord);
		SetObjectY(LegSprite,  (yCoord + legOffset));
	}
		
	void CharacterSprite::setLeftDirection()
	//Sets which way the player is facing
	{
		if (facingRight){
			ObjBuffer[spriteNumber].attr1^= ATTR1_HFLIP;
			ObjBuffer[LegSprite].attr1 ^= ATTR1_HFLIP;
			facingRight = false;
		}
	}
		
	void CharacterSprite::setRightDirection()
	//Sets which way the player is facing
	{
		if (!facingRight){
			ObjBuffer[spriteNumber].attr1^= ATTR1_HFLIP;
			ObjBuffer[LegSprite].attr1 ^= ATTR1_HFLIP;
			facingRight = true;
		}
	}
		
	void CharacterSprite::runWalkCycle()
	{	//changes the leg sprite for the walk cycle
		if (frameCounter % 8 == 0){
			
			if (isJumping){
				walkCyclePointer = 3;
			}
			else if (!isMoving){
				walkCyclePointer = 0;
			}
			else {
				walkCyclePointer++;
				if (walkCyclePointer > 3){
					walkCyclePointer = 0;	
				}		
			}
		}
		ObjBuffer[LegSprite].attr2 = ATTR2_ID8(walkCycle[walkCyclePointer]);
	}
		
	void CharacterSprite::takeDamage()
	{
		health--;
	}
		
	void CharacterSprite::checkHealth()
	{
		if (health == 0){
			//hides sprite
			ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
			ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
			xCoord = 0;
			yCoord = 0;
			isActive = false;
		}
	}
		
	void CharacterSprite::gravity()
	{	//accellerates the y speed downwards while not at ground level
		if (isJumping){
			yAcceleration += 0.1;
			yCoord += int(yAcceleration);	
		}  
						 
		if (yCoord >= (SpriteDefines::groundlevel - height)){
			yCoord = SpriteDefines::groundlevel - height;
			isJumping = false;
			yAcceleration = 0;
		}
	}
	
	void CharacterSprite::activate()
	{
		isActive =true;
		health = maxHealth;
		
		ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
		ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
	}
	
		
	Player::Player()
	//Intialise values
	{	spriteNumber = 1;
	    LegSprite = spriteNumber + 1;
		facingRight = true;
		gunFacingRight = true;
		height = 64;
		width = 32;
		visible = false;
		isActive = false;
		xCoord = 0;
		yCoord = SpriteDefines::groundlevel - height;
		legOffset = 48;
		maxHealth = 100000;
		yAcceleration = 0;
		maxJumpSpeed = 6;
		previousButton = 0;
		horizontalFlip = 0;
		walkCyclePointer = 0;
		walkCycle[0] = 32;
		walkCycle[1] = 40;
		walkCycle[2] = 32;
		walkCycle[3] = 48;
		gunAmmo.spriteNumber = 21;
		gunSpriteNumber = 0;
		gunSpriteLocation = 224;
		loadRobotSprite();			
	}
		
	void Player::jump()
	{	//detect if jump is pushed
		if (((REG_P1 & KEY_B) == 0) && ((REG_P1 & KEY_B) != (previousButton & KEY_B)) && !isJumping) {
			isJumping = true;
			yAcceleration = maxJumpSpeed * -1;
		}
		// add to the acceleration, evenutly going positive and brings player back to ground level
		if (isJumping){
			yCoord += int(yAcceleration);
			yAcceleration += 0.5;
		}  
				 
		if (yCoord >= (SpriteDefines::groundlevel - height)){
			yCoord = SpriteDefines::groundlevel - height;;
			isJumping = false;
		}
		previousButton	= REG_P1;					 
	}
		
	void Player::detectMovement()
	{//checks for user input 	
		isMoving = false;
		if ((REG_P1 & KEY_LEFT) == 0){
			--xCoord;
			setLeftDirection();
			isMoving = true;	
		}
		if ((REG_P1 & KEY_RIGHT) == 0){
			++xCoord;	
			setRightDirection();	
			isMoving = true;	
		}
	}
		
	void Player::scrollScreen()
	{	
		// keeps the player from the edge of the screen
		if (xCoord > (SCREEN_WIDTH - (width * 2))){
			xCoord = SCREEN_WIDTH - (width * 2);
		}
		if (xCoord < (32)){
			xCoord = 32;
		}
	}
		
	void Player::checkGunFired()
	{	
		gunTimer--;
		if (((REG_P1 & KEY_A) == 0) && !gunAmmo.isFired && gunTimer < 1){	
			gunTimer = 30;
			gunAmmo.xCoord = xCoord;
			gunAmmo.yCoord = yCoord + 23;
			gunAmmo.facingRight = facingRight;
			gunAmmo.fire();
		}
	
		if (gunAmmo.isFired == true){
			gunAmmo.update();
		}
	}
	
	void Player::activate()
	{
		isActive =true;
		health = maxHealth;
		
		ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
		ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
		ObjBuffer[gunSpriteNumber].attr0 ^= ATTR0_HIDE;
	}
	
	
	void Player::setGunCoords()
	{	
		int offset = 10;
		if (!gunFacingRight){
			offset = -10;
		}
		
		SetObjectX(gunSpriteNumber,  xCoord + offset);
		SetObjectY(gunSpriteNumber,  yCoord + 18);
	}
	
		
	void Player::setGunDirection()
	//Sets which way the player is facing
	{
		if (gunFacingRight != facingRight ){
			ObjBuffer[gunSpriteNumber].attr1^= ATTR1_HFLIP;
			gunFacingRight = facingRight;
		}
		
	}

	void Player::update()
	//Detects Joypad and moves character accordingly	
	{	
		jump();
		checkGunFired();
		detectMovement();
		runWalkCycle();
		scrollScreen();	
		setCoords();
		setGunDirection();
		setGunCoords();
		frameCounter++;
	}
		
	void Player::loadRobotSprite()
	{
		arraySize = 32;
		spriteSize = 3;
		spriteShape = 2;
		spriteLocation = 0;
		paletteOffset =  SpriteDefines::robotOffset;
		loadSprite(robot1u8Tiles);
		setSprite(spriteNumber);
			
		arraySize = 8;
		spriteSize = 2;
		spriteShape = 1;
		spriteLocation = 32;
		loadSprite(robotIdleLegsu8Tiles);
		setSprite(LegSprite);
		
		spriteLocation = 40;
		paletteOffset = SpriteDefines::robotLegOffset; 
		loadSprite(robotleg1u8Tiles);
		
		spriteLocation = 48;
		loadSprite(robotleg2u8Tiles);
		
		spriteLocation = gunSpriteLocation;
		paletteOffset = SpriteDefines::gunOffset;
		loadSprite(gunu8Tiles);
		setSprite(gunSpriteNumber);
		
	}
		
	Npc::Npc()
	{
		spriteNumber = 3;
		LegSprite = spriteNumber + 1;
		legOffset = 24;
		height = 32;
		width = 16;
		visible = false;
		xCoord = 100;
		yCoord = SpriteDefines::groundlevel - height;
		maxHealth = 1;
		health = maxHealth;
		isMoving = false; 
		facingRight = false;
		isActive = false;
		targetPosition = 0;
		horizontalFlip = 0;
		walkCycle[0] = 64;
		walkCycle[1] = 64;
		walkCycle[2] = 68;
		walkCycle[3] = 68;
		loadGirlSprite();
	}
		
	void Npc::getXPosition(bool Right, int playerX)
	//find an x value behind the player
	{
		if (Right){
			targetPosition = playerX - 20;
		}
		else{
			targetPosition = playerX + 35;
		}
	}
		
	bool Npc::skipping()
	{// checks the position of the walk cycle
		if ((walkCyclePointer == 2) || (walkCyclePointer == 3)){
			return true;
		}
		else {
			return false;}
	}
		
	void Npc::moveTowardsTarget()
	{
		if (frameCounter % 3 == 0){
				
				isMoving = false;	
				if (xCoord > targetPosition){
					xCoord--;
					setLeftDirection(); 
					isMoving = true;
				}
				else if (xCoord < targetPosition){
					xCoord++;
					setRightDirection(); 
					isMoving = true;
				} 
		}
	}
		
	void Npc::update()
	//moves player towards target position
	{	
		moveTowardsTarget();
		runWalkCycle();
		keepOnScreen();	
			
		if(skipping()){
			yCoord-=2;
		}
		setCoords();
			if(skipping()){
				yCoord+=2;
			}
			checkHealth();
			frameCounter++;
	}
	
    void Npc::loadGirlSprite()
	{
		arraySize = 8;
		spriteSize = 2;
		spriteShape = 2;
		spriteLocation = 56;
		paletteOffset =  SpriteDefines::girlOffset;	
		loadSprite(smallergirlu8Tiles);
		setSprite(spriteNumber);
		arraySize = 2;
		spriteSize = 0;
		spriteShape = 1;
		spriteLocation = 64;
		loadSprite(smallergirlidleu8Tiles);
		setSprite(LegSprite);
		spriteLocation = 68;
		paletteOffset =  SpriteDefines::girlLegOffset;
		loadSprite(smallergirlskippingu8Tiles);
	}
	
	EnemySolder::EnemySolder()
	{
		width = 32;
		height = 46;
		visible = false;
		maxHealth = 10;
		legOffset = 30;
		walkCycle[0] = 86;
		walkCycle[1] = 94;
		walkCycle[2] = 86;
		walkCycle[3] = 102;
		isMoving = false;
		walkCyclePointer = 0;
		yCoord = 0;
		isActive = false;
		isFiring = false;
		isIndicatorVisible = false;
		fireRate = 200;	
		loadEnemySprite();
		LoadTile8(4, 240, exclamationu8Tiles);
	}
	
	void EnemySolder::initliseEnemyLeft()
	{
		spriteNumber = 10;
		LegSprite = spriteNumber +1;
		gunAmmo.spriteNumber = 30;
		spriteLocation = 70;
		spriteSize = 2;
		spriteShape = 0;
		setSprite(spriteNumber);
		spriteSize = 2;
		spriteShape = 1;
		spriteLocation = 86;
		setSprite(LegSprite);
		facingRight = true;
		
		indicatorSpriteNumber = 18;
		spriteLocation = 240;
		spriteSize = 0;
		spriteShape = 0;
		setSprite(indicatorSpriteNumber);
		
		
	}
	
	void EnemySolder::initliseEnemyRight()
	{
		spriteNumber = 12;
		LegSprite = spriteNumber + 1;
		gunAmmo.spriteNumber = 31;
		spriteLocation = 70;
		spriteSize = 2;
		spriteShape = 0;
		setSprite(spriteNumber);
		spriteSize = 2;
		spriteShape = 1;
		spriteLocation = 86;
		setSprite(LegSprite);
		facingRight = false;
		
		indicatorY = xCoord - 10;
		ObjBuffer[spriteNumber].attr1^= ATTR1_HFLIP;
		ObjBuffer[LegSprite].attr1^= ATTR1_HFLIP;
		
		indicatorSpriteNumber = 19;
		spriteLocation = 240;
		spriteSize = 0;
		spriteShape = 0;
		setSprite(indicatorSpriteNumber);
		
	}
	
	void EnemySolder::spawnRobotLeft()
	{
		isActive = true;
		xCoord = 0;
		yCoord = 0;
		ySpeed = 0;
		yAcceleration = 0;
		isJumping = true;
		health = maxHealth;
		ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
		ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
	}
	
	void EnemySolder::spawnRobotRight()
	{
		isActive = true;
		xCoord = 208; 
		yCoord = 0; 
		ySpeed = 0;
		yAcceleration = 0;
		isJumping = true;
		health = maxHealth;
		ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
		ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
	} 
	void EnemySolder::fireGun()
	{	
		if ((isFiring) && (!gunAmmo.isFired) && (frameCounter % 120 == 0)){
			isFiring = false;
			makeIndicatorInvisible();
			gunAmmo.xCoord = xCoord;
			gunAmmo.yCoord = yCoord + 17;
			gunAmmo.facingRight = facingRight;
			gunAmmo.fire();	
		}
	
		if (gunAmmo.isFired == true){
				gunAmmo.update();
		}
	}	
	
	void EnemySolder::makeIndicatorVisible()
	{	
		isIndicatorVisible = true;
		indicatorX = xCoord + (width / 2);
		indicatorY = yCoord - (height / 5);
		ObjBuffer[indicatorSpriteNumber].attr0 ^= ATTR0_HIDE;
		SetObjectX(indicatorSpriteNumber,  indicatorX);
		SetObjectY(indicatorSpriteNumber,  indicatorY);
	}
	
	void EnemySolder::makeIndicatorInvisible()
	{
		isIndicatorVisible = false;
		ObjBuffer[indicatorSpriteNumber].attr0 ^= ATTR0_HIDE;
	}
	
	void EnemySolder::update(bool playerMoving)
	{	
		frameCounter++;
		
		if (isActive){	
			checkstatus();	
			if (playerMoving){
				isMoving = true;
			} 
			else{
				isMoving = false;
			}
				
			gravity();
			runWalkCycle();
			fireGun();		
			setCoords();
		}
	}	
	
	void EnemySolder::checkstatus()
	{//checks health
		if (health < 1){
			isActive = false;
			ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
			ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
			gunAmmo.reset();
			isFiring = false;
			if(isIndicatorVisible){
				makeIndicatorInvisible();
			}
		}
	}
	
	void EnemySolder::loadEnemySprite()
	{
		arraySize = 16;
		spriteLocation = 70;
		paletteOffset =  SpriteDefines::enemyOffset;
		loadSprite(enemyrobotu8Tiles);
		
		arraySize = 8;
		spriteLocation = 86;
		paletteOffset =  SpriteDefines::enemyLegOffset; 	
		
		loadSprite(enemyIdleLegsu8Tiles);
		spriteLocation = 94;
		loadSprite(enemyMovingLegs1u8Tiles);
		spriteLocation = 102;
		loadSprite(enemyMovingLegs2u8Tiles);
	}
	
	EnemyArray::EnemyArray()
	{	
		waveNumber = 0;
		maxWaveNumber =0;
		numberOfEnemies = 2;
		frameCounter = 0;
		waveEnded = false;
		enemies[0].initliseEnemyLeft();
		enemies[1].initliseEnemyRight();
	}
	
	void EnemyArray::update(bool moving)
	{	
		displayInformation();
		spawnRobots();
		if (!enemies[0].isJumping && !enemies[1].isJumping){
			fireGun();
		}
		enemies[0].update(moving);
		enemies[1].update(moving);
		frameCounter++;
	}
	
		
	void EnemyArray::spawnRobots()
	{
		if (!enemies[0].isActive && !enemies[1].isActive){
			waveNumber++;
			if (waveNumber > maxWaveNumber){
				waveEnded = true;
			}
			else{
				 enemies[0].spawnRobotLeft();
				 enemies[1].spawnRobotRight();
			}
		}
	}
	
	void EnemyArray::fireGun()
	{	//alternates between 2 enemies firing
		if (!enemies[0].isFiring && !enemies[1].isFiring && bothRobotsActive()){
		
			if (rand() % 2 == 0){
				enemies[0].isFiring = true;
				enemies[0].makeIndicatorVisible();
				enemies[1].isFiring = false;
			}
			else{
				enemies[1].isFiring = true;
				enemies[1].makeIndicatorVisible();
				enemies[0].isFiring = false;
			}
		}
	}
	
	void EnemyArray::displayInformation()
	{	//display text on screen
		clearText();
		char buffer[32];
		snprintf(buffer, sizeof buffer, "Enemy Waves Left: %d", (maxWaveNumber - waveNumber+ 1));
		drawText(0, 0, buffer);
		snprintf(buffer, sizeof buffer, "Health: %d", enemies[0].health );
		drawText(0, 1, buffer);
		snprintf(buffer, sizeof buffer, " Health: %d", enemies[1].health );
		drawText(19, 1, buffer);
	}
	
	bool EnemyArray::bothRobotsActive()
	{
		if (enemies[0].isActive && enemies[1].isActive)
			return true;
		else return false;
	}
	
	BossSprite::BossSprite()
	{ 		
		spriteNumber = 6;
	    LegSprite = spriteNumber + 1;
		frameCounter = 0;
		facingRight = true;
		isJumping = true;
		yStart = 2;
		height = 64;
		width = 64;
		visible = false;
		xCoord = 50;
		yCoord = yStart;
		yAcceleration = 0;
		ySpeed = 0;
		legOffset = 32;
		maxHealth = 12;
		health = maxHealth;
		walkCyclePointer = 0;
		walkCycle[0] = 32;
		walkCycle[1] = 40;
		walkCycle[2] = 32;
		walkCycle[3] = 48;
		isActive =false;
		attacking = false;
		isDefeated = false;
		loadBoss();
	}
	
	void BossSprite::movement()
	{	
		if (frameCounter % 2 == 0){
				
			if (!attacking && (yCoord = yStart)){
					
				if (facingRight){
					xCoord++;
				}
				else {
					xCoord--;
				}
			}
				
			if (attacking && yCoord < SpriteDefines::groundlevel - height){
				gravity();
			}	
			if (!isJumping){
				takeOff();
			}
		}			  
	}
	
	void BossSprite::takeOff()
	{//if at ground level, accelerate upwards
		if(yCoord > yStart){
			yAcceleration += 0.1;
			yCoord -= int(yAcceleration);	
		}
		else{ 
			yCoord = yStart;
			isJumping = true;
			attacking = false;
			yAcceleration = 0;	
		}			 
	}
	
	void BossSprite::displayInformation()
	{
		clearText();
		char buffer[32];
		snprintf(buffer, sizeof buffer, "Boss Health: %d", health );
		drawText(0, 0, buffer);
		
	}
	
	void BossSprite::OnScreen()
	{
		if (xCoord > (SCREEN_WIDTH - width)){
			facingRight = false;
		}
		if (xCoord < 1){
			facingRight = true;
			xCoord = 1;
		}
	}
	
	void BossSprite::checkTarget(int targetX)
	{//checks if the girl is directly below the boss
		if ((xCoord == targetX) && (yCoord == yStart)){
			attacking = true;
		}
	}
	
	void BossSprite::checkStatus()
	{
		if (health < 1){
			isActive = false;
			isDefeated = true;
			 ObjBuffer[spriteNumber].attr0 ^= ATTR0_HIDE;
			ObjBuffer[LegSprite].attr0 ^= ATTR0_HIDE;
		}
	}
	
	void BossSprite::update(int targetX)
	{	
		displayInformation();
		checkStatus();
		checkTarget(targetX);
		OnScreen();movement();
		runWalkCycle();
		setCoords();
		frameCounter++;
	}
	
	void BossSprite::loadBoss()
	{
		arraySize = 64;
		spriteSize = 3;
		spriteShape = 0;
		spriteLocation = 120;
		paletteOffset =  SpriteDefines::bossOffset;
		loadSprite(boss2u8Tiles);
		setSprite(spriteNumber);
		spriteLocation = 251;
		LoadTile8(4, spriteLocation, BlankTile);	
		spriteSize = 0;
		spriteShape = 0;
		setSprite(LegSprite);
	}
	
	bool collision(Sprite bullet, Sprite target)
	{//check for collisons between 2 sprites
	 // offset makes sure there is visable overlap between sprites
		int offset = 4;
	
		if ((bullet.xCoord + bullet.width - offset) < (target.xCoord))
			return false;
		if ((bullet.xCoord ) > (target.xCoord +(target.width))- offset)	
			return false;
		if ((bullet.yCoord +(bullet.height)) < (target.yCoord ))
			return false;
		if ((bullet.yCoord ) > (target.yCoord +(target.height)))	
			return false;
			
			return true;
	}
	
	void bulletHitDetection(Projectile &bullet, CharacterSprite &target)
	{//collision detection between a projectile and a character
		if (collision(bullet, target)){
			target.takeDamage();
			bullet.reset();
		}
	}  
	
	void bossHitDetection(CharacterSprite &boss, CharacterSprite &target)
	{//collision detection between the boss and a character
		if (collision(boss, target)){
			target.takeDamage();
			boss.isJumping = false;
		}
	}  
	