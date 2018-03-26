#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS

void mainGameLoop(sf::RenderWindow &window);
int mainMenu(sf::RenderWindow &window);
void eventLoop(sf::RenderWindow &window, Window &inGameWindow, Player* player, bool &changeLocation, bool &endGame, bool &isDebugModeActive);
void spawnEnemies(EnemyDatabase& enemyDB, Location* location, int &spawnCounter);
void enemyAction(Location* location, Player* player, int synClock);
void landingCheck(Location* location, Player* player);
void fallingCheck(Player* player);
void ladderGrabbingCheck(Location* location, Player* player);
void ladderFallingCheck(Location* location, Player* player);
void portalCheck(Location* location, Player* player, bool &changeLocation, std::string &locationSource);
void keyboardInput(Location* location, Player* player, ItemDatabase* itemDB, int itemSeed);
void gravityInfluence(Player* player);
void meleeAttack(Player* player, Location* location, ItemDatabase* itemDB, int itemSeed);
void rangedAttack(Player* player, Location* location);
void moveProjectiles(Player* player, Location* location, ItemDatabase* itemDB, int synClock, int itemSeed);
std::ostringstream displayDebugInfo(Player*, Location*, int, int);
bool generateSpawn(float, int, int, int);
int generateRandomI(float, float);
bool playerIsDead(Player* player, sf::RenderWindow &window, bool &changeLocation);
Player* createCharacter(sf::RenderWindow &window);
void dropItems(Location* location, ItemDatabase* itemDB, Enemy* enemy, int itemSeed);
void toggleDebugMode(bool &isDebugModeActive);

int main()
{
	//Setting up window
	sf::Vector2f screenSize(1360, 768);
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "The Game" ,sf::Style::Fullscreen);

	//Launch main menu
	mainMenu(window);
}

