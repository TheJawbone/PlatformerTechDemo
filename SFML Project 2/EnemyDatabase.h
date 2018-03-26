#pragma once
#ifndef ENEMYDATABASE_H
#define ENEMYDATABASE_H

#include "Enemy.h"
#include <fstream>
#include <iostream>

class EnemyDatabase
{
private:
	Enemy** registers_;
	int registerCount_;
public:
	EnemyDatabase();
	EnemyDatabase(std::string fileName);
	~EnemyDatabase();
	void loadDatabase(std::string fileName);
	Enemy* getById(int id);
	Enemy* getByName(std::string name);
};

#endif