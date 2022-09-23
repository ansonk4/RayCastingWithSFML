#include "stdafx.h"

class ray 
{
public:
	float angle;
	float x;
	float y;

	bool operator< (const ray& r) const;
	void printRay();

	// constructor and destructor
	ray();
	ray(float ang, float X, float Y);
	virtual ~ray();
};


