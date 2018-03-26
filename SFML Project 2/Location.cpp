#include "Location.h"

Location::Location(std::string filename = "Location1.txt")
{
	//Load parameters from file
	std::ifstream file(filename);
	if (file.is_open())
	{
		//Load location parameters
		std::string flagDump;
		file >> flagDump >> name_ >> flagDump >> size_.x >> flagDump >> size_.y >> flagDump >> spawnLimit_ >> flagDump >> spawnRate_;

		//Load platforms' parameters
		int platformCount = 0, nodeCount = 0, boxNodeCount = 0;
		float spawnRate = 0;
		sf::Vector2f* nodePos, *boxNodePos;
		bool passable = false;
		file >> flagDump >> platformCount;
		Platform* platform = NULL;
		for (int i = 0; i < platformCount; i++)
		{
			file >> flagDump >> nodeCount;
			nodePos = new sf::Vector2f[nodeCount];
			for (int j = 0; j < nodeCount; j++)
			{
				file >> nodePos[j].x >> nodePos[j].y;
			}

			//Load platforms' boxes parameters
			file >> boxNodeCount;
			boxNodePos = new sf::Vector2f[boxNodeCount];
			for (int j = 0; j < boxNodeCount; j++)
			{
				file >> boxNodePos[j].x >> boxNodePos[j].y;
			}
			file >> spawnRate >> passable;
			platform = new Platform(nodeCount, nodePos, boxNodeCount, boxNodePos, spawnRate, passable);
			platformList_.push_back(platform);
			delete nodePos;
			delete boxNodePos;
		}

		//Load ladders
		int ladderCount = 0;
		sf::Vector2f ladderCbPos, ladderCbSize, ladderTbSize;
		file >> flagDump >> ladderCount;
		Ladder* ladder = NULL;
		for (int i = 0; i < ladderCount; i++)
		{
			file >> flagDump >> ladderCbPos.x >> ladderCbPos.y >> ladderCbSize.x >> ladderCbSize.y >> ladderTbSize.x >> ladderTbSize.y;
			ladder = new Ladder(ladderCbPos, ladderCbSize, ladderTbSize);
			ladderList_.push_back(ladder);
		}

		//Load portals
		int portalCount = 0;
		sf::Vector2f portalPos, playerPos;
		file >> flagDump >> portalCount;
		Portal* portal = NULL;
		std::string destination;
		for (int i = 0; i < portalCount; i++)
		{
			file >> flagDump >> portalPos.x >> portalPos.y >> playerPos.x >> playerPos.y >> destination;
			portal = new Portal(portalPos, playerPos, destination);
			portalList_.push_back(portal);
		}

		//Load enemies' IDs into array
		int enemyTypeCount = 0;
		file >> flagDump >> enemyTypeCount >> flagDump;
		enemyID_ = new int[enemyTypeCount + 1];
		enemyID_[0] = enemyTypeCount;
		for (int i = 1; i < enemyTypeCount + 1; ++i)
			file >> enemyID_[i];
		
		file.close();
	}
	else
		std::cerr << "Unable to open file";

	//Create background
	background_ = new Background();

	//Set up minimap's box
	minimapBox_.setOutlineThickness(2);
	minimapBox_.setOutlineColor(sf::Color::Black);
	minimapBox_.setFillColor(sf::Color(0, 0, 0, 150));
}

Location::~Location()
{
	int platformCount = platformList_.size();
	for (int i = 0; i < platformCount; i++)
		platformList_.pop_front();
}

sf::Vector2f Location::getSize()
{
	return size_;
}