void mainGameLoop(sf::RenderWindow &window)
{
	//Start the clock for random seeds
	srand(time(NULL));

	//Load fonts
	sf::Font arial;
	arial.loadFromFile("Fonts/arial.ttf");

	//Setting up scrolling view
	sf::View scrollView;
	scrollView.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	scrollView.setViewport(sf::FloatRect(0, 0, 1, 1));
	sf::Vector2f viewPos;

	//Load location
	Location* location = NULL;
	std::string locationSource = "Location1.txt";
	bool changeLocation = false;

	//Load databases
	EnemyDatabase enemyDB("Enemies.txt");
	ItemDatabase* itemDB = new ItemDatabase("Item.txt");

	//Clock
	sf::Clock clock;
	sf::Clock clock2;
	int synClock = 0;
	int tickCounter2 = 0;
	int gameSpeed = 0;
	int spawnCounter = 0;

	//Boolean variables
	bool endGame = false;
	bool isDebugModeActive = false;

	//Create player
	Player* player = createCharacter(window);

	//Set up player's HUD
	player->hud_.setPosition(window.getSize());
	player->hud_.setFont(arial);

	//Create window <--TEMP!!!
	Window inGameWindow;

	//Main game loop
	while (!endGame)
	{
		//Changing location
		if (location)
		{
			delete location;
			changeLocation = false;
			synClock = 0;
		}

		location = new Location(locationSource);

		//Setting up view for the minimap
		sf::View minimapView;
		minimapView.setSize(location->getSize());
		minimapView.setCenter(location->getSize().x / 2, location->getSize().y / 2);
		minimapView.setViewport(sf::FloatRect(0.85f, 0, 0.15f, 0.15 * location->getSize().x / location->getSize().y));
		location->minimapBox_.setPosition(window.getSize().x * 0.85, 0);
		location->minimapBox_.setSize(sf::Vector2f(window.getSize().x * 0.15, 0.15 * window.getSize().y * location->getSize().x / location->getSize().y));
		system("cls");

		//Loop while on current location
		while (!changeLocation)
		{
			//Event loop
			eventLoop(window, inGameWindow, player, changeLocation, endGame, isDebugModeActive);

			//Generate seed for item drops (needs to be outside the thread)
			int itemSeed = generateRandomI(3, 5);
			std::thread t1([&]
			{
				//Keyboard input
				keyboardInput(location, player, itemDB, itemSeed);

				//Check if player lands on a platform
				landingCheck(location, player);

				//Check if player walked off the platform
				fallingCheck(player);

				//Check if player grabbed the ladder
				ladderGrabbingCheck(location, player);

				//Check if player went off the ladder
				ladderFallingCheck(location, player);

				//Check if player entered the portal
				portalCheck(location, player, changeLocation, locationSource);

				//Be pulled by gravity
				gravityInfluence(player);
			});

			std::thread t2([&]
			{
				//Setting up view part 2
				if (player->getCbPos().x < window.getSize().x / 2)
					viewPos.x = window.getSize().x / 2;
				else if (player->getCbPos().x >= window.getSize().x / 2 && player->getCbPos().x <= location->getSize().x - window.getSize().x / 2)
					viewPos.x = player->getCbPos().x;
				else
					viewPos.x = location->getSize().x - window.getSize().x / 2;

				if (player->getCbPos().y > location->getSize().y - window.getSize().y / 2)	//view fixed at the bottom
					viewPos.y = location->getSize().y - window.getSize().y / 2;
				else if (player->getCbPos().y <= location->getSize().y - window.getSize().y / 2 && player->getCbPos().y > window.getSize().y / 2)
					viewPos.y = player->getCbPos().y;
				else																		//view fixed at the top
					viewPos.y = window.getSize().y / 2;
				scrollView.setCenter(viewPos.x, viewPos.y);
			});

			//Generate texts to display
			sf::Text debugInfo(displayDebugInfo(player, location, gameSpeed, synClock).str(), arial, 12);
			sf::Text versionInfo("Early development build\n                                  v0.02", arial, 12);
			sf::Text controlsInfo("Movement: arrow keys\nJump: Left Alt\nMeele attack: Left Control\nRanged attack: Left Shift", arial, 15);
			std::thread t3([&]
			{
				debugInfo.setFillColor(sf::Color::White);
				versionInfo.setPosition(window.getSize().x - 135, window.getSize().y - 30);
				controlsInfo.setOrigin(controlsInfo.getLocalBounds().width / 2, controlsInfo.getLocalBounds().height);
				controlsInfo.setPosition(window.getSize().x / 2, 80);
				controlsInfo.setFillColor(sf::Color::Black);
			});

			//Spawn enemies
			spawnEnemies(enemyDB, location, spawnCounter);

			//Move enemies
			enemyAction(location, player, synClock);

			//Check if player died
			endGame = playerIsDead(player, window, changeLocation);

			t1.join();
			t2.join();
			t3.join();

			//Correct and set player position
			if (player->getCbPos().x < 0)
				player->setCbPos(sf::Vector2f(0, player->getCbPos().y));
			else if (player->getCbPos().x > location->getSize().x)
				player->setCbPos(sf::Vector2f(location->getSize().x, player->getCbPos().y));
			player->setPosition();

			//Set counters
			for (int i = 1; i < 6; ++i)
				player->decrementCounters(i);

			//Set attack stuff
			if (player->attackCounter_ == 0)
			{
				if (player->currentWeapon_ == meleeWeapon)
				{
					if (player->facing_ == right)
						player->MwBox_.setRotation(-20);
					else if (player->facing_ == left)
						player->MwBox_.setRotation(20);
				}
			}

			//Move projectiles shot by the player
			moveProjectiles(player, location, itemDB, synClock, itemSeed);

			//Update player's HUD
			player->hud_.update(player->getName(), player->getLevel(), player->getMaxHP(), player->getHP(), player->getExp(), player->getMoney());

			//Set game speed to n ticks per second
			while (clock.getElapsedTime().asMilliseconds() < 10) {}
			clock.restart().asSeconds();

			//Draw&Display, limit FPS to 50
			if (!changeLocation)
			{
				//Background
				window.setView(window.getDefaultView());
				location->background_->draw(window);
				window.setView(scrollView);
				if (synClock % 2 == 0)
				{
					//Minimap box
					window.setView(window.getDefaultView());
					window.draw(location->minimapBox_);
					
					//Platforms
					for (std::list<Platform*>::iterator it = location->platformList_.begin(); it != location->platformList_.end(); ++it)
					{
						window.setView(scrollView);
						window.draw((*it)->getCollisionBox());
						window.setView(minimapView);
						window.draw((*it)->getCollisionBox());
					}
					
					//Ladders
					for (std::list<Ladder*>::iterator it = location->ladderList_.begin(); it != location->ladderList_.end(); ++it)
					{
						window.setView(scrollView);
						window.draw((*it)->getTextureBox());		
						window.draw((*it)->getCollisionBox());
						window.setView(minimapView);
						window.draw((*it)->getCollisionBox());
					}
					
					//Enemies
					for (std::list<Enemy*>::iterator it = location->enemyList_.begin(); it != location->enemyList_.end(); ++it)
					{
						window.setView(scrollView);
						window.draw((*it)->getTextureBox());
						window.draw((*it)->getCollisionBox());
						window.setView(minimapView);
						window.draw((*it)->getCollisionBox());
						window.setView(scrollView);
						if ((*it)->attackedCounter_ > 0)
						{
							window.draw((*it)->getHealthBarBorder());
							window.draw((*it)->getHealthBar());
							(*it)->attackedCounter_--;
						}
					}

					//Player
					window.setView(scrollView);
					player->setSprite();
					window.draw(player->getTextureBox());
					if (player->currentWeapon_ == meleeWeapon)
						window.draw(player->MwBox_);
					else if (player->currentWeapon_ == rangedWeapon)
						window.draw(player->RwBox_);
					window.setView(window.getDefaultView());
					player->hud_.draw(window, arial);
					window.setView(minimapView);
					window.draw(player->getCollisionBox());

					//Items on ground
					window.setView(scrollView);
					for (std::list<Item*>::iterator it = location->itemList_.begin(); it != location->itemList_.end(); ++it)
						window.draw((*it)->getTextureBox());
					
					//Projectiles
					window.setView(scrollView);
					std::list<Projectile*>::iterator projectileIt;
					for (projectileIt = player->projectileList_.begin(); projectileIt != player->projectileList_.end(); ++projectileIt)
					{
						Projectile* listedProjectile = *projectileIt;
						window.draw(listedProjectile->getCollisionBox());
					}
					
					//Portals
					for (std::list<Portal*>::iterator it = location->portalList_.begin(); it != location->portalList_.end(); ++it)
					{
						window.setView(scrollView);
						window.draw((*it)->getCollisionBox());
						window.draw((*it)->getTextureBox());
						window.setView(minimapView);
						window.draw((*it)->getCollisionBox());
					}
					
					//Text
					window.setView(window.getDefaultView());
					window.draw(versionInfo);
					if (isDebugModeActive)
					{
						window.draw(debugInfo);
						window.draw(controlsInfo);
					}
					
					//Equipment window
					window.setView(window.getDefaultView());
					inGameWindow.draw(window);

					window.display();
					window.clear();
				}
			}
			tickCounter2++;
			if (clock2.getElapsedTime().asMilliseconds() > 1000)
			{
				gameSpeed = tickCounter2;
				if (gameSpeed > 100)
					gameSpeed = 100;
				tickCounter2 = 0;
				clock2.restart();
			}
			spawnCounter++;
			synClock++;
			if (synClock > 1199)
				synClock = 0;
		}
	}
}

