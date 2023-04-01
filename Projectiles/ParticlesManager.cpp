#include <SFML/Graphics.hpp>
#include "ParticlesManager.h"





ParticlesManager::ParticlesManager(sf::RenderWindow& mWindow) {
	attractor.x = 100.f;
	attractor.y = 100.f;

	for (int i = 0; i < NUM_PARTICLES; i++) {
		sf::CircleShape* particle = new sf::CircleShape;
		particle->setRadius(5.f);
		particle->setPosition((float)(rand() % mWindow.getSize().x), (float)(rand() % mWindow.getSize().x));
		particle->setFillColor(sf::Color::Cyan);

		particles.push_back(particle);
		mass.push_back((rand() % 10) + 1);
		velocity.push_back(sf::Vector2f(0.f, 0.f));
	}
}



void ParticlesManager::updatePositions(sf::RenderWindow& mWindow) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		for (int j = 0; j < NUM_PARTICLES; j++) {
			if (i == j) continue;
			sf::Vector2f pos1 = particles[i]->getPosition(), pos2 = particles[j]->getPosition();
			float d = std::max(0.1, std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2)));
			float F = mass[i] * mass[j] / d;
			float a = F / mass[i];
			float dv = a * 0.1;

			if (pos1.x > pos2.x) {
				velocity[i].x -= dv;
			}
			else {
				velocity[i].x += dv;
			}

			if (pos1.y > pos2.y) {
				velocity[i].y -= dv;
			}
			else {
				velocity[i].y += dv;
			}
		}

	}


}


void ParticlesManager::drawParticles(sf::RenderWindow& mWindow) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i]->move(velocity[i]);
	}
	for (int i = 0; i < NUM_PARTICLES; i++) {
		mWindow.draw(*particles[i]);
	}
}