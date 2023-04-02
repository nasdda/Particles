#pragma once
#include <SFML/Graphics.hpp>

#define NUM_PARTICLES 20
#define ATTRACTION_MASS 0
#define MAX_PARTICLE_MASS 50
#define MIN_PARTICLE_MASS 20


class ParticlesManager
{
public:
	ParticlesManager(sf::RenderWindow& mWindow);
	void updatePositions(sf::RenderWindow& mWindow);
	void drawParticles(sf::RenderWindow& mWindow);
	float dmin;
	float dmax;
	float attractVel;
	void attract(sf::RenderWindow& mWindow);
private:
	std::vector<sf::CircleShape*> particles;
	sf::Vector2f attractor;
	std::vector<int> mass;
	std::vector<sf::Vector2f> velocity;

};


