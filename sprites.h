/* Font tile data for AG0700.
 */

#ifndef SPRITES_H
#define SPRITES_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "art.h"
#include "gba.h"
#include "tiles.h"
#include "text.h"

void loadSpritePallette();

class SpriteDefines
{	public:
		static const int groundlevel ;
		static const int robotOffset;
		static const int robotLegOffset ;
		static const int gunOffset;
		static const int girlOffset;
		static const int girlLegOffset;
		static const int enemyOffset;
		static const int enemyLegOffset;
		static const int bossOffset;
};
class Sprite
{	
	public:
		int xCoord;
		int yCoord;
		int height;
		int width;
		int arraySize;
		int spriteSize;
		int spriteShape;
		int spriteLocation;
		int paletteOffset;
		bool facingRight;
		bool visible;
		int spriteNumber;
		int frameCounter;
	
		Sprite();
		void keepOnScreen();
		bool collision(Sprite target);
		void loadSprite(const unsigned char externalArray[] );
		void setSprite(int number );
};

class Projectile: public Sprite
{
	public:
		Projectile();
		int intialXCoord;
		int damage;
		bool isFired;
		void update();
		void reset();
		void fire();
		void checkOnScreen();
		
};

class CharacterSprite:	public Sprite
{
	public:
		int legArraySize;
		int legSize;
		int legShape;
		int legLocation;
		int legPaletteOffset; 
		bool isMoving;
		bool isJumping;
		int LegSprite;
		int legOffset;
		int walkCycle[4];
		int walkCyclePointer;
		int health;
		int maxHealth;
		bool isActive;
		float yAcceleration;
		int ySpeed;
		uint8_t horizontalFlip;
		Projectile gunAmmo;
		int gunXCoord;
		int gunYCoord;
		int gunSpriteNumber;
		int gunSpriteLocation;
		
		CharacterSprite();
		void loadLegSprite(const unsigned char externalArray[]);
		void setLegSprite();
		void setCoords();
		void setLeftDirection();
		void setRightDirection();
		void runWalkCycle();
		void takeDamage();
		void gravity();
		void checkHealth();
		void activate();
	
};


class Player : public CharacterSprite
{
	private:
		float yAcceleration;
		int maxJumpSpeed;
		uint8_t previousButton;
		int gunTimer;
		bool gunFacingRight;
		void jump();
		void detectMovement();
		void scrollScreen();
		void checkGunFired();
		void setGunCoords();
		void setGunDirection();
		void loadRobotSprite();
	
	public:	
		Player();
		void activate();
		void update();
};

class Npc	: public CharacterSprite
{
	private:
		int targetPosition;
		int walkSprite1No;
		int ySpeed;
		void moveTowardsTarget();
		bool skipping();
		void loadGirlSprite();
		
	public:
		Npc();
		void update();
		void getXPosition(bool Right, int playerX);
};

class EnemySolder: public CharacterSprite
{
	public:
		int fireRate;
		bool isFiring;
		bool isIndicatorVisible;
		int indicatorSpriteNumber;
		int indicatorX;
		int indicatorY;
		void initliseEnemyLeft();
		void initliseEnemyRight();
		void spawnRobotLeft();
		void spawnRobotRight();
		void fireGun();
		void makeIndicatorVisible();
		void makeIndicatorInvisible();
		void checkstatus();
		void loadEnemySprite();	
		void update(bool playerMoving);
		EnemySolder();
};

class EnemyArray
{
	private:
		int waveNumber;
		int maxWaveNumber;
		int frameCounter;
		
		bool bothRobotsActive();
		void spawnRobots();
		void fireGun();
		void displayInformation();
		
	public:
		EnemyArray();
		void update(bool moving);
		int numberOfEnemies;
		EnemySolder enemies[2];
		bool waveEnded;
};

class BossSprite: public CharacterSprite
{
	private:
		int yStart;
		
		bool attacking;
		void movement();
		void OnScreen();
		void checkTarget(int targetX);
		void checkStatus();
		void takeOff();
		void displayInformation();
		void loadBoss();
		
	public:	
		bool isDefeated;
		BossSprite();
		void update(int targetX);
};

bool collision(Sprite bullet, Sprite target);
void bulletHitDetection(Projectile &bullet, CharacterSprite &target);
void bossHitDetection(CharacterSprite &boss, CharacterSprite &target);

#endif
