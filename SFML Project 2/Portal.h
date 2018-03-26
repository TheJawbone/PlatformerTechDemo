#pragma once
#ifndef PORTAL_H
#define PORTAL_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "PhysicalGameObject.h"
#include "GlobalParameters.h"

class Portal : public PhysicalGameObject
{
private:
	sf::Vector2f playerPos_;
	sf::SoundBuffer soundBuffer_;
	sf::Sound sound_;
	std::string destination_;
public:
	Portal();
	Portal(sf::Vector2f, sf::Vector2f, std::string);
	~Portal();
	std::string getDestination() { return destination_; }
	sf::Vector2f getPlayerPos() { return playerPos_; }
	void playSound();
};

#endif
