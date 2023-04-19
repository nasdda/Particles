#pragma once
#include <SFML/Graphics.hpp>
#include "ConfigManager.h"


#define NUM_PARTICLES 1000
//#define ATTRACTION_MASS 50000
#define MAX_PARTICLE_MASS 50
#define MIN_PARTICLE_MASS 20


class ParticlesManager
{
public:
	ParticlesManager(sf::RenderWindow& mWindow, ConfigManager* confManager);
	float attractVel;
	bool isPaused();
	void updatePositions(sf::RenderWindow& mWindow);
	void drawParticles(sf::RenderWindow& mWindow);
	void attract(sf::RenderWindow& mWindow);
	void togglePaused();
private:
	ConfigManager* cm;
	bool paused;
	std::vector<sf::CircleShape*> particles;
	std::vector<int> mass;
	std::vector<sf::Vector2f> velocity;
	sf::Vector2f attractor;
};


