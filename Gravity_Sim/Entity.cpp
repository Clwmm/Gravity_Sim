#include "Entity.h"

Entity::Entity(float _radius, float _speed, float _mass, float _angle, float _x, float _y, float _color[3])
	:r{ _radius }, speed{ _speed }, mass{_mass}, angle{ _angle }, x{ _x }, y{ _y }
{
	color[0] = _color[0];
	color[1] = _color[1];
	color[2] = _color[2];
	this->shape.setFillColor(sf::Color
	(
		(int)(color[0] * 255),
		(int)(color[1] * 255),
		(int)(color[2] * 255)

	));
	this->shape.setRadius(_radius);
	this->shape.setOrigin(_radius, _radius);
	this->shape.setPointCount(CIRCLE_SEGMENTS);

	this->dx = cos(this->angle*(-DEGTORAD)) * speed;
	this->dy = sin(this->angle*(-DEGTORAD)) * speed;

}

void Entity::update(float deltaTime)
{
	this->x += deltaTime * this->dx;
	this->y += deltaTime * this->dy;
}

void Entity::draw(sf::RenderWindow& window)
{
	this->shape.setRadius(this->r);
	this->shape.setPosition(sf::Vector2f(this->x, this->y));
	window.draw(this->shape);
}

Entity::~Entity()
{
}
