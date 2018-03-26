#include "Consumables.h"

Consumables::Consumables()
{

}

Consumables::Consumables(Item &item, std::list<Effect> effects, std::string texture)
{
	cbSize_ = item.getCbSize();
	tbSize_ = item.getTbSize();
	collisionBox_ = item.getCollisionBox();
	textureBox_ = item.getTextureBox();
	texture_.loadFromFile("Images/" + texture);
	textureBox_.setTexture(&texture_);
	id_ = item.getId();
	name_ = item.getName();
	value_ = item.getValue();
	effects_ = effects;
}

Consumables::Consumables(Consumables* item)
{
	cbSize_ = item->getCbSize();
	tbSize_ = item->getTbSize();
	collisionBox_ = item->getCollisionBox();
	textureBox_ = item->getTextureBox();
	id_ = item->getId();
	name_ = item->getName();
	value_ = item->getValue();
	effects_ = item->effects_;

}

Consumables::~Consumables()
{
	
}

void Consumables::consume(Player* player)
{
	for (std::list<Effect>::iterator it = effects_.begin(); it != effects_.end(); ++it)
	{
		switch (it->effect_)
		{
		case recHP:
			player->setHP(player->getHP() + it->effectValue_);
			if (player->getHP() > player->getMaxHP())
				player->setHP(player->getMaxHP());
			break;
		default:
			break;
		}
	}
}