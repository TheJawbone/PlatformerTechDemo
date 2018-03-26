#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "PhysicalGameObject.h"

class Projectile : public PhysicalGameObject
{
public:
	float travelDistance_;
	sf::Vector2f velocity_;
	Projectile();
	Projectile(sf::Vector2f cbPos, sf::Vector2f cbSize, sf::Vector2f tbSize, float velocityX);
	~Projectile();
};

#endif