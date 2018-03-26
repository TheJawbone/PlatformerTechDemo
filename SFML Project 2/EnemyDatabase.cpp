#include "EnemyDatabase.h"

EnemyDatabase::EnemyDatabase()
{

}

EnemyDatabase::EnemyDatabase(std::string fileName)
{
	loadDatabase(fileName);
}

EnemyDatabase::~EnemyDatabase()
{
	for (int i = 0; i < registerCount_; ++i)
		delete registers_[i];
	delete registers_;
}

void EnemyDatabase::loadDatabase(std::string fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
		std::cerr << "Unable to open file " << fileName << std::endl;
	else
	{
		std::string flagDump;
		file >> flagDump >> registerCount_;
		getline(file, flagDump);
		getline(file, flagDump);
		std::string name;
		sf::Vector2f cbSize, tbSize;
		float moveSpeed, activeness, attack;
		int mobility, maxHP, exp;
		registers_ = new Enemy*[registerCount_];
		for (int i = 0; i < registerCount_; ++i)
		{
			file >> flagDump >> name >> cbSize.x >> cbSize.y >> tbSize.x >> tbSize.y >> moveSpeed >> activeness >> mobility >> maxHP >> exp >> attack;
			registers_[i] = new Enemy(name, cbSize, tbSize, moveSpeed, activeness, mobility, maxHP, exp, attack);
		}
	}
}

Enemy* EnemyDatabase::getById(int id)
{
	return registers_[id - 1];
}

Enemy* EnemyDatabase::getByName(std::string name)
{
	std::string dump;
	for (int i = 0; i < registerCount_; ++i)
	{
		if (registers_[i]->getName() == name)
			return registers_[i];
	}
}