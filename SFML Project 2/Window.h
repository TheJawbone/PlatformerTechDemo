#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "GameObject.h"
#include "Item.h"
#include <vector>
#include <list>
#include <string>
#include <algorithm>

class Window
{
private:
	struct Record
	{
		std::string itemName;
		int itemQuantity;
		sf::Text text;
	};
	GameObject window_;
	GameObject exitButton_;
	sf::Font arial_;
	std::vector<sf::Text> itemTexts_;
	std::vector<Record> items_;
public:
	bool visible_;

	Window();
	Window(sf::Vector2f size);
	~Window();
	void setPosition();
	void toggleVisibility();
	void update(std::list<Item*> items);
	void draw(sf::RenderWindow &window);
};

#endif