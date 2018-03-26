#include "Player.h"

Player::Player()
{
	//Set collision box
	cbPos_.x = 25;
	cbPos_.y = 600;
	cbSize_.x = 60;
	cbSize_.y = 80;
	collisionBox_.setPosition(cbPos_);
	collisionBox_.setSize(cbSize_);
	collisionBox_.setOrigin(cbSize_.x / 2, cbSize_.y);
	collisionBox_.setFillColor(sf::Color::Green);

	//Set texture box
	tbPos_.x = cbPos_.x;
	tbPos_.y = cbPos_.y + 3;
	tbSize_.x = 80;
	tbSize_.y = 100;
	textureBox_.setPosition(tbPos_);
	textureBox_.setSize(tbSize_);
	textureBox_.setOrigin(tbSize_.x / 2, tbSize_.y);

	spriteSource_.x = 0;
	spriteSource_.y = 0;
	texture_.loadFromFile("Images/CharacterTest.png");
	textureBox_.setTexture(&texture_);
	textureBox_.setTextureRect(sf::IntRect(spriteSource_.x * tbSize_.x, spriteSource_.y * tbSize_.y, tbSize_.x, tbSize_.y));

	//Set character info
	name_ = "NoName";
	level_ = 1;
	maxHp_ = 100;
	hp_ = maxHp_;
	exp_ = 0;

	//Set HUD
	hud_.setUp(name_, level_, hp_, maxHp_, exp_, money_);

	//Set melee weapon
	MwSize_.x = 20;
	MwSize_.y = 100;
	MwPos_.x = cbPos_.x + 10;
	MwPos_.y = cbPos_.y - 20;
	MwBox_.setPosition(MwPos_);
	MwBox_.setSize(MwSize_);
	MwBox_.setOrigin(MwSize_.x / 2, MwSize_.y);
	MwBox_.setRotation(-20);
	MwTexture_.loadFromFile("Images/Mw.png");
	MwBox_.setTexture(&MwTexture_);

	//Set ranged weapon
	RwSize_.x = 40;
	RwSize_.y = 25;
	RwPos_.x = cbPos_.x + 10;
	RwPos_.y = cbPos_.y - 20;
	RwBox_.setPosition(RwPos_);
	RwBox_.setSize(RwSize_);
	RwBox_.setOrigin(RwSize_.x / 2, RwSize_.y);
	RwTexture_.loadFromFile("Images/Rw.png");
	RwBox_.setTexture(&RwTexture_);
	RwBox_.setTextureRect(sf::IntRect(40, 0, 40, 25));

	velocity_.x = velocity_.y = 0;
	physics_.gravity = 0.035f;
	physics_.terminalVel = 10.0f;
	moveSpeed_ = 2;
	jumpForce_ = 8;
	climbSpeed_ = 2;
	moveSpeedMod_ = 1.0f;
	jumpForceMod_ = 1.0f;
	fallCounter_ = 0;
	portalCounter_ = 0;
	facing_ = right;
	score_ = 0;
	attackCounter_ = 0;
	attackedCounter_ = 0;
	ladderCounter_ = 0;
	currentWeapon_ = meleeWeapon;

	isOnGround_ = false;
	isJumping_ = false;
	isFalling_ = true;
	isOnLadder_ = false;
	isJumpingDown_ = false;
	isInputUp_ = false;
}

Player::Player(sf::Vector2f tbPos, sf::Vector2f tbSize, sf::Vector2f cbPos, sf::Vector2f cbSize)
{

}

Player::~Player()
{

}

void Player::setPosition()
{
	collisionBox_.setPosition(cbPos_.x, cbPos_.y);
	tbPos_.x = cbPos_.x;
	tbPos_.y = cbPos_.y + 3;
	textureBox_.setPosition(tbPos_.x, tbPos_.y);

	if (facing_ == right)
	{
		MwPos_.x = cbPos_.x + 10;
		RwPos_.x = cbPos_.x + 10;
		RwBox_.setTextureRect(sf::IntRect(40, 0, 40, 25));
	}
	else if (facing_ == left)
	{
		MwPos_.x = cbPos_.x - 10;
		RwPos_.x = cbPos_.x - 10;
		RwBox_.setTextureRect(sf::IntRect(0, 0, 40, 25));
	}
	if(isOnLadder_)
	{
		MwPos_.x = cbPos_.x;
		MwBox_.setRotation(0);
		RwPos_.x = cbPos_.x;
	}
	MwPos_.y = cbPos_.y - 20;
	MwBox_.setPosition(MwPos_);
	RwPos_.y = cbPos_.y - 9;
	RwBox_.setPosition(RwPos_);
}