int mainMenu(sf::RenderWindow &window)
{
	sf::Event event;
	sf::Clock clock;
	sf::Font arial;
	arial.loadFromFile("Fonts/arial.ttf");

	sf::Text newGame("New Game", arial, 24);
	sf::Text loadGame("Load Game", arial, 24);
	sf::Text options("Options", arial, 24);
	sf::Text quit("Quit", arial, 24);

	newGame.setOrigin(newGame.getLocalBounds().width / 2, newGame.getLocalBounds().height);
	loadGame.setOrigin(loadGame.getLocalBounds().width / 2, loadGame.getLocalBounds().height);
	options.setOrigin(options.getLocalBounds().width / 2, options.getLocalBounds().height);
	quit.setOrigin(quit.getLocalBounds().width / 2, quit.getLocalBounds().height);

	newGame.setPosition(window.getSize().x / 2, 200);
	loadGame.setPosition(window.getSize().x / 2, 250);
	options.setPosition(window.getSize().x / 2, 300);
	quit.setPosition(window.getSize().x / 2, 350);

	bool newGameIsActive = true, loadGameIsActive = false, optionsIsActive = false, quitIsActive = false;
	int newGameCounter = 0, loadGameCounter = 0, optionsCounter = 0, quitCounter = 0;
	
	while (1)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
					return 0;
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					if (newGameIsActive)
					{
						newGameIsActive = false;
						quitIsActive = true;
					}
					else if (loadGameIsActive)
					{
						loadGameIsActive = false;
						newGameIsActive = true;
					}
					else if (optionsIsActive)
					{
						optionsIsActive = false;
						loadGameIsActive = true;
					}
					else if (quitIsActive)
					{
						quitIsActive = false;
						optionsIsActive = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if (newGameIsActive)
					{
						newGameIsActive = false;
						loadGameIsActive = true;
					}
					else if (loadGameIsActive)
					{
						loadGameIsActive = false;
						optionsIsActive = true;
					}
					else if (optionsIsActive)
					{
						optionsIsActive = false;
						quitIsActive = true;
					}
					else if (quitIsActive)
					{
						quitIsActive = false;
						newGameIsActive = true;
					}
				}
				break;
			default:
				break;
			}
		}

		if (abs(mousePos.x - newGame.getPosition().x) <= newGame.getLocalBounds().width / 2
			&& abs(mousePos.y + newGame.getLocalBounds().height / 2 - newGame.getPosition().y) <= newGame.getLocalBounds().height / 2)
		{
			newGameIsActive = true;
			loadGameIsActive = false;
			optionsIsActive = false;
			quitIsActive = false;
		}
		else if (abs(mousePos.x - loadGame.getPosition().x) <= loadGame.getLocalBounds().width / 2
			&& abs(mousePos.y + loadGame.getLocalBounds().height / 2 - loadGame.getPosition().y) <= loadGame.getLocalBounds().height / 2)
		{
			newGameIsActive = false;
			loadGameIsActive = true;
			optionsIsActive = false;
			quitIsActive = false;
		}
		else if (abs(mousePos.x - options.getPosition().x) <= options.getLocalBounds().width / 2
			&& abs(mousePos.y + options.getLocalBounds().height / 2 - options.getPosition().y) <= options.getLocalBounds().height / 2)
		{
			newGameIsActive = false;
			loadGameIsActive = false;
			optionsIsActive = true;
			quitIsActive = false;
		}
		else if (abs(mousePos.x - quit.getPosition().x) <= quit.getLocalBounds().width / 2
			&& abs(mousePos.y + quit.getLocalBounds().height / 2 - quit.getPosition().y) <= quit.getLocalBounds().height / 2)
		{
			newGameIsActive = false;
			loadGameIsActive = false;
			optionsIsActive = false;
			quitIsActive = true;
		}

		if (newGameIsActive && newGameCounter < 255)
			newGameCounter += 8;
		else if (!newGameIsActive && newGameCounter > 0)
			newGameCounter -= 8;
		if (loadGameIsActive && loadGameCounter < 255)
			loadGameCounter += 8;
		else if (!loadGameIsActive && loadGameCounter > 0)
			loadGameCounter -= 8;
		if (optionsIsActive && optionsCounter < 255)
			optionsCounter += 8;
		else if (!optionsIsActive && optionsCounter > 0)
			optionsCounter -= 8;
		if (quitIsActive && quitCounter < 255)
			quitCounter += 8;
		else if (!quitIsActive && quitCounter > 0)
			quitCounter -= 8;

		if (newGameCounter == 256)
			newGameCounter--;
		if (loadGameCounter == 256)
			loadGameCounter--;
		if (optionsCounter == 256)
			optionsCounter--;
		if (quitCounter == 256)
			quitCounter--;

		newGame.setFillColor(sf::Color(255, 255 - newGameCounter, 255 - newGameCounter));
		loadGame.setFillColor(sf::Color(255, 255 - loadGameCounter, 255 - loadGameCounter));
		options.setFillColor(sf::Color(255, 255 - optionsCounter, 255 - optionsCounter));
		quit.setFillColor(sf::Color(255, 255 - quitCounter, 255 - quitCounter));

		if (newGameCounter == 255)
			newGameCounter++;
		if (loadGameCounter == 255)
			loadGameCounter++;
		if (optionsCounter == 255)
			optionsCounter++;
		if (quitCounter == 255)
			quitCounter++;

		if (newGameIsActive && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return)))
		{
			mainGameLoop(window);
		}
		else if (loadGameIsActive && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return)))
		{

		}
		else if (optionsIsActive && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return)))
		{

		}
		else if (quitIsActive && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return)))
		{
			return 0;
		}


		window.setView(window.getDefaultView());

		window.draw(newGame);
		window.draw(loadGame);
		window.draw(options);
		window.draw(quit);

		window.display();
		window.clear();

		//Limit speed / FPS
		while (clock.getElapsedTime().asMilliseconds() < 10) {}
		clock.restart().asSeconds();
	}
}

