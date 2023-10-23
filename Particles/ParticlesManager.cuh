#pragma once
#include <SFML/Graphics.hpp>
#include "ConfigManager.h"


#define NUM_PARTICLES 400000
#define MAX_PARTICLE_MASS 100
#define MIN_PARTICLE_MASS 10
#define G_CONSTANT 5
#define T_INCREMENT 0.02f
// CPU
#define CPU_THREADS 16
// GPU
#define BLOCK_SIZE 256
#define NUM_BLOCKS ((NUM_PARTICLES + BLOCK_SIZE - 1) / BLOCK_SIZE)


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
	short version;
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
	short* particleOpacity;
	float t; // For calculating particle color 
};


