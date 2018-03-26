#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>
#include "Platform.h"
#include "PhysicalGameObject.h"

class Enemy : public PhysicalGameObject
{
private:
	sf::RectangleShape healthBar_;
	sf::RectangleShape healthBarBorder_;
public:
	int movementCounter_;
	int moveDirection_;				//0 - left, 1 - right
	int mobility_;					//determines the maximum amount of steps that enemy can make on one movement seed
	int exp_;
	int attackedCounter_;
	float moveSpeed_;
	float activeness_;				//affects probability of generating movement seed for the enemy
	float HP_;
	float maxHP_;
	float attack_;
	std::string name_;
	Platform* currentPlatform_;

	Enemy();
	Enemy(std::string name, sf::Vector2f cbSize, sf::Vector2f tbSize, float moveSpeed, float activeness, int mobility, int maxHP, int exp, float attack);
	Enemy operator=(Enemy &enemy);
	void generateMovement(int movSeed, int actSeed, int direction);
	void move();
	void setPosition();
	void setHealthBar();
	std::string getName();
	sf::RectangleShape getHealthBar();
	sf::RectangleShape getHealthBarBorder();
};

#endif