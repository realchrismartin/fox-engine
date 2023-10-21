#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

using namespace std;

int main()
{
	 sf::RenderWindow window(sf::VideoMode(800, 600), "Chris and J's Game");

	 window.setFramerateLimit(60);

	 sf::RectangleShape shape;
	 shape.setSize(sf::Vector2f(100, 101));
	 shape.setFillColor(sf::Color::Red);
	 shape.setPosition(sf::Vector2f(400.f,300.f));

	 // The main loop - ends as soon as the window is closed
	 while (window.isOpen())
	 {
		 // Event processing
		 sf::Event event;
		 while (window.pollEvent(event))
		 {
			 // Request for closing the window
			 if (event.type == sf::Event::Closed)
				 window.close();
		 }

		 // Clear the whole window before rendering a new frame
		 window.clear();

		 window.draw(shape);

		 // Draw some graphical entities
		 /*
		 window.draw(sprite);
		 window.draw(circle);
		 window.draw(text);
		 */

		 // End the current frame and display its contents on screen
		 window.display();
	}
	return 0;
}
