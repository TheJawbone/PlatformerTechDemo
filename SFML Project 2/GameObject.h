#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SFML/Graphics.hpp"

class GameObject
{
protected:
	sf::Vector2f tbPos_;
	sf::Vector2f tbSize_;
	sf::Texture texture_;
public:
	sf::RectangleShape textureBox_;
	GameObject();
	GameObject(sf::Vector2f tbPos, sf::Vector2f size, std::string texture);
	~GameObject();
	void set(sf::Vector2f tbPos, sf::Vector2f size, std::string texture);
	void setTbPos(sf::Vector2f pos);
	sf::Vector2f getTbPos();
	sf::Vector2f getTbSize();
	virtual sf::RectangleShape getTextureBox();
	virtual void setSprite() {};
	virtual void draw() {}
};

#endif
