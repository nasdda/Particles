#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include "ConfigManager.h"


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


bool mouseInRectangle(const sf::RenderWindow& window, const sf::RectangleShape& rectangle) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
	return rectangle.getGlobalBounds().contains(worldPosition);
}


ConfigManager::ConfigManager(sf::RenderWindow& mWindow) {

	clickedCircle = -1;
	hide = true;
	versions[0] = VER1;
	versions[1] = VER2;
	version = VER1;


	sf::Font* font = new sf::Font();
	if (!font->loadFromFile("arial.ttf"))
	{
		std::cout << "FAILED TO LOAD FONT\n";
	}

	// Initial config
	mouseMass = 20000;
	minD = 170;
	maxD = 200;
	attractVel = 5.0;

	ratios[0] = (float)mouseMass / MOUSE_MASS_MAX;
	ratios[1] = (float)minD / RANGE_MAX_D;
	ratios[2] = (float)maxD / RANGE_MAX_D;
	ratios[3] = (float)attractVel / ATTRACT_VEL_MAX;

	// Update to resolve rounding errors
	mouseMass = getRatioValue(ratios[0], MOUSE_MASS_MIN, MOUSE_MASS_MAX);
	minD = getRatioValue(ratios[1], RANGE_MIN_D, RANGE_MAX_D);
	maxD = getRatioValue(ratios[2], RANGE_MIN_D, RANGE_MAX_D);
	attractVel = getRatioValue(ratios[3], ATTRACT_VEL_MIN, ATTRACT_VEL_MAX);


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

		sliderTexts[i] = sf::Text();
		sliderTexts[i].setFont(*font);
		sliderTexts[i].setFillColor(sf::Color::White);
		sliderTexts[i].setCharacterSize(CONFIG_FONT_SIZE);
		sliderTexts[i].setPosition(STARTING_X, i * MARGIN_Y + (MARGIN_Y - 40));
	}

	updateTexts();

	std::string buttonTextContent[2] = { "VER 1", "VER 2" };

	// Buttons
	for (int i = 0; i < NUM_VERSIONS; i++) {
		std::cout << i << std::endl;
		buttons[i] = sf::RectangleShape(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
		buttons[i].setPosition(sf::Vector2f(STARTING_X + (BUTTON_WIDTH + MARGIN_X) * i, NUM_OPTIONS * MARGIN_Y + MARGIN_Y));
		buttons[i].setFillColor(sf::Color(255, 255, 255));
		buttons[i].setOutlineThickness(2);
		buttons[i].setOutlineColor(sf::Color(100, 100, 100));

		buttonTexts[i] = sf::Text();
		buttonTexts[i].setFont(*font);
		buttonTexts[i].setFillColor(sf::Color::Black);
		buttonTexts[i].setCharacterSize(CONFIG_FONT_SIZE);

		buttonTexts[i].setString(buttonTextContent[i]);
		sf::FloatRect textBounds = buttonTexts[i].getLocalBounds();
		sf::FloatRect rectBounds = buttons[i].getLocalBounds();
		buttonTexts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
		buttonTexts[i].setPosition(buttons[i].getPosition().x + rectBounds.width / 2.0f, buttons[i].getPosition().y + rectBounds.height / 2.0f);
	}

	// Initially version 1 is selected
	buttons[0].setFillColor(sf::Color(200, 200, 200));

}


void ConfigManager::drawSliders(sf::RenderWindow& mWindow) {
	if (hide) return;
	for (int i = 0; i < NUM_OPTIONS; i++) {
		mWindow.draw(sliderTexts[i]);
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
	if (hide) return;

	sf::Vector2i mPos = sf::Mouse::getPosition(mWindow);

	for (int i = 0; i < NUM_OPTIONS; i++) {
		if (inCircle(circles[i], mPos)) {
			clickedCircle = i;
			break;
		}
	}

	checkVersionClicked(mWindow);
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
	// Special case for minD and maxD
	if (clickedCircle == 1) {
		pos.x = std::min(pos.x, circles[2].getPosition().x);
	}
	else if (clickedCircle == 2) {
		pos.x = std::max(pos.x, circles[1].getPosition().x);
	}
	circles[clickedCircle].setPosition(pos);

	float gap = pos.x - STARTING_X;

	ratios[clickedCircle] = gap / LINE_WIDTH;

	mouseMass = getRatioValue(ratios[0], MOUSE_MASS_MIN, MOUSE_MASS_MAX);
	minD = getRatioValue(ratios[1], RANGE_MIN_D, RANGE_MAX_D);
	minD = std::min(minD, maxD);
	maxD = getRatioValue(ratios[2], RANGE_MIN_D, RANGE_MAX_D);
	maxD = std::max(minD, maxD);
	attractVel = getRatioValue(ratios[3], ATTRACT_VEL_MIN, ATTRACT_VEL_MAX);

	updateTexts();
}


void ConfigManager::updateTexts() {
	sliderTexts[0].setString(string_format("Mouse Mass: %d", mouseMass));
	sliderTexts[1].setString(string_format("Min D: %.2f", minD));
	sliderTexts[2].setString(string_format("Max D: %.2f", maxD));
	sliderTexts[3].setString(string_format("Left-Click Attraction: %.2f", attractVel));
}


void ConfigManager::toggleControls() {
	hide = !hide;
}


void ConfigManager::setVersion(int ver) {
	version = ver;
}


void ConfigManager::drawButtons(sf::RenderWindow& mWindow) {
	if (hide) return;
	for (int i = 0; i < NUM_VERSIONS; i++) {
		mWindow.draw(buttons[i]);
		mWindow.draw(buttonTexts[i]);
	}
}


void ConfigManager::checkVersionClicked(sf::RenderWindow& mWindow) {
	for (int i = 0; i < NUM_VERSIONS; i++) {
		if (mouseInRectangle(mWindow, buttons[i])) {
			version = versions[i];
			buttons[i].setFillColor(sf::Color(200, 200, 200));
			for (int j = 0; j < NUM_VERSIONS; j++) {
				if (j == i) continue;
				buttons[j].setFillColor(sf::Color(255, 255, 255));
			}
			break;
		}
	}
}