#include <Box2D/Box2D.h>

#define SFML_STATIC
#include <SFML/Window.hpp>
#include<SFML/Graphics.hpp>

using namespace sf;

// The different states the game can be in
enum GameMode {
	MAIN_MENU = 0,
	GAME = 1,
	END_MENU = 2
};

void update(RenderWindow&);
void display(RenderWindow&);
void timeStep();
void checkInput(RenderWindow&);
void checkForOvershoot();
void createWorld(RenderWindow&);
void checkForClose(RenderWindow&);
void checkCollisions();
void deleteWorld();
void resetSpriteScales();