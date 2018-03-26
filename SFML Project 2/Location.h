#pragma once

#ifndef LOCATION_H
#define LOCATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include "GlobalParameters.h"
#include "Platform.h"
#include "Portal.h"
#include "Ladder.h"
#include "Enemy.h"
#include "Background.h"
#include "Item.h"

class Location
{
private:
	sf::Vector2f size_;
	std::string name_;
	int spawnLimit_;
	int spawnRate_;
	int enemyCount_;
	int* enemyID_;			//ID przeciwników z bazy danych, 1sza wartoœæ to wielkoœæ tablicy
public:
	std::list<Platform*> platformList_;
	std::list<Portal*> portalList_;
	std::list<Ladder*> ladderList_;
	std::list<Enemy*> enemyList_;
	std::list<Item*> itemList_;
	Background* background_;
	sf::RectangleShape minimapBox_;
	Location(std::string);
	~Location();
	void incrementEnemyCount() { enemyCount_++; }
	void decrementEnemyCount() { enemyCount_--; }
	Background* getBackground() { return background_; }
	int getPlatformCount() { return platformList_.size(); }
	std::list<Platform*> getPlatformList() { return platformList_; }
	sf::Vector2f getSize();
	int getEnemyCount() { return enemyCount_; }
	int getSpawnRate() { return spawnRate_; }
	int getSpawnLimit() { return spawnLimit_; }
};
#endif