std::ostringstream displayDebugInfo(Player* player, Location* location, int gameSpeed, int synClock)
{
	std::ostringstream oss;

	oss << "Player:\nPosX: " << player->getCollisionBox().getPosition().x << "\nPosY: " << player->getCollisionBox().getPosition().y << std::endl;
	oss << "VelocityX: " << player->velocity_.x << "\nVelocityY: " << player->velocity_.y << std::endl;
	oss << "FallCounter: " << player->fallCounter_ << "\nPortalCounter: " << player->portalCounter_ << std::endl;
	oss << "Score: " << player->score_ << std::endl;
	oss << "IsJumping: " << player->isJumping_ << "\nIsFalling: " << player->isFalling_ << "\nIsOnGround: "
		<< player->isOnGround_ << "\nIsOnLadder: " << player->isOnLadder_ << "\nIsJumpingDown: " << player->isJumpingDown_ << std::endl << std::endl;
	oss << "EnemyCount: " << location->getEnemyCount() << std::endl << std::endl;
	oss << "Game speed: " << gameSpeed << "%";
	oss << "\nSynClock: " << synClock << std::endl;
	oss << "\nSpriteSource x: " << player->spriteSource_.x;
	oss << "\nFacing: " << player->facing_;


	return oss;
}

bool generateSpawn(float platformSpawnRate, int locationSpawnRate, int enemyCount, int enemySpawnLimit)
{
	float aux = rand() % 1001;
	if (enemyCount == 0 || aux < (1.0 / (enemyCount) - 1.0 / (enemySpawnLimit)) * platformSpawnRate * locationSpawnRate)
		return true;
	else
		return false;
}

int generateRandomI(float limLow, float limHigh)
{
	int aux = rand() % ((int)(limHigh - limLow) + 1) + (int)limLow;
	return aux;
}

void eventLoop(sf::RenderWindow &window, Window &inGameWindow, Player* player, bool &changeLocation, bool &endGame, bool &isDebugModeActive)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			changeLocation = true;
			endGame = true;
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				changeLocation = true;
				endGame = true;
			}
			if (event.key.code == sf::Keyboard::E)
			{
				if (!inGameWindow.visible_)
					inGameWindow.update(player->itemList_);
				inGameWindow.toggleVisibility();
			}
			if (event.key.code == sf::Keyboard::F3)
				toggleDebugMode(isDebugModeActive);
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Left && player->velocity_.x <= 0)
				player->velocity_.x = 0;
			if (event.key.code == sf::Keyboard::Right)
				player->velocity_.x = 0;
			if (player->isOnLadder_ && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down))
				player->velocity_.y = 0;
			if (event.key.code == sf::Keyboard::LAlt)
				player->isJumping_ = false;
			break;
		default:
			break;
		}
	}
}

