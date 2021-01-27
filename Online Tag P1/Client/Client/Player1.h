#pragma once
#include <SFML/Graphics.hpp>

class Player1
{
public:
	Player1();
	~Player1();
	void setUp(sf::Color t_color, sf::Vector2f t_pos , bool t_playerOne);
	void draw(sf::RenderWindow& t_window);
	void update(sf::Vector2f t_pos);
	void movePlayer1();
	void movePlayer2(sf::Vector2f t_pos);
	sf::Vector2f getPosition()
	{
		return player.getPosition();
	};

private:
	sf::CircleShape player;
	sf::Vector2f pos;
	bool playerOne = false;
	sf::Vector2f m_Player2Position;
};
