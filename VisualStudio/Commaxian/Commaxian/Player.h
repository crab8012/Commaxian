#pragma once
#include <SFML/Graphics.hpp>

class Player
{
	private:
		sf::Texture texture;
		sf::Vector2f location;
		sf::IntRect size;
		sf::Sprite sprite;

		void updatePlayer();

	public:
		sf::Sprite getSprite();
		sf::Vector2f getLocation();
		sf::IntRect getSize();
		sf::Texture getTexture();
		void setTexture(sf::Texture& newTexture);
		void setLocation(sf::Vector2f newLocation);
		void setSize(sf::IntRect newSize);

		void move(float x, float y);

		void handleInput();

		Player();
		Player(sf::Vector2f location, sf::IntRect size, sf::Texture& texture);
};

