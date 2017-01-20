#include <iostream>

#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Hello World", sf::Style::Default);

	std::cout << "Hello World" << std::endl;

	glm::vec3 test(0.4f, 0.4f, 1.0f);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();

		window.display();
	}

	return 0;
}