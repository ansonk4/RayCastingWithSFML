#include "Game.h"

// inti

void Game::initVariables()
{
	this->window = nullptr;
	this->drawing = true;
	std::vector<std::vector<wall>> temp (30, std::vector<wall> (30));
	this->walls = temp;
	this->font.loadFromFile("font/LEMONMILK-Light.otf");
}

void Game::initText(sf::Text& text, float x, float y)
{
	text.setFont(this->font);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Cyan);
	text.setPosition(sf::Vector2f(x,y));
}

void Game::initWindow()
{
	this->videoMode.height = window_height;
	this->videoMode.width = window_width;
	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

// game action

std::pair<int, int> Game::calculateWallPos(sf::Vector2i pos)
{
	int X = pos.x / 30, Y = pos.y / 30;
	if (X >= 29 || Y >= 29 || X < 1 || Y < 1) {
		return { -1,-1 };
	}
	return {X, Y};
}

void Game::drawWall()
{
	// draw wall if left mouse is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		std::pair<int, int> wallPos = calculateWallPos(this->mousePos);
		if (wallPos.first == -1) return;
		wall newWall(wallPos);
		this->walls[wallPos.second][wallPos.first] = newWall;
	}
}

void Game::clearWalls()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		for (auto& row : walls)
			for (auto& cell : row) {
				wall newWall;
				cell = newWall;
			}

}

// Edge and Ray calculation

void Game::generateEdges()
{
	// check and create top, bottom, left, right edges for all wall
	for (int i = 1; i < 29; i++) {
		for (int j = 1; j < 29; j++) {
			if (this->walls[i][j].isExist()) {
				auto pos = this->walls[i][j].getPos();
				int x = pos.first, y = pos.second;

				// if cell has no left neighbour, it need a left edge
				if (!this->walls[i][j - 1].isExist()) {
					// if top neighbour have no left edge, generate a new left edge
					if (this->walls[i - 1][j].getEdge("left") == -1) {
						edge newEdge = { x, y, x, y + 30 };
						edges.push_back(newEdge);
						walls[i][j].updateEdge("left", edges.size() - 1);
					}
					// else, extense the left edge of the top neighbour;
					else {
						int old_edge = this->walls[i - 1][j].getEdge("left");
						edges[old_edge].y2 += 30;
						walls[i][j].updateEdge("left", old_edge);
					}
				}

				// if cell has no right neighbour, it need a right edge
				if (!this->walls[i][j + 1].isExist()) {
					// if top neighbour have no right edge, generate a new right edge
					if (this->walls[i - 1][j].getEdge("right") == -1) {
						edge newEdge = { x + 30, y, x + 30, y + 30 };
						edges.push_back(newEdge);
						walls[i][j].updateEdge("right", edges.size() - 1);
					}
					// else, extense the right edge of the top neighbour;
					else {
						int old_edge = walls[i - 1][j].getEdge("right");
						edges[old_edge].y2 += 30;
						walls[i][j].updateEdge("right", old_edge);
					}
				}

				// if cell has no top neighbour, it need a top edge
				if (!this->walls[i - 1][j].isExist()) {
					// if left neighbour have no top edge, generate a new top edge
					if (this->walls[i][j - 1].getEdge("top") == -1) {
						edge newEdge = { x, y, x + 30, y };
						edges.push_back(newEdge);
						walls[i][j].updateEdge("top", edges.size() - 1);
					}
					// else, extense the left edge of the top neighbour;
					else {
						int old_edge = walls[i][j - 1].getEdge("top");
						edges[old_edge].x2 += 30;
						walls[i][j].updateEdge("top", old_edge);
					}
				}

				// if cell has no bottom neighbour, it need a bottom edge
				if (!this->walls[i + 1][j].isExist()) {
					// if left neighbour have no left edge or not exist, generate a new left edge
					if (this->walls[i][j - 1].getEdge("bottom") == -1) {
						edge newEdge = { x, y + 30, x + 30, y + 30 };
						edges.push_back(newEdge);
						walls[i][j].updateEdge("bottom", edges.size() - 1);
					}
					// else, extense the left edge of the top neighbour;
					else {
						int old_edge = walls[i][j - 1].getEdge("bottom");
						edges[old_edge].x2 += 30;
						walls[i][j].updateEdge("bottom", old_edge);
					}
				}
			}
		}
	}

	//generate 4 edge for 4 side of the window
	edge newEdge = { 0, 0, 0, 900 };
	edges.push_back(newEdge);
	newEdge = { 0, 0, 900, 0 };
	edges.push_back(newEdge);
	newEdge = { 900, 0, 900, 900 };
	edges.push_back(newEdge);
	newEdge = { 0, 900, 900, 900 };
	edges.push_back(newEdge);
}


void Game::generateRays()
{
	// generateRay for all edge
	for (const auto& edge : this->edges) {
		generate3Ray(edge.x1, edge.y1);
		generate3Ray(edge.x2, edge.y2);
	}

	// create a sorted and unique list of rays
	std::set<ray> exist;
	for (auto r: this->rays)
		exist.insert(r);
	this->rays.resize(0);
	for (auto r : exist)
		this->rays.push_back(r);
}

