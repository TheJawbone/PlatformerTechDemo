#include "Money.h"

Money::Money()
{

}

Money::Money(int value)
{
	value_ = value;
	cbSize_.x = cbSize_.y = 15;
	tbSize_.x = tbSize_.y = 20;
	collisionBox_.setSize(cbSize_);
	collisionBox_.setOrigin(cbSize_.x / 2, cbSize_.y);
	textureBox_.setSize(tbSize_);
	textureBox_.setOrigin(tbSize_.x / 2, tbSize_.y);
	texture_.loadFromFile("Images/money1.png");
	textureBox_.setTexture(&texture_);
}

Money::~Money()
{

}