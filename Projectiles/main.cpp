#include <SFML/Graphics.hpp>
#include <vector>
#include "ParticlesManager.h"


class Game
{
public:
	Game();
	void run();
private:
	void processEvents();
	void update();
	void render();
	void handlePlayerInput(bool isPressed);
	bool mIsMovingUp = false;
	bool mIsMovingDown = false;
	bool mIsMovingLeft = false;
	bool mIsMovingRight = false;
	bool pressed = false;

	float PlayerSpeed = 2.f;
	std::vector<sf::CircleShape*> particles;
	sf::Vector2f attractor;

	ParticlesManager* pm;

private:
	sf::RenderWindow mWindow;
	sf::CircleShape mPlayer;
};

Game::Game() : mWindow(sf::VideoMode(640 * 2, 480 * 2), "SFML Application"), mPlayer()
{
	pm = new ParticlesManager(mWindow);
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
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			handlePlayerInput(true);
			break;
		case sf::Event::MouseButtonReleased:
			handlePlayerInput(false);
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::handlePlayerInput(bool isPressed)
{
	pressed = isPressed;
}


void Game::update()
{
	if (pressed) {
		pm->attract(mWindow);
	}
}

void Game::render()
{
	mWindow.clear();
	pm->drawParticles(mWindow);
	mWindow.display();
}

int main()
{
	Game game;
	game.run();
}