void spawnEnemies(EnemyDatabase &enemyDB, Location* location, int &spawnCounter)
{
	if (spawnCounter == 10 && location->getEnemyCount() < location->getSpawnLimit())
	{
		spawnCounter = 0;
		for (std::list<Platform*>::iterator it = location->platformList_.begin(); it != location->platformList_.end(); ++it)
		{
			if (generateSpawn((*it)->getSpawnRate(), location->getSpawnRate(), location->getEnemyCount(), location->getSpawnLimit())
				&& location->getEnemyCount() < location->getSpawnLimit()) //test passed, spawn
			{
				//Generate spawn position and allocate memory for a new enemy
				sf::Vector2f spawnPos;
				spawnPos.x = generateRandomI((*it)->getNodePos(0).x, (*it)->getNodePos((*it)->getNodeCount() - 1).x);
				spawnPos.y = (*it)->calculateElevation(spawnPos.x);
				Enemy* aux = new Enemy();

				//Assign parameters from database to new enemy
				*aux = *enemyDB.getById(generateRandomI(1, 3));
				aux->setCbPos(spawnPos);
				aux->setPosition();
				aux->currentPlatform_ = (*it);
				location->enemyList_.push_back(aux);
				location->incrementEnemyCount();
			}
		}
	}
}

void enemyAction(Location* location, Player* player, int synClock)
{
	for (std::list<Enemy*>::iterator it = location->enemyList_.begin(); it != location->enemyList_.end(); ++it)
	{
		//Generate enemy's movement
		if (synClock % 100 == 0)
			(*it)->generateMovement(generateRandomI(0, 100), generateRandomI(0, 100), generateRandomI(0, 1));
		(*it)->move();

		//Passively damage the player, collision check is TEMPORARY!!!
		if (player->attackedCounter_ == 0 && abs((*it)->getCbPos().x - player->getCbPos().x) < 20 && abs((*it)->getCbPos().y - player->getCbPos().y) < 20)
		{
			player->getDamaged((*it)->attack_);
			player->attackedCounter_ = 100;
		}
	}
}

void landingCheck(Location* location, Player* player)
{
	if (!player->isOnGround_)
	{
		for (std::list<Platform*>::iterator it = location->platformList_.begin(); it != location->platformList_.end(); ++it)
		{
			//First check if player is between farmost left and right nodes
			if (player->getCbPos().x >= (*it)->getNodePos(0).x - player->getCbSize().x / 2
				&& player->getCbPos().x <= (*it)->getNodePos((*it)->getNodeCount() - 1).x + player->getCbSize().x / 2
				&& player->velocity_.y > 0)
			{
				//If so, check if Y position is within 5 pixels of platform's ground level
				for (int i = 0; i < (*it)->getNodeCount() - 1; i++)
				{
					int correctPos1 = 0, correctPos2 = 0;
					if (i == 0)
						correctPos1 = player->getCbSize().x / 2;
					if (i == (*it)->getNodeCount() - 2)
						correctPos2 = player->getCbSize().x / 2;
					if (player->getCbPos().x >= (*it)->getNodePos(i).x - correctPos1
						&& player->getCbPos().x <= (*it)->getNodePos(i + 1).x + correctPos2
						&& abs(player->getCbPos().y - (*it)->calculateElevation(player->getCbPos().x)) <= 5
						&& player->fallCounter_ == 0 && player->ladderCounter_ == 0)
					{
						switch (player->facing_)
						{
						case left:
							player->spriteSource_.y = player->stationaryLeft;
							break;
						case right:
							player->spriteSource_.y = player->stationaryRight;
							break;
						}
						player->isOnLadder_ = false;
						player->currentLadder_ = NULL;
						player->currentPlatform_ = (*it);
						player->setCbPos(sf::Vector2f(player->getCbPos().x, (*it)->calculateElevation(player->getCbPos().x)));
						player->isOnGround_ = true;
						player->isFalling_ = false;
						player->velocity_.y = 0;
					}
				}
			}
		}
	}
}

void fallingCheck(Player* player)
{
	if (player->isOnGround_ && player->currentPlatform_)
	{
		if ((player->getCbPos().x < player->currentPlatform_->getNodePos(0).x - player->getCbSize().x / 2
			|| player->getCbPos().x > player->currentPlatform_->getNodePos(player->currentPlatform_->getNodeCount() - 1).x + player->getCbSize().x / 2))
		{
			player->isFalling_ = true;
			player->isOnGround_ = false;
		}
	}
}

void ladderGrabbingCheck(Location* location, Player* player)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		for (std::list<Ladder*>::iterator it = location->ladderList_.begin(); it != location->ladderList_.end(); ++it)
		{
			if (abs(player->getCbPos().x - (*it)->getCbPos().x) < 5
				&& ((player->getCbPos().y - player->getCbSize().y <= (*it)->getCbPos().y + (*it)->getCbSize().y && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player->getCbPos().y > (*it)->getCbPos().y)
				|| (player->getCbPos().y == (*it)->getCbPos().y && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))))
			{
				player->spriteSource_.y = player->onLadder;
				player->currentLadder_ = (*it);
				player->isOnLadder_ = true;
				player->isOnGround_ = false;
				player->isFalling_ = false;
				player->velocity_.x = player->velocity_.y = 0;
				player->setCbPos(sf::Vector2f((*it)->getCbPos().x, player->getCbPos().y));
				player->ladderCounter_ = 10;
				break;
			}
		}
	}
}

