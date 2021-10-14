#include "Target.h"
#include "Snake.h"

Target::Target()
{
}

// Builds a target
Target::Target(b2World& world, sf::RenderWindow& window)
{
	// Create the target
	bodyDef.position.Set(randomRange(50, window.getSize().x - 50), randomRange(50, window.getSize().y - 50));
	body = world.CreateBody(&bodyDef);

	box.SetAsBox(0.5f, 0.5f);
	body->CreateFixture(&box, 0.0f);

	position = body->GetPosition();

	circle = sf::CircleShape(10.0f);
	circle.setFillColor(sf::Color(255, 0, 0));
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(position.x, position.y);
}

Target::~Target()
{
	body = nullptr;
}

// If the target has been hit, moves it to a random location in the world
void Target::Update() {
	if (isHit) {
		body->SetTransform(b2Vec2(randomRange(50, 750), randomRange(50, 550)), body->GetAngle());
		position = body->GetPosition();
		circle.setPosition(position.x, position.y);
		isHit = false;
		hitCount++;
	}
}

// Generates a random number in the
// inclusive range (low, high)
short Target::randomRange(short low, short high) {
	return (rand() % (high - low + 1)) + low;
}
