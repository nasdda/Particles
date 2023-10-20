#include <SFML/Graphics.hpp>
#include "ParticlesManager.h"
#include "iostream"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


ParticlesManager::ParticlesManager(sf::RenderWindow& mWindow, ConfigManager* confManager) {

	cm = confManager;

	attractor.x = 100.f;
	attractor.y = 100.f;
	/*attractVel = 5.0;*/
	paused = false;

	sf::Vector2f center = sf::Vector2f(mWindow.getSize().x / 2.0f, mWindow.getSize().y / 2.0f);
	


	for (int i = 0; i < NUM_PARTICLES; i++) {
		mass.push_back(std::max((float)MIN_PARTICLE_MASS, ((float)rand() / RAND_MAX)* MAX_PARTICLE_MASS));
		
		// Spawn randomly in a circular fashion
		float radius = 500 * ((float)rand() / RAND_MAX);
		double theta = ((double)rand() / RAND_MAX) * 2 * M_PI;
		float x = center.x + radius * cos(theta);
		float y = center.y + radius * sin(theta);

		particles[i].position = sf::Vector2f(x, y);
		particles[i].color = sf::Color::Black;
		velocity.push_back(sf::Vector2f(0.f, 0.f));
	}
}



void ParticlesManager::updatePositions(sf::RenderWindow& mWindow, sf::Vector2i &mousePos) {
	if (paused) return;

	for (int i = 0; i < NUM_PARTICLES; i++) {
		sf::Vector2f particlePos = particles[i].position;
		float dx = mousePos.x - particlePos.x, dy = mousePos.y - particlePos.y;
		float d = std::sqrt(dx * dx + dy * dy);
		d = std::max(d, cm->minD);
		d = std::min(d, cm->maxD);
		float a = G * (cm->mouseMass / mass[i]) / (d * d);

		float tot = abs(dx) + abs(dy);
		float rX = 0;
		float rY = 0;
		// Replace tot with d or vice versa to have different attraction behavior
		if (cm->version == VER1) {
			rX = dx / tot;
			rY = dy / tot;
		}
		else if (cm->version == VER2) {
			rX = dx / d;
			rY = dy / d;
		}

		sf::Vector2f movement(rX * a, rY * a);

		// The more accurate formula, though less interesting visually.
		//sf::Vector2f movement(dx * a, dy * a);

		velocity[i] += movement;
 	}


}


void ParticlesManager::drawParticles(sf::RenderWindow& mWindow) {
	if (!paused) {
		sf::Vector2u winSize = mWindow.getSize();
		for (int i = 0; i < NUM_PARTICLES; i++) {
			particles[i].position += velocity[i];
		}
	}

	mWindow.draw(particles);
}




void ParticlesManager::attract(sf::RenderWindow& mWindow) {
	if (paused) return;

	for (int i = 0; i < NUM_PARTICLES; i++) {
		sf::Vector2i mosPos = sf::Mouse::getPosition(mWindow);
		sf::Vector2f pos = particles[i].position;

		float dx = mosPos.x - pos.x, dy = mosPos.y - pos.y;
		float tot = abs(dx) + abs(dy);

		sf::Vector2f movement(dx * cm->attractVel / 100, dy * cm->attractVel / 100);

		particles[i].position += movement;
	}
}



void ParticlesManager::togglePaused() {
	paused = !paused;
}

bool ParticlesManager::isPaused() {
	return paused;
}