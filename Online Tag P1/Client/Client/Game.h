#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <sstream>
#include "Globals.h"
#include "client.h"
#include "Player1.h"

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	Player1 playerOne;
	Player1 playerTwo;
	Client *client1;
	//Game Loop
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
	void setupAssets();

	//Font
	sf::Font m_ArialBlackfont;

	//Screens
	sf::RenderWindow m_window;
	bool playerOneTagged = false;
	bool m_collision = false;
	sf::Vector2f m_Player2Position;
	bool m_exitGame;
};

#endif