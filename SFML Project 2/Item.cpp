#include "Item.h"

Item::Item()
{

}

Item::Item(sf::Vector2f cbSize, sf::Vector2f tbSize, std::string name, std::string texture, int id, int value)
{
	cbSize_ = cbSize;
	tbSize_ = tbSize;
	collisionBox_.setSize(cbSize_);
	collisionBox_.setOrigin(cbSize_.x / 2, cbSize_.y);
	textureBox_.setSize(tbSize_);
	textureBox_.setOrigin(tbSize_.x / 2, tbSize_.y);
	texture_.loadFromFile("Images/" + texture);
	textureBox_.setTexture(&texture_);
	name_ = name;
	id_ = id;
	value_ = value;
}

Item::Item(Item &item)
{
	cbSize_ = item.cbSize_;
	tbSize_ = item.tbSize_;
	collisionBox_ = item.collisionBox_;
	textureBox_ = item.textureBox_;
	name_ = item.name_;
	id_ = item.id_;
	value_ = item.value_;
}

Item::~Item()
{

}

std::string Item::getName()
{
	return name_;
}

int Item::getId()
{
	return id_;
}

int Item::getValue()
{
	return value_;
}