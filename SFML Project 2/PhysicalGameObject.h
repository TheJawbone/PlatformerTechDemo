#pragma once
#ifndef PHYSICALGAMEOBJECT_H
#define PHYSICALGAMEOBJECT_H

#include "GameObject.h"

class PhysicalGameObject : public GameObject
{
protected:
	sf::Vector2f cbPos_;
	sf::Vector2f cbSize_;
	sf::RectangleShape collisionBox_;
public:
	PhysicalGameObject();
	PhysicalGameObject(sf::Vector2f tbPos, sf::Vector2f tbSize, sf::Vector2f cbPos, sf::Vector2f cbSize);
	~PhysicalGameObject();
	sf::Vector2f getCbPos();
	sf::Vector2f getCbSize();
	virtual void setPosition();
	sf::RectangleShape getCollisionBox();
	void setCbPos(sf::Vector2f pos);
};

#endif