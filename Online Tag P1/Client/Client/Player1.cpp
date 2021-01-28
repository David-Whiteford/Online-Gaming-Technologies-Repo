#include "Player1.h"

Player1::Player1()
{
}

Player1::~Player1()
{
}

void Player1::setUp(sf::Color t_color, sf::Vector2f t_pos, bool t_playerOne)
{
	player.setRadius(playerRadius);
	player.setOrigin(sf::Vector2f(playerRadius, playerRadius));
	player.setOutlineColor(t_color);
	player.setOutlineThickness(5);
	player.setPosition(t_pos);
	playerOne = t_playerOne;
}

void Player1::draw(sf::RenderWindow& t_window)
{
	t_window.draw(player);
}

void Player1::update(sf::Vector2f t_pos)
{
	m_Player2Position = t_pos;
	if (playerOne == true)
	{
		movePlayer1();
	}
	else 
	{
		movePlayer2(t_pos);
	}
}

void Player1::movePlayer1()
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

void Player1::movePlayer2(sf::Vector2f t_pos)
{
	player.setPosition(t_pos);
}

bool Player1::playersCollision(sf::Vector2f t_playerTwoPos)
{
	
	if (player.getPosition().x - playerRadius < t_playerTwoPos.x + playerRadius
		&& player.getPosition().x + playerRadius > t_playerTwoPos.x - playerRadius
		&& player.getPosition().y - playerRadius < t_playerTwoPos.y + playerRadius
		&& player.getPosition().y + playerRadius > t_playerTwoPos.y - playerRadius)
	{
		m_colision = true;
	}
	else
	{
		m_colision = false;
	}
	return m_colision;
}


