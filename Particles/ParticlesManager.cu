#include <SFML/Graphics.hpp>
#include "ParticlesManager.cuh"
#include "iostream"
#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


Config* currentConfig;

// CUDA kernel to update particle positions
__global__ void cudaUpdatePositions(Vertex* position, Vertex* velocity, float* mass, ParticleColor* particleColors, Config* config)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i >= config->N) return;

	Vertex particlePos = position[i];
	float dx = config->mouseX - particlePos.x, dy = config->mouseY - particlePos.y;
	float actualD = std::sqrt(dx * dx + dy * dy);
	float d = fmaxf(actualD, config->minD);
	d = fminf(d, config->maxD);
	float a = G_CONSTANT * (config->mouseMass / mass[i]) / (d * d);

	float tot = abs(dx) + abs(dy);
	float rX = 0;
	float rY = 0;
	rX = dx / tot;
	rY = dy / tot;

	velocity[i].x += rX * a;
	velocity[i].y += rY * a;
	position[i].x += velocity[i].x;
	position[i].y += velocity[i].y;

	float vel = sqrt(velocity[i].x * velocity[i].x + velocity[i].y * velocity[i].y);
	float velPercent =  fmaxf(vel / 30.f, 0.1f);
	particleColors[i] = ParticleColor{ 0, 170, 255, (short) (velPercent * 255) };

	if (config->attract) {
		float dx = config->mouseX - position[i].x, dy = config->mouseY - position[i].y;
		float tot = abs(dx) + abs(dy);
		position[i].x += dx * config->attractVel / 100;
		position[i].y += dy * config->attractVel / 100;
	}
}


ParticlesManager::ParticlesManager(sf::RenderWindow& mWindow, ConfigManager* confManager) {

	cm = confManager;
	attractor.x = 100.f;
	attractor.y = 100.f;
	paused = false;
	sf::Vector2f center = sf::Vector2f(mWindow.getSize().x / 2.0f, mWindow.getSize().y / 2.0f);

	cudaMallocManaged(&cudaPositions, NUM_PARTICLES * sizeof(Vertex));
	cudaMallocManaged(&cudaVelocity, NUM_PARTICLES * sizeof(Vertex));
	cudaMallocManaged(&mass, NUM_PARTICLES * sizeof(float));
	cudaMallocManaged(&currentConfig, sizeof(Config));
	cudaMallocManaged(&particleColors, NUM_PARTICLES * sizeof(ParticleColor));

	for (int i = 0; i < NUM_PARTICLES; i++) {
		mass[i] = (std::max((float)MIN_PARTICLE_MASS, ((float)rand() / RAND_MAX) * MAX_PARTICLE_MASS));

		// Spawn randomly in a circular fashion
		float radius = 500 * ((float)rand() / RAND_MAX);
		double theta = ((double)rand() / RAND_MAX) * 2 * M_PI;
		float x = center.x + radius * cos(theta);
		float y = center.y + radius * sin(theta);

		particles[i].position = sf::Vector2f(x, y);
		particles[i].color = sf::Color::Black;

		cudaPositions[i] = Vertex{ x, y };
		cudaVelocity[i] = Vertex{ 0.f, 0.f };
		particleColors[i] = ParticleColor{255, 255, 255, 255};
	}
}

ParticlesManager::~ParticlesManager() {
	cudaFree(cudaPositions);
	cudaFree(cudaVelocity);
	cudaFree(mass);
	cudaFree(currentConfig);
	cudaFree(particleColors);
}

void ParticlesManager::updatePositions(sf::RenderWindow& mWindow, sf::Vector2i& mousePos, bool shouldAttract) {
	if (paused) return;
	// Update config to reflect current condition
	currentConfig->mouseMass = cm->mouseMass;
	currentConfig->mouseX = mousePos.x;
	currentConfig->mouseY = mousePos.y;
	currentConfig->minD = cm->minD;
	currentConfig->maxD = cm->maxD;
	currentConfig->N = NUM_PARTICLES;
	currentConfig->attract = shouldAttract;
	currentConfig->attractVel = cm->attractVel;
	// Run on multiple blocks
	int blockSize = 256;
	int numBlocks = (NUM_PARTICLES + blockSize - 1) / blockSize;
	cudaUpdatePositions<<<numBlocks, blockSize>>>(cudaPositions, cudaVelocity, mass, particleColors, currentConfig);
	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();
	float vel = 0;
	for (int i = 0; i < NUM_PARTICLES; i++) {
		vel = std::max(vel, cudaVelocity[i].x);
	}
}


void ParticlesManager::drawParticles(sf::RenderWindow& mWindow) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position = sf::Vector2f(cudaPositions[i].x, cudaPositions[i].y);
		particles[i].color = sf::Color(particleColors[i].R, particleColors[i].G, particleColors[i].B, particleColors[i].O);
	}
	mWindow.draw(particles);
}


void ParticlesManager::togglePaused() {
	paused = !paused;
}

bool ParticlesManager::isPaused() {
	return paused;
}


