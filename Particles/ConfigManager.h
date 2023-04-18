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
#define STARTING_X 20
#define LINE_HEIGHT 6 
#define LINE_WIDTH 400
#define MARGIN_Y 60

#define MOUSE_MASS_MAX 100000
#define MOUSE_MASS_MIN -2000

#define RANGE_MAX_D 600
#define RANGE_MIN_D 1

#define CONFIG_FONT_SIZE 24


class ConfigManager
{
public:
	ConfigManager(sf::RenderWindow& mWindow);
	void drawSliders(sf::RenderWindow& mWindow);
	void handleMousePress(sf::RenderWindow& mWindow);
	void handleMouseRelease();
	void updateCirclePosition(sf::RenderWindow& mWindow);
	int mouseMass;
	float minD, maxD;
private:
	sf::RectangleShape lines[NUM_OPTIONS];
	sf::CircleShape circles[NUM_OPTIONS];
	int clickedCircle; // -1 if no circles clicked
	sf::Text texts[NUM_OPTIONS];
	float ratios[NUM_OPTIONS];
};


