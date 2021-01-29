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
	bool playersCollision(sf::Vector2f t_playerTwoPos);
	void setPosition(sf::Vector2f t_pos){
		player.setPosition(t_pos);
	}
	void setColor(sf::Color t_color){
		player.setFillColor(t_color);
	}
	sf::Vector2f getPosition()
	{
		return player.getPosition();
	};
	void setTagged(bool t_tagged)
	{
		m_tagged = t_tagged;
	}
	bool getTagged()
	{
		return m_tagged;
	}

private:
	sf::CircleShape player;
	sf::Vector2f pos;
	bool playerOne = false;
	bool m_tagged = false;
	sf::Vector2f m_Player2Position;
	float playerRadius = 10.0f;
	bool m_colision = false;
	
};