void ladderFallingCheck(Location* location, Player* player)
{
	if (player->currentLadder_ && player->isOnLadder_)
	{
		//Player reached top or bottom end of the ladder
		if (player->getCbPos().y - player->getCbSize().y > player->currentLadder_->getCbSize().y + player->currentLadder_->getCbPos().y
			|| player->getCbPos().y < player->currentLadder_->getCbPos().y)
		{
			player->currentLadder_ = NULL;
			player->isOnLadder_ = false;
		}
	}
}

void portalCheck(Location* location, Player* player, bool &changeLocation, std::string &locationSource)
{
	for (std::list<Portal*>::iterator it = location->portalList_.begin(); it != location->portalList_.end(); ++it)
	{
		if (abs(player->getCbPos().x - (*it)->getCbPos().x) < 25
			&& abs(player->getCbPos().y - (*it)->getCbPos().y) < 25
			&& sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
			&& player->portalCounter_ == 0 && player->isOnGround_)
		{
			locationSource = (*it)->getDestination();
			changeLocation = true;
			player->portalCounter_ = 100;
			player->setCbPos((*it)->getPlayerPos());
			(*it)->playSound();
			break;
		}
	}
}

void keyboardInput(Location* location, Player* player, ItemDatabase* itemDB, int itemSeed)
{
	//Left movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player->getCbPos().x > 0
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !player->isOnLadder_)
		player->move(left);

	//Right movement
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player->getCbPos().x < location->getSize().x
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !player->isOnLadder_)
		player->move(right);

	//Up movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		player->move(top);

	//Down movement
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		player->move(bottom);

	//Jump down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)
		&& player->isOnGround_ && player->currentPlatform_->isPassable())
	{
		player->isJumpingDown_ = true;
		if (player->fallCounter_ == 0)
			player->fallCounter_ = 10;
		player->fall();
	}

	//Jump while on the ground or off the ladder
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && !player->isFalling_)
	{
		if (player->isOnLadder_)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				player->velocity_.x = -(player->moveSpeed_ * player->moveSpeedMod_);
				player->jump(0.5);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				player->velocity_.x = player->moveSpeed_ * player->moveSpeedMod_;
				player->jump(0.5);
			}
		}
		else
			player->jump(1);
	}

	//Attack
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (!player->isOnLadder_ || player->isOnGround_))
	{
		player->currentWeapon_ = meleeWeapon;
		meleeAttack(player, location, itemDB, itemSeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && (!player->isOnLadder_ || player->isOnGround_))
	{
		player->currentWeapon_ = rangedWeapon;
		rangedAttack(player, location);
	}

	//Pick up item
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		for (std::list<Item*>::iterator itemIt = location->itemList_.begin(); itemIt != location->itemList_.end(); ++itemIt)
		{
			if (player->pickCounter_ == 0 && abs((*itemIt)->getCbPos().x - player->getCbPos().x) < 10 && abs((*itemIt)->getCbPos().y - player->getCbPos().y) < 10)
			{
				if (typeid(**itemIt).name() == typeid(class Money).name())
					player->money_ += (*itemIt)->getValue();
				else
				{
					player->itemList_.push_back(*itemIt);
					//DEBUG
					std::cout << "Picked up: Item" << std::endl;
				}
				location->itemList_.remove(*itemIt);
				player->pickCounter_ = 10;
				break;
			}
		}
	}
}

void gravityInfluence(Player* player)
{
	if (!player->isJumping_ && !player->isOnGround_ && !player->isOnLadder_)
		player->fall();
	if (player->fallCounter_ > 0)
		player->decrementCounters(0);
	else if (player->fallCounter_ == 0)
		player->isJumpingDown_ = false;
}

void dropItems(Location* location, ItemDatabase* itemDB, Enemy* enemy, int itemSeed)
{
	//Dropped consumable item
	if (typeid(*itemDB->getById(itemSeed)).name() == typeid(class Consumables).name())
	{
		Consumables* listedItem = dynamic_cast<Consumables*>(itemDB->getById(itemSeed));
		Consumables* droppedItem = new Consumables(listedItem);
		droppedItem->setCbPos(sf::Vector2f(enemy->getCbPos().x + generateRandomI(-20, 20), enemy->getCbPos().y));
		droppedItem->setPosition();
		location->itemList_.push_back(droppedItem);
	}

	//Dropped misc item
	else
	{
		Item* listedItem = itemDB->getById(itemSeed);
		Item* droppedItem = new Item(*listedItem);
		droppedItem->setCbPos(sf::Vector2f(enemy->getCbPos().x + generateRandomI(-20, 20), enemy->getCbPos().y));
		droppedItem->setPosition();
		location->itemList_.push_back(droppedItem);
	}

	//Drop coins
	Money* droppedMoney = new Money(generateRandomI(1, 10));
	droppedMoney->setCbPos(sf::Vector2f(enemy->getCbPos().x + generateRandomI(-20, 20), enemy->getCbPos().y));
	droppedMoney->setPosition();
	location->itemList_.push_back(droppedMoney);
}

