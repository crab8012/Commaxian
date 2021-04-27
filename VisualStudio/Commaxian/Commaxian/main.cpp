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
void quit(sf::RenderWindow& window);
void gameLoop(sf::RenderWindow& window);
sf::Vector2f gridToScreen(int x, int y);

// Feature support
bool useShaders = false; // False by default... We don't know if shaders are supported yet

// States
bool isGameOver = false; // This determines if the specialized game loops should run
bool isInMenu = true; // This determines if the specialized game loops should run
bool isPaused = false;
int score = 0; // The temporary score variable

// Key debounce variables
bool escapePress = false, escapeRelease = true, escapeTapped = false;
bool yPress = false, yRelease = true, yTapped = false;
bool nPress = false, nRelease = true, nTapped = false;
bool enterPress = false, enterRelease = true, enterTapped = false;

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

// UI text
sf::Text scoreTxt;
sf::Text attributionTxt;
sf::Text gameOverTxt;

// Menu text
sf::Text titleTxt;
sf::Text pauseTxt;

// Clocks
sf::Clock enemyClock; // Used to determine when an enemy updates or moves

// Shaders
sf::Shader testShader; // The testShader currently does nothing... It's here for future expansion if I have time to learn GLSL

int main(){
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
	player = Player(gridToScreen(10, 13), sf::IntRect(0, 0, 32, 32), texture3);

	// Create the Enemies
	for (int i = 0; i < 10; i++) {
		Enemy e = Enemy(gridToScreen(i + 2, 2), sf::IntRect(0, 0, 32, 32), texture3);
		e.getSprite().setColor(sf::Color(40, 10, 30));
		e.getSprite().setRotation(180.0f);
		e.setMoveDirection(32.0f);
		enemies.push_back(e);
	}
	for (int i = 0; i < 10; i++) {
		Enemy e = Enemy(gridToScreen(i + 2, 4), sf::IntRect(0, 0, 32, 32), texture3);
		e.getSprite().setColor(sf::Color(40, 10, 30));
		e.getSprite().setRotation(180.0f);
		e.setMoveDirection(-32.0f);
		enemies.push_back(e);
	}

	// Create the UI
	//		The score text
	scoreTxt.setFont(font);
	scoreTxt.setString("Score: 000,000,000");
	scoreTxt.setCharacterSize(25); // px, not pt
	scoreTxt.setFillColor(sf::Color::White);
	scoreTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	scoreTxt.setPosition(gridToScreen(13, 0));
	//		The Attribution text
	attributionTxt.setFont(font);
	attributionTxt.setString("Font: Sharp Retro by JROB774 on OpenGameArt.org");
	attributionTxt.setCharacterSize(10); // px, not pt
	attributionTxt.setFillColor(sf::Color::White);
	attributionTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	attributionTxt.setPosition(gridToScreen(1, 14));
	//		The Game Over text
	gameOverTxt.setFont(font);
	gameOverTxt.setString("GAME OVER");
	gameOverTxt.setCharacterSize(100); // px, not pt
	gameOverTxt.setFillColor(sf::Color::White);
	gameOverTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	gameOverTxt.setPosition(window.getSize().x / 6, window.getSize().y / 3);
	//		The Title Text
	titleTxt.setFont(font);
	titleTxt.setString("Commaxian");
	titleTxt.setCharacterSize(100); // px, not pt
	titleTxt.setFillColor(sf::Color::White);
	titleTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	titleTxt.setPosition(window.getSize().x / 6, window.getSize().y / 3);
	//		The Pause Text
	pauseTxt.setFont(font);
	pauseTxt.setString("Quit? Y/N");
	pauseTxt.setCharacterSize(50); // px, not pt
	pauseTxt.setFillColor(sf::Color::White);
	pauseTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	pauseTxt.setPosition(2*window.getSize().x / 6, 2*window.getSize().y / 3);

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

		if (!isGameOver && !isInMenu && !isPaused) {
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
							enemies[j].getSprite().setPosition(gridToScreen(1 + rand() % 18, 2)); // Or provide infinite gameplay
							score++; // Add 1 to the score
							break; // We hit an enemy. No need to continue, so break out of loop
						}
					}
				}
			}

			sf::Time elapsed = enemyClock.getElapsedTime();
			// If the clock hits a predetermined interval, move the enemies
			if (elapsed.asSeconds() >= 0.1f) {
				for (int i = 0; i < enemies.size(); i++) {
					if (enemies[i].getLocation().x >= 630 || enemies[i].getLocation().x <= 10) {
						enemies[i].setMoveDirection(enemies[i].getMoveDirection() * -1);
						enemies[i].move(enemies[i].getMoveDirection(), 32.0f);
					}
					enemies[i].move(enemies[i].getMoveDirection(), 0.0f);

					// Game over if the enemy is in the player's row
					if (enemies[i].getLocation().y >= player.getSprite().getPosition().y)
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


	// Idea: Use pairs of booleans to manage the state of a key
	// Read: Debounce
	// Then use the booleans to perform actions
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		escapePress = true;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		if (escapePress) {
			escapeTapped = true;
			escapePress = false;
		}
		escapeRelease = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		yPress = true;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		if (yPress) {
			yTapped = true;
			yPress = false;
		}
		yRelease = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
		nPress = true;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
		if (nPress) {
			nTapped = true;
			nPress = false;
		}
		nRelease = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		enterPress = true;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		if (enterPress) {
			enterTapped = true;
			enterPress = false;
		}
		enterRelease = true;
	}


	if (!isInMenu && !isGameOver && !isPaused) {
		// If the escape key is pressed, for now just close the window
		if (escapeTapped) {
			isPaused = true;
			escapeTapped = false;
		}

		// Below does not yet need a debounce
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			// Create a new laser at the player position, then fire
			if (!laser.isAlive()) { // But only if a laser does not exist.
				laser.getSprite().setPosition(player.getSprite().getPosition());
				// run it
				laserThread.launch(); // Fire the laser
			}
		}
	}

	if (isGameOver && !isInMenu && !isPaused) {
		if (yTapped) {
			yTapped = false;
			quit(window);
		}
	}

	if (isInMenu && !isPaused && !isGameOver) {
		// We are in the menu, so change key actions to be appropriate
		// If in main menu, Hit escape to quit
		if (escapeTapped) {
			escapeTapped = false;
			quit(window);
		}
		// Hit enter to start the game
		if (enterTapped) {
			enemyClock.restart();
			isInMenu = false;
		}
	}

	// Only if we are paused can we quit using the prompt
	if (isPaused && !isInMenu && !isGameOver) {
		if (yTapped) {
			yTapped = false;
			quit(window);
		}
		if (nTapped) {
			nTapped = false;
			isPaused = false;
		}
	}

}

