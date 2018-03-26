#include "PlayerHUD.h"

PlayerHUD::PlayerHUD()
{
	
}

PlayerHUD::~PlayerHUD()
{

}

void PlayerHUD::setUp(std::string name, int level, int hp, int maxHp, int exp, int money)
{
	mainTb_.set(sf::Vector2f(0, 718), sf::Vector2f(1360, 50), "HUD.png");
	mainTb_.textureBox_.setFillColor(sf::Color(0, 0, 0, 100));

	sf::Font arial;
	arial.loadFromFile("Fonts/arial.ttf");
	std::string aux;

	//Set up name
	name_.setString(name);
	name_.setPosition(100, 730);
	name_.setCharacterSize(18);
	name_.setFillColor(sf::Color::Yellow);

	//Set up level
	aux = "Level: " + std::to_string(level);
	level_.setString(aux);
	level_.setPosition(300, 730);
	level_.setCharacterSize(18);
	level_.setFillColor(sf::Color::Yellow);

	//Set up HP
	aux = "HP: " + std::to_string(hp) + "/" + std::to_string(maxHp);
	hp_.setString(aux);
	hp_.setPosition(500, 730);
	hp_.setCharacterSize(18);
	hp_.setFillColor(sf::Color::Yellow);
	healthBar_.textureBox_.setSize(sf::Vector2f(100 * hp / maxHp, 20));
	healthBar_.textureBox_.setPosition(528, 730);
	healthBar_.textureBox_.setFillColor(sf::Color::Green);
	healthBarBorder_.textureBox_.setSize(sf::Vector2f(100, 20));
	healthBarBorder_.textureBox_.setPosition(528, 730);
	healthBarBorder_.textureBox_.setFillColor(sf::Color::Red);
	healthBarBorder_.textureBox_.setOutlineThickness(1);
	healthBarBorder_.textureBox_.setOutlineColor(sf::Color::Black);

	//Set up exp
	aux = "Exp: " + std::to_string(exp) + "/" + std::to_string(10 * level * level);
	exp_.setString(aux);
	exp_.setPosition(700, 730);
	exp_.setCharacterSize(18);
	exp_.setFillColor(sf::Color::Yellow);
	expBar_.textureBox_.setSize(sf::Vector2f(100 *( exp - 10 * (level - 1) * (level - 1)) / (10 * level * level - 10 * (level - 1) * (level - 1)), 20));
	expBar_.textureBox_.setPosition(738, 730);
	expBar_.textureBox_.setFillColor(sf::Color::Cyan);
	expBarBorder_.textureBox_.setSize(sf::Vector2f(100, 20));
	expBarBorder_.textureBox_.setPosition(738, 730);
	expBarBorder_.textureBox_.setFillColor(sf::Color::Magenta);
	expBarBorder_.textureBox_.setOutlineThickness(1);
	expBarBorder_.textureBox_.setOutlineColor(sf::Color::Black);

	//Set up money
	aux = "Money: " + std::to_string(money);
	money_.setString(aux);
	money_.setPosition(950, 730);
	money_.setCharacterSize(18);
	money_.setFillColor(sf::Color::Yellow);
}

void PlayerHUD::setPosition(sf::Vector2u screenSize)
{
	mainTb_.setTbPos(sf::Vector2f(0, screenSize.y - 50));
}

void PlayerHUD::setFont(sf::Font &font)
{
	name_.setFont(font);
	level_.setFont(font);
	hp_.setFont(font);
	exp_.setFont(font);
}

void PlayerHUD::update(std::string name, int level, int maxHp, int hp, int exp, int money)
{
	name_.setString(name);
	std::string aux = "Level: " + std::to_string(level);
	level_.setString(aux);
	aux = "HP: " + std::to_string(hp) + "/" + std::to_string(maxHp);
	hp_.setString(aux);
	aux = "Exp: " + std::to_string(exp) + "/" + std::to_string(10 * level * level);
	exp_.setString(aux);
	aux = "Money: " + std::to_string(money);
	money_.setString(aux);
	healthBar_.textureBox_.setSize(sf::Vector2f(100 * hp / maxHp, 20));
	expBar_.textureBox_.setSize(sf::Vector2f(100 * (exp - 10 * (level - 1) * (level - 1)) / (10 * level * level - 10 * (level - 1) * (level - 1)), 20));
}

void PlayerHUD::draw(sf::RenderWindow &window, sf::Font &font)
{
	window.draw(mainTb_.textureBox_);
	window.draw(healthBarBorder_.textureBox_);
	window.draw(healthBar_.textureBox_);
	window.draw(expBarBorder_.textureBox_);
	window.draw(expBar_.textureBox_);

	setFont(font);

	window.draw(name_);
	window.draw(level_);
	window.draw(hp_);
	window.draw(exp_);
	window.draw(money_);
}