void meleeAttack(Player* player, Location* location, ItemDatabase* itemDB, int itemSeed)
{
	if (player->attackCounter_ == 0)
	{
		player->attackCounter_ = 40;
		std::list<Enemy*>::iterator it;
		for (it = location->enemyList_.begin(); it != location->enemyList_.end(); ++it)
		{
			Enemy* listedEnemy = *it;
			if ((player->facing_ == right && player->getCbPos().x - listedEnemy->getCbPos().x <= 0 && player->getCbPos().x - listedEnemy->getCbPos().x > -100 && abs(player->getCbPos().y - listedEnemy->getCbPos().y) < 50) ||
				(player->facing_ == left && player->getCbPos().x - listedEnemy->getCbPos().x >= 0 && player->getCbPos().x - listedEnemy->getCbPos().x < 100 && abs(player->getCbPos().y - listedEnemy->getCbPos().y) < 50))
			{
				listedEnemy->HP_ -= 30;
				listedEnemy->attackedCounter_ = 200;
				listedEnemy->setHealthBar();
				if (listedEnemy->HP_ <= 0)
				{
					//Drop items
					dropItems(location, itemDB, *it, itemSeed);

					//Add exp and kill enemy
					player->addExp(listedEnemy->exp_);
					location->enemyList_.remove(listedEnemy);
					location->decrementEnemyCount();
					player->score_++;
				}
				break;
			}
		}
	}
}

void rangedAttack(Player* player, Location* location)
{
	//Shoot new projectiles
	if (player->attackCounter_ == 0)
	{
		player->attackCounter_ = 50;
		int velocityX;
		sf::Vector2f projectilePos;
		if (player->facing_ == left)
		{
			velocityX = -10;
			projectilePos.x = player->getCbPos().x - player->getCbSize().x / 2;
		}
		else if (player->facing_ == right)
		{
			velocityX = 10;
			projectilePos.x = player->getCbPos().x + player->getCbSize().x / 2;
		}
		projectilePos.y = player->getCbPos().y - 30;
		Projectile* bullet = new Projectile(projectilePos, sf::Vector2f(10, 10), sf::Vector2f(10, 10), velocityX);
		player->projectileList_.push_back(bullet);
	}
}

void moveProjectiles(Player* player, Location* location, ItemDatabase* itemDB, int synClock, int itemSeed)
{
	for (std::list<Projectile*>::iterator projectileIt = player->projectileList_.begin(); projectileIt != player->projectileList_.end(); ++projectileIt)
	{
		//Move existing projectiles
		(*projectileIt)->setCbPos(sf::Vector2f((*projectileIt)->getCbPos().x + (*projectileIt)->velocity_.x, (*projectileIt)->getCbPos().y));
		(*projectileIt)->setPosition();
		(*projectileIt)->travelDistance_ += abs((*projectileIt)->velocity_.x);
		
		//Delete projectiles
		if ((*projectileIt)->travelDistance_ > 600)
		{
			projectileIt = player->projectileList_.erase(projectileIt);
			if (player->projectileList_.size() == 0 || projectileIt == player->projectileList_.end())
				break;
		}
		else
		{
			for (std::list<Enemy*>::iterator it = location->enemyList_.begin(); it != location->enemyList_.end(); ++it)
			{
				//Detect projectile collision with the enemy
				if (abs((*projectileIt)->getCbPos().y - (*it)->getCbPos().y) < 100
					&& abs((*projectileIt)->getCbPos().x - (*it)->getCbPos().x) < 10)
				{
					if (player->projectileList_.size() > 1)
					{
						int x = 0;
					}
					(*it)->HP_ -= 10;
					(*it)->attackedCounter_ = 200;
					(*it)->setHealthBar();
					(*projectileIt)->travelDistance_ = 10000;
					if ((*it)->HP_ <= 0)
					{
						//Drop items
						dropItems(location, itemDB, *it, itemSeed);
						player->addExp((*it)->exp_);
						location->enemyList_.remove((*it));
						location->decrementEnemyCount();
						player->score_++;
					}
					break;
				}
			}
		}
	}
}

bool playerIsDead(Player* player, sf::RenderWindow &window, bool &changeLocation)
{
	if (player->getHP() <= 0)
	{
		window.clear();
		window.setView(window.getDefaultView());
		sf::RectangleShape deathScreen(sf::Vector2f(1360, 768));
		deathScreen.setPosition(sf::Vector2f(0, 0));
		sf::Texture deathScreenT;
		deathScreenT.loadFromFile("Images/skull2.png");
		deathScreen.setTexture(&deathScreenT);
		std::string sourceText = "You died!\nHere's some statistics:\n\nLevel achieved: " + std::to_string(player->getLevel()) + "\nEnemies killed: " + std::to_string(player->score_) + "\nGold gathered: " + std::to_string(player->money_);
		std::string text;
		sf::Font arial;
		arial.loadFromFile("Fonts/arial.ttf");
		sf::Text textTxt;
		textTxt.setFont(arial);
		textTxt.setCharacterSize(40);
		textTxt.setPosition(sf::Vector2f(800, 100));
		textTxt.setString(text);
		textTxt.setFillColor(sf::Color::White);
		int i = 0;
		sf::Clock clock, clock2;
		while (clock.getElapsedTime().asSeconds() < 8)
		{
			if (clock2.getElapsedTime().asMilliseconds() == 50 && i < sourceText.size())
			{
				clock2.restart();
				text += sourceText[i];
				textTxt.setString(text);
				i++;
				window.draw(deathScreen);
				window.draw(textTxt);
				window.display();
				window.clear();
			}
		}
		changeLocation = true;
		return true;
	}
	else
		return false;
}

