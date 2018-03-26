#pragma once
#ifndef CONSUMABLES_H
#define CONSUMABLES_H

#include "Item.h"
#include "Player.h"

struct Effect
{
	int effect_;
	int effectValue_;
};

enum EffectTypes { recHP, damHP };

class Consumables : public Item
{
private:
	std::list<Effect> effects_;
public:
	Consumables();
	Consumables(Item &item, std::list<Effect> effects, std::string texture);
	Consumables(Consumables* item);
	~Consumables();
	void consume(Player* player);
};

#endif