#pragma once
#ifndef MONEY_H
#define MONEY_H

#include "Item.h"

class Money : public Item
{
public:
	Money();
	Money(int value);
	~Money();
	int getValue() { return value_; }
};

#endif