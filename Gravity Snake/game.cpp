#include <time.h>

#include "game.h"
#include "Snake.h"
#include "Target.h"

// Create world
b2Vec2 gravity(0.0f, 9.8f);
b2World* world;

// Create borders
b2BodyDef groundBodyDef;
b2Body* groundBody;
b2PolygonShape groundBox;
sf::RectangleShape groundRect;

b2BodyDef leftBodyDef;
b2Body* leftBody;
b2PolygonShape leftBox;
sf::RectangleShape leftRect;

b2BodyDef rightBodyDef;
b2Body* rightBody;
b2PolygonShape rightBox;
sf::RectangleShape rightRect;

b2BodyDef topBodyDef;
b2Body* topBody;
b2PolygonShape topBox;
sf::RectangleShape topRect;

// Create snake
Snake snake;
Direction xDirection;
Direction yDirection;

// Create target
Target target;

// Create timing variables
sf::Clock deltaClock;
sf::Time deltaTime;

// Create UI elements
sf::Texture* wTexture;
sf::Texture* aTexture;
sf::Texture* sTexture;
sf::Texture* dTexture;
sf::Texture* wClickedTexture;
sf::Texture* aClickedTexture;
sf::Texture* sClickedTexture;
sf::Texture* dClickedTexture;
sf::Texture* closeTexture;
sf::Texture* backgroundTexture;
sf::Texture* playTexture;
sf::Texture* againTexture;
sf::Texture* menuTexture;

sf::Sprite wSprite;
sf::Sprite aSprite;
sf::Sprite sSprite;
sf::Sprite dSprite;
sf::Sprite closeSprite;
sf::Sprite backgroundSprite;
sf::Sprite playSprite;
sf::Sprite againSprite;
sf::Sprite menuSprite;

sf::Font* font;
sf::Text menuText;
sf::Text controlsText;
sf::Text closeText;
sf::Text endText;

// Create game state variable
GameMode state = MAIN_MENU;

// Runs the game until the player hits 2 targets or presses Escape
void update(sf::RenderWindow& window)
{
	// Main game loop
	while (window.isOpen()) {
		// Only check for input in the main menu
		if (state == MAIN_MENU) {
			checkInput(window);
		}
		// Actually update the world when the game is going
		else if (state == GAME) {
			// Update world, get user input, update entities
			timeStep();

			checkInput(window);

			snake.Update(xDirection, yDirection);

			checkCollisions();

			checkForOvershoot();

			target.Update();
		}
		// Only check for input in the end screen and update the text if the player won
		else {
			checkInput(window);

			// Displays stats if the player won
			if (target.hitCount >= 2) {
				std::string victory = "Congratulations! You won!\nYou overshot the target " + std::to_string(snake.xOvershootCount);
				victory += " times on the x - axis\n and " + std::to_string(snake.yOvershootCount);
				victory += " times on the y-axis.";
				if (snake.xOvershootCount != 0 || snake.yOvershootCount != 0) {
					victory += "\nShoot for 0 overshoots next time!";
				}

				endText = sf::Text(victory, *font, 24);
				endText.setStyle(sf::Text::Bold);
				endText.setPosition(window.getSize().x / 2.0f - endText.getGlobalBounds().width / 2.0f, 70);
			}
		}

		// Display results, check game end condition
		display(window);

		checkForClose(window);
	}

	// Delete the world when the game ends
	deleteWorld();
}

// Displays all sprites, shapes, and text
void display(sf::RenderWindow& window)
{
	// Reset the screen
	window.clear(Color::Black);

	// Draw different things depending on the game state
	if (state == MAIN_MENU) {
		window.draw(backgroundSprite);
		window.draw(menuText);
		window.draw(playSprite);
	}
	else if (state == GAME) {
		window.draw(groundRect);
		window.draw(leftRect);
		window.draw(rightRect);
		window.draw(topRect);
		window.draw(wSprite);
		window.draw(aSprite);
		window.draw(sSprite);
		window.draw(dSprite);
		window.draw(closeSprite);
		window.draw(controlsText);
		window.draw(closeText);
		window.draw(snake.circle);
		window.draw(target.circle);
	}
	else {
		window.draw(endText);
		window.draw(againSprite);
		window.draw(menuSprite);
	}

	// Display all drawn entities
	window.display();
}

