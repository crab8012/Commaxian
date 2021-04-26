#pragma once
#include <SFML/Graphics.hpp>

class Laser
{
private:

	// What is required to use the laser object
	// Texture
	// Position
	// Bounding Box
	// Sprite
	// Size of the Texture
	// Clock to manage the laser's movement
	
	// Update method (Called every time to update the sprite with the object position
	// Move method (Move the laser, only in one direction
	// Fire method that, when called, starts moving our laser


	sf::Texture texture;
	sf::IntRect size;
	sf::Sprite sprite;
	sf::FloatRect boundingBox;
	sf::Clock clock;
	bool alive = false;

	void updateLaser();

public:
	sf::Sprite& getSprite();

	sf::Vector2f getLocation();
	void setLocation(sf::Vector2f newLocation);

	sf::IntRect getSize();
	void setSize(sf::IntRect newSize);

	sf::Texture getTexture();
	void setTexture(sf::Texture& newTexture);

	sf::FloatRect getBoundingBox();

	void move(float x, float y);

	void fire();

	void kill();
	bool isAlive();

	Laser();
	Laser(sf::Vector2f location, sf::IntRect size, sf::Texture& texture);
};