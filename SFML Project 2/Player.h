#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <list>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "PhysicalGameObject.h"
#include "Platform.h"
#include "Ladder.h"
#include "GlobalParameters.h"
#include "PlayerHUD.h"
#include "Item.h"
#include "Projectile.h"

class Player : public PhysicalGameObject
{
private:
	std::string name_;
	int level_;
	int hp_;
	int maxHp_;
	int exp_;
public:
	bool isJumping_;
	bool isFalling_;
	bool isOnGround_;
	bool isOnLadder_;
	bool isJumpingDown_;
	bool isInputUp_;
	int fallCounter_;
	int portalCounter_;
	int ladderCounter_;
	int pickCounter_;
	int jumpForce_;
	int moveSpeed_;
	int climbSpeed_;
	int facing_;
	int money_;
	int score_;	//TEMP
	int currentWeapon_;
	float jumpForceMod_;
	float moveSpeedMod_;
	int attackCounter_;
	int attackedCounter_;

	//Temporary melee weapon
	sf::RectangleShape MwBox_;
	sf::Texture MwTexture_;
	sf::Vector2f MwPos_;
	sf::Vector2f MwSize_;

	//Temporary ranged weapon
	sf::RectangleShape RwBox_;
	sf::Texture RwTexture_;
	sf::Vector2f RwPos_;
	sf::Vector2f RwSize_;

	//Player HUD
	PlayerHUD hud_;

	//Projectiles
	std::list<Projectile*> projectileList_;

	//Items
	std::list<Item*> itemList_;

	sf::Vector2f velocity_;
	sf::Sound sound_;
	sf::SoundBuffer soundBuffer_;
	sf::Vector2i spriteSource_;
	Platform* currentPlatform_;
	Ladder* currentLadder_;
	Physics physics_;
	enum spriteDirection { stationaryRight, stationaryLeft, moveRight, moveLeft, onLadder };

	Player();
	Player(sf::Vector2f tbPos, sf::Vector2f tbSize, sf::Vector2f cbPos, sf::Vector2f cbSize);
	~Player();
	void move(int);
	void jump(float mod);
	void fall();
	void decrementCounters(int);
	void addExp(int exp);
	void levelUp();
	void setPosition();
	void setHP(int hp);
	void setSprite();
	void setName(std::string name);
	void getDamaged(int damage);
	int getLevel();
	int getHP();
	int getMaxHP();
	int getExp();
	int getMoney();
	std::string getName();
};
#endif