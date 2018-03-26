#include "Platform.h"

Platform::Platform()
{

}

Platform::Platform(int nodeCount, sf::Vector2f* nodes, int boxNodeCount, sf::Vector2f* boxNodePos, float spawnRate, bool passable)
{
	nodeCount_ = nodeCount;						//Set up walkable surface of platform
	nodePos_ = new sf::Vector2f[nodeCount_];
	for (int i = 0; i < nodeCount; i++)
	{
		nodePos_[i].x = nodes[i].x;
		nodePos_[i].y = nodes[i].y;
	}

	boxNodePos_ = boxNodePos;						//Set up convex shape displaying platform
	collisionBox_.setPointCount(boxNodeCount);
	sf::Vector2f nodeTL(10e5, 10e5), nodeBR(0, 0);
	for (int i = 0; i < boxNodeCount; i++)
	{
		collisionBox_.setPoint(i, boxNodePos[i]);
		if (boxNodePos[i].x < nodeTL.x)			//Find farmost nodes of the convex
			nodeTL.x = boxNodePos[i].x;			//to create collision box
		if (boxNodePos[i].y < nodeTL.y)
			nodeTL.y = boxNodePos[i].y;
		if (boxNodePos[i].x > nodeBR.x)
			nodeBR.x = boxNodePos[i].x;
		if (boxNodePos[i].y > nodeBR.y)
			nodeBR.y = boxNodePos[i].y;
	}
	collisionBox_.setFillColor(sf::Color(35, 35, 0));

	textureBox_.setSize(sf::Vector2f(nodeBR.x - nodeTL.x + 100, nodeBR.y - nodeTL.y + 100));
	textureBox_.setPosition(sf::Vector2f(nodeTL.x - 50, nodeTL.y - 50));
	textureBox_.setFillColor(sf::Color(255, 0, 0, 100));

	spawnRate_ = spawnRate;
	passable_ = passable;
}

Platform::~Platform()
{
	delete nodePos_;
	delete boxNodePos_;
}

sf::Vector2f Platform::getNodePos(int i)
{
	return nodePos_[i];
}

sf::ConvexShape Platform::getCollisionBox()
{
	return collisionBox_;
}

int Platform::getId()
{
	return id_;
}

int Platform::getNodeCount()
{
	return nodeCount_;
}

float Platform::getSpawnRate()
{
	return spawnRate_;
}

bool Platform::isPassable()
{
	return passable_;
}

float Platform::calculateElevation(float playerPosX)
{
	int i = 0;
	for (int j = 0; j < nodeCount_ - 1; j++)
	{
		if (playerPosX >= nodePos_[j].x && j == nodeCount_ - 2)
		{
			i = j;
			break;
		}
		else if (playerPosX >= nodePos_[j].x && playerPosX < nodePos_[j + 1].x)
		{
			i = j;
			break;
		}
	}
	float deltaX = nodePos_[i + 1].x - nodePos_[i].x;
	float deltaY = nodePos_[i].y - nodePos_[i + 1].y;
	float a = deltaY / deltaX;
	if (a == 0)
		return nodePos_[i].y;
	else
	{
		float b = nodePos_[i].y - a * nodePos_[i].x;
		float returnVal = nodePos_[i].y - a * (playerPosX - nodePos_[i].x);
		return returnVal;
	}
}

float Platform::calculateInclanation(float playerPosX)
{
	int i = 0;
	for (int j = 0; j < nodeCount_ - 1; j++)
	{
		if (playerPosX >= nodePos_[i].x && playerPosX < nodePos_[i + 1].x)
		{
			i = j;
			break;
		}
	}
	float deltaX = nodePos_[i + 1].x - nodePos_[i].x;
	float deltaY = nodePos_[i].y - nodePos_[i + 1].y;
	float inc = abs(deltaX / (deltaX + deltaY));
	return inc;
}