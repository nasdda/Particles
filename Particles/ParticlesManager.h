#pragma once
#include <SFML/Graphics.hpp>
#ifndef ConfigManager
#include "ConfigManager.h"
#endif


#define NUM_PARTICLES 1000
//#define ATTRACTION_MASS 50000
#define MAX_PARTICLE_MASS 50
#define MIN_PARTICLE_MASS 20


class ParticlesManager
{
public:
	ParticlesManager(sf::RenderWindow& mWindow, ConfigManager* confManager);
	void updatePositions(sf::RenderWindow& mWindow);
	void drawParticles(sf::RenderWindow& mWindow);
	float attractVel;
	void attract(sf::RenderWindow& mWindow);
	void togglePaused();
	bool isPaused();
private:
	std::vector<sf::CircleShape*> particles;
	sf::Vector2f attractor;
	std::vector<int> mass;
	std::vector<sf::Vector2f> velocity;
	ConfigManager* cm;

	bool paused;
};