void quit(sf::RenderWindow& window) {
	laserThread.terminate();
	window.close();
}

sf::Vector2f gridToScreen(int x, int y) {
	// Makes sure our item is within the screen
	if (x < 0) // If we are off-screen to the left, put us on-screen
		x = 0;
	if (x > 19)
		x = 19; // If we are off-screen to the right, put us on-screen

	if (y < 0)
		y = 0; // If we are off-screen to the top, put us on screen
	if (y > 14)
		y = 14; // If we are off-screen to the bottom, put us on screen

	return sf::Vector2f(x * 32, y * 32);
}

// Draw things on the screen
void renderItems(sf::RenderWindow& window) {
	// Render with shaders, if supported
	if (useShaders) {
		if (!isGameOver && !isInMenu) { // If the game isn't over, draw the entities
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
	else {
		if (!isGameOver && !isInMenu) { // If the game isn't over, draw the entities
			window.draw(player.getSprite()); // Draw the player sprite
			for (int i = 0; i < enemies.size(); i++) { // Draw the enemies
				window.draw(enemies[i].getSprite());
			}
			if (laser.isAlive()) {
				window.draw(laser.getSprite());
			}
		}
	}

	if (isInMenu && !isPaused) {
		window.draw(titleTxt);
	}

	if (isPaused && !isInMenu) {
		window.draw(pauseTxt);
	}

	// These will not be rendered using shaders.
	scoreTxt.setString("Score: " + std::string(9 - std::to_string(score).length(), '0') + std::to_string(score)); // Update the score text from the score variable
	window.draw(scoreTxt);
	window.draw(attributionTxt);

	// If the game is over, draw the GameOver text
	if (isGameOver) {
		window.draw(gameOverTxt);
		window.draw(pauseTxt);
	}

}