#include "Player.h"

sf::Sprite Player::getSprite()
{
	return this->sprite;
}

sf::Vector2f Player::getLocation() {
	return this->location;
}
sf::IntRect Player::getSize() {
	return this->size;
}
sf::Texture Player::getTexture() {
	return this->texture;
}
void Player::setTexture(sf::Texture& newTexture) {
	this->texture = newTexture;
	updatePlayer();
}
void Player::setTexture(std::string textureLocation) {
	//this->texture = newTexture;
	if (!this->texture.loadFromFile(textureLocation))
	{
		// Somehow show an issue
	}
	updatePlayer();
}
void Player::setLocation(sf::Vector2f newLocation) {
	this->location = newLocation;
	updatePlayer();
}
void Player::setSize(sf::IntRect newSize) {
	this->size = newSize;
	updatePlayer();
}

void Player::move(float x, float y)
{
	this->sprite.move(x, y);
	this->location = this->sprite.getPosition();
}

void Player::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // If the left arrow is pressed
	{
		if (getLocation().x > 10) { // If our player is within it's little box of movement
			move(-0.25f, 0.f);// Move left
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // If the right arrow is pressed
	{
		if (getLocation().x < 600) { // If our player is within it's little box of movement
			move(0.25f, 0.f); // Move right
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // If the spacebar is pressed
		// Fire the laser!
	}
}

void Player::updatePlayer() {
	this->sprite.setTextureRect(this->size);
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(this->location);
}

Player::Player() {
	this->location = sf::Vector2f(10.0f, 10.0f);
	this->size = sf::IntRect(10, 10, 32, 32);
	this->texture = sf::Texture();
	this->sprite = sf::Sprite();
	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(size);
	this->sprite.setPosition(location);
}

Player::Player(sf::Vector2f location, sf::IntRect size, sf::Texture& texture) {
	this->location = location;
	this->size = size;
	this->texture = texture;
	this->sprite = sf::Sprite();
	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(size);
	this->sprite.setPosition(location);
}

Player::Player(sf::Vector2f location, sf::IntRect size, std::string textureLocation) {
	this->location = location;
	this->size = size;
	setTexture(textureLocation);
	this->sprite = sf::Sprite();
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(size);
	this->sprite.setPosition(location);
}