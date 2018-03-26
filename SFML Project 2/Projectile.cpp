#include "Projectile.h"

Projectile::Projectile()
{

}

Projectile::Projectile(sf::Vector2f cbPos, sf::Vector2f cbSize, sf::Vector2f tbSize, float velocityX)
{
	travelDistance_ = 0;
	cbPos_ = cbPos;
	cbSize_ = cbSize;
	tbSize_ = tbSize;
	velocity_.x = velocityX;
	collisionBox_.setSize(cbSize_);
	collisionBox_.setPosition(cbPos_);
	collisionBox_.setFillColor(sf::Color::Yellow);
	collisionBox_.setOutlineThickness(1);
	collisionBox_.setOutlineColor(sf::Color::Black);
	texture_.loadFromFile("crap.png");
	collisionBox_.setTexture(&texture_);
}

Projectile::~Projectile()
{

}