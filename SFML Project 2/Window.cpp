#include "Window.h"

Window::Window()
{
	window_.set(sf::Vector2f(0, 0), sf::Vector2f(400, 200), "");
	exitButton_.set(sf::Vector2f(window_.getTbPos().x + window_.getTbSize().x - 10, window_.getTbPos().y), sf::Vector2f(10, 10), "");
	exitButton_.textureBox_.setFillColor(sf::Color::Red);
	arial_.loadFromFile("Fonts/arial.ttf");
}

Window::Window(sf::Vector2f size)
{

}

Window::~Window()
{

}

void Window::setPosition()
{

}

void Window::toggleVisibility()
{
	if (!visible_)
		visible_ = true;
	else if (visible_)
		visible_ = false;
}

void Window::update(std::list<Item*> items)
{
	//Rewrite names and quantities of items into structure vector from player's item list
	std::vector<Record>::iterator it2 = items_.begin();
	for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it)
	{
		//If there are no items of the kind currently in the window - create new record
		if (!std::any_of(items_.begin(), items_.end(), [&](Record record)->bool {return record.itemName == (*it)->getName(); }))
		{
			Record record;
			record.itemName = (*it)->getName();
			record.itemQuantity = 1;
			items_.push_back(record);
		}
		//If there is a duplicate item, increase it's count
		else
		{
			it2 = std::find_if(items_.begin(), items_.end(), [&](Record record)->bool {return record.itemName == (*it)->getName(); });
			it2->itemQuantity++;
		}
	}

	//Generate texts to display in window
	sf::Vector2f pos;
	pos.x = window_.getTbPos().x + 20;
	pos.y = window_.getTbPos().y + 20;
	for (std::vector<Record>::iterator it = items_.begin(); it != items_.end(); ++it)
	{
		it->text.setCharacterSize(12);
		std::string itemDesc = it->itemName + "(" + std::to_string(it->itemQuantity) + ")";
		it->text.setString(itemDesc);
		it->text.setFillColor(sf::Color::Black);
		it->text.setPosition(pos);
		it->text.setFont(arial_);
		pos.y += 20;

		//Reset item quantity
		it->itemQuantity = 0;
	}
}

void Window::draw(sf::RenderWindow &window)
{
	if (visible_)
	{
		window.draw(window_.textureBox_);
		window.draw(exitButton_.textureBox_);
		for (std::vector<Record>::iterator it = items_.begin(); it != items_.end(); ++it)
			window.draw(it->text);
	}
}