#include <Box2D/Box2D.h>
#include <iostream>

#define SFML_STATIC
#include <SFML/Window.hpp>
#include<SFML/Graphics.hpp>

// Representations of direction used to
// scale the direction of forces applied
// to the snake
enum Direction
{
	NONE = 0,
	UP = -1,
	RIGHT = 1,
	DOWN = 1,
	LEFT = -1
};

// Represents the player
class Snake
{
private:
	// Physics object variables
	b2BodyDef bodyDef;
	b2PolygonShape box;
	b2FixtureDef fixtureDef;
	b2Body* body;

	// Movement variables
	float xAcceleration = 200;
	float yAcceleration = 200;

public:
	// Position variables
	b2Vec2 position;
	b2Vec2 prevPosition;

	sf::CircleShape circle;

	// Score variables
	unsigned long xOvershootCount = 0;
	unsigned long yOvershootCount = 0;

	Snake();
	Snake(b2World&);
	~Snake();

	void Update(Direction, Direction);
};

