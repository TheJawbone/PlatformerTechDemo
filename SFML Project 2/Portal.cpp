#include "Portal.h"

Portal::Portal()
{

}

Portal::Portal(sf::Vector2f portalPos, sf::Vector2f playerPos, std::string destination)
{
	cbPos_ = portalPos;
	playerPos_ = playerPos;
	destination_ = destination;

	collisionBox_.setSize(sf::Vector2f(50, 50));
	collisionBox_.setOrigin(25, 50);
	collisionBox_.setPosition(cbPos_);
	collisionBox_.setFillColor(sf::Color(255, 255, 0, 100));

	textureBox_.setSize(sf::Vector2f(70, 70));
	textureBox_.setOrigin(35, 70);
	textureBox_.setPosition(sf::Vector2f(cbPos_.x, cbPos_.y + 10));
	textureBox_.setFillColor(sf::Color(255, 0, 0, 100));
}

Portal::~Portal()
{

}

void Portal::playSound()
{
	/*if (!soundBuffer_.loadFromFile("portal1.wav"))
		std::cerr << "Unable to loat portal1.wav";
	else
	{
		sound_.setBuffer(soundBuffer_);
		sound_.setVolume(0);
		sound_.play();
	}*/
}