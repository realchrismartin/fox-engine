#include "src/Window.hpp"
#include "SFML/Graphics/View.hpp"

Window::Window()
{
	m_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1600, 1200), "Chris and J's Game");
	m_renderWindow->setFramerateLimit(60);

	//Make a "camera" that's a view of the window.
	sf::View camera = sf::View(sf::Vector2f(800.f,600.f), sf::Vector2f(1600.f,1200.f));
	//camera.rotate(50.f);
	m_renderWindow->setView(camera);
}

void Window::clear()
{
	m_renderWindow->clear();
}

void Window::display()
{
	m_renderWindow->display();
}

void Window::pollForEvents()
{
	m_events.clear();

	sf::Event event;
	while (m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			//Fuck it, we're just gonna close up shop here.
			m_renderWindow->close();
		}

		m_events.push_back(event);
	}
}

bool Window::isOpen() const
{
	return m_renderWindow->isOpen();
}

const std::vector<sf::Event>& Window::getEvents() const
{
	return m_events;
}
