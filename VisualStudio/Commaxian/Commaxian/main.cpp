#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Enemy.h"
#include "Player.h"

// Functions past Main
void renderItems(sf::RenderWindow& window);
void getInput(sf::RenderWindow& window);
void gameLoop(sf::RenderWindow& window);

// Entities
Player player;
std::vector<Enemy> enemies;

// Textures
sf::Texture texture2;
sf::Texture texture3;

// Fonts
sf::Font font;

// UI
sf::Text scoreTxt;
sf::Text attributionTxt;

// The Enemy Clock
sf::Clock enemyClock;
int position = 0;
bool isGameOver = false;

int main() {
	sf::RenderWindow window(sf::VideoMode(640, 480), "Commaxian");

	// Load textures
	if (!texture3.loadFromFile("textures/player.png"))
		std::cout << "Error loading Player texture" << std::endl;

	// Load fonts
	//if (!font.loadFromFile("fonts/arial.ttf"))
	//	std::cout << "Error loading font arial.ttf" << std::endl;
	if (!font.loadFromFile("fonts/SharpRetro.ttf"))
		std::cout << "Error loading font SharpRetro.ttf" << std::endl;

	// Create player
	player = Player(sf::Vector2f(100.0f, 435.0f), sf::IntRect(0, 0, 32, 32), texture3);
	
	// Create the Enemies
	for(int i = 0; i < 4; i++) {
		Enemy e = Enemy(sf::Vector2f((i*100.0f) + 50.0f, 100.0f), sf::IntRect(0, 0, 32, 32), texture3);
		e.getSprite().setColor(sf::Color(40, 10, 30));
		e.getSprite().setRotation(180.0f);
		enemies.push_back(e);
	}

	// Create the UI
	//		The score text
	scoreTxt.setFont(font);
	scoreTxt.setString("Score: 000,000,000");
	scoreTxt.setCharacterSize(25); // px, not pt
	scoreTxt.setFillColor(sf::Color::White);
	scoreTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	scoreTxt.setPosition(sf::Vector2f(410, 0));
	//		The Attribution text
	attributionTxt.setFont(font);
	attributionTxt.setString("Font: Sharp Retro by JROB774 on OpenGameArt.org");
	attributionTxt.setCharacterSize(10); // px, not pt
	attributionTxt.setFillColor(sf::Color::White);
	attributionTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	attributionTxt.setPosition(sf::Vector2f(5, 465));
	
	enemyClock.restart(); // Restart the clock
	gameLoop(window);
	return 0;
}

void gameLoop(sf::RenderWindow& window) {
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		
		if (!isGameOver) {
			sf::Time elapsed = enemyClock.getElapsedTime();
			// If the clock hits a predetermined interval, move the enemies
			if (elapsed.asSeconds() >= 0.02f) {
				for (int i = 0; i < enemies.size(); i++) {
					if (enemies[i].getLocation().x >= 630 || enemies[i].getLocation().x <= 10) {
						enemies[i].setMoveDirection(enemies[i].getMoveDirection() * -1);
						enemies[i].move(enemies[i].getMoveDirection(), 32.0f);
					}
					enemies[i].move(enemies[i].getMoveDirection(), 0.0f);

					if (enemies[i].getLocation().y >= 450)
						isGameOver = true;
				}
				enemyClock.restart();
			}
		}
		

		getInput(window);

		window.clear();
		renderItems(window);
		window.display();
	}
}

void getInput(sf::RenderWindow& window) {
	player.handleInput();
	
	// If the escape key is pressed, for now just close the window
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}
}

void renderItems(sf::RenderWindow& window) {
	// Draw things on the screen
	window.draw(player.getSprite()); // Draw the player sprite
	
	for (int i = 0; i < enemies.size(); i++) {
		window.draw(enemies[i].getSprite());
	}

	window.draw(scoreTxt);
	window.draw(attributionTxt);
}