void Player::move(int direction)
{
	if(velocity_.x == 0)
		spriteSource_.x = 0;
	if (!isOnLadder_)
	{
		if (direction == left)
		{
			facing_ = left;
			if (isOnGround_)
				velocity_.x -= 0.1 * moveSpeed_ * moveSpeedMod_;
			else
				velocity_.x -= 0.015 * moveSpeed_ * moveSpeedMod_;
			if (-velocity_.x > moveSpeed_ * moveSpeedMod_)
				velocity_.x = -moveSpeed_ * moveSpeedMod_;
			cbPos_.x += velocity_.x;
		}
		else if (direction == right)
		{
			facing_ = right;
			if (isOnGround_)
				velocity_.x += 0.1 * moveSpeed_ * moveSpeedMod_;
			else
				velocity_.x += 0.015 * moveSpeed_ * moveSpeedMod_;
			if (velocity_.x > moveSpeed_ * moveSpeedMod_)
				velocity_.x = moveSpeed_ * moveSpeedMod_;
			cbPos_.x += velocity_.x;
		}
	}
	else
	{
		spriteSource_.x++;
		if (direction == top)
			velocity_.y = -climbSpeed_;
		else if (direction == bottom)
			velocity_.y = climbSpeed_;
		cbPos_.y += velocity_.y;
	}
	if (isOnGround_)
	{
		cbPos_.y = currentPlatform_->calculateElevation(cbPos_.x);
		currentPlatform_->calculateInclanation(cbPos_.x);
		velocity_.x *= currentPlatform_->calculateInclanation(cbPos_.x);
	}
}

void Player::jump(float mod)
{
	if (isOnGround_ || isOnLadder_)
	{
		velocity_.y = -jumpForce_ * jumpForceMod_ * mod;
		/*if (!soundBuffer_.loadFromFile("jump1.wav"))
			std::cerr << "Unable to load jump1.wav";
		sound_.setBuffer(soundBuffer_);
		sound_.setVolume(0);
		sound_.play();*/
	}
	isOnGround_ = false;
	isOnLadder_ = false;
	isJumping_ = true;
	isFalling_ = false;
	velocity_.y += physics_.gravity * jumpForce_ * jumpForceMod_;
	cbPos_.y += velocity_.y;
	if (velocity_.y > -0.2 && velocity_.y < 0.2)
	{
		isJumping_ = false;
		isFalling_ = true;
	}
}

void Player::fall()
{
	isOnGround_ = false;
	if (fallCounter_ == 10)
	{
	/*	if (!soundBuffer_.loadFromFile("jump2.wav"))
			std::cerr << "Unable to load jump2.wav";
		sound_.setBuffer(soundBuffer_);
		sound_.play();*/
	}
	if (velocity_.y > 0)
		isFalling_ = true;
	if (velocity_.y < physics_.terminalVel)
		velocity_.y += physics_.gravity * jumpForce_ * jumpForceMod_;
	cbPos_.y += velocity_.y;
}

void Player::decrementCounters(int i)
{
	switch (i)
	{
	case 0:
		if (fallCounter_ > 0)
			fallCounter_--;
		break;
	case 1:
		if (portalCounter_ > 0)
			portalCounter_--;
	case 2:
		if (attackCounter_ > 0)
		{
			attackCounter_--;
			if (facing_ == right)
				MwBox_.rotate(3);
			else if (facing_ == left)
				MwBox_.rotate(-3);
		}
		break;
	case 3:
		if (attackedCounter_ > 0)
			attackedCounter_--;
		break;
	case 4:
		if (ladderCounter_ > 0)
			ladderCounter_--;
	case 5:
		if (pickCounter_ > 0)
			pickCounter_--;
		break;
	default:
		break;
	}
}

void Player::addExp(int exp)
{
	exp_ += exp;
	while (exp_ >= 10 * level_ * level_)
		levelUp();
}

void Player::setSprite()
{
	//Setting X source
	if (!isOnLadder_)
	{
		if (spriteSource_.x < 37)
			spriteSource_.x++;
		else
			spriteSource_.x = 0;
	}
	else if (velocity_.y != 0)
		spriteSource_.x++;

	//Setting Y source
	if (isOnGround_)
	{
		if (velocity_.x == 0)
		{
			switch (facing_)
			{
			case left:
				spriteSource_.y = stationaryLeft;
				break;
			case right:
				spriteSource_.y = stationaryRight;
				break;
			}
		}
		else
		{
			switch (facing_)
			{
			case left:
				spriteSource_.y = moveLeft;
				break;
			case right:
				spriteSource_.y = moveRight;
				break;
			}
		}
	}

	textureBox_.setTextureRect(sf::IntRect(spriteSource_.x * tbSize_.x, spriteSource_.y * tbSize_.y, tbSize_.x, tbSize_.y));
}

void Player::getDamaged(int damage) { hp_ -= damage; }
void Player::setHP(int hp) { hp_ = hp; }
void Player::setName(std::string name) { name_ = name; }
void Player::levelUp() { level_++; }
int Player::getLevel() { return level_; }
int Player::getHP() { return hp_; }
int Player::getMaxHP() { return maxHp_; }
int Player::getExp() { return exp_; }
std::string Player::getName() { return name_; }
int Player::getMoney() { return money_; }