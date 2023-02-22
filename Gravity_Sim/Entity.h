#pragma once
#include <SFML/Graphics.hpp>

#define DEGTORAD 0.017453
#define CIRCLE_SEGMENTS 100

class Entity
{
public:
	double speed = 0;
	double mass = 0;
	double angle = 0;
	double dx = 0, dy = 0;
	double x = 0, y = 0, r = 0;
	float color[4] = { 0, 0, 0, 0 };

	sf::CircleShape shape;

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	Entity(double _radius, double _speed, double _mass, double _angle, double _x, double _y, float _color[4]);
	~Entity();
};

