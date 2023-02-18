#include "Entity.h"

Entity::Entity(float _radius, float _speed, float _angle, float _x, float _y, float color[3])
	:r{_radius}, speed{speed}, angle{_angle}, x{_x}, y{_y}
{
	this->shape.setFillColor(sf::Color
	(
		(int)(color[0] * 255),
		(int)(color[0] * 255),
		(int)(color[0] * 255)

	));
	this->shape.setRadius(_radius);
	this->shape.setOrigin(_radius, _radius);
	this->shape.setPointCount(CIRCLE_SEGMENTS);
}

void Entity::update(float deltaTime)
{
	this->shape.setPosition(sf::Vector2f(this->x, this->y));
}

void Entity::draw(sf::RenderWindow& window)
{
	window.draw(this->shape);
}

Entity::~Entity()
{
}
