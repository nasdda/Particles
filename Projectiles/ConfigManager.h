#pragma once
#include <SFML/Graphics.hpp>

/*
Must be divisible by 2:
CIRCLE_RADIUS
LINE_HEIGHT
*/


#define NUM_OPTIONS 3
#define CIRCLE_RADIUS 10

// Slider
#define LINE_HEIGHT 6 
#define LINE_WIDTH 300
#define MARGIN_Y 50

#define STARTING_X 20


class ConfigManager
{
public:
	ConfigManager(sf::RenderWindow& mWindow);
	void drawSliders(sf::RenderWindow& mWindow);
	void handleMousePress(sf::RenderWindow& mWindow);
	void handleMouseRelease();
	void updateCirclePosition(sf::RenderWindow& mWindow);
private:
	sf::RectangleShape lines[NUM_OPTIONS];
	sf::CircleShape circles[NUM_OPTIONS];
	int clickedCircle; // -1 if no circles clicked
};


