#include <SFML/Graphics.hpp>
#include <iostream>
#include "ConfigManager.h"
#include "ParticlesManager.cuh"



class Particles
{
public:
	Particles();
	void run();
private:
	void processEvents();
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

Particles::Particles() : mWindow(sf::VideoMode(1900, 1200), "SFML Application"), mPlayer()
{
	cm = new ConfigManager(mWindow);
	pm = new ParticlesManager(mWindow, cm);
	mWindow.setFramerateLimit(60);
}

void Particles::run()
{
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		processEvents();
		sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
		pm->updatePositions(mWindow, mousePos, pressed);
		cm->updateCirclePosition(mWindow);
		render();
	}
}

void Particles::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		handlePlayerInput(event);
	}
}

void Particles::handlePlayerInput(sf::Event event)
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


void Particles::render()
{
	mWindow.clear(sf::Color::White);
	pm->drawParticles(mWindow);
	cm->drawSliders(mWindow);
	cm->drawButtons(mWindow);
	mWindow.display();
}

int main()
{
	Particles particles;
	particles.run();
}


