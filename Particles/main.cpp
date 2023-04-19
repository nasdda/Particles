#include <SFML/Graphics.hpp>
#include <iostream>
#include "ParticlesManager.h"
#ifndef ConfigManager
#include "ConfigManager.h"
#endif


class Game
{
public:
	Game();
	void run();
private:
	void processEvents();
	void update();
	void render();
	void handlePlayerInput(sf::Event event);
	bool mIsMovingUp = false;
	bool mIsMovingDown = false;
	bool mIsMovingLeft = false;
	bool mIsMovingRight = false;
	bool pressed = false;

	float PlayerSpeed = 2.f;
	std::vector<sf::CircleShape*> particles;
	sf::Vector2f attractor;

	ParticlesManager* pm;
	ConfigManager* cm;

private:
	sf::RenderWindow mWindow;
	sf::CircleShape mPlayer;
};

Game::Game() : mWindow(sf::VideoMode(1900, 1200), "SFML Application"), mPlayer()
{
	cm = new ConfigManager(mWindow);
	pm = new ParticlesManager(mWindow, cm);
	mWindow.setFramerateLimit(60);
}

void Game::run()
{
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		processEvents();
		update();
		pm->updatePositions(mWindow);
		cm->updateCirclePosition(mWindow);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		handlePlayerInput(event);
	}
}

void Game::handlePlayerInput(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		pressed = true;
		cm->handleMousePress(mWindow);
		break;
	case sf::Event::MouseButtonReleased:
		pressed = false;
		cm->handleMouseRelease();
		break;
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Space) {
			pm->togglePaused();
			cm->toggleControls();
		}
		break;
	case sf::Event::Closed:
		mWindow.close();
		break;
	}
}


void Game::update()
{
	if (pressed) {
		pm->attract(mWindow);
	}
}

void Game::render()
{
	mWindow.clear(sf::Color::White);
	pm->drawParticles(mWindow);
	cm->drawSliders(mWindow);
	cm->drawButtons(mWindow);
	mWindow.display();
}

int main()
{
	Game game;
	game.run();
}


