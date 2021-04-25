#include "Enemy.h"

sf::RectangleShape Enemy::getShape()
{
	return sf::RectangleShape();
}

sf::Vector2f Enemy::getLocation() {
	return this->location;
}
sf::Vector2f Enemy::getSize() {
	return this->size;
}
sf::Texture Enemy::getTexture() {
	return this->texture;
}
void Enemy::setTexture(sf::Texture newTexture) {
	this->texture = newTexture;
	updateEnemy();
}
void Enemy::setLocation(sf::Vector2f newLocation) {
	this->location = newLocation;
	updateEnemy();
}
void Enemy::setSize(sf::Vector2f newSize) {
	this->size = newSize;
	updateEnemy();
}

void Enemy::updateEnemy() {
	this->shape.setSize(this->size);
	this->shape.setTexture(&this->texture);
	this->shape.setPosition(this->location);
}

Enemy::Enemy() {
	this->location = sf::Vector2f(10.0f, 10.0f);
	this->size = sf::Vector2f(5.0f, 5.0f);
	this->texture = sf::Texture();
	this->shape = sf::RectangleShape(size);
	this->shape.setTexture(&texture);
	this->shape.setPosition(location);
}

Enemy::Enemy(sf::Vector2f location, sf::Vector2f size, sf::Texture texture) {
	this->location = location;
	this->size = size;
	this->texture = texture;
	this->shape = sf::RectangleShape(size);
	this->shape.setTexture(&texture);
	this->shape.setPosition(location);
}