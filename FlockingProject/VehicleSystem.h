#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "Vehicle.h"

class VehicleSystem
{
public:

	VehicleSystem(sf::RenderWindow* window);
	~VehicleSystem();
	void Update(sf::RenderWindow* window, float deltaTime);
	void AddVehicle(sf::RenderWindow* window);
	void RemoveVehicle();
	void ApplyBehaviors(sf::RenderWindow* window, Vehicle & myVehicle, std::vector<Vehicle*>* otherVehicle);
	void Draw(sf::RenderWindow* window);

	//void WindToggle();
	//void GravityToggle();
	void SeekToggle();
	void SeparateToggle();
	void AlignmentToggle();
	void CohesionToggle();

	int getVehicleCount();
	/*bool separateToggle;
	bool alignmentToggle;
	bool cohesionToggle;*/
private:

	std::vector<Vehicle> VehicleGroup;

	float r = 30.0f;
	void Separate(Vehicle& myVehicle, std::vector<Vehicle*>* otherVehicle);
	void Alignment(Vehicle& myVehicle, std::vector<Vehicle*>* otherVehicle);
	void Cohesion(Vehicle& myVehicle, std::vector<Vehicle*>* otherVehicle);
	void Seek(sf::RenderWindow * window, Vehicle& myVehicle);

	//BUCKET
	static const int COLUMNS = 10;
	static const int ROWS = 10;
	int gridWidth;
	int gridHeight;
	std::vector<Vehicle*> grid[ROWS][COLUMNS];
	sf::Vector2i GetBucket(sf::Vector2f pos);
	void AddBucket(sf::Vector2i bucket, Vehicle* v);
	void RemoveBucket(sf::Vector2i bucket, Vehicle* v);

	bool windToggle = true;
	bool gravityToggle = true;
	bool seekToggle = true;
	bool separateToggle;
	bool alignmentToggle;
	bool cohesionToggle;
};