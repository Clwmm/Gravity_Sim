#pragma once
#include <SFML/Graphics.hpp>

#define DEGTORAD 0.017453
#define CIRCLE_SEGMENTS 100

class Entity
{
public:
	float speed = 0;
	float angle = 0;
	float dx = 0, dy = 0;
	float x = 0, y = 0, r = 0;

	sf::CircleShape shape;

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	Entity(float _radius, float _speed, float _angle, float _x, float _y, float color[3]);
	~Entity();
};

