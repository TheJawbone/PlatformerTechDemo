#include "Background.h"

Background::Background()
{
	GameObject* bg = new GameObject(sf::Vector2f(0, 0), sf::Vector2f(1366, 768), "Images/Bg1.png");
	staticBg_.push_back(bg);
}

Background::~Background()
{
	for (std::list<GameObject*>::iterator it = staticBg_.begin(); it != staticBg_.end(); ++it)
		delete (*it);
	for (std::list<GameObject*>::iterator it = dynamicBg_.begin(); it != dynamicBg_.end(); ++it)
		delete (*it);
}

void Background::draw(sf::RenderWindow &window)
{
	for (std::list<GameObject*>::iterator it = staticBg_.begin(); it != staticBg_.end(); ++it)
	{
		window.draw((*it)->textureBox_);
	}
}