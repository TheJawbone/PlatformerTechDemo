#pragma once
#ifndef PLAYERHUD_H
#define PLAYERHUD_H

#include "GameObject.h"
#include <string>
#include <iostream>

class PlayerHUD
{
private:
	sf::Text name_;
	sf::Text level_;
	sf::Text hp_;
	sf::Text exp_;
	sf::Text money_;
	GameObject mainTb_;
	GameObject healthBar_;
	GameObject healthBarBorder_;
	GameObject expBar_;
	GameObject expBarBorder_;
public:
	PlayerHUD();
	~PlayerHUD();
	void update(std::string name, int level, int maxHp, int hp, int exp, int money);
	void setUp(std::string name, int level, int hp, int maxHp, int exp, int money);
	void setPosition(sf::Vector2u screenSize);
	void setFont(sf::Font &font);
	void draw(sf::RenderWindow &window, sf::Font &font);
};

#endif