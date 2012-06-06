#include <stdint.h>
#include <stdlib.h>
#include "gba.h"
#include "font.h"
#include "art.h"
#include "sprites.h"
#include "tiles.h"
#include "text.h"

int main()
{	
	//initilise
	ClearObjects();
	setModeZero();
	
	loadSpritePallette();
	loadBackgroundPallette();
	loadAlphabet(); 
	
	DynamicSky dayNightCycle ;
	Background background ;
	
	Text text;
	GameState game;

	//displays start screen
	text.startScreen();

	while (true)//game loop
	{	
		//declarations
		Player robot;
		Npc girl; 
		EnemyArray enemy;
		BossSprite boss;
		game = START;
		
		while (game != END)
		{
			dayNightCycle.update();
			switch(game){
				case START: {	//waits on player to start the game
								
								text.checkForStart();
								background.moveBackground();
								if (text.startGame){
									game = LEVEL;
									//initlises player & enemies
									clearText();
									robot.activate();
									girl.activate();
									
								}
							}
					break;
				case LEVEL: {	
								//Hit Detection	
								for (int i = 0; i< enemy.numberOfEnemies; i++)
								{	if ((enemy.enemies[i]).isActive){
										bulletHitDetection(robot.gunAmmo, enemy.enemies[i]);
										bulletHitDetection(enemy.enemies[i].gunAmmo, girl);
										bulletHitDetection(enemy.enemies[i].gunAmmo, robot);
									}
								}
								//Update Sprites
								background.update(robot.isMoving, robot.facingRight);
								robot.update();
								girl.getXPosition(robot.facingRight,robot.xCoord);
								girl.update();
								enemy.update(robot.isMoving);	
								
								//checks girl is alive
								if (!girl.isActive){
									game = RESULTS;
								}	
								
								if (enemy.waveEnded){
									game = BOSS;
									clearText();
									boss.activate();
								}
							}
					break;
				case BOSS: {	
								//hit detection
								bossHitDetection(boss, girl);
								bossHitDetection(boss, robot);
								bulletHitDetection(robot.gunAmmo, boss);
								
								//update
								boss.update(girl.xCoord);
								robot.update();
								girl.getXPosition(robot.facingRight,robot.xCoord);
								girl.update();
								
								if (!girl.isActive || boss.isDefeated){
									game = RESULTS;
								}	
								
							}
					break;
				case RESULTS: {	
								clearText();	
								ClearObjects();
								if (boss.isDefeated){
									text.winScreen();
								}
								else{
									text.gameOverScreen();				
								}
								
								text.startGame = false;
								game = END;
							}
					break;
			}
			WaitVSync();
			UpdateObjects();
		}
	
		
	}
	return 0;
}

