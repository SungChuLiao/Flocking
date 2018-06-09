#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "VehicleSystem.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Sung Chu Liao Flocking Assignment");

	sf::Clock clock;
	sf::Clock myTimer;
	float timer = 0.1f;

	float deltaTime;

	VehicleSystem* myVehicleSystem = new VehicleSystem(&window);

	/*sf::Text* m_text = new sf::Text;
	sf::Font* m_font = new sf::Font;
	m_font->loadFromFile("Fonts/georgiab.ttf");
	m_text->setFont(*m_font);
	m_text->setCharacterSize(16);
	m_text->setString("separateToggle: " + std::to_string(myVehicleSystem->separateToggle) + "\n" + "AlignmentToggle: " + std::to_string(myVehicleSystem->alignmentToggle));
	m_text->setPosition(50, 50);*/

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
		
			if (event.type == sf::Event::Closed)
				window.close();
			if (timer < .3f)
			{
				timer += myTimer.restart().asSeconds();
			}
			else
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					myVehicleSystem->AddVehicle(&window);
					timer = 0;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					myVehicleSystem->RemoveVehicle();
					timer = 0;
				}
			}
		}
		

		deltaTime = clock.restart().asSeconds();

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			myVehicleSystem->WindToggle();
			timer = 0;
		}*/
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			myVehicleSystem->GravityToggle();
			timer = 0;
		}*/
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			myVehicleSystem->SeekToggle();
			timer = 0;
		}*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			myVehicleSystem->SeparateToggle();
			timer = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			myVehicleSystem->AlignmentToggle();
			timer = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			myVehicleSystem->CohesionToggle();
			timer = 0;
		}

		myVehicleSystem->Update(&window, deltaTime);
		window.clear();
		myVehicleSystem->Draw(&window);
		window.display();
	}

	return 0;


}
