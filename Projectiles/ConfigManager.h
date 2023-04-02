#pragma once
#include <SFML/Graphics.hpp>

#define NUM_OPTIONS 3
#define CIRCLE_RADIUS 10


class ConfigManager
{
public:
	ConfigManager(sf::RenderWindow& mWindow);
	void drawSliders(sf::RenderWindow& mWindow);
	void handleMouseDown(sf::RenderWindow& mWindow);
private:
	sf::RectangleShape lines[NUM_OPTIONS];
	sf::CircleShape circles[NUM_OPTIONS];
};


