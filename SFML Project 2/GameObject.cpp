#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(sf::Vector2f tbPos, sf::Vector2f tbSize, std::string texture)
{
	set(tbPos, tbSize, texture);
}

GameObject::~GameObject()
{

}

void GameObject::set(sf::Vector2f tbPos, sf::Vector2f tbSize, std::string texture)
{
	tbPos_ = tbPos;
	tbSize_ = tbSize;
	textureBox_.setPosition(tbPos);
	textureBox_.setSize(tbSize);
	texture_.loadFromFile(texture);
	textureBox_.setTexture(&texture_);
}

void GameObject::setTbPos(sf::Vector2f pos)
{
	tbPos_ = pos;
	textureBox_.setPosition(tbPos_);
}

sf::Vector2f GameObject::getTbPos()
{
	return tbPos_;
}

sf::Vector2f GameObject::getTbSize()
{
	return tbSize_;
}

sf::RectangleShape GameObject::getTextureBox()
{
	return textureBox_;
}