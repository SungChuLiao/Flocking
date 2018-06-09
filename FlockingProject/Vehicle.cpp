#include "Vehicle.h"
#include <cmath>

Vehicle::Vehicle(sf::RenderWindow* window)
{
	acceleration = sf::Vector2f(0.0f, 0.0f);
	velocity = sf::Vector2f(1.0f, 0);
	location = (sf::Vector2f)sf::Mouse::getPosition(*window);
	r = 5.0;
	maxSpeed = 300;
	maxforce = 1.0f;

	//Set shape
	//myVehicleShape.setFillColor(color);

	/*myVehicleShape.setPointCount(3);
	myVehicleShape.setPoint(0, location + sf::Vector2f(0, -r * 2));
	myVehicleShape.setPoint(1, location + sf::Vector2f(-r, r * 2));
	myVehicleShape.setPoint(2, location + sf::Vector2f(r, r * 2));*/

	myVehicleShape.setRadius(6);
	myVehicleShape.setPointCount(3);
	myVehicleShape.setScale(1, 2);
	myVehicleShape.setPosition(location);
}
Vehicle::~Vehicle()
{

}

void Vehicle::Update(sf::RenderWindow* window, float deltaTime)
{
	velocity += acceleration * deltaTime;
	location += velocity * deltaTime;
	acceleration *= 0.0f;

	float theta = atan2f(velocity.x, -velocity.y) * 180 / PI;
	myVehicleShape.setPosition(location);
	myVehicleShape.setRotation(theta);
	//location == (velocity);

	if (myVehicleShape.getPosition().x > window->getSize().x - 5)
		myVehicleShape.setPosition(5, myVehicleShape.getPosition().y);
	if (myVehicleShape.getPosition().x < 5)
		myVehicleShape.setPosition(window->getSize().x - 5, myVehicleShape.getPosition().y);
	if (myVehicleShape.getPosition().y > window->getSize().y - 5)
		myVehicleShape.setPosition(myVehicleShape.getPosition().x, 5);
	if (myVehicleShape.getPosition().y < 5)
		myVehicleShape.setPosition(myVehicleShape.getPosition().y, window->getSize().y - 5);
}
void Vehicle::applyForce(sf::Vector2f force)
{
	acceleration += force;
}
void Vehicle::seek(sf::Vector2f target)
{
	location = myVehicleShape.getPosition();

	sf::Vector2f desired = sf::Vector2f(0, 0);

	sf::Vector2f newDesired = target - location;
	float length = sqrtf(powf(newDesired.x, 2) + powf(newDesired.y, 2));
	//desired = desired / length;
	if (length > 50)
	{
		desired = newDesired / length * maxSpeed;
		sf::Vector2f steer = desired - velocity;
		/*if (steer.x > maxforce)
		{
		steer.x = maxforce;
		}
		if (steer.y > maxforce)
		{
		steer.y = maxforce;
		}*/
		applyForce(steer);
	}
	else
	{
		//float m = maxSpeed - (maxSpeed * (length / 100));
		float m = (maxSpeed * (length / 100));
		desired * m;
	}
}
