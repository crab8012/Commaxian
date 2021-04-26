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
	sf::IntRect getSize();
	sf::Texture getTexture();
	void setTexture(sf::Texture& newTexture);
	void setTexture(std::string textureLocation);
	void setLocation(sf::Vector2f newLocation);
	void setSize(sf::IntRect newSize);

	void move(float x, float y);
	float getMoveDirection();
	void setMoveDirection(float newDirection);

	void handleInput();

	Enemy();
	Enemy(sf::Vector2f location, sf::IntRect size, sf::Texture& texture);
	Enemy(sf::Vector2f location, sf::IntRect size, std::string textureLocation);
};