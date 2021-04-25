#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
	private:
		sf::Texture texture;
		sf::Vector2f location;
		sf::Vector2f size;
		sf::RectangleShape shape;

		void updateEnemy();

	public:
		sf::RectangleShape getShape();
		sf::Vector2f getLocation();
		sf::Vector2f getSize();
		sf::Texture getTexture();
		void setTexture(sf::Texture newTexture);
		void setLocation(sf::Vector2f newLocation);
		void setSize(sf::Vector2f newSize);

		Enemy();
		Enemy(sf::Vector2f location, sf::Vector2f size, sf::Texture texture);
};

