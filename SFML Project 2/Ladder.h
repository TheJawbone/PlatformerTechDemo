#ifndef LADDER_H
#define LADDER_H

#include "PhysicalGameObject.h"

class Ladder : public PhysicalGameObject
{
public:
	Ladder();
	Ladder(sf::Vector2f cbPos, sf::Vector2f cbSize, sf::Vector2f tbSize);
	~Ladder();
};

#endif