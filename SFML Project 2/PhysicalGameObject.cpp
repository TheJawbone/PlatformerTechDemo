#include "PhysicalGameObject.h"

PhysicalGameObject::PhysicalGameObject()
{

}

PhysicalGameObject::PhysicalGameObject(sf::Vector2f tbPos, sf::Vector2f tbSize, sf::Vector2f cbPos, sf::Vector2f cbSize)
	:GameObject(tbPos, tbSize, "")
{
	cbPos_ = cbPos;
	cbSize_ = cbSize;
	collisionBox_.setPosition(cbPos_);
	collisionBox_.setSize(cbSize_);
}

PhysicalGameObject::~PhysicalGameObject()
{

}

sf::Vector2f PhysicalGameObject::getCbPos()
{
	return cbPos_;
}

void PhysicalGameObject::setCbPos(sf::Vector2f pos)
{
	cbPos_ = pos;
}

sf::Vector2f PhysicalGameObject::getCbSize()
{
	return cbSize_;
}

void PhysicalGameObject::setPosition()
{
	collisionBox_.setPosition(cbPos_.x, cbPos_.y);
	tbPos_.x = cbPos_.x;
	tbPos_.y = cbPos_.y + (tbSize_.y - cbSize_.y) / 2;
	textureBox_.setPosition(tbPos_.x, tbPos_.y);
}

sf::RectangleShape PhysicalGameObject::getCollisionBox()
{
	return collisionBox_;
}