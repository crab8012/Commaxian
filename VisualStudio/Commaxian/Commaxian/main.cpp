#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>

#include "Enemy.h"
#include "Player.h"
#include "Laser.h"

// Functions past Main
void renderItems(sf::RenderWindow& window);
void getInput(sf::RenderWindow& window);
void gameLoop(sf::RenderWindow& window);

// Feature support
bool useShaders = false; // False by default... We don't know if shaders are supported yet

// States
bool isGameOver = false; // This determines if the specialized game loops should run
int score = 0; // The temporary score variable

// Textures and Fonts
sf::Texture laserTexture;
sf::Texture texture2;
sf::Texture texture3;
sf::Font font;

// Entities
Player player;
std::vector<Enemy> enemies;
Laser laser;
sf::Thread laserThread(&Laser::fire, &laser); // Setup a thread to launch the laser

std::vector<sf::Vector2f> enemyRespawnLocations = {sf::Vector2f(50, 100), sf::Vector2f(100, 100), 
												   sf::Vector2f(150, 100), sf::Vector2f(200, 100) }; // Spawn a new enemy in a random position in Enemy Row 1

// UI text
sf::Text scoreTxt;
sf::Text attributionTxt;
sf::Text gameOverTxt;

// Clocks
sf::Clock enemyClock; // Used to determine when an enemy updates or moves

// Shaders
sf::Shader testShader; // The testShader currently does nothing... It's here for future expansion if I have time to learn GLSL

int main() {
	srand(time(NULL));
	// Check if features are supported
	useShaders = sf::Shader::isAvailable(); // Are shaders supported?

	sf::RenderWindow window(sf::VideoMode(640, 480), "Commaxian", sf::Style::Titlebar | sf::Style::Close); // Set the game window to a size of 640x480px with a title "Commaxian". Also disables the ability to resize the window

	// Load textures
	if (!texture3.loadFromFile("res/textures/player.png"))
		std::cout << "Error loading Player texture" << std::endl;

	if (!laserTexture.loadFromFile("res/textures/laser.png"))
		std::cout << "Error loading Laser texture" << std::endl;


	// Load fonts
	//if (!font.loadFromFile("fonts/arial.ttf"))
	//	std::cout << "Error loading font arial.ttf" << std::endl;
	if (!font.loadFromFile("res/fonts/SharpRetro.ttf"))
		std::cout << "Error loading font SharpRetro.ttf" << std::endl;

	laser = Laser(player.getLocation(), sf::IntRect(0, 0, 8, 32), laserTexture); // We will only have one laser at a time

	// Create player
	player = Player(sf::Vector2f(100.0f, 435.0f), sf::IntRect(0, 0, 32, 32), texture3);
	
	// Create the Enemies
	for(int i = 0; i < 10; i++) {
		Enemy e = Enemy(sf::Vector2f((i*50.0f) + 50.0f, 100.0f), sf::IntRect(0, 0, 32, 32), texture3);
		e.getSprite().setColor(sf::Color(40, 10, 30));
		e.getSprite().setRotation(180.0f);
		enemies.push_back(e);
	}
	for (int i = 0; i < 10; i++) {
		Enemy e = Enemy(sf::Vector2f((i * 50.0f) + 50.0f, 150.0f), sf::IntRect(0, 0, 32, 32), texture3);
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
	//		The Game Over text
	gameOverTxt.setFont(font);
	gameOverTxt.setString("GAME OVER");
	gameOverTxt.setCharacterSize(100); // px, not pt
	gameOverTxt.setFillColor(sf::Color::White);
	gameOverTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	gameOverTxt.setPosition(window.getSize().x / 6, window.getSize().y / 3);

	// Load the Shaders, but only if they are supported
	if (useShaders) {
		if (!testShader.loadFromFile("res/shaders/test.vs", "res/shaders/test.fs"))
			std::cout << "Error loading Test shader." << std::endl;
		else // If the shader loaded successfully, configure it
			testShader.setUniform("texture", sf::Shader::CurrentTexture);
	}
	
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
			// If we have a laser
			if (laser.isAlive()) {
				// Test if it hits the edge of the screen
				if (laser.getLocation().y < 30) {
					laser.kill();
				}
				else { // Otherwise we still have the laser, and can try to remove the enemies
					for (int j = enemies.size() - 1; j >= 0; j--) {
						if (laser.getBoundingBox().intersects(enemies[j].getSprite().getGlobalBounds())) { // If the laser hits an enemy
							laser.kill();
							//enemies.erase(enemies.begin() + j); // We can erase the enemies
							enemies[j].getSprite().setPosition(enemyRespawnLocations[rand() % enemyRespawnLocations.size()]); // Or provide infinite gameplay
							score++; // Add 1 to the score
						}
					}
				}
			}

			sf::Time elapsed = enemyClock.getElapsedTime();
			// If the clock hits a predetermined interval, move the enemies
			if (elapsed.asSeconds() >= 0.2f) {
				for (int i = 0; i < enemies.size(); i++) {
					if (enemies[i].getLocation().x >= 630 || enemies[i].getLocation().x <= 10) {
						enemies[i].setMoveDirection(enemies[i].getMoveDirection() * -1);
						enemies[i].move(enemies[i].getMoveDirection(), 32.0f);
					}
					enemies[i].move(enemies[i].getMoveDirection(), 0.0f);

					// Game over if the enemy is in the player's row
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
		laserThread.terminate();
		window.close();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
		isGameOver = true; // End the game for testing purposes
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		// Create a new laser at the player position, then fire
		if (!laser.isAlive()) { // But only if a laser does not exist.
			laser.getSprite().setPosition(player.getSprite().getPosition());
			// run it
			laserThread.launch(); // Fire the laser
		}
	}
}

// Draw things on the screen
void renderItems(sf::RenderWindow& window) {
	// Render with shaders, if supported
	if (useShaders) {
		if (!isGameOver) { // If the game isn't over, draw the entities
			window.draw(player.getSprite(), &testShader); // Draw the player sprite
			for (int i = 0; i < enemies.size(); i++) { // Draw the enemies
				window.draw(enemies[i].getSprite(), &testShader);
			}
			if (laser.isAlive()) {
				window.draw(laser.getSprite(), &testShader);
			}
		}
	}
	// Otherwise, render normally
	else{
		if (!isGameOver) { // If the game isn't over, draw the entities
			window.draw(player.getSprite()); // Draw the player sprite
			for (int i = 0; i < enemies.size(); i++) { // Draw the enemies
				window.draw(enemies[i].getSprite());
			}
			if (laser.isAlive()) {
				window.draw(laser.getSprite());
			}
		}
	}
	
	
	// These will not be rendered using shaders.
	scoreTxt.setString("Score: " + std::string(9 - std::to_string(score).length(), '0') + std::to_string(score)); // Update the score text from the score variable
	window.draw(scoreTxt);
	window.draw(attributionTxt);

	// If the game is over, draw the GameOver text
	if (isGameOver) {
		window.draw(gameOverTxt);
	}

}