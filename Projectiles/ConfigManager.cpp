#include <SFML/Graphics.hpp>
#include "ConfigManager.h"



ConfigManager::ConfigManager(sf::RenderWindow& mWindow) {

	for (int i = 0; i < NUM_OPTIONS; i++) {
		lines[i] = sf::RectangleShape(sf::Vector2f(200, 5));
		lines[i].setPosition(sf::Vector2f(10, i * 30 + 30));
		lines[i].setFillColor(sf::Color(200, 200, 200));

		circles[i] = sf::CircleShape(CIRCLE_RADIUS);
		circles[i].setPosition(5, i * 30 + 30 - 7);
		circles[i].setFillColor(sf::Color::White);
		circles[i].setOutlineColor(sf::Color::Black);
		circles[i].setOutlineThickness(1);
	}

}


void ConfigManager::drawSliders(sf::RenderWindow& mWindow) {

	for (int i = 0; i < NUM_OPTIONS; i++) {
		mWindow.draw(lines[i]);
		mWindow.draw(circles[i]);
	}

}


void ConfigManager::handleMouseDown(sf::RenderWindow& mWindow) {
	sf::Vector2i mPos = sf::Mouse::getPosition(mWindow);


}