void Game::generate3Ray(int x, int y)
{

	double base_ang = atan2f((float) (y - this->mosY),(float) (x - this->mosX));

	// generate 3 ray for each point with small deviation to the angle to allow the ray go behind the wall.
	std::vector<double> diff = { 0.0, 0.001, -0.001 };
	for (auto d : diff) {
		double ang = base_ang + d;
		double dx = 900.f * cosf(ang), dy = 900.f * sinf(ang);

		double min_t1 = INT_MAX, min_x, min_y, min_ang;
		// find the closest intersection point of the ray and all the edges
		for (const auto& edge : this->edges) {
			// edge vector
			int edgeX = edge.x2 - edge.x1;
			int edgeY = edge.y2 - edge.y1;

			double distance2 = (dx * (edge.y1 - this->mosY) + (dy * (this->mosX - edge.x1))) / (edgeX * dy - edgeY * dx);
			double distance1 = (edge.x1 + edgeX * distance2 - this->mosX) / dx;
			if (distance1 > 0 && distance2 >= 0 && distance2 <= 1.0f) {
				if (distance1 < min_t1) {
					min_t1 = distance1;
					min_x = this->mosX + dx * distance1;
					min_y = this->mosY + dy * distance1;
					min_ang = atan2f(min_y - this->mosY, min_x - this->mosX);
				}
			}
		}
		if (min_t1 != INT_MAX) {
			ray newRay(min_ang, min_x, min_y);
			rays.push_back(newRay);
		}
	}
}

// update game and display 

void Game::updateMousePositions()
{
	this->mousePos = sf::Mouse::getPosition(*this->window);
	this->mosX = this->mousePos.x;
	this->mosY = this->mousePos.y;
}

void Game::updateText()
{
	std::stringstream ss;	
	if (drawing) 
		ss << "Drawing Mode\n" << "Press 2 to switch mode\n" << "Press 3 to clear";
	else
		ss << "Lighting Mode\n" << "Press 1 to switch mode\n" << "Press 3 to clear\n" << "Hold Q to show light rays\n";
		
	this->leftText.setString(ss.str());

	ss.str(std::string());
	ss << "Rays Count: ";
	if (drawing) 
		ss << 0;
	else 
		ss << this->rays.size() + 1;
	ss << "\nWalls Count: ";
	
	int wallCount = 0;
	for (auto row : walls)
		for (auto cell : row)
			if (cell.isExist())
				wallCount++;
	ss << wallCount;
	this->rightText.setString(ss.str());
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::displayWall()
{
	for (const auto& row : this->walls) {
		for (auto cell : row) {
			if (cell.isExist())
				this->window->draw(cell.getRect());
		}
	}
}

void Game::displayRays()
{
	for (const auto& ray : rays) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f((float)this->mosX, (float)this->mosY)),
			sf::Vertex(sf::Vector2f(ray.x, ray.y))
		};
		this->window->draw(line, 2, sf::Lines);
	}
}

void Game::displayLightArea()
{
	if (rays.size() == 0)
		return;

	for (size_t i = 0; i < rays.size()-1; i++) {
		sf::ConvexShape lightArea;
		lightArea.setPointCount(3);
		// generate light area - a triangle with 3 points: mouse, ray[i] and ray[i+1]
		lightArea.setPoint(0, sf::Vector2f((float)mosX, (float)mosY));
		lightArea.setPoint(1, sf::Vector2f((float)rays[i].x, (float)rays[i].y));
		lightArea.setPoint(2, sf::Vector2f((float)rays[i + 1].x, (float)rays[i + 1].y));
		this->window->draw(lightArea);
	}

	// generate light area between ray[0] and ray[i]
	sf::ConvexShape lightArea;
	lightArea.setPointCount(3);
	lightArea.setPoint(0, sf::Vector2f((float)mosX, (float)mosY));
	lightArea.setPoint(1, sf::Vector2f(rays[rays.size()-1].x, rays[rays.size()-1].y));
	lightArea.setPoint(2, sf::Vector2f(rays[0].x, rays[0].y));
	this->window->draw(lightArea);
}

void Game::clearObject() 
{
	this->edges.clear();
	for (auto& row : this->walls) {
		for (auto& cell : row) {
			cell.clearEdge();
		}
	}
	this->rays.clear();
}
 
// game mode

void Game::drawing_mode()
{
	this->drawWall();
	this->generateEdges();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		this->drawing = false;
}

void Game::lighting_mode()
{
	this->generateEdges();
	this->generateRays();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		this->drawing = true;
}

// public function

Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initText(this->leftText, 0, 0);
	this->initText(this->rightText, 705, 0);
}

Game::~Game()
{
	delete this->window;
}

const bool Game::isRunning() const
{
	return this->window->isOpen();
}

void Game::update()
{
	this->pollEvents();
	this->updateMousePositions();
	this->updateText();
	this->clearObject();
	if (this->drawing) 
		this->drawing_mode();
	else 
		this->lighting_mode();
	this->clearWalls();
}

void Game::render()
{
	this->window->clear();
	this->displayWall();
	this->displayRays();
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		this->displayLightArea();
	this->window->draw(this->leftText);
	this->window->draw(this->rightText);
	this->window->display();
}


