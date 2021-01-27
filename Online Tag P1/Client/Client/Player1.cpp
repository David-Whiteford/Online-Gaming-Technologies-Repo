#include "Player1.h"

Player1::Player1()
{
}

Player1::~Player1()
{
}

void Player1::setUp()
{
	player.setRadius(10);
	player.setOutlineColor(sf::Color::Green);
	player.setOutlineThickness(5);
	player.setPosition(10, 20);
}

void Player1::draw(sf::RenderWindow& t_window)
{
	t_window.draw(player);
}

void Player1::update()
{
	move();
}

void Player1::move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		pos = player.getPosition();
		pos.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		pos = player.getPosition();
		pos.x += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		pos = player.getPosition();
		pos.y -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		pos = player.getPosition();
		pos.y += 1.0f;
	}
	player.setPosition(pos);
}
