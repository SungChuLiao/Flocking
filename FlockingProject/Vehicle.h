#pragma once
#include <SFML/Graphics.hpp>
class Vehicle
{
public:
	Vehicle(sf::RenderWindow* window);
	~Vehicle();
	void Update(sf::RenderWindow* window, float deltaTime);
	void applyForce(sf::Vector2f force);
	void seek(sf::Vector2f target);

	//sf::ConvexShape myVehicleShape;
	sf::CircleShape myVehicleShape;
	sf::Vector2f location;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float maxSpeed;

private:
	const float PI = 3.14159265359f;
	float maxforce;
	float r;

};