#pragma once
#include <SFML/Graphics.hpp>

/*
Must be divisible by 2:
CIRCLE_RADIUS
LINE_HEIGHT
*/

#define NUM_OPTIONS 4
#define CIRCLE_RADIUS 10

// Slider
#define STARTING_X 20
#define LINE_HEIGHT 6 
#define LINE_WIDTH 400
#define MARGIN_Y 60
#define BUTTON_HEIGHT 50
#define BUTTON_WIDTH 110
#define MARGIN_X 20

#define MOUSE_MASS_MAX 100000
#define MOUSE_MASS_MIN -2000

#define RANGE_MAX_D 600
#define RANGE_MIN_D 1

#define ATTRACT_VEL_MAX 50
#define ATTRACT_VEL_MIN 0

#define CONFIG_FONT_SIZE 24

#define NUM_VERSIONS 2
#define VER1 1
#define VER2 2


class ConfigManager
{
public:
	ConfigManager(sf::RenderWindow& mWindow);
	void drawSliders(sf::RenderWindow& mWindow);
	void drawButtons(sf::RenderWindow& mWindow);
	void handleMousePress(sf::RenderWindow& mWindow);
	void handleMouseRelease();
	void updateCirclePosition(sf::RenderWindow& mWindow);
	void toggleControls();
	void setVersion(int ver);
	int mouseMass;
	float minD, maxD;
	float attractVel;
	int version;
private:
	sf::RectangleShape lines[NUM_OPTIONS];
	sf::RectangleShape buttons[NUM_VERSIONS];
	sf::CircleShape circles[NUM_OPTIONS];
	sf::Text sliderTexts[NUM_OPTIONS];
	sf::Text buttonTexts[NUM_VERSIONS];
	int clickedCircle;
	float ratios[NUM_OPTIONS];
	bool show;
	void updateTexts();
};