// Steps 'deltaTime' seconds forward in the world
void timeStep() {
	deltaTime = deltaClock.getElapsedTime();
	deltaClock.restart();
	(*world).Step(deltaTime.asSeconds(), 6, 2);
}

// Reads user input to set the direction the snake will move in or click buttons
void checkInput(sf::RenderWindow& window) {
	xDirection = NONE;
	yDirection = NONE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		yDirection = UP;
		wSprite.setTexture(*wClickedTexture);
	}
	else {
		wSprite.setTexture(*wTexture);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		yDirection = DOWN;
		sSprite.setTexture(*sClickedTexture);
	}
	else {
		sSprite.setTexture(*sTexture);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		xDirection = RIGHT;
		dSprite.setTexture(*dClickedTexture);
	}
	else {
		dSprite.setTexture(*dTexture);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		xDirection = LEFT;
		aSprite.setTexture(*aClickedTexture);
	}
	else {
		aSprite.setTexture(*aTexture);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// If the mouse is within the bounds of the button when clicked, do the corresponding task
		if (state == MAIN_MENU) {
			if (sf::Mouse::getPosition(window).x >= playSprite.getPosition().x &&
				sf::Mouse::getPosition(window).x <= playSprite.getPosition().x + playSprite.getGlobalBounds().width
				&&
				sf::Mouse::getPosition(window).y >= playSprite.getPosition().y &&
				sf::Mouse::getPosition(window).y <= playSprite.getPosition().y + playSprite.getGlobalBounds().height) {
				state = GAME;
			}
		}
		else if (state == GAME) {
			if (sf::Mouse::getPosition(window).x >= closeSprite.getPosition().x &&
				sf::Mouse::getPosition(window).x <= closeSprite.getPosition().x + closeSprite.getGlobalBounds().width
				&&
				sf::Mouse::getPosition(window).y >= closeSprite.getPosition().y &&
				sf::Mouse::getPosition(window).y <= closeSprite.getPosition().y + closeSprite.getGlobalBounds().height) {
				state = END_MENU;
			}
		}
		else {
			if (sf::Mouse::getPosition(window).x >= againSprite.getPosition().x &&
				sf::Mouse::getPosition(window).x <= againSprite.getPosition().x + againSprite.getGlobalBounds().width
				&&
				sf::Mouse::getPosition(window).y >= againSprite.getPosition().y &&
				sf::Mouse::getPosition(window).y <= againSprite.getPosition().y + againSprite.getGlobalBounds().height) {
				state = GAME;

				// Reset the world when playing again
				deleteWorld();
				resetSpriteScales();
				createWorld(window);
			}
			else if (sf::Mouse::getPosition(window).x >= menuSprite.getPosition().x &&
				sf::Mouse::getPosition(window).x <= menuSprite.getPosition().x + menuSprite.getGlobalBounds().width
				&&
				sf::Mouse::getPosition(window).y >= menuSprite.getPosition().y &&
				sf::Mouse::getPosition(window).y <= menuSprite.getPosition().y + menuSprite.getGlobalBounds().height) {
				state = MAIN_MENU;

				// Reset the world when playing again
				deleteWorld();
				resetSpriteScales();
				createWorld(window);
			}
		}
	}
}

// Checks for window events like closing the window or winning
void checkForClose(sf::RenderWindow& windowToCheck) {
	if (state != END_MENU && (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || target.hitCount >= 2)) {
		state = END_MENU;
	}
	else {
		sf::Event event;
		while (windowToCheck.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				windowToCheck.close();
		}
	}
}

