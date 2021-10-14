#include "Snake.h"

Snake::Snake()
{
}

// Creates a snake in the given world
Snake::Snake(b2World& _world)
{
	// Create the snake
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(50.0f, 50.0f);
	body = _world.CreateBody(&bodyDef);

	box.SetAsBox(1.0f, 1.0f);

	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

	circle = sf::CircleShape(20.0f);
	circle.setFillColor(sf::Color(1, 140, 33));
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(position.x, position.y);
}


Snake::~Snake()
{
	body = nullptr;
}

// Updates the snakes position and applies force in the given directions
void Snake::Update(Direction xDirection, Direction yDirection) {
	prevPosition = position;
	position = body->GetPosition();
	circle.setPosition(position.x, position.y);
	body->ApplyForce((b2Vec2(xAcceleration * xDirection, yAcceleration * yDirection)), position, true);
}
