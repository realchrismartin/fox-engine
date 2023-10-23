#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

class GraphicsComponent;

class Window
{
public:
	Window();
	void clear();
	void display();
	void draw(GraphicsComponent& graphicsComponent);
	void pollForEvents();
	bool isOpen() const;
	const std::vector<sf::Event>& getEvents() const;
private:
	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::vector<sf::Event> m_events;

};
#endif