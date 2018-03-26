#include "ItemDatabase.h"

ItemDatabase::ItemDatabase()
{

}

ItemDatabase::ItemDatabase(std::string fileName)
{
	loadDatabase(fileName);
}

ItemDatabase::~ItemDatabase()
{
	for (std::vector<Item*>::iterator it = registers_.begin(); it != registers_.end(); ++it)
		delete *it;
}

void ItemDatabase::loadDatabase(std::string fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
		std::cerr << "Unable to open file " << fileName;
	else
	{
		//Skip flags in the input file
		std::string flagDump;
		file >> flagDump >> registerCount_;
		getline(file, flagDump);
		getline(file, flagDump);

		//Variables used to store read data from file
		std::string name, type, texture;
		int id, value, effectsCount, effectValue, effectType;
		sf::Vector2f cbSize, tbSize;

		//Adjust vector size, read all registers from file
		for (int i = 0; i < registerCount_; ++i)
		{
			file >> id >> name >> value >> cbSize.x >> cbSize.y >> tbSize.x >> tbSize.y >> texture >> type;
			Item* item = new Item(cbSize, tbSize, name, texture, id, value);
			
			//Consumable item -> read effects from database
			if (type == "cons")
			{
				file >> effectsCount;
				std::list<Effect> effects;
				for (int j = 0; j < effectsCount; ++j)
				{
					file >> effectType >> effectValue;
					Effect effect;
					effect.effect_ = effectType;
					effect.effectValue_ = effectValue;
					effects.push_back(effect);
				}
				Consumables* cons = new Consumables(*item, effects, texture);
				delete item;
				registers_.push_back(cons);
			}

			//Misc item -> push item as it is
			else
				registers_.push_back(item);
		}
	}
}

Item* ItemDatabase::getById(int id)
{
	return registers_[id];
}

Item* ItemDatabase::getByName(std::string name)
{
	std::string dump;
	for (int i = 0; i < registerCount_; ++i)
	{
		if (registers_[i]->getName() == name)
			return registers_[i];
	}
}