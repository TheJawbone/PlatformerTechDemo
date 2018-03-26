#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GlobalParameters.h"
#include <iostream>

class Platform : public GameObject
{
private:
	sf::Vector2f* nodePos_;
	sf::Vector2f* boxNodePos_;
	sf::ConvexShape collisionBox_;
	int nodeCount_;
	int id_;
	float spawnRate_;
	bool passable_;
public:
	Platform();
	Platform(int nodeCount, sf::Vector2f* nodes, int boxNodeCount, sf::Vector2f* boxNodePos, float spawnRate, bool passable);
	~Platform();
	sf::Vector2f getNodePos(int i);
	sf::ConvexShape getCollisionBox();
	int getId();
	int getNodeCount();
	float getSpawnRate();
	bool isPassable();
	float calculateElevation(float playerPosX);
	float calculateInclanation(float playerPosX);
};
#endif
