#include "Enemy.h"

Enemy::Enemy()
{
	moveDirection_ = 0;
	moveSpeed_ = 1;
	mobility_ = 100;
	activeness_ = 0.5;
}

Enemy::Enemy(std::string name, sf::Vector2f cbSize, sf::Vector2f tbSize, float moveSpeed, float activeness, int mobility, int maxHP, int exp, float attack)
{
	name_ = name;
	cbSize_ = cbSize;
	tbSize_ = tbSize;
	moveSpeed_ = moveSpeed;
	activeness_ = activeness;
	mobility_ = mobility;
	maxHP_ = maxHP;
	HP_ = maxHP_;
	exp_ = exp;
	attack_ = attack;
	attackedCounter_ = 0;

	collisionBox_.setSize(cbSize_);
	collisionBox_.setOrigin(cbSize_.x / 2, cbSize_.y);
	collisionBox_.setPosition(cbPos_);
	collisionBox_.setFillColor(sf::Color::Red);

	setPosition();
	textureBox_.setSize(tbSize_);
	textureBox_.setOrigin(tbSize_.x / 2, tbSize_.y);
	textureBox_.setPosition(tbPos_);
	//texture_.loadFromFile("Enemy1.jpg");
	//textureBox_.setTexture(&texture_);

	healthBarBorder_.setSize(sf::Vector2f(80, 10));
	healthBarBorder_.setPosition(cbPos_.x - 40, cbPos_.y + 20);
	healthBarBorder_.setFillColor(sf::Color::Red);
	healthBarBorder_.setOutlineThickness(1);
	healthBarBorder_.setOutlineColor(sf::Color::Black);

	healthBar_.setSize(sf::Vector2f(80, 10));
	healthBar_.setPosition(cbPos_.x - 40, cbPos_.y + 20);
	healthBar_.setFillColor(sf::Color::Green);

	movementCounter_ = 0;
}

Enemy Enemy::operator=(Enemy &enemy)
{
	cbSize_ = enemy.cbSize_;
	tbSize_ = enemy.tbSize_;
	collisionBox_ = enemy.collisionBox_;
	textureBox_ = enemy.textureBox_;
	healthBar_ = enemy.healthBar_;
	healthBarBorder_ = enemy.healthBarBorder_;
	collisionBox_.setSize(cbSize_);
	textureBox_.setSize(tbSize_);
	moveSpeed_ = enemy.moveSpeed_;
	activeness_ = enemy.activeness_;
	maxHP_ = enemy.maxHP_;
	mobility_ = enemy.mobility_;
	HP_ = enemy.maxHP_;
	exp_ = enemy.exp_;
	attack_ = enemy.attack_;
	attackedCounter_ = 0;
	return *this;
}

void Enemy::generateMovement(int movSeed, int actSeed, int direction)
{
	if (actSeed / 100.0 >= activeness_ && movementCounter_ == 0)	//probability test passed & enemy isn't currently moving
	{
		movementCounter_ = movSeed / 100.0 * mobility_ + 10;
		moveDirection_ = direction;
	}
}

void Enemy::move()
{
	if (movementCounter_ > 0)
	{
		switch (moveDirection_)
		{
		case 0:
			if (cbPos_.x > currentPlatform_->getNodePos(0).x)
				cbPos_.x -= moveSpeed_;
			break;
		case 1:
			if (cbPos_.x < currentPlatform_->getNodePos(currentPlatform_->getNodeCount() - 1).x)
				cbPos_.x += moveSpeed_;
			break;
		default:
			break;
		}
		if (cbPos_.x < currentPlatform_->getNodePos(0).x)
			cbPos_.x = currentPlatform_->getNodePos(0).x + 1;
		else if (cbPos_.x > currentPlatform_->getNodePos(currentPlatform_->getNodeCount() - 1).x)
			cbPos_.x = currentPlatform_->getNodePos(currentPlatform_->getNodeCount() - 1).x - 1;
		cbPos_.y = currentPlatform_->calculateElevation(cbPos_.x);
		setPosition();
		movementCounter_--;
	}
}

void Enemy::setPosition()
{
	collisionBox_.setPosition(cbPos_.x, cbPos_.y);
	tbPos_.x = cbPos_.x;
	tbPos_.y = cbPos_.y + (tbSize_.y - cbSize_.y) / 2;
	textureBox_.setPosition(tbPos_.x, tbPos_.y);
	healthBarBorder_.setPosition(cbPos_.x - 40, cbPos_.y + 20);
	healthBar_.setPosition(healthBarBorder_.getPosition().x, healthBarBorder_.getPosition().y);
}

void Enemy::setHealthBar()
{
	healthBar_.setSize(sf::Vector2f(HP_ / maxHP_ * healthBarBorder_.getSize().x, healthBarBorder_.getSize().y));
}

std::string Enemy::getName()
{
	return name_;
}

sf::RectangleShape Enemy::getHealthBar()
{
	return healthBar_;
}

sf::RectangleShape Enemy::getHealthBarBorder()
{
	return healthBarBorder_;
}