// Checks if the snake has collided with the target
void checkCollisions() {
	if (b2DistanceSquared(snake.position, target.position) <= pow((snake.circle.getRadius() + target.circle.getRadius()), 2)) {
		target.isHit = true;
	}
}

// Checks if the snake has overshot the target on the x- or y-axes
void checkForOvershoot() {
	if ((snake.prevPosition.x < target.position.x && snake.position.x > target.position.x) ||
		(snake.prevPosition.x > target.position.x && snake.position.x < target.position.x)) {
		snake.xOvershootCount++;
	}
	else if ((snake.prevPosition.y < target.position.y && snake.position.y > target.position.y) ||
		(snake.prevPosition.y > target.position.y && snake.position.y < target.position.y)) {
		snake.yOvershootCount++;
	}
}

// Creates the world that the game is played in
void createWorld(sf::RenderWindow& window) {
	// Prepare random number generation
	srand(time(NULL));

	// Create the world
	world = new b2World(gravity);

	// Create snake and target
	snake = Snake((*world));
	target = Target((*world), window);

	// Create the borders
	groundBodyDef.position.Set(window.getSize().x / 2.0f, window.getSize().y - (snake.circle.getRadius() / 2.0f));
	groundBody = (*world).CreateBody(&groundBodyDef);

	groundBox.SetAsBox(window.getSize().x / 2.0f, 30.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	groundRect = sf::RectangleShape(sf::Vector2f(window.getSize().x, 30.0f));
	groundRect.setPosition(0, window.getSize().y - snake.circle.getRadius());



	leftBodyDef.position.Set(snake.circle.getRadius() / 2.0f, window.getSize().y / 2.0f);
	leftBody = (*world).CreateBody(&leftBodyDef);

	leftBox.SetAsBox(30.0f, window.getSize().y / 2.0f);
	leftBody->CreateFixture(&leftBox, 0.0f);

	leftRect = sf::RectangleShape(sf::Vector2f(30.0f, window.getSize().y));
	leftRect.setPosition(-10.0f, 0);



	rightBodyDef.position.Set(window.getSize().x - (snake.circle.getRadius() / 2.0f), window.getSize().y / 2.0f);
	rightBody = (*world).CreateBody(&rightBodyDef);

	rightBox.SetAsBox(30.0f, window.getSize().y / 2.0f);
	rightBody->CreateFixture(&rightBox, 0.0f);

	rightRect = sf::RectangleShape(sf::Vector2f(30.0f, window.getSize().y));
	rightRect.setPosition(window.getSize().x - snake.circle.getRadius(), 0);



	topBodyDef.position.Set(window.getSize().x / 2.0f, snake.circle.getRadius() / 2.0f);
	topBody = (*world).CreateBody(&topBodyDef);

	topBox.SetAsBox(window.getSize().x, 30.0f);
	topBody->CreateFixture(&topBox, 0.0f);

	topRect = sf::RectangleShape(sf::Vector2f(window.getSize().x, 30.0f));
	topRect.setPosition(0, -10.0f);


	// Create textures, sprites, and fonts
	wTexture = new sf::Texture();
	(*wTexture).loadFromFile("textures/letter_W.png");

	wClickedTexture = new sf::Texture();
	(*wClickedTexture).loadFromFile("textures/letter_W_Clicked.png");

	wSprite.setTexture(*wTexture);
	wSprite.setPosition(65, 60);
	wSprite.setScale(wSprite.getScale() / 7.0f);


	aTexture = new sf::Texture();
	(*aTexture).loadFromFile("textures/letter_A.png");

	aClickedTexture = new sf::Texture();
	(*aClickedTexture).loadFromFile("textures/letter_A_Clicked.png");

	aSprite.setTexture(*aTexture);
	aSprite.setPosition(30, 95);
	aSprite.setScale(aSprite.getScale() / 7.0f);


	sTexture = new sf::Texture();
	(*sTexture).loadFromFile("textures/letter_S.png");

	sClickedTexture = new sf::Texture();
	(*sClickedTexture).loadFromFile("textures/letter_S_Clicked.png");

	sSprite.setTexture(*sTexture);
	sSprite.setPosition(65, 95);
	sSprite.setScale(sSprite.getScale() / 7.0f);


	dTexture = new sf::Texture();
	(*dTexture).loadFromFile("textures/letter_D.png");

	dClickedTexture = new sf::Texture();
	(*dClickedTexture).loadFromFile("textures/letter_D_Clicked.png");

	dSprite.setTexture(*dTexture);
	dSprite.setPosition(100, 95);
	dSprite.setScale(dSprite.getScale() / 7.0f);


	closeTexture = new sf::Texture();
	(*closeTexture).loadFromFile("textures/close.png");

	closeSprite.setTexture(*closeTexture);
	closeSprite.setPosition(740, 60);
	closeSprite.setScale(closeSprite.getScale() / 1.5f);

	backgroundTexture = new sf::Texture();
	(*backgroundTexture).loadFromFile("textures/background.png");

	backgroundSprite.setTexture(*backgroundTexture);

	font = new sf::Font();
	(*font).loadFromFile("fonts/times.ttf");

	menuText = sf::Text("Welcome to Gravity Snake!\n"
		"Use WASD to push the snake up,\n"
		"left, down, and right respectively.\n"
		"Your goal is to hit 2 targets.", *font, 24);
	menuText.setPosition(20, 70);
	menuText.setStyle(sf::Text::Bold);

	playTexture = new sf::Texture();
	(*playTexture).loadFromFile("textures/play_Text.png");

	playSprite.setTexture(*playTexture);
	playSprite.setPosition(100, 250);
	playSprite.setScale(playSprite.getScale() / 2.0f);

	controlsText = sf::Text("Controls", *font, 24);
	controlsText.setPosition(40, 20);
	controlsText.setStyle(sf::Text::Bold);

	closeText = sf::Text("Click to end game", *font, 24);
	closeText.setPosition(575, 20);
	closeText.setStyle(sf::Text::Bold);

	endText = sf::Text("Oh no! You didn't win!\nI'm sure you'll get em next time!", *font, 24);
	endText.setStyle(sf::Text::Bold);
	endText.setPosition(window.getSize().x / 2.0f - endText.getGlobalBounds().width / 2.0f, 70);

	againTexture = new sf::Texture();
	(*againTexture).loadFromFile("textures/again_Text.png");

	againSprite.setTexture(*againTexture);
	againSprite.setScale(againSprite.getScale() / 2.0f);
	againSprite.setPosition(window.getSize().x / 2.0f - (50 + againSprite.getGlobalBounds().width), 450);

	menuTexture = new sf::Texture();
	(*menuTexture).loadFromFile("textures/menu_Text.png");

	menuSprite.setTexture(*menuTexture);
	menuSprite.setPosition(window.getSize().x / 2.0f + 50, 450);
	menuSprite.setScale(menuSprite.getScale() / 2.0f);
}

// Deletes all necessary things to prevent memory leaks
void deleteWorld() {
	delete world;
	delete font;
	delete wTexture;
	delete aTexture;
	delete sTexture;
	delete dTexture;
	delete wClickedTexture;
	delete aClickedTexture;
	delete sClickedTexture;
	delete dClickedTexture;
	delete closeTexture;
	delete backgroundTexture;
	delete playTexture;
	delete againTexture;
	delete menuTexture;
}

// Resets the scale of all sprites when resetting the world
void resetSpriteScales() {
	wSprite.setScale(1, 1);
	aSprite.setScale(1, 1);
	sSprite.setScale(1, 1);
	dSprite.setScale(1, 1);
	closeSprite.setScale(1, 1);
	backgroundSprite.setScale(1, 1);
	playSprite.setScale(1, 1);
	againSprite.setScale(1, 1);
	menuSprite.setScale(1, 1);
}