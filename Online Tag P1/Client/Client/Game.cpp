#include "Game.h"

Game::Game() : m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32 }, "Online Tag by David Whiteford" }, m_exitGame{ false }
{
	//player one set as tagged and setup the players
	playerOneTagged = true;
	setupAssets();
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
	//gets the data from the player and converts to a string 
	float xVal = playerOne.getPosition().x;
	std::ostringstream stringStreamPlayer1X;
	stringStreamPlayer1X << xVal;
	float yVal = playerOne.getPosition().y;
	std::ostringstream stringStreamPlayer1Y;
	stringStreamPlayer1Y << yVal;
	//gets the data for which player is tagged
	std::ostringstream stringStreamPlayerTagged;
	stringStreamPlayerTagged << playerOneTagged;
	//puts all data in a string and sends the data to the server
	std::string s(stringStreamPlayer1X.str() + " " + stringStreamPlayer1Y.str() + " " + stringStreamPlayerTagged.str());
	client1->SendPlayerData(s);
	//gets the data o the player x y 
	std::string playerTwoData = client1->GetPlayerData();
	std::istringstream iss{ playerTwoData };
	//stores the data for the player2 in the floats x and y 
	//also stores the data for the player is tagged in the playerTaged bool
	float x{}, y{};
	bool playerTaged;
	iss >> x >> y >> playerTaged;
	//pass in the player 2 positions
	m_Player2Position = sf::Vector2f(x, y);
	playerOne.update(sf::Vector2f(0,0));
	playerTwo.update(m_Player2Position);
	//check for a collision between the two player
	//sets the player that was running to be now tagged 
	if (playerOne.playersCollision(m_Player2Position))
	{
		playerOne.setPosition(sf::Vector2f(100, 100));
		if (playerOneTagged == true && m_collision) {
			playerOneTagged = false;
			m_collision = false;
		}
		else if (playerOneTagged == false&& m_collision) {
			playerOneTagged = true;
			m_collision = false;
		}
		m_collision = true;
	}
	//makes the player that is tagged blue and the other white
	if (playerOneTagged)
	{
		playerOne.setColor(sf::Color::Blue);
		playerTwo.setColor(sf::Color::White);
	}
	else
	{
		playerOne.setColor(sf::Color::White);
		playerTwo.setColor(sf::Color::Blue);
	}
	if (m_exitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	//render both players 
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
