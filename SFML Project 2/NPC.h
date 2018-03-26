#pragma once
#ifndef NPC_H
#define NPC_H

#include "PhysicalGameObject.h"

class NPC
{
public:
	NPC();
	~NPC();
	bool isClicked(sf::Vector2f mousePos);
};

#endif