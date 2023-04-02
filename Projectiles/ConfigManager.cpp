#include <SFML/Graphics.hpp>

#ifndef ConfigManager
#include "ConfigManager.h"
#endif


#include <iostream>


ConfigManager::ConfigManager(sf::RenderWindow& mWindow) {

	clickedCircle = -1;

	for (int i = 0; i < NUM_OPTIONS; i++) {
		lines[i] = sf::RectangleShape(sf::Vector2f(LINE_WIDTH, LINE_HEIGHT));
		lines[i].setPosition(sf::Vector2f(STARTING_X, i * MARGIN_Y + MARGIN_Y));
		lines[i].setFillColor(sf::Color(200, 200, 200));

		circles[i] = sf::CircleShape(CIRCLE_RADIUS);
		circles[i].setPosition(STARTING_X + (CIRCLE_RADIUS / 2), i * MARGIN_Y + MARGIN_Y + (LINE_HEIGHT / 2));
		circles[i].setFillColor(sf::Color::White);
		circles[i].setOutlineColor(sf::Color::Black);
		circles[i].setOutlineThickness(1);
		circles[i].setOrigin(circles[i].getRadius(), circles[i].getRadius());
	}

}


void ConfigManager::drawSliders(sf::RenderWindow& mWindow) {

	for (int i = 0; i < NUM_OPTIONS; i++) {
		mWindow.draw(lines[i]);
		mWindow.draw(circles[i]);
	}

}

bool inCircle(sf::CircleShape& circle, sf::Vector2i& mPos) {
	sf::Vector2f cPos = circle.getPosition();
	float d = std::sqrt(std::pow(cPos.x - mPos.x, 2) + std::pow(cPos.y - mPos.y, 2));
	return d <= circle.getRadius();
}

void ConfigManager::handleMousePress(sf::RenderWindow& mWindow) {
	sf::Vector2i mPos = sf::Mouse::getPosition(mWindow);

	for (int i = 0; i < NUM_OPTIONS; i++) {
		if (inCircle(circles[i], mPos)) {
			clickedCircle = i;
			break;
		}
	}

}


void ConfigManager::handleMouseRelease() {
	clickedCircle = -1;
}


void ConfigManager::updateCirclePosition(sf::RenderWindow& mWindow) {
	//std::cout << clickedCircle << std::endl;
	if (clickedCircle == -1) {
		// No circle selected
		return;
	}
	sf::Vector2f pos = circles[clickedCircle].getPosition();
	pos.x = sf::Mouse::getPosition(mWindow).x;
	pos.x = std::min((float)(STARTING_X + LINE_WIDTH), pos.x);
	pos.x = std::max((float)(STARTING_X + (CIRCLE_RADIUS / 2)), pos.x);
	circles[clickedCircle].setPosition(pos);
}

