#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "SFML/Graphics.hpp"
#include <list>
#include "GameObject.h"

class Background
{
private:
	std::list<GameObject*> staticBg_;
	std::list<GameObject*> dynamicBg_;
public:
	Background();
	~Background();
	void draw(sf::RenderWindow &window);
};

#endif
