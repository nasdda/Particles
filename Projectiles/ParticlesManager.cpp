#include <SFML/Graphics.hpp>

#ifndef ParticlesManager
#include "ParticlesManager.h"
#endif



ParticlesManager::ParticlesManager(sf::RenderWindow& mWindow) {
	attractor.x = 100.f;
	attractor.y = 100.f;

	dmin = 200;
	dmax = 250;

	attractVel = 5.0;

	for (int i = 0; i < NUM_PARTICLES; i++) {
		sf::CircleShape* particle = new sf::CircleShape;
		mass.push_back(std::max(MIN_PARTICLE_MASS, (rand() % MAX_PARTICLE_MASS)));
		particle->setRadius(5.f);
		particle->setPosition(rand() % mWindow.getSize().x, rand() % mWindow.getSize().y);

		particle->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));


		particles.push_back(particle);
		velocity.push_back(sf::Vector2f(0.f, 0.f));
	}
}



void ParticlesManager::updatePositions(sf::RenderWindow& mWindow) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		sf::Vector2f pos1 = particles[i]->getPosition();
		sf::Vector2i pos2 = sf::Mouse::getPosition(mWindow);
		float dx = std::abs(pos1.x - pos2.x), dy = std::abs(pos1.y - pos2.y);
		float d = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
		d = std::max(d, dmin);
		d = std::min(d, dmax);
		float F = 0.5 * mass[i] * ATTRACTION_MASS / (d * d);
		float a = F / mass[i];
		float dv = a;

		float tot = dx + dy;

		sf::Vector2f movement(0.f, 0.f);

		if (pos1.x > pos2.x) {
			movement.x = -(dx / tot) * dv;
		}
		else {
			movement.x = (dx / tot) * dv;
		}

		if (pos1.y > pos2.y) {
			movement.y = -(dy / tot) * dv;
		}
		else {
			movement.y = (dy / tot) * dv;
		}

		//movement *= dv;

		velocity[i] += movement;
	}


}


void ParticlesManager::drawParticles(sf::RenderWindow& mWindow) {
	sf::Vector2u winSize = mWindow.getSize();
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i]->move(velocity[i]);
	}
	for (int i = 0; i < NUM_PARTICLES; i++) {
		mWindow.draw(*particles[i]);
	}
}




void ParticlesManager::attract(sf::RenderWindow& mWindow) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		sf::Vector2f movement(0.f, 0.f);
		sf::Vector2i mosPos = sf::Mouse::getPosition(mWindow);

		if (particles[i]->getPosition().x > mosPos.x) {
			movement.x = -attractVel;
		}
		else {
			movement.x = attractVel;
		}

		if (particles[i]->getPosition().y > mosPos.y) {
			movement.y = -attractVel;
		}
		else {
			movement.y = attractVel;
		}

		particles[i]->move(movement);
	}
}