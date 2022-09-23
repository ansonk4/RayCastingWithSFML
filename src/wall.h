#include "stdafx.h"

class wall 
{

private:
	sf::RectangleShape rect;
	int top_edge;
	int bottom_edge;
	int left_edge;
	int right_edge;
	bool exist;

public:

	bool isExist();

	sf::RectangleShape getRect();
	int getEdge(std::string side);
	std::pair<int, int> getPos();

	void updateEdge(std::string side, int edge_id);
	void clearEdge();

	// constructor and destructor
	wall();
	wall(std::pair<int,int>);
	~wall();

};
