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
	void pollForEvents();
	bool isOpen() const;
	const std::vector<sf::Event>& getEvents() const;

	template<typename T>
	void draw(T drawableComponent)
	{
		m_renderWindow->draw(drawableComponent.getDrawable());
	}

private:
	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::vector<sf::Event> m_events;

};
#endif

