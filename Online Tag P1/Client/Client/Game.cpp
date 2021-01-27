#include "Game.h"

Game::Game() : m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32 }, "Online Tag by David Whiteford" }, m_exitGame{ false }
{

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
	std::string playerdata = "I AM AWSOME";
	client1->SendPlayerData(playerdata);
	playerOne.update();
	if (m_exitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	playerOne.draw(m_window);
	m_window.display();
}

void Game::setupAssets()
{
	playerOne.setUp();

}
