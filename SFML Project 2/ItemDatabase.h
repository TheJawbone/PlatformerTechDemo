#pragma once
#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include "Item.h"
#include "Consumables.h"
#include <fstream>
#include <iostream>
#include <vector>

class ItemDatabase
{
private:
	int registerCount_;
public:
	ItemDatabase();
	ItemDatabase(std::string fileName);
	~ItemDatabase();
	void loadDatabase(std::string fileName);
	Item* getById(int id);
	Item* getByName(std::string name);

	std::vector<Item*> registers_;
};

#endif