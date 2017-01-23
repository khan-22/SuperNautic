//SuperNautic

#include <iostream>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <glm\glm.hpp>

#include "Log.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Hello World", sf::Style::Default);

	LOG("Hello World! ", 2017, ". The Log is working.");
	LOG_ERROR("This is how errors will look");

	glm::vec3 test(0.4f, 0.4f, 1.0f);

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed) 
			{
				window.close();
			}
		}
		window.clear();

		window.display();
	}

	CLOSE_LOG();
	return 0;
}