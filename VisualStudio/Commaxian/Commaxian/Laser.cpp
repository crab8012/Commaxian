#include "Laser.h"

void Laser::updateLaser() {
	this->boundingBox = this->sprite.getGlobalBounds();
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(this->size);
}

sf::Sprite& Laser::getSprite() {
	return this->sprite;
}

sf::Vector2f Laser::getLocation() {
	return this->sprite.getPosition();
}

void Laser::setLocation(sf::Vector2f newLocation) {
	this->sprite.setPosition(newLocation);
}

sf::IntRect Laser::getSize() {
	return this->sprite.getTextureRect();
}

void Laser::setSize(sf::IntRect newSize) {
	this->sprite.setTextureRect(newSize);
}

sf::Texture Laser::getTexture() {
	return this->texture;
}

void Laser::setTexture(sf::Texture& newTexture) {
	this->texture = newTexture;
	this->updateLaser();
}

sf::FloatRect Laser::getBoundingBox() {
	return this->sprite.getGlobalBounds();
}

void Laser::move(float x, float y) {
	this->sprite.move(x, y);
}

void Laser::fire() { // Pixels per Second
	this->alive = true;
	while (this->alive) {
		if (this->clock.getElapsedTime().asMilliseconds() >= 50) { // Every 1/100s, move the appropriate amount
			this->move(0, -(400.0f / 10));
			this->clock.restart();
		}
	}
}

void Laser::kill() {
	this->alive = false;
}

bool Laser::isAlive() {
	return this->alive;
}

Laser::Laser() {
	this->sprite = sf::Sprite();
	this->setLocation(sf::Vector2f(10.0f, 10.0f));
	this->size = sf::IntRect(10, 10, 32, 32);
	this->texture = sf::Texture();

	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(size);
	this->sprite.setPosition(this->getLocation());
}

Laser::Laser(sf::Vector2f location, sf::IntRect size, sf::Texture& texture) {
	this->sprite = sf::Sprite();
	this->setLocation(location);
	this->size = size;
	this->texture = texture;

	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(size);
	this->sprite.setPosition(location);
}