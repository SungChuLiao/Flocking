#include "VehicleSystem.h"
#include <iostream>

VehicleSystem::VehicleSystem(sf::RenderWindow* window)
{
	//Bucket Grid size
	gridWidth = window->getSize().x / COLUMNS;
	gridHeight = window->getSize().y / ROWS;
	separateToggle = false;
	alignmentToggle = false;
	cohesionToggle = false;
}

VehicleSystem::~VehicleSystem()
{
	VehicleGroup.clear();
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			grid[i][j].clear();
		}
	}
}
void VehicleSystem::Update(sf::RenderWindow* window, float deltaTime)
{
	//Foreach vehicle in group
	for (auto &myVehicle : VehicleGroup)
	{
		sf::Vector2i currentBucket = GetBucket(myVehicle.location);
		myVehicle.Update(window, deltaTime);
		sf::Vector2i nextBucket = GetBucket(myVehicle.location);

		//Move to next Bucket
		if (currentBucket != nextBucket)
		{
			RemoveBucket(currentBucket, &myVehicle);
			AddBucket(currentBucket, &myVehicle);
		}
		//If not null
		if (&grid[nextBucket.x][nextBucket.y] != nullptr)
		{
			ApplyBehaviors(window, myVehicle, &grid[nextBucket.x][nextBucket.y]);
		}
	}

}
void VehicleSystem::ApplyBehaviors(sf::RenderWindow * window, Vehicle & myVehicle, std::vector<Vehicle*>* const otherVehicle)
{
	if (windToggle)
	{
		sf::Vector2f wind = sf::Vector2f(1, 0);
		myVehicle.applyForce(wind);
	}
	if (gravityToggle)
	{
		sf::Vector2f gravity = sf::Vector2f(0, 0.1);
		myVehicle.applyForce(gravity);
	}

	if (seekToggle)
		Seek(window, myVehicle);
	if (separateToggle)
		Separate(myVehicle, otherVehicle);
	if (alignmentToggle)
		Alignment(myVehicle, otherVehicle);
	if (cohesionToggle)
		Cohesion(myVehicle, otherVehicle);
}
void VehicleSystem::Draw(sf::RenderWindow* window)
{
	for (Vehicle myVehicle : VehicleGroup)
	{
		window->draw(myVehicle.myVehicleShape);
	}
}
void VehicleSystem::AddVehicle(sf::RenderWindow* window)
{
	VehicleGroup.push_back(Vehicle(window));
}

void VehicleSystem::RemoveVehicle()
{
	if (VehicleGroup.size() >= 1)
	{
		VehicleGroup.pop_back();
	}
}

int VehicleSystem::getVehicleCount()
{
	return (int)VehicleGroup.size();
}

void VehicleSystem::Seek(sf::RenderWindow * window, Vehicle& myVehicle)
{
	sf::Vector2f target = (sf::Vector2f)sf::Mouse::getPosition(*window);
	myVehicle.location = myVehicle.myVehicleShape.getPosition();

	sf::Vector2f desired = target - myVehicle.location;
	float length = sqrtf(powf(desired.x, 2) + powf(desired.y, 2));
	sf::Vector2f newDesired = desired/length;//Normalize

	if (length > r)
		newDesired *= myVehicle.maxSpeed;
	else
	{
		float m = (myVehicle.maxSpeed * (length / r * 3));

		newDesired *= m;
	}
	sf::Vector2f steer = newDesired - myVehicle.velocity;
	myVehicle.applyForce(steer);
}

void VehicleSystem::Separate(Vehicle& myVehicle, std::vector<Vehicle*>* otherVehicle)
{
	float desiredSeparation = r * 2;

	sf::Vector2f sum;
	int count = 0;
	if (otherVehicle != nullptr)
	{
		for (auto *other : *otherVehicle)
		{
			sf::Vector2f dir = myVehicle.location - other->location;
			float length = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));

			if ((length > 0) && (length < desiredSeparation))
			{
				//Might have bug
				sf::Vector2f diff = dir / powf(length, 2);
				sum += diff;
				count++;
			}
		}
		if (count > 0)
		{
			sum.x / count;
			sum.y / count;
			myVehicle.applyForce((sum / sqrtf(powf(sum.x, 2) + powf(sum.y, 2))) * myVehicle.maxSpeed - myVehicle.velocity);
		}
	}
}

void VehicleSystem::Alignment(Vehicle& myVehicle, std::vector<Vehicle*>* otherVehicle)
{
	float distToOther = 120.0f;
	sf::Vector2f sum;
	int count = 0;
	for (auto *other : *otherVehicle)
	{
		sf::Vector2f dir = myVehicle.location - other->location;
		float length = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
		if ((length > 0) && (length < distToOther))
		{
			sum += other->velocity;
			count++;
		}
	}
	if (count > 0)
	{
		sum.x / count;
		sum.y / count;
		myVehicle.applyForce((sum / sqrtf(powf(sum.x, 2) + powf(sum.y, 2))) * myVehicle.maxSpeed - myVehicle.velocity);
	}
	else
	{
		myVehicle.applyForce(sf::Vector2f(0, 0));
	}

}

void VehicleSystem::Cohesion(Vehicle& myVehicle, std::vector<Vehicle*>* const otherVehicle)
{
	float neighborDist = 120.0f;
	sf::Vector2f sum;
	int count = 0;
	for (auto *other : *otherVehicle)
	{
		sf::Vector2f dir = myVehicle.location - other->location;
		float length = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
		if ((length > 0) && (length < neighborDist))
		{
			sum += other->location;
			count++;
		}
	}

	if (count > 0)
	{
		sum.x / count;
		sum.y / count;
		myVehicle.seek(sum);
	}
	else
		myVehicle.applyForce(sf::Vector2f(0, 0));
}

//Bucket
sf::Vector2i VehicleSystem::GetBucket(sf::Vector2f pos)
{
	return sf::Vector2i(pos.x / gridWidth, pos.y / gridHeight);
}

void VehicleSystem::AddBucket(sf::Vector2i bucket, Vehicle* v)
{
	grid[bucket.x][bucket.y].push_back(v); //add vector to the last
}

void VehicleSystem::RemoveBucket(sf::Vector2i bucket, Vehicle* v)
{
	auto i = std::find(grid[bucket.x][bucket.y].begin(), grid[bucket.x][bucket.y].end(), v);
	if (i != grid[bucket.x][bucket.y].end())
		grid[bucket.x][bucket.y].erase(i);
}

//void VehicleSystem::WindToggle()
//{
//	windToggle = !windToggle;
//}

//void VehicleSystem::GravityToggle()
//{
//	gravityToggle = !gravityToggle;
//}

void VehicleSystem::SeekToggle()
{
	seekToggle = !seekToggle;
}

void VehicleSystem::SeparateToggle()
{
	separateToggle = !separateToggle;
	std::cout << "separateToggle: " + separateToggle << std::flush;;
}

void VehicleSystem::AlignmentToggle()
{
	alignmentToggle = !alignmentToggle;
	std::cout << "alignmentToggle: " + alignmentToggle << std::flush;
}

void VehicleSystem::CohesionToggle()
{
	cohesionToggle = !cohesionToggle;
	std::cout << "cohesionToggle: " + cohesionToggle << std::flush;;
}

