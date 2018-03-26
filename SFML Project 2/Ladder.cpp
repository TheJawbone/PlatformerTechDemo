#include "Ladder.h"

Ladder::Ladder()
{

}

Ladder::Ladder(sf::Vector2f cbPos, sf::Vector2f cbSize, sf::Vector2f tbSize)
{
	cbSize_ = cbSize;
	cbPos_ = cbPos;
	collisionBox_.setSize(cbSize_);
	collisionBox_.setOrigin(collisionBox_.getSize().x / 2, 0);
	collisionBox_.setPosition(cbPos_);
	collisionBox_.setFillColor(sf::Color::Blue);

	tbSize_ = tbSize;
	tbPos_.x = cbPos_.x;
	textureBox_.setSize(tbSize_);
	tbPos_.y = cbPos_.y - (0.5 * (tbSize_.y - cbSize_.y));
	textureBox_.setOrigin(textureBox_.getSize().x / 2, 0);
	textureBox_.setPosition(tbPos_);
	textureBox_.setFillColor(sf::Color(100, 100, 100, 100));
}

Ladder::~Ladder()
{

}