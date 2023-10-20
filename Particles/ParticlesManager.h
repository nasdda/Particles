#pragma once
#include <SFML/Graphics.hpp>
#include "ConfigManager.h"


#define NUM_PARTICLES 20000
//#define ATTRACTION_MASS 50000
#define MAX_PARTICLE_MASS 100
#define MIN_PARTICLE_MASS 10

#define G 5


class ParticlesManager
{
public:
	ParticlesManager(sf::RenderWindow& mWindow, ConfigManager* confManager);
	float attractVel;
	bool isPaused();
	void updatePositions(sf::RenderWindow& mWindow, sf::Vector2i &mousePos);
	void drawParticles(sf::RenderWindow& mWindow);
	void attract(sf::RenderWindow& mWindow);
	void togglePaused();
private:
	ConfigManager* cm;
	bool paused;
	sf::VertexArray particles = sf::VertexArray(sf::Points, NUM_PARTICLES);
	std::vector<float> mass;
	std::vector<sf::Vector2f> velocity;
	sf::Vector2f attractor;
};


