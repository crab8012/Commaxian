#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Enemy.h"
#include "Player.h"

void renderItems(sf::RenderWindow& window);
void getInput();

sf::Sprite sprite;

Player player;

int main() {
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

	// Create player
	// Load texture from file
	sf::Texture texture;
	

	// Create enemy
	//Enemy testEnemy(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(25.0f, 25.0f), texture);
	//window.draw(testEnemy.getShape());


	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(10, 10, 32, 32));
	sprite.setPosition(sf::Vector2f(50.0f, 430.0f));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		getInput();

		window.clear();
		renderItems(window);
		window.display();
	}

	return 0;
}

void getInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (sprite.getPosition().x > 10) {
			// left key is pressed: move our character
			sprite.move(-0.25f, 0.f);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (sprite.getPosition().x < 600) {
			// right key is pressed: move our character
			sprite.move(0.25f, 0.f);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		// Fire the laser!
	}
}

void renderItems(sf::RenderWindow& window) {
	window.draw(sprite);
}