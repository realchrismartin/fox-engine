#include "Game.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

Game::Game()
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Chris and J's Game");
	m_window->setFramerateLimit(60);
}

void Game::play()
{
	 sf::RectangleShape shape;
	 shape.setSize(sf::Vector2f(100, 101));
	 shape.setFillColor(sf::Color::Red);
	 shape.setPosition(sf::Vector2f(400.f,300.f));

	 while (m_window->isOpen())
	 {
		 sf::Event event;

		 while (m_window->pollEvent(event))
		 {
			 // Request for closing the window
			 if (event.type == sf::Event::Closed)
			 {
				m_window->close();
			 }
		 }

		 m_window->clear();
		 m_window->draw(shape);
		 m_window->display();
	}
}

Game::~Game()
{
}