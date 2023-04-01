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
	void update(sf::Time deltaTime);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	bool mIsMovingUp = false;
	bool mIsMovingDown = false;
	bool mIsMovingLeft = false;
	bool mIsMovingRight = false;

	float PlayerSpeed = 2.f;
	std::vector<sf::CircleShape*> particles;
	sf::Vector2f attractor;

	ParticlesManager *pm;

private:
	sf::RenderWindow mWindow;
	sf::CircleShape mPlayer;
};

Game::Game() : mWindow(sf::VideoMode(640, 480), "SFML Application"), mPlayer()
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
		/*case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;*/
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}


void Game::update(sf::Time deltaTime)
{
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


