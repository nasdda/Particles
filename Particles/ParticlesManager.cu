#include <SFML/Graphics.hpp>
#include "ParticlesManager.cuh"
#include "iostream"
#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include <thread>
#include <random>

#ifndef PI
#define PI 3.14159265358979323846
#endif


Config* currentConfig;


// CUDA kernel to update particle positions
__global__ void cudaUpdatePositions(Vertex* position, Vertex* velocity, float* mass, short* particleOpacity, Config* config)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i >= config->N) return;

	Vertex particlePos = position[i];
	float dx = config->mouseX - particlePos.x, dy = config->mouseY - particlePos.y;
	float actualD = std::sqrt(dx * dx + dy * dy);
	float d = fmaxf(actualD, config->minD);
	d = fminf(d, config->maxD);
	float a;

	if (config->version == 1) {
		a = G_CONSTANT * (config->mouseMass / mass[i]) / (d * d);
		float tot = abs(dx) + abs(dy);
		float rX = dx / tot;
		float rY = dy / tot;
		velocity[i].x += rX * a;
		velocity[i].y += rY * a;
	}
	else {
		a = G_CONSTANT * (config->mouseMass) / (d * d);
		velocity[i].x += dx * a / 100.f;
		velocity[i].y += dy * a / 100.f;
	}

	position[i].x += velocity[i].x;
	position[i].y += velocity[i].y;
	float oPercent = fmaxf(abs(a) / 5.f, 0.2);
	particleOpacity[i] = fminf(255, oPercent * 255);

	if (config->attract) {
		float dx = config->mouseX - position[i].x, dy = config->mouseY - position[i].y;
		float tot = abs(dx) + abs(dy);
		position[i].x += dx * config->attractVel / 100;
		position[i].y += dy * config->attractVel / 100;
	}
}


void updateParticles(int start, int end, sf::VertexArray& particles, Vertex* cudaPositions, short* particleOpacity, sf::Color color) {
	for (int i = start; i < end; i++) {
		particles[i].position.x = cudaPositions[i].x;
		particles[i].position.y = cudaPositions[i].y;
		sf::Color pre = particles[i].color;
		particles[i].color = sf::Color(color.r, color.g, color.b, particleOpacity[i]);
	}
}


short randRange(int low, int high) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}


sf::Color getNextColor(float t) {
	int r = 220 + 35 * std::sin(PI * t / 2);
	int g = 60 + 50 * std::sin(PI * t / 3 + 2.0);
	int b = 5 + 5 * std::sin(PI * t / 4 + 4.0);
	return sf::Color(r, g, b);
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
	cudaMallocManaged(&particleOpacity, NUM_PARTICLES * sizeof(short));

	for (int i = 0; i < NUM_PARTICLES; i++) {
		mass[i] = (std::max((float)MIN_PARTICLE_MASS, ((float)rand() / RAND_MAX) * MAX_PARTICLE_MASS));

		// Spawn randomly in a circular fashion
		float radius = 500 * ((float)rand() / RAND_MAX);
		double theta = ((double)rand() / RAND_MAX) * 2 * PI;
		float x = center.x + radius * cos(theta);
		float y = center.y + radius * sin(theta);

		particles[i].position = sf::Vector2f(x, y);
		// Initial color of particles
		particles[i].color = sf::Color::White;

		cudaPositions[i] = Vertex{ x, y };
		cudaVelocity[i] = Vertex{ 0.f, 0.f };
		particleOpacity[i] = 255;
	}

	t = 0.f;
}


ParticlesManager::~ParticlesManager() {
	cudaFree(cudaPositions);
	cudaFree(cudaVelocity);
	cudaFree(mass);
	cudaFree(currentConfig);
	cudaFree(particleOpacity);
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
	currentConfig->version = cm->version;
	// Run on multiple blocks
	cudaUpdatePositions << <NUM_BLOCKS, BLOCK_SIZE >> > (cudaPositions, cudaVelocity, mass, particleOpacity, currentConfig);
	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();
}


void ParticlesManager::drawParticles(sf::RenderWindow& mWindow) {
	if (paused) {
		mWindow.draw(particles);
		return;
	}
	// Update SFML particles 
	int particlesPerThread = (NUM_PARTICLES / CPU_THREADS) + 1;
	std::vector<std::thread> threads;
	sf::Color newColor = getNextColor(t);

	for (int i = 0; i < CPU_THREADS; i++) {
		int start = i * particlesPerThread;
		int end = std::min(NUM_PARTICLES, start + particlesPerThread);
		threads.emplace_back(updateParticles, start, end, std::ref(particles), cudaPositions, particleOpacity, newColor);
	}
	for (auto& th : threads) {
		th.join();
	}

	t += T_INCREMENT;
	mWindow.draw(particles);
}


void ParticlesManager::togglePaused() {
	paused = !paused;
}


bool ParticlesManager::isPaused() {
	return paused;
}


