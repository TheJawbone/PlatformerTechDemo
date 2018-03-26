#pragma once
#ifndef ITEM_H
#define ITEM_H

#include "PhysicalGameObject.h"
#include <list>

enum WeaponType {meleeWeapon, rangedWeapon};

class Item : public PhysicalGameObject
{
protected:
	std::string name_;
	int id_;
	int value_;
public:
	Item();
	Item(sf::Vector2f cbSize, sf::Vector2f tbSize, std::string name, std::string texture, int id, int value);
	Item(Item &item);
	~Item();
	std::string getName();
	int getId();
	virtual int getValue();
};

#endif