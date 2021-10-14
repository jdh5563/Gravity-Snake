#include <Box2D/Box2D.h>

#define SFML_STATIC
#include <SFML/Window.hpp>
#include<SFML/Graphics.hpp>

// Represents a target being chased by the snake
class Target
{
private:
	// Create physics object variables
	b2BodyDef bodyDef;
	b2PolygonShape box;
	b2FixtureDef fixtureDef;
	b2Body* body;

	short randomRange(short, short);

public:
	short hitCount = 0;
	bool isHit = false;
	b2Vec2 position;

	sf::CircleShape circle;

	Target();
	Target(b2World&, sf::RenderWindow&);
	~Target();
	void Update();
};