Player* createCharacter(sf::RenderWindow &window)
{
	sf::RectangleShape characterCreatorScreen(sf::Vector2f(window.getSize().x, window.getSize().y));
	sf::Texture characterCreatorScreenT;
	characterCreatorScreenT.loadFromFile("Images/characterCreatorScreen.png");
	characterCreatorScreen.setTexture(&characterCreatorScreenT);

	sf::RectangleShape playerBody(sf::Vector2f(160, 200));
	sf::Texture playerBodyT;
	playerBodyT.loadFromFile("Images/Flash/CharacterTest/Character0001.png");
	playerBody.setTexture(&playerBodyT);
	playerBody.setPosition(sf::Vector2f(800, 300));

	sf::Event event;

	std::string nameInput;

	sf::Font arial;
	arial.loadFromFile("Fonts/arial.ttf");

	sf::Text name;
	name.setFont(arial);
	name.setCharacterSize(32);
	name.setPosition(180, 44);
	name.setFillColor(sf::Color::White);

	int str = 0, end = 0, dex = 0, ing = 0, luc = 0, sum = 0;
	char strS[3], endS[3], dexS[3], ingS[3], lucS[3];

	sf::Text strTxt;
	strTxt.setFont(arial);
	strTxt.setCharacterSize(32);
	strTxt.setPosition(240, 118);
	strTxt.setFillColor(sf::Color::White);

	sf::Text endTxt;
	endTxt.setFont(arial);
	endTxt.setCharacterSize(32);
	endTxt.setPosition(240, 155);
	endTxt.setFillColor(sf::Color::White);

	sf::Text dexTxt;
	dexTxt.setFont(arial);
	dexTxt.setCharacterSize(32);
	dexTxt.setPosition(240, 192);
	dexTxt.setFillColor(sf::Color::White);

	sf::Text ingTxt;
	ingTxt.setFont(arial);
	ingTxt.setCharacterSize(32);
	ingTxt.setPosition(240, 229);
	ingTxt.setFillColor(sf::Color::White);

	sf::Text lucTxt;
	lucTxt.setFont(arial);
	lucTxt.setCharacterSize(32);
	lucTxt.setPosition(240, 266);
	lucTxt.setFillColor(sf::Color::White);

	bool characterIsDone = false;

	while (!characterIsDone)
	{
		//Event Loop
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					characterIsDone = true;
					window.close();
					break;
				case sf::Keyboard::Return:
					characterIsDone = true;
					break;
				case sf::Keyboard::BackSpace:
					if (nameInput.size() > 0)
					{
						nameInput.erase(nameInput.end() - 1);
						name.setString(nameInput);
					}
					break;
				case sf::Keyboard::LAlt:
					while (sum != 25)
					{
						str = generateRandomI(1, 10);
						end = generateRandomI(1, 10);
						dex = generateRandomI(1, 10);
						ing = generateRandomI(1, 10);
						luc = generateRandomI(1, 10);
						sum = str + end + dex + ing + luc;
					}
					sum = 0;
					break;
				default:
					break;
				}
			}
			if (event.type == sf::Event::Closed)
			{
				characterIsDone = true;
				window.close();
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (nameInput.size() < 30 && ((event.text.unicode >= 65 && event.text.unicode <= 90) || (event.text.unicode >= 97 && event.text.unicode <= 122) || event.text.unicode == 32))
				{
					nameInput += static_cast<char>(event.text.unicode);
					name.setString(nameInput);
				}
			}
		}

		//Update rolled stats
		_itoa(str, strS, 10);
		strTxt.setString(strS);
		_itoa(end, endS, 10);
		endTxt.setString(endS);
		_itoa(dex, dexS, 10);
		dexTxt.setString(dexS);
		_itoa(ing, ingS, 10);
		ingTxt.setString(ingS);
		_itoa(luc, lucS, 10);
		lucTxt.setString(lucS);

		window.draw(characterCreatorScreen);
		window.draw(name);
		window.draw(strTxt);
		window.draw(endTxt);
		window.draw(dexTxt);
		window.draw(ingTxt);
		window.draw(lucTxt);
		window.draw(playerBody);
		window.display();
		window.clear();
	}
	Player* player = new Player();
	player->setName(nameInput);
	player->hud_.update(nameInput, player->getLevel(), player->getMaxHP(), player->getHP(), player->getExp(), player->getMoney());
	return player;
}

void toggleDebugMode(bool &isDebugModeActive)
{
	if (isDebugModeActive)
		isDebugModeActive = false;
	else
		isDebugModeActive = true;
}