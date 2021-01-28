#include "Game.h"

Game::Game() : m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32 }, "Online Tag by David Whiteford" }, m_exitGame{ false }
{

	setupAssets();
	playerOne.setTagged(true);
	playerTwo.setTagged(false);
	client1 = new Client("127.0.0.1", 1111);
	if (!client1->Connect())
	{
		//output message if failed to connect to server and pause system
		std::cout << "Failed connection to the server" << std::endl;
	}



}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.0f / 60.f);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
			render();
		}
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type)
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type)
		{
			if ((sf::Keyboard::Escape == event.key.code))
			{
				//Pause
			}
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{
	float xVal = playerOne.getPosition().x;
	std::ostringstream stringStreamPlayer1X;
	stringStreamPlayer1X << xVal;
	float yVal = playerOne.getPosition().y;
	std::ostringstream stringStreamPlayer1Y;
	stringStreamPlayer1Y << yVal;
	std::string s(stringStreamPlayer1X.str() + " " + stringStreamPlayer1Y.str());
	client1->SendPlayerData(s);
	//std::cout << client1->GetPlayerData();

	std::string playerTwoData = client1->GetPlayerData();
	std::istringstream iss{ playerTwoData };
	float x{}, y{};
	iss >> x >> y;
	std::cout << "x: " << x << "y:" << y << std::endl;
	//pass in the player 2 positions
	m_Player2Position = sf::Vector2f(x, y);
	playerOne.update(sf::Vector2f(0,0));
	playerTwo.update(m_Player2Position);
	//if (playerOne.playersCollision(m_Player2Position)== true && playerOne.getTagged() == true)
	//{
	//	playerTwo.setPosition(sf::Vector2f(500, 10));
	//	playerOne.setPosition(sf::Vector2f(10, 10));
	//	playerTwo.setTagged(true);
	//	
	//}
	//else if (playerOne.playersCollision(m_Player2Position) == true && playerTwo.getTagged() == true)
	//{
	//	playerTwo.setPosition(sf::Vector2f(500, 10));
	//	playerOne.setPosition(sf::Vector2f(10, 10));
	//	playerOne.setTagged(true);
	//}
	std::cout << "COL: " << playerOne.playersCollision(m_Player2Position);

	if (m_exitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	playerOne.draw(m_window);
	playerTwo.draw(m_window);
	m_window.display();
}

void Game::setupAssets()
{
	//set up the players
	playerOne.setUp(sf::Color::Green,sf::Vector2f(10,20) , true);
	playerTwo.setUp(sf::Color::Red, sf::Vector2f(200, 20) , false);

}
