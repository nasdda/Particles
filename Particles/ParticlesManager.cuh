#pragma once
#include <SFML/Graphics.hpp>
#include "ConfigManager.h"


#define NUM_PARTICLES 200000
//#define ATTRACTION_MASS 50000
#define MAX_PARTICLE_MASS 100
#define MIN_PARTICLE_MASS 10

#define G 5


struct Vertex {
	float x, y;
};

struct Config {
	int mouseMass;
	int mouseX;
	int mouseY;
	float minD;
	float maxD;
	int N;
	bool attract;
	float attractVel;
};

class ParticlesManager
{
public:
	ParticlesManager(sf::RenderWindow& mWindow, ConfigManager* confManager);
	~ParticlesManager();
	float attractVel;
	bool isPaused();
	void updatePositions(sf::RenderWindow& mWindow, sf::Vector2i& mousePos, bool shouldAttract);
	void drawParticles(sf::RenderWindow& mWindow);
	void togglePaused();
private:
	ConfigManager* cm;
	bool paused;
	sf::VertexArray particles = sf::VertexArray(sf::Points, NUM_PARTICLES);
	float* mass;
	sf::Vector2f attractor;
	Vertex* cudaPositions;
	Vertex* cudaVelocity;
};


