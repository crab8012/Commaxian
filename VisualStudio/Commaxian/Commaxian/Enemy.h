#pragma once
#include <SFML/Graphics.hpp>

class Enemy
{
private:
	sf::Texture texture;
	sf::Vector2f location;
	sf::IntRect size;
	sf::Sprite sprite;

	float moveDirection;

	void updateEnemy();

public:
	sf::Sprite& getSprite();

	sf::Vector2f getLocation();
	void setLocation(sf::Vector2f newLocation);
	sf::IntRect getSize();
	void setSize(sf::IntRect newSize);

	sf::Texture getTexture();
	void setTexture(sf::Texture& newTexture);

	float getMoveDirection();
	void setMoveDirection(float newDirection);
	void move(float x, float y);
	void start(float interval);

	Enemy();
	Enemy(sf::Vector2f location, sf::IntRect size, sf::Texture& texture);
};