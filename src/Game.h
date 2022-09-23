#pragma once
#include "stdafx.h"
#include "wall.h"
#include "ray.h"

#include <cmath>
#include <set>

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	// game state
	bool drawing;

	// Mouse pos
	sf::Vector2i mousePos;
	int mosX;
	int mosY;

	// Game Object
	std::vector<std::vector<wall>> walls;
	struct edge {
		int x1, y1, x2, y2;
	};
	std::vector<ray> rays;
	std::vector<edge> edges;
	
	// font
	sf::Font font;
	sf::Text text;

	// init
	void initVariables();
	void initWindow();
	void initFont();

	// game action
	std::pair<int,int> calculateWallPos(sf::Vector2i pos);
	void drawWall();
	void clearWalls();

	// Edge and Ray calculation
	void generateEdges();
	void generateRays();
	void generate3Ray(int x, int y);

	// update game and display 
	void updateMousePositions();
	void updateText();
	void pollEvents();
	void displayWall();
	void displayRays();
	void displayLightArea();
	void clearObject();

	// game mode
	void drawing_mode();
	void lighting_mode();

public:
	Game();
	virtual ~Game();

	const bool isRunning() const;

	void update();
	void render();
};

