#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
int bgPos = 0;
int bgMax = 1919;
int score = 0; // The temporary score variable

// Key debounce variables
bool escapePress = false, escapeRelease = true, escapeTapped = false;
bool yPress = false, yRelease = true, yTapped = false;
bool nPress = false, nRelease = true, nTapped = false;
bool enterPress = false, enterRelease = true, enterTapped = false;

// Volume Values
const float SFX_VOL = 25.0f;
const float MUSIC_VOL = 75.0f;
const float MENU_SFX_VOL = 100.0f;

// Textures and Fonts
sf::Texture laserTexture;
sf::Texture texture2;
sf::Texture texture3;
sf::Texture bg; // The 'huge' background texture
sf::Font titleFont;
sf::Font instructionFont;

// Sounds and Music
sf::SoundBuffer shootFx;
sf::Sound shootSound;
sf::SoundBuffer explodeFx;
sf::Sound explodeSound;
sf::SoundBuffer menuSelectFx;
sf::Sound menuSelectSound;
sf::Music backgroundMusic;

// Entities
sf::Sprite background; // The background sprite
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
sf::Text instructionsTxt;
sf::Text pauseTxt;

// Clocks
sf::Clock enemyClock; // Used to determine when an enemy updates or moves
sf::Clock bgUpdateClock; // Used to determine when the background moves
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

	if (!bg.loadFromFile("res/textures/bg.png"))
		std::cout << "Error loading Background texture" << std::endl;
	bgMax = bg.getSize().y;

	// Load sounds Effects
	if (!shootFx.loadFromFile("res/sfx/shoot.wav"))
		std::cout << "Error loading Shoot SFX" << std::endl;
	shootSound.setBuffer(shootFx);
	shootSound.setVolume(SFX_VOL);

	if (!menuSelectFx.loadFromFile("res/sfx/select.wav"))
		std::cout << "Error loading Select SFX" << std::endl;
	menuSelectSound.setBuffer(menuSelectFx);
	menuSelectSound.setVolume(MENU_SFX_VOL);

	if (!explodeFx.loadFromFile("res/sfx/explode.wav"))
		std::cout << "Error loading Explode SFX" << std::endl;
	explodeSound.setBuffer(explodeFx);
	explodeSound.setVolume(SFX_VOL);

	
	

	// Load Background Music
	if (!backgroundMusic.openFromFile("res/music/a.ogg"))
		return -1; // error

	// Load fonts
	//if (!font.loadFromFile("fonts/arial.ttf"))
	//	std::cout << "Error loading font arial.ttf" << std::endl;
	if (!titleFont.loadFromFile("res/fonts/SharpRetro.ttf"))
		std::cout << "Error loading font SharpRetro.ttf" << std::endl;

	if (!instructionFont.loadFromFile("res/fonts/OpenSans-BoldItalic.ttf"))
		std::cout << "Error loading font OpenSans-BoldItalic.ttf" << std::endl;

	// Create the background
	background.setPosition(0, 0);
	background.setTexture(bg);
	background.setTextureRect(sf::IntRect(0, bgPos, 640, 480)); // start x, start y, width, height

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
	scoreTxt.setFont(titleFont);
	scoreTxt.setString("Score: 000,000,000");
	scoreTxt.setCharacterSize(25); // px, not pt
	scoreTxt.setFillColor(sf::Color::White);
	scoreTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	scoreTxt.setPosition(gridToScreen(13, 0));
	//		The Attribution text
	attributionTxt.setFont(titleFont);
	attributionTxt.setString("Font: Sharp Retro by JROB774 on OpenGameArt.org");
	attributionTxt.setCharacterSize(10); // px, not pt
	attributionTxt.setFillColor(sf::Color::White);
	attributionTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	attributionTxt.setPosition(gridToScreen(1, 14));
	//		The Game Over text
	gameOverTxt.setFont(titleFont);
	gameOverTxt.setString("GAME OVER");
	gameOverTxt.setCharacterSize(100); // px, not pt
	gameOverTxt.setFillColor(sf::Color::White);
	gameOverTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	gameOverTxt.setPosition(window.getSize().x / 6, window.getSize().y / 3);
	//		The Title Text
	titleTxt.setFont(titleFont);
	titleTxt.setString("Commaxian");
	titleTxt.setCharacterSize(100); // px, not pt
	titleTxt.setFillColor(sf::Color::White);
	titleTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	titleTxt.setPosition(window.getSize().x / 6, window.getSize().y / 3);
	//      The Instructions Text
	instructionsTxt.setFont(instructionFont);
	instructionsTxt.setString("Don't let the enemies reach you.\nLeft and Right Arrows to move.\nSpace to shoot.\nEscape to pause.\nPress Enter to Play!");
	instructionsTxt.setCharacterSize(25); // px, not pt
	instructionsTxt.setFillColor(sf::Color::White);
	instructionsTxt.setStyle(sf::Text::Italic | sf::Text::Underlined);
	instructionsTxt.setPosition(window.getSize().x / 6, 7 * window.getSize().y / 12);
	//		The Pause Text
	pauseTxt.setFont(instructionFont);
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

	bgUpdateClock.restart(); // Restart the background update clock
	enemyClock.restart(); // Restart the clock
	backgroundMusic.setLoop(true); // Loops the playback of the bgm
	backgroundMusic.setVolume(MUSIC_VOL); // Sets the bgm volume
	backgroundMusic.play(); // Starts playing the bgm
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

		// Update the background
		if (bgUpdateClock.getElapsedTime().asMilliseconds() > 50) {
			bgUpdateClock.restart();
			if (bgPos < bgMax) {
				bgPos++;
			}
			else {
				bgPos = 0;
			}

			background.setTextureRect(sf::IntRect(0, bgPos, 640, 480)); // start x, start y, width, height
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
							explodeSound.play(); // Play explosion sound
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


	/*-----------------Key Debounce-------------------*/
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

	/*-----------------Key Actions-------------------*/

	if (!isInMenu && !isGameOver && !isPaused) {
		if (escapeTapped) {
			menuSelectSound.play(); // Play Menu Select sound
			isPaused = true;
			escapeTapped = false;
		}

		// Below does not yet need a debounce
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			// Create a new laser at the player position, then fire
			if (!laser.isAlive()) { // But only if a laser does not exist.
				laser.getSprite().setPosition(player.getSprite().getPosition());
				shootSound.play(); // Play Laser Shoot sound
				// run it
				laserThread.launch(); // Fire the laser
			}
		}
	}

	if (isGameOver && !isInMenu && !isPaused) {
		if (yTapped) {
			yTapped = false;
			menuSelectSound.play(); // Play Menu Select sound
			quit(window);
		}
	}

	if (isInMenu && !isPaused && !isGameOver) {
		// We are in the menu, so change key actions to be appropriate
		// If in main menu, Hit escape to quit
		if (escapeTapped) {
			escapeTapped = false;
			menuSelectSound.play(); // Play Menu Select sound
			quit(window);
		}
		// Hit enter to start the game
		if (enterTapped) {
			enemyClock.restart();
			menuSelectSound.play(); // Play Menu Select sound
			isInMenu = false;
		}
	}

	// Only if we are paused can we quit using the prompt
	if (isPaused && !isInMenu && !isGameOver) {
		if (yTapped) {
			yTapped = false;
			menuSelectSound.play(); // Play Menu Select sound
			quit(window);
		}
		if (nTapped) {
			nTapped = false;
			isPaused = false;
			menuSelectSound.play(); // Play Menu Select sound
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
	// Draw the background first
	window.draw(background);
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
		window.draw(instructionsTxt);
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