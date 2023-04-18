#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#ifndef ConfigManager
#include "ConfigManager.h"
#endif



template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

int getRatioValue(float ratio, int min, int max) {
	int add = (max - min) * ratio;
	return min + add;
}


ConfigManager::ConfigManager(sf::RenderWindow& mWindow) {
	
	clickedCircle = -1;

	sf::Font* font = new sf::Font();
	if (!font->loadFromFile("arial.ttf"))
	{
		std::cout << "FAILED TO LOAD FONT\n";
	}

	// Initial config
	mouseMass = 20000;
	minD = 170;
	maxD = 200;

	ratios[0] = (float) mouseMass / MOUSE_MASS_MAX;
	ratios[1] = (float) minD / RANGE_MAX_D;
	ratios[2] = (float) maxD / RANGE_MAX_D;

	// Update to resolve rounding errors
	mouseMass = getRatioValue(ratios[0], MOUSE_MASS_MIN, MOUSE_MASS_MAX);
	minD = getRatioValue(ratios[1], RANGE_MIN_D, RANGE_MAX_D);
	maxD = getRatioValue(ratios[2], RANGE_MIN_D, RANGE_MAX_D);

	std::cout << ratios[0] << std::endl;

	for (int i = 0; i < NUM_OPTIONS; i++) {
		lines[i] = sf::RectangleShape(sf::Vector2f(LINE_WIDTH, LINE_HEIGHT));
		lines[i].setPosition(sf::Vector2f(STARTING_X, i * MARGIN_Y + MARGIN_Y));
		lines[i].setFillColor(sf::Color(200, 200, 200));

		circles[i] = sf::CircleShape(CIRCLE_RADIUS);
		circles[i].setPosition(STARTING_X + (ratios[i] * LINE_WIDTH), i * MARGIN_Y + MARGIN_Y + (LINE_HEIGHT / 2));
		circles[i].setFillColor(sf::Color::White);
		circles[i].setOutlineColor(sf::Color::Black);
		circles[i].setOutlineThickness(1);
		circles[i].setOrigin(circles[i].getRadius(), circles[i].getRadius());

		texts[i] = sf::Text();
		texts[i].setFont(*font);
		texts[i].setFillColor(sf::Color::Black);
		texts[i].setCharacterSize(CONFIG_FONT_SIZE);
		texts[i].setPosition(STARTING_X, i * MARGIN_Y + (MARGIN_Y - 40));
	}

	
	texts[0].setString(string_format("Mouse Mass: %d", mouseMass));
	texts[1].setString(string_format("Min D: %.2f", minD));
	texts[2].setString(string_format("Max D: %.2f", maxD));

}


void ConfigManager::drawSliders(sf::RenderWindow& mWindow) {

	for (int i = 0; i < NUM_OPTIONS; i++) {
		mWindow.draw(texts[i]);
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
	pos.x = std::max((float)(STARTING_X), pos.x);
	circles[clickedCircle].setPosition(pos);

	float gap = pos.x - STARTING_X;

	ratios[clickedCircle] = gap / LINE_WIDTH;

	mouseMass = getRatioValue(ratios[0], MOUSE_MASS_MIN, MOUSE_MASS_MAX);
	minD = getRatioValue(ratios[1], RANGE_MIN_D, RANGE_MAX_D);
	minD = std::min(minD, maxD);
	maxD = getRatioValue(ratios[2], RANGE_MIN_D, RANGE_MAX_D);
	maxD = std::max(minD, maxD);

	texts[0].setString(string_format("Mouse Mass: %d", mouseMass));
	texts[1].setString(string_format("Min D: %.2f", minD));
	texts[2].setString(string_format("Max D: %.2f", maxD));
}

