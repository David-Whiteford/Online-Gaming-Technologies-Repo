#pragma once
#include <SFML/Graphics.hpp>

class Player1
{
public:
	Player1();
	~Player1();
	void setUp();
	void draw(sf::RenderWindow& t_window);
	void update();
	void move();

private:
	sf::CircleShape player;
	sf::Vector2f pos;
};
