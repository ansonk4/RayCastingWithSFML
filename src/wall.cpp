#include "wall.h"

bool wall::isExist() 
{
	return this->exist;
}

sf::RectangleShape wall::getRect() 
{
	return this->rect;
}

int wall::getEdge(std::string side) 
{
	if (side == "top")
		return this->top_edge;
	else if (side == "bottom")
		return this->bottom_edge;
	else if (side == "left")
		return this->left_edge;
	else if (side == "right")
		return this->right_edge;
}

std::pair<int, int> wall::getPos()
{
	return { this->rect.getPosition().x, this->rect.getPosition().y };
}

void wall::updateEdge(std::string side, int edge_id) 
{
	if (side == "top")
		this->top_edge = edge_id;
	else if (side == "bottom")
		this->bottom_edge = edge_id;
	else if (side == "left")
		this->left_edge = edge_id;
	else if (side == "right")
		this->right_edge = edge_id;
}

void wall::clearEdge() 
{
	this->top_edge = -1;
	this->bottom_edge = -1;
	this->left_edge = -1;
	this->right_edge = -1;
}

wall::wall() 
{
	sf::RectangleShape temp;
	this->rect = temp;
	this->top_edge = -1;
	this->bottom_edge = -1;
	this->left_edge = -1;
	this->right_edge = -1;
	this->exist = false;
}

wall::wall(std::pair<int,int> wallPos) 
{
	sf::RectangleShape temp;
	temp.setPosition(sf::Vector2f(wallPos.first * 30, wallPos.second * 30));
	temp.setSize(sf::Vector2f(30.f, 30.f));
	temp.setFillColor(sf::Color::Cyan);

	this->rect = temp;
	this->top_edge = -1;
	this->bottom_edge = -1;
	this->left_edge = -1;
	this->right_edge = -1;
	this->exist = true;
}

wall::~wall()
{
	
}


