#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
using namespace sf;

const float ratio = 1.4055; // to compatibility with 1366 x 768 resolution
const int NUM_BRANCHES = 6;

void updateBranches(int seed);
Sprite branches[NUM_BRANCHES];
// Where is the player/branch?
// Left ot Right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];


int main()
{
	// Create and open a window for the game
	RenderWindow window(VideoMode::getDesktopMode(), "Timber!!");
	View view(sf::FloatRect(0, 0, 1366, 768));
	window.setView(view);

	// Create a texture to hold a graphinc on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("/Templates/c++/timber/graphics/background.png");

	// Create a sprite

	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("/Templates/c++/timber/graphics/tree.png");
	Sprite spriteTree(textureTree);
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810/ratio, 0);

	// Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("/Templates/c++/timber/graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 500/ratio);
	// Is the bee currently moving?
	bool beeActive = false;
	// How fast the bee fly
	float beeSpeed = 0.0f;

	// make 3 cloud sprites from 1 texture
	Texture textueCloud;
	// Load 1 new texture
	Texture textureCloud;
	textureCloud.loadFromFile("/Templates/c++/timber/graphics/cloud.png");
	// 3 new sprites with the same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// Position the clouds to the screen
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 150/ratio);
	spriteCloud3.setPosition(0, 300/ratio);
	// Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// How gast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	// Variables to control time itself
	Clock clock;
	
	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400/ratio;
	float timeBarHeight = 80/ratio;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1366 / 2) - (timeBarStartWidth / 2), 768 - timeBarHeight);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running
	bool paused = true;
		// Draw some text
	int score = 0;
	Text messageText;
	Text scoreText;

	// We need to choose a font
	Font font;
	font.loadFromFile("/Templates/c++/timber/fonts/KOMIKAP_.ttf");
	
	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	// // Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, 
	textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1366 / 2.0f, 768 / 2.0f);
	scoreText.setPosition(20, 20);

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("/Templates/c++/timber/graphics/branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000/ratio, -2000/ratio);
		// Set the sprite's origin to dead center
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}
	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);
	while(window.isOpen()) {
		/*
		***********************************************
		Handle the players input
		**********************************************
		*/

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		// Start the game
		if(Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;
			
			// Reset the time and the score
			score = 0;
			timeRemaining = 5;
		}
		/*
		**************************************
		Update the scene
		**************************************
		*/
		if (!paused) {
			// Measure time
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining,
				timeBarHeight));

			if (timeRemaining <= 0.0f) {
				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");
				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, 
						textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1366 / 2.0f, 768 / 2.0f);
			}
			// Setup the bee
			if (!beeActive) {
				// How fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;
				
				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000/ratio, height/ratio);
				beeActive = true;
			}
			else // Move the bee 
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x -(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// Has the bee reached the right hand edge of the screen?
				if(spriteBee.getPosition().x < -100) {
					beeActive = false;
				}
			}
			// Manage the clouds
			// Cloud 1
			if (!cloud1Active) {
				// How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200/ratio, height/ratio);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if(spriteCloud1.getPosition().x > (1920/ratio)) {
					cloud1Active = false;
				}
			}	
			// Cloud 2
			if (!cloud2Active) {
				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 150);
				spriteCloud2.setPosition(-200/ratio, height/ratio);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if(spriteCloud2.getPosition().x > (1920/ratio)) {
					cloud2Active = false;
				}
			}	
			// Cloud 3
			if (!cloud3Active) {
				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200/ratio);

				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 150/ratio);
				spriteCloud3.setPosition(-200/ratio, height/ratio);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if(spriteCloud3.getPosition().x > (1920/ratio)) {
					cloud3Active = false;
				}
			}
			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str()); 
			// Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150 / ratio;
				if (branchPositions[i] == side::LEFT) {
					// Move the sprite to the left side
					branches[i].setPosition(610/ratio, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					// Move the sprite to the right side
					branches[i].setPosition(1330/ratio, height);
					// Flip the sprite round the other way
					branches[i].setRotation(0);
				}
				else {
					// Hide the branch
					branches[i].setPosition(3000/ratio, height);
				}
			}
		}
		/*
		**************************************
		Draw the scene
		**************************************
		*/
		// Clear everything from the last frame
		window.clear();
		// Draw our game scene here
		window.draw(spriteBackground);
		
		// Draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// Draw the tree
		window.draw(spriteTree);
		
		// Draw the insect
		window.draw(spriteBee);

		// Draw the score
		window.draw(scoreText);
		
		// Draw the timebar
		window.draw(timeBar);

		if (paused) {
			window.draw(messageText);
		}


		// Show everything we just drew
		window.display();
	}


    return 0;
}

void updateBranches(int seed) {
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
		branchPositions[j] = branchPositions[j - 1];
	// Spawn a new branch at position 0
	// LEFT, RIGHT ot NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
		case 0:
			branchPositions[0] = side::LEFT;
			break;
		case 1:
			branchPositions[0] = side::RIGHT;
			break;
		default:
			branchPositions[0] = side::NONE;
			break;
	}
}
