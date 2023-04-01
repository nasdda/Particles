#pragma once
#include <SFML/Graphics.hpp>

#define NUM_PARTICLES 500

class ParticlesManager
{
public:
	ParticlesManager(sf::RenderWindow& mWindow);
	void updatePositions(sf::RenderWindow& mWindow);
	void drawParticles(sf::RenderWindow& mWindow);
private:
	std::vector<sf::CircleShape*> particles;
	sf::Vector2f attractor;
	std::vector<float> velocity;
	std::vector<int> mass;
	std::vector<sf::Vector2f> delta;

private:

};